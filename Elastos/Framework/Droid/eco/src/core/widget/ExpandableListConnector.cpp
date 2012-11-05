
#include "widget/ExpandableListConnector.h"
#include "widget/CGroupMetadata.h"
#include "widget/CPositionMetadata.h"
#include "widget/CExpandableListPosition.h"
#include "os/SystemClock.h"
#include "widget/AdapterView.h"

using namespace Elastos::Core;

ArrayOf<IPositionMetadata*>* ExpandableListConnector::PositionMetadata::sPool  =
ArrayOf<IPositionMetadata*>::Alloc(ExpandableListConnector::PositionMetadata::MAX_POOL_SIZE);

ExpandableListConnector::ExpandableListConnector()
{

}
/**
 * Constructs the connector
 */
ExpandableListConnector::ExpandableListConnector(
    /* [in] */ IExpandableListAdapter* expandableListAdapter) 
{
    //mExpGroupMetadataList = new ArrayList<GroupMetadata>();
    mExpGroupMetadataList = ArrayOf<IGroupMetadata*>::Alloc(1);

    SetExpandableListAdapter(expandableListAdapter);
}

/**
 * Point to the {@link ExpandableListAdapter} that will give us data/Views
 * 
 * @param expandableListAdapter the adapter that supplies us with data/Views
 */
ECode ExpandableListConnector::SetExpandableListAdapter(
    /* [in] */ IExpandableListAdapter* expandableListAdapter)
{
    if (mExpandableListAdapter.Get() != NULL) {
        mExpandableListAdapter->UnregisterDataSetObserver(mDataSetObserver);
    }
    
    mExpandableListAdapter = expandableListAdapter;
    expandableListAdapter->RegisterDataSetObserver(mDataSetObserver);
}

/**
 * Translates a flat list position to either a) group pos if the specified
 * flat list position corresponds to a group, or b) child pos if it
 * corresponds to a child.  Performs a binary search on the expanded
 * groups list to find the flat list pos if it is an exp group, otherwise
 * finds where the flat list pos fits in between the exp groups.
 * 
 * @param flPos the flat list position to be translated
 * @return the group position or child position of the specified flat list
 *         position encompassed in a {@link PositionMetadata} object
 *         that contains additional useful info for insertion, etc.
 */
