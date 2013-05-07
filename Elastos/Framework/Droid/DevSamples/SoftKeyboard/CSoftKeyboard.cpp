
#include "CSoftKeyboard.h"
#include "LatinKeyboard.h"
#include <elastos/System.h>


#define VALIDATE_NOT_NULL(x) if (!(x)) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }
#define ASSERT_SUCCEEDED(expr) assert(SUCCEEDED(expr));

// #define FAIL_RETURN(expr) \
//     do { \
//         ECode ec = expr; \
//         if (FAILED(ec)) return ec; \
//     } while(0);

const Boolean CSoftKeyboard::PROCESS_HARD_KEYS;

CSoftKeyboard::CSoftKeyboard()
    : mCompletions(NULL)
    , mComposing(100)
    , mPredictionOn(FALSE)
    , mCompletionOn(FALSE)
    , mLastDisplayWidth(0)
    , mCapsLock(FALSE)
    , mLastShiftTime(0)
    , mMetaState(0)
{}

CSoftKeyboard::~CSoftKeyboard()
{
    if (mCompletions != NULL) {
        for (Int32 i = 0; i < mCompletions->GetLength(); ++i) {
            (*mCompletions)[i] = NULL;
        }
        ArrayOf< AutoPtr<ICompletionInfo> >::Free(mCompletions);
    }
}

PInterface CSoftKeyboard::Probe(
    /* [in] */ REIID riid)
{
    return _CSoftKeyboard::Probe(riid);
}

ECode CSoftKeyboard::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    VALIDATE_NOT_NULL(assetManager);
    return InputMethodService::GetAssets(assetManager);
}

ECode CSoftKeyboard::GetResources(
    /* [out] */ IResources** resources)
{
    VALIDATE_NOT_NULL(resources)
    return InputMethodService::GetResources(resources);
}

ECode CSoftKeyboard::GetCapsuleManager(
    /* [out] */ ILocalCapsuleManager** capsuleManager)
{
    VALIDATE_NOT_NULL(capsuleManager);
    return InputMethodService::GetCapsuleManager(capsuleManager);
}

ECode CSoftKeyboard::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    VALIDATE_NOT_NULL(resolver);
    return InputMethodService::GetContentResolver(resolver);
}

ECode CSoftKeyboard::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    return InputMethodService::GetApplicationContext(ctx);
}

ECode CSoftKeyboard::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    return InputMethodService::GetText(resId, text);
}

ECode CSoftKeyboard::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return InputMethodService::GetString(resId, str);
}

ECode CSoftKeyboard::SetTheme(
    /* [in] */ Int32 resid)
{
    return InputMethodService::SetTheme(resid);
}

ECode CSoftKeyboard::GetTheme(
    /* [out] */ ITheme** theme)
{
    VALIDATE_NOT_NULL(theme);
    return InputMethodService::GetTheme(theme);
}

ECode CSoftKeyboard::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32> & attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    return InputMethodService::ObtainStyledAttributes(attrs, styles);
}

ECode CSoftKeyboard::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32> & attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    return InputMethodService::ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode CSoftKeyboard::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32> & attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    return InputMethodService::ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode CSoftKeyboard::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32> & attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    return InputMethodService::ObtainStyledAttributesEx3(set, attrs, defStyleAttr, defStyleRes, styles);
}

ECode CSoftKeyboard::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    VALIDATE_NOT_NULL(loader);
    return InputMethodService::GetClassLoader(loader);
}

ECode CSoftKeyboard::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    VALIDATE_NOT_NULL(capsuleName);
    return InputMethodService::GetCapsuleName(capsuleName);
}

ECode CSoftKeyboard::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return InputMethodService::GetApplicationInfo(info);
}

ECode CSoftKeyboard::GetCapsuleResourcePath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    return InputMethodService::GetCapsuleResourcePath(path);
}

ECode CSoftKeyboard::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    VALIDATE_NOT_NULL(filesDir);
    return InputMethodService::GetFilesDir(filesDir);
}

ECode CSoftKeyboard::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    VALIDATE_NOT_NULL(filesDir);
    return InputMethodService::GetExternalFilesDir(type, filesDir);
}

ECode CSoftKeyboard::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    VALIDATE_NOT_NULL(cacheDir);
    return InputMethodService::GetCacheDir(cacheDir);
}

ECode CSoftKeyboard::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir);
    return InputMethodService::GetDir(name, mode, dir);
}

ECode CSoftKeyboard::StartActivity(
    /* [in] */ IIntent* intent)
{
    return InputMethodService::StartActivity(intent);
}

ECode CSoftKeyboard::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return InputMethodService::StartIntentSender(intent, fillInIntent,
        flagsMask, flagsValues, extraFlags);
}

ECode CSoftKeyboard::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return InputMethodService::SendBroadcast(intent);
}

ECode CSoftKeyboard::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    return InputMethodService::StartService(service, name);
}

ECode CSoftKeyboard::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return InputMethodService::StopService(service, succeeded);
}

ECode CSoftKeyboard::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return InputMethodService::BindService(service, conn, flags, succeeded);
}

ECode CSoftKeyboard::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return InputMethodService::UnbindService(conn);
}

ECode CSoftKeyboard::GetSystemService(
    /* [in] */ CString name,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return InputMethodService::GetSystemService(name, object);
}

ECode CSoftKeyboard::CreateCapsuleContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    return InputMethodService::CreateCapsuleContext(capsuleName, flags, ctx);
}

ECode CSoftKeyboard::CheckCallingPermission(
    /* [in] */ CString permission,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return InputMethodService::CheckCallingPermission(permission, value);
}

ECode CSoftKeyboard::EnforceCallingOrSelfPermission(
    /* [in] */ CString permission,
    /* [in] */ CString message)
{
    return InputMethodService::EnforceCallingOrSelfPermission(permission, message);
}

