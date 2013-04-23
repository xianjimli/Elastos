
#include "server/CInputMethodManagerService.h"
#include "server/CCapsuleManagerService.h"
#include "server/CInputMethodManagerServiceMethodCallback.h"
#include "os/ServiceManager.h"
#include "os/Binder.h"
#include "os/SystemClock.h"


using namespace Elastos::Core;


CInputMethodManagerService::SessionState::SessionState(
    /* [in] */ ClientState* client,
    /* [in] */ IInputMethod* method,
    /* [in] */ IInputMethodSession* session)
    : mClient(client)
    , mMethod(method)
    , mSession(session)
{}

UInt32 CInputMethodManagerService::SessionState::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 CInputMethodManagerService::SessionState::Release()
{
	return ElRefBase::Release();
}

CInputMethodManagerService::ClientState::ClientState(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IInputContext* inputContext,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid)
	: mClient(client)
	, mInputContext(inputContext)
	, mUid(uid)
	, mPid(pid)
	, mSessionRequested(FALSE)
{
    assert(mInputContext != NULL);
    ASSERT_SUCCEEDED(CInputBinding::New(NULL, IBinder::Probe(mInputContext),
            mUid, mPid, (IInputBinding**)&mBinding));
}

UInt32 CInputMethodManagerService::ClientState::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 CInputMethodManagerService::ClientState::Release()
{
	return ElRefBase::Release();
}

CInputMethodManagerService::ScreenOnOffReceiver::ScreenOnOffReceiver(
    /* [in] */ CInputMethodManagerService* host)
    : mHost(host)
{}

ECode CInputMethodManagerService::ScreenOnOffReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(Intent_ACTION_SCREEN_ON)) {
        mHost->mScreenOn = TRUE;
    }
    else if (action.Equals(Intent_ACTION_SCREEN_OFF)) {
        mHost->mScreenOn = FALSE;
    }
    else if (action.Equals(Intent_ACTION_CLOSE_SYSTEM_DIALOGS)) {
        mHost->HideInputMethodMenu();
        return NOERROR;
    }
    else {
        assert(0);
        // Slog.w(TAG, "Unexpected intent " + intent);
    }

    // Inform the current client of the change in active status
    //try {
    if (mHost->mCurClient != NULL && mHost->mCurClient->mClient != NULL) {
        return mHost->mCurClient->mClient->SetActive(mHost->mScreenOn);
    }
    // } catch (RemoteException e) {
    //     Slog.w(TAG, "Got RemoteException sending 'screen on/off' notification to pid "
    //             + mCurClient.pid + " uid " + mCurClient.uid);
    // }

    return NOERROR;
}


CString CInputMethodManagerService::TAG = "CInputMethodManagerService";
CString CInputMethodManagerService::ENABLED_STR = "com.android.inputmethod.latin/.LatinIME:com.android.inputmethod.pinyin/.PinyinIME:SoftKeyboard/CSoftKeyboard";
//CString CInputMethodManagerService::DEFAULT_INPUT_METHOD = "com.android.inputmethod.pinyin/.PinyinIME";
CString CInputMethodManagerService::DEFAULT_INPUT_METHOD = "SoftKeyboard/CSoftKeyboard";
const Int32 CInputMethodManagerService::MSG_UNBIND_INPUT;
const Int32 CInputMethodManagerService::MSG_BIND_INPUT;
const Int32 CInputMethodManagerService::MSG_SHOW_SOFT_INPUT;
const Int32 CInputMethodManagerService::MSG_HIDE_SOFT_INPUT;
const Int32 CInputMethodManagerService::MSG_ATTACH_TOKEN;
const Int32 CInputMethodManagerService::MSG_CREATE_SESSION;
const Int32 CInputMethodManagerService::MSG_START_INPUT;
const Int32 CInputMethodManagerService::MSG_RESTART_INPUT;
const Int32 CInputMethodManagerService::MSG_UNBIND_METHOD;
const Int32 CInputMethodManagerService::MSG_BIND_METHOD;
const Int64 CInputMethodManagerService::TIME_TO_RECONNECT;

CInputMethodManagerService::CInputMethodManagerService()
    : mSystemReady(FALSE)
    , mCurSeq(0)
    , mHaveConnection(FALSE)
    , mShowRequested(FALSE)
    , mShowExplicitlyRequested(FALSE)
    , mShowForced(FALSE)
    , mInputShown(FALSE)
    , mLastBindTime(0)
    , mBoundToMethod(FALSE)
    , mScreenOn(TRUE)
    , mIms(NULL)
    , mItems(NULL)
{
    ASSERT_SUCCEEDED(CInputBindResult::New(NULL, String(), -1, (IInputBindResult**)&mNoBinding));
    mStringColonSplitter = new TextUtils::SimpleStringSplitter(':');
}

CInputMethodManagerService::~CInputMethodManagerService()
{
    if (mStringColonSplitter != NULL) {
        delete mStringColonSplitter;
        mStringColonSplitter = NULL;
    }
    if (mIms != NULL) {
        for (Int32 i = 0; i < mIms->GetLength(); ++i) {
            (*mIms)[i] = NULL;
        }
        ArrayOf< AutoPtr<IInputMethodInfo> >::Free(mIms);
    }
    if (mItems != NULL) {
        for (Int32 i = 0; i < mItems->GetLength(); ++i) {
            (*mItems)[i] = NULL;
        }
        ArrayOf< AutoPtr<ICharSequence> >::Free(mItems);
    }
}

ECode CInputMethodManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IStatusBarService* statusBar)
{
    mContext = context;
    AutoPtr<IApartmentHelper> helper;
    ASSERT_SUCCEEDED(CApartmentHelper::AcquireSingleton((IApartmentHelper**)&helper));
    ASSERT_SUCCEEDED(helper->GetMainApartment((IApartment**)&mHandler));
    mIWindowManager = IWindowManager::Probe(
            ServiceManager::GetService(String(Context_WINDOW_SERVICE)).Get());
    // mCaller = new HandlerCaller(context, new HandlerCaller.Callback() {
    //     public void executeMessage(Message msg) {
    //         handleMessage(msg);
    //     }
    // });
    ASSERT_SUCCEEDED(helper->GetMainApartment((IApartment**)&mCaller));

    //(new MyPackageMonitor()).register(mContext, TRUE);

    AutoPtr<IIntentFilter> screenOnOffFilt;
    CIntentFilter::New((IIntentFilter**)&screenOnOffFilt);
    screenOnOffFilt->AddAction(String(Intent_ACTION_SCREEN_ON));
    screenOnOffFilt->AddAction(String(Intent_ACTION_SCREEN_OFF));
    screenOnOffFilt->AddAction(String(Intent_ACTION_CLOSE_SYSTEM_DIALOGS));
    //mContext.registerReceiver(new ScreenOnOffReceiver(), screenOnOffFilt);

    mStatusBar = statusBar;
    //statusBar->SetIconVisibility(String("ime"), FALSE);

    BuildInputMethodListLocked(mMethodList, mMethodMap);

    // String enabledStr = Settings.Secure.getString(
    //         mContext.getContentResolver(),
    //         Settings.Secure.ENABLED_INPUT_METHODS);
    String enabledStr(ENABLED_STR);
    // Slog.i(TAG, "Enabled input methods: " + enabledStr);
    // final String defaultIme = Settings.Secure.getString(mContext
    //         .getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD);
    String defaultIme(DEFAULT_INPUT_METHOD);
    if (enabledStr.IsNull() || defaultIme.IsNullOrEmpty()) {
        // Slog.i(TAG, "Enabled input methods or default IME has not been set, enabling all");
        AutoPtr<IInputMethodInfo> defIm;
        StringBuffer sb(256);
        List< AutoPtr<IInputMethodInfo> >::Iterator it;
        for (it = mMethodList.Begin(); it != mMethodList.End(); ++it) {
            IInputMethodInfo* imi = *it;
            // Slog.i(TAG, "Adding: " + imi.getId());
            if (it != mMethodList.Begin()) sb += ':';
            assert(imi != NULL);
            String id;
            imi->GetId(&id);
            sb += id;
            Int32 defaultResId = 0;
            if (defIm == NULL &&
                    (imi->GetIsDefaultResourceId(&defaultResId), defaultResId != 0)) {
                String capName;
                imi->GetCapsuleName(&capName);
                AutoPtr<IContext> ctx;
                mContext->CreateCapsuleContext(capName, 0, (IContext**)&ctx);
                assert(ctx != NULL);
                AutoPtr<IResources> res;
                ctx->GetResources((IResources**) &res);
                Boolean state = FALSE;
                if (res->GetBoolean(defaultResId, &state), state) {
                    defIm = imi;
                    // Slog.i(TAG, "Selected default: " + imi.getId());
                }
                // } catch (PackageManager.NameNotFoundException ex) {
                // } catch (Resources.NotFoundException ex) {
                // }
            }
        }
        if (defIm == NULL && mMethodList.Begin() != mMethodList.End()) {
            defIm = *mMethodList.Begin();
            // Slog.i(TAG, "No default found, using " + defIm.getId());
        }
        // Settings.Secure.putString(mContext.getContentResolver(),
        //         Settings.Secure.ENABLED_INPUT_METHODS, sb.toString());
        // if (defIm != NULL) {
        //     Settings.Secure.putString(mContext.getContentResolver(),
        //             Settings.Secure.DEFAULT_INPUT_METHOD, defIm.getId());
        // }
    }

    //mSettingsObserver = new SettingsObserver(mHandler);
    UpdateFromSettingsLocked();

    return NOERROR;
}

