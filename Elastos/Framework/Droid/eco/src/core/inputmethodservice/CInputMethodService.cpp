
#include "inputmethodservice/CInputMethodService.h"
#include "view/ViewTreeObserver.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/inputmethod/CEditorInfo.h"
#include "view/inputmethod/CExtractedTextRequest.h"
#include "view/inputmethod/CExtractedText.h"
#include "widget/CLinearLayoutLayoutParams.h"
#include "content/CConfiguration.h"
#include "os/SystemClock.h"
#include "view/CKeyEvent.h"
#include "view/animation/AnimationUtils.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

ICONTEXT_METHODS_IMPL(CInputMethodService, _InputMethodService, _InputMethodService);

static Int32 R_Styleable_InputMethodService[]= {
    0x0101022c, 0x01010268, 0x01010269
};

CString CInputMethodService::TAG = "CInputMethodService";
const Int32 CInputMethodService::MOVEMENT_DOWN;
const Int32 CInputMethodService::MOVEMENT_UP;
const Int32 CInputMethodService::Insets::TOUCHABLE_INSETS_FRAME;
const Int32 CInputMethodService::Insets::TOUCHABLE_INSETS_CONTENT;
const Int32 CInputMethodService::Insets::TOUCHABLE_INSETS_VISIBLE;


_InputMethodService::_InputMethodService():
     mThemeId(0x01030054 /*android.R.style.Theme_InputMethod*/)
{

}

ECode _InputMethodService::SetTheme(
    /* [in] */ Int32 theme)
{
    if (mWindow != NULL) {
        // throw new IllegalStateException("Must be called before onCreate()");
        assert(0);
    }

    mThemeId = theme;
    return NOERROR;
}

CInputMethodService::_OnComputeInternalInsetsListener::_OnComputeInternalInsetsListener(
	/* [in] */ CInputMethodService* host):
	mHost(host)
{}

CInputMethodService::_OnComputeInternalInsetsListener::~_OnComputeInternalInsetsListener()
{}

PInterface CInputMethodService::_OnComputeInternalInsetsListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IOnComputeInternalInsetsListener) {
        return (IOnComputeInternalInsetsListener*)this;
    }

    return NULL;
}

UInt32 CInputMethodService::_OnComputeInternalInsetsListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CInputMethodService::_OnComputeInternalInsetsListener::Release()
{
    return ElRefBase::Release();
}

ECode CInputMethodService::_OnComputeInternalInsetsListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IOnComputeInternalInsetsListener*)this) {
        *pIID = EIID_IOnComputeInternalInsetsListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CInputMethodService::_OnComputeInternalInsetsListener::OnComputeInternalInsets(
    /* [in] */ IInternalInsetsInfo* info)
{
	assert(mHost != NULL && info != NULL);
	Boolean shown = FALSE;
	mHost->IsExtractViewShown(&shown);

    if (shown) {
        // In true fullscreen mode, we just say the window isn't covering
        // any content so we don't impact whatever is behind.
        AutoPtr<IDialog> dialog;
        mHost->GetWindow((IDialog**) &dialog);
        assert(dialog != NULL);

        AutoPtr<IWindow> window;
        dialog->GetWindow((IWindow**) &window);
        assert(window != NULL);

        AutoPtr<IView> decor;
        window->GetDecorView((IView**) &decor);

        Int32 h = 0;
        decor->GetHeight(&h);

        ((ViewTreeObserver::InternalInsetsInfo*)info)->mContentInsets->SetTop(h);
        ((ViewTreeObserver::InternalInsetsInfo*)info)->mVisibleInsets->SetTop(h);

        info->SetTouchableInsets(ViewTreeObserver::InternalInsetsInfo::TOUCHABLE_INSETS_FRAME);
    } else {
        mHost->OnComputeInsets(mHost->mTmpInsets);
        ((ViewTreeObserver::InternalInsetsInfo*)info)->mContentInsets->SetTop(
                mHost->mTmpInsets->mContentTopInsets);

        ((ViewTreeObserver::InternalInsetsInfo*)info)->mVisibleInsets->SetTop(
            mHost->mTmpInsets->mVisibleTopInsets);

        info->SetTouchableInsets(mHost->mTmpInsets->mTouchableInsets);
    }

    return NOERROR;
}

CInputMethodService::_OnClickListener::_OnClickListener(
	/* [in] */ CInputMethodService* host):
	mHost(host)
{}

CInputMethodService::_OnClickListener::~_OnClickListener()
{}

PInterface CInputMethodService::_OnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

UInt32 CInputMethodService::_OnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CInputMethodService::_OnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode CInputMethodService::_OnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IViewOnClickListener*)this) {
        *pIID = EIID_IViewOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CInputMethodService::_OnClickListener::OnClick(
    /* [in] */ IView* v)
{
	assert(mHost != NULL);
    AutoPtr<IEditorInfo> ei;
    mHost->GetCurrentInputEditorInfo((IEditorInfo**) &ei);

    AutoPtr<IInputConnection> ic;
    mHost->GetCurrentInputConnection((IInputConnection**) &ic);

    if (ei != NULL && ic != NULL) {
        Boolean ret = FALSE;

        if (((CEditorInfo*)ei.Get())->mActionId != 0) {
            ic->PerformEditorAction(((CEditorInfo*)ei.Get())->mActionId, &ret);
        }
        else if ((((CEditorInfo*)ei.Get())->mImeOptions & EditorInfo_IME_MASK_ACTION)
                != EditorInfo_IME_ACTION_NONE)
        {
            ic->PerformEditorAction(((CEditorInfo*)ei.Get())->mImeOptions
                    & EditorInfo_IME_MASK_ACTION, &ret);
        }
    }

    return NOERROR;
}

CInputMethodService::InputMethodImpl::InputMethodImpl(
    /* [in] */ CInputMethodService* host):
    AbstractInputMethodImpl((AbstractInputMethodService*)host),
    mHost(host)
{
    assert(mHost != NULL);
}

CInputMethodService::InputMethodImpl::~InputMethodImpl()
{}

ECode CInputMethodService::InputMethodImpl::AttachToken(
    /* [in] */ IBinder* token)
{
    if (mHost->mToken == NULL) {
        mHost->mToken = token;
        mHost->mWindow->SetToken(token);
    }

    return NOERROR;
}

ECode CInputMethodService::InputMethodImpl::BindInput(
    /* [in] */ IInputBinding* binding)
{
    mHost->mInputBinding = binding;

    assert(binding != NULL && mHost != NULL);
    binding->GetConnection((IInputConnection**) &mHost->mInputConnection);
    // if (DEBUG) Log.v(TAG, "bindInput(): binding=" + binding
    //         + " ic=" + mInputConnection);

    AutoPtr<IInputConnection> ic;
    mHost->GetCurrentInputConnection((IInputConnection**) &ic);
    if (ic != NULL) {
        Boolean tmpState = FALSE;
        ic->ReportFullscreenMode(mHost->mIsFullscreen, &tmpState);
    }

    mHost->Initialize();
    return mHost->OnBindInput();
}

ECode CInputMethodService::InputMethodImpl::UnbindInput()
{
    assert(mHost != NULL);
    // if (DEBUG) Log.v(TAG, "unbindInput(): binding=" + mInputBinding
    //         + " ic=" + mInputConnection);
    mHost->OnUnbindInput();
    mHost->mInputStarted = FALSE;
    mHost->mInputBinding = NULL;
    mHost->mInputConnection = NULL;

    return NOERROR;
}

ECode CInputMethodService::InputMethodImpl::StartInput(
    /* [in] */ IInputConnection* ic,
    /* [in] */ IEditorInfo* attribute)
{
    assert(mHost != NULL);
    // if (DEBUG) Log.v(TAG, "startInput(): editor=" + attribute);
    mHost->DoStartInput(ic, attribute, FALSE);

    return NOERROR;
}

ECode CInputMethodService::InputMethodImpl::RestartInput(
    /* [in] */ IInputConnection* ic,
    /* [in] */ IEditorInfo* attribute)
{
    assert(mHost != NULL);
    // if (DEBUG) Log.v(TAG, "restartInput(): editor=" + attribute);
    mHost->DoStartInput(ic, attribute, TRUE);

    return NOERROR;
}

ECode CInputMethodService::InputMethodImpl::HideSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    assert(mHost != NULL);
    // if (DEBUG) Log.v(TAG, "hideSoftInput()");
    Boolean wasVis = FALSE;
    mHost->IsInputViewShown(&wasVis);

    mHost->mShowInputFlags = 0;
    mHost->mShowInputRequested = FALSE;
    mHost->mShowInputForced = FALSE;

    mHost->HideWindow();
    if (resultReceiver != NULL) {
        Boolean tmpVis = FALSE;
        mHost->IsInputViewShown(&tmpVis);
        resultReceiver->Send(wasVis != tmpVis
                ? InputMethodManager_RESULT_HIDDEN
                : (wasVis ? InputMethodManager_RESULT_UNCHANGED_SHOWN
                        : InputMethodManager_RESULT_UNCHANGED_HIDDEN), NULL);
    }

    return NOERROR;
}