ECode CSoftKeyboard::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return InputMethodService::RevokeUriPermission(uri, modeFlags);
}

ECode CSoftKeyboard::CheckCallingOrSelfPermission(
    /* [in] */ CString permission,
    /* [out] */ Int32* perm)
{
    VALIDATE_NOT_NULL(perm);
    return InputMethodService::CheckCallingOrSelfPermission(permission, perm);
}

ECode CSoftKeyboard::CheckPermission(
	/* [in] */ CString permission,
	/* [in] */ Int32 pid,
	/* [in] */ Int32 uid,
	/* [out] */ Int32 * result)
{
	return InputMethodService::CheckPermission(permission, pid, uid, result);
}

ECode CSoftKeyboard::CheckUriPermission(
	/* [in] */ IUri * uri,
	/* [in] */ CString readPermission,
	/* [in] */ CString writePermission,
	/* [in] */ Int32 pid,
	/* [in] */ Int32 uid,
	/* [in] */ Int32 modeFlags,
	/* [out] */ Int32 * result)
{
	return InputMethodService::CheckUriPermission(
            uri,
            readPermission,
            writePermission,
            pid,
            uid,
            modeFlags,
            result);
}

ECode CSoftKeyboard::CheckUriPermissionEx(
	/* [in] */ IUri * uri,
	/* [in] */ Int32 pid,
	/* [in] */ Int32 uid,
	/* [in] */ Int32 modeFlags,
	/* [out] */ Int32 * result)
{
	return InputMethodService::CheckUriPermissionEx(
            uri,
            pid,
            uid,
            modeFlags,
            result);
}

ECode CSoftKeyboard::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return InputMethodService::GrantUriPermission(toCapsule, uri, modeFlags);
}

ECode CSoftKeyboard::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    return InputMethodService::GetBaseContext(ctx);
}

ECode CSoftKeyboard::Attach(
    /* [in] */ IContext* ctx,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ const String& className,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IActivityManager* activityManager)
{
    return InputMethodService::Attach(ctx, apartment, className, token, application, activityManager);
}

ECode CSoftKeyboard::Create()
{
    InputMethodService::Create();
    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    ASSERT_SUCCEEDED(res->GetString(0x7f070001/*R.string.word_separators*/, &mWordSeparators));
    return NOERROR;
}

ECode CSoftKeyboard::Start(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return InputMethodService::Start(intent, startId);
}

ECode CSoftKeyboard::StartCommand(
    /* [in] */ IIntent * intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return InputMethodService::StartCommand(intent, flags, startId, result);
}

ECode CSoftKeyboard::Destroy()
{
    return InputMethodService::Destroy();
}

ECode CSoftKeyboard::Bind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    return InputMethodService::Bind(intent, binder);
}

ECode CSoftKeyboard::Unbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return InputMethodService::Unbind(intent, succeeded);
}

ECode CSoftKeyboard::Rebind(
    /* [in] */ IIntent* intent)
{
    return InputMethodService::Rebind(intent);
}

ECode CSoftKeyboard::GetClassName(
    /* [out] */ String* className)
{
    VALIDATE_NOT_NULL(className);
    return InputMethodService::GetClassName(className);
}

ECode CSoftKeyboard::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
    VALIDATE_NOT_NULL(dispatcherState);
    return InputMethodService::GetKeyDispatcherState(dispatcherState);
}

ECode CSoftKeyboard::OnCreateInputMethodInterface(
    /* [out] */ IAbstractInputMethodImpl** inputMethodImpl)
{
    VALIDATE_NOT_NULL(inputMethodImpl);
    return InputMethodService::OnCreateInputMethodInterface(inputMethodImpl);
}

ECode CSoftKeyboard::OnCreateInputMethodSessionInterface(
    /* [out] */ IAbstractInputMethodSessionImpl** abstractInputMethodSessionImpl)
{
    VALIDATE_NOT_NULL(abstractInputMethodSessionImpl)
    return InputMethodService::OnCreateInputMethodSessionInterface(abstractInputMethodSessionImpl);
}

ECode CSoftKeyboard::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);
    return InputMethodService::OnTrackballEvent(event, state);
}

ECode CSoftKeyboard::OnInitializeInterface()
{
    if (mQwertyKeyboard != NULL) {
        // Configuration changes can happen after the keyboard gets recreated,
        // so we need to be able to re-build the keyboards if the available
        // space has changed.
        Int32 displayWidth = 0;
        GetMaxWidth(&displayWidth);
        if (displayWidth == mLastDisplayWidth) {
            return NOERROR;
        }
        mLastDisplayWidth = displayWidth;
    }

    AutoPtr<IContext> ctx = (IContext*)this->Probe(EIID_IContext);
    assert(ctx != NULL);
    mQwertyKeyboard = new LatinKeyboard(ctx, 0x7f040001 /*R.xml.qwerty*/);
    mSymbolsKeyboard = new LatinKeyboard(ctx, 0x7f040002 /*R.xml.symbols*/);
    mSymbolsShiftedKeyboard = new LatinKeyboard(ctx, 0x7f040003 /*R.xml.symbols_shift*/);
    return NOERROR;
}

ECode CSoftKeyboard::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater);
    return InputMethodService::GetLayoutInflater(inflater);
}

ECode CSoftKeyboard::GetWindow(
    /* [out] */ IDialog** dialog)
{
    VALIDATE_NOT_NULL(dialog);
    return InputMethodService::GetWindow(dialog);
}

ECode CSoftKeyboard::GetMaxWidth(
    /* [out] */ Int32* maxWidth)
{
    VALIDATE_NOT_NULL(maxWidth);
    return InputMethodService::GetMaxWidth(maxWidth);
}