ECode CInputMethodManagerService::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputMethodManagerService::SystemReady()
{
    Mutex::Autolock lock(mMethodMapLock);
    if (!mSystemReady) {
        mSystemReady = TRUE;
        //try {
        StartInputInnerLocked();
        // } catch (RuntimeException e) {
        //     Slog.w(TAG, "Unexpected exception", e);
        // }
    }
    return NOERROR;
}

ECode CInputMethodManagerService::GetInputMethodList(
    /* [out] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    FAIL_RETURN(CParcelableObjectContainer::New(infos));

    Mutex::Autolock lock(mMethodMapLock);

    List< AutoPtr<IInputMethodInfo> >::Iterator it;
    for (it = mMethodList.Begin(); it != mMethodList.End(); ++it) {
        IInputMethodInfo* item = *it;
        (*infos)->Add(item);
    }

    return NOERROR;
}

ECode CInputMethodManagerService::GetEnabledInputMethodList(
    /* [out] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    Mutex::Autolock lock(mMethodMapLock);
    return GetEnabledInputMethodListLocked(infos);
}

ECode CInputMethodManagerService::GetEnabledInputMethodListLocked(
    /* [out] */ IObjectContainer** list)
{
    assert(list != NULL);
    FAIL_RETURN(CParcelableObjectContainer::New(list));

    // final String enabledStr = Settings.Secure.getString(
    //         mContext.getContentResolver(),
    //         Settings.Secure.ENABLED_INPUT_METHODS);
    String enabledStr(ENABLED_STR);
    if (!enabledStr.IsNull()) {
        mStringColonSplitter->SetString(enabledStr);

        while (mStringColonSplitter->HasNext()) {
            String str;
            mStringColonSplitter->Next(&str);
            HashMap<String, AutoPtr<IInputMethodInfo> >::Iterator it =
                    mMethodMap.Find(str);
            if (it != mMethodMap.End()) {
                (*list)->Add(it->mSecond.Get());
            }
        }
    }

    return NOERROR;
}

ECode CInputMethodManagerService::AddClient(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IInputContext* inputContext,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid)
{
    Mutex::Autolock lock(mMethodMapLock);
    mClients[IBinder::Probe(client)] = new ClientState(client, inputContext, uid, pid);

    return NOERROR;
}

ECode CInputMethodManagerService::RemoveClient(
    /* [in] */ IInputMethodClient* client)
{
    Mutex::Autolock lock(mMethodMapLock);
    mClients.Erase(IBinder::Probe(client));

    return NOERROR;
}

void CInputMethodManagerService::UnbindCurrentClientLocked()
{
    if (mCurClient != NULL) {
        // if (DEBUG) Slog.v(TAG, "unbindCurrentInputLocked: client = "
        //         + mCurClient.client.asBinder());
        if (mBoundToMethod) {
            mBoundToMethod = FALSE;
            if (mCurMethod != NULL) {
                // executeOrSendMessage(mCurMethod, mCaller.obtainMessageO(
                //         MSG_UNBIND_INPUT, mCurMethod));
                if (IBinder::Probe(mCurMethod) != NULL) {
                    void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethod*);

                    pHandlerFunc = &CInputMethodManagerService::HandleUnbindInput;
                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    params->WriteInterfacePtr(mCurMethod.Get());

                    mCaller->PostCppCallbackDelayed(
                        (Handle32)this, *(Handle32*)&pHandlerFunc,
                        params, 0, 0);
                }
                else {
                    HandleUnbindInput(mCurMethod);
                }
            }
        }
        // executeOrSendMessage(mCurClient.client, mCaller.obtainMessageIO(
        //         MSG_UNBIND_METHOD, mCurSeq, mCurClient.client));
        if (IBinder::Probe(mCurClient->mClient) != NULL) {
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodClient*,Int32);

            pHandlerFunc = &CInputMethodManagerService::HandleUnbindMethod;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(mCurClient->mClient.Get());
            params->WriteInt32(mCurSeq);

            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            HandleUnbindMethod(mCurClient->mClient, mCurSeq);
        }
        mCurClient->mSessionRequested = FALSE;

        // Call setActive(FALSE) on the old client
        // try{
        mCurClient->mClient->SetActive(FALSE);
        // } catch (RemoteException e) {
        //     Slog.w(TAG, "Got RemoteException sending setActive(FALSE) notification to pid "
        //             + mCurClient.pid + " uid " + mCurClient.uid);
        // }
        mCurClient = NULL;

        HideInputMethodMenuLocked();
    }
}

Int32 CInputMethodManagerService::GetImeShowFlags()
{
    Int32 flags = 0;
    if (mShowForced) {
        flags |= InputMethod_SHOW_FORCED
                | InputMethod_SHOW_EXPLICIT;
    }
    else if (mShowExplicitlyRequested) {
        flags |= InputMethod_SHOW_EXPLICIT;
    }

    return flags;
}

Int32 CInputMethodManagerService::GetAppShowFlags()
{
    Int32 flags = 0;
    if (mShowForced) {
        flags |= InputMethodManager_SHOW_FORCED;
    }
    else if (!mShowExplicitlyRequested) {
        flags |= InputMethodManager_SHOW_IMPLICIT;
    }

    return flags;
}

AutoPtr<IInputBindResult> CInputMethodManagerService::AttachNewInputLocked(
    /* [in] */ Boolean initial,
    /* [in] */ Boolean needResult)
{
    if (!mBoundToMethod) {
        // executeOrSendMessage(mCurMethod, mCaller.obtainMessageOO(
        //         MSG_BIND_INPUT, mCurMethod, mCurClient.binding));
        if (IBinder::Probe(mCurMethod) != NULL) {
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethod*,IInputBinding*);

            pHandlerFunc = &CInputMethodManagerService::HandleBindInput;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(mCurMethod.Get());
            params->WriteInterfacePtr(mCurClient->mBinding.Get());

            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            HandleBindInput(mCurMethod, mCurClient->mBinding);
        }
        mBoundToMethod = TRUE;
    }

    if (initial) {
        // executeOrSendMessage(session.method, mCaller.obtainMessageOOO(
        //         MSG_START_INPUT, session, mCurInputContext, mCurAttribute));
        if (IBinder::Probe(mCurClient->mCurSession->mMethod) != NULL) {
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(SessionState*,
                    IInputContext*,IEditorInfo*);

            pHandlerFunc = &CInputMethodManagerService::HandleStartInput;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32((Handle32)mCurClient->mCurSession.Get());
            params->WriteInterfacePtr(mCurInputContext.Get());
            params->WriteInterfacePtr(mCurAttribute.Get());
            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            HandleStartInput(mCurClient->mCurSession, mCurInputContext, mCurAttribute);
        }
    }
    else {
        // executeOrSendMessage(session.method, mCaller.obtainMessageOOO(
        //         MSG_RESTART_INPUT, session, mCurInputContext, mCurAttribute));
        if (IBinder::Probe(mCurClient->mCurSession->mMethod) != NULL) {
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(SessionState*,
                    IInputContext*,IEditorInfo*);

            pHandlerFunc = &CInputMethodManagerService::HandleRestartInput;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32((Handle32)mCurClient->mCurSession.Get());
            params->WriteInterfacePtr(mCurInputContext.Get());
            params->WriteInterfacePtr(mCurAttribute.Get());

            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            HandleRestartInput(mCurClient->mCurSession, mCurInputContext, mCurAttribute);
        }
    }

    if (mShowRequested) {
        // if (DEBUG) Slog.v(TAG, "Attach new input asks to show input");
        ShowCurrentInputLocked(GetAppShowFlags(), NULL);
    }

    if (needResult) {
        AutoPtr<IInputBindResult> retObj;
        CInputBindResult::New(mCurClient->mCurSession->mSession, mCurId, mCurSeq,
                    (IInputBindResult**)&retObj);
        return retObj;
    }
    return NULL;
}

