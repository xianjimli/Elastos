
#include "view/inputmethod/CLocalInputMethodManager.h"
#include "view/inputmethod/CEditorInfo.h"
#include "view/inputmethod/CBaseInputConnection.h"
#include "view/CInputBindResult.h"
#include "os/CApartment.h"
#include "graphics/CRect.h"


CString CLocalInputMethodManager::TAG = "InputMethodManager";
AutoPtr<ILocalInputMethodManager> CLocalInputMethodManager::mInstance;
const Int32 CLocalInputMethodManager::MSG_DUMP;
const Int32 CLocalInputMethodManager::MSG_BIND;
const Int32 CLocalInputMethodManager::MSG_UNBIND;
const Int32 CLocalInputMethodManager::MSG_SET_ACTIVE;
Mutex CLocalInputMethodManager::sStaticInstanceLock;
Mutex CLocalInputMethodManager::sStaticHandlerLock;

CLocalInputMethodManager::ControlledInputConnectionWrapper::ControlledInputConnectionWrapper(
    /* [in] */ IApartment* mainLooper,
    /* [in] */ IInputConnection* conn,
    /* [in] */ CLocalInputMethodManager* host):
    IInputConnectionWrapper(mainLooper, conn),
    mHost(host)
{

}

Boolean CLocalInputMethodManager::ControlledInputConnectionWrapper::IsActive()
{
	return mHost->mActive;
}

PInterface CLocalInputMethodManager::ControlledInputConnectionWrapper::Probe(
    /* [in] */ REIID riid)
{
    return IInputConnectionWrapper::Probe(riid);
}

UInt32 CLocalInputMethodManager::ControlledInputConnectionWrapper::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 CLocalInputMethodManager::ControlledInputConnectionWrapper::Release()
{
	return ElRefBase::Release();
}

ECode CLocalInputMethodManager::ControlledInputConnectionWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
	return IInputConnectionWrapper::GetInterfaceID(pObject, pIID);
}

CLocalInputMethodManager::_InputMethodClientStub::_InputMethodClientStub(
    /* [in] */ CLocalInputMethodManager* host):
    mHost(host)
{

}

CLocalInputMethodManager::_InputMethodClientStub::~_InputMethodClientStub()
{

}

ECode CLocalInputMethodManager::_InputMethodClientStub::SetUsingInputMethod(
    /* [in] */ Boolean state)
{
	return NOERROR;
}

ECode CLocalInputMethodManager::_InputMethodClientStub::OnBindMethod(
    /* [in] */ IInputBindResult* res)
{
    assert(mHost != NULL && mHost->mH != NULL);
    void (STDCALL CLocalInputMethodManager::*pHandlerFunc)(IInputBindResult*);
    pHandlerFunc = &CLocalInputMethodManager::HandleMsgBind;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)res);

    return mHost->mH->PostCppCallbackDelayed((Handle32)mHost, *(Handle32*)&pHandlerFunc,
        params, 0, 0);
}

ECode CLocalInputMethodManager::_InputMethodClientStub::OnUnbindMethod(
    /* [in] */ Int32 sequence)
{
    assert(mHost != NULL && mHost->mH != NULL);
    void (STDCALL CLocalInputMethodManager::*pHandlerFunc)(Int32);
    pHandlerFunc = &CLocalInputMethodManager::HandleMsgUnBind;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(sequence);

    return mHost->mH->PostCppCallbackDelayed((Handle32)mHost, *(Handle32*)&pHandlerFunc,
        params, 0, 0);
}

ECode CLocalInputMethodManager::_InputMethodClientStub::SetActive(
    /* [in] */ Boolean active)
{
    assert(mHost != NULL && mHost->mH != NULL);
    void (STDCALL CLocalInputMethodManager::*pHandlerFunc)(Int32);
    pHandlerFunc = &CLocalInputMethodManager::HandleMsgSetActive;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);

    Int32 data = active ? 1 : 0;
    params->WriteInt32(data);

    return mHost->mH->PostCppCallbackDelayed((Handle32)mHost, *(Handle32*)&pHandlerFunc,
        params, 0, 0);
}

PInterface CLocalInputMethodManager::_InputMethodClientStub::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputMethodClientStub == riid) {
        return (IInputMethodClientStub *)this;
    }

    return NULL;
}

