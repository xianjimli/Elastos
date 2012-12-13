
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
    /* [in] */ IInputContextCallbackStub* clback,
    /* [in] */ Int32 seq):
    mArg1(arg1),
    mArg2(arg2),
    mCallback(clback),
    mSeq(seq)
{
}

IInputConnectionWrapper::SomeArgs::~SomeArgs()
{

}

IInputConnectionWrapper::Message::Message(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ SomeArgs* someArg):
    mWhat(what),
    mArg1(arg1),
    mArg2(arg2),
    mArgObj(someArg)
{

}

IInputConnectionWrapper::Message::~Message()
{
    if (mArgObj != NULL) {
        delete mArgObj;
        mArgObj = NULL;
    }
}

IInputConnectionWrapper::IInputConnectionWrapper(
    /* [in] */ IApartment* mainLooper,
    /* [in] */ IInputConnection* conn):
    mInputConnection(conn),
    mMainLooper(mainLooper)
{
    //mInputConnection = new WeakReference<InputConnection>(conn);
    //mH = new MyHandler(mMainLooper);
    CApartment::New(TRUE, (IApartment**)&mH);
    mH->Start(ApartmentAttr_New);
}

PInterface IInputConnectionWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputContextStub == riid) {
        return (IInputContextStub *)this;
    }

    return NULL;
}

UInt32 IInputConnectionWrapper::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 IInputConnectionWrapper::Release()
{
	return ElRefBase::Release();
}

ECode IInputConnectionWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IInputContextStub*)this) {
        *pIID = EIID_IInputContextStub;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

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
    /* [in] */ IInputContextCallbackStub* callback)
{
    return DispatchMessage(ObtainMessageIISC(DO_GET_TEXT_AFTER_CURSOR, length, flags, seq, callback));
}

ECode IInputConnectionWrapper::GetTextBeforeCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallbackStub* callback)
{
    return DispatchMessage(ObtainMessageIISC(DO_GET_TEXT_BEFORE_CURSOR, length, flags, seq, callback));
}

ECode IInputConnectionWrapper::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallbackStub* callback)
{
    return DispatchMessage(ObtainMessageISC(DO_GET_SELECTED_TEXT, flags, seq, callback));
}

ECode IInputConnectionWrapper::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallbackStub* callback)
{
    return DispatchMessage(ObtainMessageISC(DO_GET_CURSOR_CAPS_MODE, reqModes, seq, callback));
}

ECode IInputConnectionWrapper::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallbackStub* callback)
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
    return DispatchMessage(ObtainMessageII(DO_REPORT_FULLSCREEN_MODE, enabled ? 1 : 0, 0));
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
    /* [in] */ Message* msg) {
    // If we are calling this from the main thread, then we can call
    // right through.  Otherwise, we need to send the message to the
    // main thread.
    assert(msg != NULL);

    AutoPtr<IApartment> apartment;
    CApartment::GetDefaultApartment((IApartment**) &apartment);
    if (apartment == mMainLooper) {
        return ExecuteMessage(msg);
    }

    ECode (STDCALL IInputConnectionWrapper::*pHandlerFunc)(Message* msg);

    pHandlerFunc = &IInputConnectionWrapper::ExecuteMessage;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32Ptr(*(Handle32*)msg);
    // params->WriteInt32(msg->mWhat);
    // params->WriteInt32(msg->mArg1);
    // params->WriteInt32(msg->mArg2);

    // if (msg->mArgObj != NULL) {
    //     params->WriteInterfacePtr(msg->mArgObj->mArg1);
    //     params->WriteInterfacePtr(msg->mArgObj->mArg2);
    //     params->WriteInterfacePtr(msg->mArgObj->mCallback);
    //     params->WriteInt32(msg->mArgObj->mSeq);
    // }

    return mH->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    //return mH.sendMessage(msg);
}

