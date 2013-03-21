
#include "view/IInputConnectionWrapper.h"
#include "os/CApartment.h"

CString IInputConnectionWrapper::TAG = "IInputConnectionWrapper";
const Int32 IInputConnectionWrapper::DO_GET_TEXT_AFTER_CURSOR;
const Int32 IInputConnectionWrapper::DO_GET_TEXT_BEFORE_CURSOR;
const Int32 IInputConnectionWrapper::DO_GET_SELECTED_TEXT;
const Int32 IInputConnectionWrapper::DO_GET_CURSOR_CAPS_MODE;
const Int32 IInputConnectionWrapper::DO_GET_EXTRACTED_TEXT;
const Int32 IInputConnectionWrapper::DO_COMMIT_TEXT;
const Int32 IInputConnectionWrapper::DO_COMMIT_COMPLETION;
const Int32 IInputConnectionWrapper::DO_SET_SELECTION;
const Int32 IInputConnectionWrapper::DO_PERFORM_EDITOR_ACTION;
const Int32 IInputConnectionWrapper::DO_PERFORM_CONTEXT_MENU_ACTION;
const Int32 IInputConnectionWrapper::DO_SET_COMPOSING_TEXT;
const Int32 IInputConnectionWrapper::DO_SET_COMPOSING_REGION;
const Int32 IInputConnectionWrapper::DO_FINISH_COMPOSING_TEXT;
const Int32 IInputConnectionWrapper::DO_SEND_KEY_EVENT;
const Int32 IInputConnectionWrapper::DO_DELETE_SURROUNDING_TEXT;
const Int32 IInputConnectionWrapper::DO_BEGIN_BATCH_EDIT;
const Int32 IInputConnectionWrapper::DO_END_BATCH_EDIT;
const Int32 IInputConnectionWrapper::DO_REPORT_FULLSCREEN_MODE;
const Int32 IInputConnectionWrapper::DO_PERFORM_PRIVATE_COMMAND;
const Int32 IInputConnectionWrapper::DO_CLEAR_META_KEY_STATES;

IInputConnectionWrapper::SomeArgs::SomeArgs(
    /* [in] */ IInterface* arg1,
    /* [in] */ IInterface* arg2,
    /* [in] */ IInputContextCallback* clback,
    /* [in] */ Int32 seq)
    : mArg1(arg1)
    , mArg2(arg2)
    , mCallback(clback)
    , mSeq(seq)
{}


IInputConnectionWrapper::Message::Message(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ const SomeArgs* someArg)
    : mWhat(what)
    , mArg1(arg1)
    , mArg2(arg2)
    , mArgObj(someArg)
{}

IInputConnectionWrapper::Message::~Message()
{
    if (mArgObj != NULL) {
        delete mArgObj;
        mArgObj = NULL;
    }
}

IInputConnectionWrapper::IInputConnectionWrapper()
{}

ECode IInputConnectionWrapper::Init(
    /* [in] */ IApartment* mainLooper,
    /* [in] */ IInputConnection* conn)
{
    //mInputConnection = new WeakReference<InputConnection>(conn);
    mInputConnection = conn;
    mMainLooper = mainLooper;
    return NOERROR;
}

Boolean IInputConnectionWrapper::IsActive()
{
    return TRUE;
}

ECode IInputConnectionWrapper::GetTextAfterCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return DispatchMessage(ObtainMessageIISC(DO_GET_TEXT_AFTER_CURSOR, length, flags, seq, callback));
}

ECode IInputConnectionWrapper::GetTextBeforeCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return DispatchMessage(ObtainMessageIISC(DO_GET_TEXT_BEFORE_CURSOR, length, flags, seq, callback));
}

ECode IInputConnectionWrapper::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return DispatchMessage(ObtainMessageISC(DO_GET_SELECTED_TEXT, flags, seq, callback));
}

