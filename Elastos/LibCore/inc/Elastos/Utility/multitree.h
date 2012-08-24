//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#if !defined(__MULTITREE_H__)
#define __MULTITREE_H__

#ifdef __cplusplus
/*!
 * @brief forward declaration.MultiTree may have any number of children, 
 * instead of at most two kids in a binary tree.To speed up the process of 
 * searching for a kid among all children, we use QuickListNode data
 * structure which manages a hidden balanced tree in addition to a list. 
 * In other words, we pay the price of 3 pointers to gain average searching
 * speed of O(log2(N)) rather than O(N).
 * @since
 * @sa
 */
class MultiTree;                

class MultiTreeNode {
/*!
 * @brief declare a friend class member of the MultiTreeNode class.
 */
    friend class MultiTree;
public:
/*!
 * @brief return parent node.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * Parent();      
/*!
 * @brief return previous node.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * Prev();        
/*!
 * @brief return next node.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * Next();         
/*!
 * @brief Constructs a MultiTreeNode instance without parent.
 */
    MultiTreeNode() : m_pParent(NULL) { }
/*!
 * @brief destructor of MultiTreeNode instance.
 */
    ~MultiTreeNode() { }

protected:
/*!
 * @brief a sorted list with quick access.
 */
    QuickListNode m_treeList; 
/*!
 * @brief everyone has a parent except root.
 */
    MultiTreeNode * m_pParent; 
};



/*!
 * @brief A MultiTree instance, first of all, is an instance of MultiTreeNode,
 * so it might be added or removed as child of another MultiTree instance.
 * A MultiTree instance controls multiple children of the same level, i.e.,
 * a parent node could add or remove a child, but a grand-parent can't do so
 * directly.  Roughly speaking, a directory in a file system corresponds
 * to a MultiTree whileas a file corresponds to a MultitreeNode.
 * @since
 * @sa
 */ 
class MultiTree : public MultiTreeNode {
public:
/*!
 * @brief Insert a child @a pChild in the right place among all children which is a 
 * sorted link-list.If the to be added child already exist in the list, remove and 
 * return the existing child.  Otherwise, return NULL.
 * @param pChild <b>[in]</b> pointer pointed to the child to be added.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode* AddChild(MultiTreeNode* pChild);
/*!
 * @brief Remove a child @a pChild from a sorted link-list.if removing the root of 
 * QuickListNode, replacement is the new root.
 * @param pChild <b>[in]</b>  pointer pointed to the child to be added.
 * @return
 * @since
 * @sa
 */
    Boolean RemoveChild(MultiTreeNode* pChild);
/*!
 * @brief gets the comparision result.
 * @param fnCompare <b>[out]</b> the value assigned to @a m_fnCompare.
 * @since
 * @sa
 */
    void RegNodeComparison(FnMultiTreeNodeComparison fnCompare);
/*!
 * @brief This method has no effect.
 * @return
 * @since
 * @sa
 */
    Boolean Lock();
/*!
 * @brief This method has no effect.
 * @return
 * @since
 * @sa
 */
    Boolean Unlock();
/*!
 * @brief The constructor of MultiTree.
 */
    MultiTree();
/*!
 * @brief The destructor of MultiTree.
 */
    ~MultiTree();

protected:
/*!
 * @brief Find the adjacent node of @a pNewNode,save the the adjacent node into
 * @a pTreeNode,and save the comparision result into @a pFlag.
 * @param pTreeNode <b>[out]</b> the adjacent node.
 * @param pNewNode <b>[in]</b> which to find its adjacent node.
 * @param pFlag <b>[out]</b> the comparision result.
 * @return pTreeNode the adjacent node.
 * @since
 * @sa
 */
    MultiTreeNode* FindAdjacentNode(
                MultiTreeNode* pTreeNode, MultiTreeNode* pNewNode, int* pFlag);
/*!
 * @brief point to sub-tree nodes.
 */
    MultiTreeNode* m_pChildren; 
/*!
 * @brief Comparison result of comparing pNode1 with pNode2.
 */
    FnMultiTreeNodeComparison m_fnCompare;
    // Mutex m_lock;
};


#else //!__cplusplus

typedef struct MultiTreeNode MultiTreeNode;

struct MultiTreeNode {
	MultiTreeNode m_treeList;
	MultiTreeNode * m_pParent;
}

typedef struct MultiTree MultiTree;

struct MultiTree :public MultiTreeNode {
    MultiTree * m_pChildren;     // point to sub-tree nodes
    FnMultiTreeNodeComparison m_fnCompare;
}

