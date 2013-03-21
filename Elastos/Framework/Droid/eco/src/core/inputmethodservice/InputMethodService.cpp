

#ifdef _FRAMEWORK_CORE
#include "inputmethodservice/InputMethodService.h"
#include "view/ViewTreeObserver.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/inputmethod/CEditorInfo.h"
#include "view/inputmethod/CExtractedTextRequest.h"
#include "view/inputmethod/CExtractedText.h"
#include "view/CKeyEvent.h"
#include "view/animation/AnimationUtils.h"
#include "widget/CLinearLayoutLayoutParams.h"
#include "content/CConfiguration.h"
#include "os/SystemClock.h"
#include "os/CApartment.h"
#else
#include "InputMethodService.h"

#define VALIDATE_NOT_NULL(x) if (!(x)) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }

#endif

#include <elastos/Character.h>


using namespace Elastos::Core;


static Int32 R_Styleable_InputMethodService[]= {
    0x0101022c, 0x01010268, 0x01010269
};


CString InputMethodService::TAG = "InputMethodService";
const Int32 InputMethodService::MOVEMENT_DOWN;
const Int32 InputMethodService::MOVEMENT_UP;
const Int32 InputMethodService::Insets::TOUCHABLE_INSETS_FRAME;
const Int32 InputMethodService::Insets::TOUCHABLE_INSETS_CONTENT;
const Int32 InputMethodService::Insets::TOUCHABLE_INSETS_VISIBLE;


InputMethodService::_OnComputeInternalInsetsListener::_OnComputeInternalInsetsListener(
    /* [in] */ InputMethodService* host)
    : mHost(host)
{}

InputMethodService::_OnComputeInternalInsetsListener::~_OnComputeInternalInsetsListener()
{}

PInterface InputMethodService::_OnComputeInternalInsetsListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IOnComputeInternalInsetsListener) {
        return (IOnComputeInternalInsetsListener*)this;
    }

    return NULL;
}

UInt32 InputMethodService::_OnComputeInternalInsetsListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::_OnComputeInternalInsetsListener::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::_OnComputeInternalInsetsListener::GetInterfaceID(
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

ECode InputMethodService::_OnComputeInternalInsetsListener::OnComputeInternalInsets(
    /* [in] */ IInternalInsetsInfo* info)
{
    assert(mHost != NULL && info != NULL);
    Boolean shown = FALSE;
    mHost->IsExtractViewShown(&shown);
    if (shown) {
        // In true fullscreen mode, we just say the window isn't covering
        // any content so we don't impact whatever is behind.
        AutoPtr<IDialog> dialog;
        mHost->GetWindow((IDialog**)&dialog);
        AutoPtr<IWindow> window;
        dialog->GetWindow((IWindow**)&window);
        AutoPtr<IView> decor;
        window->GetDecorView((IView**)&decor);
        Int32 h = 0;
        decor->GetHeight(&h);
        AutoPtr<IRect> contentInsets;
        info->GetContentInsets((IRect**)&contentInsets);
        contentInsets->SetTop(h);
        AutoPtr<IRect> visibleInsets;
        info->GetVisibleInsets((IRect**)&visibleInsets);
        visibleInsets->SetTop(h);
        info->SetTouchableInsets(ViewTreeObserver_InternalInsetsInfo_TOUCHABLE_INSETS_FRAME);
    }
    else {
        mHost->OnComputeInsets(mHost->mTmpInsets);
        AutoPtr<IRect> contentInsets;
        info->GetContentInsets((IRect**)&contentInsets);
        contentInsets->SetTop(mHost->mTmpInsets->mContentTopInsets);
        AutoPtr<IRect> visibleInsets;
        info->GetVisibleInsets((IRect**)&visibleInsets);
        visibleInsets->SetTop(mHost->mTmpInsets->mVisibleTopInsets);
        info->SetTouchableInsets(mHost->mTmpInsets->mTouchableInsets);
    }
    return NOERROR;
}

InputMethodService::_OnClickListener::_OnClickListener(
    /* [in] */ InputMethodService* host)
    : mHost(host)
{}

InputMethodService::_OnClickListener::~_OnClickListener()
{}

PInterface InputMethodService::_OnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

UInt32 InputMethodService::_OnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::_OnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::_OnClickListener::GetInterfaceID(
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

ECode InputMethodService::_OnClickListener::OnClick(
    /* [in] */ IView* v)
{
    AutoPtr<IEditorInfo> ei;
    mHost->GetCurrentInputEditorInfo((IEditorInfo**)&ei);
    AutoPtr<IInputConnection> ic;
    mHost->GetCurrentInputConnection((IInputConnection**)&ic);
    if (ei != NULL && ic != NULL) {
        Boolean ret = FALSE;
        Int32 actionId, imeOptions;
        if (ei->GetActionId(&actionId), actionId != 0) {
            ic->PerformEditorAction(actionId, &ret);
        }
        else if (ei->GetImeOptions(&imeOptions), (imeOptions & EditorInfo_IME_MASK_ACTION)
                != EditorInfo_IME_ACTION_NONE) {
            ic->PerformEditorAction(imeOptions & EditorInfo_IME_MASK_ACTION, &ret);
        }
    }
    return NOERROR;
}


InputMethodService::InputMethodImpl::InputMethodImpl(
    /* [in] */ InputMethodService* host)
    : AbstractInputMethodImpl((AbstractInputMethodService*)host)
    , mHost(host)
{}

InputMethodService::InputMethodImpl::~InputMethodImpl()
{}

PInterface InputMethodService::InputMethodImpl::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IAbstractInputMethodImpl == riid) {
        return (IAbstractInputMethodImpl *)this;
    }
    if (EIID_ILocalInputMethod == riid) {
        return (ILocalInputMethod *)this;
    }

    return NULL;
}