AutoPtr<IPositionMetadata> ExpandableListConnector::GetUnflattenedPos(
    /* [in] */ Int32 flPos) 
{
    /* Keep locally since frequent use */
    ArrayOf<IGroupMetadata*>* egml = mExpGroupMetadataList;
    Int32 numExpGroups = egml->GetLength();
    
    /* Binary search variables */
    Int32 leftExpGroupIndex = 0;
    Int32 rightExpGroupIndex = numExpGroups - 1;
    Int32 midExpGroupIndex = 0;
    AutoPtr<IGroupMetadata> midExpGm; 
    
    if (numExpGroups == 0) {
        /*
         * There aren't any expanded groups (hence no visible children
         * either), so flPos must be a group and its group pos will be the
         * same as its flPos
         */
        return PositionMetadata::Obtain(flPos, ExpandableListPosition::GROUP, flPos,
                -1, NULL, 0);
    }

    /*
     * Binary search over the expanded groups to find either the exact
     * expanded group (if we're looking for a group) or the group that
     * contains the child we're looking for. If we are looking for a
     * collapsed group, we will not have a direct match here, but we will
     * find the expanded group just before the group we're searching for (so
     * then we can calculate the group position of the group we're searching
     * for). If there isn't an expanded group prior to the group being
     * searched for, then the group being searched for's group position is
     * the same as the flat list position (since there are no children before
     * it, and all groups before it are collapsed).
     */
    while (leftExpGroupIndex <= rightExpGroupIndex) {
        midExpGroupIndex =
                (rightExpGroupIndex - leftExpGroupIndex) / 2
                        + leftExpGroupIndex;
        midExpGm = (*egml)[midExpGroupIndex];
        
        if (flPos > ((CGroupMetadata*)midExpGm.Get())->lastChildFlPos) {
            /*
             * The flat list position is after the current middle group's
             * last child's flat list position, so search right
             */
            leftExpGroupIndex = midExpGroupIndex + 1;
        } else if (flPos < ((CGroupMetadata*)midExpGm.Get())->flPos) {
            /*
             * The flat list position is before the current middle group's
             * flat list position, so search left
             */
            rightExpGroupIndex = midExpGroupIndex - 1;
        } else if (flPos == ((CGroupMetadata*)midExpGm.Get())->flPos) {
            /*
             * The flat list position is this middle group's flat list
             * position, so we've found an exact hit
             */
            return PositionMetadata::Obtain(flPos, ExpandableListPosition::GROUP,
                    ((CGroupMetadata*)midExpGm.Get())->gPos, -1, midExpGm, midExpGroupIndex);
        } else if (flPos <= ((CGroupMetadata*)midExpGm.Get())->lastChildFlPos
                /* && flPos > midGm.flPos as deduced from previous
                 * conditions */) {
            /* The flat list position is a child of the middle group */
            
            /* 
             * Subtract the first child's flat list position from the
             * specified flat list pos to get the child's position within
             * the group
             */
            Int32 childPos = flPos - (((CGroupMetadata*)midExpGm.Get())->flPos + 1);
            return PositionMetadata::Obtain(flPos, ExpandableListPosition::CHILD,
                    ((CGroupMetadata*)midExpGm.Get())->gPos, childPos, midExpGm, midExpGroupIndex);
        } 
    }

    /* 
     * If we've reached here, it means the flat list position must be a
     * group that is not expanded, since otherwise we would have hit it
     * in the above search.
     */


    /**
     * If we are to expand this group later, where would it go in the
     * mExpGroupMetadataList ?
     */
    Int32 insertPosition = 0;
    
    /** What is its group position in the list of all groups? */
    Int32 groupPos = 0;
    
    /*
     * To figure out exact insertion and prior group positions, we need to
     * determine how we broke out of the binary search.  We backtrack
     * to see this.
     */ 
    if (leftExpGroupIndex > midExpGroupIndex) {
        
        /*
         * This would occur in the first conditional, so the flat list
         * insertion position is after the left group. Also, the
         * leftGroupPos is one more than it should be (since that broke out
         * of our binary search), so we decrement it.
         */  
        AutoPtr<IGroupMetadata> leftExpGm = (*egml)[leftExpGroupIndex-1];            

        insertPosition = leftExpGroupIndex;

        /*
         * Sums the number of groups between the prior exp group and this
         * one, and then adds it to the prior group's group pos
         */
        groupPos =
            (flPos - ((CGroupMetadata*)leftExpGm.Get())->lastChildFlPos) + ((CGroupMetadata*)leftExpGm.Get())->gPos;            
    } else if (rightExpGroupIndex < midExpGroupIndex) {

        /*
         * This would occur in the second conditional, so the flat list
         * insertion position is before the right group. Also, the
         * rightGroupPos is one less than it should be, so increment it.
         */
        AutoPtr<IGroupMetadata> rightExpGm = (*egml)[++rightExpGroupIndex];            

        insertPosition = rightExpGroupIndex;
        
        /*
         * Subtracts this group's flat list pos from the group after's flat
         * list position to find out how many groups are in between the two
         * groups. Then, subtracts that number from the group after's group
         * pos to get this group's pos.
         */
        groupPos = ((CGroupMetadata*)rightExpGm.Get())->gPos - (((CGroupMetadata*)rightExpGm.Get())->flPos - flPos);
    } else {
        // TODO: clean exit
        //throw new RuntimeException("Unknown state");
    }
    
    return PositionMetadata::Obtain(flPos, ExpandableListPosition::GROUP, groupPos, -1,
            NULL, insertPosition);
}

/**
 * Translates either a group pos or a child pos (+ group it belongs to) to a
 * flat list position.  If searching for a child and its group is not expanded, this will
 * return NULL since the child isn't being shown in the ListView, and hence it has no
 * position.
 * 
 * @param pos a {@link ExpandableListPosition} representing either a group position
 *        or child position
 * @return the flat list position encompassed in a {@link PositionMetadata}
 *         object that contains additional useful info for insertion, etc., or NULL.
 */
