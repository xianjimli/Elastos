

#ifndef  _IINPUTCONNECTIONWRQPPER_H__
#define  _IINPUTCONNECTIONWRQPPER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

class IInputConnectionWrapper:
    public ElRefBase,
    public IInputContextStub {
private:
    class SomeArgs {
    public:
        SomeArgs(
            /* [in] */ IInterface* arg1 = NULL,
            /* [in] */ IInterface* arg2 = NULL,
            /* [in] */ IInputContextCallbackStub* clback = NULL,
            /* [in] */ Int32 seq = 0);

        ~SomeArgs();

        AutoPtr<IInterface> mArg1;
        AutoPtr<IInterface> mArg2;
        AutoPtr<IInputContextCallbackStub> mCallback;
        Int32 mSeq;
    };

    class Message {
    public:
        Message(
            /* [in] */ Int32 what,
            /* [in] */ Int32 arg1 = 0,
            /* [in] */ Int32 arg2 = 0,
            /* [in] */ SomeArgs* someArg = NULL);

        ~Message();

    public:
        Int32 mWhat;
        Int32 mArg1;
        Int32 mArg2;
        SomeArgs* mArgObj;
    };

    // class MyHandler extends Handler {
    //     MyHandler(Looper looper) {
    //         super(looper);
    //     }

    //     @Override
    //     public void handleMessage(Message msg) {
    //         ExecuteMessage(msg);
    //     }
    // };

public:
    IInputConnectionWrapper(
        /* [in] */ IApartment* mainLooper,
        /* [in] */ IInputConnection* conn);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(Boolean) IsActive();

    CARAPI GetTextAfterCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallbackStub* callback);

    CARAPI GetTextBeforeCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallbackStub* callback);

    CARAPI GetSelectedText(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallbackStub* callback);

    CARAPI GetCursorCapsMode(
        /* [in] */ Int32 reqModes,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallbackStub* callback);

    CARAPI GetExtractedText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallbackStub* callback);

    CARAPI CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    CARAPI CommitCompletion(
        /* [in] */ ICompletionInfo* text);

    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI PerformEditorAction(
        /* [in] */ Int32 id);

    CARAPI PerformContextMenuAction(
        /* [in] */ Int32 id);

    CARAPI SetComposingRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SetComposingText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    CARAPI FinishComposingText();

    CARAPI SendKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI ClearMetaKeyStates(
        /* [in] */ Int32 states);

    CARAPI DeleteSurroundingText(
        /* [in] */ Int32 leftLength,
        /* [in] */ Int32 rightLength);

    CARAPI BeginBatchEdit();

    CARAPI EndBatchEdit();

    CARAPI ReportFullscreenMode(
        /* [in] */ Boolean enabled);

    CARAPI PerformPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

private:
    CARAPI DispatchMessage(
        /* [in] */ Message* msg);

    CARAPI ExecuteMessage(
        /* [in] */ Message* msg);

    Message* ObtainMessage(
        /* [in] */ Int32 what);

    Message* ObtainMessageII(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2);

    Message* ObtainMessageO(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1);

    Message* ObtainMessageISC(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallbackStub* callback);

    Message* ObtainMessageIISC(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallbackStub* callback);

    Message* ObtainMessageIOSC(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallbackStub* callback);

    Message* ObtainMessageIO(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ IInterface* arg2);

    Message* ObtainMessageOO(
        /* [in] */ Int32 what,
        /* [in] */ const String& arg1,
        /* [in] */ IInterface* arg2);

private:
    static CString TAG;

    static const Int32 DO_GET_TEXT_AFTER_CURSOR = 10;
    static const Int32 DO_GET_TEXT_BEFORE_CURSOR = 20;
    static const Int32 DO_GET_SELECTED_TEXT = 25;
    static const Int32 DO_GET_CURSOR_CAPS_MODE = 30;
    static const Int32 DO_GET_EXTRACTED_TEXT = 40;
    static const Int32 DO_COMMIT_TEXT = 50;
    static const Int32 DO_COMMIT_COMPLETION = 55;
    static const Int32 DO_SET_SELECTION = 57;
    static const Int32 DO_PERFORM_EDITOR_ACTION = 58;
    static const Int32 DO_PERFORM_CONTEXT_MENU_ACTION = 59;
    static const Int32 DO_SET_COMPOSING_TEXT = 60;
    static const Int32 DO_SET_COMPOSING_REGION = 63;
    static const Int32 DO_FINISH_COMPOSING_TEXT = 65;
    static const Int32 DO_SEND_KEY_EVENT = 70;
    static const Int32 DO_DELETE_SURROUNDING_TEXT = 80;
    static const Int32 DO_BEGIN_BATCH_EDIT = 90;
    static const Int32 DO_END_BATCH_EDIT = 95;
    static const Int32 DO_REPORT_FULLSCREEN_MODE = 100;
    static const Int32 DO_PERFORM_PRIVATE_COMMAND = 120;
    static const Int32 DO_CLEAR_META_KEY_STATES = 130;

    //WeakReference<InputConnection> mInputConnection;
    AutoPtr<IInputConnection> mInputConnection;

    AutoPtr<IApartment> mMainLooper;

    AutoPtr<IApartment> mH;
};

#endif  //_IINPUTCONNECTIONWRQPPER_H__
