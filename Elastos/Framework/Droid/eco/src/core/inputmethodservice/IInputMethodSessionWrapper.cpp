
#include "inputmethodservice/IInputMethodSessionWrapper.h"

CString IInputMethodSessionWrapper::TAG = "InputMethodWrapper";
const Boolean IInputMethodSessionWrapper::DEBUG;

const Int32 IInputMethodSessionWrapper::DO_FINISH_INPUT;
const Int32 IInputMethodSessionWrapper::DO_DISPLAY_COMPLETIONS;
const Int32 IInputMethodSessionWrapper::DO_UPDATE_EXTRACTED_TEXT;
const Int32 IInputMethodSessionWrapper::DO_DISPATCH_KEY_EVENT;
const Int32 IInputMethodSessionWrapper::DO_DISPATCH_TRACKBALL_EVENT;
const Int32 IInputMethodSessionWrapper::DO_UPDATE_SELECTION;
const Int32 IInputMethodSessionWrapper::DO_UPDATE_CURSOR;
const Int32 IInputMethodSessionWrapper::DO_APP_PRIVATE_COMMAND;
const Int32 IInputMethodSessionWrapper::DO_TOGGLE_SOFT_INPUT;
const Int32 IInputMethodSessionWrapper::DO_FINISH_SESSION;


IInputMethodSessionWrapper::InputMethodEventCallbackWrapper::InputMethodEventCallbackWrapper(
	/* [in] */ IInputMethodCallbackStub* cb):
	mCb(cb)
{

}

PInterface IInputMethodSessionWrapper::InputMethodEventCallbackWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ILocalInputMethodSessionEventCallback == riid) {
        return (ILocalInputMethodSessionEventCallback *)this;
    }

    return NULL;
}

UInt32 IInputMethodSessionWrapper::InputMethodEventCallbackWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 IInputMethodSessionWrapper::InputMethodEventCallbackWrapper::Release()
{
    return ElRefBase::Release();
}

ECode IInputMethodSessionWrapper::InputMethodEventCallbackWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(ILocalInputMethodSessionEventCallback*)this) {
        *pIID = EIID_ILocalInputMethodSessionEventCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode IInputMethodSessionWrapper::InputMethodEventCallbackWrapper::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    // try {
    return mCb->FinishedEvent(seq, handled);
    // } catch (RemoteException e) {
    // }
}

IInputMethodSessionWrapper::IInputMethodSessionWrapper(
    /* [in] */ IContext* context,
    /* [in] */ ILocalInputMethodSession* inputMethodSession):
    mInputMethodSession(inputMethodSession)
{
    //mCaller = new HandlerCaller(context, this);
}

PInterface IInputMethodSessionWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputMethodSessionStub == riid) {
        return (IInputMethodSessionStub *)this;
    }

    return NULL;
}

UInt32 IInputMethodSessionWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 IInputMethodSessionWrapper::Release()
{
    return ElRefBase::Release();
}

