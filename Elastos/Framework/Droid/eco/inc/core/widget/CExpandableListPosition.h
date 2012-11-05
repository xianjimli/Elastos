
#ifndef __CEXPANDABLELISTPOSITION_H__
#define __CEXPANDABLELISTPOSITION_H__

#include "_CExpandableListPosition.h"
#include "widget/ExpandableListPosition.h"

CarClass(CExpandableListPosition), public ExpandableListPosition
{
public:
    CARAPI GetPackedPosition(
        /* [out] */ Int64 * pPos);

    CARAPI ObtainGroupPosition(
        /* [in] */ Int32 groupPosition,
        /* [out] */ IExpandableListPosition ** ppPosition);

    CARAPI ObtainChildPosition(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ IExpandableListPosition ** ppPosition);

    CARAPI ObtainPosition(
        /* [in] */ Int64 packedPosition,
        /* [out] */ IExpandableListPosition ** ppPosition);

    CARAPI Obtain(
        /* [in] */ Int32 type,
        /* [in] */ Int32 groupPos,
        /* [in] */ Int32 childPos,
        /* [in] */ Int32 flatListPos,
        /* [out] */ IExpandableListPosition ** ppPosition);

    CARAPI Recycle();

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CEXPANDABLELISTPOSITION_H__