ECode CInputMethodService::InputMethodImpl::ShowSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    assert(mHost != NULL);
    // if (DEBUG) Log.v(TAG, "showSoftInput()");
    Boolean wasVis = FALSE;
    mHost->IsInputViewShown(&wasVis);

    mHost->mShowInputFlags = 0;
    Boolean requested = FALSE;
    mHost->OnShowInputRequested(flags, FALSE, &requested);
    if (requested) {
        mHost->ShowWindow(TRUE);
    }

    if (resultReceiver != NULL) {
        Boolean tmpVis = FALSE;
        mHost->IsInputViewShown(&tmpVis);

        resultReceiver->Send(wasVis != tmpVis
                ? InputMethodManager_RESULT_SHOWN
                : (wasVis ? InputMethodManager_RESULT_UNCHANGED_SHOWN
                        : InputMethodManager_RESULT_UNCHANGED_HIDDEN), NULL);
    }

    return NOERROR;
}

CInputMethodService::InputMethodSessionImpl::InputMethodSessionImpl(
    /* [in] */ CInputMethodService* host):
    AbstractInputMethodSessionImpl((AbstractInputMethodService*)host)
{}

CInputMethodService::InputMethodSessionImpl::~InputMethodSessionImpl()
{}

ECode CInputMethodService::InputMethodSessionImpl::FinishInput()
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    IsEnabled(&enabled);

    if (!enabled) {
        return NOERROR;
    }

    // if (DEBUG) Log.v(TAG, "finishInput() in " + this);
    mHost->DoFinishInput();

    return NOERROR;
}

ECode CInputMethodService::InputMethodSessionImpl::DisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    IsEnabled(&enabled);

    if (!enabled) {
        return NOERROR;
    }

    mHost->mCurCompletions = completions;
    return mHost->OnDisplayCompletions(completions);
}

ECode CInputMethodService::InputMethodSessionImpl::UpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    IsEnabled(&enabled);

    if (!enabled) {
        return NOERROR;
    }

    return mHost->OnUpdateExtractedText(token, text);
}

ECode CInputMethodService::InputMethodSessionImpl::UpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    IsEnabled(&enabled);

    if (!enabled) {
        return NOERROR;
    }

    return mHost->OnUpdateSelection(oldSelStart, oldSelEnd,
            newSelStart, newSelEnd, candidatesStart, candidatesEnd);
}

ECode CInputMethodService::InputMethodSessionImpl::UpdateCursor(
    /* [in] */ IRect* newCursor)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    IsEnabled(&enabled);

    if (!enabled) {
        return NOERROR;
    }

    return mHost->OnUpdateCursor(newCursor);
}

ECode CInputMethodService::InputMethodSessionImpl::AppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    IsEnabled(&enabled);

    if (!enabled) {
        return NOERROR;
    }

    return mHost->OnAppPrivateCommand(action, data);
}

ECode CInputMethodService::InputMethodSessionImpl::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    mHost->OnToggleSoftInput(showFlags, hideFlags);
    return NOERROR;
}

CInputMethodService::Insets::Insets():
    mContentTopInsets(0),
    mVisibleTopInsets(0),
    mTouchableInsets(0)
{

}


UInt32 CInputMethodService::Insets::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CInputMethodService::Insets::Release()
{
    return ElRefBase::Release();
}

CInputMethodService::CInputMethodService():
    mInitialized(FALSE),
    mWindowCreated(FALSE),
    mWindowAdded(FALSE),
    mWindowVisible(FALSE),
    mWindowWasVisible(FALSE),
    mInShowWindow(FALSE),
    mInputStarted(FALSE),
    mInputViewStarted(FALSE),
    mCandidatesViewStarted(FALSE),
    mShowInputFlags(0),
    mShowInputRequested(FALSE),
    mLastShowInputRequested(FALSE),
    mCandidatesVisibility(0),
    mCurCompletions(NULL),
    mShowInputForced(FALSE),
    mFullscreenApplied(FALSE),
    mIsFullscreen(FALSE),
    mExtractViewHidden(FALSE),
    mExtractedToken(0),
    mIsInputViewShown(FALSE),
    mStatusIcon(0),
    mTmpLocation(NULL)
{
    mTmpInsets = new CInputMethodService::Insets();
    mTmpLocation = new Int32[2];
}

PInterface CInputMethodService::Probe(
    /* [in] */ REIID riid)
{
    return _CInputMethodService::Probe(riid);
}

ECode CInputMethodService::constructor()
{
    mInsetsComputer = new _OnComputeInternalInsetsListener(this);
    mActionClickListener = new _OnClickListener(this);

    return NOERROR;
}

CInputMethodService::~CInputMethodService()
{
    if (mTmpLocation != NULL) {
        delete mTmpLocation;
        mTmpLocation = NULL;
    }
}

ECode CInputMethodService::OnCreate()
{
    AbstractInputMethodService::SetTheme(mThemeId);
    AbstractInputMethodService::OnCreate();

    AbstractInputMethodService::GetSystemService(
        String(Context_INPUT_METHOD_SERVICE), (IInterface**) &mImm);
    assert(mImm != NULL);

    AbstractInputMethodService::GetSystemService(
        String(Context_LAYOUT_INFLATER_SERVICE), (IInterface**) &mInflater);
    assert(mInflater != NULL);

    mWindow = new SoftInputWindow((IContext*)this->Probe(EIID_IContext),
        mThemeId, mDispatcherState);

    InitViews();

    AutoPtr<IWindow> window;
    mWindow->GetWindow((IWindow**) &window);
    assert(window != NULL);

    return window->SetLayout(ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_WRAP_CONTENT);
}

ECode CInputMethodService::OnInitializeInterface()
{
    return NOERROR;
}

ECode CInputMethodService::Initialize()
{
    if (!mInitialized) {
        mInitialized = TRUE;
        OnInitializeInterface();
    }

    return NOERROR;
}

ECode CInputMethodService::OnDestroy()
{
    AbstractInputMethodService::OnDestroy();

    AutoPtr<IViewTreeObserver> observer;
    mRootView->GetViewTreeObserver((IViewTreeObserver**) &observer);
    assert(observer != NULL);

    observer->RemoveOnComputeInternalInsetsListener(mInsetsComputer);

    if (mWindowAdded) {
        return mWindow->Dismiss();
    }

    return NOERROR;
}

ECode CInputMethodService::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AbstractInputMethodService::OnConfigurationChanged(newConfig);

    Boolean visible = mWindowVisible;
    Int32 showFlags = mShowInputFlags;
    Boolean showingInput = mShowInputRequested;
    ArrayOf<ICompletionInfo*>* completions = mCurCompletions;

    InitViews();
    mInputViewStarted = FALSE;
    mCandidatesViewStarted = FALSE;
    if (mInputStarted) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**) &ic);
        AutoPtr<IEditorInfo> ei;
        GetCurrentInputEditorInfo((IEditorInfo**) &ei);

        DoStartInput(ic, ei, TRUE);
    }

    if (visible) {
        if (showingInput) {
            // If we were last showing the soft keyboard, try to do so again.
            Boolean requested = FALSE;
            OnShowInputRequested(showFlags, TRUE, &requested);

            if (requested) {
                ShowWindow(TRUE);

                if (completions != NULL) {
                    mCurCompletions = completions;
                    OnDisplayCompletions(completions);
                }
            } else {
                HideWindow();
            }
        } else if (mCandidatesVisibility == View_VISIBLE) {
            // If the candidates are currently visible, make sure the
            // window is shown for them.
            ShowWindow(FALSE);
        } else {
            // Otherwise hide the window.
            HideWindow();
        }
    }

    return NOERROR;
}

ECode CInputMethodService::OnLowMemory()
{
    return AbstractInputMethodService::OnLowMemory();
}

