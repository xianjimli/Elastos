
#include "ext/frameworkext.h"
#include "server/CInputMethodManagerService.h"
#include "os/ServiceManager.h"
#include "os/Binder.h"
#include "os/SystemClock.h"


using namespace Elastos::Core;

CString CInputMethodManagerService::TAG = "CInputMethodManagerService";
CString CInputMethodManagerService::ENABLED_STR = "com.android.inputmethod.latin/.LatinIME:com.android.inputmethod.pinyin/.PinyinIME";
CString CInputMethodManagerService::DEFAULT_INPUT_METHOD = "com.android.inputmethod.pinyin/.PinyinIME";
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
const long CInputMethodManagerService::TIME_TO_RECONNECT;


CInputMethodManagerService::SessionState::SessionState(
    /* [in] */ ClientState* _client,
    /* [in] */ IInputMethodStub* _method,
    /* [in] */ IInputMethodSessionStub* _session):
    //mClient(_client),
    mMethod(_method),
    mSession(_session)
{

}

UInt32 CInputMethodManagerService::SessionState::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 CInputMethodManagerService::SessionState::Release()
{
	return ElRefBase::Release();
}

CInputMethodManagerService::ClientState::ClientState(
    /* [in] */ IInputMethodClientStub* _client,
    /* [in] */ IInputContextStub* _inputContext,
    /* [in] */ Int32 _uid,
    /* [in] */ Int32 _pid):
	mClient(_client),
	mInputContext(_inputContext),
	mUid(_uid),
	mPid(_pid),
	mSessionRequested(FALSE)
{
    assert(mInputContext != NULL);
    CInputBinding::New(NULL, IBinder::Probe(mInputContext),
            mUid, mPid, (IInputBinding**)&mBinding);
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
    /* [in] */ CInputMethodManagerService* host):
    BroadcastReceiver(),
    mHost(host)
{

}

CInputMethodManagerService::ScreenOnOffReceiver::~ScreenOnOffReceiver()
{
}

ECode CInputMethodManagerService::ScreenOnOffReceiver::OnReceive(
    /* [in] */ IContext *pContext,
    /* [in] */ IIntent *intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(Intent_ACTION_SCREEN_ON)) {
        mHost->mScreenOn = TRUE;
    } else if (action.Equals(Intent_ACTION_SCREEN_OFF)) {
        mHost->mScreenOn = FALSE;
    } else if (action.Equals(Intent_ACTION_CLOSE_SYSTEM_DIALOGS)) {
        mHost->HideInputMethodMenu();
        return NOERROR;
    } else {
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

CInputMethodManagerService::MethodCallback::MethodCallback(
    /* [in] */ IInputMethodStub* method,
    /* [in] */ CInputMethodManagerService* host):
    mMethod(method),
    mHost(host)
{
}

UInt32 CInputMethodManagerService::MethodCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CInputMethodManagerService::MethodCallback::Release()
{
    return ElRefBase::Release();
}

PInterface CInputMethodManagerService::MethodCallback::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputMethodCallbackStub == riid) {
        return (IInputMethodCallbackStub *)this;
    }

    return NULL;
}

ECode CInputMethodManagerService::MethodCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IInputMethodCallbackStub*)this) {
        *pIID = EIID_IInputMethodCallbackStub;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CInputMethodManagerService::MethodCallback::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    return NOERROR;
}

ECode CInputMethodManagerService::MethodCallback::SessionCreated(
    /* [in] */ IInputMethodSessionStub* session)
{
    assert(mHost != NULL);
    mHost->OnSessionCreated(mMethod, session);
    return NOERROR;
}

ECode CInputMethodManagerService::MethodCallback::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

CInputMethodManagerService::CInputMethodManagerService()
{
    CInputBindResult::New(NULL, String(""), -1, (IInputBindResult**) &mNoBinding);

    mStringColonSplitter = new TextUtils::SimpleStringSplitter(':');
    mSystemReady = FALSE;
    mCurSeq = 0;
    mHaveConnection = FALSE;
    mShowRequested = FALSE;
    mShowExplicitlyRequested = FALSE;
    mShowForced = FALSE;
    mInputShown = FALSE;
    mLastBindTime = 0;
    mBoundToMethod = FALSE;
    mScreenOn = TRUE;
    mIms = NULL;
    mItems = NULL;
}

CInputMethodManagerService::~CInputMethodManagerService()
{
    if (mStringColonSplitter != NULL) {
        delete mStringColonSplitter;
        mStringColonSplitter = NULL;
    }
}

