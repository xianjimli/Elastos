
#ifndef  __CIINPUTMETHODWRAPPER_H__
#define  __CIINPUTMETHODWRAPPER_H__

#include "_CIInputMethodWrapper.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


/**
 * Implements the internal IInputMethod interface to convert incoming calls
 * on to it back to calls on the public InputMethod interface, scheduling
 * them on the main thread of the process.
 */
CarClass(CIInputMethodWrapper)
{
private:
    // static class Notifier {
    //     Boolean notified;
    // };

    // NOTE: we should have a cache of these.
    class InputMethodSessionCallbackWrapper
        : public ElRefBase
        , public ILocalInputMethodSessionCallback
    {
    public:
        InputMethodSessionCallbackWrapper(
            /* [in] */ IContext* context,
            /* [in] */ IInputMethodCallback* cb);

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
        AutoPtr<IInputMethodCallback> mCb;
    };

public:
    CARAPI constructor(
        /* [in] */ IAbstractInputMethodService* context,
        /* [in] */ ILocalInputMethod* inputMethod);

    CARAPI_(AutoPtr<ILocalInputMethod>) GetInternalInputMethod();

    CARAPI AttachToken(
        /* [in] */ IBinder* token);

    CARAPI BindInput(
        /* [in] */ IInputBinding* binding);

    CARAPI UnbindInput();

    CARAPI StartInput(
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* attribute);

    CARAPI RestartInput(
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* attribute);

    CARAPI CreateSession(
        /* [in] */ IInputMethodCallback* callback);

    CARAPI SetSessionEnabled(
        /* [in] */ IInputMethodSession* session,
        /* [in] */ Boolean enabled);

    CARAPI RevokeSession(
        /* [in] */ IInputMethodSession* session);

    CARAPI ShowSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver);

    CARAPI HideSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver);

    CARAPI GetDescription(
        /* [out] */ String* description);

private:
    CARAPI HandleAttachToken(
        /* [in] */ IBinder* token);

    CARAPI HandleSetInputContext(
        /* [in] */ IInputBinding* nu);

    CARAPI HandleUnsetInputContext();

    CARAPI HandleStartInput(
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* attribute);

    CARAPI HandleRestartInput(
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* attribute);

    CARAPI HandleCreateSession(
        /* [in] */ IInputMethodCallback* callback);

    CARAPI HandleSetSessionEnabled(
        /* [in] */ Boolean enabled,
        /* [in] */ ILocalInputMethodSession* ls);

    CARAPI HandleRevokeSession(
        /* [in] */ ILocalInputMethodSession* ls);

    CARAPI HandleShowSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver);

    CARAPI HandleHideSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver);

private:
    static const CString TAG;
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
    AutoPtr<IApartment> mCaller;
    //final WeakReference<ILocalInputMethod> mInputMethod;
    AutoPtr<ILocalInputMethod> mInputMethod;
    AutoPtr<IContext> mContext;
};

#endif  //__CIINPUTMETHODWRAPPER_H__