ECode IInputConnectionWrapper::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return DispatchMessage(ObtainMessageISC(DO_GET_CURSOR_CAPS_MODE, reqModes, seq, callback));
}

ECode IInputConnectionWrapper::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return DispatchMessage(ObtainMessageIOSC(DO_GET_EXTRACTED_TEXT, flags,
            request, seq, callback));
}

ECode IInputConnectionWrapper::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    return DispatchMessage(ObtainMessageIO(DO_COMMIT_TEXT, newCursorPosition, text));
}

ECode IInputConnectionWrapper::CommitCompletion(
    /* [in] */ ICompletionInfo* text)
{
    return DispatchMessage(ObtainMessageO(DO_COMMIT_COMPLETION, text));
}

ECode IInputConnectionWrapper::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DispatchMessage(ObtainMessageII(DO_SET_SELECTION, start, end));
}

ECode IInputConnectionWrapper::PerformEditorAction(
    /* [in] */ Int32 id)
{
    return DispatchMessage(ObtainMessageII(DO_PERFORM_EDITOR_ACTION, id, 0));
}

ECode IInputConnectionWrapper::PerformContextMenuAction(
    /* [in] */ Int32 id)
{
    return DispatchMessage(ObtainMessageII(DO_PERFORM_CONTEXT_MENU_ACTION, id, 0));
}

ECode IInputConnectionWrapper::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DispatchMessage(ObtainMessageII(DO_SET_COMPOSING_REGION, start, end));
}

ECode IInputConnectionWrapper::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    return DispatchMessage(ObtainMessageIO(DO_SET_COMPOSING_TEXT, newCursorPosition, text));
}

ECode IInputConnectionWrapper::FinishComposingText()
{
    return DispatchMessage(ObtainMessage(DO_FINISH_COMPOSING_TEXT));
}

ECode IInputConnectionWrapper::SendKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return DispatchMessage(ObtainMessageO(DO_SEND_KEY_EVENT, event));
}

ECode IInputConnectionWrapper::ClearMetaKeyStates(
    /* [in] */ Int32 states)
{
    return DispatchMessage(ObtainMessageII(DO_CLEAR_META_KEY_STATES, states, 0));
}

ECode IInputConnectionWrapper::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength)
{
    return DispatchMessage(ObtainMessageII(DO_DELETE_SURROUNDING_TEXT,
        leftLength, rightLength));
}

ECode IInputConnectionWrapper::BeginBatchEdit()
{
    return DispatchMessage(ObtainMessage(DO_BEGIN_BATCH_EDIT));
}

ECode IInputConnectionWrapper::EndBatchEdit()
{
    return DispatchMessage(ObtainMessage(DO_END_BATCH_EDIT));
}

ECode IInputConnectionWrapper::ReportFullscreenMode(
    /* [in] */ Boolean enabled)
{
    //todo:
    // return DispatchMessage(ObtainMessageII(DO_REPORT_FULLSCREEN_MODE, enabled ? 1 : 0, 0));
    return NOERROR;
}

ECode IInputConnectionWrapper::PerformPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return DispatchMessage(ObtainMessageOO(DO_PERFORM_PRIVATE_COMMAND, action, data));
}