ECode CSoftKeyboard::GetCurrentInputBinding(
    /* [out] */ IInputBinding** binding)
{
    VALIDATE_NOT_NULL(binding);
    return InputMethodService::GetCurrentInputBinding(binding);
}

ECode CSoftKeyboard::GetCurrentInputConnection(
    /* [out] */ IInputConnection** inputConnection)
{
    VALIDATE_NOT_NULL(inputConnection);
    return InputMethodService::GetCurrentInputConnection(inputConnection);
}

ECode CSoftKeyboard::GetCurrentInputStarted(
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);
    return InputMethodService::GetCurrentInputStarted(state);
}

ECode CSoftKeyboard::GetCurrentInputEditorInfo(
    /* [out] */ IEditorInfo** editorInfo)
{
    VALIDATE_NOT_NULL(editorInfo);
    return InputMethodService::GetCurrentInputEditorInfo(editorInfo);
}

ECode CSoftKeyboard::UpdateFullscreenMode()
{
    return InputMethodService::UpdateFullscreenMode();
}

ECode CSoftKeyboard::OnConfigureWindow(
    /* [in] */ IWindow* win,
    /* [in] */ Boolean isFullscreen,
    /* [in] */ Boolean isCandidatesOnly)
{
    return InputMethodService::OnConfigureWindow(win, isFullscreen, isCandidatesOnly);
}

ECode CSoftKeyboard::IsFullscreenMode(
    /* [out] */ Boolean* fullScreenMode)
{
    VALIDATE_NOT_NULL(fullScreenMode);
    return InputMethodService::IsFullscreenMode(fullScreenMode);
}

ECode CSoftKeyboard::OnEvaluateFullscreenMode(
    /* [out] */ Boolean* screenMode)
{
    VALIDATE_NOT_NULL(screenMode);
    return InputMethodService::OnEvaluateFullscreenMode(screenMode);
}

ECode CSoftKeyboard::SetExtractViewShown(
    /* [in] */ Boolean shown)
{
    return InputMethodService::SetExtractViewShown(shown);
}

ECode CSoftKeyboard::IsExtractViewShown(
    /* [out] */ Boolean* shown)
{
    VALIDATE_NOT_NULL(shown);
    return InputMethodService::IsExtractViewShown(shown);
}

ECode CSoftKeyboard::UpdateInputViewShown()
{
    return InputMethodService::UpdateInputViewShown();
}

ECode CSoftKeyboard::IsShowInputRequested(
    /* [out] */ Boolean* requested)
{
    VALIDATE_NOT_NULL(requested);
    return InputMethodService::IsShowInputRequested(requested);
}

ECode CSoftKeyboard::IsInputViewShown(
    /* [out] */ Boolean* shown)
{
    VALIDATE_NOT_NULL(shown);
    return InputMethodService::IsInputViewShown(shown);
}

ECode CSoftKeyboard::OnEvaluateInputViewShown(
    /* [out] */ Boolean* shown)
{
    VALIDATE_NOT_NULL(shown);
    return InputMethodService::OnEvaluateInputViewShown(shown);
}

ECode CSoftKeyboard::SetCandidatesViewShown(
    /* [in] */ Boolean shown)
{
    return InputMethodService::SetCandidatesViewShown(shown);
}

ECode CSoftKeyboard::GetCandidatesHiddenVisibility(
    /* [out] */ Int32* visibility)
{
    VALIDATE_NOT_NULL(visibility);
    return InputMethodService::GetCandidatesHiddenVisibility(visibility);
}

ECode CSoftKeyboard::ShowStatusIcon(
    /* [in] */ Int32 iconResId)
{
    return InputMethodService::ShowStatusIcon(iconResId);
}

ECode CSoftKeyboard::HideStatusIcon()
{
    return InputMethodService::HideStatusIcon();
}

ECode CSoftKeyboard::SwitchInputMethod(
    /* [in] */ const String& id)
{
    return InputMethodService::SwitchInputMethod(id);
}

ECode CSoftKeyboard::SetExtractView(
    /* [in] */ IView* view)
{
    return InputMethodService::SetExtractView(view);
}

ECode CSoftKeyboard::SetCandidatesView(
    /* [in] */ IView* view)
{
    return InputMethodService::SetCandidatesView(view);
}

ECode CSoftKeyboard::SetInputView(
    /* [in] */ IView* view)
{
    return InputMethodService::SetInputView(view);
}

ECode CSoftKeyboard::OnCreateExtractTextView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return InputMethodService::OnCreateExtractTextView(view);
}

ECode CSoftKeyboard::OnCreateCandidatesView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    CCandidateView::New((IContext*)this->Probe(EIID_IContext), (ICandidateView**)&mCandidateView);
    mCandidateView->SetService((ISoftKeyboard*)this);
    *view = mCandidateView.Get();
    (*view)->AddRef();
    return NOERROR;
}

ECode CSoftKeyboard::OnCreateInputView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<ILayoutInflater> inflater;
    GetLayoutInflater((ILayoutInflater**)&inflater);
    inflater->Inflate(0x7f030000 /*R.layout.input*/, NULL, (IView**)&mInputView);
    mInputView->SetOnKeyboardActionListener(this);
    mInputView->SetKeyboard(mQwertyKeyboard);
    *view = mInputView.Get();
    (*view)->AddRef();
    return NOERROR;
}

ECode CSoftKeyboard::OnStartInputView(
    /* [in] */ IEditorInfo* info,
    /* [in] */ Boolean restarting)
{
    InputMethodService::OnStartInputView(info, restarting);

    // Apply the selected keyboard to the input view.
    mInputView->SetKeyboard(mCurKeyboard);
    return mInputView->Closing();
}

ECode CSoftKeyboard::OnFinishInputView(
    /* [in] */ Boolean finishingInput)
{
    return InputMethodService::OnFinishInputView(finishingInput);
}