ECode CInputMethodManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IStatusBarService* statusBar)
{
    mContext = context;

    //TODO -->Apartment->Start
    CApartment::New(TRUE, (IApartment**)&mHandler);
    mHandler->Start(ApartmentAttr_New);
    CApartment::New(TRUE, (IApartment**)&mCaller);
    mCaller->Start(ApartmentAttr_New);

    mIWindowManager = IWindowManagerStub::Probe(
            ServiceManager::GetService(String(Context_WINDOW_SERVICE)).Get());

    //TODO
    // mCaller = new HandlerCaller(context, new HandlerCaller.Callback() {
    //     public void executeMessage(Message msg) {
    //         handleMessage(msg);
    //     }
    // });

    //(new MyPackageMonitor()).register(mContext, TRUE);

    AutoPtr<IIntentFilter> screenOnOffFilt;
    CIntentFilter::New((IIntentFilter**) &screenOnOffFilt);

    screenOnOffFilt->AddAction(String(Intent_ACTION_SCREEN_ON));
    screenOnOffFilt->AddAction(String(Intent_ACTION_SCREEN_OFF));
    screenOnOffFilt->AddAction(String(Intent_ACTION_CLOSE_SYSTEM_DIALOGS));

    //TODO
    assert(0);
    //mContext->RegisterReceiver(new ScreenOnOffReceiver(), screenOnOffFilt);

    mStatusBar = statusBar;
    statusBar->SetIconVisibility(String("ime"), FALSE);

    BuildInputMethodListLocked(mMethodList, mMethodMap);

    //TODO
    // String enabledStr = Settings.Secure.getString(
    //         mContext.getContentResolver(),
    //         Settings.Secure.ENABLED_INPUT_METHODS);
    String enabledStr((const char*)ENABLED_STR);


    //TODO
    // Slog.i(TAG, "Enabled input methods: " + enabledStr);
    // final String defaultIme = Settings.Secure.getString(mContext
    //         .getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD);
    String defaultIme((const char*)DEFAULT_INPUT_METHOD);

    AutoPtr<ICharSequence> str;
    CStringWrapper::New(defaultIme, (ICharSequence**) &str);
    if (enabledStr == NULL || TextUtils::IsEmpty(str)) {
        // Slog.i(TAG, "Enabled input methods or default IME has not been set, enabling all");
        AutoPtr<IInputMethodInfo> defIm;
        StringBuf_<256> sb;
        const Int32 N = mMethodList.GetSize();
        Int32 defaultResId = 0;
        String id;
        for (Int32 i=0; i<N; i++) {
            AutoPtr<IInputMethodInfo> imi = mMethodList[i];
            // Slog.i(TAG, "Adding: " + imi.getId());
            if (i > 0) sb.Append(':');

            assert(imi != NULL);
            imi->GetId(&id);
            sb.Append(id);

            imi->GetIsDefaultResourceId(&defaultResId);
            if (defIm == NULL && defaultResId != 0) {
                String capName;
                imi->GetCapsuleName(&capName);
                AutoPtr<IContext> ctx;
                mContext->CreateCapsuleContext(capName, 0, (IContext**) &ctx);
                assert(ctx != NULL);

                AutoPtr<IResources> res;
                ctx->GetResources((IResources**) &res);

                Boolean state = FALSE;
                res->GetBoolean(defaultResId, &state);
                if (state) {
                    defIm = imi;
                    // Slog.i(TAG, "Selected default: " + imi.getId());
                }
                // } catch (PackageManager.NameNotFoundException ex) {
                // } catch (Resources.NotFoundException ex) {
                // }
            }
        }

        if (defIm == NULL && N > 0) {
            defIm = mMethodList[0];
            // Slog.i(TAG, "No default found, using " + defIm.getId());
        }

        //TODO
        // Settings.Secure.putString(mContext.getContentResolver(),
        //         Settings.Secure.ENABLED_INPUT_METHODS, sb.toString());
        // if (defIm != NULL) {
        //     Settings.Secure.putString(mContext.getContentResolver(),
        //             Settings.Secure.DEFAULT_INPUT_METHOD, defIm.getId());
        // }
    }

    //TODO
    //mSettingsObserver = new SettingsObserver(mHandler);
    UpdateFromSettingsLocked();
    return NOERROR;
}

ECode CInputMethodManagerService::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputMethodManagerService::OnTransact(
    /* [in] */ Int32 code,
    /* [in] */ IParcel* data,
    /* [in] */ IParcel* reply,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* state)
{
    //TODO
    // try {
    //     return super.OnTransact(code, data, reply, flags);
    // } catch (RuntimeException e) {
    //     // The input method manager only throws security exceptions, so let's
    //     // log all others.
    //     if (!(e instanceof SecurityException)) {
    //         Slog.e(TAG, "Input Method Manager Crash", e);
    //     }
    //     throw e;
    // }

    assert(0);
    return NOERROR;
}

ECode CInputMethodManagerService::SystemReady()
{
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
    }

    return NOERROR;
}

ECode CInputMethodManagerService::GetInputMethodList(
    /* [out] */ IObjectContainer** infos)
{
    assert(infos != NULL);

    Mutex::Autolock lock(mMethodMapLock);
    List< AutoPtr<IInputMethodInfo> >::Iterator it;

    for (it = mMethodList.Begin(); it != mMethodList.End(); it++) {
        AutoPtr<IInputMethodInfo> item = *it;
        (*infos)->Add(item);

        if (item != NULL) {
            item->AddRef();
        }
    }

    return NOERROR;
}

ECode CInputMethodManagerService::GetEnabledInputMethodList(
    /* [out] */ IObjectContainer** list)
{
    assert(list != NULL);

    Mutex::Autolock lock(mMethodMapLock);
    return GetEnabledInputMethodListLocked(list);
}

ECode CInputMethodManagerService::GetEnabledInputMethodListLocked(
    /* [out] */ IObjectContainer** list)
{
    assert(list != NULL);

    //TODO
    // final String enabledStr = Settings.Secure.getString(
    //         mContext.getContentResolver(),
    //         Settings.Secure.ENABLED_INPUT_METHODS);
    String enabledStr((const char*)ENABLED_STR);

    if (enabledStr != NULL) {
        mStringColonSplitter->SetString(enabledStr);

        while (mStringColonSplitter->HasNext()) {
            String str;
            mStringColonSplitter->Next(&str);
            AutoPtr<IInputMethodInfo> info = mMethodMap[str];
            if (info != NULL) {
                (*list)->Add(info);
            }
        }
    }

    return NOERROR;
}

ECode CInputMethodManagerService::AddClient(
    /* [in] */ IInputMethodClientStub* client,
    /* [in] */ IInputContextStub* inputContext,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid)
{
    Mutex::Autolock lock(mMethodMapLock);
    mClients[IBinder::Probe(client)] = new ClientState(client, inputContext, uid, pid);

    return NOERROR;
}

ECode CInputMethodManagerService::RemoveClient(
    /* [in] */ IInputMethodClientStub* client)
{
    Mutex::Autolock lock(mMethodMapLock);
    mClients.Erase(IBinder::Probe(client));

    return NOERROR;
}

