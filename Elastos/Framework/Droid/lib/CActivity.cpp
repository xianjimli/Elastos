
#include "CActivity.h"
#include "frameworkext.h"

#include <stdio.h>

CActivity::CActivity()
    : mWindowAdded(FALSE)
    , mVisibleFromClient(TRUE)
    , mTitleColor(0)
    , mTitleReady(FALSE)
{
    AddRef();
}

CActivity::~CActivity()
{}

ECode CActivity::Initialize()
{
    return NOERROR;
}

PInterface CActivity::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IActivity) {
        return (IActivity*)this;
    }
    else if (riid == EIID_IContextWrapper) {
        return (IContextWrapper*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }
    else {
        return CBaseObject::Probe(riid);
    }
}

UInt32 CActivity::AddRef()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is increased
    Int32 ref = atomic_inc(&mRef);
    // so we should increase ref before return
    return ++ref;
}

UInt32 CActivity::Release()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is decreased
    Int32 ref = atomic_dec(&mRef);
    // so we should decrease ref
    if (--ref == 0) {
        delete this;
    }
    assert(ref >= 0);
    return ref;
}

ECode CActivity::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IActivity *)this) {
        *pIID = EIID_IActivity;
        return NOERROR;
    }
    else {
        return CBaseObject::GetInterfaceID(pObject, pIID);
    }
}

ECode CActivity::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivity::IsChild(
    /* [out] */ Boolean* isChild)
{
    if (isChild == NULL) return E_INVALID_ARGUMENT;
    *isChild = mParent != NULL;
    return NOERROR;
}

ECode CActivity::GetID(
    /* [out] */ String* id)
{
    if (id == NULL) return E_INVALID_ARGUMENT;
    *id = String::Duplicate(mEmbeddedID);
    return NOERROR;
}

ECode CActivity::CreateCapsuleContext(
    /* [in] */ String capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateCapsuleContext(capsuleName, flags, ctx);
}

ECode CActivity::CheckCallingPermission(
    /* [in] */ String permission,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivity::EnforceCallingOrSelfPermission(
    /* [in] */ String permission,
    /* [in] */ String message)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivity::GetResources(
    /* [out] */ IResources** resources)
{
    return mBase->GetResources(resources);
}

ECode CActivity::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return mBase->GetContentResolver(resolver);
}

ECode CActivity::SetTheme(
    /* [in] */ Int32 resid)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivity::GetTheme(
    /* [out] */ ITheme** theme)
{
    return mBase->GetTheme(theme);
}

ECode CActivity::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributes(attrs, styles);
}

ECode CActivity::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode CActivity::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode CActivity::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributesEx3(set, attrs,
            defStyleAttr, defStyleRes, styles);
}

ECode CActivity::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return mBase->GetClassLoader(loader);
}

ECode CActivity::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    return mBase->GetCapsuleName(capsuleName);
}

ECode CActivity::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    if (ctx == NULL) return E_INVALID_ARGUMENT;

    *ctx = mBase;
    if (*ctx) (*ctx)->AddRef();
    return NOERROR;
}

// internal use.
ECode CActivity::IsStartedActivity(
    /* [out] */ Boolean* isStartedActivity)
{
    if (!isStartedActivity) {
        return E_INVALID_ARGUMENT;
    }
    *isStartedActivity = mStartedActivity;
    return NOERROR;
}

