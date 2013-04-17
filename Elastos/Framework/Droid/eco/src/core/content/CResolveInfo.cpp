
#include "ext/frameworkdef.h"
#include "content/CResolveInfo.h"

ECode CResolveInfo::constructor()
{
    return NOERROR;
}

ECode CResolveInfo::constructor(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);
    Int32 type = 0;
    source->ReadInt32(&type);
    switch (type) {
        case 1:
            assert(0);
            //TODO
            // activityInfo = ActivityInfo.CREATOR.createFromParcel(source);
            // serviceInfo = NULL;
            break;
        case 2:
            assert(0);
            //TODO
            // serviceInfo = ServiceInfo.CREATOR.createFromParcel(source);
            // activityInfo = NULL;
            break;
        default:
            mActivityInfo = NULL;
            mServiceInfo = NULL;
            break;
    }

    source->ReadInt32(&type);
    assert(0);
    // if (type != 0) {
    //     filter = IntentFilter.CREATOR.createFromParcel(source);
    // }

    // priority = source.readInt();
    // preferredOrder = source.readInt();
    // match = source.readInt();
    // specificIndex = source.readInt();
    // labelRes = source.readInt();
    // nonLocalizedLabel
    //         = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    // icon = source.readInt();
    // resolvePackageName = source.readString();

    return NOERROR;
}

ECode CResolveInfo::GetActivityInfo(
    /* [out] */ IActivityInfo** activityInfo)
{
    if (activityInfo == NULL) return E_INVALID_ARGUMENT;

    *activityInfo = (IActivityInfo*)mActivityInfo.Get();
    if (*activityInfo != NULL) (*activityInfo)->AddRef();

    return NOERROR;
}

ECode CResolveInfo::SetActivityInfo(
    /* [in] */ IActivityInfo* activityInfo)
{
    mActivityInfo = (CActivityInfo*)activityInfo;
    return NOERROR;
}

ECode CResolveInfo::GetServiceInfo(
    /* [out] */ IServiceInfo** serviceInfo)
{
    if (serviceInfo == NULL) return E_INVALID_ARGUMENT;

    *serviceInfo = (IServiceInfo*)mServiceInfo.Get();
    if (*serviceInfo != NULL) (*serviceInfo)->AddRef();

    return NOERROR;
}

ECode CResolveInfo::SetServiceInfo(
    /* [in] */ IServiceInfo* serviceInfo)
{
    mServiceInfo = (CServiceInfo*)serviceInfo;
    return NOERROR;
}

ECode CResolveInfo::GetIntentFilter(
    /* [out] */ IIntentFilter** intentFilter)
{
    if (intentFilter == NULL) return E_INVALID_ARGUMENT;

    *intentFilter = (IIntentFilter*)mFilter.Get();
    (*intentFilter)->AddRef();

    return NOERROR;
}

ECode CResolveInfo::SetIntentFilter(
    /* [in] */ IIntentFilter* intentFilter)
{
    mFilter = (CIntentFilter*)intentFilter;
    return NOERROR;
}

ECode CResolveInfo::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    *priority = mPriority;
    return NOERROR;
}

ECode CResolveInfo::SetPriority(
    /* [in] */ Int32 priority)
{
    mPriority = priority;
    return NOERROR;
}

ECode CResolveInfo::GetPreferredOrder(
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order);
    *order = mPreferredOrder;
    return NOERROR;
}

ECode CResolveInfo::SetPreferredOrder(
    /* [in] */ Int32 order)
{
    mPreferredOrder = order;
    return NOERROR;
}

ECode CResolveInfo::GetMatch(
    /* [out] */ Int32* match)
{
    VALIDATE_NOT_NULL(match);
    *match = mMatch;
    return NOERROR;
}

ECode CResolveInfo::SetMatch(
    /* [in] */ Int32 match)
{
    mMatch = match;
    return NOERROR;
}

ECode CResolveInfo::GetSpecificIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mSpecificIndex;
    return NOERROR;
}

ECode CResolveInfo::SetSpecificIndex(
    /* [in] */ Int32 index)
{
    mSpecificIndex = index;
    return NOERROR;
}

ECode CResolveInfo::IsDefault(
    /* [out] */ Boolean* isDefault)
{
    VALIDATE_NOT_NULL(isDefault);
    *isDefault = mIsDefault;
    return NOERROR;
}

ECode CResolveInfo::SetDefault(
    /* [in] */ Boolean isDefault)
{
    mIsDefault = isDefault;
    return NOERROR;
}

ECode CResolveInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CResolveInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode CResolveInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    if (*label != NULL) (*label)->AddRef();
    return NOERROR;
}

ECode CResolveInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode CResolveInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CResolveInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CResolveInfo::GetResolveCapsuleName(
    /* [out] */ String* capsuleName)
{
    VALIDATE_NOT_NULL(capsuleName);
    *capsuleName = mResolveCapsuleName;
    return NOERROR;
}

ECode CResolveInfo::SetResolveCapsuleName(
    /* [in] */ const String& capsuleName)
{
    mResolveCapsuleName = capsuleName;
    return NOERROR;
}

ECode CResolveInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return NOERROR;
}

ECode CResolveInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return NOERROR;
}

ECode CResolveInfo::LoadLabel(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResolveInfo::LoadIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    return E_NOT_IMPLEMENTED;
}

