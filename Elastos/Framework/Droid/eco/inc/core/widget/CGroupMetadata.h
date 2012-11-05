
#ifndef __CGROUPMETADATA_H__
#define __CGROUPMETADATA_H__

#include "_CGroupMetadata.h"

#include "widget/ExpandableListConnector.h"

CarClass(CGroupMetadata), public ExpandableListConnector::GroupMetadata
{
public:
    CARAPI Obtain(
        /* [in] */ Int32 flPos,
        /* [in] */ Int32 lastChildFlPos,
        /* [in] */ Int32 gPos,
        /* [in] */ Int64 gId,
        /* [out] */ IGroupMetadata ** ppGroupMetadata);

    CARAPI CompareTo(
        /* [in] */ IGroupMetadata * pAnother,
        /* [out] */ Int32 * pPos);

    CARAPI DescribeContents(
        /* [out] */ Int32 * pContents);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel * pSource);

    CARAPI WriteToParcel(
        /* [in] */ IParcel * pDest);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CGROUPMETADATA_H__
