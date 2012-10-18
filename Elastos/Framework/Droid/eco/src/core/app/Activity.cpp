
#ifdef _FRAMEWORK_CORE
#include "app/Activity.h"
#include "impl/CPolicyManager.h"
#include "os/CServiceManager.h"
//TODO
//#include "Build.h"  //using the integer.
#else
#include "Activity.h"
#endif

#include <stdio.h>

Int32 Activity::DEFAULT_KEYS_DISABLE = 0;
Int32 Activity::DEFAULT_KEYS_DIALER = 1;
Int32 Activity::DEFAULT_KEYS_SHORTCUT = 2;
Int32 Activity::DEFAULT_KEYS_SEARCH_LOCAL = 3;
Int32 Activity::DEFAULT_KEYS_SEARCH_GLOBAL = 4;

Activity::Activity()
    : mManagedDialogs(NULL)
    , mWindowAdded(FALSE)
    , mVisibleFromClient(TRUE)
    , mTitleColor(0)
    , mTitleReady(FALSE)
    , mDefaultKeyMode(DEFAULT_KEYS_DISABLE)
{
}

Activity::~Activity()
{}

ECode Activity::Initialize()
{
    return NOERROR;
}

PInterface Activity::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IActivity*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IActivity) {
        return (IActivity*)this;
    }
    else if (riid == EIID_IContextThemeWrapper) {
        return (IContextThemeWrapper*)this;
    }
    else if (riid == EIID_IContextWrapper) {
        return (IContextWrapper*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }
    else if (riid == EIID_IWindowCallback) {
        return (IWindowCallback*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    }

    return NULL;
}

UInt32 Activity::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Activity::Release()
{
    return ElRefBase::Release();
}

ECode Activity::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IActivity *)this) {
        *pIID = EIID_IActivity;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IWindowCallback *)this) {
        *pIID = EIID_IWindowCallback;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IKeyEventCallback *)this) {
        *pIID = EIID_IKeyEventCallback;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Activity::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::IsChild(
    /* [out] */ Boolean* isChild)
{
    if (isChild == NULL) return E_INVALID_ARGUMENT;
    *isChild = mParent != NULL;
    return NOERROR;
}

ECode Activity::GetID(
    /* [out] */ String* id)
{
    if (id == NULL) return E_INVALID_ARGUMENT;
    *id = mEmbeddedID;
    return NOERROR;
}

ECode Activity::CreateCapsuleContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateCapsuleContext(capsuleName, flags, ctx);
}

ECode Activity::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::EnforceCallingOrSelfPermission(
    /* [in] */ CString permission,
    /* [in] */ CString message)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return mBase->GetAssets(assetManager);
}

ECode Activity::GetResources(
    /* [out] */ IResources** resources)
{
    return mBase->GetResources(resources);
}

ECode Activity::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return mBase->GetContentResolver(resolver);
}

ECode Activity::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    return mBase->GetText(resId, text);
}

ECode Activity::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return InitializeTheme();
}

ECode Activity::GetTheme(
    /* [out] */ ITheme** theme)
{
    if (mTheme == NULL) {
        if (mThemeResource == 0) {
            mThemeResource = 0x01030005/*com.android.internal.R.style.Theme*/;
        }
        InitializeTheme();
    }

    *theme = mTheme;
    mTheme->AddRef();

    return NOERROR;
}

ECode Activity::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributes(attrs, styles);
}

ECode Activity::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode Activity::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode Activity::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx3(set, attrs,
            defStyleAttr, defStyleRes, styles);
}

ECode Activity::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return mBase->GetClassLoader(loader);
}

ECode Activity::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    return mBase->GetCapsuleName(capsuleName);
}

ECode Activity::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    return mBase->GetApplicationInfo(info);
}

ECode Activity::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    if (ctx == NULL) return E_INVALID_ARGUMENT;

    *ctx = mBase;
    if (*ctx) (*ctx)->AddRef();
    return NOERROR;
}

// internal use.
ECode Activity::IsStartedActivity(
    /* [out] */ Boolean* isStartedActivity)
{
    if (!isStartedActivity) {
        return E_INVALID_ARGUMENT;
    }
    *isStartedActivity = mStartedActivity;
    return NOERROR;
}

ECode Activity::GetWindowEx(
    /* [out] */ IWindow** window)
{
    if (!window) {
        return E_INVALID_ARGUMENT;
    }
    *window = mWindow.Get();
    if (*window) {
        (*window)->AddRef();
    }
    return NOERROR;
}

ECode Activity::SetDecorView(
    /* [in] */ IView* decor)
{
    mDecor = decor;
    return NOERROR;
}

ECode Activity::GetDecorView(
    /* [out] */ IView** decor)
{
    if (!decor) {
        return E_INVALID_ARGUMENT;
    }
    *decor = mDecor.Get();
    if (*decor) {
        (*decor)->AddRef();
    }
    return NOERROR;
}

ECode Activity::IsVisibleFromClient(
    /* [out] */ Boolean* visible)
{
    if (!visible) {
        return E_INVALID_ARGUMENT;
    }
    *visible = mVisibleFromClient;
    return NOERROR;
}

ECode Activity::SetWindowAdded(
    /* [in] */ Boolean added)
{
    mWindowAdded = added;
    return NOERROR;
}

ECode Activity::GetWindowManagerEx(
    /* [out] */ IWindowManager** mgr)
{
    if (!mgr) {
        return E_INVALID_ARGUMENT;
    }
    *mgr = mWindowManager.Get();
    if (*mgr) {
        (*mgr)->AddRef();
    }
    return NOERROR;
}

EXTERN_C ECode GrafixInit();

ECode Activity::Create(
    /* [in] */ IBundle* savedInstanceState)
{
    return OnCreate(savedInstanceState);
}