ECode CInputMethodManagerService::StartInput(
    /* [in] */ IInputMethodClientStub* client,
    /* [in] */ IInputContextStub* inputContext,
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean initial,
    /* [in] */ Boolean needResult,
    /* [out] */ IInputBindResult** res)
{
    Mutex::Autolock lock(mMethodMapLock);
    long ident = Binder::ClearCallingIdentity();

    StartInputLocked(client, inputContext, attribute,
            initial, needResult);

    Binder::RestoreCallingIdentity(ident);

    return NOERROR;
}

ECode CInputMethodManagerService::FinishInput(
    /* [in] */ IInputMethodClientStub* client)
{
    return NOERROR;
}

ECode CInputMethodManagerService::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    Mutex::Autolock lock(mMethodMapLock);
    assert(name != NULL && service != NULL && mCurIntent != NULL);

    AutoPtr<IComponentName> tmpName;
    mCurIntent->GetComponent((IComponentName**) &tmpName);

    Boolean equal = FALSE;
    if (mCurIntent != NULL && (name->Equals(tmpName, &equal), equal)) {
        mCurMethod = IInputMethodStub::Probe(service);
        if (mCurToken == NULL) {
            // Slog.w(TAG, "Service connected without a token!");
            UnbindCurrentMethodLocked(FALSE);
            return NOERROR;
        }

        // if (DEBUG) Slog.v(TAG, "Initiating attach with token: " + mCurToken);
        // executeOrSendMessage(mCurMethod, mCaller.obtainMessageOO(
        //         MSG_ATTACH_TOKEN, mCurMethod, mCurToken));

        if (mCurMethod != NULL) {
            //Send msg.
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodStub*, IBinder*);
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
            //Handle msg.
            HandleAttachToken(mCurMethod, mCurToken);
        }

        if (mCurClient != NULL) {
            // if (DEBUG) Slog.v(TAG, "Creating first session while with client "
            //         + mCurClient);
            // executeOrSendMessage(mCurMethod, mCaller.obtainMessageOO(
            //         MSG_CREATE_SESSION, mCurMethod,
            //         new MethodCallback(mCurMethod)));

            if (mCurMethod != NULL) {
                //Send msg.
                void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodStub*, IInputMethodCallbackStub*);
                pHandlerFunc = &CInputMethodManagerService::HandleCreateSession;
                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);
                params->WriteInterfacePtr(mCurMethod.Get());
                params->WriteInterfacePtr(new MethodCallback(mCurMethod, this));

                mCaller->PostCppCallbackDelayed(
                    (Handle32)this, *(Handle32*)&pHandlerFunc,
                    params, 0, 0);
            }
            else {
                //Handle msg.
                HandleCreateSession(mCurMethod, new MethodCallback(mCurMethod, this));
            }
        }
    }

    return NOERROR;
}

ECode CInputMethodManagerService::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    Mutex::Autolock lock(mMethodMapLock);

    // if (DEBUG) Slog.v(TAG, "Service disconnected: " + name
    //         + " mCurIntent=" + mCurIntent);

    AutoPtr<IComponentName> tmpName;
    mCurIntent->GetComponent((IComponentName**) &tmpName);
    Boolean equal = FALSE;
    if (mCurMethod != NULL && mCurIntent != NULL
            && (name->Equals(tmpName, &equal), equal))
    {
        ClearCurMethodLocked();
        // We consider this to be a new bind attempt, since the system
        // should now try to restart the service for us.
        mLastBindTime = SystemClock::UptimeMillis();
        mShowRequested = mInputShown;
        mInputShown = FALSE;

        if (mCurClient != NULL) {
            // executeOrSendMessage(mCurClient.client, mCaller.obtainMessageIO(
            //         MSG_UNBIND_METHOD, mCurSeq, mCurClient.client));

            if (IBinder::Probe(mCurClient->mClient) != NULL) {
                //Send msg.
                void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodClientStub*, Int32);
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
                //Handle msg.
                HandleUnbindMethod(mCurClient->mClient.Get(), mCurSeq);
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
    long ident = Binder::ClearCallingIdentity();

    if (token == NULL || mCurToken.Get() != token) {
        // Slog.w(TAG, "Ignoring setInputMethod of uid " + uid + " token: " + token);
        return NOERROR;
    }

    {
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
    }

    Binder::RestoreCallingIdentity(ident);
    return NOERROR;
}

void CInputMethodManagerService::UpdateFromSettingsLocked()
{
    // We are assuming that whoever is changing DEFAULT_INPUT_METHOD and
    // ENABLED_INPUT_METHODS is taking care of keeping them correctly in
    // sync, so we will never have a DEFAULT_INPUT_METHOD that is not
    // enabled.

    //TODO
    // String id = Settings.Secure.getString(mContext.getContentResolver(),
    //     Settings.Secure.DEFAULT_INPUT_METHOD);

    String id((const char*)DEFAULT_INPUT_METHOD);

    if (id != NULL && id.GetLength() > 0) {
        ECode ec = SetInputMethodLocked(id);
        if (ec != 0) {
            mCurMethodId = NULL;
            UnbindCurrentMethodLocked(TRUE);
        }
        // } catch (IllegalArgumentException e) {
        //     Slog.w(TAG, "Unknown input method from prefs: " + id, e);
        //     mCurMethodId = NULL;
        //     UnbindCurrentMethodLocked(TRUE);
        // }
    } else {
        // There is no longer an input method set, so stop any current one.
        mCurMethodId = NULL;
        UnbindCurrentMethodLocked(TRUE);
    }
}

ECode CInputMethodManagerService::SetInputMethodLocked(
    /* [in] */ const String& id)
{
    AutoPtr<IInputMethodInfo> info = mMethodMap[id];
    if (info == NULL) {
        //throw new IllegalArgumentException("Unknown id: " + id);
        return E_INVALID_ARGUMENT;
    }

    if (id.Equals(mCurMethodId)) {
        return NOERROR;
    }

    long ident = Binder::ClearCallingIdentity();
    mCurMethodId = id;

    //TODO
    // Settings.Secure.putString(mContext.getContentResolver(),
    //     Settings.Secure.DEFAULT_INPUT_METHOD, id);

    //For ActivityManagerNative.IsSystemReady()
    assert(0);
    if (/*ActivityManagerNative.IsSystemReady()*/ TRUE) {
        AutoPtr<IIntent> intent;
        CIntent::New(String(Intent_ACTION_INPUT_METHOD_CHANGED), (IIntent**) &intent);
        intent->AddFlags(Intent_FLAG_RECEIVER_REPLACE_PENDING);
        intent->PutStringExtra(String("input_method_id"), id);
        mContext->SendBroadcast(intent);
    }

    UnbindCurrentClientLocked();

    Binder::RestoreCallingIdentity(ident);

    return NOERROR;
}

ECode CInputMethodManagerService::ShowSoftInput(
    /* [in] */ IInputMethodClientStub* client,
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    //Int32 uid = Binder::GetCallingUid();
    long ident = Binder::ClearCallingIdentity();

    Boolean retState = FALSE;
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
                // Slog.w(TAG, "Ignoring showSoftInput of uid " + uid + ": " + client);
                *state = FALSE;
                return NOERROR;
            }
            // } catch (RemoteException e) {
            //     return FALSE;
            // }
        }

        // if (DEBUG) Slog.v(TAG, "Client requesting input be shown");
        retState = ShowCurrentInputLocked(flags, resultReceiver);
    }

    Binder::RestoreCallingIdentity(ident);
    *state = retState;
    return NOERROR;
}