AutoPtr<IPositionMetadata> ExpandableListConnector::GetFlattenedPos(
    /* [in] */ IExpandableListPosition* pos) 
{
    ArrayOf<IGroupMetadata*>* egml = mExpGroupMetadataList;
    Int32 numExpGroups = egml->GetLength();

    /* Binary search variables */
    Int32 leftExpGroupIndex = 0;
    Int32 rightExpGroupIndex = numExpGroups - 1;
    Int32 midExpGroupIndex = 0;
    AutoPtr<IGroupMetadata> midExpGm; 
    
    if (numExpGroups == 0) {
        /*
         * There aren't any expanded groups, so flPos must be a group and
         * its flPos will be the same as its group pos.  The
         * insert position is 0 (since the list is empty).
         */
        return PositionMetadata::Obtain(((CExpandableListPosition*)pos)->groupPos, ((CExpandableListPosition*)pos)->type,
                ((CExpandableListPosition*)pos)->groupPos, ((CExpandableListPosition*)pos)->childPos, NULL, 0);
    }

    /*
     * Binary search over the expanded groups to find either the exact
     * expanded group (if we're looking for a group) or the group that
     * contains the child we're looking for.
     */
    while (leftExpGroupIndex <= rightExpGroupIndex) {
        midExpGroupIndex = (rightExpGroupIndex - leftExpGroupIndex)/2 + leftExpGroupIndex;
        midExpGm = (*egml)[midExpGroupIndex];
        
        if (((CExpandableListPosition*)pos)->groupPos > ((CGroupMetadata*)midExpGm.Get())->gPos) {
            /*
             * It's after the current middle group, so search right
             */
            leftExpGroupIndex = midExpGroupIndex + 1;
        } else if (((CExpandableListPosition*)pos)->groupPos < ((CGroupMetadata*)midExpGm.Get())->gPos) {
            /*
             * It's before the current middle group, so search left
             */
            rightExpGroupIndex = midExpGroupIndex - 1;
        } else if (((CExpandableListPosition*)pos)->groupPos == ((CGroupMetadata*)midExpGm.Get())->gPos) {
            /*
             * It's this middle group, exact hit
             */
            
            if (((CExpandableListPosition*)pos)->type == ExpandableListPosition::GROUP) {
                /* If it's a group, give them this matched group's flPos */
                return PositionMetadata::Obtain(((CGroupMetadata*)midExpGm.Get())->flPos, ((CExpandableListPosition*)pos)->type,
                        ((CExpandableListPosition*)pos)->groupPos, ((CExpandableListPosition*)pos)->childPos, midExpGm, midExpGroupIndex);
            } else if (((CExpandableListPosition*)pos)->type == ExpandableListPosition::CHILD) {
                /* If it's a child, calculate the flat list pos */
                return PositionMetadata::Obtain(((CGroupMetadata*)midExpGm.Get())->flPos + ((CExpandableListPosition*)pos)->childPos
                        + 1, ((CExpandableListPosition*)pos)->type, ((CExpandableListPosition*)pos)->groupPos, ((CExpandableListPosition*)pos)->childPos,
                        midExpGm, midExpGroupIndex);
            } else {
                return NULL;
            }
        } 
    }

    /* 
     * If we've reached here, it means there was no match in the expanded
     * groups, so it must be a collapsed group that they're search for
     */
    if (((CExpandableListPosition*)pos)->type != ExpandableListPosition::GROUP) {
        /* If it isn't a group, return NULL */
        return NULL;
    }
    
    /*
     * To figure out exact insertion and prior group positions, we need to
     * determine how we broke out of the binary search. We backtrack to see
     * this.
     */ 
    if (leftExpGroupIndex > midExpGroupIndex) {
        
        /*
         * This would occur in the first conditional, so the flat list
         * insertion position is after the left group.
         * 
         * The leftGroupPos is one more than it should be (from the binary
         * search loop) so we subtract 1 to get the actual left group.  Since
         * the insertion point is AFTER the left group, we keep this +1
         * value as the insertion point
         */  
        AutoPtr<IGroupMetadata> leftExpGm = (*egml)[leftExpGroupIndex-1];            
        Int32 flPos =
                ((CGroupMetadata*)leftExpGm.Get())->lastChildFlPos
                        + (((CExpandableListPosition*)pos)->groupPos - ((CGroupMetadata*)leftExpGm.Get())->gPos);

        return PositionMetadata::Obtain(flPos, ((CExpandableListPosition*)pos)->type, ((CExpandableListPosition*)pos)->groupPos,
                ((CExpandableListPosition*)pos)->childPos, NULL, leftExpGroupIndex);
    } else if (rightExpGroupIndex < midExpGroupIndex) {

        /*
         * This would occur in the second conditional, so the flat list
         * insertion position is before the right group. Also, the
         * rightGroupPos is one less than it should be (from binary search
         * loop), so we increment to it.
         */
        AutoPtr<IGroupMetadata> rightExpGm = (*egml)[++rightExpGroupIndex];            
        Int32 flPos =
                ((CGroupMetadata*)rightExpGm.Get())->flPos
                        - (((CGroupMetadata*)rightExpGm.Get())->gPos - ((CExpandableListPosition*)pos)->groupPos);
        return PositionMetadata::Obtain(flPos, ((CExpandableListPosition*)pos)->type, ((CExpandableListPosition*)pos)->groupPos,
                ((CExpandableListPosition*)pos)->childPos, NULL, rightExpGroupIndex);
    } else {
        return NULL;
    }
}

Boolean ExpandableListConnector::AreAllItemsEnabled()
{
    Boolean enabled;
    mExpandableListAdapter->AreAllItemsEnabled(&enabled);
    return enabled;
}