ECode Activity::PostCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    return OnPostCreate(savedInstanceState);
}

ECode Activity::Start()
{
    ECode ec = OnStart();

    return ec;
}

ECode Activity::Restart()
{
    OnRestart();

    return Start();
}

ECode Activity::Resume()
{
    return OnResume();
}

ECode Activity::Pause()
{
    return OnPause();
}

ECode Activity::Stop()
{
    return OnStop();
}

ECode Activity::Destroy()
{
    return OnDestroy();
}

ECode Activity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
//    mVisibleFromClient = mWindow.getWindowStyle().getBoolean(
//            com.android.internal.R.styleable.Window_windowNoDisplay, TRUE);
    mCalled = TRUE;
    return NOERROR;
}

AutoPtr<IDialog> Activity::CreateDialog(
    /* [in] */ Int32 dialogId,
    /* [in] */ IBundle* state,
    /* [in] */ IBundle* args)
{
    AutoPtr<IDialog> dialog = OnCreateDialog(dialogId, args);
    if (dialog == NULL) {
        return NULL;
    }
    dialog->DispatchOnCreate(state);

    return dialog;
}

//private static String Activity::savedDialogKeyFor(Int32 key) {
//    return SAVED_DIALOG_KEY_PREFIX + key;
//}
//
//private static String Activity::savedDialogArgsKeyFor(Int32 key) {
//    return SAVED_DIALOG_ARGS_KEY_PREFIX + key;
//}

ECode Activity::OnPostCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Boolean isChild;
    IsChild(&isChild);
    if (!isChild) {
        mTitleReady = TRUE;
        AutoPtr<ICharSequence> title;
        Int32 titleColor;
        GetTitle((ICharSequence**)&title);
        GetTitleColor(&titleColor);
        OnTitleChanged(title, titleColor);
    }
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnStart()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnRestart()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnResume()
{
    return NOERROR;
}

ECode Activity::OnPause()
{
    return NOERROR;
}

ECode Activity::OnPostResume()
{
//    final Window win = getWindow();
//    if (win != NULL) win.makeActive();
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnStop()
{
    return NOERROR;
}

ECode Activity::OnDestroy()
{
    mCalled = TRUE;

    // dismiss any dialogs we are managing.
    if (mManagedDialogs != NULL) {
        HashMap<Int32, ManagedDialog*>::Iterator iter = mManagedDialogs->Begin();
        for (; iter != mManagedDialogs->End(); ++iter) {
            Boolean isShowing;
            iter->mSecond->mDialog->IsShowing(&isShowing);
            if (isShowing) {
                iter->mSecond->mDialog->Dismiss();
            }
            delete iter->mSecond;
        }
        delete mManagedDialogs;
        mManagedDialogs = NULL;
    }
    return NOERROR;
}

AutoPtr<IView> Activity::FindViewById(
    /* [in] */ Int32 id)
{
    AutoPtr<IView> view;
    GetWindow()->FindViewById(id, (IView**)&view);
    return view;
}

ECode Activity::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    return GetWindow()->SetContentView(layoutResID);
}

ECode Activity::SetContentView(
    /* [in] */ IView* view)
{
    return GetWindow()->SetContentViewEx(view);
}

ECode Activity::SetContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return GetWindow()->SetContentViewEx2(view, params);
}

AutoPtr<IWindowManager> Activity::GetWindowManager()
{
    return mWindowManager;
}

AutoPtr<IWindow> Activity::GetWindow()
{
    return mWindow;
}

ECode Activity::Attach(
    /* [in] */ IContext* context,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ IInstrumentation* instr,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICharSequence* title,
    /* [in] */ IActivity* parent,
    /* [in] */ const String& id,
    /* [in] */ IInterface* lastNonConfigurationInstance,
    /* [in] */ IConfiguration* config)
{
    return AttachEx(context, apartment, instr, token, 0, application,
            intent, info, title, parent, id, lastNonConfigurationInstance, NULL, config);
}

ECode Activity::AttachEx(
    /* [in] */ IContext* context,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ IInstrumentation* instr,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 ident,
    /* [in] */ IApplication* application,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICharSequence* title,
    /* [in] */ IActivity* parent,
    /* [in] */ const String& id,
    /* [in] */ IInterface* lastNonConfigurationInstance,
    /* [in] */ IObjectStringMap* lastNonConfigurationChildInstances,
    /* [in] */ IConfiguration* config)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    mBase = context;
    mApartment = apartment;
    mInstrumentation = instr;
    mToken = token;
    mIntent = intent;
    ECode ec = NOERROR;
    intent->GetComponent((IComponentName**)&mComponent);

    AutoPtr<IPolicyManager> pm;
    CPolicyManager::AcquireSingleton((IPolicyManager**)&pm);
    ec = pm->MakeNewWindow(this, (IWindow**)&mWindow);
    if (FAILED(ec)) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        return ec;
    }

    //mWindow->SetCallback((IWindowCallback*)(this->Probe(EIID_IWindowCallback)));
    printf("==== File: %s, Line: %d ====, FUNC : %s, mWindow = [%0X].\n", __FILE__, __LINE__, __FUNCTION__, mWindow.Get());
    mWindow->SetCallback((IWindowCallback*)(this->Probe(EIID_IWindowCallback)));
    //mWindow->SetCallback(NULL);

//    if (info.softInputMode != WindowManager.LayoutParams.SOFT_INPUT_STATE_UNSPECIFIED) {
//        mWindow.setSoftInputMode(info.softInputMode);
//    }

    mTitle = title;

    String str;
    mComponent->FlattenToString(&str);
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
printf("==== File: %s, Line: %d ====, FUNC : %s, mWindow = [%0X].\n", __FILE__, __LINE__, __FUNCTION__, mWindow.Get());
    ec = mWindow->SetWindowManager(NULL, mToken, str);

printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
//mWindow->SetCallback((IWindowCallback*)(this->Probe(EIID_IWindowCallback)));
printf("==== File: %s, Line: %d ====, FUNC : %s, mWindow = [%0X].\n", __FILE__, __LINE__, __FUNCTION__, mWindow.Get());

    if (mParent != NULL) {
        AutoPtr<IWindow> pWindow;
        mParent->GetWindowEx((IWindow**) &pWindow);
        mWindow->SetContainer(pWindow);
    }
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    mWindow->GetWindowManager((IWindowManager**)&mWindowManager);
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    return NOERROR;
}

ECode Activity::DispatchNewIntent(
    /* [in] */ IIntent *intent)
{
    return OnNewIntent(intent);
}

ECode Activity::SetCalled(
    /* [in] */ Boolean called)
{
    mCalled = called;
    return NOERROR;
}

ECode Activity::IsCalled(
    /* [out] */ Boolean* called)
{
    if (called == NULL) return E_INVALID_ARGUMENT;
    *called = mCalled;
    return NOERROR;
}

ECode Activity::SetFinished(
    /* [in] */ Boolean finished)
{
    mFinished = finished;
    return NOERROR;
}

ECode Activity::IsFinished(
    /* [out] */ Boolean* finished)
{
    if (finished == NULL) return E_INVALID_ARGUMENT;
    *finished = mFinished;
    return NOERROR;
}

ECode Activity::StartActivity(
    /* [in] */ IIntent *intent)
{
    return StartActivityForResult(intent, -1);
}

/**
 * Launch an activity for which you would like a result when it finished.
 * When this activity exits, your
 * onActivityResult() method will be called with the given requestCode.
 * Using a negative requestCode is the same as calling
 * {@link #startActivity} (the activity is not launched as a sub-activity).
 *
 * <p>Note that this method should only be used with Intent protocols
 * that are defined to return a result.  In other protocols (such as
 * {@link Intent#ACTION_MAIN} or {@link Intent#ACTION_VIEW}), you may
 * not get the result when you expect.  For example, if the activity you
 * are launching uses the singleTask launch mode, it will not run in your
 * task and thus you will immediately receive a cancel result.
 *
 * <p>As a special case, if you call startActivityForResult() with a requestCode
 * >= 0 during the initial onCreate(Bundle savedInstanceState)/onResume() of your
 * activity, then your window will not be displayed until a result is
 * returned back from the started activity.  This is to avoid visible
 * flickering when redirecting to another activity.
 *
 * <p>This method throws {@link android.content.ActivityNotFoundException}
 * if there was no Activity found to run the given Intent.
 *
 * @param intent The intent to start.
 * @param requestCode If >= 0, this code will be returned in
 *                    onActivityResult() when the activity exits.
 *
 * @throws android.content.ActivityNotFoundException
 *
 * @see #startActivity
 */
ECode Activity::StartActivityForResult(
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 requestCode)
{
    if (mParent == NULL) {
        AutoPtr<IActivityResult> result;
        mInstrumentation->ExecStartActivity(
                mBase, mApartment, mToken, this,
                intent, requestCode, (IActivityResult**)&result);
        if (result != NULL) {
            Int32 resultCode;
            AutoPtr<IIntent> resultData;
            result->GetResultCode(&resultCode);
            result->GetResultData((IIntent**)&resultData);
            mApartment->SendActivityResult(
                mToken, mEmbeddedID, requestCode, resultCode, resultData);
        }
        if (requestCode >= 0) {
            // If this start is requesting a result, we can avoid making
            // the activity visible until the result is received.  Setting
            // this code during onCreate(Bundle savedInstanceState) or onResume() will keep the
            // activity hidden during this time, to avoid flickering.
            // This can only be done when a result is requested because
            // that guarantees we will get information back when the
            // activity is finished, no matter what happens to it.
            mStartedActivity = TRUE;
        }
    } else {
        mParent->StartActivityFromChild(this, intent, requestCode);
    }
    return NOERROR;
}

ECode Activity::StartActivityFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode)
{
    AutoPtr<IActivityResult> result;
    mInstrumentation->ExecStartActivity(
            mBase, mApartment, mToken, child,
            intent, requestCode, (IActivityResult**)&result);
    if (result != NULL) {
        String id;
        Int32 resultCode;
        AutoPtr<IIntent> resultData;
        child->GetID(&id);
        result->GetResultCode(&resultCode);
        result->GetResultData((IIntent**)&resultData);
        mApartment->SendActivityResult(
            mToken, id, requestCode, resultCode, resultData);
    }
    return NOERROR;
}

ECode Activity::Finish()
{
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<IActivityManager> activityManager;

    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
    serviceManager->GetService(String("ActivityManagerService"), (IInterface**)&activityManager);
    assert(activityManager != NULL);
    Boolean finished;
    return activityManager->FinishActivity(mToken, mResultCode, mResultData, &finished);
}

ECode Activity::SendBroadcast(
    /* [in] */ IIntent *intent)
{
    return mBase->SendBroadcast(intent);
}

ECode Activity::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return mBase->StartService(service, name);
}

ECode Activity::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StopService(service, succeeded);
}

ECode Activity::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, succeeded);
}

ECode Activity::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return mBase->UnbindService(conn);
}