ECode CActivity::GetWindowEx(
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

ECode CActivity::SetDecorView(
    /* [in] */ IView* decor)
{
    mDecor = decor;
    return NOERROR;
}

ECode CActivity::GetDecorView(
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

ECode CActivity::IsVisibleFromClient(
    /* [out] */ Boolean* visible)
{
    if (!visible) {
        return E_INVALID_ARGUMENT;
    }
    *visible = mVisibleFromClient;
    return NOERROR;
}

ECode CActivity::SetWindowAdded(
    /* [in] */ Boolean added)
{
    mWindowAdded = added;
    return NOERROR;
}

ECode CActivity::GetWindowManagerEx(
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

ECode CActivity::Create(
    /* [in] */ IBundle* savedInstanceState)
{
    return OnCreate(savedInstanceState);
}

ECode CActivity::PostCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    return OnPostCreate(savedInstanceState);
}

ECode CActivity::Start()
{
    ECode ec = OnStart();

    return ec;
}

ECode CActivity::Restart()
{
    OnRestart();

    return Start();
}

ECode CActivity::Resume()
{
    return OnResume();
}

ECode CActivity::Pause()
{
    return OnPause();
}

ECode CActivity::Stop()
{
    return OnStop();
}

ECode CActivity::Destroy()
{
    return OnDestroy();
}

ECode CActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
//    mVisibleFromClient = mWindow.getWindowStyle().getBoolean(
//            com.android.internal.R.styleable.Window_windowNoDisplay, true);
    mCalled = TRUE;
    return NOERROR;
}

ECode CActivity::OnPostCreate(
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

ECode CActivity::OnStart()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode CActivity::OnRestart()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode CActivity::OnResume()
{
    return NOERROR;
}

ECode CActivity::OnPause()
{
    return NOERROR;
}

ECode CActivity::OnPostResume()
{
//    final Window win = getWindow();
//    if (win != null) win.makeActive();
    mCalled = TRUE;
    return NOERROR;
}

ECode CActivity::OnStop()
{
    return NOERROR;
}

ECode CActivity::OnDestroy()
{
    return NOERROR;
}

AutoPtr<IView> CActivity::FindViewById(
    /* [in] */ Int32 id)
{
    AutoPtr<IView> view;
    GetWindow()->FindViewById(id, (IView**)&view);
    return view;
}

ECode CActivity::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    return GetWindow()->SetContentView(layoutResID);
}

ECode CActivity::SetContentView(
    /* [in] */ IView* view)
{
    return GetWindow()->SetContentViewEx(view);
}

ECode CActivity::SetContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return GetWindow()->SetContentViewEx2(view, params);
}

AutoPtr<IWindowManager> CActivity::GetWindowManager()
{
    return mWindowManager;
}

AutoPtr<IWindow> CActivity::GetWindow()
{
    return mWindow;
}

ECode CActivity::Attach(
    /* [in] */ IContext* context,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ IInstrumentation* instr,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICharSequence* title,
    /* [in] */ IActivity* parent,
    /* [in] */ String id,
    /* [in] */ IInterface* lastNonConfigurationInstance,
    /* [in] */ IConfiguration* config)
{
    return AttachEx(context, apartment, instr, token, 0, application,
            intent, info, title, parent, id, lastNonConfigurationInstance, NULL, config);
}

ECode CActivity::AttachEx(
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
    /* [in] */ String id,
    /* [in] */ IInterface* lastNonConfigurationInstance,
    /* [in] */ IObjectStringMap* lastNonConfigurationChildInstances,
    /* [in] */ IConfiguration* config)
{
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
        return ec;
    }
//    mWindow.setCallback(this);
//    if (info.softInputMode != WindowManager.LayoutParams.SOFT_INPUT_STATE_UNSPECIFIED) {
//        mWindow.setSoftInputMode(info.softInputMode);
//    }

    mTitle = title;

    String str;
    mComponent->FlattenToString(&str);
    mWindow->SetWindowManager(NULL, mToken, str);
    String::Free(str);
//    if (mParent != null) {
//        mWindow.setContainer(mParent.getWindow());
//    }
    mWindow->GetWindowManager((IWindowManager**)&mWindowManager);

    return NOERROR;
}

ECode CActivity::DispatchNewIntent(
    /* [in] */ IIntent *intent)
{
    return OnNewIntent(intent);
}

ECode CActivity::SetCalled(
    /* [in] */ Boolean called)
{
    mCalled = called;
    return NOERROR;
}

ECode CActivity::IsCalled(
    /* [out] */ Boolean* called)
{
    if (called == NULL) return E_INVALID_ARGUMENT;
    *called = mCalled;
    return NOERROR;
}

ECode CActivity::SetFinished(
    /* [in] */ Boolean finished)
{
    mFinished = finished;
    return NOERROR;
}

ECode CActivity::IsFinished(
    /* [out] */ Boolean* finished)
{
    if (finished == NULL) return E_INVALID_ARGUMENT;
    *finished = mFinished;
    return NOERROR;
}