ECode CInputMethodManagerService::HideSoftInput(
    /* [in] */ IInputMethodClientStub* client,
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    //Int32 uid = Binder::GetCallingUid();
    long ident = Binder::ClearCallingIdentity();

    Boolean retState = FALSE;
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
                return NOERROR;
            }
            // } catch (RemoteException e) {
            //     return FALSE;
            // }
        }

        // if (DEBUG) Slog.v(TAG, "Client requesting input be hidden");
        retState = HideCurrentInputLocked(flags, resultReceiver);
    }

    Binder::RestoreCallingIdentity(ident);

    *state = retState;
    return NOERROR;
}

ECode CInputMethodManagerService::WindowGainedFocus(
    /* [in] */ IInputMethodClientStub* client,
    /* [in] */ IBinder* windowToken,
    /* [in] */ Boolean viewHasFocus,
    /* [in] */ Boolean isTextEditor,
    /* [in] */ Int32 softInputMode,
    /* [in] */ Boolean first,
    /* [in] */ Int32 windowFlags)
{
    long ident = Binder::ClearCallingIdentity();

    {
        Mutex::Autolock lock(mMethodMapLock);
        // if (DEBUG) Slog.v(TAG, "windowGainedFocus: " + client.asBinder()
        //         + " viewHasFocus=" + viewHasFocus
        //         + " isTextEditor=" + isTextEditor
        //         + " softInputMode=#" + Integer.toHexString(softInputMode)
        //         + " first=" + first + " flags=#"
        //         + Integer.toHexString(windowFlags));

        if (mCurClient == NULL || client == NULL
                || IBinder::Probe(mCurClient->mClient) != IBinder::Probe(client))
        {
            // We need to check if this is the current client with
            // focus in the window manager, to allow this call to
            // be made before input is started in it.
            Boolean ret = FALSE;
            mIWindowManager->InputMethodClientHasFocus(client, &ret);
            if (!ret) {
                // Slog.w(TAG, "Client not active, ignoring focus gain of: " + client);
                return NOERROR;
            }
        }

        if (mCurFocusedWindow.Get() == windowToken) {
            // Slog.w(TAG, "Window already focused, ignoring focus gain of: " + client);
            return NOERROR;
        }

        mCurFocusedWindow = windowToken;

        switch (softInputMode & WindowManagerLayoutParams_SOFT_INPUT_MASK_STATE) {
            case WindowManagerLayoutParams_SOFT_INPUT_STATE_UNSPECIFIED:
                if (!isTextEditor || (softInputMode &
                        WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST)
                        != WindowManagerLayoutParams_SOFT_INPUT_ADJUST_RESIZE) {

                    AutoPtr<IWindowManagerLayoutParamsHelper> wmlpHelper;
                    CWindowManagerLayoutParamsHelper::AcquireSingleton(
                            (IWindowManagerLayoutParamsHelper**)&wmlpHelper);
                    assert(wmlpHelper != NULL);

                    Boolean mayUse = FALSE;
                    wmlpHelper->MayUseInputMethodHelper(windowFlags, &mayUse);
                    if (mayUse) {
                        // There is no focus view, and this window will
                        // be behind any soft input window, so hide the
                        // soft input window if it is shown.
                        // if (DEBUG) Slog.v(TAG, "Unspecified window will hide input");
                        HideCurrentInputLocked(InputMethodManager_HIDE_NOT_ALWAYS, NULL);
                    }
                } else if (isTextEditor && (softInputMode &
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
    /* [in] */ IInputMethodClientStub* client)
{
    Mutex::Autolock lock(mMethodMapLock);
    if (mCurClient == NULL || client == NULL
            || IBinder::Probe(mCurClient->mClient) != IBinder::Probe(client)) {
        // Slog.w(TAG, "Ignoring showInputMethodDialogFromClient of uid "
        //         + Binder::GetCallingUid() + ": " + client);
    }

    //Send msg.
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
        }
    } else if (mCurToken.Get() != token) {
        // Slog.w(TAG, "Ignoring setInputMethod of uid " + Binder::GetCallingUid()
        //         + " token: " + token);
        return NOERROR;
    }

    long ident = Binder::ClearCallingIdentity();
    SetInputMethodLocked(id);
    Binder::RestoreCallingIdentity(ident);

    return NOERROR;
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

    long ident = Binder::ClearCallingIdentity();
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

    long ident = Binder::ClearCallingIdentity();
    ShowCurrentInputLocked(flags, NULL);
    Binder::RestoreCallingIdentity(ident);

    return NOERROR;
}

ECode CInputMethodManagerService::SetInputMethodEnabled(
    /* [in] */ const String& id,
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    Mutex::Autolock lock(mMethodMapLock);

    Int32 perm = 0;
    mContext->CheckCallingOrSelfPermission(
            String("")/*android.Manifest.permission.WRITE_SECURE_SETTINGS*/, &perm);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
        assert(0);
        // throw new SecurityException(
        //         "Requires permission "
        //         + android.Manifest.permission.WRITE_SECURE_SETTINGS);
    }

    long ident = Binder::ClearCallingIdentity();

    Boolean retState = SetInputMethodEnabledLocked(id, enabled);
    Binder::RestoreCallingIdentity(ident);

    *state = retState;
    return NOERROR;
}

Boolean CInputMethodManagerService::SetInputMethodEnabledLocked(
    /* [in] */ const String& id,
    /* [in] */ Boolean enabled)
{
    // Make sure this is a valid input method.
    AutoPtr<IInputMethodInfo> imm = mMethodMap[id];
    if (imm == NULL) {
        if (imm == NULL) {
            assert(0);
            //throw new IllegalArgumentException("Unknown id: " + mCurMethodId);
        }
    }

    //StringBuilder builder = new StringBuilder(256);
    StringBuf_<256> builder;

    Boolean removed = FALSE;
    String firstId;

    // Look through the currently enabled input methods.

    //TODO
    // String enabledStr = Settings.Secure.getString(mContext.getContentResolver(),
    //         Settings.Secure.ENABLED_INPUT_METHODS);
    String enabledStr((const char*)ENABLED_STR);

    if (enabledStr != NULL) {
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
            } else if (!enabled) {
                // We are building a new list of input methods that
                // doesn't contain the given one.
                if (firstId == NULL) firstId = curId;
                if (builder.GetLength() > 0) builder.Append(':');
                builder.Append(curId);
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

        //TODO
/*        Settings.Secure.putString(mContext.getContentResolver(),
                Settings.Secure.ENABLED_INPUT_METHODS, builder.toString());*/
        // We the disabled input method is currently selected, switch
        // to another one.

        //TODO
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
    StringBuf_<256> enabledBuf;
    if (enabledStr == NULL || enabledStr.GetLength() == 0) {
        enabledStr = id;
        enabledBuf.Append(enabledStr);
    } else {
        //enabledStr = enabledStr + ':' + id;
        enabledBuf.Append(enabledStr);
        enabledBuf.Append(':');
        enabledBuf.Append(id);
    }

    //TODO
    assert(0);
    // Settings.Secure.putString(mContext.getContentResolver(),
    //         Settings.Secure.ENABLED_INPUT_METHODS, enabledBuf.GetPayload());

    // Previous state was disabled.
    return FALSE;
}

Boolean CInputMethodManagerService::ShowCurrentInputLocked(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
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
        if (IBinder::Probe(mCurMethod) != NULL) {
            //Send msg.
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodStub*,Int32,IResultReceiver*);
            pHandlerFunc = &CInputMethodManagerService::HandleShowSoftInput;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32(GetImeShowFlags());
            params->WriteInterfacePtr(mCurMethod.Get());
            params->WriteInterfacePtr(resultReceiver);

            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            //Handle msg.
            HandleShowSoftInput(mCurMethod, GetImeShowFlags(), resultReceiver);
        }

        mInputShown = TRUE;
        res = TRUE;
    } else if (mHaveConnection && SystemClock::UptimeMillis()
            < (mLastBindTime + TIME_TO_RECONNECT)) {
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

Boolean CInputMethodManagerService::HideCurrentInputLocked(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
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
            //Send msg.
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodStub*, IResultReceiver*);
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
            //Handle msg.
            HandleHideSoftInput(mCurMethod, resultReceiver);
        }

        res = TRUE;
    } else {
        res = FALSE;
    }

    mInputShown = FALSE;
    mShowRequested = FALSE;
    mShowExplicitlyRequested = FALSE;
    mShowForced = FALSE;
    return res;
}

