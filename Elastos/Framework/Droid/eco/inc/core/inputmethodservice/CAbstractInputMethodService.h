
#ifndef  _CABSTRACTINPUTMETHODSERVICE_H__
#define  _CABSTRACTINPUTMETHODSERVICE_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#define __USE_MALLOC
#include "Elastos.Framework.Core.h"
#endif

#include "_CAbstractInputMethodService.h"
#include "inputmethodservice/AbstractInputMethodService.h"
#include "content/ContextMacro.h"
#include "content/ContextWrapper.h"

CarClass(CAbstractInputMethodService),
	public AbstractInputMethodService,
	public ContextWrapper
{
public:
	ICONTEXT_METHODS_DECL();

	CAbstractInputMethodService();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Return the global {@link KeyEvent.DispatcherState KeyEvent.DispatcherState}
     * for used for processing events from the target application.
     * Normally you will not need to use this directly, but
     * just use the standard high-level event callbacks like {@link #onKeyDown}.
     */
    CARAPI GetKeyDispatcherState(
        /* [out] */ IDispatcherState** dispatcherState);

    /**
     * Called by the framework during initialization, when the InputMethod
     * interface for this service needs to be created.
     */
    CARAPI OnCreateInputMethodInterface(
        /* [out] */ IAbstractInputMethodImpl** impl);

    /**
     * Called by the framework when a new InputMethodSession interface is
     * needed for a new client of the input method.
     */
    CARAPI OnCreateInputMethodSessionInterface(
        /* [out] */ IAbstractInputMethodSessionImpl** impl);

    /**
     * Implement this to handle {@link android.os.Binder#dump Binder.dump()}
     * calls on your input method.
     */
    // protected void dump(FileDescriptor fd, PrintWriter fout, String[] args) {
    // }

    CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    CARAPI OnTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* state);

    //For IKeyEventCallback interface.
    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyLongPress(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    //For IContextWrapper
    CARAPI GetBaseContext(
        /* [out] */ IContext** ctx);

    //For IService
    CARAPI Attach(
        /* [in] */ IContext* ctx,
        /* [in] */ IApplicationApartment* apartment,
        /* [in] */ const String& className,
        /* [in] */ IBinder* token,
        /* [in] */ IApplication* application,
        /* [in] */ IActivityManager* activityManager);

    CARAPI Create();

    CARAPI Start(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 startId);

    CARAPI StartCommand(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 startId,
        /* [out] */ Int32* result);

    CARAPI Destroy();

    CARAPI Bind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    CARAPI Unbind(
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* succeeded);

    CARAPI Rebind(
        /* [in] */ IIntent* intent);

    CARAPI GetClassName(
        /* [out] */ String* className);
};

#endif   //_CABSTRACTINPUTMETHODSERVICE_H__
