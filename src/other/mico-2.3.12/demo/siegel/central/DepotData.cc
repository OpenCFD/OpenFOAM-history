/*

Copyright (C) 1998, 2000 by the Object Management Group, Inc.  All  
rights reserved.  

Permission to use, copy, modify, and distribute this software for
any teaching or instructional purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that copyright notice and this permission
notice appear in supporting documentation, and that the names of
Object Management Group, Inc. (OMG) and John Wiley and Sons, Inc.
(Wiley) not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

THIS SOFTWARE IS SOLD AS IS WITHOUT WARRANTY OF ANY KIND, EITHER
EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  Neither OMG, Wiley, nor their dealers or distributors
assume any liability of any alleged or actual damages arising
from the use of or the inability to use this software.  (Some
states do not allow the exclusion of implied warranties, so the
exclusion may not apply to you).

*/

#include "DepotData.h"
#include "TreeNode.h"

unsigned long DepotData::locate(const char* bc,AStore::ItemInfo &i) {
    TreeNode *t;
    unsigned long status = _locateNode(bc,t);
    if (status)
        i=t->m_item;
    return status;
}

void DepotData::insert(const AStore::ItemInfo &i) {
    TreeNode *temp;
    const char *s = i.item;
    if (!_locateNode(s,temp)) {
        TreeNode *new_node=new TreeNode;
        new_node->m_item=i;
        if (m_root==0) {
            m_root=new_node;
            return;
        }
        if (strcmp(i.item,temp->m_item.item)<0)
            temp->m_left=new_node;
        else
            temp->m_right=new_node;
    }
    else {
        temp->m_item=i;
    }
}

unsigned long DepotData::_locateNode(const char* bc, TreeNode * &ret_node) {
    int compare;
    ret_node=m_root;
    if (ret_node==0) return 0;

    compare=strcmp(bc,ret_node->m_item.item);
    while (compare!=0) {
        if (compare<0) {
            if (ret_node->m_left==0) {
                return 0;
            }
            else
                ret_node=ret_node->m_left;
        }
        else {
            if (ret_node->m_right==0)
            {
                return 0;
            }
            else
                ret_node=ret_node->m_right;
        }

        compare=strcmp(bc,ret_node->m_item.item);
    }
    return 1;
}
