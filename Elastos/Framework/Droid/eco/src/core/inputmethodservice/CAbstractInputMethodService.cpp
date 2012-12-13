
#include "inputmethodservice/CAbstractInputMethodService.h"

ICONTEXT_METHODS_IMPL(CAbstractInputMethodService, AbstractInputMethodService, AbstractInputMethodService);

CAbstractInputMethodService::CAbstractInputMethodService()
{

}

PInterface CAbstractInputMethodService::Probe(
    /* [in] */ REIID riid)
{
    return _CAbstractInputMethodService::Probe(riid);
}

ECode CAbstractInputMethodService::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return _CAbstractInputMethodService::GetInterfaceID(pObject, pIID);
}

ECode CAbstractInputMethodService::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
	return AbstractInputMethodService::GetKeyDispatcherState(dispatcherState);
}

ECode CAbstractInputMethodService::OnCreateInputMethodInterface(
    /* [out] */ IAbstractInputMethodImpl** impl)
{
    assert(0);
	return E_NOT_IMPLEMENTED;
}

ECode CAbstractInputMethodService::OnCreateInputMethodSessionInterface(
    /* [out] */ IAbstractInputMethodSessionImpl** impl)
{
    assert(0);
	return E_NOT_IMPLEMENTED;
}

ECode CAbstractInputMethodService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
	return AbstractInputMethodService::OnBind(intent, binder);
}

ECode CAbstractInputMethodService::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
	return AbstractInputMethodService::OnTrackballEvent(event, state);
}

ECode CAbstractInputMethodService::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CAbstractInputMethodService::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CAbstractInputMethodService::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CAbstractInputMethodService::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

//For IContextWrapper
ECode CAbstractInputMethodService::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    return AbstractInputMethodService::GetBaseContext(ctx);
}

ECode CAbstractInputMethodService::Attach(
    /* [in] */ IContext* ctx,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ const String& className,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IActivityManager* activityManager)
{
    return AbstractInputMethodService::Attach(ctx, apartment, className,
            token, application, activityManager);
}

ECode CAbstractInputMethodService::Create()
{
    return AbstractInputMethodService::Create();
}

ECode CAbstractInputMethodService::Start(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return AbstractInputMethodService::Start(intent, startId);
}

ECode CAbstractInputMethodService::StartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    return AbstractInputMethodService::StartCommand(intent, flags, startId, result);
}

ECode CAbstractInputMethodService::Destroy()
{
    return AbstractInputMethodService::Destroy();
}

ECode CAbstractInputMethodService::Bind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    return AbstractInputMethodService::Bind(intent, binder);
}

ECode CAbstractInputMethodService::Unbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return AbstractInputMethodService::Unbind(intent, succeeded);
}

ECode CAbstractInputMethodService::Rebind(
    /* [in] */ IIntent* intent)
{
    return AbstractInputMethodService::Rebind(intent);
}

ECode CAbstractInputMethodService::GetClassName(
    /* [out] */ String* className)
{
    return AbstractInputMethodService::GetClassName(className);
}