ECode CInputMethodService::OnCreateInputMethodInterface(
    /* [out] */ IAbstractInputMethodImpl** inputMethodImpl)
{
    assert(inputMethodImpl != NULL);
    *inputMethodImpl = new InputMethodImpl(this);
    return NOERROR;
}

ECode CInputMethodService::OnCreateInputMethodSessionInterface(
    /* [out] */ IAbstractInputMethodSessionImpl** abstractInputMethodSessionImpl)
{
    assert(abstractInputMethodSessionImpl != NULL);
    *abstractInputMethodSessionImpl = new InputMethodSessionImpl(this);

    return NOERROR;
}

ECode CInputMethodService::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    assert(inflater != NULL);
    *inflater = mInflater;
    if (*inflater != NULL) {
        (*inflater)->AddRef();
    }

    return NOERROR;
}

ECode CInputMethodService::GetWindow(
    /* [out] */ IDialog** dialog)
{
    assert(dialog != NULL);
    *dialog = mWindow;
    if (*dialog != NULL) {
        (*dialog)->AddRef();
    }

    return NOERROR;
}

ECode CInputMethodService::GetMaxWidth(
    /* [out] */ Int32* maxWidth)
{
    AutoPtr<IWindowManager> wm;
    AbstractInputMethodService::GetSystemService(
            String(Context_WINDOW_SERVICE), (IInterface**) &wm);

    assert(wm != NULL);

    AutoPtr<IDisplay> display;
    wm->GetDefaultDisplay((IDisplay**) &display);
    assert(display != NULL);

    return display->GetWidth(maxWidth);
}

/**
 * Return the currently active InputBinding for the input method, or
 * NULL if there is none.
 */
ECode CInputMethodService::GetCurrentInputBinding(
    /* [out] */ IInputBinding** binding)
{
    assert(binding != NULL);
    *binding = mInputBinding;
    if (*binding != NULL) {
        (*binding)->AddRef();
    }

    return NOERROR;
}

ECode CInputMethodService::GetCurrentInputConnection(
    /* [out] */ IInputConnection** inputConnection)
{
    assert(inputConnection != NULL);

    if (mStartedInputConnection != NULL) {
        *inputConnection = mStartedInputConnection;
        (*inputConnection)->AddRef();

        return NOERROR;
    }

    *inputConnection = mInputConnection;
    if (*inputConnection != NULL) {
        (*inputConnection)->AddRef();
    }

    return NOERROR;
}

ECode CInputMethodService::GetCurrentInputStarted(
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = mInputStarted;
    return NOERROR;
}

ECode CInputMethodService::GetCurrentInputEditorInfo(
    /* [out] */ IEditorInfo** editorInfo)
{
    assert(editorInfo != NULL);
    *editorInfo = mInputEditorInfo;
    if (*editorInfo != NULL) {
        (*editorInfo)->AddRef();
    }

    return NOERROR;
}

ECode CInputMethodService::UpdateFullscreenMode()
{
    Boolean tmpMode = FALSE;
    OnEvaluateFullscreenMode(&tmpMode);

    Boolean isFullscreen = mShowInputRequested && tmpMode;
    Boolean changed = mLastShowInputRequested != mShowInputRequested;

    if (mIsFullscreen != isFullscreen || !mFullscreenApplied) {
        changed = TRUE;
        mIsFullscreen = isFullscreen;
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**) &ic);

        if (ic != NULL) {
            Boolean tmpState = FALSE;
            ic->ReportFullscreenMode(isFullscreen, &tmpState);
        }

        mFullscreenApplied = TRUE;
        Initialize();

        assert(mFullscreenArea != NULL);
        AutoPtr<IViewGroupLayoutParams> lp;
        mFullscreenArea->GetLayoutParams((IViewGroupLayoutParams**) &lp);

        if (isFullscreen) {
            assert(mThemeAttrs != NULL);

            AutoPtr<IDrawable> drawable;

            //TODO: verify the InputMethodService_imeFullscreenBackground's value.
            mThemeAttrs->GetDrawable(
                0 /*com.android.internal.R.styleable.InputMethodService_imeFullscreenBackground*/,
                (IDrawable**)&drawable);

            mFullscreenArea->SetBackgroundDrawable(drawable);
            ((CLinearLayoutLayoutParams*)lp.Get())->mHeight = 0;
            ((CLinearLayoutLayoutParams*)lp.Get())->mWeight = 1;
        } else {
            mFullscreenArea->SetBackgroundDrawable(NULL);
            ((CLinearLayoutLayoutParams*)lp.Get())->mHeight = ViewGroupLayoutParams_WRAP_CONTENT;
            ((CLinearLayoutLayoutParams*)lp.Get())->mWeight = 0;
        }

        AutoPtr<IViewParent> parent;
        mFullscreenArea->GetParent((IViewParent**)&parent);
        assert(parent != NULL);

        AutoPtr<IViewManager> viewM = IViewManager::Probe(parent);
        assert(viewM != NULL);
        viewM->UpdateViewLayout(mFullscreenArea, lp);

        if (isFullscreen) {
            if (mExtractView == NULL) {
                AutoPtr<IView> v;
                OnCreateExtractTextView((IView**) &v);
                if (v != NULL) {
                    SetExtractView(v);
                }
            }

            StartExtractingText(FALSE);
        }

        UpdateExtractFrameVisibility();
    }

    if (changed) {
        AutoPtr<IWindow> window;
        mWindow->GetWindow((IWindow**) &window);
        OnConfigureWindow(window, isFullscreen, !mShowInputRequested);
        mLastShowInputRequested = mShowInputRequested;
    }

    return NOERROR;
}

ECode CInputMethodService::OnConfigureWindow(
    /* [in] */ IWindow* win,
    /* [in] */ Boolean isFullscreen,
    /* [in] */ Boolean isCandidatesOnly)
{
    assert(mWindow != NULL);
    AutoPtr<IWindow> window;
    mWindow->GetWindow((IWindow**) &window);
    assert(window != NULL);

    if (isFullscreen) {
        window->SetLayout(ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_MATCH_PARENT);
    } else {
        window->SetLayout(ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_WRAP_CONTENT);
    }

    return NOERROR;
}

ECode CInputMethodService::IsFullscreenMode(
    /* [out] */ Boolean* screenMode)
{
    assert(screenMode != NULL);
    *screenMode = mIsFullscreen;
    return NOERROR;
}

/**
 * Override this to control when the input method should run in
 * fullscreen mode.  The default implementation runs in fullsceen only
 * when the screen is in landscape mode.  If you change what
 * this returns, you will need to call {@link #updateFullscreenMode()}
 * yourself whenever the returned value may have changed to have it
 * re-evaluated and applied.
 */
ECode CInputMethodService::OnEvaluateFullscreenMode(
    /* [out] */ Boolean* screenMode)
{
    assert(screenMode != NULL);

    AutoPtr<IResources> res;
    AbstractInputMethodService::GetResources((IResources**) &res);
    assert(res != NULL);

    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**) &config);
    assert(config != NULL);

    if (((CConfiguration*)config.Get())->mOrientation !=
            Configuration_ORIENTATION_LANDSCAPE) {
        *screenMode = FALSE;
        return NOERROR;
    }
    if (mInputEditorInfo != NULL
            && (((CEditorInfo*)mInputEditorInfo.Get())->mImeOptions & EditorInfo_IME_FLAG_NO_FULLSCREEN) != 0) {
        *screenMode = FALSE;
        return NOERROR;
    }

    *screenMode = TRUE;
    return NOERROR;
}

ECode CInputMethodService::SetExtractViewShown(
    /* [in] */ Boolean shown)
{
    if (mExtractViewHidden == shown) {
        mExtractViewHidden = !shown;
        UpdateExtractFrameVisibility();
    }

    return NOERROR;
}

ECode CInputMethodService::IsExtractViewShown(
    /* [out] */ Boolean* shown)
{
    assert(shown != NULL);
    *shown = mIsFullscreen && !mExtractViewHidden;
    return NOERROR;
}