ECode CActivity::StartActivity(
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
ECode CActivity::StartActivityForResult(
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

ECode CActivity::StartActivityFromChild(
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
        String::Free(id);
    }
    return NOERROR;
}

ECode CActivity::Finish()
{
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<IActivityManager> activityManager;

    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    serviceManager->GetService("ActivityManagerService", (IInterface**)&activityManager);
    assert(activityManager != NULL);
    Boolean finished;
    return activityManager->FinishActivity(mToken, mResultCode, mResultData, &finished);
}

ECode CActivity::SendBroadcast(
    /* [in] */ IIntent *intent)
{
    return mBase->SendBroadcast(intent);
}

ECode CActivity::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return mBase->StartService(service, name);
}

ECode CActivity::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StopService(service, succeeded);
}

ECode CActivity::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, succeeded);
}

ECode CActivity::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return mBase->UnbindService(conn);
}

ECode CActivity::GetSystemService(
    /* [in] */ String name,
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

    if (!String(Context_WINDOW_SERVICE).Compare(name)) {
        *object = (IInterface*)mWindowManager.Get();
        if (*object != NULL) (*object)->AddRef();
        return NOERROR;
    }
    else if (!String(Context_SEARCH_SERVICE).Compare(name)) {
//        ensureSearchManager();
//        return mSearchManager;
    }
//    else if (!String(Context_LAYOUT_INFLATER_SERVICE).Compare(name)) {
//        if (mInflater == NULL) {
//            mInflater = LayoutInflater.from(mBase).cloneInContext(this);
//        }
//        return mInflater;
//    }
    return mBase->GetSystemService(name, object);
}

/**
 * Change the title associated with this activity.  If this is a
 * top-level activity, the title for its window will change.  If it
 * is an embedded activity, the parent can do whatever it wants
 * with it.
 */
ECode CActivity::SetTitle(
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
ECode CActivity::SetTitleEx(
    /* [in] */ Int32 titleId)
{
//    setTitle(getText(titleId));
    return E_NOT_IMPLEMENTED;
}

ECode CActivity::SetTitleColor(
    /* [in] */ Int32 textColor)
{
    mTitleColor = textColor;
    OnTitleChanged(mTitle, textColor);
    return NOERROR;
}

ECode CActivity::GetTitle(
    /* [out] */ ICharSequence** title)
{
    if (title == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    *title = mTitle;
    if (*title != NULL) (*title)->AddRef();
    return NOERROR;
}

ECode CActivity::GetTitleColor(
    /* [out] */ Int32* textColor)
{
    if (textColor == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    *textColor = mTitleColor;
    return NOERROR;
}

/*protected*/
ECode CActivity::OnTitleChanged(
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

ECode CActivity::DispatchActivityResult(
    /* [in] */ String who,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return OnActivityResult(requestCode, resultCode, data);
}

ECode CActivity::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}

ECode CActivity::OnNewIntent(
    /* [in] */ IIntent *intent)
{
    return NOERROR;
}

ECode CActivity::SetResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *resultData)
{
    mResultCode = resultCode;
    mResultData = resultData;

    return NOERROR;
}

AutoPtr<IIntent> CActivity::GetIntent()
{
    return mIntent;
}

ECode CActivity::MakeVisible()
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

ECode CActivity::GetActivityToken(
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

ECode CActivity::PerformStart()
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

ECode CActivity::PerformRestart()
{
//    final int N = mManagedCursors.size();
//    for (int i=0; i<N; i++) {
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

ECode CActivity::PerformResume()
{
    PerformRestart();

//    mLastNonConfigurationInstance = null;

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

ECode CActivity::PerformPause()
{
    return Pause();
}

ECode CActivity::PerformUserLeaving()
{
    OnUserInteraction();
    OnUserLeaveHint();
    return NOERROR;
}

ECode CActivity::PerformStop()
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

//        final int N = mManagedCursors.size();
//        for (int i=0; i<N; i++) {
//            ManagedCursor mc = mManagedCursors.get(i);
//            if (!mc.mReleased) {
//                mc.mCursor.deactivate();
//                mc.mReleased = true;
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
ECode CActivity::PerformSaveInstanceState(
    /* [in] */ IBundle* outState)
{
//    OnSaveInstanceState(outState);
//    saveManagedDialogs(outState);
    return NOERROR;
}

void CActivity::OnUserInteraction()
{
}

void CActivity::OnUserLeaveHint()
{
}