void CInputMethodManagerService::SetEnabledSessionInMainThread(
    /* [in] */ SessionState* session)
{
    if (mEnabledSession.Get() != session) {
        if (mEnabledSession != NULL) {
                // if (DEBUG) Slog.v(TAG, "Disabling: " + mEnabledSession);
            mEnabledSession->mMethod->SetSessionEnabled(
                    mEnabledSession->mSession, FALSE);
            // } catch (RemoteException e) {
            // }
        }

        mEnabledSession = session;

        // if (DEBUG) Slog.v(TAG, "Enabling: " + mEnabledSession);
        session->mMethod->SetSessionEnabled(
                session->mSession, TRUE);
        // } catch (RemoteException e) {
        // }
    }
}

void CInputMethodManagerService::BuildInputMethodListLocked(
    /* [in] */ List<AutoPtr<IInputMethodInfo> >& InfoList,
    /* [in] */ HashMap<String, AutoPtr<IInputMethodInfo> >& InfoMap)
{
    InfoList.Clear();
    InfoMap.Clear();
    assert(0);

    //TODO
    // PackageManager pm = mContext->GetCapsuleManager();
    // final Configuration config = mContext.getResources().getConfiguration();
    // final Boolean haveHardKeyboard = config.keyboard == Configuration.KEYBOARD_QWERTY;
    // String disabledSysImes = Settings.Secure.getString(mContext.getContentResolver(),
    //         Secure.DISABLED_SYSTEM_INPUT_METHODS);
    // if (disabledSysImes == NULL) disabledSysImes = "";

    // List<ResolveInfo> services = pm.queryIntentServices(
    //         new Intent(InputMethod_SERVICE_INTERFACE),
    //         CapsuleManager_GET_META_DATA);

    // for (Int32 i = 0; i < services.size(); ++i) {
    //     ResolveInfo ri = services.get(i);
    //     ServiceInfo si = ri.serviceInfo;
    //     ComponentName compName = new ComponentName(si.packageName, si.name);
    //     if (!android.Manifest.permission.BIND_INPUT_METHOD.Equals(
    //             si.permission)) {
    //         Slog.w(TAG, "Skipping input method " + compName
    //                 + ": it does not require the permission "
    //                 + android.Manifest.permission.BIND_INPUT_METHOD);
    //         continue;
    //     }

    //     if (DEBUG) Slog.d(TAG, "Checking " + compName);

    //     try {
    //         IInputMethodInfo p = new IInputMethodInfo(mContext, ri);
    //         list.add(p);
    //         final String id = p.getId();
    //         map.put(id, p);

    //         // System IMEs are enabled by default, unless there's a hard keyboard
    //         // and the system IME was explicitly disabled
    //         if (isSystemIme(p) && (!haveHardKeyboard || disabledSysImes.indexOf(id) < 0)) {
    //             setInputMethodEnabledLocked(id, TRUE);
    //         }

    //         if (DEBUG) {
    //             Slog.d(TAG, "Found a third-party input method " + p);
    //         }

    //     } catch (XmlPullParserException e) {
    //         Slog.w(TAG, "Unable to load input method " + compName, e);
    //     } catch (IOException e) {
    //         Slog.w(TAG, "Unable to load input method " + compName, e);
    //     }
    // }

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
    //TODO
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

// void CInputMethodManagerService::ExecuteOrSendMessage(
//     /* [in] */ IInterface* target,
//     /* [in] */ Message msg)
// {
//      if (IBinder::Probe(target) != NULL) {
//          mCaller.sendMessage(msg);
//      } else {
//          handleMessage(msg);
//          msg.recycle();
//      }
// }

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
                    //Send msg.
                    void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodStub*);
                    pHandlerFunc = &CInputMethodManagerService::HandleUnbindInput;
                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    params->WriteInterfacePtr(mCurMethod.Get());

                    mCaller->PostCppCallbackDelayed(
                        (Handle32)this, *(Handle32*)&pHandlerFunc,
                        params, 0, 0);
                }
                else {
                    //Handle msg.
                    HandleUnbindInput(mCurMethod);
                }
            }
        }

        // executeOrSendMessage(mCurClient.client, mCaller.obtainMessageIO(
        //         MSG_UNBIND_METHOD, mCurSeq, mCurClient.client));
        if (IBinder::Probe(mCurClient->mClient) != NULL) {
            //Send msg.
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodClientStub*,Int32);
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
            //Handle msg.
            HandleUnbindMethod(mCurClient->mClient, mCurSeq);
        }

        mCurClient->mSessionRequested = FALSE;

        // Call setActive(FALSE) on the old client
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
    } else if (mShowExplicitlyRequested) {
        flags |= InputMethod_SHOW_EXPLICIT;
    }

    return flags;
}