AutoPtr<IInputBindResult> CInputMethodManagerService::StartInputLocked(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean initial,
    /* [in] */ Boolean needResult)
{
    // If no method is currently selected, do nothing.
    if (mCurMethodId.IsNull()) {
        return mNoBinding;
    }

    HashMap<AutoPtr<IBinder>, AutoPtr<ClientState> >::Iterator it = mClients.Find(IBinder::Probe(client));
    if (it == mClients.End()) {
        assert(0);
        // throw new IllegalArgumentException("unknown client "
        //         + client.asBinder());
    }
    AutoPtr<ClientState> cs = it->mSecond;

    // try{
    Boolean ret = FALSE;
    if (mIWindowManager->InputMethodClientHasFocus(cs->mClient, &ret), !ret) {
        // Check with the window manager to make sure this client actually
        // has a window with focus.  If not, reject.  This is thread safe
        // because if the focus changes some time before or after, the
        // next client receiving focus that has any interest in input will
        // be calling through here after that change happens.
        // Slog.w(TAG, "Starting input on non-focused client " + cs.client
        //         + " (uid=" + cs.uid + " pid=" + cs.pid + ")");
        return NULL;
    }
    // } catch(RemoteException e) {
    // }

    if (mCurClient != cs) {
        // If the client is changing, we need to switch over to the new
        // one.
        UnbindCurrentClientLocked();
        // if (DEBUG) Slog.v(TAG, "switching to client: client = "
        //         + cs.client.asBinder());

        // If the screen is on, inform the new client it is active
        if (mScreenOn) {
            // try {
            cs->mClient->SetActive(mScreenOn);
            // } catch (RemoteException e) {
            //     Slog.w(TAG, "Got RemoteException sending setActive notification to pid "
            //             + cs.pid + " uid " + cs.uid);
            // }
        }
    }

    // Bump up the sequence for this client and attach it.
    mCurSeq++;
    if (mCurSeq <= 0) mCurSeq = 1;
    mCurClient = cs;
    mCurInputContext = inputContext;
    mCurAttribute = attribute;

    // Check if the input method is changing.
    if (!mCurId.IsNull() && mCurId.Equals(mCurMethodId)) {
        if (cs->mCurSession != NULL) {
            // Fast case: if we are already connected to the input method,
            // then just return it.
            return AttachNewInputLocked(initial, needResult);
        }
        if (mHaveConnection) {
            if (mCurMethod != NULL) {
                if (!cs->mSessionRequested) {
                    cs->mSessionRequested = TRUE;
                    // if (DEBUG) Slog.v(TAG, "Creating new session for client " + cs);
                    // executeOrSendMessage(mCurMethod, mCaller.obtainMessageOO(
                    //         MSG_CREATE_SESSION, mCurMethod,
                    //         new MethodCallback(mCurMethod)));
                    if (IBinder::Probe(mCurMethod) != NULL) {
                        void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethod*, IInputMethodCallback*);
                        pHandlerFunc = &CInputMethodManagerService::HandleCreateSession;
                        AutoPtr<IParcel> params;
                        CCallbackParcel::New((IParcel**)&params);
                        params->WriteInterfacePtr(mCurMethod.Get());
                        AutoPtr<IInputMethodCallback> methodCallback;
                        CInputMethodManagerServiceMethodCallback::New(
                                mCurMethod, (Handle32)this, (IInputMethodCallback**)&methodCallback);
                        params->WriteInterfacePtr(methodCallback.Get());
                        mCaller->PostCppCallbackDelayed(
                            (Handle32)this, *(Handle32*)&pHandlerFunc,
                            params, 0, 0);
                    }
                    else {
                        AutoPtr<IInputMethodCallback> methodCallback;
                        CInputMethodManagerServiceMethodCallback::New(
                                mCurMethod, (Handle32)this, (IInputMethodCallback**)&methodCallback);
                        HandleCreateSession(mCurMethod, methodCallback);
                    }
                }
                // Return to client, and we will get back with it when
                // we have had a session made for it.
                AutoPtr<IInputBindResult> retObj;
                CInputBindResult::New(NULL, mCurId, mCurSeq, (IInputBindResult**)&retObj);
                return retObj;
            }
            else if (SystemClock::GetUptimeMillis()
                    < (mLastBindTime + TIME_TO_RECONNECT)) {
                // In this case we have connected to the service, but
                // don't yet have its interface.  If it hasn't been too
                // long since we did the connection, we'll return to
                // the client and wait to get the service interface so
                // we can report back.  If it has been too long, we want
                // to fall through so we can try a disconnect/reconnect
                // to see if we can get back in touch with the service.
                AutoPtr<IInputBindResult> retObj;
                CInputBindResult::New(NULL, mCurId, mCurSeq, (IInputBindResult**) &retObj);
                return retObj;
            }
            else {
                // EventLog.writeEvent(EventLogTags.IMF_FORCE_RECONNECT_IME,
                //         mCurMethodId, SystemClock::UptimeMillis()-mLastBindTime, 0);
            }
        }
    }

    return StartInputInnerLocked();
}

AutoPtr<IInputBindResult> CInputMethodManagerService::StartInputInnerLocked()
{
    if (mCurMethodId.IsNull()) {
        return mNoBinding;
    }

    if (!mSystemReady) {
        // If the system is not yet ready, we shouldn't be running third
        // party code.
        AutoPtr<IInputBindResult> retObj;
        CInputBindResult::New(NULL, mCurMethodId, mCurSeq, (IInputBindResult**)&retObj);
        return retObj;
    }

    HashMap<String, AutoPtr<IInputMethodInfo> >::Iterator it = mMethodMap.Find(mCurMethodId);
    if (it == mMethodMap.End()) {
        assert(0);
        //throw new IllegalArgumentException("Unknown id: " + mCurMethodId);
    }
    IInputMethodInfo* info = it->mSecond;

    UnbindCurrentMethodLocked(FALSE);

    CIntent::New(String(InputMethod_SERVICE_INTERFACE), (IIntent**)&mCurIntent);
    AutoPtr<IComponentName> tmpName;
    info->GetComponent((IComponentName**)&tmpName);
    mCurIntent->SetComponent(tmpName);
    mCurIntent->PutInt32Extra(String(Intent_EXTRA_CLIENT_LABEL),
            0x0104036d /*com.android.internal.R.string.input_method_binding_label*/);
    // mCurIntent.putExtra(Intent_EXTRA_CLIENT_INTENT, PendingIntent->GetActivity(
    //         mContext, 0, new Intent(Settings.ACTION_INPUT_METHOD_SETTINGS), 0));
    Boolean succeeded = FALSE;
    mContext->BindService(mCurIntent, (IServiceConnection*)this->Probe(EIID_IServiceConnection),
            Context_BIND_AUTO_CREATE, &succeeded);
    if (succeeded) {
        mLastBindTime = SystemClock::GetUptimeMillis();
        mHaveConnection = TRUE;
        info->GetId(&mCurId);
        CBinder::New((IBinder**) &mCurToken);
        // try {
        // if (DEBUG) Slog.v(TAG, "Adding window token: " + mCurToken);
        mIWindowManager->AddWindowToken(mCurToken,
                WindowManagerLayoutParams_TYPE_INPUT_METHOD);
        // } catch (RemoteException e) {
        // }
        AutoPtr<IInputBindResult> retObj;
        CInputBindResult::New(NULL, mCurId, mCurSeq, (IInputBindResult**)&retObj);
        return retObj;
    }
    else {
        mCurIntent = NULL;
        // Slog.w(TAG, "Failure connecting to input method service: "
        //         + mCurIntent);
    }
    return NULL;
}

ECode CInputMethodManagerService::StartInput(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean initial,
    /* [in] */ Boolean needResult,
    /* [out] */ IInputBindResult** res)
{
    VALIDATE_NOT_NULL(res);

    Mutex::Autolock lock(mMethodMapLock);

    Int64 ident = Binder::ClearCallingIdentity();
    AutoPtr<IInputBindResult> resObj = StartInputLocked(client, inputContext, attribute,
            initial, needResult);
    *res = resObj.Get();
    if (*res != NULL) (*res)->AddRef();
    Binder::RestoreCallingIdentity(ident);
    return NOERROR;
}