ECode IInputConnectionWrapper::GetDescription(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode IInputConnectionWrapper::DispatchMessage(
    /* [in] */ Message* msg)
{
    // If we are calling this from the main thread, then we can call
    // right through.  Otherwise, we need to send the message to the
    // main thread.
    // todo:
    // AutoPtr<IApartment> apartment;
    // CApartment::GetDefaultApartment((IApartment**)&apartment);
    // if (apartment == mMainLooper) {
    //     return ExecuteMessage(msg);
    // }
    ECode (STDCALL IInputConnectionWrapper::*pHandlerFunc)(Message* msg);
    pHandlerFunc = &IInputConnectionWrapper::ExecuteMessage;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)msg);
    return mMainLooper->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode IInputConnectionWrapper::ExecuteMessage(
    /* [in] */ Message* _msg) {
    AutoMessage msg(_msg);
    switch (msg->mWhat) {
        case DO_GET_TEXT_AFTER_CURSOR: {
            const SomeArgs* args = msg->mArgObj;
            // try {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getTextAfterCursor on inactive InputConnection");
                return args->mCallback->SetTextAfterCursor(NULL, args->mSeq);
            }
            AutoPtr<ICharSequence> text;
            ic->GetTextAfterCursor(msg->mArg1, msg->mArg2, (ICharSequence**)&text);
            return args->mCallback->SetTextAfterCursor(text, args->mSeq);
            // } catch (RemoteException e) {
            //     Log.w(TAG, "Got RemoteException calling setTextAfterCursor", e);
            // }
        }
        case DO_GET_TEXT_BEFORE_CURSOR: {
            const SomeArgs* args = msg->mArgObj;
            // try {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getTextBeforeCursor on inactive InputConnection");
                return args->mCallback->SetTextBeforeCursor(NULL, args->mSeq);
            }
            AutoPtr<ICharSequence> text;
            ic->GetTextBeforeCursor(msg->mArg1, msg->mArg2, (ICharSequence**)&text);
            return args->mCallback->SetTextBeforeCursor(text, args->mSeq);
            // } catch (RemoteException e) {
            //     Log.w(TAG, "Got RemoteException calling setTextAfterCursor", e);
            // }
        }
        case DO_GET_SELECTED_TEXT: {
            const SomeArgs* args = msg->mArgObj;
            // try {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getSelectedText on inactive InputConnection");
                return args->mCallback->SetSelectedText(NULL, args->mSeq);
            }
            AutoPtr<ICharSequence> text;
            ic->GetSelectedText(msg->mArg1, (ICharSequence**)&text);
            return args->mCallback->SetSelectedText(text, args->mSeq);
            // } catch (RemoteException e) {
            //     Log.w(TAG, "Got RemoteException calling setTextAfterCursor", e);
            // }
        }
        case DO_GET_CURSOR_CAPS_MODE: {
            const SomeArgs* args = msg->mArgObj;
            // try {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getCursorCapsMode on inactive InputConnection");
                return args->mCallback->SetCursorCapsMode(0, args->mSeq);
            }
            Int32 capsMode = 0;
            ic->GetCursorCapsMode(msg->mArg1, &capsMode);
            return args->mCallback->SetCursorCapsMode(capsMode, args->mSeq);
            // } catch (RemoteException e) {
            //     Log.w(TAG, "Got RemoteException calling setTextAfterCursor", e);
            // }
        }
        case DO_GET_EXTRACTED_TEXT: {
            const SomeArgs* args = msg->mArgObj;
            // try {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getExtractedText on inactive InputConnection");
                return args->mCallback->SetExtractedText(NULL, args->mSeq);
            }
            AutoPtr<IExtractedText> extText;
            ic->GetExtractedText(IExtractedTextRequest::Probe(args->mArg1),
                    msg->mArg1, (IExtractedText**)&extText);
            return args->mCallback->SetExtractedText(extText, args->mSeq);
            // } catch (RemoteException e) {
            //     Log.w(TAG, "Got RemoteException calling setTextAfterCursor", e);
            // }
        }
        case DO_COMMIT_TEXT: {
            const SomeArgs* args = msg->mArgObj;
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "commitText on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->CommitText(ICharSequence::Probe(args->mArg1), msg->mArg1, &ret);
        }
        case DO_SET_SELECTION: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "setSelection on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->SetSelection(msg->mArg1, msg->mArg2, &ret);
        }
        case DO_PERFORM_EDITOR_ACTION: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "performEditorAction on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->PerformEditorAction(msg->mArg1, &ret);
        }
        case DO_PERFORM_CONTEXT_MENU_ACTION: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "performContextMenuAction on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->PerformContextMenuAction(msg->mArg1, &ret);
        }
        case DO_COMMIT_COMPLETION: {
            const SomeArgs* args = msg->mArgObj;
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "commitCompletion on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->CommitCompletion(ICompletionInfo::Probe(args->mArg1), &ret);
        }
        case DO_SET_COMPOSING_TEXT: {
            const SomeArgs* args = msg->mArgObj;
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "setComposingText on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->SetComposingText(ICharSequence::Probe(args->mArg1), msg->mArg1, &ret);
        }
        case DO_SET_COMPOSING_REGION: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "setComposingRegion on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->SetComposingRegion(msg->mArg1, msg->mArg2, &ret);
        }
        case DO_FINISH_COMPOSING_TEXT: {
            IInputConnection* ic = mInputConnection;
            // Note we do NOT check IsActive() here, because this is safe
            // for an IME to call at any time, and we need to allow it
            // through to clean up our state after the IME has switched to
            // another client.
            if (ic == NULL) {
                //Log.w(TAG, "finishComposingText on inactive InputConnection");
                return NOERROR;;
            }
            Boolean ret = FALSE;
            return ic->FinishComposingText(&ret);
        }
        case DO_SEND_KEY_EVENT: {
            const SomeArgs* args = msg->mArgObj;
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "sendKeyEvent on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->SendKeyEvent(IKeyEvent::Probe(args->mArg1), &ret);
        }
        case DO_CLEAR_META_KEY_STATES: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "clearMetaKeyStates on inactive InputConnection");
                return NOERROR;;
            }
            Boolean ret = FALSE;
            return ic->ClearMetaKeyStates(msg->mArg1, &ret);
        }
        case DO_DELETE_SURROUNDING_TEXT: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "deleteSurroundingText on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->DeleteSurroundingText(msg->mArg1, msg->mArg2, &ret);
        }
        case DO_BEGIN_BATCH_EDIT: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "beginBatchEdit on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->BeginBatchEdit(&ret);
        }
        case DO_END_BATCH_EDIT: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "endBatchEdit on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->EndBatchEdit(&ret);
        }
        case DO_REPORT_FULLSCREEN_MODE: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "showStatusIcon on inactive InputConnection");
                return NOERROR;
            }
            Boolean ret = FALSE;
            return ic->ReportFullscreenMode(msg->mArg1 == 1, &ret);
        }
        case DO_PERFORM_PRIVATE_COMMAND: {
            IInputConnection* ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "performPrivateCommand on inactive InputConnection");
                return NOERROR;
            }
            const SomeArgs* args = msg->mArgObj;
            String text;
            ICharSequence::Probe(args->mArg1)->ToString(&text);
            Boolean ret = FALSE;
            return ic->PerformPrivateCommand(text, IBundle::Probe(args->mArg2), &ret);
        }
    }
    //Log.w(TAG, "Unhandled message code: " + msg.what);

    return NOERROR;
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessage(
    /* [in] */ Int32 what)
{
    return new Message(what);
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageII(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    return new Message(what, arg1, arg2);
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageO(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* arg1)
{
    return new Message(what, 0, 0, new SomeArgs(arg1));
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageISC(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return new Message(what, arg1, 0, new SomeArgs(NULL, NULL, callback, seq));
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageIISC(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return new Message(what, arg1, arg2, new SomeArgs(NULL, NULL, callback, seq));
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageIOSC(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ IInterface* arg2,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return new Message(what, arg1, 0, new SomeArgs(arg2, NULL, callback, seq));
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageIO(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ IInterface* arg2)
{
    return new Message(what, arg1, 0, new SomeArgs(arg2));
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageOO(
    /* [in] */ Int32 what,
    /* [in] */ const String& arg1,
    /* [in] */ IInterface* arg2)
{
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(arg1, (ICharSequence**)&text);
    return new Message(what, 0, 0, new SomeArgs(text, arg2));
}