Int32 CInputMethodManagerService::GetAppShowFlags()
{
    Int32 flags = 0;
    if (mShowForced) {
        flags |= InputMethodManager_SHOW_FORCED;
    } else if (!mShowExplicitlyRequested) {
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
            //Send msg.
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodStub*,IInputBinding*);
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
            //Handle msg.
            HandleBindInput(mCurMethod, mCurClient->mBinding);
        }

        mBoundToMethod = TRUE;
    }

    //AutoPtr<SessionState> session = mCurClient->mCurSession;
    if (initial) {
        // executeOrSendMessage(session.method, mCaller.obtainMessageOOO(
        //         MSG_START_INPUT, session, mCurInputContext, mCurAttribute));
        if (IBinder::Probe(mCurClient->mCurSession->mMethod) != NULL) {
            //Send msg.
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(SessionState*,
                    IInputContextStub*,IEditorInfo*);

            pHandlerFunc = &CInputMethodManagerService::HandleStartInput;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32Ptr(*(Handle32*)mCurClient->mCurSession.Get());
            params->WriteInterfacePtr(mCurInputContext.Get());
            params->WriteInterfacePtr(mCurAttribute.Get());

            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            //Handle msg.
            HandleStartInput(mCurClient->mCurSession, mCurInputContext, mCurAttribute);
        }

    } else {
        // executeOrSendMessage(session.method, mCaller.obtainMessageOOO(
        //         MSG_RESTART_INPUT, session, mCurInputContext, mCurAttribute));

        if (IBinder::Probe(mCurClient->mCurSession->mMethod) != NULL) {
            //Send msg.
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(SessionState*,
                    IInputContextStub*,IEditorInfo*);

            pHandlerFunc = &CInputMethodManagerService::HandleRestartInput;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32Ptr(*(Handle32*)mCurClient->mCurSession.Get());
            params->WriteInterfacePtr(mCurInputContext.Get());
            params->WriteInterfacePtr(mCurAttribute.Get());

            mCaller->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                params, 0, 0);
        }
        else {
            //Handle msg.
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
                    (IInputBindResult**) &retObj);

        return retObj;
    }

    return NULL;
}