ECode CInputMethodManagerService::FinishInput(
    /* [in] */ IInputMethodClient* client)
{
    return NOERROR;
}

ECode CInputMethodManagerService::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    assert(name != NULL && service != NULL);
    Mutex::Autolock lock(mMethodMapLock);

    if (mCurIntent != NULL) {
        AutoPtr<IComponentName> tmpName;
        mCurIntent->GetComponent((IComponentName**)&tmpName);
        Boolean equal = FALSE;
        if (name->Equals(tmpName, &equal), equal) {
            mCurMethod = IInputMethod::Probe(service);
            if (mCurToken == NULL) {
                // Slog.w(TAG, "Service connected without a token!");
                UnbindCurrentMethodLocked(FALSE);
                return NOERROR;
            }
            // if (DEBUG) Slog.v(TAG, "Initiating attach with token: " + mCurToken);
            // executeOrSendMessage(mCurMethod, mCaller.obtainMessageOO(
            //         MSG_ATTACH_TOKEN, mCurMethod, mCurToken));
            if (mCurMethod != NULL) {
                void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethod*, IBinder*);
                pHandlerFunc = &CInputMethodManagerService::HandleAttachToken;
                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);
                params->WriteInterfacePtr(mCurMethod.Get());
                params->WriteInterfacePtr(mCurToken.Get());
                mCaller->PostCppCallbackDelayed(
                    (Handle32)this, *(Handle32*)&pHandlerFunc,
                    params, 0, 0);
            }
            else {
                HandleAttachToken(mCurMethod, mCurToken);
            }

            if (mCurClient != NULL) {
                // if (DEBUG) Slog.v(TAG, "Creating first session while with client "
                //         + mCurClient);
                // executeOrSendMessage(mCurMethod, mCaller.obtainMessageOO(
                //         MSG_CREATE_SESSION, mCurMethod,
                //         new MethodCallback(mCurMethod)));
                if (mCurMethod != NULL) {
                    void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethod*, IInputMethodCallback*);
                    pHandlerFunc = &CInputMethodManagerService::HandleCreateSession;
                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    params->WriteInterfacePtr(mCurMethod.Get());
                    AutoPtr<IInputMethodCallback> methodCallback;
                    CInputMethodManagerServiceMethodCallback::New(
                            mCurMethod, (Handle32)this, (IInputMethodCallback**)&methodCallback);
                    params->WriteInterfacePtr(methodCallback);
                    mCaller->PostCppCallbackDelayed(
                        (Handle32)this, *(Handle32*)&pHandlerFunc,
                        params, 0, 0);
                }
                else {
                    AutoPtr<IInputMethodCallback> methodCallback;
                    CInputMethodManagerServiceMethodCallback::New(
                            mCurMethod, (Handle32)this, (IInputMethodCallback**)&methodCallback);
                    HandleCreateSession(mCurMethod, methodCallback);
                }
            }
        }
    }

    return NOERROR;
}

void CInputMethodManagerService::OnSessionCreated(
    /* [in] */ IInputMethod* method,
    /* [in] */ IInputMethodSession* session)
{
    Mutex::Autolock lock(mMethodMapLock);

    if (mCurMethod != NULL && method != NULL
            && IBinder::Probe(mCurMethod) == IBinder::Probe(method)) {
        if (mCurClient != NULL) {
            mCurClient->mCurSession = new SessionState(mCurClient,
                    method, session);
            mCurClient->mSessionRequested = FALSE;
            AutoPtr<IInputBindResult> res = AttachNewInputLocked(TRUE, TRUE);
            AutoPtr<IInputMethodSession> method;
            res->GetIIMSession((IInputMethodSession**)&method);
            if (method != NULL) {
                // executeOrSendMessage(mCurClient.client, mCaller.obtainMessageOO(
                //         MSG_BIND_METHOD, mCurClient.client, res));
                if (IBinder::Probe(mCurClient->mClient) != NULL) {
                    void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodClient*,
                            IInputBindResult*);

                    pHandlerFunc = &CInputMethodManagerService::HandleBindMethod;
                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    params->WriteInterfacePtr(mCurClient->mClient);
                    params->WriteInterfacePtr(res);
                    mCaller->PostCppCallbackDelayed(
                        (Handle32)this, *(Handle32*)&pHandlerFunc,
                        params, 0, 0);
                }
                else {
                    HandleBindMethod(mCurClient->mClient, res);
                }
            }
        }
    }
}

void CInputMethodManagerService::UnbindCurrentMethodLocked(
    /* [in] */ Boolean reportToClient)
{
    if (mHaveConnection) {
        mContext->UnbindService((IServiceConnection*)this->Probe(EIID_IServiceConnection));
        mHaveConnection = FALSE;
    }

    if (mCurToken != NULL) {
        // try {
        // if (DEBUG) Slog.v(TAG, "Removing window token: " + mCurToken);
        mIWindowManager->RemoveWindowToken(mCurToken);
        // } catch (RemoteException e) {
        // }
        mCurToken = NULL;
    }

    mCurId = NULL;
    ClearCurMethodLocked();

    if (reportToClient && mCurClient != NULL) {
        // executeOrSendMessage(mCurClient.client, mCaller.obtainMessageIO(
        //         MSG_UNBIND_METHOD, mCurSeq, mCurClient.client));
        if (IBinder::Probe(mCurClient->mClient) != NULL) {
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodClient*,
                    Int32);

            pHandlerFunc = &CInputMethodManagerService::HandleUnbindMethod;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(mCurClient->mClient);
            params->WriteInt32(mCurSeq);
            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            HandleUnbindMethod(mCurClient->mClient, mCurSeq);
        }
    }
}

void CInputMethodManagerService::FinishSession(
    /* [in] */ SessionState* sessionState)
{
    if (sessionState != NULL && sessionState->mSession != NULL) {
        // try {
        sessionState->mSession->FinishSession();
        // } catch (RemoteException e) {
        //     Slog.w(TAG, "Session failed to close due to remote exception", e);
        // }
    }
}

void CInputMethodManagerService::ClearCurMethodLocked()
{
    if (mCurMethod != NULL) {
        HashMap<AutoPtr<IBinder>, AutoPtr<ClientState> >::Iterator it;
        for (it = mClients.Begin(); it != mClients.End(); ++it) {
            ClientState* cs = it->mSecond;
            cs->mSessionRequested = FALSE;
            FinishSession(cs->mCurSession);
            cs->mCurSession = NULL;
        }

        FinishSession(mEnabledSession);
        mEnabledSession = NULL;
        mCurMethod = NULL;
    }
    //mStatusBar->SetIconVisibility(String("ime"), FALSE);
}

ECode CInputMethodManagerService::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    Mutex::Autolock lock(mMethodMapLock);

    // if (DEBUG) Slog.v(TAG, "Service disconnected: " + name
    //         + " mCurIntent=" + mCurIntent);
    if (mCurMethod != NULL && mCurIntent != NULL) {
        AutoPtr<IComponentName> tmpName;
        mCurIntent->GetComponent((IComponentName**)&tmpName);
        Boolean equal = FALSE;
        if (name->Equals(tmpName, &equal), equal) {
            ClearCurMethodLocked();
            // We consider this to be a new bind attempt, since the system
            // should now try to restart the service for us.
            mLastBindTime = SystemClock::GetUptimeMillis();
            mShowRequested = mInputShown;
            mInputShown = FALSE;
            if (mCurClient != NULL) {
                // executeOrSendMessage(mCurClient.client, mCaller.obtainMessageIO(
                //         MSG_UNBIND_METHOD, mCurSeq, mCurClient.client));
                if (IBinder::Probe(mCurClient->mClient) != NULL) {
                    void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodClient*, Int32);
                    pHandlerFunc = &CInputMethodManagerService::HandleUnbindMethod;
                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    params->WriteInterfacePtr(mCurClient->mClient.Get());
                    params->WriteInt32(mCurSeq);
                    mCaller->PostCppCallbackDelayed(
                        (Handle32)this, *(Handle32*)&pHandlerFunc,
                        params, 0, 0);
                }
                else {
                    HandleUnbindMethod(mCurClient->mClient, mCurSeq);
                }
            }
        }
    }

    return NOERROR;
}