ECode Activity::GetSystemService(
    /* [in] */ CString name,
    /* [out] */ IInterface** object)
{
    if (object == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IContext> ctx;
    GetBaseContext((IContext**)&ctx);
    if (ctx == NULL) {
//        throw new IllegalStateException(
//                "System services not available to Activities before onCreate()");
        return E_NOT_IMPLEMENTED;
    }

    if (!CString(Context_WINDOW_SERVICE).Compare(name)) {
        *object = (IInterface*)mWindowManager.Get();
        if (*object != NULL) (*object)->AddRef();
        return NOERROR;
    }
    else if (!CString(Context_SEARCH_SERVICE).Compare(name)) {
//        ensureSearchManager();
//        return mSearchManager;
    }
    else if (!CString(Context_LAYOUT_INFLATER_SERVICE).Compare(name)) {
        if (mInflater == NULL) {
            AutoPtr<ILayoutInflater> inflater;
            mBase->GetSystemService(
                Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

            if (inflater == NULL) {
                return E_INVALID_ARGUMENT;
            }

            inflater->CloneInContext(
                (IContext*)this->Probe(EIID_IContext),
                (ILayoutInflater**)&mInflater);
        }
        *object = mInflater;
        if (*object) {
            (*object)->AddRef();
        }
        return NOERROR;
    }
    return mBase->GetSystemService(name, object);
}

/**
 * Returns complete component name of this activity.
 *
 * @return Returns the complete component name for this activity
 */
ECode Activity::GetComponentName(
    /* [out] */ IComponentName** name)
{
    if (name == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    *name = mComponent;
    if (*name != NULL) (*name)->AddRef();
    return NOERROR;
}

/**
 * Change the title associated with this activity.  If this is a
 * top-level activity, the title for its window will change.  If it
 * is an embedded activity, the parent can do whatever it wants
 * with it.
 */
ECode Activity::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    OnTitleChanged(title, mTitleColor);

//    if (mParent != NULL) {
//        mParent->OnChildTitleChanged(this, title);
//    }
    return NOERROR;
}

/**
 * Change the title associated with this activity.  If this is a
 * top-level activity, the title for its window will change.  If it
 * is an embedded activity, the parent can do whatever it wants
 * with it.
 */
ECode Activity::SetTitleEx(
    /* [in] */ Int32 titleId)
{
//    setTitle(getText(titleId));
    return E_NOT_IMPLEMENTED;
}

ECode Activity::SetTitleColor(
    /* [in] */ Int32 textColor)
{
    mTitleColor = textColor;
    OnTitleChanged(mTitle, textColor);
    return NOERROR;
}

ECode Activity::GetTitle(
    /* [out] */ ICharSequence** title)
{
    if (title == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    *title = mTitle;
    if (*title != NULL) (*title)->AddRef();
    return NOERROR;
}

ECode Activity::GetTitleColor(
    /* [out] */ Int32* textColor)
{
    if (textColor == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    *textColor = mTitleColor;
    return NOERROR;
}

/*protected*/
ECode Activity::OnTitleChanged(
    /* [in] */ ICharSequence* title,
    /* [in] */ Int32 color)
{
    if (TRUE/*mTitleReady*/) {
        const AutoPtr<IWindow> win = GetWindow();
        if (win != NULL) {
            win->SetTitle(title);
            if (color != 0) {
                win->SetTitleColor(color);
            }
        }
    }
    return NOERROR;
}

ECode Activity::DispatchActivityResult(
    /* [in] */ const String& who,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return OnActivityResult(requestCode, resultCode, data);
}

ECode Activity::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}

ECode Activity::OnNewIntent(
    /* [in] */ IIntent *intent)
{
    return NOERROR;
}

ECode Activity::SetResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *resultData)
{
    mResultCode = resultCode;
    mResultData = resultData;

    return NOERROR;
}

AutoPtr<IIntent> Activity::GetIntent()
{
    return mIntent;
}

ECode Activity::MakeVisible()
{
    if (!mWindowAdded) {
        AutoPtr<IWindowManagerLayoutParams> wmlp;
        GetWindow()->GetAttributes((IWindowManagerLayoutParams**)&wmlp);
        AutoPtr<IViewManager> wm = GetWindowManager();
        wm->AddViewEx5(mDecor, wmlp);
        mWindowAdded = TRUE;
    }
    mDecor->SetVisibility(View_VISIBLE);
    return NOERROR;
}

ECode Activity::GetActivityToken(
    /* [out] */ IBinder** binder)
{
    if (!binder) {
        return E_INVALID_ARGUMENT;
    }
    if (mParent != NULL) {
        return mParent->GetActivityToken(binder);
    }
    *binder = mToken.Get();
    if (*binder) {
        (*binder)->AddRef();
    }
    return NOERROR;
}

ECode Activity::PerformStart()
{
    mCalled = FALSE;
    mInstrumentation->CallActivityOnStart((IActivity*)this);
//    if (!mCalled) {
//        throw new SuperNotCalledException(
//            "Activity " + mComponent.toShortString() +
//            " did not call through to super.onStart()");
//    }
    return NOERROR;
}

ECode Activity::PerformRestart()
{
//    final Int32 N = mManagedCursors.size();
//    for (Int32 i=0; i<N; i++) {
//        ManagedCursor mc = mManagedCursors.get(i);
//        if (mc.mReleased || mc.mUpdated) {
//            mc.mCursor.requery();
//            mc.mReleased = false;
//            mc.mUpdated = false;
//        }
//    }

    if (mStopped) {
        mStopped = FALSE;
        mCalled = FALSE;
        mInstrumentation->CallActivityOnRestart(this);
//        if (!mCalled) {
//            throw new SuperNotCalledException(
//                "Activity " + mComponent.toShortString() +
//                " did not call through to super.onRestart()");
//        }
        PerformStart();
    }
    return NOERROR;
}

ECode Activity::PerformResume()
{
    PerformRestart();

//    mLastNonConfigurationInstance = NULL;

    // First call onResume() -before- setting mResumed, so we don't
    // send out any status bar / menu notifications the client makes.
    mCalled = FALSE;
    mInstrumentation->CallActivityOnResume(this);
//    if (!mCalled) {
//        throw new SuperNotCalledException(
//            "Activity " + mComponent.toShortString() +
//            " did not call through to super.onResume()");
//    }

    // Now really resume, and install the current status bar and menu.
    mResumed = TRUE;
    mCalled = FALSE;
    OnPostResume();
//    if (!mCalled) {
//        throw new SuperNotCalledException(
//            "Activity " + mComponent.toShortString() +
//            " did not call through to super.onPostResume()");
//    }
    return NOERROR;
}

ECode Activity::PerformPause()
{
    return Pause();
}

ECode Activity::PerformUserLeaving()
{
    OnUserInteraction();
    OnUserLeaveHint();
    return NOERROR;
}

ECode Activity::PerformStop()
{
    if (!mStopped) {
//        if (mWindow != NULL) {
//            mWindow.closeAllPanels();
//        }

        mCalled = FALSE;
        mInstrumentation->CallActivityOnStop(this);
//        if (!mCalled) {
//            throw new SuperNotCalledException(
//                "Activity " + mComponent.toShortString() +
//                " did not call through to super.onStop()");
//        }

//        final Int32 N = mManagedCursors.size();
//        for (Int32 i=0; i<N; i++) {
//            ManagedCursor mc = mManagedCursors.get(i);
//            if (!mc.mReleased) {
//                mc.mCursor.deactivate();
//                mc.mReleased = TRUE;
//            }
//        }

        mStopped = TRUE;
    }
    mResumed = FALSE;
    return NOERROR;
}

/**
 * The hook for {@link ActivityThread} to save the state of this activity.
 *
 * Calls {@link #onSaveInstanceState(android.os.Bundle)}
 * and {@link #saveManagedDialogs(android.os.Bundle)}.
 *
 * @param outState The bundle to save the state to.
 */
ECode Activity::PerformSaveInstanceState(
    /* [in] */ IBundle* outState)
{
//    OnSaveInstanceState(outState);
//    saveManagedDialogs(outState);
    return NOERROR;
}

void Activity::OnUserInteraction()
{
}

void Activity::OnUserLeaveHint()
{
}

ECode Activity::AttachBaseContext(
    /* [in] */ IContext* newBase)
{
    if (mBase != NULL) {
        //throw new IllegalStateException("Base context already set");
        return E_INVALID_ARGUMENT;
    }
    mBase = newBase;

    return NOERROR;
}

ECode Activity::OnApplyThemeResource(
    /* [in] */ ITheme* theme,
    /* [in] */ Int32 resid,
    /* [in] */ Boolean first)
{
    return theme->ApplyStyle(resid, TRUE);
}

ECode Activity::InitializeTheme()
{
    Boolean first = mTheme == NULL;
    if (first) {
        AutoPtr<IResources> resources;
        GetResources((IResources**)&resources);
        resources->NewTheme((ITheme**)&mTheme);
        AutoPtr<ITheme> theme;
        mBase->GetTheme((ITheme**)&theme);
        if (theme != NULL) {
            mTheme->SetTo(theme);
        }
    }

    return OnApplyThemeResource(mTheme, mThemeResource, first);
}

/**
 * @deprecated Old no-arguments version of {@link #onCreateDialog(Int32, Bundle)}.
 */
AutoPtr<IDialog> Activity::OnCreateDialog(
    /* [in] */ Int32 id)
{
    return NULL;
}

/**
 * Callback for creating dialogs that are managed (saved and restored) for you
 * by the activity.  The default implementation calls through to
 * {@link #onCreateDialog(Int32)} for compatibility.
 *
 * <p>If you use {@link #showDialog(Int32)}, the activity will call through to
 * this method the first time, and hang onto it thereafter.  Any dialog
 * that is created by this method will automatically be saved and restored
 * for you, including whether it is showing.
 *
 * <p>If you would like the activity to manage saving and restoring dialogs
 * for you, you should override this method and handle any ids that are
 * passed to {@link #showDialog}.
 *
 * <p>If you would like an opportunity to prepare your dialog before it is shown,
 * override {@link #onPrepareDialog(Int32, Dialog, Bundle)}.
 *
 * @param id The id of the dialog.
 * @param args The dialog arguments provided to {@link #showDialog(Int32, Bundle)}.
 * @return The dialog.  If you return NULL, the dialog will not be created.
 *
 * @see #onPrepareDialog(Int32, Dialog, Bundle)
 * @see #showDialog(Int32, Bundle)
 * @see #dismissDialog(Int32)
 * @see #removeDialog(Int32)
 */
AutoPtr<IDialog> Activity::OnCreateDialog(
    /* [in] */ Int32 id,
    /* [in] */ IBundle* args)
{
    return OnCreateDialog(id);
}

/**
 * @deprecated Old no-arguments version of
 * {@link #onPrepareDialog(Int32, Dialog, Bundle)}.
 */
void Activity::OnPrepareDialog(
    /* [in] */ Int32 id,
    /* [in] */ IDialog* dialog)
{
    dialog->SetOwnerActivity(this);
}

/**
 * Provides an opportunity to prepare a managed dialog before it is being
 * shown.  The default implementation calls through to
 * {@link #onPrepareDialog(Int32, Dialog)} for compatibility.
 *
 * <p>
 * Override this if you need to update a managed dialog based on the state
 * of the application each time it is shown. For example, a time picker
 * dialog might want to be updated with the current time. You should call
 * through to the superclass's implementation. The default implementation
 * will set this Activity as the owner activity on the Dialog.
 *
 * @param id The id of the managed dialog.
 * @param dialog The dialog.
 * @param args The dialog arguments provided to {@link #showDialog(Int32, Bundle)}.
 * @see #onCreateDialog(Int32, Bundle)
 * @see #showDialog(Int32)
 * @see #dismissDialog(Int32)
 * @see #removeDialog(Int32)
 */
void Activity::OnPrepareDialog(
    /* [in] */ Int32 id,
    /* [in] */ IDialog* dialog,
    /* [in] */ IBundle* args)
{
    OnPrepareDialog(id, dialog);
}

/**
 * Simple version of {@link #showDialog(Int32, Bundle)} that does not
 * take any arguments.  Simply calls {@link #showDialog(Int32, Bundle)}
 * with NULL arguments.
 */
ECode Activity::ShowDialog(
    /* [in] */ Int32 id)
{
    Boolean res;
    return ShowDialogEx(id, NULL, &res);
}

/**
 * Show a dialog managed by this activity.  A call to {@link #onCreateDialog(Int32, Bundle)}
 * will be made with the same id the first time this is called for a given
 * id.  From thereafter, the dialog will be automatically saved and restored.
 *
 * <p>Each time a dialog is shown, {@link #onPrepareDialog(Int32, Dialog, Bundle)} will
 * be made to provide an opportunity to do any timely preparation.
 *
 * @param id The id of the managed dialog.
 * @param args Arguments to pass through to the dialog.  These will be saved
 * and restored for you.  Note that if the dialog is already created,
 * {@link #onCreateDialog(Int32, Bundle)} will not be called with the new
 * arguments but {@link #onPrepareDialog(Int32, Dialog, Bundle)} will be.
 * If you need to rebuild the dialog, call {@link #removeDialog(Int32)} first.
 * @return Returns TRUE if the Dialog was created; false is returned if
 * it is not created because {@link #onCreateDialog(Int32, Bundle)} returns false.
 *
 * @see Dialog
 * @see #onCreateDialog(Int32, Bundle)
 * @see #onPrepareDialog(Int32, Dialog, Bundle)
 * @see #dismissDialog(Int32)
 * @see #removeDialog(Int32)
 */
ECode Activity::ShowDialogEx(
    /* [in] */ Int32 id,
    /* [in] */ IBundle* args,
    /* [out] */ Boolean* res)
{
    if (!res) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mManagedDialogs == NULL) {
        mManagedDialogs = new HashMap<Int32, ManagedDialog*>();
    }

    ManagedDialog* md = NULL;
    HashMap<Int32, ManagedDialog*>::Iterator find = mManagedDialogs->Find(id);
    if (find == mManagedDialogs->End()) {
        md = new ManagedDialog();
        md->mDialog = CreateDialog(id, NULL, args);
        if (md->mDialog == NULL) {
            *res = FALSE;
            return NOERROR;
        }
        (*mManagedDialogs)[id] = md;
    }
    else {
        md = find->mSecond;
    }

    md->mArgs = args;
    OnPrepareDialog(id, md->mDialog, args);
    md->mDialog->Show();
    *res = TRUE;

    return NOERROR;
}

/**
 * Dismiss a dialog that was previously shown via {@link #showDialog(Int32)}.
 *
 * @param id The id of the managed dialog.
 *
 * @throws IllegalArgumentException if the id was not previously shown via
 *   {@link #showDialog(Int32)}.
 *
 * @see #onCreateDialog(Int32, Bundle)
 * @see #onPrepareDialog(Int32, Dialog, Bundle)
 * @see #showDialog(Int32)
 * @see #removeDialog(Int32)
 */
ECode Activity::DismissDialog(
    /* [in] */ Int32 id)
{
    if (mManagedDialogs == NULL) {
        //throw missingDialog(id);
        return NOERROR;
    }

    HashMap<Int32, ManagedDialog*>::Iterator find = mManagedDialogs->Find(id);
    if (find == mManagedDialogs->End()) {
        //throw missingDialog(id);
        return NOERROR;
    }
    find->mSecond->mDialog->Dismiss();

    return NOERROR;
}

/**
 * Removes any internal references to a dialog managed by this Activity.
 * If the dialog is showing, it will dismiss it as part of the clean up.
 *
 * <p>This can be useful if you know that you will never show a dialog again and
 * want to avoid the overhead of saving and restoring it in the future.
 *
 * <p>As of {@link android.os.Build.VERSION_CODES#GINGERBREAD}, this function
 * will not throw an exception if you try to remove an ID that does not
 * currently have an associated dialog.</p>
 *
 * @param id The id of the managed dialog.
 *
 * @see #onCreateDialog(Int32, Bundle)
 * @see #onPrepareDialog(Int32, Dialog, Bundle)
 * @see #showDialog(Int32)
 * @see #dismissDialog(Int32)
 */
ECode Activity::RemoveDialog(
    /* [in] */ Int32 id)
{
    if (mManagedDialogs != NULL) {
        HashMap<Int32, ManagedDialog*>::Iterator find = mManagedDialogs->Find(id);
        if (find != mManagedDialogs->End()) {
            find->mSecond->mDialog->Dismiss();
            delete find->mSecond;
            mManagedDialogs->Erase(find);
        }
    }

    return NOERROR;
}

ECode Activity::OnCreatePanelMenu(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToShow) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (allowToShow == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (featureId == Window_FEATURE_OPTIONS_PANEL) {
        return OnCreateOptionsMenu(menu, allowToShow);
    }

    *allowToShow = FALSE;
    return NOERROR;
}

ECode Activity::OnCreateOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToShow) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (allowToShow == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mParent != NULL) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        mParent->OnCreateOptionsMenu(menu, allowToShow);
    }

    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    *allowToShow = TRUE;
    return NOERROR;
}