ECode CSoftKeyboard::OnStartCandidatesView(
    /* [in] */ IEditorInfo* info,
    /* [in] */ Boolean restarting)
{
    return InputMethodService::OnStartCandidatesView(info, restarting);
}

ECode CSoftKeyboard::OnFinishCandidatesView(
    /* [in] */ Boolean finishingInput)
{
    return InputMethodService::OnFinishCandidatesView(finishingInput);
}

ECode CSoftKeyboard::OnShowInputRequested(
    /* [in] */ Int32 flags,
    /* [in] */ Boolean configChange,
    /* [out] */ Boolean* requested)
{
    VALIDATE_NOT_NULL(requested);
    return InputMethodService::OnShowInputRequested(flags, configChange, requested);
}

ECode CSoftKeyboard::ShowWindow(
    /* [in] */ Boolean showInput)
{
    return InputMethodService::ShowWindow(showInput);
}

ECode CSoftKeyboard::HideWindow()
{
    return InputMethodService::HideWindow();
}

ECode CSoftKeyboard::OnWindowShown()
{
    return InputMethodService::OnWindowShown();
}

ECode CSoftKeyboard::OnWindowHidden()
{
    return InputMethodService::OnWindowHidden();
}

ECode CSoftKeyboard::OnBindInput()
{
    return InputMethodService::OnBindInput();
}

ECode CSoftKeyboard::OnUnbindInput()
{
    return InputMethodService::OnUnbindInput();
}

ECode CSoftKeyboard::OnStartInput(
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean restarting)
{
    InputMethodService::OnStartInput(attribute, restarting);

    // Reset our state.  We want to do this even if restarting, because
    // the underlying state of the text editor could have changed in any way.
    // mComposing.SetLength(0);
    UpdateCandidates();

    if (!restarting) {
        // Clear shift states.
        mMetaState = 0;
    }

    mPredictionOn = FALSE;
    mCompletionOn = FALSE;
    mCompletions = NULL;

    Int32 inputType = 0, variation = 0;
    attribute->GetInputType(&inputType);

    // We are now going to initialize our state based on the type of
    // text being edited.
    switch (inputType & InputType_TYPE_MASK_CLASS) {
        case InputType_TYPE_CLASS_NUMBER:
        case InputType_TYPE_CLASS_DATETIME:
            // Numbers and dates default to the symbols keyboard, with
            // no extra features.
            mCurKeyboard = mSymbolsKeyboard;
            break;

        case InputType_TYPE_CLASS_PHONE:
            // Phones will also default to the symbols keyboard, though
            // often you will want to have a dedicated phone keyboard.
            mCurKeyboard = mSymbolsKeyboard;
            break;

        case InputType_TYPE_CLASS_TEXT:
            // This is general text editing.  We will default to the
            // normal alphabetic keyboard, and assume that we should
            // be doing predictive text (showing candidates as the
            // user types).
            mCurKeyboard = mQwertyKeyboard;
            //todo:
            // mPredictionOn = TRUE;

            // We now look for a few special variations of text that will
            // modify our behavior.
            variation = inputType & InputType_TYPE_MASK_VARIATION;
            if (variation == InputType_TYPE_TEXT_VARIATION_PASSWORD ||
                    variation == InputType_TYPE_TEXT_VARIATION_VISIBLE_PASSWORD) {
                // Do not display predictions / what the user is typing
                // when they are entering a password.
                mPredictionOn = FALSE;
            }

            if (variation == InputType_TYPE_TEXT_VARIATION_EMAIL_ADDRESS
                    || variation == InputType_TYPE_TEXT_VARIATION_URI
                    || variation == InputType_TYPE_TEXT_VARIATION_FILTER) {
                // Our predictions are not useful for e-mail addresses
                // or URIs.
                mPredictionOn = FALSE;
            }

            if ((inputType & InputType_TYPE_TEXT_FLAG_AUTO_COMPLETE) != 0) {
                // If this is an auto-complete text view, then our predictions
                // will not be shown and instead we will allow the editor
                // to supply their own.  We only show the editor's
                // candidates when in fullscreen mode, otherwise relying
                // own it displaying its own UI.
                mPredictionOn = FALSE;
                IsFullscreenMode(&mCompletionOn);
            }

            // We also want to look at the current state of the editor
            // to decide whether our alphabetic keyboard should start out
            // shifted.
            UpdateShiftKeyState(attribute);
            break;

        default: {
            // For all unknown input types, default to the alphabetic
            // keyboard with no special features.
            mCurKeyboard = mQwertyKeyboard;
            UpdateShiftKeyState(attribute);
        }
    }

    // Update the label on the enter key, depending on what the application
    // says it will do.
    AutoPtr<IResources> res;
    GetResources((IResources**) &res);

    Int32 imeOptions = 0;
    attribute->GetImeOptions(&imeOptions);
    ((LatinKeyboard*)mCurKeyboard.Get())->SetImeOptions(res, imeOptions);

    return NOERROR;
}

ECode CSoftKeyboard::OnFinishInput()
{
    InputMethodService::OnFinishInput();

    // Clear current composing text and candidates.
    // mComposing.SetLength(0);
    UpdateCandidates();

    // We only hide the candidates window when finishing input on
    // a particular editor, to avoid popping the underlying application
    // up and down if the user is entering text into the bottom of
    // its window.
    SetCandidatesViewShown(FALSE);

    mCurKeyboard = mQwertyKeyboard;
    if (mInputView != NULL) {
        mInputView->Closing();
    }

    return NOERROR;
}

