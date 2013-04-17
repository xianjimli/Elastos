
#include "app/CLauncherActivityListItem.h"
#include "content/CResolveInfo.h"

ECode CLauncherActivityListItem::constructor(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [in] */ IResolveInfo* resolveInfo,
    /* [in] */ ILauncherActivityIconResizer* resizer)
{
    mResolveInfo = resolveInfo;
    resolveInfo->LoadLabel(pm, (ICharSequence**)&mLabel);
    AutoPtr<IComponentInfo> ci;
    resolveInfo->GetActivityInfo((IActivityInfo**)&ci);
    if (ci == NULL) resolveInfo->GetServiceInfo((IServiceInfo**)&ci);
    if (mLabel == NULL && ci != NULL) {
        AutoPtr<IActivityInfo> aInfo;
        resolveInfo->GetActivityInfo((IActivityInfo**)&aInfo);
        String name;
        aInfo->GetName(&name);
        CStringWrapper::New(name, (ICharSequence**)&mLabel);
    }

    if (resizer != NULL) {
        AutoPtr<IDrawable> icon;
        resolveInfo->LoadIcon(pm, (IDrawable**)&icon);
        resizer->CreateIconThumbnail(icon, (IDrawable**)&mIcon);
    }
    AutoPtr<IApplicationInfo> appInfo;
    ci->GetApplicationInfo((IApplicationInfo**)&appInfo);
    appInfo->GetCapsuleName(&mCapsuleName);
    ci->GetName(&mClassName);
    return NOERROR;
}

ECode CLauncherActivityListItem::constructor()
{
    return NOERROR;
}