
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetProviderInfo.h"
#include "content/CComponentName.h"

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
    mProvider = name;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetProvider(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    *name = mProvider;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetMinWidth(
    /* [in] */ Int32 width)
{
    mMinWidth = width;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetMinWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mMinWidth;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetMinHeight(
    /* [in] */ Int32 height)
{
    mMinHeight = height;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetMinHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mMinHeight;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetUpdatePeriodMillis(
    /* [in] */ Int32 period)
{
    mUpdatePeriodMillis = period;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetUpdatePeriodMillis(
    /* [out] */ Int32* period)
{
    VALIDATE_NOT_NULL(period);
    *period = mUpdatePeriodMillis;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetInitialLayout(
    /* [in] */ Int32 layout)
{
    mInitialLayout = layout;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetInitialLayout(
    /* [out] */ Int32* layout)
{
    VALIDATE_NOT_NULL(layout);
    *layout = mInitialLayout;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetConfigure(
    /* [in] */ IComponentName* name)
{
    mConfigure = name;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetConfigure(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    *name = mConfigure;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetLabel(
    /* [in] */ const String& label)
{
    mLabel = label;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetLabel(
    /* [out] */ String* label)
{
    VALIDATE_NOT_NULL(label);
    *label = mLabel;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetPreviousName(
    /* [in] */ const String& name)
{
    mOldName = name;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetPreviousName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mOldName;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    Int32 value;
    source->ReadInt32(&value);
    if (0 != value) {
        // CComponentName::New(in, (IComponentName**)&mProvider);
    }
    source->ReadInt32(&mMinWidth);
    source->ReadInt32(&mMinHeight);
    source->ReadInt32(&mUpdatePeriodMillis);
    source->ReadInt32(&mInitialLayout);
    source->ReadInt32(&value);
    if (0 != value) {
        // CComponentName::New(in, (IComponentName**)&mConfigure);
    }
    source->ReadString(&mLabel);
    source->ReadInt32(&mIcon);
    return NOERROR;
}

ECode CAppWidgetProviderInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    if (mProvider != NULL) {
        dest->WriteInt32(1);
        AutoPtr<IParcelable> p;
        if ((IParcelable*)mProvider->Probe(EIID_IParcelable) != NULL) {
            p = (IParcelable*)mProvider->Probe(EIID_IParcelable);
        }
        p->WriteToParcel(dest);
    }
    else {
        dest->WriteInt32(0);
    }
    dest->WriteInt32(mMinWidth);
    dest->WriteInt32(mMinHeight);
    dest->WriteInt32(mUpdatePeriodMillis);
    dest->WriteInt32(mInitialLayout);
    if (mConfigure != NULL) {
        dest->WriteInt32(1);
        AutoPtr<IParcelable> p;
        if ((IParcelable*)mConfigure->Probe(EIID_IParcelable) != NULL) {
            p = (IParcelable*)mProvider->Probe(EIID_IParcelable);
        }
        p->WriteToParcel(dest);
    }
    else {
        dest->WriteInt32(0);
    }
    dest->WriteString(mLabel);
    dest->WriteInt32(mIcon);

    return NOERROR;
}

ECode CAppWidgetProviderInfo::constructor()
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::constructor(
    /* [in] */ IParcel* source)
{
    return ReadFromParcel(source);
}