ECode CInputMethodService::OnComputeInsets(
    /* [in] */ Insets* outInsets)
{
    assert(mInputFrame != NULL && mTmpLocation != NULL && outInsets != NULL);

    Int32 vis = 0;
    mInputFrame->GetVisibility(&vis);
    if (vis == View_VISIBLE) {
        mInputFrame->GetLocationInWindow(&mTmpLocation[0], &mTmpLocation[1]);
    } else {
        AutoPtr<IDialog> dlg;
        GetWindow((IDialog**) &dlg);
        assert(dlg != NULL);

        AutoPtr<IWindow> win;
        dlg->GetWindow((IWindow**) &win);
        assert(win != NULL);

        AutoPtr<IView> decor;
        win->GetDecorView((IView**) &decor);
        assert(decor != NULL);

        decor->GetHeight(&mTmpLocation[1]);
    }

    Boolean ret = FALSE;
    if (IsFullscreenMode(&ret), ret) {
        // In fullscreen mode, we never resize the underlying window.
        AutoPtr<IDialog> dlg;
        GetWindow((IDialog**) &dlg);
        assert(dlg != NULL);

        AutoPtr<IWindow> win;
        dlg->GetWindow((IWindow**) &win);
        assert(win != NULL);

        AutoPtr<IView> decor;
        win->GetDecorView((IView**) &decor);
        assert(decor != NULL);

        decor->GetHeight(&outInsets->mContentTopInsets);
    } else {
        outInsets->mContentTopInsets = mTmpLocation[1];
    }

    mCandidatesFrame->GetVisibility(&vis);
    if (vis == View_VISIBLE) {
        mCandidatesFrame->GetLocationInWindow(&mTmpLocation[0], &mTmpLocation[1]);
    }

    outInsets->mVisibleTopInsets = mTmpLocation[1];
    outInsets->mTouchableInsets = Insets::TOUCHABLE_INSETS_VISIBLE;

    return NOERROR;
}

ECode CInputMethodService::UpdateInputViewShown()
{
    Boolean isShown = FALSE;
    OnEvaluateInputViewShown(&isShown);
    isShown = mShowInputRequested && isShown;

    if (mIsInputViewShown != isShown && mWindowVisible) {
        mIsInputViewShown = isShown;
        assert(mInputFrame != NULL);
        mInputFrame->SetVisibility(isShown ? View_VISIBLE : View_GONE);
        if (mInputView == NULL) {
            Initialize();
            AutoPtr<IView> v;
            OnCreateInputView((IView**) &v);

            if (v != NULL) {
                return SetInputView(v);
            }
        }
    }

    return NOERROR;
}

ECode CInputMethodService::IsShowInputRequested(
    /* [out] */ Boolean* requested)
{
    assert(requested != NULL);
    *requested = mShowInputRequested;
    return NOERROR;
}

ECode CInputMethodService::IsInputViewShown(
    /* [out] */ Boolean* shown)
{
    assert(shown != NULL);
    *shown = mIsInputViewShown && mWindowVisible;
    return NOERROR;
}

ECode CInputMethodService::OnEvaluateInputViewShown(
    /* [out] */ Boolean* shown)
{
    assert(shown != NULL);

    AutoPtr<IResources> res;
    AbstractInputMethodService::GetResources((IResources**) &res);
    assert(res != NULL);

    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**) &config);

    *shown = ((CConfiguration*)config.Get())->mKeyboard == Configuration_KEYBOARD_NOKEYS
        || ((CConfiguration*)config.Get())->mHardKeyboardHidden == Configuration_KEYBOARDHIDDEN_YES;

    return NOERROR;
}

ECode CInputMethodService::SetCandidatesViewShown(
    /* [in] */ Boolean shown)
{
    UpdateCandidatesVisibility(shown);

    if (!mShowInputRequested && mWindowVisible != shown) {
        // If we are being asked to show the candidates view while the app
        // has not asked for the input view to be shown, then we need
        // to update whether the window is shown.
        if (shown) {
            return ShowWindow(FALSE);
        } else {
            return HideWindow();
        }
    }

    return NOERROR;
}

ECode CInputMethodService::GetCandidatesHiddenVisibility(
    /* [out] */ Int32* vis)
{
    assert(vis != NULL);
    Boolean shown = FALSE;
    IsExtractViewShown(&shown);

    *vis = shown ? View_GONE : View_INVISIBLE;
    return NOERROR;
}

ECode CInputMethodService::ShowStatusIcon(
    /* [in] */ Int32 iconResId)
{
    mStatusIcon = iconResId;

    //TODO: GetPackageName
    //return mImm->ShowStatusIcon(mToken, GetPackageName(), iconResId);
    return mImm->ShowStatusIcon(mToken, String("CInputMethodService"), iconResId);
}

ECode CInputMethodService::HideStatusIcon()
{
    mStatusIcon = 0;
    return mImm->HideStatusIcon(mToken);
}

ECode CInputMethodService::SwitchInputMethod(
    /* [in] */ const String& id)
{
    return mImm->SetInputMethod(mToken, id);
}

ECode CInputMethodService::SetExtractView(
    /* [in] */ IView* view)
{
    assert(mExtractFrame != NULL);
    mExtractFrame->RemoveAllViews();

    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_MATCH_PARENT,
        (IViewGroupLayoutParams**)&params);

    mExtractFrame->AddViewEx3(view, params.Get());

    mExtractView = view;
    if (view != NULL) {
        view->FindViewById(0x01020025 /*com.android.internal.R.id.inputExtractEditText*/,
            (IView**) &mExtractEditText);

        mExtractEditText->SetIME((IInputMethodService*)this->Probe(EIID_IInputMethodService));

        view->FindViewById(0x010201b9 /*com.android.internal.R.id.inputExtractAction*/,
            (IView**) &mExtractAction);

        if (mExtractAction != NULL) {
            view->FindViewById(0x010201b8 /*com.android.internal.R.id.inputExtractAccessories*/,
                    (IView**) &mExtractAccessories);
        }

        StartExtractingText(FALSE);
        return NOERROR;
    } else {
        mExtractEditText = NULL;
        mExtractAccessories = NULL;
        mExtractAction = NULL;
    }

    return NOERROR;
}

/**
 * Replaces the current candidates view with a new one.  You only need to
 * call this when dynamically changing the view; normally, you should
 * implement {@link #onCreateCandidatesView()} and create your view when
 * first needed by the input method.
 */
ECode CInputMethodService::SetCandidatesView(
    /* [in] */ IView* view)
{
    assert(mCandidatesFrame != NULL);
    mCandidatesFrame->RemoveAllViews();

    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_WRAP_CONTENT,
        (IViewGroupLayoutParams**)&params);

    return mCandidatesFrame->AddViewEx3(view, params.Get());
}

/**
 * Replaces the current input view with a new one.  You only need to
 * call this when dynamically changing the view; normally, you should
 * implement {@link #onCreateInputView()} and create your view when
 * first needed by the input method.
 */
ECode CInputMethodService::SetInputView(
    /* [in] */ IView* view)
{
    assert(mInputFrame != NULL);
    mInputFrame->RemoveAllViews();

    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_WRAP_CONTENT,
        (IViewGroupLayoutParams**)&params);

    mInputFrame->AddViewEx3(view, params.Get());
    mInputView = view;

    return NOERROR;
}

/**
 * Called by the framework to create the layout for showing extacted text.
 * Only called when in fullscreen mode.  The returned view hierarchy must
 * have an {@link ExtractEditText} whose ID is
 * {@link android.R.id#inputExtractEditText}.
 */
ECode CInputMethodService::OnCreateExtractTextView(
    /* [out] */ IView** view)
{
    assert(mInflater != NULL);
    return mInflater->Inflate(0x0109002b /*com.android.internal.R.layout.input_method_extract_view*/,
            NULL, view);
}

ECode CInputMethodService::OnCreateCandidatesView(
    /* [out] */ IView** view)
{
    assert(view != NULL);
    *view = NULL;
    return NOERROR;
}

ECode CInputMethodService::OnCreateInputView(
    /* [out] */ IView** view)
{
    assert(view != NULL);
    *view = NULL;
    return NOERROR;
}

ECode CInputMethodService::OnStartInputView(
    /* [in] */ IEditorInfo* info,
    /* [in] */ Boolean restarting)
{
    return NOERROR;
}

/**
 * Called when the input view is being hidden from the user.  This will
 * be called either prior to hiding the window, or prior to switching to
 * another target for editing.
 *
 * <p>The default
 * implementation uses the InputConnection to clear any active composing
 * text; you can override this (not calling the base class implementation)
 * to perform whatever behavior you would like.
 *
 * @param finishingInput If TRUE, {@link #onFinishInput} will be
 * called immediately after.
 */
ECode CInputMethodService::OnFinishInputView(
    /* [in] */ Boolean finishingInput)
{
    if (!finishingInput) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**) &ic);
        if (ic != NULL) {
            Boolean flag = FALSE;
            return ic->FinishComposingText(&flag);
        }
    }

    return NOERROR;
}

