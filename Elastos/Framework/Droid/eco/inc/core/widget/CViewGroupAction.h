
#ifndef __CVIEWGROUPACTION_H__
#define __CVIEWGROUPACTION_H__

#include "_CViewGroupAction.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

#define ViewGroupAction_TAG (4)

CarClass(CViewGroupAction)
{
public:
    CARAPI Apply(
        /* [in] */ IView* root);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ Int32 viewId,
        /* [in] */ IRemoteViews* nestedViews);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

private:
    Int32 mViewId;
    AutoPtr<IRemoteViews> mNestedViews;
};

#endif // __CVIEWGROUPACTION_H__