ECode CInputMethodManagerService::UpdateStatusIcon(
    /* [in] */ IBinder* token,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 iconId)
{
    //Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();

    if (token == NULL || mCurToken.Get() != token) {
        // Slog.w(TAG, "Ignoring setInputMethod of uid " + uid + " token: " + token);
        return NOERROR;
    }

    /*{
        Mutex::Autolock lock(mMethodMapLock);
        assert(mStatusBar != NULL);
        if (iconId == 0) {
            // if (DEBUG) Slog.d(TAG, "hide the small icon for the input method");
            mStatusBar->SetIconVisibility(String("ime"), FALSE);
        } else if (packageName != NULL) {
            // if (DEBUG) Slog.d(TAG, "show a small icon for the input method");
            mStatusBar->SetIcon(String("ime"), packageName, iconId, 0);
            mStatusBar->SetIconVisibility(String("ime"), TRUE);
        }
    }*/

    Binder::RestoreCallingIdentity(ident);
    return NOERROR;
}

void CInputMethodManagerService::UpdateFromSettingsLocked()
{
    // We are assuming that whoever is changing DEFAULT_INPUT_METHOD and
    // ENABLED_INPUT_METHODS is taking care of keeping them correctly in
    // sync, so we will never have a DEFAULT_INPUT_METHOD that is not
    // enabled.
    // String id = Settings.Secure.getString(mContext.getContentResolver(),
    //     Settings.Secure.DEFAULT_INPUT_METHOD);
    String id(DEFAULT_INPUT_METHOD);
    if (!id.IsNull() && id.GetCharCount() > 0) {
        // try {
        ECode ec = SetInputMethodLocked(id);
        if (ec == E_ILLEGAL_ARGUMENT_EXCEPTION) {
            mCurMethodId = NULL;
            UnbindCurrentMethodLocked(TRUE);
        }
        // } catch (IllegalArgumentException e) {
        //     Slog.w(TAG, "Unknown input method from prefs: " + id, e);
        //     mCurMethodId = NULL;
        //     UnbindCurrentMethodLocked(TRUE);
        // }
    }
    else {
        // There is no longer an input method set, so stop any current one.
        mCurMethodId = NULL;
        UnbindCurrentMethodLocked(TRUE);
    }
}

ECode CInputMethodManagerService::SetInputMethodLocked(
    /* [in] */ const String& id)
{
    HashMap<String, AutoPtr<IInputMethodInfo> >::Iterator it =
            mMethodMap.Find(id);
    if (it == mMethodMap.End()) {
        //throw new IllegalArgumentException("Unknown id: " + id);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (id.Equals(mCurMethodId)) {
        return NOERROR;
    }

    Int64 ident = Binder::ClearCallingIdentity();
    mCurMethodId = id;
    // Settings.Secure.putString(mContext.getContentResolver(),
    //     Settings.Secure.DEFAULT_INPUT_METHOD, id);

    if (/*ActivityManagerNative.IsSystemReady()*/ TRUE) {
        AutoPtr<IIntent> intent;
        FAIL_RETURN(CIntent::New(String(Intent_ACTION_INPUT_METHOD_CHANGED), (IIntent**)&intent));
        intent->AddFlags(Intent_FLAG_RECEIVER_REPLACE_PENDING);
        intent->PutStringExtra(String("input_method_id"), id);
        mContext->SendBroadcast(intent);
    }
    UnbindCurrentClientLocked();
    Binder::RestoreCallingIdentity(ident);
    return NOERROR;
}

ECode CInputMethodManagerService::ShowSoftInput(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ Int32 flags,
    /* [in] */ ILocalResultReceiver* resultReceiver,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);

    //Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();
    {
        Mutex::Autolock lock(mMethodMapLock);

        if (mCurClient == NULL || client == NULL
                || IBinder::Probe(mCurClient->mClient) != IBinder::Probe(client)) {
            // try {
            // We need to check if this is the current client with
            // focus in the window manager, to allow this call to
            // be made before input is started in it.
            Boolean ret = FALSE;
            if (mIWindowManager->InputMethodClientHasFocus(client, &ret), !ret) {
                // Slog.w(TAG, "Ignoring showSoftInput of uid " + uid + ": " + client);
                *state = FALSE;
                Binder::RestoreCallingIdentity(ident);
                return NOERROR;
            }
            // } catch (RemoteException e) {
            //     return FALSE;
            // }
        }

        // if (DEBUG) Slog.v(TAG, "Client requesting input be shown");
        *state = ShowCurrentInputLocked(flags, resultReceiver);
    }

    Binder::RestoreCallingIdentity(ident);
    return NOERROR;
}

Boolean CInputMethodManagerService::ShowCurrentInputLocked(
    /* [in] */ Int32 flags,
    /* [in] */ ILocalResultReceiver* resultReceiver)
{
    mShowRequested = TRUE;
    if ((flags & InputMethodManager_SHOW_IMPLICIT) == 0) {
        mShowExplicitlyRequested = TRUE;
    }

    if ((flags & InputMethodManager_SHOW_FORCED) != 0) {
        mShowExplicitlyRequested = TRUE;
        mShowForced = TRUE;
    }

    if (!mSystemReady) {
        return FALSE;
    }

    Boolean res = FALSE;
    if (mCurMethod != NULL) {
        // executeOrSendMessage(mCurMethod, mCaller.obtainMessageIOO(
        //         MSG_SHOW_SOFT_INPUT, GetImeShowFlags(), mCurMethod,
        //         resultReceiver));
//         if (IBinder::Probe(mCurMethod) != NULL) {
//             void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethod*,Int32,ILocalResultReceiver*);
//             pHandlerFunc = &CInputMethodManagerService::HandleShowSoftInput;
//             AutoPtr<IParcel> params;
//             CCallbackParcel::New((IParcel**)&params);
//             params->WriteInt32(GetImeShowFlags());
//             params->WriteInterfacePtr(mCurMethod.Get());
//             params->WriteInterfacePtr(resultReceiver);

//             mCaller->PostCppCallbackDelayed(
//                 (Handle32)this, *(Handle32*)&pHandlerFunc,
//                 params, 0, 0);
//         }
//         else {
            HandleShowSoftInput(mCurMethod, GetImeShowFlags(), resultReceiver);
        // }
        mInputShown = TRUE;
        res = TRUE;
    }
    else if (mHaveConnection && SystemClock::GetUptimeMillis() < (mLastBindTime + TIME_TO_RECONNECT)) {
        // The client has asked to have the input method shown, but
        // we have been sitting here too long with a connection to the
        // service and no interface received, so let's disconnect/connect
        // to try to prod things along.
        // EventLog.writeEvent(EventLogTags.IMF_FORCE_RECONNECT_IME, mCurMethodId,
        //         SystemClock::UptimeMillis()-mLastBindTime,1);
        mContext->UnbindService((IServiceConnection*)this->Probe(EIID_IServiceConnection));
        mContext->BindService(mCurIntent, (IServiceConnection*)this->Probe(EIID_IServiceConnection),
            Context_BIND_AUTO_CREATE, &res);
    }

    return res;
}

ECode CInputMethodManagerService::HideSoftInput(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ Int32 flags,
    /* [in] */ ILocalResultReceiver* resultReceiver,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);

    //Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();

    {
        Mutex::Autolock lock(mMethodMapLock);
        if (mCurClient == NULL || client == NULL
                || IBinder::Probe(mCurClient->mClient) != IBinder::Probe(client))
        {
            // We need to check if this is the current client with
            // focus in the window manager, to allow this call to
            // be made before input is started in it.
            Boolean ret = FALSE;
            mIWindowManager->InputMethodClientHasFocus(client, &ret);
            if (!ret) {
                // if (DEBUG) Slog.w(TAG, "Ignoring hideSoftInput of uid "
                //         + uid + ": " + client);
                *state = FALSE;
                Binder::RestoreCallingIdentity(ident);
                return NOERROR;
            }
            // } catch (RemoteException e) {
            //     return FALSE;
            // }
        }

        // if (DEBUG) Slog.v(TAG, "Client requesting input be hidden");
        *state = HideCurrentInputLocked(flags, resultReceiver);
    }

    Binder::RestoreCallingIdentity(ident);
    return NOERROR;
}