ECode CInputMethodService::OnStartCandidatesView(
    /* [in] */ IEditorInfo* info,
    /* [in] */ Boolean restarting)
{
    return NOERROR;
}

ECode CInputMethodService::OnFinishCandidatesView(
    /* [in] */ Boolean finishingInput)
{
    if (!finishingInput) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**) &ic);

        if (ic != NULL) {
            Boolean flag = FALSE;
            return ic->FinishComposingText(&flag);
        }
    }

    return NOERROR;
}

ECode CInputMethodService::OnShowInputRequested(
    /* [in] */ Int32 flags,
    /* [in] */ Boolean configChange,
    /* [out] */ Boolean* requested)
{
    assert(requested != NULL);

    Boolean tmp = FALSE;
    if (OnEvaluateInputViewShown(&tmp), !tmp) {
        *requested = FALSE;
        return NOERROR;
    }

    if ((flags & InputMethod_SHOW_EXPLICIT) == 0) {
        if (!configChange && (OnEvaluateFullscreenMode(&tmp), tmp)) {
            // Don't show if this is not explicitly requested by the user and
            // the input method is fullscreen.  That would be too disruptive.
            // However, we skip this change for a config change, since if
            // the IME is already shown we do want to go into fullscreen
            // mode at this point.
            *requested = FALSE;
            return NOERROR;
        }

        AutoPtr<IResources> res;
        AbstractInputMethodService::GetResources((IResources**) &res);
        assert(res != NULL);

        AutoPtr<IConfiguration> config;
        res->GetConfiguration((IConfiguration**) &config);

        if (((CConfiguration*)config.Get())->mKeyboard != Configuration_KEYBOARD_NOKEYS) {
            // And if the device has a hard keyboard, even if it is
            // currently hidden, don't show the input method implicitly.
            // These kinds of devices don't need it that much.
            *requested = FALSE;
            return NOERROR;
        }
    }

    if ((flags & InputMethod_SHOW_FORCED) != 0) {
        mShowInputForced = TRUE;
    }

    *requested = TRUE;
    return NOERROR;
}

ECode CInputMethodService::ShowWindow(
    /* [in] */ Boolean showInput)
{
    // if (DEBUG) Log.v(TAG, "Showing window: showInput=" + showInput
    //         + " mShowInputRequested=" + mShowInputRequested
    //         + " mWindowAdded=" + mWindowAdded
    //         + " mWindowCreated=" + mWindowCreated
    //         + " mWindowVisible=" + mWindowVisible
    //         + " mInputStarted=" + mInputStarted);

    if (mInShowWindow) {
        // Log.w(TAG, "Re-entrance in to showWindow");
        return NOERROR;
    }

    mWindowWasVisible = mWindowVisible;
    mInShowWindow = TRUE;
    ShowWindowInner(showInput);

    mWindowWasVisible = TRUE;
    mInShowWindow = FALSE;

    return NOERROR;
}

ECode CInputMethodService::HideWindow()
{
    if (mInputViewStarted) {
        // if (DEBUG) Log.v(TAG, "CALL: onFinishInputView");
        OnFinishInputView(FALSE);
    } else if (mCandidatesViewStarted) {
        // if (DEBUG) Log.v(TAG, "CALL: onFinishCandidatesView");
        OnFinishCandidatesView(FALSE);
    }

    mInputViewStarted = FALSE;
    mCandidatesViewStarted = FALSE;
    if (mWindowVisible) {
        mWindow->Hide();

        mWindowVisible = FALSE;
        OnWindowHidden();
        mWindowWasVisible = FALSE;
    }

    return NOERROR;
}

/**
 * Called when the input method window has been shown to the user, after
 * previously not being visible.  This is done after all of the UI setup
 * for the window has occurred (creating its views etc).
 */
ECode CInputMethodService::OnWindowShown()
{
    return NOERROR;
}

/**
 * Called when the input method window has been hidden from the user,
 * after previously being visible.
 */
ECode CInputMethodService::OnWindowHidden()
{
    return NOERROR;
}

ECode CInputMethodService::OnBindInput()
{
    return NOERROR;
}

ECode CInputMethodService::OnUnbindInput()
{
    return NOERROR;
}

ECode CInputMethodService::OnStartInput(
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean restarting)
{
    return NOERROR;
}

ECode CInputMethodService::OnFinishInput()
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**) &ic);

    if (ic != NULL) {
        Boolean flag = FALSE;
        return ic->FinishComposingText(&flag);
    }

    return NOERROR;
}

ECode CInputMethodService::OnDisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    return NOERROR;
}

ECode CInputMethodService::OnUpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    if (mExtractedToken != token) {
        return NOERROR;
    }

    if (text != NULL) {
        if (mExtractEditText != NULL) {
            mExtractedText = text;
            return mExtractEditText->SetExtractedText(text);
        }
    }

    return NOERROR;
}

ECode CInputMethodService::OnUpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    Boolean fullscreen = FALSE;
    IsFullscreenMode(&fullscreen);

    if (mExtractEditText != NULL && fullscreen && mExtractedText != NULL) {
        Int32 off = ((CExtractedText*)mExtractedText.Get())->mStartOffset;
        mExtractEditText->StartInternalChanges();
        newSelStart -= off;
        newSelEnd -= off;

        Int32 len = 0;
        AutoPtr<ICharSequence> text;
        mExtractEditText->GetText((ICharSequence**) &text);
        assert(text != NULL);

        text->GetLength(&len);

        if (newSelStart < 0) {
            newSelStart = 0;
        }
        else if (newSelStart > len) {
            newSelStart = len;
        }

        if (newSelEnd < 0) {
            newSelEnd = 0;
        }
        else if (newSelEnd > len) {
            newSelEnd = len;
        }

        mExtractEditText->SetSelection(newSelStart, newSelEnd);
        mExtractEditText->FinishInternalChanges();
    }

    return NOERROR;
}

ECode CInputMethodService::OnUpdateCursor(
    /* [in] */ IRect* newCursor)
{
    return NOERROR;
}

ECode CInputMethodService::RequestHideSelf(
    /* [in] */ Int32 flags)
{
    return mImm->HideSoftInputFromInputMethod(mToken, flags);
}

ECode CInputMethodService::RequestShowSelf(
    /* [in] */ Int32 flags)
{
    return mImm->ShowSoftInputFromInputMethod(mToken, flags);
}

ECode CInputMethodService::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(event != NULL && state != NULL);

    Int32 code = 0;
    event->GetKeyCode(&keyCode);
    if (code == KeyEvent_KEYCODE_BACK) {
        if (HandleBack(FALSE)) {
            event->StartTracking();

            *state = TRUE;
            return NOERROR;
        }

        *state = FALSE;
        return NOERROR;
    }

   *state = DoMovementKey(keyCode, event, MOVEMENT_DOWN);
    return NOERROR;
}

ECode CInputMethodService::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;
    return NOERROR;
}

ECode CInputMethodService::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = DoMovementKey(keyCode, event, count);
    return NOERROR;
}

ECode CInputMethodService::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(event != NULL && state != NULL);
    Int32 code = 0;
    event->GetKeyCode(&code);

    Boolean tracking = FALSE;
    event->IsTracking(&tracking);

    Boolean canceld = FALSE;
    event->IsCanceled(&canceld);

    if (code == KeyEvent_KEYCODE_BACK && tracking && !canceld) {
        *state = HandleBack(TRUE);
        return NOERROR;
    }

    *state = DoMovementKey(keyCode, event, MOVEMENT_UP);
    return NOERROR;
}

ECode CInputMethodService::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;
    return NOERROR;
}

ECode CInputMethodService::OnAppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return NOERROR;
}

ECode CInputMethodService::SendDownUpKeyEvents(
    /* [in] */ Int32 keyEventCode)
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**) &ic);

    if (ic == NULL) return NOERROR;

    long eventTime = SystemClock::UptimeMillis();

    AutoPtr<IKeyEvent> event;
    CKeyEvent::New(eventTime, eventTime,
            KeyEvent_ACTION_DOWN, keyEventCode, 0, 0, 0, 0,
            KeyEvent_FLAG_SOFT_KEYBOARD | KeyEvent_FLAG_KEEP_TOUCH_MODE,
            (IKeyEvent**) &event);

    Boolean tmpState = FALSE;
    ic->SendKeyEvent(event, &tmpState);

    CKeyEvent::New(SystemClock::UptimeMillis(), eventTime,
            KeyEvent_ACTION_UP, keyEventCode, 0, 0, 0, 0,
            KeyEvent_FLAG_SOFT_KEYBOARD | KeyEvent_FLAG_KEEP_TOUCH_MODE,
            (IKeyEvent**) &event);

    return ic->SendKeyEvent(event, &tmpState);
}