ECode IInputConnectionWrapper::ExecuteMessage(
    /* [in] */ Message* msg) {
    assert(msg != NULL);
    ECode ec = NOERROR;

    switch (msg->mWhat) {
        case DO_GET_TEXT_AFTER_CURSOR: {
            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            AutoPtr<IInputConnection> ic = mInputConnection;
            assert(args->mCallback != NULL);
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getTextAfterCursor on inactive InputConnection");
                ec = args->mCallback->SetTextAfterCursor(NULL, args->mSeq);
                break;
            }

            AutoPtr<ICharSequence> text;
            ec = ic->GetTextAfterCursor(msg->mArg1, msg->mArg2, (ICharSequence**) &text);
            ec = args->mCallback->SetTextAfterCursor(text, args->mSeq);

            break;
        }
        case DO_GET_TEXT_BEFORE_CURSOR: {
            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            AutoPtr<IInputConnection> ic = mInputConnection;
            assert(args->mCallback != NULL);
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getTextBeforeCursor on inactive InputConnection");
                ec = args->mCallback->SetTextBeforeCursor(NULL, args->mSeq);
                break;
            }

            AutoPtr<ICharSequence> text;
            ec = ic->GetTextBeforeCursor(msg->mArg1, msg->mArg2, (ICharSequence**) &text);
            ec = args->mCallback->SetTextBeforeCursor(text, args->mSeq);

            break;
        }
        case DO_GET_SELECTED_TEXT: {
            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            AutoPtr<IInputConnection> ic = mInputConnection;
            assert(args->mCallback != NULL);
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getSelectedText on inactive InputConnection");
                ec = args->mCallback->SetSelectedText(NULL, args->mSeq);
                break;
            }

            AutoPtr<ICharSequence> text;
            ec = ic->GetSelectedText(msg->mArg1, (ICharSequence**) &text);
            ec = args->mCallback->SetSelectedText(text, args->mSeq);

            break;
        }
        case DO_GET_CURSOR_CAPS_MODE: {
            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            AutoPtr<IInputConnection> ic = mInputConnection;
            assert(args->mCallback != NULL);
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getCursorCapsMode on inactive InputConnection");
                ec = args->mCallback->SetCursorCapsMode(0, args->mSeq);
                break;
            }

            Int32 capsMode = 0;
            ec = ic->GetCursorCapsMode(msg->mArg1, &capsMode);
            ec = args->mCallback->SetCursorCapsMode(capsMode, args->mSeq);

            break;
        }
        case DO_GET_EXTRACTED_TEXT: {
            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            AutoPtr<IInputConnection> ic = mInputConnection;
            assert(args->mCallback != NULL);
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "getExtractedText on inactive InputConnection");
                ec = args->mCallback->SetExtractedText(NULL, args->mSeq);
                break;
            }

            AutoPtr<IExtractedText> etedText;
            ec = ic->GetExtractedText(IExtractedTextRequest::Probe(args->mArg1),
                    msg->mArg1, (IExtractedText**) &etedText);

            ec = args->mCallback->SetExtractedText(NULL, args->mSeq);

            break;
        }
        case DO_COMMIT_TEXT: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "commitText on inactive InputConnection");
                break;
            }

            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            Boolean ret = FALSE;
            ec = ic->CommitText(ICharSequence::Probe(args->mArg1), msg->mArg1, &ret);

            break;
        }
        case DO_SET_SELECTION: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "setSelection on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->SetSelection(msg->mArg1, msg->mArg2, &ret);
            break;
        }
        case DO_PERFORM_EDITOR_ACTION: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "performEditorAction on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->PerformEditorAction(msg->mArg1, &ret);
            break;
        }
        case DO_PERFORM_CONTEXT_MENU_ACTION: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "performContextMenuAction on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->PerformContextMenuAction(msg->mArg1, &ret);
            break;
        }
        case DO_COMMIT_COMPLETION: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "commitCompletion on inactive InputConnection");
                break;
            }

            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            Boolean ret = FALSE;
            ec = ic->CommitCompletion(ICompletionInfo::Probe(args->mArg1), &ret);
            break;
        }
        case DO_SET_COMPOSING_TEXT: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "setComposingText on inactive InputConnection");
                break;
            }

            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            Boolean ret = FALSE;
            ec = ic->SetComposingText(ICharSequence::Probe(args->mArg1), msg->mArg1, &ret);
            break;
        }
        case DO_SET_COMPOSING_REGION: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "setComposingRegion on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->SetComposingRegion(msg->mArg1, msg->mArg2, &ret);
            break;
        }
        case DO_FINISH_COMPOSING_TEXT: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            // Note we do NOT check IsActive() here, because this is safe
            // for an IME to call at any time, and we need to allow it
            // through to clean up our state after the IME has switched to
            // another client.
            if (ic == NULL) {
                //Log.w(TAG, "finishComposingText on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->FinishComposingText(&ret);
            break;
        }
        case DO_SEND_KEY_EVENT: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "sendKeyEvent on inactive InputConnection");
                break;
            }

            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);

            Boolean ret = FALSE;
            ec = ic->SendKeyEvent(IKeyEvent::Probe(args->mArg1), &ret);
            break;
        }
        case DO_CLEAR_META_KEY_STATES: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "clearMetaKeyStates on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->ClearMetaKeyStates(msg->mArg1, &ret);
            break;
        }
        case DO_DELETE_SURROUNDING_TEXT: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "deleteSurroundingText on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->DeleteSurroundingText(msg->mArg1, msg->mArg2, &ret);
            break;
        }
        case DO_BEGIN_BATCH_EDIT: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "beginBatchEdit on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->BeginBatchEdit(&ret);
            break;
        }
        case DO_END_BATCH_EDIT: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "endBatchEdit on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->EndBatchEdit(&ret);
            break;
        }
        case DO_REPORT_FULLSCREEN_MODE: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "showStatusIcon on inactive InputConnection");
                break;
            }

            Boolean ret = FALSE;
            ec = ic->ReportFullscreenMode(msg->mArg1 == 1, &ret);
            break;
        }
        case DO_PERFORM_PRIVATE_COMMAND: {
            AutoPtr<IInputConnection> ic = mInputConnection;
            if (ic == NULL || !IsActive()) {
                //Log.w(TAG, "performPrivateCommand on inactive InputConnection");
                break;
            }


            SomeArgs* args = msg->mArgObj;
            assert(args != NULL);
            assert(args->mArg1 != NULL);
            String text;
            ICharSequence::Probe(args->mArg1)->ToString(&text);

            Boolean ret = FALSE;
            ec = ic->PerformPrivateCommand(text, IBundle::Probe(args->mArg2), &ret);
            break;
        }
    }

    //Log.w(TAG, "Unhandled message code: " + msg.what);

    delete msg;
    msg = NULL;
    return ec;
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
    SomeArgs* argObj = new SomeArgs(arg1);
    return new Message(what, 0, 0, argObj);
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageISC(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallbackStub* callback)
{
    SomeArgs* argObj = new SomeArgs(NULL, NULL, callback, seq);
    return new Message(what, arg1, 0, argObj);
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageIISC(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallbackStub* callback)
{
    SomeArgs* argObj = new SomeArgs(NULL, NULL, callback, seq);
    return new Message(what, arg1, arg2, argObj);
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageIOSC(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ IInterface* arg2,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallbackStub* callback)
{
    SomeArgs* argObj = new SomeArgs(arg2, NULL, callback, seq);
    return new Message(what, arg1, 0, argObj);
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageIO(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ IInterface* arg2)
{
    SomeArgs* argObj = new SomeArgs(arg2);
    return new Message(what, arg1, 0, argObj);
}

IInputConnectionWrapper::Message* IInputConnectionWrapper::ObtainMessageOO(
    /* [in] */ Int32 what,
    /* [in] */ const String& arg1,
    /* [in] */ IInterface* arg2)
{
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(arg1, (ICharSequence**) &text);

    SomeArgs* argObj = new SomeArgs(text, arg2);
    return new Message(what, 0, 0, argObj);
}