ECode CSoftKeyboard::OnDisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    if (mCompletionOn) {
        if (mCompletions != NULL) {
            for (Int32 i = 0; i < mCompletions->GetLength(); ++i) {
                (*mCompletions)[i] = NULL;
            }
            ArrayOf< AutoPtr<ICompletionInfo> >::Free(mCompletions);
        }
        mCompletions = (ArrayOf< AutoPtr<ICompletionInfo> >*)completions;
        if (completions == NULL) {
            SetSuggestions(NULL, FALSE, FALSE);
            return NOERROR;
        }

        ArrayOf<ICharSequence*>* stringList = ArrayOf<ICharSequence*>::Alloc(completions->GetLength());
        for (Int32 i = 0; i < (completions != NULL ? completions->GetLength() : 0); i++) {
            AutoPtr<ICompletionInfo> ci = (*completions)[i];

            if (ci != NULL) {
                AutoPtr<ICharSequence> text;
                ci->GetText((ICharSequence**) &text);

                (*stringList)[i] = text.Get();
            }
            else {
                (*stringList)[i] = NULL;
            }
        }

        SetSuggestions(stringList, TRUE, TRUE);
        ArrayOf<ICharSequence*>::Free(stringList);
    }

    return NOERROR;
}

ECode CSoftKeyboard::OnUpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    return InputMethodService::OnUpdateExtractedText(token, text);
}

ECode CSoftKeyboard::OnUpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    InputMethodService::OnUpdateSelection(oldSelStart, oldSelEnd, newSelStart, newSelEnd,
            candidatesStart, candidatesEnd);

    // If the current selection in the text view changes, we should
    // clear whatever candidate text we have.
    if (mComposing.GetLength() > 0 && (newSelStart != candidatesEnd
            || newSelEnd != candidatesEnd)) {
        // mComposing.SetLength(0);
        UpdateCandidates();

        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean temp = FALSE;
            return ic->FinishComposingText(&temp);
        }
    }

    return NOERROR;
}

ECode CSoftKeyboard::OnUpdateCursor(
    /* [in] */ IRect* newCursor)
{
    return InputMethodService::OnUpdateCursor(newCursor);
}

ECode CSoftKeyboard::RequestHideSelf(
    /* [in] */ Int32 flags)
{
    return InputMethodService::RequestHideSelf(flags);
}

ECode CSoftKeyboard::OnAppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return InputMethodService::OnAppPrivateCommand(action, data);
}

ECode CSoftKeyboard::SendDownUpKeyEvents(
    /* [in] */ Int32 keyEventCode)
{
    return InputMethodService::SendDownUpKeyEvents(keyEventCode);
}

ECode CSoftKeyboard::SendDefaultEditorAction(
    /* [in] */ Boolean fromEnterKey,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);
    return InputMethodService::SendDefaultEditorAction(fromEnterKey, state);
}

ECode CSoftKeyboard::SendKeyChar(
    /* [in] */ Char32 charCode)
{
    return InputMethodService::SendKeyChar(charCode);
}

ECode CSoftKeyboard::OnExtractedSelectionChanged(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return InputMethodService::OnExtractedSelectionChanged(start, end);
}

ECode CSoftKeyboard::OnExtractedTextClicked()
{
    return InputMethodService::OnExtractedTextClicked();
}

ECode CSoftKeyboard::OnExtractedCursorMovement(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    return InputMethodService::OnExtractedCursorMovement(dx, dy);
}

ECode CSoftKeyboard::OnExtractTextContextMenuItem(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);
    return InputMethodService::OnExtractTextContextMenuItem(id, state);
}

ECode CSoftKeyboard::GetTextForImeAction(
    /* [in] */ Int32 imeOptions,
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    return InputMethodService::GetTextForImeAction(imeOptions, text);
}

ECode CSoftKeyboard::OnUpdateExtractingVisibility(
    /* [in] */ IEditorInfo* ei)
{
    return InputMethodService::OnUpdateExtractingVisibility(ei);
}

ECode CSoftKeyboard::OnUpdateExtractingViews(
    /* [in] */ IEditorInfo* ei)
{
    return InputMethodService::OnUpdateExtractingViews(ei);
}

ECode CSoftKeyboard::OnExtractingInputChanged(
    /* [in] */ IEditorInfo* ei)
{
    return InputMethodService::OnExtractingInputChanged(ei);
}

ECode CSoftKeyboard::OnPress(
    /* [in] */ Int32 primaryCode)
{
    return NOERROR;
}

ECode CSoftKeyboard::OnRelease(
    /* [in] */ Int32 primaryCode)
{
    return NOERROR;
}

ECode CSoftKeyboard::OnKey(
    /* [in] */ Int32 primaryCode,
    /* [in] */ ArrayOf<Int32>* keyCodes)
{
    if (IsWordSeparator(primaryCode)) {
        // Handle separator
        if (mComposing.GetLength() > 0) {
            AutoPtr<IInputConnection> ic;
            GetCurrentInputConnection((IInputConnection**)&ic);
            CommitTyped(ic);
        }
        SendKey(primaryCode);
        AutoPtr<IEditorInfo> editerinfo;
        GetCurrentInputEditorInfo((IEditorInfo**)&editerinfo);
        UpdateShiftKeyState(editerinfo);
    }
    else if (primaryCode == Keyboard_KEYCODE_DELETE) {
        HandleBackspace();
    }
    else if (primaryCode == Keyboard_KEYCODE_SHIFT) {
        HandleShift();
    }
    else if (primaryCode == Keyboard_KEYCODE_CANCEL) {
        HandleClose();
        return NOERROR;
    }
    else if (primaryCode == KEYCODE_OPTIONS) {
        // Show a menu or somethin'
    }
    else if (primaryCode == Keyboard_KEYCODE_MODE_CHANGE
            && mInputView != NULL) {
        AutoPtr<IKeyboard> current;
        mInputView->GetKeyboard((IKeyboard**)&current);
        if (current == mSymbolsKeyboard || current == mSymbolsShiftedKeyboard) {
            current = mQwertyKeyboard;
        }
        else {
            current = mSymbolsKeyboard;
        }
        mInputView->SetKeyboard(current);
        if (current == mSymbolsKeyboard) {
            Boolean temp = FALSE;
            current->SetShifted(FALSE, &temp);
        }
    }
    else {
        HandleCharacter(primaryCode, keyCodes);
    }
    return NOERROR;
}

