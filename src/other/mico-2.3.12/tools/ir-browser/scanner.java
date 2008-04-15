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

import java_cup.runtime.*;
import java.io.*;
import sym;

public class scanner {
  /* single lookahead character */
  protected static int next_char;
  private static StringReader bufstr;

  /* advance input by one character */
  protected static void advance() throws java.io.IOException 
    {
        next_char = bufstr.read();
        /*
         * fix for HotJava
         */
        if (next_char != -1)
            next_char &= 255;
    }

  /* initialize the scanner */
  public static void init(String toBeParsed) throws java.io.IOException 
    { 
       bufstr= new StringReader (toBeParsed);
       advance();
    }

  private static void eat_comment ()
        throws IOException
    {
        while (true) {
            while (next_char != '/' && next_char != '*' && next_char != -1)
                advance();
            switch (next_char) {
            case -1:
                return;

            case '*':
                advance();
                if (next_char == '/') {
                    advance();
                    return;
                }
                break;

            case '/':
                advance();
                if (next_char == '*') {
                    advance();
                    eat_comment();
                }
                break;
            }
        }
    }

  /* recognize and return the next complete token */
  public static Symbol next_token() throws java.io.IOException
    {
      int pre_char;
      String str = new String();
      StringBuffer strbuf = new StringBuffer();

      for (;;) {
          if (next_char == '/') {
              advance();
              if (next_char != '*')
                  return new Symbol(sym.TOKEN_UNDEF);
              advance();
              eat_comment();
          }
          switch (next_char){

          case ' ': case '\t':case '\n':
            advance();		
            continue;

          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9': 
	        /* parse a decimal integer */
	        int i_val = 0;
	        do {
	          i_val = i_val * 10 + (next_char - '0');
	          advance();
	        } while (next_char >= '0' && next_char <= '9');
	        return new Symbol(sym.VALUE, new Integer( i_val ) );

          case ',':  
            advance();	
            return new Symbol(sym.COMMA);

	  case '+':
	    advance();
	    if (next_char >= '0' && next_char <= '9')
		return next_token ();
            return new Symbol(sym.TOKEN_UNDEF);
	    
          case '-': 
            advance();
	    if (next_char >= '0' && next_char <= '9') {
		Symbol t = next_token();
		t.value = new Integer( 0 - ((Integer) t.value).intValue() );
		return t;
	    } else if ( next_char == '>' ){
              advance();
              return new Symbol(sym.CONNECTS);
            }
            return new Symbol(sym.SUBGRAPH_OPEN);

          case '.':  
            advance();	
            return new Symbol(sym.SUBGRAPH_CLOSE);

          case '(':  
            advance();	
            return new Symbol(sym.RELATION_OPEN);

          case ')':  
            advance();	
            return new Symbol(sym.RELATION_CLOSE);

          case '[':  
            advance();	
            return new Symbol(sym.CONCEPT_OPEN);

          case ']':  
            advance();	
            return new Symbol(sym.CONCEPT_CLOSE);

          case ':':  
            advance();	
            return new Symbol(sym.COLON);

          case '=':  
            advance();	
            return new Symbol(sym.EQUAL);
          
          case '§':
            advance();
	        strbuf = new StringBuffer();
            do {
                strbuf.append((char)next_char);
                advance();
            } while ( next_char != '§');

            advance();
            return new Symbol(sym.WORD, strbuf.toString());

          case '\'':
          case '"':
            int delim = pre_char = next_char;
            advance();
            strbuf = new StringBuffer();
            while (!(next_char == delim && pre_char != '\\')) {
                strbuf.append((char)next_char);
                pre_char = next_char;
                advance();
            }

            advance();
            return new Symbol(sym.WORD, strbuf.toString());


          case -1: 	
            return new Symbol(sym.EOF);


          case '@': 
	        advance(); 
            strbuf = new StringBuffer();
            do {
              strbuf.append((char)next_char);
              advance();
            } while ( next_char == '-' ||
                     (next_char >= 'a' && next_char <= 'z') );

            str = new String(strbuf.toString());

            if ( str.compareTo("error") == 0 ){
              return new Symbol(sym.ERROR);
            }
            if ( str.compareTo("answer-ok") == 0 ){
              return new Symbol(sym.ANSWER_OK);
            }
            if ( str.compareTo("yes") == 0 ){
              return new Symbol(sym.ANSWER_YES);
            }
            if ( str.compareTo("no") == 0 ){
              return new Symbol(sym.ANSWER_NO);
            }
            if ( str.compareTo("answer-graph") == 0 ){
              return new Symbol(sym.ANSWER_GRAPH);
            }
            if ( str.compareTo("answer-flag") == 0 ){
              return new Symbol(sym.ANSWER_FLAG);
            }
            if ( str.compareTo("answer-string") == 0 ){
              return new Symbol(sym.ANSWER_STR);
            }
            if ( str.compareTo("answer-key") == 0 ){
              return new Symbol(sym.ANSWER_KEY);
            }
            else{
              return new Symbol(sym.TOKEN_UNDEF);
            }

          case 'a': case 'b': case 'c': case 'd': case 'e': 
          case 'f': case 'g': case 'h': case 'i': case 'j': 
          case 'k': case 'l': case 'm': case 'n': case 'o': 
          case 'p': case 'q': case 'r': case 's': case 't': 
          case 'u': case 'v': case 'w': case 'x': case 'y': 
          case 'z': 
          case 'A': case 'B': case 'C': case 'D': case 'E': 
          case 'F': case 'G': case 'H': case 'I': case 'J': 
          case 'K': case 'L': case 'M': case 'N': case 'O': 
          case 'P': case 'Q': case 'R': case 'S': case 'T': 
          case 'U': case 'V': case 'W': case 'X': case 'Y': 
          case 'Z': 
            strbuf = new StringBuffer();
            do {
                strbuf.append((char)next_char);
                advance();
            } while ( (next_char >= 'A' && next_char <= 'Z') || 
                      (next_char >= 'a' && next_char <= 'z') || 
                      (next_char >= '0' && next_char <= '9') || 
                       next_char == '-' || next_char == '_');

            return new Symbol(sym.WORD, strbuf.toString());


          default:
            return new Symbol(sym.TOKEN_UNDEF);

	  }
      }
    }
};
