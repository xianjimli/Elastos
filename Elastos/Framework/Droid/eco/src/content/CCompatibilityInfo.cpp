
#include "content/CCompatibilityInfo.h"
#include <content/CompatibilityInfo.h>
#include <ext/frameworkdef.h>

ECode CCompatibilityInfo::Copy(
    /* [out] */ ICompatibilityInfo** info)
{
    VALIDATE_NOT_NULL(info);

    return CCompatibilityInfo::New(mAppFlags, mCompatibilityFlags,
            mApplicationDensity, mApplicationScale, mApplicationInvertedScale, info);
}

ECode CCompatibilityInfo::SetExpandable(
    /* [in] */ Boolean expandable)
{
    CompatibilityInfo::SetExpandable(expandable);
    return NOERROR;
}

ECode CCompatibilityInfo::SetLargeScreens(
    /* [in] */ Boolean expandable)
{
    CompatibilityInfo::SetLargeScreens(expandable);
    return NOERROR;
}

ECode CCompatibilityInfo::SetXLargeScreens(
    /* [in] */ Boolean expandable)
{
    CompatibilityInfo::SetXLargeScreens(expandable);
    return NOERROR;
}

ECode CCompatibilityInfo::IsConfiguredExpandable(
    /* [out] */ Boolean* isExpandable)
{
    VALIDATE_NOT_NULL(isExpandable);

    *isExpandable = CompatibilityInfo::IsConfiguredExpandable();
    return NOERROR;
}

ECode CCompatibilityInfo::IsConfiguredLargeScreens(
    /* [out] */ Boolean* isLarge)
{
    VALIDATE_NOT_NULL(isLarge);

    *isLarge = CompatibilityInfo::IsConfiguredLargeScreens();
    return NOERROR;
}

ECode CCompatibilityInfo::IsConfiguredXLargeScreens(
    /* [out] */ Boolean* isLarge)
{
    VALIDATE_NOT_NULL(isLarge);

    *isLarge = CompatibilityInfo::IsConfiguredXLargeScreens();
    return NOERROR;
}

ECode CCompatibilityInfo::IsScalingRequired(
    /* [out] */ Boolean* isRequired)
{
    VALIDATE_NOT_NULL(isRequired);

    *isRequired = CompatibilityInfo::IsScalingRequired();
    return NOERROR;
}

ECode CCompatibilityInfo::SupportsScreen(
    /* [out] */ Boolean* isSupported)
{
    VALIDATE_NOT_NULL(isSupported);

    *isSupported = CompatibilityInfo::SupportsScreen();
    return NOERROR;
}

ECode CCompatibilityInfo::GetDescription(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    *str = CompatibilityInfo::GetDescription();
    return NOERROR;
}

ECode CCompatibilityInfo::GetTranslator(
    /* [out] */ ITranslator** translator)
{
    VALIDATE_NOT_NULL(translator);

    return CompatibilityInfo::GetTranslator(translator);
}

ECode CCompatibilityInfo::UpdateCompatibleScreenFrame(
    /* [in] */ IDisplayMetrics* dm,
    /* [in] */ Int32 orientation,
    /* [in, out] */ IRect* outRect)
{
    VALIDATE_NOT_NULL(dm);
    VALIDATE_NOT_NULL(outRect);

    CompatibilityInfo::UpdateCompatibleScreenFrame(dm, orientation, outRect);
    return NOERROR;
}

ECode CCompatibilityInfo::constructor(
    /* [in] */ IApplicationInfo* appInfo)
{
    VALIDATE_NOT_NULL(appInfo);

    CompatibilityInfo::constructor(appInfo);
    return NOERROR;
}

ECode CCompatibilityInfo::constructor(
    /* [in] */ Int32 appFlags,
    /* [in] */ Int32 compFlags,
    /* [in] */ Int32 dens,
    /* [in] */ Float scale,
    /* [in] */ Float invertedScale)
{
    CompatibilityInfo::constructor(appFlags, compFlags, dens, scale, invertedScale);
    return NOERROR;
}

ECode CCompatibilityInfo::constructor()
{
    CompatibilityInfo::constructor();
    return NOERROR;
}

