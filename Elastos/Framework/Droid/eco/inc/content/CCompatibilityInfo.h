
#ifndef __CCOMPATIBILITYINFO_H__
#define __CCOMPATIBILITYINFO_H__

#include "_CCompatibilityInfo.h"
#include "content/CompatibilityInfo.h"

CarClass(CCompatibilityInfo), public CompatibilityInfo
{
public:
    CARAPI Copy(
        /* [out] */ ICompatibilityInfo** info);

    CARAPI SetExpandable(
        /* [in] */ Boolean expandable);

    CARAPI SetLargeScreens(
        /* [in] */ Boolean expandable);

    CARAPI SetXLargeScreens(
        /* [in] */ Boolean expandable);

    CARAPI IsConfiguredExpandable(
        /* [out] */ Boolean* isExpandable);

    CARAPI IsConfiguredLargeScreens(
        /* [out] */ Boolean* isLarge);

    CARAPI IsConfiguredXLargeScreens(
        /* [out] */ Boolean* isLarge);

    CARAPI IsScalingRequired(
        /* [out] */ Boolean* isRequired);

    CARAPI SupportsScreen(
        /* [out] */ Boolean* isSupported);

    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI GetTranslator(
        /* [out] */ ITranslator** translator);

    CARAPI UpdateCompatibleScreenFrame(
        /* [in] */ IDisplayMetrics* dm,
        /* [in] */ Int32 orientation,
        /* [in, out] */ IRect* outRect);

    CARAPI constructor(
        /* [in] */ IApplicationInfo* appInfo);

    CARAPI constructor(
        /* [in] */ Int32 appFlags,
        /* [in] */ Int32 compFlags,
        /* [in] */ Int32 dens,
        /* [in] */ Float scale,
        /* [in] */ Float invertedScale);

    CARAPI constructor();
};

#endif // __CCOMPATIBILITYINFO_H__