ECode Activity::OnPreparePanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IView* view,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToShow) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (allowToShow == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *allowToShow = TRUE;

    if (featureId == Window_FEATURE_OPTIONS_PANEL && menu != NULL) {
        Boolean goforit = FALSE;
        OnPrepareOptionsMenu(menu, &goforit);
        printf("==== File: %s, Line: %d ====, FUNC : %s, goforit=[%d].\n", __FILE__, __LINE__, __FUNCTION__, goforit);

        Boolean visible = FALSE;
        menu->HasVisibleItems(&visible);
        printf("==== File: %s, Line: %d ====, FUNC : %s, visible=[%d].\n", __FILE__, __LINE__, __FUNCTION__, visible);

        *allowToShow = goforit && visible;
    }

    printf("==== File: %s, Line: %d ====, FUNC : %s, allowToShow=[%d].\n", __FILE__, __LINE__, __FUNCTION__, *allowToShow);
    return NOERROR;
}

ECode Activity::OnPrepareOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* res) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (res == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *res = TRUE;

    if (mParent != NULL) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        mParent->OnPrepareOptionsMenu(menu, res);
    }

    return NOERROR;
}

ECode Activity::OnMenuOpened(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToOpen) {
    if (allowToOpen == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *allowToOpen = TRUE;

    return NOERROR;
}

ECode Activity::OnMenuItemSelected(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* toFinish) {
    if (toFinish == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    switch(featureId) {
        case Window_FEATURE_OPTIONS_PANEL: {
            OnOptionsItemSelected(item, toFinish);
            return NOERROR;
        }
        case Window_FEATURE_CONTEXT_MENU: {
            OnContextItemSelected(item, toFinish);
            return NOERROR;
        }
    }

    *toFinish = FALSE;

    return NOERROR;
}

ECode Activity::OnOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* res) {
    if (res == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *res = FALSE;
    if (mParent != NULL) {
        return mParent->OnOptionsItemSelected(item, res);
    }

    return NOERROR;
}

ECode Activity::OnContextItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* res) {
    if (res == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *res = FALSE;
    if (mParent != NULL) {
        return mParent->OnContextItemSelected(item, res);
    }

    return NOERROR;
}

ECode Activity::OnPanelClosed(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu) {
    switch(featureId) {
        case Window_FEATURE_OPTIONS_PANEL: {
            return OnOptionsMenuClosed(menu);
        }
        case Window_FEATURE_CONTEXT_MENU: {
            return OnContextMenuClosed(menu);
        }
    }

    return NOERROR;
}

ECode Activity::OnOptionsMenuClosed(
    /* [in] */ IMenu* menu) {
    if (mParent != NULL) {
        return mParent->OnOptionsMenuClosed(menu);
    }

    return NOERROR;
}

ECode Activity::OnContextMenuClosed(
        /* [in] */ IMenu* menu) {
    if (mParent != NULL) {
        return mParent->OnContextMenuClosed(menu);
    }

    return NOERROR;
}

ECode Activity::OpenOptionsMenu() {
    return mWindow->OpenPanel(Window_FEATURE_OPTIONS_PANEL, NULL);
}


ECode Activity::CloseOptionsMenu() {
    return mWindow->ClosePanel(Window_FEATURE_OPTIONS_PANEL);
}


ECode Activity::RegisterForContextMenu(
    /* [in] */ IView* view) {
    if (view == NULL) {
        return E_INVALID_ARGUMENT;
    }

    return view->SetOnCreateContextMenuListener(
        (IViewOnCreateContextMenuListener*)this->Probe(
        EIID_IViewOnCreateContextMenuListener));
}


ECode Activity::UnregisterForContextMenu(
    /* [in] */ IView* view) {
    if (view == NULL) {
        return E_INVALID_ARGUMENT;
    }

    return view->SetOnCreateContextMenuListener(NULL);
}


ECode Activity::OpenContextMenu(
    /* [in] */ IView* view) {
    Boolean res;
    return view->ShowContextMenu(&res);
}

ECode Activity::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isConsumed) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    *isConsumed = FALSE;
    OnUserInteraction();

    AutoPtr<IWindow> win;
    GetWindowEx((IWindow**) &win);

    Boolean succeeded;
    win->SuperDispatchKeyEvent(event, &succeeded);
    printf("==== File: %s, Line: %d ====, FUNC : %s, succeeded==[%d].\n", __FILE__, __LINE__, __FUNCTION__, succeeded);
    if (succeeded) {
        *isConsumed = TRUE;
        return NOERROR;
    }

    AutoPtr<IView> decor = mDecor;
    if (decor == NULL) {
        win->GetDecorView((IView**) &decor);
    }

    AutoPtr<IDispatcherState> dispatcher;
    if (decor != NULL) {
        decor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);
    }
    else {
        dispatcher = NULL;
    }
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    event->DispatchEx((IKeyEventCallback*)this->Probe(EIID_IKeyEventCallback), dispatcher
                , this->Probe(EIID_IInterface), isConsumed);

    return NOERROR;
}