AutoPtr<IInputBindResult> CInputMethodManagerService::StartInputLocked(
    /* [in] */ IInputMethodClientStub* client,
    /* [in] */ IInputContextStub* inputContext,
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean initial,
    /* [in] */ Boolean needResult)
{
    // If no method is currently selected, do nothing.
    if (mCurMethodId == NULL) {
        return mNoBinding;
    }

    AutoPtr<ClientState> cs = mClients[IBinder::Probe(client)];
    if (cs == NULL) {
        assert(0);
        // throw new IllegalArgumentException("unknown client "
        //         + client.asBinder());
    }

    Boolean ret = FALSE;
    mIWindowManager->InputMethodClientHasFocus(cs->mClient, &ret);
    if (!ret) {
        // Check with the window manager to make sure this client actually
        // has a window with focus.  If not, reject.  This is thread safe
        // because if the focus changes some time before or after, the
        // next client receiving focus that has any interest in input will
        // be calling through here after that change happens.
        // Slog.w(TAG, "Starting input on non-focused client " + cs.client
        //         + " (uid=" + cs.uid + " pid=" + cs.pid + ")");
        return NULL;
    }

    if (mCurClient != cs) {
        // If the client is changing, we need to switch over to the new
        // one.
        UnbindCurrentClientLocked();
        // if (DEBUG) Slog.v(TAG, "switching to client: client = "
        //         + cs.client.asBinder());

        // If the screen is on, inform the new client it is active
        if (mScreenOn) {
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
    if (mCurId != NULL && mCurId.Equals(mCurMethodId)) {
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
                        //Send msg.
                        void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodStub*, IInputMethodCallbackStub*);
                        pHandlerFunc = &CInputMethodManagerService::HandleCreateSession;
                        AutoPtr<IParcel> params;
                        CCallbackParcel::New((IParcel**)&params);
                        params->WriteInterfacePtr(mCurMethod.Get());
                        params->WriteInterfacePtr(new MethodCallback(mCurMethod, this));

                        mCaller->PostCppCallbackDelayed(
                            (Handle32)this, *(Handle32*)&pHandlerFunc,
                            params, 0, 0);
                    }
                    else {
                        //Handle msg.
                        HandleCreateSession(mCurMethod, new MethodCallback(mCurMethod, this));
                    }
                }

                // Return to client, and we will get back with it when
                // we have had a session made for it.
                AutoPtr<IInputBindResult> retObj;
                CInputBindResult::New(NULL, mCurId, mCurSeq, (IInputBindResult**) &retObj);
                return retObj;
            } else if (SystemClock::UptimeMillis()
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
            } else {
                // EventLog.writeEvent(EventLogTags.IMF_FORCE_RECONNECT_IME,
                //         mCurMethodId, SystemClock::UptimeMillis()-mLastBindTime, 0);
            }
        }
    }

    return StartInputInnerLocked();
}

AutoPtr<IInputBindResult> CInputMethodManagerService::StartInputInnerLocked()
{
    if (mCurMethodId == NULL) {
        return mNoBinding;
    }

    if (!mSystemReady) {
        // If the system is not yet ready, we shouldn't be running third
        // party code.
        AutoPtr<IInputBindResult> retObj;
        CInputBindResult::New(NULL, mCurMethodId, mCurSeq, (IInputBindResult**) &retObj);
        return retObj;
    }

    AutoPtr<IInputMethodInfo> info = mMethodMap[mCurMethodId];
    if (info == NULL) {
        assert(0);
        //throw new IllegalArgumentException("Unknown id: " + mCurMethodId);
    }

    UnbindCurrentMethodLocked(FALSE);

    CIntent::New(String(InputMethod_SERVICE_INTERFACE), (IIntent**) &mCurIntent);

    AutoPtr<IComponentName> tmpName;
    info->GetComponent((IComponentName**) &tmpName);

    mCurIntent->SetComponent(tmpName);
    mCurIntent->PutInt32Extra(String(Intent_EXTRA_CLIENT_LABEL),
            0x0104036d /*com.android.internal.R.string.input_method_binding_label*/);

    //TODO
    assert(0);
    // mCurIntent.putExtra(Intent_EXTRA_CLIENT_INTENT, PendingIntent.getActivity(
    //         mContext, 0, new Intent(Settings.ACTION_INPUT_METHOD_SETTINGS), 0));

    Boolean succeeded = FALSE;
    mContext->BindService(mCurIntent, (IServiceConnection*)this->Probe(EIID_IServiceConnection),
            Context_BIND_AUTO_CREATE, &succeeded);

    if (succeeded) {
        mLastBindTime = SystemClock::UptimeMillis();
        mHaveConnection = TRUE;
        info->GetId(&mCurId);

        //TODO
        assert(0);
        //mCurToken = new Binder();

        // if (DEBUG) Slog.v(TAG, "Adding window token: " + mCurToken);
        mIWindowManager->AddWindowToken(mCurToken,
                WindowManagerLayoutParams_TYPE_INPUT_METHOD);
        // } catch (RemoteException e) {
        // }

        AutoPtr<IInputBindResult> retObj;
        CInputBindResult::New(NULL, mCurId, mCurSeq, (IInputBindResult**) &retObj);
        return retObj;
    } else {
        mCurIntent = NULL;
        // Slog.w(TAG, "Failure connecting to input method service: "
        //         + mCurIntent);
    }

    return NULL;
}