UInt32 InputMethodService::InputMethodImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::InputMethodImpl::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::InputMethodImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(ILocalInputMethod*)this) {
        *pIID = EIID_ILocalInputMethod;
    }
    else if (pObject == (IInterface*)(IAbstractInputMethodImpl*)this) {
        *pIID = EIID_IAbstractInputMethodImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::AttachToken(
    /* [in] */ IBinder* token)
{
    assert(mHost != NULL);
    if (mHost->mToken == NULL) {
        mHost->mToken = token;
        mHost->mWindow->SetToken(token);
    }
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::BindInput(
    /* [in] */ IInputBinding* binding)
{
    assert(mHost != NULL);
    mHost->mInputBinding = binding;
    binding->GetConnection((IInputConnection**)&mHost->mInputConnection);
    // if (DEBUG) Log.v(TAG, "bindInput(): binding=" + binding
    //         + " ic=" + mInputConnection);
    AutoPtr<IInputConnection> ic;
    mHost->GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic != NULL) {
        Boolean tmpState = FALSE;
        ic->ReportFullscreenMode(mHost->mIsFullscreen, &tmpState);
    }
    mHost->Initialize();
    return mHost->OnBindInput();
}

ECode InputMethodService::InputMethodImpl::UnbindInput()
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

ECode InputMethodService::InputMethodImpl::StartInput(
    /* [in] */ IInputConnection* ic,
    /* [in] */ IEditorInfo* attribute)
{
    assert(mHost != NULL);
    // if (DEBUG) Log.v(TAG, "startInput(): editor=" + attribute);
    mHost->DoStartInput(ic, attribute, FALSE);
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::RestartInput(
    /* [in] */ IInputConnection* ic,
    /* [in] */ IEditorInfo* attribute)
{
    assert(mHost != NULL);
    // if (DEBUG) Log.v(TAG, "restartInput(): editor=" + attribute);
    mHost->DoStartInput(ic, attribute, TRUE);
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::HideSoftInput(
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

ECode InputMethodService::InputMethodImpl::ShowSoftInput(
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


InputMethodService::InputMethodSessionImpl::InputMethodSessionImpl(
    /* [in] */ InputMethodService* host)
    : AbstractInputMethodSessionImpl((AbstractInputMethodService*)host)
    , mHost(host)
{}

InputMethodService::InputMethodSessionImpl::~InputMethodSessionImpl()
{}

PInterface InputMethodService::InputMethodSessionImpl::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ILocalInputMethodSession == riid) {
        return (ILocalInputMethodSession *)this;
    }

    if (EIID_IAbstractInputMethodSessionImpl == riid) {
        return (IAbstractInputMethodSessionImpl *)this;
    }

    return NULL;
}

UInt32 InputMethodService::InputMethodSessionImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::InputMethodSessionImpl::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::InputMethodSessionImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ILocalInputMethodSession*)this) {
        *pIID = EIID_ILocalInputMethodSession;
    }
    else if (pObject == (IInterface*)(IAbstractInputMethodSessionImpl*)this) {
        *pIID = EIID_IAbstractInputMethodSessionImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode InputMethodService::InputMethodSessionImpl::FinishInput()
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    // if (DEBUG) Log.v(TAG, "finishInput() in " + this);
    mHost->DoFinishInput();
    return NOERROR;
}

ECode InputMethodService::InputMethodSessionImpl::DisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    mHost->mCurCompletions = completions;
    return mHost->OnDisplayCompletions(completions);
}

ECode InputMethodService::InputMethodSessionImpl::UpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    return mHost->OnUpdateExtractedText(token, text);
}

ECode InputMethodService::InputMethodSessionImpl::UpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    return mHost->OnUpdateSelection(oldSelStart, oldSelEnd,
            newSelStart, newSelEnd, candidatesStart, candidatesEnd);
}

ECode InputMethodService::InputMethodSessionImpl::UpdateCursor(
    /* [in] */ IRect* newCursor)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    return mHost->OnUpdateCursor(newCursor);
}

ECode InputMethodService::InputMethodSessionImpl::AppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    return mHost->OnAppPrivateCommand(action, data);
}

ECode InputMethodService::InputMethodSessionImpl::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    mHost->OnToggleSoftInput(showFlags, hideFlags);
    return NOERROR;
}


InputMethodService::Insets::Insets()
    : mContentTopInsets(0)
    , mVisibleTopInsets(0)
    , mTouchableInsets(0)
{}

UInt32 InputMethodService::Insets::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::Insets::Release()
{
    return ElRefBase::Release();
}

InputMethodService::InputMethodService()
    : mTheme(0x01030054 /*android.R.style.Theme_InputMethod*/)
    , mInitialized(FALSE)
    , mWindowCreated(FALSE)
    , mWindowAdded(FALSE)
    , mWindowVisible(FALSE)
    , mWindowWasVisible(FALSE)
    , mInShowWindow(FALSE)
    , mInputStarted(FALSE)
    , mInputViewStarted(FALSE)
    , mCandidatesViewStarted(FALSE)
    , mShowInputFlags(0)
    , mShowInputRequested(FALSE)
    , mLastShowInputRequested(FALSE)
    , mCandidatesVisibility(0)
    , mCurCompletions(NULL)
    , mShowInputForced(FALSE)
    , mFullscreenApplied(FALSE)
    , mIsFullscreen(FALSE)
    , mExtractViewHidden(FALSE)
    , mExtractedToken(0)
    , mIsInputViewShown(FALSE)
    , mStatusIcon(0)
{
    mTmpInsets = new InputMethodService::Insets();
    mTmpLocation = new Int32[2];
    mInsetsComputer = new _OnComputeInternalInsetsListener(this);
    mActionClickListener = new _OnClickListener(this);
}

InputMethodService::~InputMethodService()
{
    if (mTmpLocation != NULL) {
        delete mTmpLocation;
        mTmpLocation = NULL;
    }

}