ECode CInputMethodService::SendDefaultEditorAction(
    /* [in] */ Boolean fromEnterKey,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    AutoPtr<IEditorInfo> ei;
    GetCurrentInputEditorInfo((IEditorInfo**) &ei);

    *state = FALSE;
    if (ei != NULL
        &&
        (!fromEnterKey || (((CEditorInfo*)ei.Get())->mImeOptions & EditorInfo_IME_FLAG_NO_ENTER_ACTION) == 0)
        &&
        (((CEditorInfo*)ei.Get())->mImeOptions & EditorInfo_IME_MASK_ACTION) != EditorInfo_IME_ACTION_NONE)
    {
        // If the enter key was pressed, and the editor has a default
        // action associated with pressing enter, then send it that
        // explicit action instead of the key event.
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**) &ic);
        if (ic != NULL) {
            Boolean tmpState = FALSE;
            ic->PerformEditorAction(((CEditorInfo*)ei.Get())->mImeOptions &
                    EditorInfo_IME_MASK_ACTION, &tmpState);
        }

        *state = TRUE;
    }

    return NOERROR;
}

ECode CInputMethodService::SendKeyChar(
    /* [in] */ Char16 charCode)
{
    switch (charCode) {
        case '\n': // Apps may be listening to an enter key to perform an action
            {
                Boolean ret = FALSE;
                SendDefaultEditorAction(TRUE, &ret);
                if (!ret) {
                    SendDownUpKeyEvents(KeyEvent_KEYCODE_ENTER);
                }

                break;
            }
        default:
            {
                // Make sure that digits go through any text watcher on the client side.
                if (charCode >= '0' && charCode <= '9') {
                    SendDownUpKeyEvents(charCode - '0' + KeyEvent_KEYCODE_0);
                } else {
                    AutoPtr<IInputConnection> ic;
                    GetCurrentInputConnection((IInputConnection**) &ic);

                    if (ic != NULL) {
                        ArrayOf_<Char8, 2> chs;
                        Int32 num = 0;
                        Character::ToChars(charCode, chs, 0, &num);
                        chs[num] = 0;
                        AutoPtr<ICharSequence> cs;
                        CStringWrapper::New(String(chs.GetPayload()), (ICharSequence**)&cs);
                        //ic->CommitText(String.ValueOf((char) charCode), 1);

                        Boolean tmpState = FALSE;
                        ic->CommitText(cs, 1, &tmpState);
                    }
                }
                break;
            }
    }

    return NOERROR;
}

ECode CInputMethodService::OnExtractedSelectionChanged(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IInputConnection> conn;
    GetCurrentInputConnection((IInputConnection**) &conn);
    if (conn != NULL) {
        Boolean tmpState = FALSE;
        return conn->SetSelection(start, end, &tmpState);
    }

    return NOERROR;
}

ECode CInputMethodService::OnExtractedTextClicked() {
    if (mExtractEditText == NULL) {
        return NOERROR;
    }

    Boolean has = FALSE;
    mExtractEditText->HasVerticalScrollBar(&has);

    if (has) {
        return SetCandidatesViewShown(FALSE);
    }

    return NOERROR;
}

ECode CInputMethodService::OnExtractedCursorMovement(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    if (mExtractEditText == NULL || dy == 0) {
        return NOERROR;
    }

    Boolean has = FALSE;
    mExtractEditText->HasVerticalScrollBar(&has);
    if (has) {
        return SetCandidatesViewShown(FALSE);
    }

    return NOERROR;
}

ECode CInputMethodService::OnExtractTextContextMenuItem(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**) &ic);

    if (ic != NULL) {
        Boolean tmpState = FALSE;
        ic->PerformContextMenuAction(id, &tmpState);
    }

    *state = TRUE;
    return NOERROR;
}

ECode CInputMethodService::GetTextForImeAction(
    /* [in] */ Int32 imeOptions,
    /* [out] */ ICharSequence** text)
{
    switch (imeOptions & EditorInfo_IME_MASK_ACTION) {
        case EditorInfo_IME_ACTION_NONE:
            *text = NULL;
            return NOERROR;
        case EditorInfo_IME_ACTION_GO:
            return AbstractInputMethodService::GetText(
                    0x0104035c /*com.android.internal.R.string.ime_action_go*/, text);
        case EditorInfo_IME_ACTION_SEARCH:
            return AbstractInputMethodService::GetText(
                    0x0104035d /*com.android.internal.R.string.ime_action_search*/, text);
        case EditorInfo_IME_ACTION_SEND:
            return AbstractInputMethodService::GetText(
                    0x0104035e /*com.android.internal.R.string.ime_action_send*/, text);
        case EditorInfo_IME_ACTION_NEXT:
            return AbstractInputMethodService::GetText(
                    0x0104035f /*com.android.internal.R.string.ime_action_next*/, text);
        case EditorInfo_IME_ACTION_DONE:
            return AbstractInputMethodService::GetText(
                    0x01040360 /*com.android.internal.R.string.ime_action_done*/, text);
        default:
            return AbstractInputMethodService::GetText(
                    0x01040361 /*com.android.internal.R.string.ime_action_default*/, text);
    }

    return NOERROR;
}

ECode CInputMethodService::OnUpdateExtractingVisibility(
    /* [in] */ IEditorInfo* ei)
{
    assert(ei != NULL);

    if (((CEditorInfo*)ei)->mInputType == InputType_TYPE_NULL ||
            (((CEditorInfo*)ei)->mImeOptions & EditorInfo_IME_FLAG_NO_EXTRACT_UI) != 0)
    {
        // No reason to show extract UI!
        return SetExtractViewShown(FALSE);
    }

    return SetExtractViewShown(TRUE);
}

ECode CInputMethodService::OnUpdateExtractingViews(
    /* [in] */ IEditorInfo* ei)
{
    Boolean shown = FALSE;
    IsExtractViewShown(&shown);

    if (!shown) {
        return NOERROR;
    }

    if (mExtractAccessories == NULL) {
        return NOERROR;
    }

    Boolean hasAction = ((CEditorInfo*)ei)->mActionLabel != NULL || (
            (((CEditorInfo*)ei)->mImeOptions & EditorInfo_IME_MASK_ACTION) != EditorInfo_IME_ACTION_NONE
            &&
            (((CEditorInfo*)ei)->mImeOptions & EditorInfo_IME_FLAG_NO_ACCESSORY_ACTION) == 0
            &&
            ((CEditorInfo*)ei)->mInputType != InputType_TYPE_NULL);

    if (hasAction) {
        mExtractAccessories->SetVisibility(View_VISIBLE);
        if (mExtractAction != NULL) {
            if (((CEditorInfo*)ei)->mActionLabel != NULL) {
                mExtractAction->SetText(((CEditorInfo*)ei)->mActionLabel);
            } else {
                AutoPtr<ICharSequence> text;
                GetTextForImeAction(((CEditorInfo*)ei)->mImeOptions, (ICharSequence**) &text);
                mExtractAction->SetText(text);
            }

            mExtractAction->SetOnClickListener(mActionClickListener);
        }
    } else {
        mExtractAccessories->SetVisibility(View_GONE);
        if (mExtractAction != NULL) {
            mExtractAction->SetOnClickListener(NULL);
        }
    }

    return NOERROR;
}

ECode CInputMethodService::OnExtractingInputChanged(
    /* [in] */ IEditorInfo* ei)
{
    if (((CEditorInfo*)ei)->mInputType == InputType_TYPE_NULL) {
        return RequestHideSelf(InputMethodManager_HIDE_NOT_ALWAYS);
    }

    return NOERROR;
}

ECode CInputMethodService::GetBaseContext(
    /* [out] */ IContext** context)
{
    return _InputMethodService::GetBaseContext(context);
}

ECode CInputMethodService::Attach(
    /* [in] */ IContext* ctx,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ const String& className,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IActivityManager* activityManager)
{
    return AbstractInputMethodService::Attach(ctx, apartment, className,
            token, application, activityManager);
}

ECode CInputMethodService::Create()
{
    return AbstractInputMethodService::Create();
}

ECode CInputMethodService::Start(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return AbstractInputMethodService::Start(intent, startId);
}

