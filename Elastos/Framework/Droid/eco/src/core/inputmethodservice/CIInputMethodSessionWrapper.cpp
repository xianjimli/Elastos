
#include "ext/frameworkdef.h"
#include "inputmethodservice/CIInputMethodSessionWrapper.h"
#include "os/CApartment.h"


CString CIInputMethodSessionWrapper::TAG = "CIInputMethodSessionWrapper";
const Boolean CIInputMethodSessionWrapper::DEBUG;

const Int32 CIInputMethodSessionWrapper::DO_FINISH_INPUT;
const Int32 CIInputMethodSessionWrapper::DO_DISPLAY_COMPLETIONS;
const Int32 CIInputMethodSessionWrapper::DO_UPDATE_EXTRACTED_TEXT;
const Int32 CIInputMethodSessionWrapper::DO_DISPATCH_KEY_EVENT;
const Int32 CIInputMethodSessionWrapper::DO_DISPATCH_TRACKBALL_EVENT;
const Int32 CIInputMethodSessionWrapper::DO_UPDATE_SELECTION;
const Int32 CIInputMethodSessionWrapper::DO_UPDATE_CURSOR;
const Int32 CIInputMethodSessionWrapper::DO_APP_PRIVATE_COMMAND;
const Int32 CIInputMethodSessionWrapper::DO_TOGGLE_SOFT_INPUT;
const Int32 CIInputMethodSessionWrapper::DO_FINISH_SESSION;


CIInputMethodSessionWrapper::InputMethodEventCallbackWrapper::InputMethodEventCallbackWrapper(
    /* [in] */ IInputMethodCallback* cb)
    : mCb(cb)
{}

PInterface CIInputMethodSessionWrapper::InputMethodEventCallbackWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ILocalInputMethodSessionEventCallback == riid) {
        return (ILocalInputMethodSessionEventCallback *)this;
    }

    return NULL;
}

UInt32 CIInputMethodSessionWrapper::InputMethodEventCallbackWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CIInputMethodSessionWrapper::InputMethodEventCallbackWrapper::Release()
{
    return ElRefBase::Release();
}

ECode CIInputMethodSessionWrapper::InputMethodEventCallbackWrapper::GetInterfaceID(
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

ECode CIInputMethodSessionWrapper::InputMethodEventCallbackWrapper::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    // try {
    return mCb->FinishedEvent(seq, handled);
    // } catch (RemoteException e) {
    // }
}

ECode CIInputMethodSessionWrapper::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILocalInputMethodSession* inputMethodSession)
{
    mInputMethodSession = inputMethodSession;
    //mCaller = new HandlerCaller(context, this);
    FAIL_RETURN(CApartment::GetMainApartment((IApartment**)&mCaller));
    return NOERROR;
}

AutoPtr<ILocalInputMethodSession> CIInputMethodSessionWrapper::GetInternalInputMethodSession()
{
    return mInputMethodSession;
}


ECode CIInputMethodSessionWrapper::FinishInput()
{
    //TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessage(DO_FINISH_INPUT));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)();
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleFinishInput;

    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
}

ECode CIInputMethodSessionWrapper::DisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageO(
    //         DO_DISPLAY_COMPLETIONS, completions));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)(ArrayOf<ICompletionInfo*>*);
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleDisplayCompletions;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteArrayOf((Handle32)completions);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodSessionWrapper::UpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIO(
    //         DO_UPDATE_EXTRACTED_TEXT, token, text));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)(Int32, IExtractedText*);
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleUpdateExtractedText;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(token);
    params->WriteInterfacePtr(text);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodSessionWrapper::DispatchKeyEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IKeyEvent* event,
    /* [in] */ IInputMethodCallback* callback)
{
    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIOO(DO_DISPATCH_KEY_EVENT, seq,
    //         event, callback));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)(Int32, IKeyEvent*, IInputMethodCallback*);
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleDispatchKeyEvent;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(seq);
    params->WriteInterfacePtr(event);
    params->WriteInterfacePtr(callback);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodSessionWrapper::DispatchTrackballEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* event,
    /* [in] */ IInputMethodCallback* callback)
{
    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIOO(DO_DISPATCH_TRACKBALL_EVENT, seq,
    //         event, callback));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)(Int32, IMotionEvent*, IInputMethodCallback*);
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleDispatchTrackballEvent;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(seq);
    params->WriteInterfacePtr(event);
    params->WriteInterfacePtr(callback);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodSessionWrapper::UpdateSelection(
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
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)(Int32, Int32, Int32, Int32, Int32, Int32);
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleUpdateSelection;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(oldSelStart);
    params->WriteInt32(oldSelEnd);
    params->WriteInt32(newSelStart);
    params->WriteInt32(newSelEnd);
    params->WriteInt32(candidatesStart);
    params->WriteInt32(candidatesEnd);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodSessionWrapper::UpdateCursor(
    /* [in] */ IRect* newCursor)
{
    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_UPDATE_CURSOR,
    //         newCursor));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)(IRect*);
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleUpdateCursor;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(newCursor);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodSessionWrapper::AppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    //TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessageOO(DO_APP_PRIVATE_COMMAND, action, data));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)(const String&, IBundle*);
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleAppPrivateCommand;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(action);
    params->WriteInterfacePtr(data);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodSessionWrapper::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    //TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessageII(DO_TOGGLE_SOFT_INPUT, showFlags, hideFlags));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)(Int32, Int32);
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleToggleSoftInput;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(showFlags);
    params->WriteInt32(hideFlags);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodSessionWrapper::FinishSession()
{
    //TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessage(DO_FINISH_SESSION));
    ECode (STDCALL CIInputMethodSessionWrapper::*pHandlerFunc)();
    pHandlerFunc = &CIInputMethodSessionWrapper::HandleFinishSession;

    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
}

ECode CIInputMethodSessionWrapper::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIInputMethodSessionWrapper::HandleFinishInput()
{
    return mInputMethodSession->FinishInput();
}

ECode CIInputMethodSessionWrapper::HandleDisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    return mInputMethodSession->DisplayCompletions(completions);
}

ECode CIInputMethodSessionWrapper::HandleUpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    return mInputMethodSession->UpdateExtractedText(token, text);
}

ECode CIInputMethodSessionWrapper::HandleDispatchKeyEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IKeyEvent* event,
    /* [in] */ IInputMethodCallback* callback)
{
    return mInputMethodSession->DispatchKeyEvent(seq, event,
            new InputMethodEventCallbackWrapper(callback));
}

ECode CIInputMethodSessionWrapper::HandleDispatchTrackballEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* event,
    /* [in] */ IInputMethodCallback* callback)
{
    return mInputMethodSession->DispatchTrackballEvent(seq, event,
            new InputMethodEventCallbackWrapper(callback));
}

ECode CIInputMethodSessionWrapper::HandleUpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    return mInputMethodSession->UpdateSelection(oldSelStart, oldSelEnd,
            newSelStart, newSelEnd, candidatesStart, candidatesEnd);
}

ECode CIInputMethodSessionWrapper::HandleUpdateCursor(
    /* [in] */ IRect* newCursor)
{
    return mInputMethodSession->UpdateCursor(newCursor);
}

ECode CIInputMethodSessionWrapper::HandleAppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return mInputMethodSession->AppPrivateCommand(action, data);
}

ECode CIInputMethodSessionWrapper::HandleToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    return mInputMethodSession->ToggleSoftInput(showFlags, hideFlags);
}

ECode CIInputMethodSessionWrapper::HandleFinishSession()
{
    mInputMethodSession = NULL;
    return NOERROR;
}
