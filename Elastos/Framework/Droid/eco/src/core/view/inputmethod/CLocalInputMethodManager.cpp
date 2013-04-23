
#include "view/inputmethod/CLocalInputMethodManager.h"
#include "view/inputmethod/CEditorInfo.h"
#include "view/inputmethod/CBaseInputConnection.h"
#include "view/CInputBindResult.h"
#include "view/inputmethod/CIInputMethodClient.h"
#include "view/inputmethod/CControlledInputConnectionWrapper.h"
#include "graphics/CRect.h"
#include "os/CApartment.h"


CString CLocalInputMethodManager::TAG = "InputMethodManager";
AutoPtr<ILocalInputMethodManager> CLocalInputMethodManager::sInstance;
const Int32 CLocalInputMethodManager::MSG_DUMP;
const Int32 CLocalInputMethodManager::MSG_BIND;
const Int32 CLocalInputMethodManager::MSG_UNBIND;
const Int32 CLocalInputMethodManager::MSG_SET_ACTIVE;
Mutex CLocalInputMethodManager::sStaticInstanceLock;
Mutex CLocalInputMethodManager::sStaticHandlerLock;


CLocalInputMethodManager::CLocalInputMethodManager()
    : mActive(FALSE)
    , mHasBeenInactive(TRUE)
    , mFullscreenMode(FALSE)
    , mNextServedNeedsStart(FALSE)
    , mServedConnecting(FALSE)
    , mCursorSelStart(0)
    , mCursorSelEnd(0)
    , mCursorCandStart(0)
    , mCursorCandEnd(0)
    , mBindSequence(0)
{
    CRect::New((IRect**)&mTmpCursorRect);
    CRect::New((IRect**)&mCursorRect);
    CIInputMethodClient::New((ILocalInputMethodManager*)this, (IInputMethodClient**)&mClient);
    CBaseInputConnection::New(this, FALSE, (IInputConnection**)&mDummyInputConnection);
}

ECode CLocalInputMethodManager::constructor(
    /* [in] */ IInputMethodManager* service,
    /* [in] */ IApartment* looper)
{
    mService = service;
    mMainLooper = looper;
    CControlledInputConnectionWrapper::New(looper, mDummyInputConnection,
            (ILocalInputMethodManager*)this, (IInputContext**)&mIInputContext);

    if (sInstance == NULL) {
        sInstance = this;
    }
    return NOERROR;
}

AutoPtr<ILocalInputMethodManager> CLocalInputMethodManager::GetInstance(
    /* [in] */ IContext* context)
{
    //TODO
    AutoPtr<IApartment> apartment;
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&apartment))
        && (apartment != NULL));
    return GetInstance(apartment/*context.getMainLooper()*/);
}

AutoPtr<ILocalInputMethodManager> CLocalInputMethodManager::GetInstance(
    /* [in] */ IApartment* mainLooper)
{
    Mutex::Autolock lock(sStaticInstanceLock);

    if (sInstance != NULL) {
        return sInstance;
    }
    AutoPtr<IServiceManager> sm;
    GetServiceManager((IServiceManager**)&sm);
    AutoPtr<IInputMethodManager> service;
    ASSERT_SUCCEEDED(sm->GetService(String(Context_INPUT_METHOD_SERVICE), (IInterface**)&service));
    ASSERT_SUCCEEDED(CLocalInputMethodManager::New(service, mainLooper, (ILocalInputMethodManager**)&sInstance));
    return sInstance;
}

AutoPtr<ILocalInputMethodManager> CLocalInputMethodManager::PeekInstance()
{
    return sInstance;
}

ECode CLocalInputMethodManager::GetClient(
    /* [out] */ IInputMethodClient** client)
{
    VALIDATE_NOT_NULL(client);
    *client = mClient;
    if (*client != NULL) {
        (*client)->AddRef();
    }
    return NOERROR;
}