Boolean CInputMethodManagerService::HideCurrentInputLocked(
    /* [in] */ Int32 flags,
    /* [in] */ ILocalResultReceiver* resultReceiver)
{
    if ((flags & InputMethodManager_HIDE_IMPLICIT_ONLY) != 0
            && (mShowExplicitlyRequested || mShowForced)) {
        // if (DEBUG) Slog.v(TAG,
        //         "Not hiding: explicit show not cancelled by non-explicit hide");
        return FALSE;
    }
    if (mShowForced && (flags & InputMethodManager_HIDE_NOT_ALWAYS) != 0) {
        // if (DEBUG) Slog.v(TAG,
        //         "Not hiding: forced show not cancelled by not-always hide");
        return FALSE;
    }
    Boolean res = FALSE;
    if (mInputShown && mCurMethod != NULL) {
        // executeOrSendMessage(mCurMethod, mCaller.obtainMessageOO(
        //         MSG_HIDE_SOFT_INPUT, mCurMethod, resultReceiver));
        if (IBinder::Probe(mCurMethod) != NULL) {
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethod*, ILocalResultReceiver*);
            pHandlerFunc = &CInputMethodManagerService::HandleHideSoftInput;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(mCurMethod.Get());
            params->WriteInterfacePtr(resultReceiver);
            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            HandleHideSoftInput(mCurMethod, resultReceiver);
        }
        res = TRUE;
    }
    else {
        res = FALSE;
    }

    mInputShown = FALSE;
    mShowRequested = FALSE;
    mShowExplicitlyRequested = FALSE;
    mShowForced = FALSE;
    return res;
}

ECode CInputMethodManagerService::WindowGainedFocus(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IBinder* windowToken,
    /* [in] */ Boolean viewHasFocus,
    /* [in] */ Boolean isTextEditor,
    /* [in] */ Int32 softInputMode,
    /* [in] */ Boolean first,
    /* [in] */ Int32 windowFlags)
{
    Int64 ident = Binder::ClearCallingIdentity();
    {
        Mutex::Autolock lock(mMethodMapLock);
        // if (DEBUG) Slog.v(TAG, "windowGainedFocus: " + client.asBinder()
        //         + " viewHasFocus=" + viewHasFocus
        //         + " isTextEditor=" + isTextEditor
        //         + " softInputMode=#" + Integer.toHexString(softInputMode)
        //         + " first=" + first + " flags=#"
        //         + Integer.toHexString(windowFlags));

        if (mCurClient == NULL || client == NULL
                || IBinder::Probe(mCurClient->mClient) != IBinder::Probe(client)) {
            // We need to check if this is the current client with
            // focus in the window manager, to allow this call to
            // be made before input is started in it.
            Boolean ret = FALSE;
            if (mIWindowManager->InputMethodClientHasFocus(client, &ret), !ret) {
                // Slog.w(TAG, "Client not active, ignoring focus gain of: " + client);
                Binder::RestoreCallingIdentity(ident);
                return NOERROR;
            }
        }

        if (mCurFocusedWindow.Get() == windowToken) {
            // Slog.w(TAG, "Window already focused, ignoring focus gain of: " + client);
            Binder::RestoreCallingIdentity(ident);
            return NOERROR;
        }
        mCurFocusedWindow = windowToken;

        switch (softInputMode & WindowManagerLayoutParams_SOFT_INPUT_MASK_STATE) {
            case WindowManagerLayoutParams_SOFT_INPUT_STATE_UNSPECIFIED:
                if (!isTextEditor || (softInputMode &
                        WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST)
                        != WindowManagerLayoutParams_SOFT_INPUT_ADJUST_RESIZE) {
                    AutoPtr<IWindowManagerLayoutParamsHelper> wmlpHelper;
                    ASSERT_SUCCEEDED(CWindowManagerLayoutParamsHelper::AcquireSingleton(
                            (IWindowManagerLayoutParamsHelper**)&wmlpHelper));
                    Boolean mayUse = FALSE;
                    if (wmlpHelper->MayUseInputMethodHelper(windowFlags, &mayUse), mayUse) {
                        // There is no focus view, and this window will
                        // be behind any soft input window, so hide the
                        // soft input window if it is shown.
                        // if (DEBUG) Slog.v(TAG, "Unspecified window will hide input");
                        HideCurrentInputLocked(InputMethodManager_HIDE_NOT_ALWAYS, NULL);
                    }
                }
                else if (isTextEditor && (softInputMode &
                        WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST)
                        == WindowManagerLayoutParams_SOFT_INPUT_ADJUST_RESIZE
                        && (softInputMode &
                                WindowManagerLayoutParams_SOFT_INPUT_IS_FORWARD_NAVIGATION) != 0) {
                    // There is a focus view, and we are navigating forward
                    // into the window, so show the input window for the user.
                    // if (DEBUG) Slog.v(TAG, "Unspecified window will show input");
                    ShowCurrentInputLocked(InputMethodManager_SHOW_IMPLICIT, NULL);
                }
                break;
            case WindowManagerLayoutParams_SOFT_INPUT_STATE_UNCHANGED:
                // Do nothing.
                break;
            case WindowManagerLayoutParams_SOFT_INPUT_STATE_HIDDEN:
                if ((softInputMode &
                        WindowManagerLayoutParams_SOFT_INPUT_IS_FORWARD_NAVIGATION) != 0) {
                    // if (DEBUG) Slog.v(TAG, "Window asks to hide input going forward");
                    HideCurrentInputLocked(0, NULL);
                }
                break;
            case WindowManagerLayoutParams_SOFT_INPUT_STATE_ALWAYS_HIDDEN:
                // if (DEBUG) Slog.v(TAG, "Window asks to hide input");
                HideCurrentInputLocked(0, NULL);
                break;
            case WindowManagerLayoutParams_SOFT_INPUT_STATE_VISIBLE:
                if ((softInputMode &
                        WindowManagerLayoutParams_SOFT_INPUT_IS_FORWARD_NAVIGATION) != 0) {
                    // if (DEBUG) Slog.v(TAG, "Window asks to show input going forward");
                    ShowCurrentInputLocked(InputMethodManager_SHOW_IMPLICIT, NULL);
                }
                break;
            case WindowManagerLayoutParams_SOFT_INPUT_STATE_ALWAYS_VISIBLE:
                // if (DEBUG) Slog.v(TAG, "Window asks to always show input");
                ShowCurrentInputLocked(InputMethodManager_SHOW_IMPLICIT, NULL);
                break;
        }
    }

    Binder::RestoreCallingIdentity(ident);
    return NOERROR;
}

ECode CInputMethodManagerService::ShowInputMethodPickerFromClient(
    /* [in] */ IInputMethodClient* client)
{
    Mutex::Autolock lock(mMethodMapLock);

    if (mCurClient == NULL || client == NULL
            || IBinder::Probe(mCurClient->mClient) != IBinder::Probe(client)) {
        // Slog.w(TAG, "Ignoring showInputMethodDialogFromClient of uid "
        //         + Binder::GetCallingUid() + ": " + client);
    }

    void (STDCALL CInputMethodManagerService::*pHandlerFunc)();
    pHandlerFunc = &CInputMethodManagerService::HandleShowIMPicker;
    return mHandler->PostCppCallbackDelayed(
        (Handle32)this, *(Handle32*)&pHandlerFunc,
        NULL, 0, 0);
}

ECode CInputMethodManagerService::SetInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ const String& id)
{
    Mutex::Autolock lock(mMethodMapLock);

    if (token == NULL) {
        Int32 perm = 0;
        mContext->CheckCallingOrSelfPermission(
                String("")/*android.Manifest.permission.WRITE_SECURE_SETTINGS*/, &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            assert(0);
            // throw new SecurityException(
            //         "Using NULL token requires permission "
            //         + android.Manifest.permission.WRITE_SECURE_SETTINGS);
            return E_SECURITY_EXCEPTION;
        }
    }
    else if (mCurToken.Get() != token) {
        // Slog.w(TAG, "Ignoring setInputMethod of uid " + Binder::GetCallingUid()
        //         + " token: " + token);
        return NOERROR;
    }

    Int64 ident = Binder::ClearCallingIdentity();
    ECode ec = SetInputMethodLocked(id);
    Binder::RestoreCallingIdentity(ident);

    return ec;
}

ECode CInputMethodManagerService::HideMySoftInput(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 flags)
{
    Mutex::Autolock lock(mMethodMapLock);

    if (token == NULL || mCurToken.Get() != token) {
        // if (DEBUG) Slog.w(TAG, "Ignoring hideInputMethod of uid "
        //         + Binder::GetCallingUid() + " token: " + token);
        return NOERROR;
    }

    Int64 ident = Binder::ClearCallingIdentity();
    HideCurrentInputLocked(flags, NULL);
    Binder::RestoreCallingIdentity(ident);

    return NOERROR;
}

ECode CInputMethodManagerService::ShowMySoftInput(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 flags)
{
    Mutex::Autolock lock(mMethodMapLock);

    if (token == NULL || mCurToken.Get() != token) {
        // Slog.w(TAG, "Ignoring showMySoftInput of uid "
        //         + Binder::GetCallingUid() + " token: " + token);
        return NOERROR;
    }

    Int64 ident = Binder::ClearCallingIdentity();
    ShowCurrentInputLocked(flags, NULL);
    Binder::RestoreCallingIdentity(ident);

    return NOERROR;
}