ECode CInputMethodService::StartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    return AbstractInputMethodService::StartCommand(intent, flags, startId, result);
}

ECode CInputMethodService::Destroy()
{
    return AbstractInputMethodService::Destroy();
}

ECode CInputMethodService::Bind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    return AbstractInputMethodService::Bind(intent, binder);
}

ECode CInputMethodService::Unbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return AbstractInputMethodService::Unbind(intent, succeeded);
}

ECode CInputMethodService::Rebind(
    /* [in] */ IIntent* intent)
{
    return AbstractInputMethodService::Rebind(intent);
}

ECode CInputMethodService::GetClassName(
    /* [out] */ String* className)
{
    return AbstractInputMethodService::GetClassName(className);
}

ECode CInputMethodService::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
    return AbstractInputMethodService::GetKeyDispatcherState(dispatcherState);
}

void CInputMethodService::InitViews()
{
    mInitialized = FALSE;
    mWindowCreated = FALSE;
    mShowInputRequested = FALSE;
    mShowInputForced = FALSE;

    // mThemeAttrs = obtainStyledAttributes(android.R.styleable.InputMethodService);
    AbstractInputMethodService::ObtainStyledAttributes(
        ArrayOf<Int32>(R_Styleable_InputMethodService, sizeof(R_Styleable_InputMethodService) / sizeof(Int32)),
        (ITypedArray**) &mThemeAttrs);

    assert(mInflater != NULL);
    mInflater->Inflate(0x0109002a /*com.android.internal.R.layout.input_method*/,
            NULL, (IView**) &mRootView);
    assert(mRootView != NULL);
    mWindow->SetContentViewEx(mRootView);

    AutoPtr<IViewTreeObserver> vto;
    mRootView->GetViewTreeObserver((IViewTreeObserver**) &vto);
    assert(vto != NULL);

    vto->AddOnComputeInternalInsetsListener(mInsetsComputer);

    //TODO
    // if (Settings.System.getInt(getContentResolver(),
    //         Settings.System.FANCY_IME_ANIMATIONS, 0) != 0) {
    //     mWindow.getWindow().setWindowAnimations(
    //             0x01030076 /*com.android.internal.R.style.Animation_InputMethodFancy*/);
    // }

    mRootView->FindViewById(0x010201b7 /*com.android.internal.R.id.fullscreenArea*/,
        (IView**) &mFullscreenArea);

    mExtractViewHidden = FALSE;
    mRootView->FindViewById(0x0102001c /*android.R.id.extractArea*/, (IView**) &mExtractFrame);

    mExtractView = NULL;
    mExtractEditText = NULL;
    mExtractAccessories = NULL;
    mExtractAction = NULL;
    mFullscreenApplied = FALSE;

    mRootView->FindViewById(0x0102001d /*android.R.id.candidatesArea*/, (IView**) &mCandidatesFrame);

    mRootView->FindViewById(0x0102001e /*android.R.id.inputArea*/, (IView**) &mInputFrame);

    mInputView = NULL;
    mIsInputViewShown = FALSE;

    mExtractFrame->SetVisibility(View_GONE);
    GetCandidatesHiddenVisibility(&mCandidatesVisibility);
    mCandidatesFrame->SetVisibility(mCandidatesVisibility);
    mInputFrame->SetVisibility(View_GONE);
}

void CInputMethodService::UpdateExtractFrameVisibility()
{
    Int32 vis = 0;
    Boolean fullscreen = FALSE;
    IsFullscreenMode(&fullscreen);
    if (fullscreen) {
        vis = mExtractViewHidden ? View_INVISIBLE : View_VISIBLE;
        mExtractFrame->SetVisibility(View_VISIBLE);
    } else {
        vis = View_VISIBLE;
        mExtractFrame->SetVisibility(View_GONE);
    }

    UpdateCandidatesVisibility(mCandidatesVisibility == View_VISIBLE);

    Int32 tmpVis = 0;
    mFullscreenArea->GetVisibility(&tmpVis);
    if (mWindowWasVisible && tmpVis != vis) {
        Int32 animRes = 0;
        mThemeAttrs->GetResourceId(vis == View_VISIBLE
                ? 1 /*com.android.internal.R.styleable.InputMethodService_imeExtractEnterAnimation*/
                : 2 /*com.android.internal.R.styleable.InputMethodService_imeExtractExitAnimation*/,
                0, &animRes);

        if (animRes != 0) {
            AutoPtr<IAnimation> ani;
            AnimationUtils::LoadAnimation((IContext*)this->Probe(EIID_IContext),
                    animRes, (IAnimation**) &ani);

            mFullscreenArea->StartAnimation(ani);
        }
    }

    mFullscreenArea->SetVisibility(vis);
}

void CInputMethodService::UpdateCandidatesVisibility(
    /* [in] */ Boolean shown)
{
    Int32 vis = View_VISIBLE;
    if (!shown) {
        GetCandidatesHiddenVisibility(&vis);
    }

    if (mCandidatesVisibility != vis) {
        mCandidatesFrame->SetVisibility(vis);
        mCandidatesVisibility = vis;
    }
}

void CInputMethodService::ShowWindowInner(
    /* [in] */ Boolean showInput)
{
    Boolean doShowInput = FALSE;
    Boolean wasVisible = mWindowVisible;
    mWindowVisible = TRUE;
    if (!mShowInputRequested) {
        if (mInputStarted) {
            if (showInput) {
                doShowInput = TRUE;
                mShowInputRequested = TRUE;
            }
        }
    } else {
        showInput = TRUE;
    }

    // if (DEBUG) Log.v(TAG, "showWindow: updating UI");
    Initialize();
    UpdateFullscreenMode();
    UpdateInputViewShown();

    if (!mWindowAdded || !mWindowCreated) {
        mWindowAdded = TRUE;
        mWindowCreated = TRUE;
        Initialize();
        // if (DEBUG) Log.v(TAG, "CALL: onCreateCandidatesView");
        AutoPtr<IView> v;
        OnCreateCandidatesView((IView**) &v);
        // if (DEBUG) Log.v(TAG, "showWindow: candidates=" + v);
        if (v != NULL) {
            SetCandidatesView(v);
        }
    }
    if (mShowInputRequested) {
        if (!mInputViewStarted) {
            // if (DEBUG) Log.v(TAG, "CALL: onStartInputView");
            mInputViewStarted = TRUE;
            OnStartInputView(mInputEditorInfo, FALSE);
        }
    } else if (!mCandidatesViewStarted) {
        // if (DEBUG) Log.v(TAG, "CALL: onStartCandidatesView");
        mCandidatesViewStarted = TRUE;
        OnStartCandidatesView(mInputEditorInfo, FALSE);
    }

    if (doShowInput) {
        StartExtractingText(FALSE);
    }

    if (!wasVisible) {
        // if (DEBUG) Log.v(TAG, "showWindow: showing!");
        OnWindowShown();
        mWindow->Show();
    }
}

void CInputMethodService::DoFinishInput()
{
    if (mInputViewStarted) {
        // if (DEBUG) Log.v(TAG, "CALL: onFinishInputView");
        OnFinishInputView(TRUE);
    } else if (mCandidatesViewStarted) {
        // if (DEBUG) Log.v(TAG, "CALL: onFinishCandidatesView");
        OnFinishCandidatesView(TRUE);
    }

    mInputViewStarted = FALSE;
    mCandidatesViewStarted = FALSE;
    if (mInputStarted) {
        // if (DEBUG) Log.v(TAG, "CALL: onFinishInput");
        OnFinishInput();
    }

    mInputStarted = FALSE;
    mStartedInputConnection = NULL;
    mCurCompletions = NULL;
}

void CInputMethodService::DoStartInput(
    /* [in] */ IInputConnection* ic,
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean restarting)
{
    if (!restarting) {
        DoFinishInput();
    }

    mInputStarted = TRUE;
    mStartedInputConnection = ic;
    mInputEditorInfo = attribute;
    Initialize();

    // if (DEBUG) Log.v(TAG, "CALL: onStartInput");
    OnStartInput(attribute, restarting);
    if (mWindowVisible) {
        if (mShowInputRequested) {
            // if (DEBUG) Log.v(TAG, "CALL: onStartInputView");
            mInputViewStarted = TRUE;
            OnStartInputView(mInputEditorInfo, restarting);
            StartExtractingText(TRUE);
        } else if (mCandidatesVisibility == View_VISIBLE) {
            // if (DEBUG) Log.v(TAG, "CALL: onStartCandidatesView");
            mCandidatesViewStarted = TRUE;
            OnStartCandidatesView(mInputEditorInfo, restarting);
        }
    }
}

