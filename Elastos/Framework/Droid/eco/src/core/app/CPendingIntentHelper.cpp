
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include "app/CPendingIntentHelper.h"
#include "app/ActivityManagerNative.h"
#include "app/CPendingIntent.h"

ECode CPendingIntentHelper::GetActivity(
    /* [in] */ IContext* context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent** activity)
{
#if 0
    VALIDATE_NOT_NULL(activity);

    String capsuleName;
    FAIL_RETURN(context->GetCapsuleName(&capsuleName));

    String resolvedType;
    if (intent != NULL) {
        AutoPtr<IContentResolver> resolver;
        FAIL_RETURN(context->GetContentResolver((IContentResolver**)&resolver));
        FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &resolvedType));
    }

    // try {
    AutoPtr<IActivityManager> manager;
    FAIL_RETURN(ActivityManagerNative::GetDefault(
        (IActivityManager**)&manager));

    AutoPtr<IIntentSender> target;
    FAIL_RETURN(manager->GetIntentSender(
        ActivityManager_INTENT_SENDER_ACTIVITY,
        capsuleName,
        NULL,
        NULL,
        requestCode,
        intent,
        resolvedType,
        flags,
        (IIntentSender**)&target));

    if (target != NULL) {
        return CPendingIntent::New(target, activity);
    }
    else {
        *activity = NULL;
        return NOERROR;
    }
    // } catch (RemoteException e) {
    // }
#endif
    // TODO: ALEX need IActivityManager::GetIntentSender
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntentHelper::GetBroadcast(
    /* [in] */ IContext* context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent** broadcast)
{
#if 0
    VALIDATE_NOT_NULL(broadcast);

    String capsuleName;
    FAIL_RETURN(context->GetCapsuleName(&capsuleName));

    String resolvedType;
    if (intent != NULL) {
        AutoPtr<IContentResolver> resolver;
        FAIL_RETURN(context->GetContentResolver((IContentResolver**)&resolver));
        FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &resolvedType));
    }

    // try {
    AutoPtr<IActivityManager> manager;
    FAIL_RETURN(ActivityManagerNative::GetDefault(
        (IActivityManager**)&manager));

    AutoPtr<IIntentSender> target;
    FAIL_RETURN(manager->GetIntentSender(
        ActivityManager_INTENT_SENDER_BROADCAST,
        capsuleName,
        NULL,
        NULL,
        requestCode,
        intent,
        resolvedType,
        flags,
        (IIntentSender**)&target));

    if (target != NULL) {
        return CPendingIntent::New(target, broadcast);
    }
    else {
        *activity = NULL;
        return NOERROR;
    }
    // } catch (RemoteException e) {
    // }
#endif
    // TODO: ALEX need IActivityManager::GetIntentSender
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntentHelper::GetService(
    /* [in] */ IContext* context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent** service)
{
#if 0
    VALIDATE_NOT_NULL(service);

    String capsuleName;
    FAIL_RETURN(context->GetCapsuleName(&capsuleName));

    String resolvedType;
    if (intent != NULL) {
        AutoPtr<IContentResolver> resolver;
        FAIL_RETURN(context->GetContentResolver((IContentResolver**)&resolver));
        FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &resolvedType));
    }

    // try {
    AutoPtr<IActivityManager> manager;
    FAIL_RETURN(ActivityManagerNative::GetDefault(
        (IActivityManager**)&manager));

    AutoPtr<IIntentSender> target;
    FAIL_RETURN(manager->GetIntentSender(
        ActivityManager_INTENT_SENDER_SERVICE,
        capsuleName,
        NULL,
        NULL,
        requestCode,
        intent,
        resolvedType,
        flags,
        (IIntentSender**)&target));

    if (target != NULL) {
        return CPendingIntent::New(target, service);
    }
    else {
        *activity = NULL;
        return NOERROR;
    }
    // } catch (RemoteException e) {
    // }
#endif
    // TODO: ALEX need IActivityManager::GetIntentSender
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntentHelper::WritePendingIntentOrNullToParcel(
    /* [in] */ IPendingIntent* sender,
    /* [in] */ IParcel* outParcel)
{
    // TODO: ALEX need IParcel::writeStrongBinder
    // out.writeStrongBinder(sender != null ? sender.mTarget.asBinder() : null);
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntentHelper::ReadPendingIntentOrNullFromParcel(
    /* [in] */ IParcel* inParcel,
    /* [out] */ IPendingIntent** service)
{
    // TODO: ALEX need IParcel::readStrongBinder
    // IBinder b = in.readStrongBinder();
    // return b != null ? new PendingIntent(b) : null;
    return E_NOT_IMPLEMENTED;
}