void CInputMethodManagerService::SetEnabledSessionInMainThread(
    /* [in] */ SessionState* session)
{
    if (mEnabledSession.Get() != session) {
        if (mEnabledSession != NULL) {
            // try {
                // if (DEBUG) Slog.v(TAG, "Disabling: " + mEnabledSession);
            mEnabledSession->mMethod->SetSessionEnabled(
                    mEnabledSession->mSession, FALSE);
            // } catch (RemoteException e) {
            // }
        }
        mEnabledSession = session;
        // try {
        // if (DEBUG) Slog.v(TAG, "Enabling: " + mEnabledSession);
        session->mMethod->SetSessionEnabled(
                session->mSession, TRUE);
        // } catch (RemoteException e) {
        // }
    }
}

Boolean CInputMethodManagerService::IsSystemIme(
    /* [in] */ IInputMethodInfo* inputMethod)
{
    assert(inputMethod != NULL);

    AutoPtr<IServiceInfo> serInfo;
    ASSERT_SUCCEEDED(inputMethod->GetServiceInfo((IServiceInfo**)&serInfo));
    AutoPtr<IApplicationInfo> appInfo;
    ASSERT_SUCCEEDED(serInfo->GetApplicationInfo((IApplicationInfo**)&appInfo));
    Int32 flag = 0;
    appInfo->GetFlags(&flag);
    return (flag & ApplicationInfo_FLAG_SYSTEM) != 0;
}

Boolean CInputMethodManagerService::ChooseNewDefaultIMELocked()
{
    AutoPtr<IObjectContainer> methods;
    GetEnabledInputMethodListLocked((IObjectContainer**)&methods);
    Int32 i = 0;
    methods->GetObjectCount(&i);
    if (i > 0) {
        // We'd prefer to fall back on a system IME, since that is safer.
        //TODO: should fall back;
        AutoPtr<IObjectEnumerator> enabledItem;
        methods->GetObjectEnumerator((IObjectEnumerator**)&enabledItem);
        Boolean hasNext = FALSE;
        while(enabledItem->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInputMethodInfo> methodInfo;
            enabledItem->Current((IInterface**)&methodInfo);
            AutoPtr<IServiceInfo> serInfo;
            methodInfo->GetServiceInfo((IServiceInfo**) &serInfo);
            AutoPtr<IApplicationInfo> appInfo;
            serInfo->GetApplicationInfo((IApplicationInfo**) &appInfo);
            Int32 flag = 0;
            appInfo->GetFlags(&flag);
            if ((flag & ApplicationInfo_FLAG_SYSTEM) != 0) {
                break;
            }
        }
        // Settings.Secure.putString(mContext.getContentResolver(),
        //         Settings.Secure.DEFAULT_INPUT_METHOD,
        //         enabled.get(i).getId());
        return TRUE;
    }

    return FALSE;
}

void CInputMethodManagerService::BuildInputMethodListLocked(
    /* [in] */ List<AutoPtr<IInputMethodInfo> >& list,
    /* [in] */ HashMap<String, AutoPtr<IInputMethodInfo> >& map)
{
    list.Clear();
    map.Clear();

    AutoPtr<ILocalCapsuleManager> pm;
    mContext->GetCapsuleManager((ILocalCapsuleManager**)&pm);
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**)&config);
    Int32 keyboard = 0;
    config->GetKeyboard(&keyboard);
    const Boolean haveHardKeyboard = keyboard == Configuration_KEYBOARD_QWERTY;
    // String disabledSysImes = Settings.Secure.getString(mContext.getContentResolver(),
    //         Secure.DISABLED_SYSTEM_INPUT_METHODS);
    // if (disabledSysImes.IsEmpty()) disabledSysImes = "";
    String disabledSysImes("");

    AutoPtr<IIntent> intent;
    CIntent::New(String(InputMethod_SERVICE_INTERFACE), (IIntent**)&intent);
    AutoPtr<IObjectContainer> query;
    pm->QueryIntentServices(intent, CapsuleManager_GET_META_DATA,
            (IObjectContainer**)&query);
    AutoPtr<IObjectEnumerator> svItem;
    query->GetObjectEnumerator((IObjectEnumerator**)&svItem);
    Boolean hasNext = FALSE;
    while(svItem->MoveNext(&hasNext), hasNext) {
        AutoPtr<IResolveInfo> ri;
        svItem->Current((IInterface**)&ri);
        AutoPtr<IServiceInfo> si;
        ri->GetServiceInfo((IServiceInfo**)&si);
        String capName, sName;
        si->GetCapsuleName(&capName);
        si->GetName(&sName);
        AutoPtr<IComponentName> compName;
        CComponentName::New(capName, sName, (IComponentName**)&compName);
        // if (!android.Manifest.permission.BIND_INPUT_METHOD.Equals(
        //         si.permission)) {
        //     Slog.w(TAG, "Skipping input method " + compName
        //             + ": it does not require the permission "
        //             + android.Manifest.permission.BIND_INPUT_METHOD);
        //     continue;
        // }

        // if (DEBUG) Slog.d(TAG, "Checking " + compName);

        //try {
        AutoPtr<IInputMethodInfo> p;
        CInputMethodInfo::New(mContext, ri, (IInputMethodInfo**)&p);
        list.PushBack(p);
        String id;
        p->GetId(&id);
        map[id] = p;
        // System IMEs are enabled by default, unless there's a hard keyboard
        // and the system IME was explicitly disabled
        if (IsSystemIme(p) && (!haveHardKeyboard || disabledSysImes.IndexOf(id) < 0)) {
            SetInputMethodEnabledLocked(id, TRUE);
        }

        // if (DEBUG) {
        //     Slog.d(TAG, "Found a third-party input method " + p);
        // }

        // } catch (XmlPullParserException e) {
        //     Slog.w(TAG, "Unable to load input method " + compName, e);
        // } catch (IOException e) {
        //     Slog.w(TAG, "Unable to load input method " + compName, e);
        // }
    }

    // String defaultIme = Settings.Secure.getString(mContext
    //         .getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD);
    // if (!TextUtils.isEmpty(defaultIme) && !map.containsKey(defaultIme)) {
    //     if (chooseNewDefaultIMELocked()) {
    //         updateFromSettingsLocked();
    //     }
    // }
}

void CInputMethodManagerService::ShowInputMethodMenu()
{
    assert(0);

    // if (DEBUG) Slog.v(TAG, "Show switching menu");

    // final Context context = mContext;

    // final PackageManager pm = context.getPackageManager();

    // String lastInputMethodId = Settings.Secure.getString(context
    //         .getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD);
    // if (DEBUG) Slog.v(TAG, "Current IME: " + lastInputMethodId);

    // final List<IInputMethodInfo> immis = getEnabledInputMethodList();

    // if (immis == NULL) {
    //     return;
    // }

    // {
    //     Mutex::Autolock lock(mMethodMapLock);
    //     hideInputMethodMenuLocked();

    //     Int32 N = immis.size();

    //     final Map<CharSequence, IInputMethodInfo> imMap =
    //         new TreeMap<CharSequence, IInputMethodInfo>(Collator.getInstance());

    //     for (Int32 i = 0; i < N; ++i) {
    //         IInputMethodInfo property = immis.get(i);
    //         if (property == NULL) {
    //             continue;
    //         }
    //         imMap.put(property.loadLabel(pm), property);
    //     }

    //     N = imMap.size();
    //     mItems = imMap.keySet().toArray(new CharSequence[N]);
    //     mIms = imMap.values().toArray(new IInputMethodInfo[N]);

    //     Int32 checkedItem = 0;
    //     for (Int32 i = 0; i < N; ++i) {
    //         if (mIms[i].getId().Equals(lastInputMethodId)) {
    //             checkedItem = i;
    //             break;
    //         }
    //     }

    //     AlertDialog.OnClickListener adocl = new AlertDialog.OnClickListener() {
    //         public void onClick(DialogInterface dialog, Int32 which) {
    //             hideInputMethodMenu();
    //         }
    //     };

    //     TypedArray a = context.obtainStyledAttributes(NULL,
    //             com.android.internal.R.styleable.DialogPreference,
    //             com.android.internal.R.attr.alertDialogStyle, 0);
    //     mDialogBuilder = new AlertDialog.Builder(context)
    //             .setTitle(com.android.internal.R.string.select_input_method)
    //             .setOnCancelListener(new OnCancelListener() {
    //                 public void onCancel(DialogInterface dialog) {
    //                     hideInputMethodMenu();
    //                 }
    //             })
    //             .setIcon(a.getDrawable(
    //                     com.android.internal.R.styleable.DialogPreference_dialogTitle));
    //     a.recycle();

    //     mDialogBuilder.setSingleChoiceItems(mItems, checkedItem,
    //             new AlertDialog.OnClickListener() {
    //                 public void onClick(DialogInterface dialog, Int32 which) {
    //                     {
    //                         Mutex::Autolock lock(mMethodMapLock);
    //                         if (mIms == NULL || mIms.length <= which) {
    //                             return;
    //                         }
    //                         IInputMethodInfo im = mIms[which];
    //                         hideInputMethodMenu();
    //                         if (im != NULL) {
    //                             setInputMethodLocked(im.getId());
    //                         }
    //                     }
    //                 }
    //             });

    //     mSwitchingDialog = mDialogBuilder.create();
    //     mSwitchingDialog.getWindow().setType(
    //             WindowManager.LayoutParams.TYPE_INPUT_METHOD_DIALOG);
    //     mSwitchingDialog.show();
    // }
}

