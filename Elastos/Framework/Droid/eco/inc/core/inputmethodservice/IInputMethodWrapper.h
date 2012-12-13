
#ifndef  _IINPUTMETHODWRAPPER_H__
#define  _IINPUTMETHODWRAPPER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


/**
 * Implements the internal IInputMethodStub interface to convert incoming calls
 * on to it back to calls on the public InputMethod interface, scheduling
 * them on the main thread of the process.
 */
class IInputMethodWrapper:
    public ElRefBase,
    public IInputMethodStub
{
private:
    // static class Notifier {
    //     Boolean notified;
    // };

    // NOTE: we should have a cache of these.
    class InputMethodSessionCallbackWrapper:
        public ElRefBase,
        public ILocalInputMethodSessionCallback
    {
    public:
        InputMethodSessionCallbackWrapper(
            /* [in] */ IContext* context,
            /* [in] */ IInputMethodCallbackStub* cb);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI SessionCreated(
            /* [in] */ ILocalInputMethodSession* session);

    private:
        AutoPtr<IContext> mContext;
        AutoPtr<IInputMethodCallbackStub> mCb;
    };


public:
    IInputMethodWrapper(
        /* [in] */ IAbstractInputMethodService* context,
        /* [in] */ ILocalInputMethod* inputMethod);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(AutoPtr<ILocalInputMethod>) GetInternalInputMethod();

    CARAPI ExecuteMessage(
        /* [in] */ /*Message msg*/);

    CARAPI AttachToken(
        /* [in] */ IBinder* token);

    CARAPI BindInput(
        /* [in] */ IInputBinding* binding);

    CARAPI UnbindInput();

    CARAPI StartInput(
        /* [in] */ IInputContextStub* inputContext,
        /* [in] */ IEditorInfo* attribute);

    CARAPI RestartInput(
        /* [in] */ IInputContextStub* inputContext,
        /* [in] */ IEditorInfo* attribute);

    CARAPI CreateSession(
        /* [in] */ IInputMethodCallbackStub* callback);

    CARAPI SetSessionEnabled(
        /* [in] */ IInputMethodSessionStub* session,
        /* [in] */ Boolean enabled);

    CARAPI RevokeSession(
        /* [in] */ IInputMethodSessionStub* session);

    CARAPI ShowSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver);

    CARAPI HideSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver);

private:
    static CString TAG;
    static const Boolean DEBUG = FALSE;

    static const Int32 DO_DUMP = 1;
    static const Int32 DO_ATTACH_TOKEN = 10;
    static const Int32 DO_SET_INPUT_CONTEXT = 20;
    static const Int32 DO_UNSET_INPUT_CONTEXT = 30;
    static const Int32 DO_START_INPUT = 32;
    static const Int32 DO_RESTART_INPUT = 34;
    static const Int32 DO_CREATE_SESSION = 40;
    static const Int32 DO_SET_SESSION_ENABLED = 45;
    static const Int32 DO_REVOKE_SESSION = 50;
    static const Int32 DO_SHOW_SOFT_INPUT = 60;
    static const Int32 DO_HIDE_SOFT_INPUT = 70;

    //final WeakReference<AbstractInputMethodService> mTarget;
    AutoPtr<IAbstractInputMethodService> mTarget;
    //final HandlerCaller mCaller;
    //final WeakReference<ILocalInputMethod> mInputMethod;
    AutoPtr<ILocalInputMethod> mInputMethod;
    AutoPtr<IContext> mContext;
};

#endif  //_IINPUTMETHODWRAPPER_H__
