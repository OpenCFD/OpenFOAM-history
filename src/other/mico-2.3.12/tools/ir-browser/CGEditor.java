/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997 Kay Roemer & Arno Puder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.applet.Applet;

import AlertBox;
import EntryBox;
import CanvasObject;
import CanvasBoxText;
import CanvasArrow;
import MyCanvas;
import ConceptualGraph;
import CGParser;
import View;
import DIIFrame;

class CGArrow extends CanvasArrow {
public CGArrow (int x0, int y0, int x1, int y1)
    {
        super (x0, y0, x1, y1, Color.black);
    }
}


class CGNode extends CanvasBoxText {
private CGNode parent;
private CGArrow arrow;
private Vector childs;
private Vector arrows;
private boolean isconcept;
public int layout_y;

public CGNode (int x, int y, String text, FontMetrics fm, boolean iscon)
    {
	super (x, y, text, fm,
	       iscon ? Color.black : Color.white,
	       iscon ? Color.white : Color.black,
	       iscon ? Color.black : Color.white);
        super.Move (-Bbox().width/2, -Bbox().height/2);
        parent = null;
        childs = new Vector();
        arrows = new Vector();
        arrow = null;
	isconcept = iscon;
	layout_y = 0;
    }

public void Move (int xinc, int yinc)
    {
        AutoUpdateOff ();
        super.Move (xinc, yinc);
        for (int i = arrows.size()-1; i >= 0; --i) {
            ((CGArrow)arrows.elementAt(i)).MoveTail (xinc, yinc);
        }
        if (arrow != null)
            arrow.MoveHead (xinc, yinc);
        AutoUpdateOn ();
    }

public void MoveTo (int x, int y)
    {
        Point p = GetPos ();
	Move (x - p.x, y - p.y);
    }

public void AddChild (CGNode n, CGArrow a)
    {
        childs.addElement (n);
        arrows.addElement (a);
        Rectangle pos = Bbox();
        a.MoveTailTo (pos.x + pos.width - 1, pos.y + pos.height/2);
    }

public void DelChild (CGNode n)
        throws NoSuchElementException
    {
        int idx = childs.indexOf (n);
        if (idx == -1)
            throw new NoSuchElementException ();
        else {
            childs.removeElementAt (idx);
            arrows.removeElementAt (idx);
        }
    }
    
public void SetParent (CGNode n, CGArrow a)
    {
        parent = n;
        arrow = a;
        if (arrow != null) {
            Rectangle pos = Bbox();
            arrow.MoveHeadTo (pos.x, pos.y + pos.height/2);
        }
    }

public CGNode GetParent ()
    {
	return parent;
    }

public CGArrow GetParentArrow ()
    {
	return arrow;
    }

private String Quote (String s)
    {
	return s.length() > 0 ? s : "''";
    }

public void SetText (String text)
    {
        int width = Bbox().width;

        AutoUpdateOff ();
        super.SetText (text);
        width = Bbox().width - width;
        for (int i = arrows.size()-1; i >= 0; --i)
            ((CGArrow)arrows.elementAt(i)).MoveTail (width, 0);
        AutoUpdateOn ();
    }

public String GetType ()
    {
	return GetType (false);
    }

public String GetType (boolean removequotes)
    {
	String t = GetText ();
	int idx = t.indexOf (':');
	if (idx >= 0)
	    t = t.substring (0, idx);
	if (removequotes) {
	    t = t.trim();
	    if (t.charAt (0) == '"' || t.charAt (0) == '\'')
		t = t.substring (1, t.length()-1);
	}
	return t;
    }

public void SetType (String t)
    {
	String inst = GetInst ();
	if (inst != null)
	    SetText (Quote (t) + ":" + inst);
	else
	    SetText (Quote (t));
    }

public String GetInst ()
    {
	String t = GetText ();
	int idx = t.indexOf (':');
	if (idx < 0)
	    return null;
	return t.substring (idx+1);
    }

public void SetInst (String t)
    {
	if (IsConcept()) {
	    SetText (GetType() + ":" + Quote (t));
	}
    }

public int NumChilds ()
    {
	return childs.size();
    }

public CGNode ChildAt (int idx)
        throws ArrayIndexOutOfBoundsException
    {
	return (CGNode)childs.elementAt (idx);
    }

public CGArrow ArrowAt (int idx)
        throws ArrayIndexOutOfBoundsException
    {
	return (CGArrow)arrows.elementAt (idx);
    }

public boolean IsConcept ()
    {
	return isconcept;
    }

public void SortChilds ()
    {
	for (int i = childs.size(); i >= 2; --i) {
	    for (int j = 0; j < i-1; ++j) {
		Point p1 = ((CGNode)childs.elementAt(j)).GetPos();
		Point p2 = ((CGNode)childs.elementAt(j+1)).GetPos();
		if (p1.y > p2.y) {
		    Object tmp = childs.elementAt (j);
		    childs.setElementAt (childs.elementAt (j+1), j);
		    childs.setElementAt (tmp, j+1);

		    tmp = arrows.elementAt (j);
		    arrows.setElementAt (arrows.elementAt (j+1), j);
		    arrows.setElementAt (tmp, j+1);
		}
	    }
	}
    }
}