#endif //__cplusplus
/*!
 * @brief FnMultiTreeNodeComparison: compares pNode1 with pNode2;
 * Return value:
 *     =0, if pNode1 is equal to pNode2;
 *     >0, if pNode1 is greater than pNode2;
 *     <0, if pNode1 is less than pNode2;
 * Remarks:
 *     User may provide this function to override the default comparison
 *     function to re-define the meaning of "greater than", "less than"
 *     or "equal to".  Default is to compare pointer address.
 */
EXTERN_C typedef int __stdcall (__stdcall *FnMultiTreeNodeComparison)(
            MultiTreeNode* pNode1, MultiTreeNode* pNode2);

/*!
 * @brief Default comparison is to compare the addresses of the two nodes.
 * @param pN1 pointer pointed to one node.
 * @param pN2 pointer pointed to another node.
 * @return
 * @since
 * @sa
 * @relates
 */
EXTERN_C int __stdcall _DefaultMultiTreeNodeComparison(
                                    MultiTreeNode* pN1, MultiTreeNode* pN2);
/*!
 * @brief Insert a child @a pChild in the right place among all children which is a sorted 
 * link-list @a pThis .If the to be added child already exist in the list, remove and return 
 * the existing child.  Otherwise, return NULL.
 * @param pThis pointer pointed to the sorted link-list.
 * @param pChild pointer pointed to the child to be added.
 * @return
 * @since
 * @sa
 * @relates
 */
EXTERN_C MultiTree* __stdcall MultiTree_AddChild(
                                    MultiTree* pThis, MultiTreeNode* pChild);
/*!
 * @brief Remove a child @a pChild from a sorted link-list @a pThis .if removing 
 * the root of QuickListNode, replacement is the new root.
 * @param pThis pointer pointed to the sorted link-list.
 * @param pChild pointer pointed to the child to be added.
 * @return
 * @since
 * @sa
 * @relates
 */
EXTERN_C Boolean __stdcall MultiTree_RemoveChild(
                                    MultiTree* pThis, MultiTreeNode* pChild);
/*!
 * @brief This method has no effect.
 * @param pThis the MultiTree.
 * @return
 * @since
 * @sa
 * @relates
 */
EXTERN_C Boolean __stdcall MultiTree_Lock(MultiTree* pThis);
/*!
 * @brief This method has no effect.
 * @param pThis the MultiTree.
 * @return
 * @since
 * @sa
 * @relates
 */
EXTERN_C Boolean __stdcall MultiTree_UnLock(MultiTree* pThis);
/*!
 * @brief Find the adjacent node of @a pNewNode,save the the adjacent node into
 * @a pTreeNode,and save the comparision result into @a pFlag.
 * @param pThis the MultiTree.
 * @param pTreeNode the adjacent node.
 * @param pNewNode which to find its adjacent node.
 * @param pFlag the comparision result.
 * @return pTreeNode the adjacent node.
 * @since
 * @sa
 * @relates
 */
EXTERN_C MultiTreeNode* __stdcall MultiTree_FindAdjacentNode(MultiTree* pThis,
                MultiTreeNode* pTreeNode, MultiTreeNode* pNewNode, int* pFlag);

#ifdef __cplusplus

CAR_INLINE MultiTreeNode * MultiTreeNode::Parent()
{
    return m_pParent;
}

CAR_INLINE MultiTreeNode * MultiTreeNode::Prev()
{
    return (MultiTreeNode*) m_treeList.Prev();
}

CAR_INLINE MultiTreeNode * MultiTreeNode::Next()
{
    return (MultiTreeNode*) m_treeList.Next();
}

//======================================================================
// MultiTree class inline functions
//
CAR_INLINE MultiTree::MultiTree() : m_pChildren(NULL), m_fnCompare(NULL)
{
    m_fnCompare = _DefaultMultiTreeNodeComparison;
}

CAR_INLINE MultiTree::~MultiTree()
{
    assert(m_pChildren == NULL);
}

CAR_INLINE void MultiTree::RegNodeComparison(FnMultiTreeNodeComparison fnCompare)
{
    m_fnCompare = fnCompare;
}

CAR_INLINE MultiTreeNode* MultiTree::AddChild(MultiTreeNode* pChild)
{
    return MultiTree_AddChild(this, pChild);
}

CAR_INLINE Boolean MultiTree::RemoveChild(MultiTreeNode* pChild)
{
    return MultiTree_RemoveChild(this,pChild);
}

CAR_INLINE Boolean MultiTree::Lock()
{
    return MultiTree_Lock(this);
}

CAR_INLINE Boolean MultiTree::Unlock()
{
    return MultiTree_UnLock(this);
}
CAR_INLINE MultiTreeNode* MultiTree::FindAdjacentNode(
                MultiTreeNode* pTreeNode, MultiTreeNode* pNewNode, int* pFlag)
{
    return MultiTree_FindAdjacentNode(this,pTreeNode,pNewNode,pFlag);
}
#endif //__cplusplus

#endif // __MULTITREE_H__
