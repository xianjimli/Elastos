
#ifndef __CEXPANDABLELISTVIEWSAVEDSTATE_H__
#define __CEXPANDABLELISTVIEWSAVEDSTATE_H__

#include "_CExpandableListViewSavedState.h"

CarClass(CExpandableListViewSavedState)
{
public:
    CARAPI GetSuperState(
        /* [out] */ IParcelable ** ppSuperState);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel * pSource);

    CARAPI WriteToParcel(
        /* [in] */ IParcel * pDest);

    CARAPI constructor(
        /* [in] */ IParcelable * pSuperState,
        /* [in] */ const ArrayOf<IGroupMetadata *> & expandedGroupMetadataList);

private:
    // TODO: Add your private member variables here.
};

#endif // __CEXPANDABLELISTVIEWSAVEDSTATE_H__