UInt32 InputMethodService::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::SetTheme(
    /* [in] */ Int32 theme)
{
    if (mWindow != NULL) {
        // throw new IllegalStateException("Must be called before onCreate()");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mTheme = theme;
    return NOERROR;
}

ECode InputMethodService::OnCreate()
{
    AbstractInputMethodService::SetTheme(mTheme);
    AbstractInputMethodService::OnCreate();
    AbstractInputMethodService::GetSystemService(
        String(Context_INPUT_METHOD_SERVICE), (IInterface**)&mImm);
    assert(mImm != NULL);
    AbstractInputMethodService::GetSystemService(
        String(Context_LAYOUT_INFLATER_SERVICE), (IInterface**)&mInflater);
    assert(mInflater != NULL);
    CSoftInputWindow::New((IContext*)this->Probe(EIID_IContext),
            mTheme, mDispatcherState, (ISoftInputWindow**)&mWindow);
    InitViews();
    AutoPtr<IWindow> window;
    mWindow->GetWindow((IWindow**)&window);
    assert(window != NULL);
    return window->SetLayout(ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_WRAP_CONTENT);
}

ECode InputMethodService::OnInitializeInterface()
{
    return NOERROR;
}

ECode InputMethodService::Initialize()
{
    if (!mInitialized) {
        mInitialized = TRUE;
        OnInitializeInterface();
    }
    return NOERROR;
}

void InputMethodService::InitViews()
{
    mInitialized = FALSE;
    mWindowCreated = FALSE;
    mShowInputRequested = FALSE;
    mShowInputForced = FALSE;

    AbstractInputMethodService::ObtainStyledAttributes(
        ArrayOf<Int32>(R_Styleable_InputMethodService, sizeof(R_Styleable_InputMethodService) / sizeof(Int32)),
        (ITypedArray**)&mThemeAttrs);
    assert(mInflater != NULL);
    mInflater->Inflate(0x0109002a /*com.android.internal.R.layout.input_method*/,
            NULL, (IView**) &mRootView);
    assert(mRootView != NULL);
    mWindow->SetContentViewEx(mRootView);
    AutoPtr<IViewTreeObserver> vto;
    mRootView->GetViewTreeObserver((IViewTreeObserver**)&vto);
    assert(vto != NULL);
    vto->AddOnComputeInternalInsetsListener(mInsetsComputer);
    //TODO
    // if (Settings.System.getInt(getContentResolver(),
    //         Settings.System.FANCY_IME_ANIMATIONS, 0) != 0) {
    //     mWindow.getWindow().setWindowAnimations(
    //             0x01030076 /*com.android.internal.R.style.Animation_InputMethodFancy*/);
    // }
    mRootView->FindViewById(0x010201b7 /*com.android.internal.R.id.fullscreenArea*/,
        (IView**)&mFullscreenArea);
    mExtractViewHidden = FALSE;
    mRootView->FindViewById(0x0102001c /*android.R.id.extractArea*/, (IView**)&mExtractFrame);
    mExtractView = NULL;
    mExtractEditText = NULL;
    mExtractAccessories = NULL;
    mExtractAction = NULL;
    mFullscreenApplied = FALSE;

    mRootView->FindViewById(0x0102001d /*android.R.id.candidatesArea*/, (IView**)&mCandidatesFrame);
    mRootView->FindViewById(0x0102001e /*android.R.id.inputArea*/, (IView**)&mInputFrame);
    mInputView = NULL;
    mIsInputViewShown = FALSE;

    mExtractFrame->SetVisibility(View_GONE);
    GetCandidatesHiddenVisibility(&mCandidatesVisibility);
    mCandidatesFrame->SetVisibility(mCandidatesVisibility);
    mInputFrame->SetVisibility(View_GONE);
}

ECode InputMethodService::OnDestroy()
{
    AbstractInputMethodService::OnDestroy();
    AutoPtr<IViewTreeObserver> observer;
    mRootView->GetViewTreeObserver((IViewTreeObserver**)&observer);
    assert(observer != NULL);
    observer->RemoveOnComputeInternalInsetsListener(mInsetsComputer);
    if (mWindowAdded) {
        return mWindow->Dismiss();
    }
    return NOERROR;
}

ECode InputMethodService::OnConfigurationChanged(
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
        GetCurrentInputConnection((IInputConnection**)&ic);
        AutoPtr<IEditorInfo> ei;
        GetCurrentInputEditorInfo((IEditorInfo**)&ei);
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
                    //todo: maybe mCurCompletions should be free first
                    mCurCompletions = completions;
                    OnDisplayCompletions(completions);
                }
            }
            else {
                HideWindow();
            }
        }
        else if (mCandidatesVisibility == View_VISIBLE) {
            // If the candidates are currently visible, make sure the
            // window is shown for them.
            ShowWindow(FALSE);
        }
        else {
            // Otherwise hide the window.
            HideWindow();
        }
    }
    return NOERROR;
}

ECode InputMethodService::OnCreateInputMethodInterface(
    /* [out] */ IAbstractInputMethodImpl** inputMethodImpl)
{
    assert(inputMethodImpl != NULL);
    *inputMethodImpl = new InputMethodImpl(this);
    (*inputMethodImpl)->AddRef();
    return NOERROR;
}

ECode InputMethodService::OnCreateInputMethodSessionInterface(
    /* [out] */ IAbstractInputMethodSessionImpl** abstractInputMethodSessionImpl)
{
    assert(abstractInputMethodSessionImpl != NULL);
    *abstractInputMethodSessionImpl = new InputMethodSessionImpl(this);
    (*abstractInputMethodSessionImpl)->AddRef();
    return NOERROR;
}

ECode InputMethodService::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    assert(inflater != NULL);
    *inflater = mInflater;
    if (*inflater != NULL) {
        (*inflater)->AddRef();
    }
    return NOERROR;
}

ECode InputMethodService::GetWindow(
    /* [out] */ IDialog** dialog)
{
    assert(dialog != NULL);
    *dialog = mWindow;
    if (*dialog != NULL) {
        (*dialog)->AddRef();
    }
    return NOERROR;
}

