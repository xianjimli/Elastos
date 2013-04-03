
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetProviderInfo.h"

CAppWidgetProviderInfo::CAppWidgetProviderInfo()
    : mMinWidth(0)
    , mMinHeight(0)
    , mUpdatePeriodMillis(0)
    , mInitialLayout(0)
    , mIcon(0)
{}

ECode CAppWidgetProviderInfo::SetProvider(
    /* [in] */ IComponentName* name)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetProvider(
    /* [out] */ IComponentName** name)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetMinWidth(
    /* [in] */ Int32 width)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetMinWidth(
    /* [out] */ Int32* width)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetMinHeight(
    /* [in] */ Int32 height)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetMinHeight(
    /* [out] */ Int32* height)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetUpdatePeriodMillis(
    /* [in] */ Int32 period)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetUpdatePeriodMillis(
    /* [out] */ Int32* period)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetInitialLayout(
    /* [in] */ Int32 layout)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetInitialLayout(
    /* [out] */ Int32* layout)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetConfigure(
    /* [in] */ IComponentName* name)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetConfigure(
    /* [out] */ IComponentName** name)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetLabel(
    /* [in] */ const String& label)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetLabel(
    /* [out] */ String* label)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetPreviousName(
    /* [in] */ const String& name)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetPreviousName(
    /* [out] */ String* name)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::constructor()
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::constructor(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}