ECode CSoftKeyboard::OnText(
    /* [in] */ ICharSequence* text)
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);

    if (ic == NULL) return NOERROR;

    Boolean temp = FALSE;
    ic->BeginBatchEdit(&temp);
    if (mComposing.GetLength() > 0) {
        CommitTyped(ic);
    }

    ic->CommitText(text, 0, &temp);
    ic->EndBatchEdit(&temp);

    AutoPtr<IEditorInfo> editerinfo;
    GetCurrentInputEditorInfo((IEditorInfo**)&editerinfo);
    UpdateShiftKeyState(editerinfo);

    return NOERROR;
}

ECode CSoftKeyboard::SwipeLeft()
{
    HandleBackspace();
    return NOERROR;
}

ECode CSoftKeyboard::SwipeRight()
{
    if (mCompletionOn) {
        PickDefaultCandidate();
    }

    return NOERROR;
}

ECode CSoftKeyboard::SwipeDown()
{
    HandleClose();
    return NOERROR;
}

ECode CSoftKeyboard::SwipeUp()
{
    return NOERROR;
}

ECode CSoftKeyboard::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return InputMethodService::OnConfigurationChanged(newConfig);
}

ECode CSoftKeyboard::OnLowMemory()
{
    return InputMethodService::OnLowMemory();
}

ECode CSoftKeyboard::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 repeat = 0;

    switch (keyCode) {
        case KeyEvent_KEYCODE_BACK:
            // The InputMethodService already takes care of the back
            // key for us, to dismiss the input method if it is shown.
            // However, our keyboard could be showing a pop-up window
            // that back should dismiss, so we first allow it to do that.
            event->GetRepeatCount(&repeat);
            if (repeat == 0 && mInputView != NULL) {
                Boolean res = FALSE;
                mInputView->HandleBack(&res);
                if (res) {
                    *result = TRUE;
                    return NOERROR;
                }
            }
            break;

        case KeyEvent_KEYCODE_DEL:
            // Special handling of the delete key: if we currently are
            // composing text for the user, we want to modify that instead
            // of let the application to the delete itself.
            if (mComposing.GetLength() > 0) {
                OnKey(Keyboard_KEYCODE_DELETE, NULL);
                *result = TRUE;
                return NOERROR;
            }
            break;

        case KeyEvent_KEYCODE_ENTER:
            // Let the underlying text editor always handle these.
            *result = FALSE;
            return NOERROR;

        default: {
            Int32 metaState = 0;
            event->GetMetaState(&metaState);
            // For all other keys, if we want to do transformations on
            // text being entered with a hard keyboard, we need to process
            // it and do the appropriate action.
            if (PROCESS_HARD_KEYS) {
                if (keyCode == KeyEvent_KEYCODE_SPACE
                        && (metaState & KeyEvent_META_ALT_ON) != 0) {
                    // A silly example: in our input method, Alt+Space
                    // is a shortcut for 'android' in lower case.
                    AutoPtr<IInputConnection> ic;
                    GetCurrentInputConnection((IInputConnection**) &ic);

                    if (ic != NULL) {
                        // First, tell the editor that it is no longer in the
                        // shift state, since we are consuming this.
                        Boolean temp = FALSE;
                        ic->ClearMetaKeyStates(KeyEvent_META_ALT_ON, &temp);
                        KeyDownUp(KeyEvent_KEYCODE_A);
                        KeyDownUp(KeyEvent_KEYCODE_N);
                        KeyDownUp(KeyEvent_KEYCODE_D);
                        KeyDownUp(KeyEvent_KEYCODE_R);
                        KeyDownUp(KeyEvent_KEYCODE_O);
                        KeyDownUp(KeyEvent_KEYCODE_I);
                        KeyDownUp(KeyEvent_KEYCODE_D);
                        // And we consume this event.
                        *result = TRUE;
                        return NOERROR;
                    }
                }
                if (mPredictionOn && TranslateKeyDown(keyCode, event)) {
                    *result = TRUE;
                    return NOERROR;
                }
            }
        }
    }

    return InputMethodService::OnKeyDown(keyCode, event, result);
}

ECode CSoftKeyboard::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return InputMethodService::OnKeyLongPress(keyCode, event, result);
}

ECode CSoftKeyboard::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // If we want to do transformations on text being entered with a hard
    // keyboard, we need to process the up events to update the meta key
    // state we are tracking.
    if (PROCESS_HARD_KEYS) {
        if (mPredictionOn) {
            //TODO
            // mMetaState = MetaKeyKeyListener::HandleKeyUp(mMetaState,
            //         keyCode, pEvent);
        }
    }

    return InputMethodService::OnKeyUp(keyCode, event, result);
}

ECode CSoftKeyboard::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return InputMethodService::OnKeyMultiple(keyCode, count, event, result);
}

ECode CSoftKeyboard::StopSelf()
{
    return InputMethodService::StopSelf();
}

ECode CSoftKeyboard::StopSelfEx(
    /* [in] */ Int32 startId)
{
    return InputMethodService::StopSelfEx(startId);
}

ECode CSoftKeyboard::StopSelfResult(
    /* [in] */ Int32 startId,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return InputMethodService::StopSelfResult(startId, res);
}

ECode CSoftKeyboard::SetForeground(
    /* [in] */ Boolean isForeground)
{
    return InputMethodService::SetForeground(isForeground);
}