UInt32 CLocalInputMethodManager::_InputMethodClientStub::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 CLocalInputMethodManager::_InputMethodClientStub::Release()
{
	return ElRefBase::Release();
}

ECode CLocalInputMethodManager::_InputMethodClientStub::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IInputMethodClientStub*)this) {
        *pIID = EIID_IInputMethodClientStub;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

CLocalInputMethodManager::CLocalInputMethodManager():
    mActive(FALSE),
    mHasBeenInactive(TRUE),

    //TODO, add
    mFullscreenMode(FALSE),
    mNextServedNeedsStart(FALSE),
    mServedConnecting(FALSE),
    mCursorSelStart(0),
    mCursorSelEnd(0),
    mCursorCandStart(0),
    mCursorCandEnd(0),
    mBindSequence(0)
{
    CRect::New((IRect**) &mTmpCursorRect);
    CRect::New((IRect**) &mCursorRect);
}

ILocalInputMethodManager* CLocalInputMethodManager::GetInstance(
    /* [in] */ IContext* context)
{
    assert(0);
    //TODO
    //return GetInstance(context.getMainLooper());
    return GetInstanceEx(NULL);
}

ILocalInputMethodManager* CLocalInputMethodManager::GetInstanceEx(
    /* [in] */ IApartment* mainLooper)
{
    Mutex::Autolock lock(sStaticInstanceLock);

    if (mInstance != NULL) {
        return mInstance;
    }

    AutoPtr<IServiceManager> sm;
    GetServiceManager((IServiceManager**)&sm);

    AutoPtr<IBinder> b;
    sm->GetService(String(Context_INPUT_METHOD_SERVICE), (IInterface**) &b);

    assert(0);
    //TODO
    //IInputMethodManagerStub service = IInputMethodManagerStub.Stub.asInterface(b);
    // mInstance = new InputMethodManager(service, mainLooper);

    AutoPtr<IInputMethodManagerStub> service;
    CLocalInputMethodManager::New(service, mainLooper, (ILocalInputMethodManager**) &mInstance);

    return mInstance;
}

ILocalInputMethodManager* CLocalInputMethodManager::PeekInstance()
{
    return mInstance;
}

ECode CLocalInputMethodManager::GetClient(
    /* [out] */ IInputMethodClientStub** client)
{
    VALIDATE_NOT_NULL(client);
    *client = mClient;
    if (*client != NULL) {
        (*client)->AddRef();
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::GetInputContext(
    /* [out] */ IInputContextStub** context)
{
    VALIDATE_NOT_NULL(context);
    *context = mIInputContext;
    if (*context != NULL) {
        (*context)->AddRef();
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::GetInputMethodList(
    /* [in] */ IObjectContainer** infos)
{
    assert(mService != NULL);

    return mService->GetInputMethodList(infos);
}

ECode CLocalInputMethodManager::GetEnabledInputMethodList(
    /* [out] */ IObjectContainer** list)
{
    assert(mService != NULL);

    return mService->GetEnabledInputMethodList(list);
}

ECode CLocalInputMethodManager::ShowStatusIcon(
    /* [in] */ IBinder* imeToken,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 iconId)
{
    assert(mService != NULL);

    return mService->UpdateStatusIcon(imeToken, packageName, iconId);
}

ECode CLocalInputMethodManager::HideStatusIcon(
    /* [in] */ IBinder* imeToken)
{
    assert(mService != NULL);

    //return mService->UpdateStatusIcon(imeToken, NULL, 0);
    return mService->UpdateStatusIcon(imeToken, String(""), 0);
}

ECode CLocalInputMethodManager::SetFullscreenMode(
    /* [in] */ Boolean fullScreen)
{
    mFullscreenMode = fullScreen;
    return NOERROR;
}

ECode CLocalInputMethodManager::IsFullscreenMode(
    /* [out] */ Boolean* isfull)
{
    assert(isfull != NULL);
    *isfull = mFullscreenMode;

    return NOERROR;
}

ECode CLocalInputMethodManager::IsActive(
    /* [in] */ IView* view,
    /* [out] */ Boolean* active)
{
    assert(active != NULL);
    CheckFocus();

    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    mServedView->CheckInputConnectionProxy(view, &check);

    *active = (mServedView.Get() == view || (mServedView != NULL && check)) && mCurrentTextBoxAttribute.Get() != NULL;

    return NOERROR;
}

ECode CLocalInputMethodManager::IsActiveEx(
    /* [out] */ Boolean* active)
{
    assert(active != NULL);
    CheckFocus();

    Mutex::Autolock lock(sStaticHandlerLock);
    *active = mServedView != NULL && mCurrentTextBoxAttribute != NULL;

    return NOERROR;
}

ECode CLocalInputMethodManager::IsAcceptingText(
    /* [out] */ Boolean* isAccepting)
{
    assert(isAccepting != NULL);
    CheckFocus();

    *isAccepting = mServedInputConnection != NULL;
    return NOERROR;
}

ECode CLocalInputMethodManager::ClearBindingLocked()
{
    ClearConnectionLocked();
    mBindSequence = -1;
    mCurId = NULL;
    mCurMethod = NULL;

    return NOERROR;
}

ECode CLocalInputMethodManager::ReportFinishInputConnection(
    /* [in] */ IInputConnection* ic)
{
    if (mServedInputConnection.Get() != ic) {
        Boolean ret = FALSE;
        return ic->FinishComposingText(&ret);
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::DisplayCompletions(
    /* [in] */ IView* view,
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    CheckFocus();
    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    mServedView->CheckInputConnectionProxy(view, &check);

    if (mServedView.Get() != view && (mServedView == NULL || !check)) {
        return NOERROR;
    }

    mCompletions = completions;
    if (mCurMethod != NULL) {
        return mCurMethod->DisplayCompletions(mCompletions);
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::UpdateExtractedText(
    /* [in] */ IView* view,
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    CheckFocus();
    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    mServedView->CheckInputConnectionProxy(view, &check);

    if (mServedView.Get() != view && (mServedView == NULL || !check)) {
        return NOERROR;
    }

    if (mCurMethod != NULL) {
        return mCurMethod->UpdateExtractedText(token, text);
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::ShowSoftInput(
    /* [in] */ IView* view,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* show)
{
    return ShowSoftInputEx(view, flags, NULL, show);
}

ECode CLocalInputMethodManager::ShowSoftInputEx(
    /* [in] */ IView* view,
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver,
    /* [out] */ Boolean* show)
{
    CheckFocus();
    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    mServedView->CheckInputConnectionProxy(view, &check);
    if (mServedView.Get() != view && (mServedView == NULL || !check)) {
        *show = FALSE;
        return NOERROR;
    }

    return mService->ShowSoftInput(mClient, flags, resultReceiver, show);
}

ECode CLocalInputMethodManager::ShowSoftInputUnchecked(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    // try {
    Boolean ret = FALSE;
    return mService->ShowSoftInput(mClient, flags, resultReceiver, &ret);
    // } catch (RemoteException e) {
    // }
}

ECode CLocalInputMethodManager::HideSoftInputFromWindow(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* hide)
{
    return HideSoftInputFromWindowEx(windowToken, flags, NULL, hide);
}

ECode CLocalInputMethodManager::HideSoftInputFromWindowEx(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver,
    /* [out] */ Boolean* hide)
{
    CheckFocus();
    Mutex::Autolock lock(sStaticHandlerLock);

    AutoPtr<IBinder> token;
    mServedView->GetWindowToken((IBinder**) &token);
    if (mServedView == NULL || token.Get() != windowToken) {
        *hide = FALSE;
        return NOERROR;
    }

    return mService->HideSoftInput(mClient, flags, resultReceiver, hide);
}

ECode CLocalInputMethodManager::ToggleSoftInputFromWindow(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    Mutex::Autolock lock(sStaticHandlerLock);

    AutoPtr<IBinder> token;
    mServedView->GetWindowToken((IBinder**) &token);
    if (mServedView == NULL || token.Get() != windowToken) {
        return NOERROR;
    }

    if (mCurMethod != NULL) {
        return mCurMethod->ToggleSoftInput(showFlags, hideFlags);
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    if (mCurMethod != NULL) {
        return mCurMethod->ToggleSoftInput(showFlags, hideFlags);
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::RestartInput(
    /* [in] */ IView* view)
{
    CheckFocus();
    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    mServedView->CheckInputConnectionProxy(view, &check);

    if (mServedView.Get() != view && (mServedView == NULL || !check)) {
        return NOERROR;
    }

    mServedConnecting = TRUE;
    StartInputInner();

    return NOERROR;
}

ECode CLocalInputMethodManager::WindowDismissed(
    /* [in] */ IBinder* appWindowToken)
{
    CheckFocus();
    Mutex::Autolock lock(sStaticHandlerLock);

    AutoPtr<IBinder> token;
    mServedView->GetWindowToken((IBinder**) &token);

    if (mServedView != NULL && token.Get() == appWindowToken) {
        FinishInputLocked();
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::FocusIn(
    /* [in] */ IView* view)
{
    Mutex::Autolock lock(sStaticHandlerLock);
    FocusInLocked(view);

    return NOERROR;
}

ECode CLocalInputMethodManager::FocusOut(
    /* [in] */ IView* view)
{
    Mutex::Autolock lock(sStaticHandlerLock);

    // if (DEBUG) Log.v(TAG, "focusOut: " + view
    //         + " mServedView=" + mServedView
    //         + " winFocus=" + view.hasWindowFocus());
    if (mNextServedView.Get() != view) {
        // The following code would auto-hide the IME if we end up
        // with no more views with focus.  This can happen, however,
        // whenever we go into touch mode, so it ends up hiding
        // at times when we don't really want it to.  For now it
        // seems better to just turn it all off.
        Boolean focus = FALSE;
        view->HasWindowFocus(&focus);
        if (FALSE && focus) {
            mNextServedView = NULL;
            ScheduleCheckFocusLocked(view);
        }
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::CheckFocus()
{
    // This is called a lot, so short-circuit before locking.
    if (mServedView == mNextServedView && !mNextServedNeedsStart) {
        return NOERROR;
    }

    AutoPtr<IInputConnection> ic;

    {
        Mutex::Autolock lock(sStaticHandlerLock);
        if (mServedView == mNextServedView && !mNextServedNeedsStart) {
            return NOERROR;
        }
        // if (DEBUG) Log.v(TAG, "checkFocus: view=" + mServedView
        //         + " next=" + mNextServedView
        //         + " restart=" + mNextServedNeedsStart);

        mNextServedNeedsStart = FALSE;
        if (mNextServedView == NULL) {
            FinishInputLocked();
            // In this case, we used to have a focused view on the window,
            // but no longer do.  We should make sure the input method is
            // no longer shown, since it serves no purpose.
            CloseCurrentInput();
            return NOERROR;
        }

        ic = mServedInputConnection;

        mServedView = mNextServedView;
        mCurrentTextBoxAttribute = NULL;
        mCompletions = NULL;
        mServedConnecting = TRUE;
    }

    if (ic != NULL) {
        Boolean ret = FALSE;
        ic->FinishComposingText(&ret);
    }

    StartInputInner();

    return NOERROR;
}

ECode CLocalInputMethodManager::OnWindowFocus(
    /* [in] */ IView* rootView,
    /* [in] */ IView* focusedView,
    /* [in] */ Int32 softInputMode,
    /* [in] */ Boolean first,
    /* [in] */ Int32 windowFlags)
{
    {
        Mutex::Autolock lock(sStaticHandlerLock);
        // if (DEBUG) Log.v(TAG, "onWindowFocus: " + focusedView
        //         + " softInputMode=" + softInputMode
        //         + " first=" + first + " flags=#"
        //         + Integer.toHexString(windowFlags));
        if (mHasBeenInactive) {
            // if (DEBUG) Log.v(TAG, "Has been inactive!  Starting fresh");
            mHasBeenInactive = FALSE;
            mNextServedNeedsStart = TRUE;
        }

        FocusInLocked(focusedView != NULL ? focusedView : rootView);
    }

    CheckFocus();

    {
        Mutex::Autolock lock(sStaticHandlerLock);

        Boolean checkIsTextEditor = FALSE;
        assert(focusedView != NULL);
        focusedView->OnCheckIsTextEditor(&checkIsTextEditor);

        Boolean isTextEditor = focusedView != NULL && checkIsTextEditor;

        assert(rootView != NULL);
        AutoPtr<IBinder> token;
        rootView->GetWindowToken((IBinder**) &token);

        mService->WindowGainedFocus(mClient, token, focusedView != NULL,
            isTextEditor, softInputMode, first, windowFlags);
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::StartGettingWindowFocus(
    /* [in] */ IView* rootView)
{
    Mutex::Autolock lock(sStaticHandlerLock);
    mCurRootView = rootView;

    return NOERROR;
}

ECode CLocalInputMethodManager::UpdateSelection(
    /* [in] */ IView* view,
    /* [in] */ Int32 selStart,
    /* [in] */ Int32 selEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    CheckFocus();

    {
        Mutex::Autolock lock(sStaticHandlerLock);

        Boolean check = FALSE;
        mServedView->CheckInputConnectionProxy(view, &check);

        if ((mServedView.Get() != view && (mServedView == NULL || !check))
                || mCurrentTextBoxAttribute == NULL || mCurMethod == NULL)
        {
            return NOERROR;
        }

        if (mCursorSelStart != selStart || mCursorSelEnd != selEnd
                || mCursorCandStart != candidatesStart
                || mCursorCandEnd != candidatesEnd)
        {
            // if (DEBUG) Log.d(TAG, "updateSelection");

            // try {
                // if (DEBUG) Log.v(TAG, "SELECTION CHANGE: " + mCurMethod);
            mCurMethod->UpdateSelection(mCursorSelStart, mCursorSelEnd,
                    selStart, selEnd, candidatesStart, candidatesEnd);

            mCursorSelStart = selStart;
            mCursorSelEnd = selEnd;
            mCursorCandStart = candidatesStart;
            mCursorCandEnd = candidatesEnd;
            // } catch (RemoteException e) {
            //     Log.w(TAG, "IME died: " + mCurId, e);
            // }
        }
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::IsWatchingCursor(
    /* [in] */ IView* view,
    /* [out] */ Boolean* isWatching)
{
    *isWatching = FALSE;
    return NOERROR;
}

ECode CLocalInputMethodManager::UpdateCursor(
    /* [in] */ IView* view,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    CheckFocus();

    {
        Mutex::Autolock lock(sStaticHandlerLock);

        Boolean check = FALSE;
        mServedView->CheckInputConnectionProxy(view, &check);

        if ((mServedView.Get() != view && (mServedView == NULL || !check))
                || mCurrentTextBoxAttribute == NULL || mCurMethod == NULL)
        {
            return NOERROR;
        }

        mTmpCursorRect->Set(left, top, right, bottom);

        Boolean equal = FALSE;
        assert(mCursorRect != NULL);
        mCursorRect->Equals(mTmpCursorRect, &equal);

        if (!equal) {
            // if (DEBUG) Log.d(TAG, "updateCursor");

            // try {
                // if (DEBUG) Log.v(TAG, "CURSOR CHANGE: " + mCurMethod);
            mCurMethod->UpdateCursor(mTmpCursorRect);
            mCursorRect->SetEx(mTmpCursorRect);
            // } catch (RemoteException e) {
            //     Log.w(TAG, "IME died: " + mCurId, e);
            // }
        }
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::SendAppPrivateCommand(
    /* [in] */ IView* view,
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    CheckFocus();

    {
        Mutex::Autolock lock(sStaticHandlerLock);

        Boolean check = FALSE;
        mServedView->CheckInputConnectionProxy(view, &check);

        if ((mServedView.Get() != view && (mServedView == NULL || !check))
                || mCurrentTextBoxAttribute == NULL || mCurMethod == NULL)
        {
            return NOERROR;
        }

        // try {
            // if (DEBUG) Log.v(TAG, "APP PRIVATE COMMAND " + action + ": " + data);
        mCurMethod->AppPrivateCommand(action, data);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        // }
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::SetInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ const String& id)
{
    return mService->SetInputMethod(token, id);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CLocalInputMethodManager::HideSoftInputFromInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 flags)
{
    return mService->HideMySoftInput(token, flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CLocalInputMethodManager::ShowSoftInputFromInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 flags)
{
    return mService->ShowMySoftInput(token, flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CLocalInputMethodManager::DispatchKeyEvent(
    /* [in] */ IContext* context,
    /* [in] */ Int32 seq,
    /* [in] */ IKeyEvent* key,
    /* [in] */ IInputMethodCallback* callback)
{
    {
        Mutex::Autolock lock(sStaticHandlerLock);
        // if (DEBUG) Log.d(TAG, "dispatchKeyEvent");

        if (mCurMethod == NULL) {
            return callback->FinishedEvent(seq, false);
            // } catch (RemoteException e) {
            // }
        }

        assert(key != NULL);
        Int32 action = 0;
        key->GetAction(&action);

        Int32 keyCode = 0;
        key->GetKeyCode(&keyCode);

        if (action == KeyEvent_ACTION_DOWN && keyCode == KeyEvent_KEYCODE_SYM) {
            ShowInputMethodPicker();

            return callback->FinishedEvent(seq, true);
            // } catch (RemoteException e) {
            // }
        }

        // try {
        //     if (DEBUG) Log.v(TAG, "DISPATCH KEY: " + mCurMethod);
            ECode ec = mCurMethod->DispatchKeyEvent(seq, key, callback);

            //TODO
            if (ec != NOERROR) {
                return callback->FinishedEvent(seq, FALSE);
            }
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId + " dropping: " + key, e);
        //     try {
        //         callback.finishedEvent(seq, false);
        //     } catch (RemoteException ex) {
        //     }
        // }
    }

    return NOERROR;
}

ECode CLocalInputMethodManager::ShowInputMethodPicker()
{
    Mutex::Autolock lock(sStaticHandlerLock);

    assert(mService != NULL);
    return mService->ShowInputMethodPickerFromClient(mClient);
    // } catch (RemoteException e) {
    //     Log.w(TAG, "IME died: " + mCurId, e);
    // }
}

ECode CLocalInputMethodManager::constructor(
    /* [in] */ IInputMethodManagerStub* service,
    /* [in] */ IApartment* looper)
{
    mClient = new _InputMethodClientStub(this);
    CBaseInputConnection::New(this, FALSE, (IInputConnection**) &mDummyInputConnection);

    mService = service;
    mMainLooper = looper;
    // mH = new H(looper);

    //TODO: The argument:TRUE
    CApartment::New(TRUE, (IApartment**)&mH);
    assert(mH != NULL);

    mH->Start(ApartmentAttr_New);

    mIInputContext = new ControlledInputConnectionWrapper(looper,
            mDummyInputConnection, this);

    if (mInstance == NULL) {
        mInstance = this;
    }

    return NOERROR;
}

void CLocalInputMethodManager::ClearConnectionLocked()
{
    mCurrentTextBoxAttribute = NULL;
    mServedInputConnection = NULL;
}

void CLocalInputMethodManager::FinishInputLocked()
{
    mNextServedView = NULL;
    if (mServedView != NULL) {
        //if (DEBUG) Log.v(TAG, "FINISH INPUT: " + mServedView);

        if (mCurrentTextBoxAttribute != NULL) {
            assert(mService != NULL);
            mService->FinishInput(mClient);
        }

        if (mServedInputConnection != NULL) {
            // We need to tell the previously served view that it is no
            // longer the input target, so it can reset its state.  Schedule
            // this call on its window's Handler so it will be on the correct
            // thread and outside of our lock.
            AutoPtr<IApartment> vh;
            mServedView->GetHandler((IApartment**) &vh);

            if (vh != NULL) {
                // This will result in a call to reportFinishInputConnection()
                // below.
                ECode (STDCALL CLocalInputMethodManager::*pHandlerFunc)(IInputConnection* msg);
                pHandlerFunc = &CLocalInputMethodManager::ReportFinishInputConnection;

                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);
                params->WriteInt32Ptr(*(Handle32*)mServedInputConnection.Get());
                vh->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);

                // vh.sendMessage(vh.obtainMessage(ViewRoot.FINISH_INPUT_CONNECTION,
                //         mServedInputConnection));
            }
        }

        mServedView = NULL;
        mCompletions = NULL;
        mServedConnecting = false;
        ClearConnectionLocked();
    }
}

void CLocalInputMethodManager::StartInputInner()
{
    // final View view;
    AutoPtr<IView> view;

    {
        Mutex::Autolock lock(sStaticHandlerLock);
        view = mServedView;

        // Make sure we have a window token for the served view.
        // if (DEBUG) Log.v(TAG, "Starting input: view=" + view);
        if (view == NULL) {
            // if (DEBUG) Log.v(TAG, "ABORT input: no served view!");
            return;
        }
    }


    // Now we need to get an input connection from the served view.
    // This is complicated in a couple ways: we can't be holding our lock
    // when calling out to the view, and we need to make sure we call into
    // the view on the same thread that is driving its view hierarchy.
    AutoPtr<IApartment> vh;
    mServedView->GetHandler((IApartment**) &vh);
    if (vh == NULL) {
        // If the view doesn't have a handler, something has changed out
        // from under us, so just bail.
        // if (DEBUG) Log.v(TAG, "ABORT input: no handler for view!");
        return;
    }

    //TODO
    // if (vh.getLooper() != Looper.myLooper()) {
    AutoPtr<IApartment> tmpLooper;
    CApartment::GetDefaultApartment((IApartment**) &tmpLooper);
    if (vh != tmpLooper) {
        // The view is running on a different thread than our own, so
        // we need to reschedule our work for over there.
        // if (DEBUG) Log.v(TAG, "Starting input: reschedule to view thread");

        // vh.post(new Runnable() {
        //     public void run() {
        //         StartInputInner();
        //     }
        // });
        void (STDCALL CLocalInputMethodManager::*pHandlerFunc)();
        pHandlerFunc = &CLocalInputMethodManager::StartInputInner;

        vh->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc,
            NULL, 0, 0);

        return;
    }

    // Okay we are now ready to call into the served view and have it
    // do its stuff.
    // Life is good: let's hook everything up!
    AutoPtr<CEditorInfo> tba;
    CEditorInfo::NewByFriend((CEditorInfo**) &tba);
    assert(tba != NULL);

    AutoPtr<IContext> tmpCtx;
    view->GetContext((IContext**) &tmpCtx);
    assert(tmpCtx != NULL);
    // tmpCtx->GetPackageName(&tba->mPackageName);
    view->GetId(&tba->mFieldId);

    AutoPtr<IInputConnection> ic;
    view->OnCreateInputConnection(tba, (IInputConnection**) &ic);

    // if (DEBUG) Log.v(TAG, "Starting input: tba=" + tba + " ic=" + ic);

    {
        Mutex::Autolock lock(sStaticHandlerLock);
        // Now that we are locked again, validate that our state hasn't
        // changed.
        if (mServedView != view || !mServedConnecting) {
            // Something else happened, so abort.
            // if (DEBUG) Log.v(TAG,
            //         "Starting input: finished by someone else (view="
            //         + mServedView + " conn=" + mServedConnecting + ")");
            return;
        }

        // If we already have a text box, then this view is already
        // connected so we want to restart it.
        Boolean initial = mCurrentTextBoxAttribute == NULL;

        // Hook 'em up and let 'er rip.
        mCurrentTextBoxAttribute = (IEditorInfo*)tba->Probe(EIID_IEditorInfo);
        mServedConnecting = FALSE;
        mServedInputConnection = ic;
        AutoPtr<IInputContextStub> servedContext;
        if (ic != NULL) {
            mCursorSelStart = tba->mInitialSelStart;
            mCursorSelEnd = tba->mInitialSelEnd;
            mCursorCandStart = -1;
            mCursorCandEnd = -1;
            mCursorRect->SetEmpty();
            servedContext = new ControlledInputConnectionWrapper(vh, ic, this);
        } else {
            servedContext = NULL;
        }

        // try {
        // if (DEBUG) Log.v(TAG, "START INPUT: " + view + " ic="
        //         + ic + " tba=" + tba + " initial=" + initial);
        AutoPtr<IInputBindResult> tmpRes;
        mService->StartInput(mClient, servedContext, tba, initial,
                mCurMethod == NULL, (IInputBindResult**) &tmpRes);

        CInputBindResult* res = (CInputBindResult*)tmpRes.Get();
        // if (DEBUG) Log.v(TAG, "Starting input: Bind result=" + res);
        if (res != NULL) {
            if (res->mId != NULL) {
                mBindSequence = res->mSequence;
                mCurMethod = res->mMethod;
            } else {
                // This means there is no input method available.
                // if (DEBUG) Log.v(TAG, "ABORT input: no input method!");
                return;
            }
        }
        if (mCurMethod != NULL && mCompletions != NULL) {
            mCurMethod->DisplayCompletions(mCompletions);
        }
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        // }
    }
}

void CLocalInputMethodManager::FocusInLocked(
    /* [in] */ IView* view)
{
    // if (DEBUG) Log.v(TAG, "focusIn: " + view);

    AutoPtr<IView> rootView;
    view->GetRootView((IView**) &rootView);
    if (mCurRootView != rootView) {
        // This is a request from a window that isn't in the window with
        // IME focus, so ignore it.
        // if (DEBUG) Log.v(TAG, "Not IME target window, ignoring");
        return;
    }

    mNextServedView = view;
    ScheduleCheckFocusLocked(view);
}

void CLocalInputMethodManager::ScheduleCheckFocusLocked(
    /* [in] */ IView* view)
{
    AutoPtr<IApartment> vh;
    view->GetHandler((IApartment**) &vh);

    // if (vh != NULL && !vh.hasMessages(ViewRoot.CHECK_FOCUS)) {
    if (vh != NULL) {
        // This will result in a call to checkFocus() below.
        ECode (STDCALL CLocalInputMethodManager::*pHandlerFunc)();
        pHandlerFunc = &CLocalInputMethodManager::CheckFocus;

        vh->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

        // vh.sendMessage(vh.obtainMessage(ViewRoot.CHECK_FOCUS));
        vh->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc,
            NULL, 0, 0);
    }
}

void CLocalInputMethodManager::CloseCurrentInput()
{
    Boolean ret = FALSE;
    mService->HideSoftInput(mClient, InputMethodManager_HIDE_NOT_ALWAYS, NULL, &ret);
}

void CLocalInputMethodManager::DispatchTrackballEvent(
    /* [in] */ IContext* context,
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* motion,
    /* [in] */ IInputMethodCallback* callback)
{
    {
        Mutex::Autolock lock(sStaticHandlerLock);
        // if (DEBUG) Log.d(TAG, "dispatchTrackballEvent");

        if (mCurMethod == NULL || mCurrentTextBoxAttribute == NULL) {
            callback->FinishedEvent(seq, false);
            // } catch (RemoteException e) {
            // }
            // return;
        }

        // try {
        //     if (DEBUG) Log.v(TAG, "DISPATCH TRACKBALL: " + mCurMethod);
        ECode ec = mCurMethod->DispatchTrackballEvent(seq, motion, callback);
        if (ec != NOERROR) {
            callback->FinishedEvent(seq, FALSE);
        }
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId + " dropping trackball: " + motion, e);
        //     try {
        //         callback.finishedEvent(seq, false);
        //     } catch (RemoteException ex) {
        //     }
        // }
    }
}

void CLocalInputMethodManager::HandleMsgBind(
    /* [in] */ IInputBindResult* resObj)
{
    assert(resObj != NULL);

    {
        Mutex::Autolock lock(sStaticHandlerLock);

        CInputBindResult* res = (CInputBindResult*)resObj;
        if (mBindSequence < 0 || mBindSequence != res->mSequence) {
            // Log.w(TAG, "Ignoring onBind: cur seq=" + mBindSequence
            //         + ", given seq=" + res.sequence);
            return;
        }

        mCurMethod = res->mMethod;
        mCurId = res->mId;
        mBindSequence = res->mSequence;
    }

    StartInputInner();
}

void CLocalInputMethodManager::HandleMsgUnBind(
    /* [in] */ Int32 sequence)
{
    Mutex::Autolock lock(sStaticHandlerLock);

    if (mBindSequence == sequence) {
        if (FALSE) {
            assert(0);
            // XXX the server has already unbound!

            //TODO
            // if (mCurMethod != NULL && mCurrentTextBoxAttribute != NULL) {
            //     try {
            //         mCurMethod.finishInput();
            //     } catch (RemoteException e) {
            //         Log.w(TAG, "IME died: " + mCurId, e);
            //     }
            // }
        }

        ClearBindingLocked();

        // If we were actively using the last input method, then
        // we would like to re-connect to the next input method.
        Boolean focused = FALSE;
        mServedView->IsFocused(&focused);
        if (mServedView != NULL && focused) {
            mServedConnecting = TRUE;
        }
    }

    StartInputInner();
}

void CLocalInputMethodManager::HandleMsgSetActive(
    /* [in] */ Int32 flag)
{
    Boolean active = flag != 0;
    Mutex::Autolock lock(sStaticHandlerLock);

    mActive = active;
    mFullscreenMode = FALSE;
    if (!active) {
        // Some other client has starting using the IME, so note
        // that this happened and make sure our own editor's
        // state is reset.
        mHasBeenInactive = TRUE;
            // Note that finishComposingText() is allowed to run
            // even when we are not active.
        mIInputContext->FinishComposingText();
        // } catch (RemoteException e) {
        // }
    }
}