ECode InputMethodService::GetMaxWidth(
    /* [out] */ Int32* maxWidth)
{
    assert(maxWidth != NULL);
    AutoPtr<ILocalWindowManager> wm;
    AbstractInputMethodService::GetSystemService(
            String(Context_WINDOW_SERVICE), (IInterface**)&wm);
    assert(wm != NULL);
    AutoPtr<IDisplay> display;
    wm->GetDefaultDisplay((IDisplay**)&display);
    assert(display != NULL);
    return display->GetWidth(maxWidth);
}

/**
 * Return the currently active InputBinding for the input method, or
 * NULL if there is none.
 */
ECode InputMethodService::GetCurrentInputBinding(
    /* [out] */ IInputBinding** binding)
{
    assert(binding != NULL);
    *binding = mInputBinding;
    if (*binding != NULL) {
        (*binding)->AddRef();
    }
    return NOERROR;
}

ECode InputMethodService::GetCurrentInputConnection(
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

ECode InputMethodService::GetCurrentInputStarted(
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = mInputStarted;
    return NOERROR;
}

ECode InputMethodService::GetCurrentInputEditorInfo(
    /* [out] */ IEditorInfo** editorInfo)
{
    assert(editorInfo != NULL);
    *editorInfo = mInputEditorInfo;
    if (*editorInfo != NULL) {
        (*editorInfo)->AddRef();
    }
    return NOERROR;
}

ECode InputMethodService::UpdateFullscreenMode()
{
    Boolean tmpMode = FALSE;
    OnEvaluateFullscreenMode(&tmpMode);
    Boolean isFullscreen = mShowInputRequested && tmpMode;
    Boolean changed = mLastShowInputRequested != mShowInputRequested;
    if (mIsFullscreen != isFullscreen || !mFullscreenApplied) {
        changed = TRUE;
        mIsFullscreen = isFullscreen;
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean tmpState = FALSE;
            ic->ReportFullscreenMode(isFullscreen, &tmpState);
        }
        mFullscreenApplied = TRUE;
        Initialize();
        assert(mFullscreenArea != NULL);
        AutoPtr<IViewGroupLayoutParams> _lp;
        mFullscreenArea->GetLayoutParams((IViewGroupLayoutParams**)&_lp);
        ILinearLayoutLayoutParams* lp = ILinearLayoutLayoutParams::Probe(_lp.Get());
        assert(lp != NULL);
        if (isFullscreen) {
            assert(mThemeAttrs != NULL);
            AutoPtr<IDrawable> drawable;
            mThemeAttrs->GetDrawable(
                0 /*com.android.internal.R.styleable.InputMethodService_imeFullscreenBackground*/,
                (IDrawable**)&drawable);
            mFullscreenArea->SetBackgroundDrawable(drawable);
            lp->SetHeight(0);
            lp->SetWeight(1);
        }
        else {
            mFullscreenArea->SetBackgroundDrawable(NULL);
            lp->SetHeight(ViewGroupLayoutParams_WRAP_CONTENT);
            lp->SetWeight(0);
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
                OnCreateExtractTextView((IView**)&v);
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
        mWindow->GetWindow((IWindow**)&window);
        OnConfigureWindow(window, isFullscreen, !mShowInputRequested);
        mLastShowInputRequested = mShowInputRequested;
    }
    return NOERROR;
}

ECode InputMethodService::OnConfigureWindow(
    /* [in] */ IWindow* win,
    /* [in] */ Boolean isFullscreen,
    /* [in] */ Boolean isCandidatesOnly)
{
    assert(mWindow != NULL);
    AutoPtr<IWindow> window;
    mWindow->GetWindow((IWindow**)&window);
    assert(window != NULL);
    if (isFullscreen) {
        return window->SetLayout(ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_MATCH_PARENT);
    }
    else {
        return window->SetLayout(ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_WRAP_CONTENT);
    }
}

ECode InputMethodService::IsFullscreenMode(
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
ECode InputMethodService::OnEvaluateFullscreenMode(
    /* [out] */ Boolean* screenMode)
{
    assert(screenMode != NULL);
    AutoPtr<IResources> res;
    AbstractInputMethodService::GetResources((IResources**) &res);
    assert(res != NULL);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**) &config);
    assert(config != NULL);
    Int32 orientation;
    config->GetOrientation(&orientation);
    if (orientation != Configuration_ORIENTATION_LANDSCAPE) {
        *screenMode = FALSE;
        return NOERROR;
    }
    Int32 imeOptions;
    if (mInputEditorInfo != NULL
            && (mInputEditorInfo->GetImeOptions(&imeOptions), (imeOptions & EditorInfo_IME_FLAG_NO_FULLSCREEN) != 0)) {
        *screenMode = FALSE;
        return NOERROR;
    }
    *screenMode = TRUE;
    return NOERROR;
}

ECode InputMethodService::SetExtractViewShown(
    /* [in] */ Boolean shown)
{
    if (mExtractViewHidden == shown) {
        mExtractViewHidden = !shown;
        UpdateExtractFrameVisibility();
    }
    return NOERROR;
}

ECode InputMethodService::IsExtractViewShown(
    /* [out] */ Boolean* shown)
{
    assert(shown != NULL);
    *shown = mIsFullscreen && !mExtractViewHidden;
    return NOERROR;
}

void InputMethodService::UpdateExtractFrameVisibility()
{
    Int32 vis = 0;
    Boolean fullscreen = FALSE;
    if (IsFullscreenMode(&fullscreen), fullscreen) {
        vis = mExtractViewHidden ? View_INVISIBLE : View_VISIBLE;
        mExtractFrame->SetVisibility(View_VISIBLE);
    }
    else {
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
#ifdef _FRAMEWORK_CORE
            AnimationUtils::LoadAnimation((IContext*)this->Probe(EIID_IContext),
                    animRes, (IAnimation**) &ani);
#else
            AutoPtr<IAnimationUtils> animationUtils;
            CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
            animationUtils->LoadAnimation((IContext*)this->Probe(EIID_IContext),
                    animRes, (IAnimation**) &ani);
#endif
            mFullscreenArea->StartAnimation(ani);
        }
    }
    mFullscreenArea->SetVisibility(vis);
}