ECode CSoftKeyboard::StartForeground(
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    return InputMethodService::StartForeground(id, notification);
}

ECode CSoftKeyboard::StopForeground(
    /* [in] */ Boolean removeNotification)
{
    return InputMethodService::StopForeground(removeNotification);
}

ECode CSoftKeyboard::constructor()
{
    return NOERROR;
}

void CSoftKeyboard::SetSuggestions(
    /* [in] */ ArrayOf<ICharSequence*>* suggestions,
    /* [in] */ Boolean completions,
    /* [in] */ Boolean typedWordValid)
{
    Boolean shown = FALSE;
    IsExtractViewShown(&shown);

    if (suggestions != NULL && suggestions->GetLength() > 0) {
        SetCandidatesViewShown(TRUE);
    }
    else if (shown) {
        SetCandidatesViewShown(TRUE);
    }

    if (mCandidateView != NULL) {
        mCandidateView->SetSuggestions(suggestions, completions, typedWordValid);
    }
}

Boolean CSoftKeyboard::IsWordSeparator(
    /* [in] */ Int32 code)
{
    ArrayOf_<Char8, 5> chs;
    Int32 num = 0;
    Character::ToChars(code, chs, 0, &num);
    chs[num] = 0;
    return mWordSeparators.Contains(chs.GetPayload());
}

void CSoftKeyboard::PickDefaultCandidate()
{
    PickSuggestionManually(0);
}

ECode CSoftKeyboard::PickSuggestionManually(
    /* [in] */ Int32 index)
{
    if (mCompletionOn && mCompletions != NULL && index >= 0
            && index < mCompletions->GetLength()) {
        AutoPtr<ICompletionInfo> ci = (*mCompletions)[index];

        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);

        Boolean temp = FALSE;
        ic->CommitCompletion(ci, &temp);
        if (mCandidateView != NULL) {
            mCandidateView->Clear();
        }

        AutoPtr<IEditorInfo> ei;
        GetCurrentInputEditorInfo((IEditorInfo**)&ei);
        UpdateShiftKeyState(ei);
    }
    else if (mComposing.GetLength() > 0) {
        // If we were generating candidate suggestions for the current
        // text, we would commit one of them here.  But for this sample,
        // we will just commit the current text.
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        CommitTyped(ic);
    }

    return NOERROR;
}

Boolean CSoftKeyboard::TranslateKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    //TODO
    // mMetaState = MetaKeyKeyListener::HandleKeyDown(mMetaState,
    //         keyCode, event);
    Int32 c = 0;

    //TODO
    // event->GetUnicodeCharEx(MetaKeyKeyListener::GetMetaState(mMetaState), &c);
    // mMetaState = MetaKeyKeyListener::AdjustMetaAfterKeypress(mMetaState);

    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    if (c == 0 || ic == NULL) {
        return FALSE;
    }

    Boolean dead = FALSE;
    if ((c & 0x80000000 /*KeyCharacterMap.COMBINING_ACCENT*/) != 0) {
        dead = TRUE;
        c = c & 0x7FFFFFFF /*KeyCharacterMap.COMBINING_ACCENT_MASK*/;
    }

    if (mComposing.GetLength() > 0) {
        Char32 accent = 0;
        Character::GetCharAt((const char*)mComposing,
                mComposing.GetLength() - 1, &accent);

        assert(0);
        // Int32 composed = CKeyEvent::GetDeadChar(accent, c);

        // if (composed != 0) {
        //     c = composed;
        //     mComposing.SetLength(mComposing.GetLength() - 1);
        // }
    }

    OnKey(c, NULL);
    return TRUE;
}

void CSoftKeyboard::CommitTyped(
    /* [in] */ IInputConnection* inputConnection)
{
    if (mComposing.GetLength() > 0) {
        AutoPtr<ICharSequence> text;
        CParcelableStringWrapper::New(String((const char*)mComposing), (ICharSequence**)&text);
        Boolean temp = FALSE;
        inputConnection->CommitText(text, mComposing.GetLength(), &temp);
        // mComposing.SetLength(0);
        UpdateCandidates();
    }
}

void CSoftKeyboard::UpdateShiftKeyState(
    /* [in] */ IEditorInfo* attr)
{
    if (attr != NULL && mInputView != NULL) {
        AutoPtr<IKeyboard> keyboard;
        mInputView->GetKeyboard((IKeyboard**)&keyboard);
        if (mQwertyKeyboard == keyboard) {
            Int32 caps = 0;
            AutoPtr<IEditorInfo> ei;
            GetCurrentInputEditorInfo((IEditorInfo**)&ei);
            Int32 inputType = 0;
            if (ei != NULL && (ei->GetInputType(&inputType), inputType != InputType_TYPE_NULL)) {
                AutoPtr<IInputConnection> ic;
                GetCurrentInputConnection((IInputConnection**) &ic);
                ic->GetCursorCapsMode(inputType, &caps);
            }
            Boolean temp = FALSE;
            mInputView->SetShifted(mCapsLock || caps != 0, &temp);
        }
    }
}

