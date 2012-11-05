
#include "widget/ExpandableListPosition.h"
#include "widget/CExpandableListPosition.h"

ArrayOf<AutoPtr<IExpandableListPosition*> >* ExpandableListPosition::sPool = ArrayOf<AutoPtr<IExpandableListPosition*> >::Alloc(ExpandableListPosition::MAX_POOL_SIZE);

void ExpandableListPosition::ResetState() 
{
    groupPos = 0;
    childPos = 0;
    flatListPos = 0;
    type = 0;
}

ExpandableListPosition::ExpandableListPosition() 
{
}

Int64 ExpandableListPosition::GetPackedPosition()
{
    if (type == CHILD) return ExpandableListView::GetPackedPositionForChild(groupPos, childPos);
    else return ExpandableListView::GetPackedPositionForGroup(groupPos);
}

AutoPtr<IExpandableListPosition> ExpandableListPosition::ObtainGroupPosition(
    /* [in] */ Int32 groupPosition)
{
    return Obtain(GROUP, groupPosition, 0, 0);
}

AutoPtr<IExpandableListPosition> ExpandableListPosition::ObtainChildPosition(
    /* [in] */ Int32 groupPosition, 
    /* [in] */ Int32 childPosition)
{
    return Obtain(CHILD, groupPosition, childPosition, 0);
}

AutoPtr<IExpandableListPosition> ExpandableListPosition::ObtainPosition(
    /* [in] */ Int64 packedPosition)
{
    if (packedPosition == ExpandableListView::PACKED_POSITION_VALUE_NULL) {
        return NULL;
    }
    
    AutoPtr<IExpandableListPosition> elp = GetRecycledOrCreate(); 
    ((CExpandableListPosition*)elp.Get())->groupPos = ExpandableListView::GetPackedPositionGroup(packedPosition);
    if (ExpandableListView::GetPackedPositionType(packedPosition) ==
        ExpandableListView::PACKED_POSITION_TYPE_CHILD) {
        ((CExpandableListPosition*)elp.Get())->type = CHILD;
        ((CExpandableListPosition*)elp.Get())->childPos = ExpandableListView::GetPackedPositionChild(packedPosition);
    } else {
        ((CExpandableListPosition*)elp.Get())->type = GROUP;
    }
    return elp;
}

AutoPtr<IExpandableListPosition> ExpandableListPosition::Obtain(
    /* [in] */ Int32 type, 
    /* [in] */ Int32 groupPos, 
    /* [in] */ Int32 childPos, 
    /* [in] */ Int32 flatListPos)
{
    AutoPtr<IExpandableListPosition> elp = GetRecycledOrCreate(); 
    ((CExpandableListPosition*)elp.Get())->type = type;
    ((CExpandableListPosition*)elp.Get())->groupPos = groupPos;
    ((CExpandableListPosition*)elp.Get())->childPos = childPos;
    ((CExpandableListPosition*)elp.Get())->flatListPos = flatListPos;
    return elp;
}

AutoPtr<IExpandableListPosition> ExpandableListPosition::GetRecycledOrCreate() 
{
    AutoPtr<IExpandableListPosition> elp;
    //synchronized (sPool) {
        if (sPool->GetLength() > 0) {
            //elp = sPool.remove(0);
        } else {
            //return new ExpandableListPosition();
            CExpandableListPosition::New((IExpandableListPosition**)&elp);
        }
    //}
    ((CExpandableListPosition*)elp.Get())->ResetState();
    return elp;
}

ECode ExpandableListPosition::Recycle() 
{
    //synchronized (sPool) {
        if (sPool->GetLength() < MAX_POOL_SIZE) {
            //sPool.add(this);
        }
    //}
}


