
#include "ext/frameworkdef.h"
#include "app/CPendingIntent.h"
#include "content/CIntentSender.h"
#include "app/CFinishedDispatcher.h"
#include "app/ActivityManagerNative.h"

ECode CPendingIntent::GetIntentSender(
    /* [out] */ IIntentSender** intentSender)
{
    return CIntentSender::New(mTarget, (IParcelable**)intentSender);
}

ECode CPendingIntent::Cancel()
{
    // try {
    AutoPtr<IActivityManager> manager;
    FAIL_RETURN(ActivityManagerNative::GetDefault(
        (IActivityManager**)&manager));

    // TODO: ALEX need IActivityManager::CancelIntentSender
    // return manager->CancelIntentSender(mTarget);
    return E_NOT_IMPLEMENTED;
    // } catch (RemoteException e) {
    // }
}

ECode CPendingIntent::Send()
{
    return Send5(NULL, 0, NULL, NULL, NULL);
}

ECode CPendingIntent::Send2(
    /* [in] */ Int32 code)
{
    return Send5(NULL, code, NULL, NULL, NULL);
}

ECode CPendingIntent::Send3(
    /* [in] */ IContext* context,
    /* [in] */ Int32 code,
    /* [in] */ IIntent* intent)
{
    return Send5(context, code, intent, NULL, NULL);
}

ECode CPendingIntent::Send4(
    /* [in] */ Int32 code,
    /* [in] */ IOnFinished* onFinished,
    /* [in] */ IHandler* handler)
{
    return Send5(NULL, code, NULL, onFinished, handler);
}

ECode CPendingIntent::Send5(
    /* [in] */ IContext* context,
    /* [in] */ Int32 code,
    /* [in] */ IIntent* intent,
    /* [in] */ IOnFinished* onFinished,
    /* [in] */ IHandler* handler)
{
    // try {
    String resolvedType;
    if (intent != NULL) {
        AutoPtr<IContentResolver> resolver;
        FAIL_RETURN(context->GetContentResolver((IContentResolver**)&resolver));
        FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &resolvedType));
    }

    AutoPtr<IIntentReceiver> receiver;
    if (onFinished != NULL) {
        FAIL_RETURN(CFinishedDispatcher::New(
            this, onFinished, handler, (IIntentReceiver**)&receiver));
    }

    Int32 res;
    FAIL_RETURN(mTarget->Send(
        code, intent, resolvedType, (onFinished != NULL)? receiver: NULL, &res));
    if (res < 0) {
        // throw new CanceledException();
        return E_CANCELED_EXCEPTION;
    }

    return NOERROR;
    // } catch (RemoteException e) {
    //     throw new CanceledException(e);
    // }
}

ECode CPendingIntent::GetTargetCapsule(
    /* [out] */ String* targetCapsule)
{
    // try {
    AutoPtr<IActivityManager> manager;
    FAIL_RETURN(ActivityManagerNative::GetDefault(
        (IActivityManager**)&manager));

    // TODO: ALEX need IActivityManager::GetCapsuleForIntentSender
    // return manager->GetCapsuleForIntentSender(mTarget);
    return E_NOT_IMPLEMENTED;
    // } catch (RemoteException e) {
    //     // Should never happen.
    //     return null;
    // }
}

ECode CPendingIntent::GetTarget(
    /* [out] */ IIntentSender** target)
{
    VALIDATE_NOT_NULL(target);

    *target = mTarget;
    (*target)->AddRef();

    return NOERROR;
}

ECode CPendingIntent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: ALEX need IIntentSender::asBinder
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    // TODO: ALEX need IIntentSender::asBinder
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntent::constructor(
    /* [in] */ IIntentSender* target)
{
    mTarget = target;

    return NOERROR;
}

ECode CPendingIntent::constructor(
    /* [in] */ IBinder* target)
{
    // TODO: ALEX need asInterface
    // mTarget = IIntentSender.Stub.asInterface(target);
    return E_NOT_IMPLEMENTED;
}