void CInputMethodManagerService::OnSessionCreated(
    /* [in] */ IInputMethodStub* method,
    /* [in] */ IInputMethodSessionStub* session)
{
    Mutex::Autolock lock(mMethodMapLock);
    if (mCurMethod != NULL && method != NULL
            && IBinder::Probe(mCurMethod) == IBinder::Probe(method))
    {
        if (mCurClient != NULL) {
            mCurClient->mCurSession = new SessionState(mCurClient,
                    method, session);

            mCurClient->mSessionRequested = FALSE;
            AutoPtr<IInputBindResult> res = AttachNewInputLocked(TRUE, TRUE);
            assert(res != NULL);

            AutoPtr<IInputMethodSessionStub> stub;
            res->GetIIMSessionStub((IInputMethodSessionStub**) &stub);

            if (stub != NULL) {
                // executeOrSendMessage(mCurClient.client, mCaller.obtainMessageOO(
                //         MSG_BIND_METHOD, mCurClient.client, res));

                if (IBinder::Probe(mCurClient->mClient) != NULL) {
                    //Send msg.
                    void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodClientStub*,
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
                    //Handle msg.
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
            //Send msg.
            void (STDCALL CInputMethodManagerService::*pHandlerFunc)(IInputMethodClientStub*,
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
            //Handle msg.
            HandleUnbindMethod(mCurClient->mClient, mCurSeq);
        }
    }
}

void CInputMethodManagerService::FinishSession(
    /* [in] */ SessionState* sessionState)
{
    if (sessionState != NULL && sessionState->mSession != NULL) {
        sessionState->mSession->FinishSession();
        // } catch (RemoteException e) {
        //     Slog.w(TAG, "Session failed to close due to remote exception", e);
        // }
    }
}

void CInputMethodManagerService::ClearCurMethodLocked()
{
    if (mCurMethod != NULL) {
        assert(0);

        //TODO
        // for (ClientState cs : mClients.values()) {
        //     cs.sessionRequested = FALSE;
        //     finishSession(cs.curSession);
        //     cs.curSession = NULL;
        // }

        FinishSession(mEnabledSession);
        mEnabledSession = NULL;
        mCurMethod = NULL;
    }

    mStatusBar->SetIconVisibility(String("ime"), FALSE);
}

Boolean CInputMethodManagerService::IsSystemIme(
    /* [in] */ IInputMethodInfo* inputMethod)
{
    assert(inputMethod != NULL);

    AutoPtr<IServiceInfo> serInfo;
    inputMethod->GetServiceInfo((IServiceInfo**) &serInfo);
    assert(serInfo != NULL);

    AutoPtr<IApplicationInfo> appInfo;
    serInfo->GetApplicationInfo((IApplicationInfo**) &appInfo);
    assert(appInfo != NULL);

    Int32 flag = 0;
    appInfo->GetFlags(&flag);

    return (flag & ApplicationInfo_FLAG_SYSTEM) != 0;
}

Boolean CInputMethodManagerService::ChooseNewDefaultIMELocked()
{
    AutoPtr<IObjectContainer> enabled;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&enabled));
    GetEnabledInputMethodListLocked((IObjectContainer**)&enabled);

    Int32 i = 0;
    enabled->GetObjectCount(&i);
    if (i > 0) {
        // We'd prefer to fall back on a system IME, since that is safer.

        AutoPtr<IObjectEnumerator> enabledItem;
        enabled->GetObjectEnumerator((IObjectEnumerator**)&enabledItem);
        Boolean hasNext = FALSE;
        AutoPtr<IInputMethodInfo> methodInfo;
        AutoPtr<IServiceInfo> serInfo;
        while(enabledItem->MoveNext(&hasNext), hasNext) {
            enabledItem->Current((IInterface**)&methodInfo);
            methodInfo->GetServiceInfo((IServiceInfo**) &serInfo);
            assert(serInfo != NULL);

            AutoPtr<IApplicationInfo> appInfo;
            serInfo->GetApplicationInfo((IApplicationInfo**) &appInfo);
            assert(appInfo != NULL);

            Int32 flag = 0;
            appInfo->GetFlags(&flag);

            if ((flag & ApplicationInfo_FLAG_SYSTEM) != 0) {
                break;
            }
        }

        assert(0);
        //TODO
        // Settings.Secure.putString(mContext.getContentResolver(),
        //         Settings.Secure.DEFAULT_INPUT_METHOD,
        //         enabled.get(i).getId());

        return TRUE;
    }

    return FALSE;
}

void CInputMethodManagerService::HandleShowIMPicker()
{
    ShowInputMethodMenu();
}

void CInputMethodManagerService::HandleUnbindInput(
    /* [in] */ IInputMethodStub* inputMethod)
{
    inputMethod->UnbindInput();
}

void CInputMethodManagerService::HandleBindInput(
    /* [in] */ IInputMethodStub* inputMethod,
    /* [in] */ IInputBinding* binding)
{
    //((IInputMethodStub)args.arg1).bindInput((InputBinding)args.arg2);
    inputMethod->BindInput(binding);
}

void CInputMethodManagerService::HandleShowSoftInput(
    /* [in] */ IInputMethodStub* inputMethod,
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    inputMethod->ShowSoftInput(flags, resultReceiver);
}

void CInputMethodManagerService::HandleHideSoftInput(
    /* [in] */ IInputMethodStub* inputMethod,
    /* [in] */ IResultReceiver* resultReceiver)
{
    inputMethod->HideSoftInput(0, resultReceiver);
}

void CInputMethodManagerService::HandleAttachToken(
    /* [in] */ IInputMethodStub* inputMethod,
    /* [in] */ IBinder* binder)
{
    inputMethod->AttachToken(binder);
}

void CInputMethodManagerService::HandleCreateSession(
    /* [in] */ IInputMethodStub* inputMethod,
    /* [in] */ IInputMethodCallbackStub* methodcallback)
{
    inputMethod->CreateSession(methodcallback);
}

void CInputMethodManagerService::HandleStartInput(
    /* [in] */ SessionState* state,
    /* [in] */ IInputContextStub* inputContext,
    /* [in] */ IEditorInfo* info)
{
    assert(state != NULL && state->mMethod != NULL);
    SetEnabledSessionInMainThread(state);
    state->mMethod->StartInput(inputContext, info);
}

void CInputMethodManagerService::HandleRestartInput(
    /* [in] */ SessionState* state,
    /* [in] */ IInputContextStub* inputContext,
    /* [in] */ IEditorInfo* info)
{
    assert(state != NULL && state->mMethod != NULL);
    SetEnabledSessionInMainThread(state);
    state->mMethod->RestartInput(inputContext, info);
}

void CInputMethodManagerService::HandleUnbindMethod(
    /* [in] */ IInputMethodClientStub* inputMethod,
    /* [in] */ Int32 flags)
{
    inputMethod->OnUnbindMethod(flags);
}

void CInputMethodManagerService::HandleBindMethod(
    /* [in] */ IInputMethodClientStub* inputMethod,
    /* [in] */ IInputBindResult* bindRes)
{
    inputMethod->OnBindMethod(bindRes);
}
