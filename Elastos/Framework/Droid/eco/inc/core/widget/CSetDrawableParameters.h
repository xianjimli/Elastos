
#ifndef __CSETDRAWABLEPARAMETERS_H__
#define __CSETDRAWABLEPARAMETERS_H__

#include "_CSetDrawableParameters.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

#define SetDrawableParameters_TAG (3)

CarClass(CSetDrawableParameters)
{
public:
    CSetDrawableParameters();

    CARAPI Apply(
        /* [in] */ IView* root);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ Int32 id,
        /* [in] */ Boolean targetBackground,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 colorFilter,
        /* [in] */ PorterDuffMode mode,
        /* [in] */ Int32 level);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

private:
    Int32 mViewId;
    Boolean mTargetBackground;
    Int32 mAlpha;
    Int32 mColorFilter;
    PorterDuffMode mFilterMode;
    Int32 mLevel;
};

#endif // __CSETDRAWABLEPARAMETERS_H__
