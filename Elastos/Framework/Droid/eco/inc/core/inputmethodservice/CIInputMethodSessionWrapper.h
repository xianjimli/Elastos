
#ifndef  __CIINPUTMETHODSESSIONWRAPPER_H__
#define  __CIINPUTMETHODSESSIONWRAPPER_H__

#include "_CIInputMethodSessionWrapper.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

CarClass(CIInputMethodSessionWrapper)
{
protected:
    // NOTE: we should have a cache of these.
    class InputMethodEventCallbackWrapper
        : public ElRefBase
        , public ILocalInputMethodSessionEventCallback
    {
    public:
        InputMethodEventCallbackWrapper(
            /* [in] */ IInputMethodCallback* cb);

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
        AutoPtr<IInputMethodCallback> mCb;
    };

public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILocalInputMethodSession* inputMethodSession);

    CARAPI_(AutoPtr<ILocalInputMethodSession>) GetInternalInputMethodSession();

    CARAPI FinishInput();

    CARAPI DisplayCompletions(
        /* [in] */ ArrayOf<ICompletionInfo*>* completions);

    CARAPI UpdateExtractedText(
        /* [in] */ Int32 token,
        /* [in] */ IExtractedText* text);

    CARAPI DispatchKeyEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IKeyEvent* event,
        /* [in] */ IInputMethodCallback* callback);

    CARAPI DispatchTrackballEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* event,
        /* [in] */ IInputMethodCallback* callback);

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

private:
    CARAPI HandleFinishInput();

    CARAPI HandleDisplayCompletions(
        /* [in] */ ArrayOf<ICompletionInfo*>* completions);

    CARAPI HandleUpdateExtractedText(
        /* [in] */ Int32 token,
        /* [in] */ IExtractedText* text);

    CARAPI HandleDispatchKeyEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IKeyEvent* event,
        /* [in] */ IInputMethodCallback* callback);

    CARAPI HandleDispatchTrackballEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* event,
        /* [in] */ IInputMethodCallback* callback);

    CARAPI HandleUpdateSelection(
        /* [in] */ Int32 oldSelStart,
        /* [in] */ Int32 oldSelEnd,
        /* [in] */ Int32 newSelStart,
        /* [in] */ Int32 newSelEnd,
        /* [in] */ Int32 candidatesStart,
        /* [in] */ Int32 candidatesEnd);

    CARAPI HandleUpdateCursor(
        /* [in] */ IRect* newCursor);

    CARAPI HandleAppPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI HandleToggleSoftInput(
        /* [in] */ Int32 showFlags,
        /* [in] */ Int32 hideFlags);

    CARAPI HandleFinishSession();

protected:
    // HandlerCaller mCaller;
    AutoPtr<IApartment> mCaller;
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

#endif  //__CIINPUTMETHODSESSIONWRAPPER_H__