Boolean CInputMethodService::HandleBack(
    /* [in] */ Boolean doIt)
{
    if (mShowInputRequested) {
        // If the soft input area is shown, back closes it and we
        // consume the back key.
        if (doIt) RequestHideSelf(0);
        return TRUE;
    } else if (mWindowVisible) {
        if (mCandidatesVisibility == View_VISIBLE) {
            // If we are showing candidates even if no input area, then
            // hide them.
            if (doIt) SetCandidatesViewShown(FALSE);
        } else {
            // If we have the window visible for some other reason --
            // most likely to show candidates -- then just get rid
            // of it.  This really shouldn't happen, but just in case...
            if (doIt) HideWindow();
        }

        return TRUE;
    }

    return FALSE;
}

void CInputMethodService::OnToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    // if (DEBUG) Log.v(TAG, "toggleSoftInput()");
    Boolean shown = FALSE;
    IsInputViewShown(&shown);

    if (shown) {
        RequestHideSelf(hideFlags);
    } else {
        RequestShowSelf(showFlags);
    }
}

void CInputMethodService::ReportExtractedMovement(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count)
{
    Int32 dx = 0, dy = 0;
    switch (keyCode) {
        case KeyEvent_KEYCODE_DPAD_LEFT:
            dx = -count;
            break;
        case KeyEvent_KEYCODE_DPAD_RIGHT:
            dx = count;
            break;
        case KeyEvent_KEYCODE_DPAD_UP:
            dy = -count;
            break;
        case KeyEvent_KEYCODE_DPAD_DOWN:
            dy = count;
            break;
    }

    OnExtractedCursorMovement(dx, dy);
}

Boolean CInputMethodService::DoMovementKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 count)
{
    Boolean extractViewShown = FALSE, inputViewShown = FALSE;
    IsExtractViewShown(&extractViewShown);
    IsInputViewShown(&inputViewShown);

    if (extractViewShown && inputViewShown && mExtractEditText != NULL) {
        // If we are in fullscreen mode, the cursor will move around
        // the extract edit text, but should NOT cause focus to move
        // to other fields.
        AutoPtr<IMovementMethod> movement;
        mExtractEditText->GetMovementMethod((IMovementMethod**) &movement);

        AutoPtr<ILayout> layout;
        mExtractEditText->GetLayout((ILayout**) &layout);
        if (movement != NULL && layout != NULL) {
            // We want our own movement method to handle the key, so the
            // cursor will properly move in our own word wrapping.
            AutoPtr<ICharSequence> text;
            mExtractEditText->GetText((ICharSequence**) &text);
            Boolean keyRet = FALSE;
            if (count == MOVEMENT_DOWN) {
                movement->OnKeyDown(mExtractEditText, ISpannable::Probe(text),
                    keyCode, event, &keyRet);

                if (keyRet)
                {
                    ReportExtractedMovement(keyCode, 1);
                    return TRUE;
                }
            } else if (count == MOVEMENT_UP) {
                movement->OnKeyUp(mExtractEditText, ISpannable::Probe(text),
                    keyCode, event, &keyRet);

                if (keyRet) {
                    return TRUE;
                }
            } else {
                movement->OnKeyOther(mExtractEditText, ISpannable::Probe(text),
                    event, &keyRet);

                if (keyRet) {
                    ReportExtractedMovement(keyCode, count);
                } else {
                    AutoPtr<IKeyEvent> down;
                    CKeyEvent::ChangeAction(event, KeyEvent_ACTION_DOWN, (IKeyEvent**) &down);

                    movement->OnKeyDown(mExtractEditText, ISpannable::Probe(text),
                            keyCode, down, &keyRet);

                    if (keyRet) {
                        AutoPtr<IKeyEvent> up;
                        CKeyEvent::ChangeAction(event, KeyEvent_ACTION_UP, (IKeyEvent**) &up);
                        movement->OnKeyUp(mExtractEditText, ISpannable::Probe(text),
                                keyCode, up, &keyRet);

                        while (--count > 0) {
                            movement->OnKeyDown(mExtractEditText,
                                    ISpannable::Probe(text), keyCode, down, &keyRet);

                            movement->OnKeyUp(mExtractEditText,
                                    ISpannable::Probe(text), keyCode, up, &keyRet);
                        }

                        ReportExtractedMovement(keyCode, count);
                    }
                }
            }
        }

        // Regardless of whether the movement method handled the key,
        // we never allow DPAD navigation to the application.
        switch (keyCode) {
            case KeyEvent_KEYCODE_DPAD_LEFT:
            case KeyEvent_KEYCODE_DPAD_RIGHT:
            case KeyEvent_KEYCODE_DPAD_UP:
            case KeyEvent_KEYCODE_DPAD_DOWN:
                return TRUE;
        }
    }

    return FALSE;
}

void CInputMethodService::StartExtractingText(
    /* [in] */ Boolean inputChanged)
{
    Boolean tmpState = FALSE, fullscreen = FALSE;
    GetCurrentInputStarted(&tmpState);
    IsFullscreenMode(&fullscreen);

    if (mExtractEditText != NULL && tmpState && fullscreen) {
        mExtractedToken++;
        AutoPtr<IExtractedTextRequest> req;
        CExtractedTextRequest::New((IExtractedTextRequest**) &req);
        ((CExtractedTextRequest*)req.Get())->mToken = mExtractedToken;
        ((CExtractedTextRequest*)req.Get())->mFlags = InputConnection_GET_TEXT_WITH_STYLES;
        ((CExtractedTextRequest*)req.Get())->mHintMaxLines = 10;
        ((CExtractedTextRequest*)req.Get())->mHintMaxChars = 10000;

        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**) &ic);
        if (ic == NULL) {
            mExtractedText = NULL;
        }
        else {
            ic->GetExtractedText(req, InputConnection_GET_EXTRACTED_TEXT_MONITOR,
                    (IExtractedText**) &mExtractedText);
        }


        if (mExtractedText == NULL || ic == NULL) {
            // Log.e(TAG, "Unexpected NULL in startExtractingText : mExtractedText = "
            //         + mExtractedText + ", input connection = " + ic);
        }

        AutoPtr<IEditorInfo> ei;
        GetCurrentInputEditorInfo((IEditorInfo**) &ei);

        try {
            mExtractEditText->StartInternalChanges();
            OnUpdateExtractingVisibility(ei);
            OnUpdateExtractingViews(ei);

            Int32 inputType = ((CEditorInfo*)ei.Get())->mInputType;
            if ((inputType & InputType_TYPE_MASK_CLASS)
                    == InputType_TYPE_CLASS_TEXT) {
                if ((inputType & InputType_TYPE_TEXT_FLAG_IME_MULTI_LINE) != 0) {
                    inputType |= InputType_TYPE_TEXT_FLAG_MULTI_LINE;
                }
            }

            mExtractEditText->SetInputType(inputType);
            mExtractEditText->SetHint(((CEditorInfo*)ei.Get())->mHintText);
            if (mExtractedText != NULL) {
                mExtractEditText->SetEnabled(TRUE);
                mExtractEditText->SetExtractedText(mExtractedText);
            } else {
                mExtractEditText->SetEnabled(FALSE);

                AutoPtr<ICharSequence> tmpText;
                CStringWrapper::New(String(""), (ICharSequence**) &tmpText);
                mExtractEditText->SetText(tmpText);
            }
        }

        mExtractEditText->FinishInternalChanges();

        if (inputChanged) {
            OnExtractingInputChanged(ei);
        }
    }
}

ECode CInputMethodService::StopSelf()
{
    return _InputMethodService::StopSelf();
}

ECode CInputMethodService::StopSelfEx(
        /* [in] */ Int32 startId)
{
    return _InputMethodService::StopSelfEx(startId);
}

ECode CInputMethodService::StopSelfResult(
        /* [in] */ Int32 startId,
        /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return _InputMethodService::StopSelfResult(startId, res);
}

ECode CInputMethodService::SetForeground(
        /* [in] */ Boolean isForeground)
{
    return _InputMethodService::SetForeground(isForeground);
}

ECode CInputMethodService::StartForeground(
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification)
{
    return _InputMethodService::StartForeground(id ,notification);
}

ECode CInputMethodService::StopForeground(
        /* [in] */ Boolean removeNotification)
{
    return _InputMethodService::StopForeground(removeNotification);
}