void CInputMethodManagerService::HideInputMethodMenu()
{
    Mutex::Autolock lock(mMethodMapLock);
    HideInputMethodMenuLocked();
}

void CInputMethodManagerService::HideInputMethodMenuLocked()
{
    // if (DEBUG) Slog.v(TAG, "Hide switching menu");

    if (mSwitchingDialog != NULL) {
        mSwitchingDialog->Dismiss();
        mSwitchingDialog = NULL;
    }

    mDialogBuilder = NULL;
    mItems = NULL;
    mIms = NULL;
}

ECode CInputMethodManagerService::SetInputMethodEnabled(
    /* [in] */ const String& id,
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);

    Mutex::Autolock lock(mMethodMapLock);

    Int32 perm = 0;
    mContext->CheckCallingOrSelfPermission(
            String("")/*android.Manifest.permission.WRITE_SECURE_SETTINGS*/, &perm);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
        // throw new SecurityException(
        //         "Requires permission "
        //         + android.Manifest.permission.WRITE_SECURE_SETTINGS);
        return E_SECURITY_EXCEPTION;
    }

    Int64 ident = Binder::ClearCallingIdentity();
    *state = SetInputMethodEnabledLocked(id, enabled);
    Binder::RestoreCallingIdentity(ident);

    return NOERROR;
}

Boolean CInputMethodManagerService::SetInputMethodEnabledLocked(
    /* [in] */ const String& id,
    /* [in] */ Boolean enabled)
{
    // Make sure this is a valid input method.
    HashMap<String, AutoPtr<IInputMethodInfo> >::Iterator it =
            mMethodMap.Find(id);
    if (it == mMethodMap.End()) {
        assert(0);
        //throw new IllegalArgumentException("Unknown id: " + mCurMethodId);
    }

    StringBuffer builder(256);

    Boolean removed = FALSE;
    String firstId;

    // Look through the currently enabled input methods.
    // String enabledStr = Settings.Secure.getString(mContext.getContentResolver(),
    //         Settings.Secure.ENABLED_INPUT_METHODS);
    String enabledStr(ENABLED_STR);
    if (!enabledStr.IsNull()) {
        mStringColonSplitter->SetString(enabledStr);
        while (mStringColonSplitter->HasNext()) {
            String curId;
            mStringColonSplitter->Next(&curId);
            if (curId.Equals(id)) {
                if (enabled) {
                    // We are enabling this input method, but it is
                    // already enabled.  Nothing to do.  The previous
                    // state was enabled.
                    return TRUE;
                }

                // We are disabling this input method, and it is
                // currently enabled.  Skip it to remove from the
                // new list.
                removed = TRUE;
            }
            else if (!enabled) {
                // We are building a new list of input methods that
                // doesn't contain the given one.
                if (firstId.IsNull()) firstId = curId;
                if (builder.GetLength() > 0) builder += ':';
                builder += curId;
            }
        }
    }

    if (!enabled) {
        if (!removed) {
            // We are disabling the input method but it is already
            // disabled.  Nothing to do.  The previous state was
            // disabled.
            return FALSE;
        }

        // Update the setting with the new list of input methods.
/*        Settings.Secure.putString(mContext.getContentResolver(),
                Settings.Secure.ENABLED_INPUT_METHODS, builder.toString());*/
        // We the disabled input method is currently selected, switch
        // to another one.
/*        String selId = Settings.Secure.getString(mContext.getContentResolver(),
                Settings.Secure.DEFAULT_INPUT_METHOD);
        if (id.Equals(selId)) {
            Settings.Secure.putString(mContext.getContentResolver(),
                    Settings.Secure.DEFAULT_INPUT_METHOD,
                    firstId != NULL ? firstId : "");
        }*/
        // Previous state was enabled.
        return TRUE;
    }

    // Add in the newly enabled input method.
    if (enabledStr.IsNullOrEmpty()) {
        enabledStr = id;
    }
    else {
        enabledStr = enabledStr + ":" + id;
    }

    // Settings.Secure.putString(mContext.getContentResolver(),
    //         Settings.Secure.ENABLED_INPUT_METHODS, enabledBuf.GetPayload());

    // Previous state was disabled.
    return FALSE;
}

void CInputMethodManagerService::HandleShowIMPicker()
{
    ShowInputMethodMenu();
}

void CInputMethodManagerService::HandleUnbindInput(
    /* [in] */ IInputMethod* inputMethod)
{
    // try {
    inputMethod->UnbindInput();
    // } catch (RemoteException e) {
    //     // There is nothing interesting about the method dying.
    // }
}

void CInputMethodManagerService::HandleBindInput(
    /* [in] */ IInputMethod* inputMethod,
    /* [in] */ IInputBinding* binding)
{
    // try {
    inputMethod->BindInput(binding);
    // } catch (RemoteException e) {
    // }
}

void CInputMethodManagerService::HandleShowSoftInput(
    /* [in] */ IInputMethod* inputMethod,
    /* [in] */ Int32 flags,
    /* [in] */ ILocalResultReceiver* resultReceiver)
{
    // try {
    inputMethod->ShowSoftInput(flags, resultReceiver);
    // } catch (RemoteException e) {
    // }
}

void CInputMethodManagerService::HandleHideSoftInput(
    /* [in] */ IInputMethod* inputMethod,
    /* [in] */ ILocalResultReceiver* resultReceiver)
{
    // try {
    inputMethod->HideSoftInput(0, resultReceiver);
    // } catch (RemoteException e) {
    // }
}

void CInputMethodManagerService::HandleAttachToken(
    /* [in] */ IInputMethod* inputMethod,
    /* [in] */ IBinder* binder)
{
    // try {
    inputMethod->AttachToken(binder);
    // if (DEBUG) Slog.v(TAG, "Sending attach of token: " + args.arg2);
    // } catch (RemoteException e) {
    // }
}

void CInputMethodManagerService::HandleCreateSession(
    /* [in] */ IInputMethod* inputMethod,
    /* [in] */ IInputMethodCallback* methodcallback)
{
    // try {
    inputMethod->CreateSession(methodcallback);
    // } catch (RemoteException e) {
    // }
}

void CInputMethodManagerService::HandleStartInput(
    /* [in] */ SessionState* state,
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* info)
{
    // try {
    assert(state != NULL && state->mMethod != NULL);
    SetEnabledSessionInMainThread(state);
    state->mMethod->StartInput(inputContext, info);
    // } catch (RemoteException e) {
    // }
}

void CInputMethodManagerService::HandleRestartInput(
    /* [in] */ SessionState* state,
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* info)
{
    // try {
    assert(state != NULL && state->mMethod != NULL);
    SetEnabledSessionInMainThread(state);
    state->mMethod->RestartInput(inputContext, info);
    // } catch (RemoteException e) {
    // }
}

void CInputMethodManagerService::HandleUnbindMethod(
    /* [in] */ IInputMethodClient* inputMethod,
    /* [in] */ Int32 flags)
{
    // try {
    inputMethod->OnUnbindMethod(flags);
    // } catch (RemoteException e) {
    //     // There is nothing interesting about the method dying.
    // }
}

void CInputMethodManagerService::HandleBindMethod(
    /* [in] */ IInputMethodClient* inputMethod,
    /* [in] */ IInputBindResult* bindRes)
{
    // try {
    inputMethod->OnBindMethod(bindRes);
    // } catch (RemoteException e) {
    //     Slog.w(TAG, "Client died receiving input method " + args.arg2);
    // }
}