ECode InputMethodService::OnComputeInsets(
    /* [in] */ Insets* outInsets)
{
    Int32 vis = 0;
    if (mInputFrame->GetVisibility(&vis), vis == View_VISIBLE) {
        mInputFrame->GetLocationInWindow(&mTmpLocation[0], &mTmpLocation[1]);
    }
    else {
        AutoPtr<IDialog> dlg;
        GetWindow((IDialog**)&dlg);
        assert(dlg != NULL);
        AutoPtr<IWindow> win;
        dlg->GetWindow((IWindow**)&win);
        assert(win != NULL);
        AutoPtr<IView> decor;
        win->GetDecorView((IView**)&decor);
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
    }
    else {
        outInsets->mContentTopInsets = mTmpLocation[1];
    }
    if (mCandidatesFrame->GetVisibility(&vis), vis == View_VISIBLE) {
        mCandidatesFrame->GetLocationInWindow(&mTmpLocation[0], &mTmpLocation[1]);
    }
    outInsets->mVisibleTopInsets = mTmpLocation[1];
    outInsets->mTouchableInsets = Insets::TOUCHABLE_INSETS_VISIBLE;
    return NOERROR;
}

ECode InputMethodService::UpdateInputViewShown()
{
    Boolean isShown = FALSE;
    OnEvaluateInputViewShown(&isShown);
    isShown = mShowInputRequested && isShown;
    if (mIsInputViewShown != isShown && mWindowVisible) {
        mIsInputViewShown = isShown;
        mInputFrame->SetVisibility(isShown ? View_VISIBLE : View_GONE);
        if (mInputView == NULL) {
            Initialize();
            AutoPtr<IView> v;
            OnCreateInputView((IView**)&v);
            if (v != NULL) {
                return SetInputView(v);
            }
        }
    }
    return NOERROR;
}

ECode InputMethodService::IsShowInputRequested(
    /* [out] */ Boolean* requested)
{
    assert(requested != NULL);
    *requested = mShowInputRequested;
    return NOERROR;
}

ECode InputMethodService::IsInputViewShown(
    /* [out] */ Boolean* shown)
{
    assert(shown != NULL);
    *shown = mIsInputViewShown && mWindowVisible;
    return NOERROR;
}

ECode InputMethodService::OnEvaluateInputViewShown(
    /* [out] */ Boolean* shown)
{
    // assert(shown != NULL);

    // AutoPtr<IResources> res;
    // GetResources((IResources**)&res);
    // assert(res != NULL);
    // AutoPtr<IConfiguration> config;
    // res->GetConfiguration((IConfiguration**)&config);

    // Int32 keyboard, hardKeyboardHidden;
    // config->GetKeyboard(&keyboard);
    // config->GetHardKeyboardHidden(&hardKeyboardHidden);

    // *shown = keyboard == Configuration_KEYBOARD_NOKEYS
    //     || hardKeyboardHidden == Configuration_KEYBOARDHIDDEN_YES;

    //todo:
    *shown = TRUE;
    return NOERROR;
}

ECode InputMethodService::SetCandidatesViewShown(
    /* [in] */ Boolean shown)
{
    UpdateCandidatesVisibility(shown);
    if (!mShowInputRequested && mWindowVisible != shown) {
        // If we are being asked to show the candidates view while the app
        // has not asked for the input view to be shown, then we need
        // to update whether the window is shown.
        if (shown) {
            return ShowWindow(FALSE);
        }
        else {
            return HideWindow();
        }
    }
    return NOERROR;
}