Boolean ExpandableListConnector::IsEnabled(
    /* [in] */ Int32 flatListPos)
{
    AutoPtr<IExpandableListPosition> pos = ((CPositionMetadata*)GetUnflattenedPos(flatListPos).Get())->position;
    
    Boolean retValue;
    if (((CExpandableListPosition*)pos.Get())->type == ExpandableListPosition::CHILD) {
        mExpandableListAdapter->IsChildSelectable(((CExpandableListPosition*)pos.Get())->groupPos, ((CExpandableListPosition*)pos.Get())->childPos, &retValue);
    } else {
        // Groups are always selectable
        retValue = TRUE;
    }
    
    pos->Recycle();
    
    return retValue;
}

Int32 ExpandableListConnector::GetCount() 
{
    /*
     * Total count for the list view is the number groups plus the 
     * number of children from currently expanded groups (a value we keep
     * cached in this class)
     */ 
    Int32 count;
    mExpandableListAdapter->GetGroupCount(&count);

    return count + mTotalExpChildrenCount;
}

AutoPtr<IInterface> ExpandableListConnector::GetItem(
    /* [in] */ Int32 flatListPos)
{
    AutoPtr<IPositionMetadata> posMetadata = GetUnflattenedPos(flatListPos);

    AutoPtr<IInterface> retValue;
    if (((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->type == ExpandableListPosition::GROUP) {
        mExpandableListAdapter->GetGroup(((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->groupPos, (IInterface**)&retValue);
    } else if (((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->type == ExpandableListPosition::CHILD) {
        mExpandableListAdapter->GetChild(((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->groupPos,
                ((CExpandableListPosition*)(((CPositionMetadata*)posMetadata.Get())->position.Get()))->childPos, (IInterface**)&retValue);
    } else {
        // TODO: clean exit
        //throw new RuntimeException("Flat list position is of unknown type");
    }
    
    posMetadata->Recycle();
    
    return retValue;
}

Int64 ExpandableListConnector::GetItemId(
    /* [in] */ Int32 flatListPos) 
{
    AutoPtr<IPositionMetadata> posMetadata = GetUnflattenedPos(flatListPos);
    Int64 groupId;
    mExpandableListAdapter->GetGroupId(((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->groupPos, &groupId);
    
    Int64 retValue;
    if (((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->type == ExpandableListPosition::GROUP) {
        mExpandableListAdapter->GetCombinedGroupId(groupId, &retValue);
    } else if (((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->type == ExpandableListPosition::CHILD) {
        Int64 childId;
        mExpandableListAdapter->GetChildId(((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->groupPos,
                ((CExpandableListPosition*)(((CPositionMetadata*)posMetadata.Get())->position.Get()))->childPos, &childId);
        mExpandableListAdapter->GetCombinedChildId(groupId, childId, &retValue);
    } else {
        // TODO: clean exit
        //throw new RuntimeException("Flat list position is of unknown type");
    }
    
    posMetadata->Recycle();
    
    return retValue;
}

AutoPtr<IView> ExpandableListConnector::GetView(
    /* [in] */ Int32 flatListPos, 
    /* [in] */ IView* convertView, 
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IPositionMetadata> posMetadata = GetUnflattenedPos(flatListPos);

    AutoPtr<IView> retValue;
    Boolean expanded;
    if (((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->type == ExpandableListPosition::GROUP) {
        mExpandableListAdapter->GetGroupView(((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->groupPos,
                (((CPositionMetadata*)posMetadata.Get())->IsExpanded(&expanded), expanded), convertView, parent, (IView**)&retValue);
    } else if (((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->type == ExpandableListPosition::CHILD) {
        Boolean isLastChild = ((CGroupMetadata*)(((CPositionMetadata*)posMetadata.Get())->groupMetadata.Get()))->lastChildFlPos == flatListPos;
        
        mExpandableListAdapter->GetChildView(((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->groupPos,
                ((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->position.Get())->childPos, isLastChild, convertView, parent, (IView**)&retValue);
    } else {
        // TODO: clean exit
        //throw new RuntimeException("Flat list position is of unknown type");
    }
    
    posMetadata->Recycle();
    
    return retValue;
}

Int32 ExpandableListConnector::GetItemViewType(
    /* [in] */ Int32 flatListPos)
{
    AutoPtr<IExpandableListPosition> pos = ((CPositionMetadata*)GetUnflattenedPos(flatListPos).Get())->position;

    Int32 retValue;
    if (mExpandableListAdapter->Probe(EIID_IHeterogeneousExpandableList)) {
        AutoPtr<IHeterogeneousExpandableList> adapter =
                (IHeterogeneousExpandableList*) (mExpandableListAdapter.Get());
        if (((CExpandableListPosition*)pos.Get())->type == ExpandableListPosition::GROUP) {
            adapter->GetGroupType(((CExpandableListPosition*)pos.Get())->groupPos, &retValue);
        } else {
            Int32 childType;
            adapter->GetChildType(((CExpandableListPosition*)pos.Get())->groupPos, ((CExpandableListPosition*)pos.Get())->childPos, &childType);
            adapter->GetGroupTypeCount(&retValue);
            retValue += childType;
        }
    } else {
        if (((CExpandableListPosition*)pos.Get())->type == ExpandableListPosition::GROUP) {
            retValue = 0;
        } else {
            retValue = 1;
        }
    }
    
    pos->Recycle();
    
    return retValue;
}

Int32 ExpandableListConnector::GetViewTypeCount() 
{
    if (mExpandableListAdapter->Probe(EIID_IHeterogeneousExpandableList)) {
        AutoPtr<IHeterogeneousExpandableList> adapter =
                (IHeterogeneousExpandableList*) mExpandableListAdapter.Get();
        Int32 groupCount, childCount;
        adapter->GetGroupTypeCount(&groupCount);
        adapter->GetChildTypeCount(&childCount);
        return groupCount + childCount;
    } else {
        return 2;
    }
}

Boolean ExpandableListConnector::HasStableIds()
{
    Boolean ret;
    mExpandableListAdapter->HasStableIds(&ret);
    return ret;
}

/**
 * Traverses the expanded group metadata list and fills in the flat list
 * positions.
 * 
 * @param forceChildrenCountRefresh Forces refreshing of the children count
 *        for all expanded groups.
 * @param syncGroupPositions Whether to search for the group positions
 *         based on the group IDs. This should only be needed when calling
 *         this from an onChanged callback.
 */
void ExpandableListConnector::RefreshExpGroupMetadataList(
    /* [in] */ Boolean forceChildrenCountRefresh,
    /* [in] */ Boolean syncGroupPositions) 
{
    ArrayOf<IGroupMetadata*>* egml = mExpGroupMetadataList;
    Int32 egmlSize = egml->GetLength();
    Int32 curFlPos = 0;
    
    /* Update child count as we go through */
    mTotalExpChildrenCount = 0;
    
    if (syncGroupPositions) {
        // We need to check whether any groups have moved positions
        Boolean positionsChanged = FALSE;
        
        for (Int32 i = egmlSize - 1; i >= 0; i--) {
            AutoPtr<IGroupMetadata> curGm = (*egml)[i];
            Int32 newGPos = FindGroupPosition(((CGroupMetadata*)curGm.Get())->gId, ((CGroupMetadata*)curGm.Get())->gPos);
            if (newGPos != ((CGroupMetadata*)curGm.Get())->gPos) {
                if (newGPos == AdapterView_INVALID_POSITION) {
                    // Doh, just remove it from the list of expanded groups
                    //egml->Remove(i);
                    egmlSize--;
                }
                
                ((CGroupMetadata*)curGm.Get())->gPos = newGPos;
                if (!positionsChanged) positionsChanged = TRUE;
            }
        }
        
        if (positionsChanged) {
            // At least one group changed positions, so re-sort
            //Collections.sort(egml);
        }
    }
    
    Int32 gChildrenCount;
    Int32 lastGPos = 0;
    for (Int32 i = 0; i < egmlSize; i++) {
        /* Store in local variable since we'll access freq */
        AutoPtr<IGroupMetadata> curGm = (*egml)[i];
        
        /*
         * Get the number of children, try to refrain from calling
         * another class's method unless we have to (so do a subtraction)
         */
        if ((((CGroupMetadata*)curGm.Get())->lastChildFlPos == GroupMetadata::REFRESH) || forceChildrenCountRefresh) {
            mExpandableListAdapter->GetChildrenCount(((CGroupMetadata*)curGm.Get())->gPos, &gChildrenCount);
        } else {
            /* Num children for this group is its last child's fl pos minus
             * the group's fl pos
             */
            gChildrenCount = ((CGroupMetadata*)curGm.Get())->lastChildFlPos - ((CGroupMetadata*)curGm.Get())->flPos;
        }
        
        /* Update */
        mTotalExpChildrenCount += gChildrenCount;
        
        /*
         * This skips the collapsed groups and increments the flat list
         * position (for subsequent exp groups) by accounting for the collapsed
         * groups
         */
        curFlPos += (((CGroupMetadata*)curGm.Get())->gPos - lastGPos);
        lastGPos = ((CGroupMetadata*)curGm.Get())->gPos;
        
        /* Update the flat list positions, and the current flat list pos */
        ((CGroupMetadata*)curGm.Get())->flPos = curFlPos;
        curFlPos += gChildrenCount; 
        ((CGroupMetadata*)curGm.Get())->lastChildFlPos = curFlPos; 
    }
}

/**
 * Collapse a group in the grouped list view
 * 
 * @param groupPos position of the group to collapse
 */
Boolean ExpandableListConnector::CollapseGroup(
    /* [in] */ Int32 groupPos)
{
    AutoPtr<IPositionMetadata> pm = GetFlattenedPos(ExpandableListPosition::Obtain(
        ExpandableListPosition::GROUP, groupPos, -1, -1)); 
    if (pm == NULL) return FALSE;
    
    Boolean retValue = CollapseGroup(pm);
    pm->Recycle();
    return retValue;
}

Boolean ExpandableListConnector::CollapseGroup(
    /* [in] */ IPositionMetadata* posMetadata)
{
    /*
     * Collapsing requires removal from mExpGroupMetadataList 
     */
    
    /*
     * If it is NULL, it must be already collapsed. This group metadata
     * object should have been set from the search that returned the
     * position metadata object.
     */
    if (((CPositionMetadata*)posMetadata)->groupMetadata == NULL) return FALSE;
    
    // Remove the group from the list of expanded groups 
    //mExpGroupMetadataList->Remove(((CPositionMetadata*)posMetadata)->groupMetadata);

    // Refresh the metadata
    RefreshExpGroupMetadataList(FALSE, FALSE);
    
    // Notify of change
    //NotifyDataSetChanged();
    
    // Give the callback
    mExpandableListAdapter->OnGroupCollapsed(((CGroupMetadata*)((CPositionMetadata*)posMetadata)->groupMetadata.Get())->gPos);
    
    return TRUE;
}

/**
 * Expand a group in the grouped list view
 * @param groupPos the group to be expanded
 */
Boolean ExpandableListConnector::ExpandGroup(
    /* [in] */ Int32 groupPos)
{
    AutoPtr<IPositionMetadata> pm = GetFlattenedPos(ExpandableListPosition::Obtain(
        ExpandableListPosition::GROUP, groupPos, -1, -1));
    Boolean retValue = ExpandGroup(pm);
    pm->Recycle();
    return retValue;
}

Boolean ExpandableListConnector::ExpandGroup(
    /* [in] */ IPositionMetadata* posMetadata)
{
    /*
     * Expanding requires insertion into the mExpGroupMetadataList 
     */

    if (((CExpandableListPosition*)((CPositionMetadata*)posMetadata)->position.Get())->groupPos < 0) {
        // TODO clean exit
        //throw new RuntimeException("Need group");
    }

    if (mMaxExpGroupCount == 0) return FALSE;
    
    // Check to see if it's already expanded
    if (((CPositionMetadata*)posMetadata)->groupMetadata != NULL) return FALSE;
    
    /* Restrict number of expanded groups to mMaxExpGroupCount */
    if (mExpGroupMetadataList->GetLength() >= mMaxExpGroupCount) {
        /* Collapse a group */
        // TODO: Collapse something not on the screen instead of the first one?
        // TODO: Could write overloaded function to take GroupMetadata to collapse
        AutoPtr<IGroupMetadata> collapsedGm = (*mExpGroupMetadataList)[0];
        
        Int32 collapsedIndex;// = mExpGroupMetadataList->IndexOf(collapsedGm.Get());
        
        CollapseGroup(((CGroupMetadata*)collapsedGm.Get())->gPos);

        /* Decrement index if it is after the group we removed */
        if (((CPositionMetadata*)posMetadata)->groupInsertIndex > collapsedIndex) {
            ((CPositionMetadata*)posMetadata)->groupInsertIndex--;
        }
    }

    Int64 id;
    
    AutoPtr<IGroupMetadata> expandedGm = GroupMetadata::Obtain(
        GroupMetadata::REFRESH,
        GroupMetadata::REFRESH,
            ((CExpandableListPosition*)((CPositionMetadata*)posMetadata)->position.Get())->groupPos,
            (mExpandableListAdapter->GetGroupId(((CExpandableListPosition*)((CPositionMetadata*)posMetadata)->position.Get())->groupPos, &id),id));
    
    //mExpGroupMetadataList->Add(((CPositionMetadata*)posMetadata)->groupInsertIndex, expandedGm);

    // Refresh the metadata
    RefreshExpGroupMetadataList(FALSE, FALSE);
    
    // Notify of change
    //NotifyDataSetChanged();
    
    // Give the callback
    mExpandableListAdapter->OnGroupExpanded(((CGroupMetadata*)expandedGm.Get())->gPos);

    return TRUE;
}

/**
 * Whether the given group is currently expanded.
 * @param groupPosition The group to check.
 * @return Whether the group is currently expanded.
 */
Boolean ExpandableListConnector::IsGroupExpanded(
    /* [in] */ Int32 groupPosition)
{
    AutoPtr<IGroupMetadata> groupMetadata;
    for (Int32 i = mExpGroupMetadataList->GetLength() - 1; i >= 0; i--) {
        groupMetadata = (*mExpGroupMetadataList)[i];
        
        if (((CGroupMetadata*)groupMetadata.Get())->gPos == groupPosition) {
            return TRUE;
        }
    }
    
    return FALSE;
}

/**
 * Set the maximum number of groups that can be expanded at any given time
 */
ECode ExpandableListConnector::SetMaxExpGroupCount(
    /* [in] */ Int32 maxExpGroupCount) 
{
    mMaxExpGroupCount = maxExpGroupCount;

    return NOERROR;
}    

AutoPtr<IExpandableListAdapter> ExpandableListConnector::GetAdapter()
{
    return mExpandableListAdapter;
}

AutoPtr<IFilter> ExpandableListConnector::GetFilter()
{
    AutoPtr<IExpandableListAdapter> adapter = GetAdapter();
    if (adapter->Probe(EIID_IFilterable)) {

        AutoPtr<IFilter> filter;
        ((IFilterable*) adapter.Get())->GetFilter((IFilter**)&filter);
        return filter;
    } else {
        return NULL;
    }
}

ArrayOf<IGroupMetadata*>* ExpandableListConnector::GetExpandedGroupMetadataList()
{
    return mExpGroupMetadataList;
}

ECode ExpandableListConnector::SetExpandedGroupMetadataList(
    /* [in] */ ArrayOf<IGroupMetadata*>* expandedGroupMetadataList)
{
    
    if ((expandedGroupMetadataList == NULL) || (mExpandableListAdapter.Get() == NULL)) {
        return NOERROR;
    }
    
    // Make sure our current data set is big enough for the previously
    // expanded groups, if not, ignore this request
    Int32 numGroups;
    mExpandableListAdapter->GetGroupCount(&numGroups);
    for (Int32 i = expandedGroupMetadataList->GetLength() - 1; i >= 0; i--) {
        if (((CGroupMetadata*)(*expandedGroupMetadataList)[i])->gPos >= numGroups) {
            // Doh, for some reason the client doesn't have some of the groups
            return NOERROR;
        }
    }
    
    mExpGroupMetadataList = expandedGroupMetadataList;
    RefreshExpGroupMetadataList(TRUE, FALSE);
}

Boolean ExpandableListConnector::IsEmpty()
{
    AutoPtr<IExpandableListAdapter> adapter = GetAdapter();

    if (adapter) {
        Boolean empty;
        adapter->IsEmpty(&empty);
        return empty;
    }

    return TRUE;
}

/**
 * Searches the expandable list adapter for a group position matching the
 * given group ID. The search starts at the given seed position and then
 * alternates between moving up and moving down until 1) we find the right
 * position, or 2) we run out of time, or 3) we have looked at every
 * position
 * 
 * @return Position of the row that matches the given row ID, or
 *         {@link AdapterView#INVALID_POSITION} if it can't be found
 * @see AdapterView#findSyncPosition()
 */
Int32 ExpandableListConnector::FindGroupPosition(
    /* [in] */ Int64 groupIdToMatch, 
    /* [in] */ Int32 seedGroupPosition) 
{
    Int32 count;
    mExpandableListAdapter->GetGroupCount(&count);

    if (count == 0) {
        return AdapterView_INVALID_POSITION;
    }

    // If there isn't a selection don't hunt for it
    if (groupIdToMatch == AdapterView_INVALID_ROW_ID) {
        return AdapterView_INVALID_POSITION;
    }

    // Pin seed to reasonable values
    seedGroupPosition = Math::Max(0, seedGroupPosition);
    seedGroupPosition = Math::Min(count - 1, seedGroupPosition);

    Int64 endTime;// = SystemClock::UptimeMillis() + AdapterView::SYNC_MAX_DURATION_MILLIS;

    Int64 rowId;

    // first position scanned so far
    Int32 first = seedGroupPosition;

    // last position scanned so far
    Int32 last = seedGroupPosition;

    // True if we should move down on the next iteration
    Boolean next = FALSE;

    // True when we have looked at the first item in the data
    Boolean hitFirst;

    // True when we have looked at the last item in the data
    Boolean hitLast;

    // Get the item ID locally (instead of getItemIdAtPosition), so
    // we need the adapter
    AutoPtr<IExpandableListAdapter> adapter = GetAdapter();
    if (adapter.Get() == NULL) {
        return AdapterView_INVALID_POSITION;
    }

    while (SystemClock::UptimeMillis() <= endTime) {
        adapter->GetGroupId(seedGroupPosition, &rowId);
        if (rowId == groupIdToMatch) {
            // Found it!
            return seedGroupPosition;
        }

        hitLast = last == count - 1;
        hitFirst = first == 0;

        if (hitLast && hitFirst) {
            // Looked at everything
            break;
        }

        if (hitFirst || (next && !hitLast)) {
            // Either we hit the top, or we are trying to move down
            last++;
            seedGroupPosition = last;
            // Try going up next time
            next = FALSE;
        } else if (hitLast || (!next && !hitFirst)) {
            // Either we hit the bottom, or we are trying to move up
            first--;
            seedGroupPosition = first;
            // Try going down next time
            next = TRUE;
        }

    }

    return AdapterView_INVALID_POSITION;
}

ExpandableListConnector::MyDataSetObserver::MyDataSetObserver(
    /* [in] */ ExpandableListConnector* owner)
{
    this->owner = owner;
}
PInterface ExpandableListConnector::MyDataSetObserver::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 ExpandableListConnector::MyDataSetObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ExpandableListConnector::MyDataSetObserver::Release()
{
    return ElRefBase::Release();
}

ECode ExpandableListConnector::MyDataSetObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ExpandableListConnector::MyDataSetObserver::OnChanged() 
{
    owner->RefreshExpGroupMetadataList(TRUE, TRUE);
        
    //NotifyDataSetChanged();
}

ECode ExpandableListConnector::MyDataSetObserver::OnInvalidated() 
{
    owner->RefreshExpGroupMetadataList(TRUE, TRUE);
        
    //NotifyDataSetInvalidated();
}
    
ExpandableListConnector::GroupMetadata::GroupMetadata() 
{
}

AutoPtr<IGroupMetadata> ExpandableListConnector::GroupMetadata::Obtain(
    /* [in] */ Int32 flPos, 
    /* [in] */ Int32 lastChildFlPos, 
    /* [in] */ Int32 gPos, 
    /* [in] */ Int64 gId) 
{
    AutoPtr<IGroupMetadata> gm;
    CGroupMetadata::New((IGroupMetadata**)&gm);
    ((CGroupMetadata*)gm.Get())->flPos = flPos;
    ((CGroupMetadata*)gm.Get())->lastChildFlPos = lastChildFlPos;
    ((CGroupMetadata*)gm.Get())->gPos = gPos;
    ((CGroupMetadata*)gm.Get())->gId = gId;
    return gm;
}

Int32 ExpandableListConnector::GroupMetadata::CompareTo(
    /* [in] */ IGroupMetadata* another) 
{
    if (another == NULL) {
        //throw new IllegalArgumentException();
    }
    
    return gPos - ((CGroupMetadata*)another)->gPos;
}

Int32 ExpandableListConnector::GroupMetadata::DescribeContents()
{
    return 0;
}

ECode ExpandableListConnector::GroupMetadata::WriteToParcel(
    /* [in] */ IParcel* dest, 
    /* [in] */ Int32 flags)
{
    dest->WriteInt32(flPos);
    dest->WriteInt32(lastChildFlPos);
    dest->WriteInt32(gPos);
    dest->WriteInt64(gId);
}
    
void ExpandableListConnector::PositionMetadata::ResetState() 
{
    position = NULL;
    groupMetadata = NULL;
    groupInsertIndex = 0;
}

/**
 * Use {@link #obtain(Int32, Int32, Int32, Int32, GroupMetadata, Int32)}
 */
ExpandableListConnector::PositionMetadata::PositionMetadata() 
{
}

AutoPtr<IPositionMetadata> ExpandableListConnector::PositionMetadata::Obtain(
    /* [in] */ Int32 flatListPos, 
    /* [in] */ Int32 type, 
    /* [in] */ Int32 groupPos,
    /* [in] */ Int32 childPos, 
    /* [in] */ IGroupMetadata* groupMetadata, 
    /* [in] */ Int32 groupInsertIndex) 
{
    AutoPtr<IPositionMetadata> pm = GetRecycledOrCreate();
    ((CPositionMetadata*)pm.Get())->position = ExpandableListPosition::Obtain(type, groupPos, childPos, flatListPos);
    ((CPositionMetadata*)pm.Get())->groupMetadata = groupMetadata;
    ((CPositionMetadata*)pm.Get())->groupInsertIndex = groupInsertIndex;
    return pm;
}

AutoPtr<IPositionMetadata> ExpandableListConnector::PositionMetadata::GetRecycledOrCreate()
{
    AutoPtr<IPositionMetadata> pm;
    //synchronized (sPool) {
        if (sPool->GetLength() > 0) {
            //pm = sPool->Remove(0);
        } else {
            CPositionMetadata::New((IPositionMetadata**)&pm);
            return pm;
        }
    //}
    ((CPositionMetadata*)pm.Get())->ResetState();
    return pm;
}

void ExpandableListConnector::PositionMetadata::Recycle()
{
    //synchronized (sPool) {
        if (sPool->GetLength() < MAX_POOL_SIZE) {
            //sPool->Add(this);
        }
    //}
}

/**
 * Checks whether the group referred to in this object is expanded,
 * or not (at the time this object was created)
 * 
 * @return whether the group at groupPos is expanded or not
 */
Boolean ExpandableListConnector::PositionMetadata::IsExpanded() 
{
    return groupMetadata != NULL;
}