ECode CLocalInputMethodManager::GetInputContext(
    /* [out] */ IInputContext** context)
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
    // try {
    return mService->GetInputMethodList(infos);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CLocalInputMethodManager::GetEnabledInputMethodList(
    /* [out] */ IObjectContainer** list)
{
    // try {
    return mService->GetEnabledInputMethodList(list);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CLocalInputMethodManager::ShowStatusIcon(
    /* [in] */ IBinder* imeToken,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 iconId)
{
    // try {
    return mService->UpdateStatusIcon(imeToken, packageName, iconId);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CLocalInputMethodManager::HideStatusIcon(
    /* [in] */ IBinder* imeToken)
{
    // try {
    return mService->UpdateStatusIcon(imeToken, String(NULL), 0);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
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
    VALIDATE_NOT_NULL(isfull);
    *isfull = mFullscreenMode;
    return NOERROR;
}

ECode CLocalInputMethodManager::IsActive(
    /* [in] */ IView* view,
    /* [out] */ Boolean* active)
{
    VALIDATE_NOT_NULL(active);
    FAIL_RETURN(CheckFocus());

    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    *active = (mServedView.Get() == view ||
            (mServedView != NULL && (mServedView->CheckInputConnectionProxy(view, &check), check))) &&
            mCurrentTextBoxAttribute.Get() != NULL;
    return NOERROR;
}

ECode CLocalInputMethodManager::IsActiveEx(
    /* [out] */ Boolean* active)
{
    VALIDATE_NOT_NULL(active);
    FAIL_RETURN(CheckFocus());

    Mutex::Autolock lock(sStaticHandlerLock);
    *active = mServedView != NULL && mCurrentTextBoxAttribute != NULL;
    return NOERROR;
}

ECode CLocalInputMethodManager::IsAcceptingText(
    /* [out] */ Boolean* isAccepting)
{
    VALIDATE_NOT_NULL(isAccepting);
    FAIL_RETURN(CheckFocus());

    *isAccepting = mServedInputConnection != NULL;
    return NOERROR;
}

void CLocalInputMethodManager::ClearBindingLocked()
{
    ClearConnectionLocked();
    mBindSequence = -1;
    mCurId = NULL;
    mCurMethod = NULL;
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
            // try {
            mService->FinishInput(mClient);
            // } catch (RemoteException e) {
            // }
        }

        if (mServedInputConnection != NULL) {
            // We need to tell the previously served view that it is no
            // longer the input target, so it can reset its state.  Schedule
            // this call on its window's Handler so it will be on the correct
            // thread and outside of our lock.
            AutoPtr<IApartment> vh;
            mServedView->GetHandler((IApartment**)&vh);
            if (vh != NULL) {
                // This will result in a call to reportFinishInputConnection()
                // below.
                ECode (STDCALL CLocalInputMethodManager::*pHandlerFunc)(IInputConnection* msg);
                pHandlerFunc = &CLocalInputMethodManager::ReportFinishInputConnection;

                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);
                params->WriteInterfacePtr(mServedInputConnection.Get());
                vh->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
            }
        }

        mServedView = NULL;
        mCompletions = NULL;
        mServedConnecting = false;
        ClearConnectionLocked();
    }
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
    FAIL_RETURN(CheckFocus());
    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    if (mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) {
        return NOERROR;
    }

    if (mCompletions != NULL) {
        for (Int32 i = 0; i < mCompletions->GetLength(); ++i) {
            (*mCompletions)[i] = NULL;
        }
        ArrayOf< AutoPtr<ICompletionInfo> >::Free(mCompletions);
    }
    mCompletions = (ArrayOf< AutoPtr<ICompletionInfo> >*)completions;
    if (mCurMethod != NULL) {
        // try {
        return mCurMethod->DisplayCompletions((ArrayOf<ICompletionInfo*>*)mCompletions);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CLocalInputMethodManager::UpdateExtractedText(
    /* [in] */ IView* view,
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    FAIL_RETURN(CheckFocus());
    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    if (mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) {
        return NOERROR;
    }

    if (mCurMethod != NULL) {
        // try {
        return mCurMethod->UpdateExtractedText(token, text);
        // } catch (RemoteException e) {
        // }
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
    /* [in] */ ILocalResultReceiver* resultReceiver,
    /* [out] */ Boolean* show)
{
    VALIDATE_NOT_NULL(show);
    FAIL_RETURN(CheckFocus());

    Mutex::Autolock lock(sStaticHandlerLock);

    Boolean check = FALSE;
    if (mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) {
        *show = FALSE;
        return NOERROR;
    }
    // try {
    return mService->ShowSoftInput(mClient, flags, resultReceiver, show);
    // } catch (RemoteException e) {
    // }
}

ECode CLocalInputMethodManager::ShowSoftInputUnchecked(
    /* [in] */ Int32 flags,
    /* [in] */ ILocalResultReceiver* resultReceiver)
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
    /* [in] */ ILocalResultReceiver* resultReceiver,
    /* [out] */ Boolean* hide)
{
    VALIDATE_NOT_NULL(hide);
    FAIL_RETURN(CheckFocus());
    Mutex::Autolock lock(sStaticHandlerLock);

    AutoPtr<IBinder> token;
    if (mServedView == NULL ||
            (mServedView->GetWindowToken((IBinder**)&token), token.Get() != windowToken)) {
        *hide = FALSE;
        return NOERROR;
    }

    // try {
    return mService->HideSoftInput(mClient, flags, resultReceiver, hide);
    // } catch (RemoteException e) {
    // }
}

ECode CLocalInputMethodManager::ToggleSoftInputFromWindow(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    Mutex::Autolock lock(sStaticHandlerLock);

    AutoPtr<IBinder> token;
    if (mServedView == NULL ||
            (mServedView->GetWindowToken((IBinder**)&token), token.Get() != windowToken)) {
        return NOERROR;
    }

    if (mCurMethod != NULL) {
        // try {
        return mCurMethod->ToggleSoftInput(showFlags, hideFlags);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CLocalInputMethodManager::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    if (mCurMethod != NULL) {
        // try {
        return mCurMethod->ToggleSoftInput(showFlags, hideFlags);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CLocalInputMethodManager::RestartInput(
    /* [in] */ IView* view)
{
    FAIL_RETURN(CheckFocus());
    {
        Mutex::Autolock lock(sStaticHandlerLock);

        Boolean check = FALSE;
        if (mServedView.Get() != view && (mServedView == NULL ||
                (mServedView->CheckInputConnectionProxy(view, &check), !check))) {
            return NOERROR;
        }

        mServedConnecting = TRUE;
    }

    StartInputInner();
    return NOERROR;
}

void CLocalInputMethodManager::StartInputInner()
{
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
    mServedView->GetHandler((IApartment**)&vh);
    if (vh == NULL) {
        // If the view doesn't have a handler, something has changed out
        // from under us, so just bail.
        // if (DEBUG) Log.v(TAG, "ABORT input: no handler for view!");
        return;
    }

    //TODO: the following implement is wrong
    // if (vh.getLooper() != Looper.myLooper()) {
    // AutoPtr<IApartment> tmpLooper;
    // CApartment::GetMainApartment((IApartment**)&tmpLooper);
    // if (vh != tmpLooper) {
    //     // The view is running on a different thread than our own, so
    //     // we need to reschedule our work for over there.
    //     // if (DEBUG) Log.v(TAG, "Starting input: reschedule to view thread");

    //     void (STDCALL CLocalInputMethodManager::*pHandlerFunc)();
    //     pHandlerFunc = &CLocalInputMethodManager::StartInputInner;
    //     vh->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc,
    //         NULL, 0, 0);
    //     return;
    // }

    // Okay we are now ready to call into the served view and have it
    // do its stuff.
    // Life is good: let's hook everything up!
    AutoPtr<CEditorInfo> tba;
    ASSERT_SUCCEEDED(CEditorInfo::NewByFriend((CEditorInfo**)&tba));
    AutoPtr<IContext> ctx;
    view->GetContext((IContext**)&ctx);
    ctx->GetCapsuleName(&tba->mCapsuleName);
    view->GetId(&tba->mFieldId);
    AutoPtr<IInputConnection> ic;
    view->OnCreateInputConnection(tba, (IInputConnection**)&ic);
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
        mCurrentTextBoxAttribute = (IEditorInfo*)tba.Get();
        mServedConnecting = FALSE;
        mServedInputConnection = ic;
        AutoPtr<IInputContext> servedContext;
        if (ic != NULL) {
            mCursorSelStart = tba->mInitialSelStart;
            mCursorSelEnd = tba->mInitialSelEnd;
            mCursorCandStart = -1;
            mCursorCandEnd = -1;
            mCursorRect->SetEmpty();
            CControlledInputConnectionWrapper::New(vh, ic,
                    (ILocalInputMethodManager*)this, (IInputContext**)&servedContext);
        }

        // try {
        // if (DEBUG) Log.v(TAG, "START INPUT: " + view + " ic="
        //         + ic + " tba=" + tba + " initial=" + initial);
        AutoPtr<IInputBindResult> res;
        mService->StartInput(mClient, servedContext, tba, initial,
                mCurMethod == NULL, (IInputBindResult**)&res);
        CInputBindResult* resObj = (CInputBindResult*)res.Get();
        // if (DEBUG) Log.v(TAG, "Starting input: Bind result=" + res);
        if (resObj != NULL) {
            if (!resObj->mId.IsNull()) {
                mBindSequence = resObj->mSequence;
                mCurMethod = resObj->mMethod;
            }
            else {
                // This means there is no input method available.
                // if (DEBUG) Log.v(TAG, "ABORT input: no input method!");
                return;
            }
        }
        if (mCurMethod != NULL && mCompletions != NULL) {
            // try {
            mCurMethod->DisplayCompletions((ArrayOf<ICompletionInfo*>*)mCompletions);
            // } catch (RemoteException e) {
            // }
        }
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        // }
    }
}

ECode CLocalInputMethodManager::WindowDismissed(
    /* [in] */ IBinder* appWindowToken)
{
    FAIL_RETURN(CheckFocus());
    Mutex::Autolock lock(sStaticHandlerLock);

    AutoPtr<IBinder> token;
    if (mServedView != NULL &&
            (mServedView->GetWindowToken((IBinder**)&token), token.Get() == appWindowToken)) {
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

void CLocalInputMethodManager::FocusInLocked(
    /* [in] */ IView* view)
{
    // if (DEBUG) Log.v(TAG, "focusIn: " + view);

    AutoPtr<IView> rootView;
    view->GetRootView((IView**)&rootView);
    if (mCurRootView != rootView) {
        // This is a request from a window that isn't in the window with
        // IME focus, so ignore it.
        // if (DEBUG) Log.v(TAG, "Not IME target window, ignoring");
        return;
    }

    mNextServedView = view;
    ScheduleCheckFocusLocked(view);
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
        if (FALSE && (view->HasWindowFocus(&focus), focus)) {
            mNextServedView = NULL;
            ScheduleCheckFocusLocked(view);
        }
    }
    return NOERROR;
}

void CLocalInputMethodManager::ScheduleCheckFocusLocked(
    /* [in] */ IView* view)
{
    AutoPtr<IApartment> vh;
    view->GetHandler((IApartment**)&vh);
    //todo
    // if (vh != NULL && !vh.hasMessages(ViewRoot.CHECK_FOCUS)) {
    if (vh != NULL) {
        // This will result in a call to checkFocus() below.
        ECode (STDCALL CLocalInputMethodManager::*pHandlerFunc)();
        pHandlerFunc = &CLocalInputMethodManager::CheckFocus;

        vh->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        vh->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc,
            NULL, 0, 0);
    }
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

void CLocalInputMethodManager::CloseCurrentInput()
{
    // try {
    Boolean ret = FALSE;
    mService->HideSoftInput(mClient, InputMethodManager_HIDE_NOT_ALWAYS, NULL, &ret);
    // } catch (RemoteException e) {
    // }
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

    FAIL_RETURN(CheckFocus());
    {
        Mutex::Autolock lock(sStaticHandlerLock);
        // try {
        Boolean checkIsTextEditor = FALSE;
        focusedView->OnCheckIsTextEditor(&checkIsTextEditor);
        Boolean isTextEditor = focusedView != NULL && checkIsTextEditor;

        AutoPtr<IBinder> token;
        rootView->GetWindowToken((IBinder**)&token);
        mService->WindowGainedFocus(mClient, token, focusedView != NULL,
            isTextEditor, softInputMode, first, windowFlags);
        // } catch (RemoteException e) {
        // }
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
    FAIL_RETURN(CheckFocus());

    {
        Mutex::Autolock lock(sStaticHandlerLock);

        Boolean check = FALSE;
        if ((mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) ||
            mCurrentTextBoxAttribute == NULL || mCurMethod == NULL) {
            return NOERROR;
        }

        if (mCursorSelStart != selStart || mCursorSelEnd != selEnd
                || mCursorCandStart != candidatesStart
                || mCursorCandEnd != candidatesEnd) {
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
    VALIDATE_NOT_NULL(isWatching);
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
    FAIL_RETURN(CheckFocus());
    {
        Mutex::Autolock lock(sStaticHandlerLock);

        Boolean check = FALSE;
        if ((mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) ||
            mCurrentTextBoxAttribute == NULL || mCurMethod == NULL) {
            return NOERROR;
        }

        mTmpCursorRect->Set(left, top, right, bottom);
        Boolean equal = FALSE;
        if (mCursorRect->Equals(mTmpCursorRect, &equal), !equal) {
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
    FAIL_RETURN(CheckFocus());
    {
        Mutex::Autolock lock(sStaticHandlerLock);

        Boolean check;
        if ((mServedView.Get() != view && (mServedView == NULL ||
                (mServedView->CheckInputConnectionProxy(view, &check), !check))) ||
                mCurrentTextBoxAttribute == NULL || mCurMethod == NULL) {
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
    // try {
    return mService->SetInputMethod(token, id);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CLocalInputMethodManager::HideSoftInputFromInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 flags)
{
    // try {
    return mService->HideMySoftInput(token, flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CLocalInputMethodManager::ShowSoftInputFromInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 flags)
{
    // try {
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
            // try {
            return callback->FinishedEvent(seq, false);
            // } catch (RemoteException e) {
            // }
        }

        Int32 action = 0;
        key->GetAction(&action);
        Int32 keyCode = 0;
        key->GetKeyCode(&keyCode);
        if (action == KeyEvent_ACTION_DOWN && keyCode == KeyEvent_KEYCODE_SYM) {
            ShowInputMethodPicker();
            // try {
            return callback->FinishedEvent(seq, true);
            // } catch (RemoteException e) {
            // }
        }
        // try {
        //     if (DEBUG) Log.v(TAG, "DISPATCH KEY: " + mCurMethod);
        ECode ec = mCurMethod->DispatchKeyEvent(seq, key, callback);
        if (FAILED(ec)) {
            return callback->FinishedEvent(seq, FALSE);
        }
        return ec;
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId + " dropping: " + key, e);
        //     try {
        //         callback.finishedEvent(seq, false);
        //     } catch (RemoteException ex) {
        //     }
        // }
    }
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
            // try {
            callback->FinishedEvent(seq, false);
            // } catch (RemoteException e) {
            // }
            return;
        }

        // try {
        //     if (DEBUG) Log.v(TAG, "DISPATCH TRACKBALL: " + mCurMethod);
        ECode ec = mCurMethod->DispatchTrackballEvent(seq, motion, callback);
        if (FAILED(ec)) {
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

ECode CLocalInputMethodManager::ShowInputMethodPicker()
{
    Mutex::Autolock lock(sStaticHandlerLock);

    // try {
    return mService->ShowInputMethodPickerFromClient(mClient);
    // } catch (RemoteException e) {
    //     Log.w(TAG, "IME died: " + mCurId, e);
    // }
}

void CLocalInputMethodManager::HandleMsgBind(
    /* [in] */ IInputBindResult* res)
{
    assert(res != NULL);
    {
        Mutex::Autolock lock(sStaticHandlerLock);

        CInputBindResult* resObj = (CInputBindResult*)res;
        if (mBindSequence < 0 || mBindSequence != resObj->mSequence) {
            // Log.w(TAG, "Ignoring onBind: cur seq=" + mBindSequence
            //         + ", given seq=" + res.sequence);
            return;
        }

        mCurMethod = resObj->mMethod;
        mCurId = resObj->mId;
        mBindSequence = resObj->mSequence;
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
        if (mServedView != NULL && (mServedView->IsFocused(&focused), focused)) {
            mServedConnecting = TRUE;
        }
    }
    StartInputInner();
}

void CLocalInputMethodManager::HandleMsgSetActive(
    /* [in] */ Boolean active)
{
    Mutex::Autolock lock(sStaticHandlerLock);

    mActive = active;
    mFullscreenMode = FALSE;
    if (!active) {
        // Some other client has starting using the IME, so note
        // that this happened and make sure our own editor's
        // state is reset.
        mHasBeenInactive = TRUE;
        // try {
        // Note that finishComposingText() is allowed to run
        // even when we are not active.
        mIInputContext->FinishComposingText();
        // } catch (RemoteException e) {
        // }
    }
}