void InputMethodService::UpdateCandidatesVisibility(
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

ECode InputMethodService::GetCandidatesHiddenVisibility(
    /* [out] */ Int32* vis)
{
    assert(vis != NULL);
    Boolean shown = FALSE;
    IsExtractViewShown(&shown);
    *vis = shown ? View_GONE : View_INVISIBLE;
    return NOERROR;
}

ECode InputMethodService::ShowStatusIcon(
    /* [in] */ Int32 iconResId)
{
    mStatusIcon = iconResId;
    //TODO: GetPackageName
    //return mImm->ShowStatusIcon(mToken, GetPackageName(), iconResId);
    return mImm->ShowStatusIcon(mToken, String("InputMethodService"), iconResId);
}

ECode InputMethodService::HideStatusIcon()
{
    mStatusIcon = 0;
    return mImm->HideStatusIcon(mToken);
}

ECode InputMethodService::SwitchInputMethod(
    /* [in] */ const String& id)
{
    return mImm->SetInputMethod(mToken, id);
}

ECode InputMethodService::SetExtractView(
    /* [in] */ IView* view)
{
    mExtractFrame->RemoveAllViews();
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_MATCH_PARENT,
        (IViewGroupLayoutParams**)&params);
    mExtractFrame->AddViewEx3(view, params.Get());
    mExtractView = view;
    if (view != NULL) {
        mExtractEditText = NULL;
        view->FindViewById(0x01020025 /*com.android.internal.R.id.inputExtractEditText*/,
            (IView**)&mExtractEditText);
        mExtractEditText->SetIME((IInputMethodService*)this->Probe(EIID_IInputMethodService));
        mExtractAction = NULL;
        view->FindViewById(0x010201b9 /*com.android.internal.R.id.inputExtractAction*/,
            (IView**)&mExtractAction);
        if (mExtractAction != NULL) {
            mExtractAccessories = NULL;
            view->FindViewById(0x010201b8 /*com.android.internal.R.id.inputExtractAccessories*/,
                    (IView**)&mExtractAccessories);
        }
        StartExtractingText(FALSE);
        return NOERROR;
    }
    else {
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
ECode InputMethodService::SetCandidatesView(
    /* [in] */ IView* view)
{
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
ECode InputMethodService::SetInputView(
    /* [in] */ IView* view)
{
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
ECode InputMethodService::OnCreateExtractTextView(
    /* [out] */ IView** view)
{
    assert(mInflater != NULL);
    return mInflater->Inflate(0x0109002b /*com.android.internal.R.layout.input_method_extract_view*/,
            NULL, view);
}

ECode InputMethodService::OnCreateCandidatesView(
    /* [out] */ IView** view)
{
    assert(view != NULL);
    *view = NULL;
    return NOERROR;
}

ECode InputMethodService::OnCreateInputView(
    /* [out] */ IView** view)
{
    assert(view != NULL);
    *view = NULL;
    return NOERROR;
}

ECode InputMethodService::OnStartInputView(
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
ECode InputMethodService::OnFinishInputView(
    /* [in] */ Boolean finishingInput)
{
    if (!finishingInput) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean flag = FALSE;
            return ic->FinishComposingText(&flag);
        }
    }
    return NOERROR;
}

ECode InputMethodService::OnStartCandidatesView(
    /* [in] */ IEditorInfo* info,
    /* [in] */ Boolean restarting)
{
    return NOERROR;
}

ECode InputMethodService::OnFinishCandidatesView(
    /* [in] */ Boolean finishingInput)
{
    if (!finishingInput) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean flag = FALSE;
            return ic->FinishComposingText(&flag);
        }
    }
    return NOERROR;
}

ECode InputMethodService::OnShowInputRequested(
    /* [in] */ Int32 flags,
    /* [in] */ Boolean configChange,
    /* [out] */ Boolean* requested)
{
    assert(requested != NULL);
    Boolean result = FALSE;
    if (OnEvaluateInputViewShown(&result), !result) {
        *requested = FALSE;
        return NOERROR;
    }
    if ((flags & InputMethod_SHOW_EXPLICIT) == 0) {
        if (!configChange && (OnEvaluateFullscreenMode(&result), result)) {
            // Don't show if this is not explicitly requested by the user and
            // the input method is fullscreen.  That would be too disruptive.
            // However, we skip this change for a config change, since if
            // the IME is already shown we do want to go into fullscreen
            // mode at this point.
            *requested = FALSE;
            return NOERROR;
        }
        AutoPtr<IResources> res;
        AbstractInputMethodService::GetResources((IResources**)&res);
        AutoPtr<IConfiguration> config;
        res->GetConfiguration((IConfiguration**)&config);
        Int32 keyboard;
        config->GetKeyboard(&keyboard);
        if (keyboard != Configuration_KEYBOARD_NOKEYS) {
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

ECode InputMethodService::ShowWindow(
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

void InputMethodService::ShowWindowInner(
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
    }
    else {
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
        OnCreateCandidatesView((IView**)&v);
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
    }
    else if (!mCandidatesViewStarted) {
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

ECode InputMethodService::HideWindow()
{
    if (mInputViewStarted) {
        // if (DEBUG) Log.v(TAG, "CALL: onFinishInputView");
        OnFinishInputView(FALSE);
    }
    else if (mCandidatesViewStarted) {
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
ECode InputMethodService::OnWindowShown()
{
    return NOERROR;
}

/**
 * Called when the input method window has been hidden from the user,
 * after previously being visible.
 */
ECode InputMethodService::OnWindowHidden()
{
    return NOERROR;
}

ECode InputMethodService::OnBindInput()
{
    return NOERROR;
}

ECode InputMethodService::OnUnbindInput()
{
    return NOERROR;
}

ECode InputMethodService::OnStartInput(
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean restarting)
{
    return NOERROR;
}

void InputMethodService::DoFinishInput()
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

void InputMethodService::DoStartInput(
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
        }
        else if (mCandidatesVisibility == View_VISIBLE) {
            // if (DEBUG) Log.v(TAG, "CALL: onStartCandidatesView");
            mCandidatesViewStarted = TRUE;
            OnStartCandidatesView(mInputEditorInfo, restarting);
        }
    }
}

ECode InputMethodService::OnFinishInput()
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic != NULL) {
        Boolean flag = FALSE;
        return ic->FinishComposingText(&flag);
    }
    return NOERROR;
}

ECode InputMethodService::OnDisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    return NOERROR;
}

ECode InputMethodService::OnUpdateExtractedText(
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

ECode InputMethodService::OnUpdateSelection(
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
        Int32 off;
        mExtractedText->GetStartOffset(&off);
        mExtractEditText->StartInternalChanges();
        newSelStart -= off;
        newSelEnd -= off;
        Int32 len = 0;
        AutoPtr<ICharSequence> text;
        mExtractEditText->GetText((ICharSequence**)&text);
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

ECode InputMethodService::OnUpdateCursor(
    /* [in] */ IRect* newCursor)
{
    return NOERROR;
}

ECode InputMethodService::RequestHideSelf(
    /* [in] */ Int32 flags)
{
    return mImm->HideSoftInputFromInputMethod(mToken, flags);
}

ECode InputMethodService::RequestShowSelf(
    /* [in] */ Int32 flags)
{
    return mImm->ShowSoftInputFromInputMethod(mToken, flags);
}

Boolean InputMethodService::HandleBack(
    /* [in] */ Boolean doIt)
{
    if (mShowInputRequested) {
        // If the soft input area is shown, back closes it and we
        // consume the back key.
        if (doIt) RequestHideSelf(0);
        return TRUE;
    }
    else if (mWindowVisible) {
        if (mCandidatesVisibility == View_VISIBLE) {
            // If we are showing candidates even if no input area, then
            // hide them.
            if (doIt) SetCandidatesViewShown(FALSE);
        }
        else {
            // If we have the window visible for some other reason --
            // most likely to show candidates -- then just get rid
            // of it.  This really shouldn't happen, but just in case...
            if (doIt) HideWindow();
        }
        return TRUE;
    }
    return FALSE;
}

ECode InputMethodService::OnKeyDown(
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

ECode InputMethodService::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;
    return NOERROR;
}

ECode InputMethodService::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = DoMovementKey(keyCode, event, count);
    return NOERROR;
}

ECode InputMethodService::OnKeyUp(
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

ECode InputMethodService::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;
    return NOERROR;
}

ECode InputMethodService::OnAppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return NOERROR;
}

void InputMethodService::OnToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    // if (DEBUG) Log.v(TAG, "toggleSoftInput()");
    Boolean shown = FALSE;
    IsInputViewShown(&shown);
    if (shown) {
        RequestHideSelf(hideFlags);
    }
    else {
        RequestShowSelf(showFlags);
    }
}

void InputMethodService::ReportExtractedMovement(
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

Boolean InputMethodService::DoMovementKey(
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
        mExtractEditText->GetMovementMethod((IMovementMethod**)&movement);
        AutoPtr<ILayout> layout;
        mExtractEditText->GetLayout((ILayout**)&layout);
        if (movement != NULL && layout != NULL) {
            // We want our own movement method to handle the key, so the
            // cursor will properly move in our own word wrapping.
            AutoPtr<ICharSequence> text;
            mExtractEditText->GetText((ICharSequence**)&text);
            Boolean keyRet = FALSE;
            if (count == MOVEMENT_DOWN) {
                movement->OnKeyDown(mExtractEditText, ISpannable::Probe(text),
                    keyCode, event, &keyRet);
                if (keyRet) {
                    ReportExtractedMovement(keyCode, 1);
                    return TRUE;
                }
            }
            else if (count == MOVEMENT_UP) {
                movement->OnKeyUp(mExtractEditText, ISpannable::Probe(text),
                    keyCode, event, &keyRet);
                if (keyRet) {
                    return TRUE;
                }
            }
            else {
                movement->OnKeyOther(mExtractEditText, ISpannable::Probe(text),
                    event, &keyRet);
                if (keyRet) {
                    ReportExtractedMovement(keyCode, count);
                }
                else {
                    AutoPtr<IKeyEvent> down;
                    // CKeyEvent::ChangeAction(event, KeyEvent_ACTION_DOWN, (IKeyEvent**)&down);
                    movement->OnKeyDown(mExtractEditText, ISpannable::Probe(text),
                            keyCode, down, &keyRet);
                    if (keyRet) {
                        AutoPtr<IKeyEvent> up;
                        // CKeyEvent::ChangeAction(event, KeyEvent_ACTION_UP, (IKeyEvent**)&up);
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

ECode InputMethodService::SendDownUpKeyEvents(
    /* [in] */ Int32 keyEventCode)
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic == NULL) return NOERROR;
    long eventTime = 0;//SystemClock::UptimeMillis();
    AutoPtr<IKeyEvent> event;
    CKeyEvent::New(eventTime, eventTime,
            KeyEvent_ACTION_DOWN, keyEventCode, 0, 0, 0, 0,
            KeyEvent_FLAG_SOFT_KEYBOARD | KeyEvent_FLAG_KEEP_TOUCH_MODE,
            (IKeyEvent**)&event);
    Boolean tmpState = FALSE;
    ic->SendKeyEvent(event, &tmpState);
    CKeyEvent::New(0/*SystemClock::UptimeMillis()*/, eventTime,
            KeyEvent_ACTION_UP, keyEventCode, 0, 0, 0, 0,
            KeyEvent_FLAG_SOFT_KEYBOARD | KeyEvent_FLAG_KEEP_TOUCH_MODE,
            (IKeyEvent**) &event);
    return ic->SendKeyEvent(event, &tmpState);
}

ECode InputMethodService::SendDefaultEditorAction(
    /* [in] */ Boolean fromEnterKey,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    AutoPtr<IEditorInfo> ei;
    GetCurrentInputEditorInfo((IEditorInfo**)&ei);
    Int32 imeOptions;
    if (ei != NULL &&
        (!fromEnterKey || (ei->GetImeOptions(&imeOptions), (imeOptions & EditorInfo_IME_FLAG_NO_ENTER_ACTION) == 0)) &&
        (imeOptions & EditorInfo_IME_MASK_ACTION) != EditorInfo_IME_ACTION_NONE) {
        // If the enter key was pressed, and the editor has a default
        // action associated with pressing enter, then send it that
        // explicit action instead of the key event.
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean tmpState = FALSE;
            ic->PerformEditorAction(imeOptions & EditorInfo_IME_MASK_ACTION, &tmpState);
        }
        *state = TRUE;
        return NOERROR;
    }
    *state = FALSE;
    return NOERROR;
}

ECode InputMethodService::SendKeyChar(
    /* [in] */ Char32 charCode)
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
                }
                else {
                    AutoPtr<IInputConnection> ic;
                    GetCurrentInputConnection((IInputConnection**)&ic);
                    if (ic != NULL) {
                        ArrayOf_<Char8, 5> chs;
                        Int32 num = 0;
                        Character::ToChars(charCode, chs, 0, &num);
                        chs[num] = 0;
                        AutoPtr<ICharSequence> cs;
                        CStringWrapper::New(String(chs.GetPayload()), (ICharSequence**)&cs);
                        Boolean tmpState = FALSE;
                        ic->CommitText(cs, 1, &tmpState);
                    }
                }
                break;
            }
    }
    return NOERROR;
}

ECode InputMethodService::OnExtractedSelectionChanged(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IInputConnection> conn;
    GetCurrentInputConnection((IInputConnection**)&conn);
    if (conn != NULL) {
        Boolean tmpState = FALSE;
        return conn->SetSelection(start, end, &tmpState);
    }
    return NOERROR;
}

ECode InputMethodService::OnExtractedTextClicked()
{
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

ECode InputMethodService::OnExtractedCursorMovement(
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

ECode InputMethodService::OnExtractTextContextMenuItem(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic != NULL) {
        Boolean tmpState = FALSE;
        ic->PerformContextMenuAction(id, &tmpState);
    }
    *state = TRUE;
    return NOERROR;
}

ECode InputMethodService::GetTextForImeAction(
    /* [in] */ Int32 imeOptions,
    /* [out] */ ICharSequence** text)
{
    assert(text != NULL);
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
}

ECode InputMethodService::OnUpdateExtractingVisibility(
    /* [in] */ IEditorInfo* ei)
{
    assert(ei != NULL);
    Int32 inputType, imeOptions;
    ei->GetInputType(&inputType);
    ei->GetImeOptions(&imeOptions);
    if (inputType == InputType_TYPE_NULL ||
            (imeOptions & EditorInfo_IME_FLAG_NO_EXTRACT_UI) != 0) {
        // No reason to show extract UI!
        return SetExtractViewShown(FALSE);
    }
    return SetExtractViewShown(TRUE);
}

ECode InputMethodService::OnUpdateExtractingViews(
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
    AutoPtr<ICharSequence> actionLabel;
    ei->GetActionLabel((ICharSequence**)&actionLabel);
    Int32 imeOptions, inputType;
    ei->GetImeOptions(&imeOptions);
    ei->GetInputType(&inputType);
    Boolean hasAction = actionLabel != NULL || (
            (imeOptions & EditorInfo_IME_MASK_ACTION) != EditorInfo_IME_ACTION_NONE &&
            (imeOptions & EditorInfo_IME_FLAG_NO_ACCESSORY_ACTION) == 0 &&
            inputType != InputType_TYPE_NULL);
    if (hasAction) {
        mExtractAccessories->SetVisibility(View_VISIBLE);
        if (mExtractAction != NULL) {
            if (actionLabel != NULL) {
                mExtractAction->SetText(actionLabel);
            }
            else {
                AutoPtr<ICharSequence> text;
                GetTextForImeAction(imeOptions, (ICharSequence**)&text);
                mExtractAction->SetText(text);
            }

            mExtractAction->SetOnClickListener(mActionClickListener);
        }
    }
    else {
        mExtractAccessories->SetVisibility(View_GONE);
        if (mExtractAction != NULL) {
            mExtractAction->SetOnClickListener(NULL);
        }
    }
    return NOERROR;
}

ECode InputMethodService::OnExtractingInputChanged(
    /* [in] */ IEditorInfo* ei)
{
    Int32 inputType;
    ei->GetInputType(&inputType);
    if (inputType == InputType_TYPE_NULL) {
        return RequestHideSelf(InputMethodManager_HIDE_NOT_ALWAYS);
    }
    return NOERROR;
}

void InputMethodService::StartExtractingText(
    /* [in] */ Boolean inputChanged)
{
    Boolean tmpState = FALSE, fullscreen = FALSE;
    GetCurrentInputStarted(&tmpState);
    IsFullscreenMode(&fullscreen);
    if (mExtractEditText != NULL && tmpState && fullscreen) {
        mExtractedToken++;
        AutoPtr<IExtractedTextRequest> req;
        CExtractedTextRequest::New((IExtractedTextRequest**)&req);
        req->SetToken(mExtractedToken);
        req->SetFlags(InputConnection_GET_TEXT_WITH_STYLES);
        req->SetHintMaxLines(10);
        req->SetHintMaxChars(10000);
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic == NULL) {
            mExtractedText = NULL;
        }
        else {
            mExtractedText = NULL;
            ic->GetExtractedText(req, InputConnection_GET_EXTRACTED_TEXT_MONITOR,
                    (IExtractedText**)&mExtractedText);
        }
        if (mExtractedText == NULL || ic == NULL) {
            // Log.e(TAG, "Unexpected NULL in startExtractingText : mExtractedText = "
            //         + mExtractedText + ", input connection = " + ic);
        }
        AutoPtr<IEditorInfo> ei;
        GetCurrentInputEditorInfo((IEditorInfo**)&ei);

        // try {
        mExtractEditText->StartInternalChanges();
        OnUpdateExtractingVisibility(ei);
        OnUpdateExtractingViews(ei);
        Int32 inputType;
        ei->GetInputType(&inputType);
        if ((inputType & InputType_TYPE_MASK_CLASS)
                == InputType_TYPE_CLASS_TEXT) {
            if ((inputType & InputType_TYPE_TEXT_FLAG_IME_MULTI_LINE) != 0) {
                inputType |= InputType_TYPE_TEXT_FLAG_MULTI_LINE;
            }
        }
        mExtractEditText->SetInputType(inputType);
        AutoPtr<ICharSequence> hintText;
        ei->GetHintText((ICharSequence**)&hintText);
        mExtractEditText->SetHint(hintText);
        if (mExtractedText != NULL) {
            mExtractEditText->SetEnabled(TRUE);
            mExtractEditText->SetExtractedText(mExtractedText);
        }
        else {
            mExtractEditText->SetEnabled(FALSE);
            AutoPtr<ICharSequence> tmpText;
            CStringWrapper::New(String(""), (ICharSequence**)&tmpText);
            mExtractEditText->SetText(tmpText);
        }
        // } finally {
        //     eet.finishInternalChanges();
        // }
        mExtractEditText->FinishInternalChanges();

        if (inputChanged) {
            OnExtractingInputChanged(ei);
        }
    }
}

ECode InputMethodService::GetBaseContext(
    /* [out] */ IContext** context)
{
    return AbstractInputMethodService::GetBaseContext(context);
}

ECode InputMethodService::Attach(
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

ECode InputMethodService::Create()
{
    return AbstractInputMethodService::Create();
}

ECode InputMethodService::Start(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return AbstractInputMethodService::Start(intent, startId);
}

ECode InputMethodService::StartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    return AbstractInputMethodService::StartCommand(intent, flags, startId, result);
}

ECode InputMethodService::Destroy()
{
    return AbstractInputMethodService::Destroy();
}

ECode InputMethodService::Bind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    return AbstractInputMethodService::Bind(intent, binder);
}

ECode InputMethodService::Unbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return AbstractInputMethodService::Unbind(intent, succeeded);
}

ECode InputMethodService::Rebind(
    /* [in] */ IIntent* intent)
{
    return AbstractInputMethodService::Rebind(intent);
}

ECode InputMethodService::GetClassName(
    /* [out] */ String* className)
{
    return AbstractInputMethodService::GetClassName(className);
}

ECode InputMethodService::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
    return AbstractInputMethodService::GetKeyDispatcherState(dispatcherState);
}