ECode Activity::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* isConsumed) {
    *isConsumed = FALSE;

    Int32 action;
    event->GetAction(&action);
    if (action == MotionEvent_ACTION_DOWN) {
        OnUserInteraction();
    }

    AutoPtr<IWindow> win;
    GetWindowEx((IWindow**) &win);

    Boolean succeeded;
    win->SuperDispatchTouchEvent(event, &succeeded);

    if (succeeded) {
        *isConsumed = TRUE;
        return NOERROR;
    }

    return OnTouchEvent(event, isConsumed);
}

ECode Activity::OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res) {
    *res = FALSE;
    return NOERROR;
}

ECode Activity::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* isConsumed) {
    *isConsumed = FALSE;

    OnUserInteraction();
    AutoPtr<IWindow> win;
    GetWindowEx((IWindow**) &win);

    Boolean succeeded;
    win->SuperDispatchTrackballEvent(event, &succeeded);

    if (succeeded) {
        *isConsumed = TRUE;
        return NOERROR;
    }

    return OnTrackballEvent(event, isConsumed);
}


ECode Activity::OnTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res) {
    *res = FALSE;
    return NOERROR;
}

ECode Activity::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* isCompleted) {
    *isCompleted = FALSE;

    //TODO
/*    event.setClassName(getClass().getName());
    event.setPackageName(getPackageName());

    LayoutParams params = getWindow().getAttributes();
    boolean isFullScreen = (params.width == LayoutParams.MATCH_PARENT) &&
        (params.height == LayoutParams.MATCH_PARENT);
    event.setFullScreen(isFullScreen);

    CharSequence title = getTitle();
    if (!TextUtils.isEmpty(title)) {
       event.getText().add(title);
    }

    return TRUE;*/

    return NOERROR;
}

