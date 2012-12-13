
#ifndef  _IINPUTMETHODSESSIONWRAPPER_H__
#define  _IINPUTMETHODSESSIONWRAPPER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


class IInputMethodSessionWrapper:
    public ElRefBase,
    public IInputMethodSessionStub
{
protected:
    // NOTE: we should have a cache of these.
    class InputMethodEventCallbackWrapper:
        public ElRefBase,
        public ILocalInputMethodSessionEventCallback
    {
    public:
        InputMethodEventCallbackWrapper(
            /* [in] */ IInputMethodCallbackStub* cb);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI FinishedEvent(
            /* [in] */ Int32 seq,
            /* [in] */ Boolean handled);

    private:
        AutoPtr<IInputMethodCallbackStub> mCb;
    };

public:
    IInputMethodSessionWrapper(
        /* [in] */ IContext* context,
        /* [in] */ ILocalInputMethodSession* inputMethodSession);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(AutoPtr<ILocalInputMethodSession>) GetInternalInputMethodSession();

    CARAPI_(void) ExecuteMessage(
        /* [in] */ /*Message msg*/);

    CARAPI FinishInput();

    CARAPI DisplayCompletions(
        /* [in] */ ArrayOf<ICompletionInfo*>* completions);

    CARAPI UpdateExtractedText(
        /* [in] */ Int32 token,
        /* [in] */ IExtractedText* text);

    CARAPI DispatchKeyEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IKeyEvent* event,
        /* [in] */ IInputMethodCallbackStub* callback);

    CARAPI DispatchTrackballEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* event,
        /* [in] */ IInputMethodCallbackStub* callback);

    CARAPI UpdateSelection(
        /* [in] */ Int32 oldSelStart,
        /* [in] */ Int32 oldSelEnd,
        /* [in] */ Int32 newSelStart,
        /* [in] */ Int32 newSelEnd,
        /* [in] */ Int32 candidatesStart,
        /* [in] */ Int32 candidatesEnd);

    CARAPI UpdateCursor(
        /* [in] */ IRect* newCursor);

    CARAPI AppPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI ToggleSoftInput(
        /* [in] */ Int32 showFlags,
        /* [in] */ Int32 hideFlags);

    CARAPI FinishSession();

    CARAPI GetDescription(
        /* [out] */ String* description);

protected:
    // HandlerCaller mCaller;
    AutoPtr<ILocalInputMethodSession> mInputMethodSession;

private:
    static CString TAG;
    static const Boolean DEBUG = FALSE;

    static const Int32 DO_FINISH_INPUT = 60;
    static const Int32 DO_DISPLAY_COMPLETIONS = 65;
    static const Int32 DO_UPDATE_EXTRACTED_TEXT = 67;
    static const Int32 DO_DISPATCH_KEY_EVENT = 70;
    static const Int32 DO_DISPATCH_TRACKBALL_EVENT = 80;
    static const Int32 DO_UPDATE_SELECTION = 90;
    static const Int32 DO_UPDATE_CURSOR = 95;
    static const Int32 DO_APP_PRIVATE_COMMAND = 100;
    static const Int32 DO_TOGGLE_SOFT_INPUT = 105;
    static const Int32 DO_FINISH_SESSION = 110;
};

#endif  //_IINPUTMETHODSESSIONWRAPPER_H__
