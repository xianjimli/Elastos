//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <ElUtilities.h>


EXTERN_C int __stdcall _DefaultMultiTreeNodeComparison(
                                MultiTreeNode* pN1, MultiTreeNode* pN2)
{
    return (int)((ADDRESS)pN1) - ((ADDRESS)pN2);
}

EXTERN_C MultiTreeNode* __stdcall MultiTree_FindAdjacentNode(MultiTree * pThis,
            MultiTreeNode* pTreeNode, MultiTreeNode* pNewNode, int* pFlag)
{
    int ret = 0;
    while (pTreeNode != NULL) { // find the adjacent node of pNewNode
        ret = (*m_fnCompare)(pTreeNode, pNewNode);
        if (ret == 0)           // a node with same value exists
            break;

        MultiTreeNode* pNode;
        if (ret > 0)            // pNewNode smaller, walk left
            pNode = (MultiTreeNode*)((QuickListNode*)pTreeNode)->Left();
        else                    // pNewNode bigger, walk right
            pNode = (MultiTreeNode*)((QuickListNode*)pTreeNode)->Right();

        if (pNode == NULL)      // we are at the bottom of the tree
            break;

        pTreeNode = pNode;
    }

    *pFlag = ret;
    return pTreeNode;
}

// Insert a child in the right place among all children which
// is a sorted link-list.  If the to be added child already exist in
// the list, remove and return the existing child.  Otherwise, return NULL.
//
EXTERN_C MultiTreeNode* __stdcall MultiTree_AddChild(
                                        MultiTree * pThis,MultiTreeNode* pChild)
{
    pChild->m_pParent = pThis;
    if (m_pChildren == NULL) {  // there is no children
        m_pChildren = pChild;
        return NULL;
    }

    int flag;                   // 0: replace, >0: left, <0: right
    MultiTreeNode* pTargetNode = FindAdjacentNode(m_pChildren, pChild, &flag);
    if (flag >= 0) {
        ((QuickListNode*)pTargetNode)->AddLeft((QuickListNode*)pChild);
        if (flag == 0) {
            MultiTree_RemoveChild(pTargetNode);
            return pTargetNode;
        }
    }
    else {
        ((QuickListNode*)pTargetNode)->AddRight((QuickListNode*)pChild);
    }
    return NULL;
}

EXTERN_C Boolean __stdcall MultiTree_RemoveChild(
                            MultiTree * pThis,MultiTreeNode* pChild)
{
    MultiTreeNode * pReplaceNode;
    pReplaceNode = (MultiTreeNode*)((QuickListNode*)pChild)->Remove();

    // if removing the root of QuickListNode, replacement is the new root
    //
    if (pThis->m_pChildren == pChild) {
        pThis->m_pChildren = pReplaceNode;
    }
    return TRUE;
}

EXTERN_C Boolean __stdcall MultiTree_Lock(MultiTree * pThis)
{
    return FALSE;
}

EXTERN_C Boolean __stdcall MultiTree_Unlock(MultiTree * pThis)
{
    return FALSE;
}

