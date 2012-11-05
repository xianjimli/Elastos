
#ifndef __CPOSITIONMETADATA_H__
#define __CPOSITIONMETADATA_H__

#include "_CPositionMetadata.h"

#include "widget/ExpandableListConnector.h"

CarClass(CPositionMetadata), public ExpandableListConnector::PositionMetadata
{
public:
    CARAPI Obtain(
        /* [in] */ Int32 flatListPos,
        /* [in] */ Int32 type,
        /* [in] */ Int32 groupPos,
        /* [in] */ Int32 childPos,
        /* [in] */ IGroupMetadata * pGroupMetadata,
        /* [in] */ Int32 groupInsertIndex,
        /* [out] */ IPositionMetadata ** ppPositionMetadata);

    CARAPI Recycle();

    CARAPI IsExpanded(
        /* [out] */ Boolean * pExpanded);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CPOSITIONMETADATA_H__