public class CGEditor extends MyCanvas
    implements EntryBoxHandler, ActionListener, MouseListener,
               MouseMotionListener, FocusListener, KeyListener {
private DIIFrame frame;
private CGNode topnode;

private CGNode curnode;
private CGArrow curarrow;
private int dragx, dragy;

private boolean readonly;

private Vector viewers;
private Hashtable menus, menumap;

private PopupMenu popup;

private final static int stateIdle = 0;
private final static int stateDrag = 1;
private final static int stateCreate = 2;
private final static int stateEdit = 3;
private int state;

private final int layoutXDist = 20;
private final int layoutYDist = 20;


public CGEditor (Dimension prefsize, boolean editable, DIIFrame f)
    {
	super (prefsize);
	frame = f;
	readonly = !editable;
        state = stateIdle;
	topnode = null;
	curnode = null;
	curarrow = null;
        viewers = new Vector();
        menus = new Hashtable();
        menumap = new Hashtable();
	popup = null;
	this.addMouseListener( this );
	this.addMouseMotionListener( this );
	this.addFocusListener( this );
	this.addKeyListener( this );
    }

public void focusGained( FocusEvent e )
    {
    }

public void focusLost( FocusEvent e )
    {
      if (readonly)
	return;
      ShowStatus (-1, -1);
    }

public void keyPressed( KeyEvent e )
    {
      if (readonly)
	return;
      if (e.getKeyChar() == '\033')
	Abort ();
      else if (e.getKeyChar() == 'l')
	Layout (null);
    }

public void keyReleased( KeyEvent e )
    {
    }

public void keyTyped( KeyEvent e )
    {
    }

public void mouseClicked (MouseEvent e)
    {
    }

public void mouseEntered (MouseEvent e)
    {
    }

public void mouseExited (MouseEvent e)
    {
    }

public void mousePressed (MouseEvent e)
    {
      if (readonly)
	return;
      Pick( e );
    }

public void mouseReleased (MouseEvent e)
    {
      if (readonly)
	return;
      Drop( e );
    }

public void mouseDragged (MouseEvent e)
    {
      if (readonly)
	return;
      Drag( e );
    }

public void mouseMoved (MouseEvent e)
    {
      if (readonly)
	return;
      ShowStatus( e.getX(), e.getY() );
    }

public void actionPerformed( ActionEvent e )
    {
        String s = e.getActionCommand();
        if (s == null)
            return;

	String menuname = (String)menumap.get (curnode.GetType(true));
	Hashtable templates = (Hashtable)menus.get (menuname);

        ConceptualGraph cg = (ConceptualGraph)templates.get(s);
        AutoUpdateOff ();
        CGNode top = ConvertInput (cg);
        CGArrow arrow = new CGArrow (0, 0, 0, 0);
        Add (arrow);
        ToBottom (arrow);
        curnode.AddChild (top, arrow);
        top.SetParent (curnode, arrow);
        Layout (curnode);
        AutoUpdateOn ();

        NotifyViewers ();
  }

private void ShowStatus (int x, int y)
    {
        String s = "";
        CGNode node = null;

        if (x >= 0 && y >= 0) {
            try {
                CanvasObject co = super.Find (x, y);
                if (co instanceof CGNode) {
                    node = (CGNode)co;
                }
            } catch (NoSuchElementException ex) {}

            if (node == null) {
                if (topnode == null)
                    s = "Button1: create";
            } else {
		s = "Button1: drag";
                if (node.IsConcept()) {
		    if (node.GetInst() != null)
			s += ", Shift-Button1: edit";
		    if (node == topnode)
			s += ", Shift-Ctrl-Button1: remove";
                    if (menumap.containsKey (node.GetType (true)))
                        s += ", Button3: menu";
                } else {
                    s += ", Shift-Ctrl-Button1: remove";
                }
            }
        }
	frame.status.setStatus (s);
    }

private void MenuPopup (MouseEvent ev, CGNode node)
    {
	if (!node.IsConcept())
	    return;

	String menuname = (String)menumap.get (node.GetType(true));
	if (menuname == null)
	    return;
	Hashtable templates = (Hashtable)menus.get (menuname);
	if (templates == null)
	    return;

	if( popup != null )
	  remove( popup );

	popup = new PopupMenu( menuname );
	
        Enumeration e = templates.keys();
        while (e.hasMoreElements()) {
            String key = (String)e.nextElement();
	    MenuItem mi = new MenuItem( key );
	    mi.setActionCommand( key );
	    mi.addActionListener( this );
	    popup.add( mi );
        }
        curnode = node;

	this.add( popup );

	popup.show( this, ev.getX(), ev.getY() );
    }

private void Pick (MouseEvent ev)
    {
	if (state != stateIdle)
	    return;

	CGNode node = null;
	try {
	    CanvasObject co = super.Find (ev.getX(), ev.getY());
	    if (co instanceof CGNode) {
		node = (CGNode)co;
	    }
	} catch (NoSuchElementException ex) {}

        if (node != null && node.IsConcept() &&
            (ev.getModifiers() & Event.META_MASK) != 0) {
            // template menu

            MenuPopup (ev, node);
        } else {
            switch (ev.getModifiers() & (Event.SHIFT_MASK | Event.CTRL_MASK)) {
            case Event.SHIFT_MASK:
                // edit
                if (node != null) {
		    String s = node.GetInst();
		    if (s != null) {
			curnode = node;
			Edit (s);
		    }
                }
                break;

            case Event.CTRL_MASK:
                // drag
                if (node != null) {
                    state = stateDrag;
                    ToTop (node);
                    curnode = node;
                    dragx = ev.getX();
                    dragy = ev.getY();
                }
                break;

            case Event.SHIFT_MASK|Event.CTRL_MASK:
                // delete subtree
                if (node != null) {
		    // ZZZ only allowed for relations or topnode
		    if (node.IsConcept() && node != topnode)
			break;
                    CGNode parent = node.GetParent ();
                    if (parent == null || parent.IsConcept() ||
                        parent.NumChilds() > 1) {
                        
                        AutoUpdateOff ();
                        if (parent != null) {
                            parent.DelChild (node);
                            Remove (node.GetParentArrow());
                        }
                        RemoveFromCanvas (node);
                        AutoUpdateOn ();
                    }
                    if (node == topnode)
                        topnode = null;
                    NotifyViewers();
                }
                break;

            default:
                if (node == null && topnode == null) {
		    // create topnode
                    topnode = new CGNode (ev.getX(), ev.getY(),
					  "OPERATION:deposit",
                                          getFontMetrics (getFont()),
                                          true);
                    Add (topnode);
                    NotifyViewers ();
                } else if (node != null) {
		    // ZZZ drag
                    state = stateDrag;
                    ToTop (node);
                    curnode = node;
                    dragx = ev.getX();
                    dragy = ev.getY();
		}
		/*
		 * ZZZ creating subnodes not allowed
		else if (node != null && topnode != null) {
                    state = stateCreate;
                    curnode = node;
                    
                    Rectangle pos = node.Bbox();
                    curarrow = new CGArrow (pos.x+pos.width-1,
                                            pos.y+pos.height/2,
                                            ev.x, ev.y);
                    Add (curarrow);
                }
		*/
                break;
            }
        }
    }

private void Drag (MouseEvent ev)
    {
	switch (state) {
	case stateDrag:
            curnode.Move (ev.getX() - dragx, ev.getY() - dragy);
            dragx = ev.getX();
            dragy = ev.getY();
	    break;

	case stateCreate:
	    curarrow.MoveHeadTo (ev.getX(), ev.getY());
	    break;
        }
    }

private void Drop (MouseEvent ev)
    {
	switch (state) {
	case stateDrag:
	    state = stateIdle;
	    CheckResize ();
	    break;

	case stateCreate:
	    CGNode node = new CGNode (ev.getX(), ev.getY(), "something",
				      getFontMetrics (getFont()),
				      !curnode.IsConcept());
            curnode.AddChild (node, curarrow);
            node.SetParent (curnode, curarrow);
            ToBottom (curarrow);
            Add (node);

	    if (!node.IsConcept()) {
		int x = ev.getX() + node.Bbox().width + layoutXDist;
		CGNode node2 = new CGNode (x, ev.getY(), "something",
					   getFontMetrics (getFont()),
					   true);
		CGArrow arrow2 = new CGArrow (0, 0, 0, 0);
		node.AddChild (node2, arrow2);
		node2.SetParent (node, arrow2);
		Add (node2);
		Add (arrow2);
                ToBottom (arrow2);
	    }
            state = stateIdle;
            NotifyViewers();
            break;
	}
        ShowStatus (ev.getX(), ev.getY());
    }

private void Abort ()
    {
	switch (state) {
	case stateCreate:
	    Remove (curarrow);
	    state = stateIdle;
	    break;
	}
        state = stateIdle;
    }

public void entryBoxHandler (EntryBox e, Object arg)
    {
	if (state != stateEdit)
	    return;
	state = stateIdle;
	if (e.GetButton() == 0) {
	    curnode.SetInst (e.GetText());
	    NotifyViewers();
	}
    }

protected void Edit (String txt)
    {
	state = stateEdit;
        EntryBox editor = new EntryBox ("Edit Node", "Enter node:", txt,
					this, null, frame);
        editor.pack();
        editor.show();
    }

private void RemoveFromCanvas (CGNode root)
    {
	super.Remove (root);
	for (int i = root.NumChilds()-1; i >= 0; --i) {
	    super.Remove (root.ArrowAt(i));
	    RemoveFromCanvas (root.ChildAt(i));
	}
    }

private int LayoutPass1 (CGNode root, int y, Vector vdist, int level)
    {
	Rectangle bbox = root.Bbox();
	root.layout_y = y;
	root.SortChilds ();

	if (level >= vdist.size()) {
	    vdist.addElement (new Integer (bbox.width));
	} else {
	    int width = ((Integer)vdist.elementAt(level)).intValue();
	    if (width < bbox.width) {
		vdist.setElementAt (new Integer (bbox.width), level);
	    }
	}
	int nchilds = root.NumChilds(), y2 = y;
	for (int i = 0; i < nchilds; ++i) {
	    y2 = LayoutPass1 (root.ChildAt(i), y2, vdist, level+1);
	}
	return Math.max (y + bbox.height + layoutYDist, y2);
    }

private void LayoutPass2 (CGNode root, int x, Vector vdist, int level)
    {
	Rectangle bbox = root.Bbox();
	int width = ((Integer)vdist.elementAt(level)).intValue();
	root.MoveTo (x + (width - bbox.width)/2, root.layout_y);

	int nchilds = root.NumChilds();
	for (int i = 0; i < nchilds; ++i) {
	    LayoutPass2 (root.ChildAt(i), x+width+layoutXDist, vdist, level+1);
	}
    }

public void Layout (CGNode root)
    {
	int x, y;
	if (root == null) {
            if ((root = topnode) == null)
                return;
	    x = layoutXDist;
	    y = layoutYDist;
	} else {
	    x = root.Bbox().x;
	    y = root.Bbox().y;
	}
	AutoUpdateOff ();
	Vector vdist = new Vector();
	LayoutPass1 (root, y, vdist, 0);
	LayoutPass2 (root, x, vdist, 0);
	CheckResize ();
	AutoUpdateOn ();
    }

private CGNode ConvertInput (ConceptualGraph cg)
    {
        StringBuffer ostr = new StringBuffer();
        cg.getNode().print (ostr);
        String s = ostr.toString();
        s = s.substring (1, s.length()-1);

        CGNode node = new CGNode (0, 0, s, getFontMetrics(getFont()),
                                  cg.getNode() instanceof ConceptNode);
        Add (node);

        ConceptualGraph cg2;
        for (cg2 = cg.getWidth(); cg2 != null; cg2 = cg2.getDepth()) {
            CGNode node2 = ConvertInput (cg2);
            CGArrow arrow2 = new CGArrow (0, 0, 0, 0);
            Add (arrow2);
            ToBottom (arrow2);

            node.AddChild (node2, arrow2);
            node2.SetParent (node, arrow2);
        }
        return node;
    }

private void ConvertOutput (StringBuffer ostr, CGNode node)
    {
	node.SortChilds ();
        String s = node.GetText();

        if (node.IsConcept()) {
            ostr.append ("[" + s + "]");
        } else {
            ostr.append ("(" + s + ")");
        }

        int nchilds = node.NumChilds();
        if (nchilds == 1) {
            ostr.append ("->");
            ConvertOutput (ostr, node.ChildAt (0));
        } else if (nchilds > 1) {
            ostr.append ("-");
            for (int i = 0; i < nchilds; ++i) {
                ostr.append ("->");
                ConvertOutput (ostr, node.ChildAt (i));
                if (i+1 != nchilds)
                    ostr.append (",");
            }
            ostr.append (".");
        }
    }

public void SetCG (ConceptualGraph cg)
    {
	AutoUpdateOff ();
	if (topnode != null) {
	    RemoveFromCanvas (topnode);
	    topnode = null;
	}
        if (cg != null) {
            topnode = ConvertInput (cg);
            Layout (null);
        }
	AutoUpdateOn ();
        Abort();
        NotifyViewers();
    }

public ConceptualGraph GetCG ()
    {
        if (topnode == null)
            return null;

        StringBuffer ostr = new StringBuffer();
        ConvertOutput (ostr, topnode);

        CGParser p = new CGParser ();
        try {
            return p.Parse (ostr.toString());
        } catch (Exception e) {
            AlertBox a = new AlertBox ("Parse Error", "Cannot parse graph");
            a.pack();
            a.show();
            return null;
        }
    }

public void AddView (View v)
    {
        viewers.addElement (v);
    }

public void DelView (View v)
    {
        viewers.removeElement (v);
    }

private void NotifyViewers ()
    {
        for (int i = 0; i < viewers.size(); ++i)
            ((View)viewers.elementAt(i)).UpdateView (this);
    }

public void AddTemplate (String menuname, Vector after,
			 String entryname, ConceptualGraph cg)
    {
	Hashtable menu = (Hashtable)menus.get (menuname);
	if (menu == null) {
	    menu = new Hashtable ();
	    menus.put (menuname, menu);
	}
	menu.put (entryname, cg);
	for (int i = 0; i < after.size(); ++i) {
	    String concept = (String)after.elementAt (i);
	    if (!menumap.containsKey (concept))
		menumap.put (concept, menuname);
	}
    }

public void DelTemplate (String name)
        throws NoSuchElementException
    {
	menus.remove (name);
	Enumeration keys = menumap.keys ();
	Enumeration elms = menumap.elements ();
	while (keys.hasMoreElements() && elms.hasMoreElements()) {
	    String key = (String)keys.nextElement();
	    String elm = (String)elms.nextElement();
	    if (elm.equals (name))
		menumap.remove (key);
	}
    }
}