ECode Activity::OnCreatePanelView(
    /* [in] */ Int32 featureId,
    /* [out] */ IView** view) {
    *view = NULL;
    return NOERROR;
}

ECode Activity::OnWindowAttributesChanged(
    /* [in] */ IWindowManagerLayoutParams* attrs) {
    if (mParent == NULL) {
        AutoPtr<IView> decor = mDecor;
        if (decor != NULL) {
            AutoPtr<IViewParent> parent;
            decor->GetParent((IViewParent**) &parent);

            if (parent != NULL) {
                AutoPtr<IWindowManager> winmanager;
                GetWindowManagerEx((IWindowManager**) &winmanager);

                if (winmanager != NULL) {
                    winmanager->UpdateViewLayout(decor, attrs);
                }
            }
        }
    }

    return NOERROR;
}

ECode Activity::OnContentChanged() {
    return NOERROR;
}

ECode Activity::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus) {
    return NOERROR;
}

ECode Activity::OnAttachedToWindow() {
    return NOERROR;
}

ECode Activity::OnDetachedFromWindow() {
    return NOERROR;
}

ECode Activity::OnSearchRequested(
    /* [out] */ Boolean* isLaunched) {
    return E_NOT_IMPLEMENTED;
}

ECode Activity::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result) {
    *result = FALSE;

    if (keyCode == KeyEvent_KEYCODE_BACK) {
        AutoPtr<IApplicationInfo> appInfo;
        GetApplicationInfo((IApplicationInfo**) &appInfo);
        Int32 targetSdkVersion;
        appInfo->GetTargetSdkVersion(&targetSdkVersion);

        //TODO
        //if (targetSdkVersion >= Build::VERSION_CODES::ECLAIR) {
        if (targetSdkVersion >= 5) {
            event->StartTracking();
        } else {
            OnBackPressed();
        }

        *result = TRUE;
        return NOERROR;
    }

    if (mDefaultKeyMode == DEFAULT_KEYS_DISABLE) {
        *result = FALSE;
        return NOERROR;
    } else if (mDefaultKeyMode == DEFAULT_KEYS_SHORTCUT) {
        AutoPtr<IWindow> win;
        GetWindowEx((IWindow**) &win);

        Boolean state;
        win->PerformPanelShortcut(Window_FEATURE_OPTIONS_PANEL,
                keyCode, event, MENU_FLAG_ALWAYS_PERFORM_CLOSE, &state);

        if (state) {
            *result = TRUE;
        }

        return NOERROR;
    } else {
        // Common code for DEFAULT_KEYS_DIALER & DEFAULT_KEYS_SEARCH_*
        Boolean clearSpannable = FALSE;
        Boolean handled, isSystem;
        Int32 repeatCount;
        event->GetRepeatCount(&repeatCount);
        event->IsSystem(&isSystem);

        if ((repeatCount != 0) || isSystem) {
            clearSpannable = TRUE;
            *result = FALSE;
        }
        //TODO
/*        else {
            handled = TextKeyListener.getInstance().onKeyDown(
                    null, mDefaultKeySsb, keyCode, event);
            if (handled && mDefaultKeySsb.length() > 0) {
                // something useable has been typed - dispatch it now.

                final String str = mDefaultKeySsb.toString();
                clearSpannable = TRUE;

                switch (mDefaultKeyMode) {
                case DEFAULT_KEYS_DIALER:
                    Intent intent = new Intent(Intent.ACTION_DIAL,  Uri.parse("tel:" + str));
                    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    startActivity(intent);
                    break;
                case DEFAULT_KEYS_SEARCH_LOCAL:
                    startSearch(str, FALSE, null, FALSE);
                    break;
                case DEFAULT_KEYS_SEARCH_GLOBAL:
                    startSearch(str, FALSE, null, TRUE);
                    break;
                }
            }
        }
        if (clearSpannable) {
            mDefaultKeySsb.clear();
            mDefaultKeySsb.clearSpans();
            Selection.setSelection(mDefaultKeySsb,0);
        }*/

        *result = handled;
        return NOERROR;
    }

    return NOERROR;
}

ECode Activity::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result) {
    *result = FALSE;

    return NOERROR;
}

ECode Activity::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result) {
    *result = FALSE;

    AutoPtr<IApplicationInfo> appInfo;
    GetApplicationInfo((IApplicationInfo**) &appInfo);
    Int32 targetSdkVersion;
    appInfo->GetTargetSdkVersion(&targetSdkVersion);

    //TODO
    //if (targetSdkVersion >= Build::VERSION_CODES::ECLAIR) {
    if (targetSdkVersion >= 5) {
        Boolean tracking, canceled;
        event->IsTracking(&tracking);
        event->IsCanceled(&canceled);

        if (keyCode == KeyEvent_KEYCODE_BACK && tracking && !canceled) {
            OnBackPressed();

            *result = TRUE;
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode Activity::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result) {
    *result = FALSE;

    return NOERROR;
}