Boolean CSoftKeyboard::IsAlphabet(
    /* [in] */ Int32 code)
{
    if (Character::IsLetter(code)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void CSoftKeyboard::KeyDownUp(
    /* [in] */ Int32 keyEventCode)
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    Boolean tmpState = FALSE;
    AutoPtr<IKeyEvent> event;
    CKeyEvent::New(KeyEvent_ACTION_DOWN, keyEventCode, (IKeyEvent**)&event);
    ic->SendKeyEvent(event, &tmpState);
    CKeyEvent::New(KeyEvent_ACTION_UP, keyEventCode, (IKeyEvent**)&event);
    ic->SendKeyEvent(event, &tmpState);
}

void CSoftKeyboard::SendKey(
    /* [in] */ Int32 keyCode)
{
    switch (keyCode) {
        case '\n':
            KeyDownUp(KeyEvent_KEYCODE_ENTER);
            break;
        default:
            if (keyCode >= '0' && keyCode <= '9') {
                KeyDownUp(keyCode - '0' + KeyEvent_KEYCODE_0);
            }
            else {
                AutoPtr<IInputConnection> ic;
                GetCurrentInputConnection((IInputConnection**) &ic);
                assert(ic != NULL);

                ArrayOf_<Char8, 2> chs;
                Int32 num = 0;
                Elastos::Core::Character::ToChars(keyCode, chs, 0, &num);
                chs[num] = 0;
                AutoPtr<ICharSequence> cs;
                CParcelableStringWrapper::New(String(chs.GetPayload()), (ICharSequence**)&cs);

                Boolean tmpState = FALSE;
                ic->CommitText(cs, 1, &tmpState);
            }
            break;
    }
}

void CSoftKeyboard::UpdateCandidates()
{
    if (!mCompletionOn) {
        if (mComposing.GetLength() > 0) {
            ArrayOf<ICharSequence*>* list = ArrayOf<ICharSequence*>::Alloc(1);
            AutoPtr<ICharSequence> text;
            CParcelableStringWrapper::New(String((const char*)mComposing), (ICharSequence**)&text);
            (*list)[0] = text.Get();
            SetSuggestions(list, TRUE, TRUE);
            ArrayOf<ICharSequence*>::Free(list);
        }
        else {
            SetSuggestions(NULL, FALSE, FALSE);
        }
    }
}

void CSoftKeyboard::HandleBackspace()
{
    Int32 length = mComposing.GetLength();
    if (length > 1) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**) &ic);
        //mComposing.Replace(length - 1, 1, (const char*)mComposing);
        mComposing.DeleteChar(length - 1);
        AutoPtr<ICharSequence> text;
        CParcelableStringWrapper::New(String((const char*)mComposing), (ICharSequence**)&text);
        Boolean temp = FALSE;
        ic->SetComposingText(text, 1, &temp);
        UpdateCandidates();
    }
    else if (length > 0) {
        // mComposing.SetLength(0);
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        AutoPtr<ICharSequence> text;
        CParcelableStringWrapper::New(String(""), (ICharSequence**)&text);
        Boolean temp = FALSE;
        ic->CommitText(text, 0, &temp);
        UpdateCandidates();
    }
    else {
        KeyDownUp(KeyEvent_KEYCODE_DEL);
    }

    AutoPtr<IEditorInfo> ei;
    GetCurrentInputEditorInfo((IEditorInfo**) &ei);
    UpdateShiftKeyState(ei);
}

void CSoftKeyboard::HandleShift()
{
    if (mInputView == NULL) {
        return;
    }

    Boolean temp = FALSE;
    AutoPtr<IKeyboard> currentKeyboard;
    mInputView->GetKeyboard((IKeyboard**)&currentKeyboard);
    if (mQwertyKeyboard == currentKeyboard) {
        // Alphabet keyboard
        CheckToggleCapsLock();

        Boolean shifted = FALSE;
        mInputView->IsShifted(&shifted);
        mInputView->SetShifted(mCapsLock || !shifted, &temp);
    }
    else if (currentKeyboard == mSymbolsKeyboard) {
        mSymbolsKeyboard->SetShifted(TRUE, &temp);
        mInputView->SetKeyboard(mSymbolsShiftedKeyboard);
        mSymbolsShiftedKeyboard->SetShifted(TRUE, &temp);
    }
    else if (currentKeyboard == mSymbolsShiftedKeyboard) {
        mSymbolsShiftedKeyboard->SetShifted(FALSE, &temp);
        mInputView->SetKeyboard(mSymbolsKeyboard);
        mSymbolsKeyboard->SetShifted(FALSE, &temp);
    }
}

void CSoftKeyboard::HandleCharacter(
    /* [in] */ Int32 primaryCode,
    /* [in] */ ArrayOf<Int32>* keyCodes)
{
    Boolean shown = FALSE;
    if (IsInputViewShown(&shown), shown) {
        Boolean shifted = FALSE;
        if (mInputView->IsShifted(&shifted), shifted) {
            primaryCode = Character::ToUpperCase(primaryCode);
        }
    }
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    if (IsAlphabet(primaryCode) && mPredictionOn) {
        mComposing += primaryCode;
        AutoPtr<ICharSequence> text;
        CParcelableStringWrapper::New(String((const char*)mComposing), (ICharSequence**)&text);
        Boolean temp = FALSE;
        ic->SetComposingText(text, 1, &temp);
        AutoPtr<IEditorInfo> ei;
        GetCurrentInputEditorInfo((IEditorInfo**)&ei);
        UpdateShiftKeyState(ei);
        UpdateCandidates();
    }
    else {
        ArrayOf_<Char8, 5> chs;
        Int32 num = 0;
        Character::ToChars(primaryCode, chs, 0, &num);
        chs[num] = 0;
        AutoPtr<ICharSequence> cs;
        CParcelableStringWrapper::New(String(chs.GetPayload()), (ICharSequence**)&cs);
        Boolean tmpState = FALSE;
        ic->CommitText(cs, 1, &tmpState);
    }
}

void CSoftKeyboard::HandleClose()
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**) &ic);
    CommitTyped(ic);
    RequestHideSelf(0);
    mInputView->Closing();
}

void CSoftKeyboard::CheckToggleCapsLock()
{
    Int64 now = System::GetCurrentTimeMillis();
    if (mLastShiftTime + 800 > now) {
        mCapsLock = !mCapsLock;
        mLastShiftTime = 0;
    }
    else {
        mLastShiftTime = now;
    }
}