ECode IInputMethodSessionWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IInputMethodSessionStub*)this) {
        *pIID = EIID_IInputMethodSessionStub;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

AutoPtr<ILocalInputMethodSession> IInputMethodSessionWrapper::GetInternalInputMethodSession()
{
    return mInputMethodSession;
}

void IInputMethodSessionWrapper::ExecuteMessage(
    /* [in] */ /*Message msg*/)
{
    // switch (msg.what) {
    //     case DO_FINISH_INPUT:
    //         mInputMethodSession.finishInput();
    //         return;
    //     case DO_DISPLAY_COMPLETIONS:
    //         mInputMethodSession.displayCompletions((CompletionInfo[])msg.obj);
    //         return;
    //     case DO_UPDATE_EXTRACTED_TEXT:
    //         mInputMethodSession.updateExtractedText(msg.arg1,
    //                 (ExtractedText)msg.obj);
    //         return;
    //     case DO_DISPATCH_KEY_EVENT: {
    //         HandlerCaller.SomeArgs args = (HandlerCaller.SomeArgs)msg.obj;
    //         mInputMethodSession.dispatchKeyEvent(msg.arg1,
    //                 (KeyEvent)args.arg1,
    //                 new InputMethodEventCallbackWrapper(
    //                         (IInputMethodCallbackStub)args.arg2));
    //         mCaller.recycleArgs(args);
    //         return;
    //     }
    //     case DO_DISPATCH_TRACKBALL_EVENT: {
    //         HandlerCaller.SomeArgs args = (HandlerCaller.SomeArgs)msg.obj;
    //         mInputMethodSession.dispatchTrackballEvent(msg.arg1,
    //                 (MotionEvent)args.arg1,
    //                 new InputMethodEventCallbackWrapper(
    //                         (IInputMethodCallbackStub)args.arg2));
    //         mCaller.recycleArgs(args);
    //         return;
    //     }
    //     case DO_UPDATE_SELECTION: {
    //         HandlerCaller.SomeArgs args = (HandlerCaller.SomeArgs)msg.obj;
    //         mInputMethodSession.updateSelection(args.argi1, args.argi2,
    //                 args.argi3, args.argi4, args.argi5, args.argi6);
    //         mCaller.recycleArgs(args);
    //         return;
    //     }
    //     case DO_UPDATE_CURSOR: {
    //         mInputMethodSession.updateCursor((Rect)msg.obj);
    //         return;
    //     }
    //     case DO_APP_PRIVATE_COMMAND: {
    //         HandlerCaller.SomeArgs args = (HandlerCaller.SomeArgs)msg.obj;
    //         mInputMethodSession.appPrivateCommand((String)args.arg1,
    //                 (Bundle)args.arg2);
    //         mCaller.recycleArgs(args);
    //         return;
    //     }
    //     case DO_TOGGLE_SOFT_INPUT: {
    //         mInputMethodSession.toggleSoftInput(msg.arg1, msg.arg2);
    //         return;
    //     }
    //     case DO_FINISH_SESSION: {
    //         mInputMethodSession = null;
    //         return;
    //     }
    // }
    // Log.w(TAG, "Unhandled message code: " + msg.what);
}

ECode IInputMethodSessionWrapper::FinishInput()
{
	//TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessage(DO_FINISH_INPUT));
    return mInputMethodSession->FinishInput();
}

ECode IInputMethodSessionWrapper::DisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageO(
    //         DO_DISPLAY_COMPLETIONS, completions));
   	return mInputMethodSession->DisplayCompletions(completions);
}

ECode IInputMethodSessionWrapper::UpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIO(
    //         DO_UPDATE_EXTRACTED_TEXT, token, text));

    return mInputMethodSession->UpdateExtractedText(token, text);
}

ECode IInputMethodSessionWrapper::DispatchKeyEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IKeyEvent* event,
    /* [in] */ IInputMethodCallbackStub* callback)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIOO(DO_DISPATCH_KEY_EVENT, seq,
    //         event, callback));
    return mInputMethodSession->DispatchKeyEvent(seq, event,
            new InputMethodEventCallbackWrapper(callback));
}

ECode IInputMethodSessionWrapper::DispatchTrackballEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* event,
    /* [in] */ IInputMethodCallbackStub* callback)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIOO(DO_DISPATCH_TRACKBALL_EVENT, seq,
    //         event, callback));

    return mInputMethodSession->DispatchTrackballEvent(seq, event,
            new InputMethodEventCallbackWrapper(callback));
}

ECode IInputMethodSessionWrapper::UpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIIIIII(DO_UPDATE_SELECTION,
    //         oldSelStart, oldSelEnd, newSelStart, newSelEnd,
    //         candidatesStart, candidatesEnd));

    return mInputMethodSession->UpdateSelection(oldSelStart, oldSelEnd,
    		newSelStart, newSelEnd, candidatesStart, candidatesEnd);
}

ECode IInputMethodSessionWrapper::UpdateCursor(
    /* [in] */ IRect* newCursor)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_UPDATE_CURSOR,
    //         newCursor));

    return mInputMethodSession->UpdateCursor(newCursor);
}

ECode IInputMethodSessionWrapper::AppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
	//TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessageOO(DO_APP_PRIVATE_COMMAND, action, data));

    return mInputMethodSession->AppPrivateCommand(action, data);
}

ECode IInputMethodSessionWrapper::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
	//TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessageII(DO_TOGGLE_SOFT_INPUT, showFlags, hideFlags));
    return mInputMethodSession->ToggleSoftInput(showFlags, hideFlags);
}

ECode IInputMethodSessionWrapper::FinishSession()
{
	//TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessage(DO_FINISH_SESSION));
    mInputMethodSession = NULL;
    return NOERROR;
}

ECode IInputMethodSessionWrapper::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
