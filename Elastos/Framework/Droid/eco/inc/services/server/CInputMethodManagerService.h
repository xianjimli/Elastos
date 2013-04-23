
#ifndef  __CINPUTMETHODMANAGERSERVICE_H__
#define  __CINPUTMETHODMANAGERSERVICE_H__

#include "ext/frameworkext.h"
#include "_CInputMethodManagerService.h"
#include "text/TextUtils.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/Mutex.h>


#ifdef _FRAMEWORK_CORE
#include "content/BroadcastReceiver.h"
#else
#include "BroadcastReceiver.h"
#endif

using namespace Elastos;
using namespace Elastos::Core;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash< AutoPtr<IBinder> >
{
    size_t operator()(AutoPtr<IBinder> name) const
    {
        assert(name != NULL);
        return (size_t)name.Get();
    }
};

template<> struct EqualTo< AutoPtr<IBinder> >
{
    Boolean operator()(const AutoPtr<IBinder>& x,
                         const AutoPtr<IBinder>& y) const
    {
        assert(x != NULL && y != NULL);
        return x.Get() == y.Get();
    }
};

_ELASTOS_NAMESPACE_END


/**
 * This class provides a system service that manages input methods.
 */
CarClass(CInputMethodManagerService)
{
    friend class CInputMethodManagerServiceMethodCallback;

private:
    class ClientState;

    class SessionState : public ElRefBase
    {
    public:
        SessionState(
            /* [in] */ ClientState* client,
            /* [in] */ IInputMethod* method,
            /* [in] */ IInputMethodSession* session);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

    public:
        AutoPtr<ClientState> mClient;
        AutoPtr<IInputMethod> mMethod;
        AutoPtr<IInputMethodSession> mSession;
    };

    class ClientState : public ElRefBase
    {
    public:
        ClientState(
            /* [in] */ IInputMethodClient* client,
            /* [in] */ IInputContext* inputContext,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 pid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

    public:
        AutoPtr<IInputMethodClient> mClient;
        AutoPtr<IInputContext> mInputContext;
        const Int32 mUid;
        const Int32 mPid;
        AutoPtr<IInputBinding> mBinding;

        Boolean mSessionRequested;
        AutoPtr<SessionState> mCurSession;
    };

    //TODO
    // class SettingsObserver extends ContentObserver {
    //     SettingsObserver(Handler handler) {
    //         super(handler);
    //         ContentResolver resolver = mContext.getContentResolver();
    //         resolver.registerContentObserver(Settings.Secure.getUriFor(
    //                 Settings.Secure.DEFAULT_INPUT_METHOD), false, this);
    //     }

    //     @Override public void onChange(Boolean selfChange) {
    //         synchronized (mMethodMap) {
    //             updateFromSettingsLocked();
    //         }
    //     }
    // };

    class ScreenOnOffReceiver : public BroadcastReceiver
    {
    public:
        ScreenOnOffReceiver(
            /* [in] */ CInputMethodManagerService* host);

    protected:
        //@Override
        virtual CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        CInputMethodManagerService* mHost;
    };

    // class MyPackageMonitor extends PackageMonitor {

    //     @Override
    //     public Boolean onHandleForceStop(Intent intent, String[] packages, Int32 uid, Boolean doit) {
    //         synchronized (mMethodMap) {
    //             String curInputMethodId = Settings.Secure.getString(mContext
    //                     .getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD);
    //             const Int32 N = mMethodList.size();
    //             if (curInputMethodId != NULL) {
    //                 for (Int32 i=0; i<N; i++) {
    //                     IInputMethodInfo imi = mMethodList.get(i);
    //                     if (imi.getId().equals(curInputMethodId)) {
    //                         for (String pkg : packages) {
    //                             if (imi.getPackageName().equals(pkg)) {
    //                                 if (!doit) {
    //                                     return true;
    //                                 }

    //                                 Settings.Secure.putString(mContext.getContentResolver(),
    //                                         Settings.Secure.DEFAULT_INPUT_METHOD, "");
    //                                 chooseNewDefaultIMELocked();
    //                                 return true;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //         return false;
    //     }

    //     @Override
    //     public void onSomePackagesChanged() {
    //         synchronized (mMethodMap) {
    //             IInputMethodInfo curIm = NULL;
    //             String curInputMethodId = Settings.Secure.getString(mContext
    //                     .getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD);
    //             const Int32 N = mMethodList.size();
    //             if (curInputMethodId != NULL) {
    //                 for (Int32 i=0; i<N; i++) {
    //                     IInputMethodInfo imi = mMethodList.get(i);
    //                     if (imi.getId().equals(curInputMethodId)) {
    //                         curIm = imi;
    //                     }
    //                     Int32 change = isPackageDisappearing(imi.getPackageName());
    //                     if (change == PACKAGE_TEMPORARY_CHANGE
    //                             || change == PACKAGE_PERMANENT_CHANGE) {
    //                         Slog.i(TAG, "Input method uninstalled, disabling: "
    //                                 + imi.getComponent());
    //                         setInputMethodEnabledLocked(imi.getId(), false);
    //                     }
    //                 }
    //             }

    //             buildInputMethodListLocked(mMethodList, mMethodMap);

    //             Boolean changed = false;

    //             if (curIm != NULL) {
    //                 Int32 change = isPackageDisappearing(curIm.getPackageName());
    //                 if (change == PACKAGE_TEMPORARY_CHANGE
    //                         || change == PACKAGE_PERMANENT_CHANGE) {
    //                     ServiceInfo si = NULL;
    //                     try {
    //                         si = mContext.getPackageManager().getServiceInfo(
    //                                 curIm.getComponent(), 0);
    //                     } catch (PackageManager.NameNotFoundException ex) {
    //                     }
    //                     if (si == NULL) {
    //                         // Uh oh, current input method is no longer around!
    //                         // Pick another one...
    //                         Slog.i(TAG, "Current input method removed: " + curInputMethodId);
    //                         if (!chooseNewDefaultIMELocked()) {
    //                             changed = true;
    //                             curIm = NULL;
    //                             curInputMethodId = "";
    //                             Slog.i(TAG, "Unsetting current input method");
    //                             Settings.Secure.putString(mContext.getContentResolver(),
    //                                     Settings.Secure.DEFAULT_INPUT_METHOD,
    //                                     curInputMethodId);
    //                         }
    //                     }
    //                 }
    //             }

    //             if (curIm == NULL) {
    //                 // We currently don't have a default input method... is
    //                 // one now available?
    //                 changed = chooseNewDefaultIMELocked();
    //             }

    //             if (changed) {
    //                 updateFromSettingsLocked();
    //             }
    //         }
    //     }
    // };

public:
    CInputMethodManagerService();

    ~CInputMethodManagerService();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IStatusBarService* statusBar);

    CARAPI GetDescription(
            /* [out] */ String* description);

    CARAPI SystemReady();

    CARAPI GetInputMethodList(
        /* [out] */ IObjectContainer** infos);

    CARAPI GetEnabledInputMethodList(
        /* [out] */ IObjectContainer** list);

    CARAPI AddClient(
        /* [in] */ IInputMethodClient* client,
        /* [in] */ IInputContext* inputContext,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 pid);

    CARAPI RemoveClient(
        /* [in] */ IInputMethodClient* client);

    CARAPI StartInput(
        /* [in] */ IInputMethodClient* client,
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* attribute,
        /* [in] */ Boolean initial,
        /* [in] */ Boolean needResult,
        /* [out] */ IInputBindResult** res);

    CARAPI FinishInput(
        /* [in] */ IInputMethodClient* client);

    CARAPI OnServiceConnected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

    CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName* name);

    CARAPI UpdateStatusIcon(
        /* [in] */ IBinder* token,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 iconId);

    CARAPI ShowSoftInput(
        /* [in] */ IInputMethodClient* client,
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver,
        /* [out] */ Boolean* state);

    CARAPI HideSoftInput(
        /* [in] */ IInputMethodClient* client,
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver,
        /* [out] */ Boolean* state);

    CARAPI WindowGainedFocus(
        /* [in] */ IInputMethodClient* client,
        /* [in] */ IBinder* windowToken,
        /* [in] */ Boolean viewHasFocus,
        /* [in] */ Boolean isTextEditor,
        /* [in] */ Int32 softInputMode,
        /* [in] */ Boolean first,
        /* [in] */ Int32 windowFlags);

    CARAPI ShowInputMethodPickerFromClient(
        /* [in] */ IInputMethodClient* client);

    CARAPI SetInputMethod(
        /* [in] */ IBinder* token,
        /* [in] */ const String& id);

    CARAPI HideMySoftInput(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 flags);

    CARAPI ShowMySoftInput(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 flags);

    CARAPI SetInputMethodEnabled(
        /* [in] */ const String& id,
        /* [in] */ Boolean enabled,
        /* [out] */ Boolean* state);

protected:
    /* packaged */ CARAPI GetEnabledInputMethodListLocked(
        /* [out] */ IObjectContainer** list);

    /* packaged */ CARAPI_(void) UnbindCurrentClientLocked();

    /* packaged */ CARAPI_(AutoPtr<IInputBindResult>) AttachNewInputLocked(
        /* [in] */ Boolean initial,
        /* [in] */ Boolean needResult);

    /* packaged */ CARAPI_(AutoPtr<IInputBindResult>) StartInputLocked(
        /* [in] */ IInputMethodClient* client,
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* attribute,
        /* [in] */ Boolean initial,
        /* [in] */ Boolean needResult);

    /* packaged */ CARAPI_(AutoPtr<IInputBindResult>) StartInputInnerLocked();

    /* packaged */ CARAPI_(void) OnSessionCreated(
        /* [in] */ IInputMethod* method,
        /* [in] */ IInputMethodSession* session);

    /* packaged */ CARAPI_(void) UnbindCurrentMethodLocked(
        /* [in] */ Boolean reportToClient);

    /* packaged */ CARAPI_(void) ClearCurMethodLocked();

    /* packaged */ CARAPI_(void) UpdateFromSettingsLocked();

    /* packaged */ CARAPI SetInputMethodLocked(
        /* [in] */ const String& id);

    /* packaged */ CARAPI_(Boolean) ShowCurrentInputLocked(
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver);

    /* packaged */ CARAPI_(Boolean) HideCurrentInputLocked(
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver);

    /* packaged */ CARAPI_(void) SetEnabledSessionInMainThread(
        /* [in] */ SessionState* session);

    /* packaged */ CARAPI_(void) BuildInputMethodListLocked(
        /* [in] */ List<AutoPtr<IInputMethodInfo> >& list,
        /* [in] */ HashMap<String, AutoPtr<IInputMethodInfo> >& map);

    /* packaged */ CARAPI_(void) ShowInputMethodMenu();

    /* packaged */ CARAPI_(void) HideInputMethodMenu();

    /* packaged */ CARAPI_(void) HideInputMethodMenuLocked();

    /* packaged */ CARAPI_(Boolean) SetInputMethodEnabledLocked(
        /* [in] */ const String& id,
        /* [in] */ Boolean enabled);

private:
    // CARAPI_(void) ExecuteOrSendMessage(
    //     /* [in] */ IInterface* target,
    //     /* [in] */ Message msg);

    CARAPI_(Int32) GetImeShowFlags();

    CARAPI_(Int32) GetAppShowFlags();

    CARAPI_(void) FinishSession(
        /* [in] */ SessionState* sessionState);

    CARAPI_(Boolean) IsSystemIme(
        /* [in] */ IInputMethodInfo* inputMethod);

    CARAPI_(Boolean) ChooseNewDefaultIMELocked();

    // Handle MSG_SHOW_IM_PICKER
    CARAPI_(void) HandleShowIMPicker();

    // Handle MSG_UNBIND_INPUT
    CARAPI_(void) HandleUnbindInput(
        /* [in] */ IInputMethod* inputMethod);

    // Handle MSG_BIND_INPUT
    CARAPI_(void) HandleBindInput(
        /* [in] */ IInputMethod* inputMethod,
        /* [in] */ IInputBinding* binding);

    //Handle MSG_SHOW_SOFT_INPUT
    CARAPI_(void) HandleShowSoftInput(
        /* [in] */ IInputMethod* inputMethod,
        /* [in] */ Int32 flags,
        /* [in] */ ILocalResultReceiver* resultReceiver);

    //Handle MSG_HIDE_SOFT_INPUT
    CARAPI_(void) HandleHideSoftInput(
        /* [in] */ IInputMethod* inputMethod,
        /* [in] */ ILocalResultReceiver* resultReceiver);

    //Handle MSG_ATTACH_TOKEN
    CARAPI_(void) HandleAttachToken(
        /* [in] */ IInputMethod* inputMethod,
        /* [in] */ IBinder* binder);

    //Handle MSG_CREATE_SESSION
    CARAPI_(void) HandleCreateSession(
        /* [in] */ IInputMethod* inputMethod,
        /* [in] */ IInputMethodCallback* methodcallback);

    //Handle MSG_START_INPUT
    CARAPI_(void) HandleStartInput(
        /* [in] */ SessionState* state,
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* info);

    //Handle MSG_RESTART_INPUT
    CARAPI_(void) HandleRestartInput(
        /* [in] */ SessionState* state,
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* info);

    //Handle MSG_UNBIND_METHOD
    CARAPI_(void) HandleUnbindMethod(
        /* [in] */ IInputMethodClient* inputMethod,
        /* [in] */ Int32 flags);

    //Handle MSG_BIND_METHOD
    CARAPI_(void) HandleBindMethod(
        /* [in] */ IInputMethodClient* inputMethod,
        /* [in] */ IInputBindResult* bindRes);

private:
    // static final Boolean DEBUG = false;
    static CString TAG;
    //TODO:
    static CString ENABLED_STR;
    static CString DEFAULT_INPUT_METHOD;

    static const Int32 MSG_SHOW_IM_PICKER = 1;

    static const Int32 MSG_UNBIND_INPUT = 1000;
    static const Int32 MSG_BIND_INPUT = 1010;
    static const Int32 MSG_SHOW_SOFT_INPUT = 1020;
    static const Int32 MSG_HIDE_SOFT_INPUT = 1030;
    static const Int32 MSG_ATTACH_TOKEN = 1040;
    static const Int32 MSG_CREATE_SESSION = 1050;

    static const Int32 MSG_START_INPUT = 2000;
    static const Int32 MSG_RESTART_INPUT = 2010;

    static const Int32 MSG_UNBIND_METHOD = 3000;
    static const Int32 MSG_BIND_METHOD = 3010;

    static const Int64 TIME_TO_RECONNECT = 10*1000;

    AutoPtr<IContext> mContext;
    AutoPtr<IApartment> mHandler;

    //final SettingsObserver mSettingsObserver;
    //final StatusBarManagerService mStatusBar;
    AutoPtr<IStatusBarService> mStatusBar;
    AutoPtr<IWindowManager> mIWindowManager;
    //TODO
    //final HandlerCaller mCaller;
    AutoPtr<IApartment> mCaller;

    AutoPtr<IInputBindResult> mNoBinding;

    // All known input methods.  mMethodMap also serves as the global
    // lock for this class.
    List< AutoPtr<IInputMethodInfo> > mMethodList;
    HashMap<String, AutoPtr<IInputMethodInfo> > mMethodMap;
    Mutex mMethodMapLock;

    TextUtils::SimpleStringSplitter* mStringColonSplitter;

    HashMap<AutoPtr<IBinder>, AutoPtr<ClientState> > mClients;

    /**
     * Set once the system is ready to run third party code.
     */
    Boolean mSystemReady;

    /**
     * Id of the currently selected input method.
     */
    String mCurMethodId;

    /**
     * The current binding sequence number, incremented every time there is
     * a new bind performed.
     */
    Int32 mCurSeq;

    /**
     * The client that is currently bound to an input method.
     */
    AutoPtr<ClientState> mCurClient;

    /**
     * The last window token that gained focus.
     */
    AutoPtr<IBinder> mCurFocusedWindow;

    /**
     * The input context last provided by the current client.
     */
    AutoPtr<IInputContext> mCurInputContext;

    /**
     * The attributes last provided by the current client.
     */
    AutoPtr<IEditorInfo> mCurAttribute;

    /**
     * The input method ID of the input method service that we are currently
     * connected to or in the process of connecting to.
     */
    String mCurId;

    /**
     * Set to true if our ServiceConnection is currently actively bound to
     * a service (whether or not we have gotten its IBinder back yet).
     */
    Boolean mHaveConnection;

    /**
     * Set if the client has asked for the input method to be shown.
     */
    Boolean mShowRequested;

    /**
     * Set if we were explicitly told to show the input method.
     */
    Boolean mShowExplicitlyRequested;

    /**
     * Set if we were forced to be shown.
     */
    Boolean mShowForced;

    /**
     * Set if we last told the input method to show itself.
     */
    Boolean mInputShown;

    /**
     * The Intent used to connect to the current input method.
     */
    AutoPtr<IIntent> mCurIntent;

    /**
     * The token we have made for the currently active input method, to
     * identify it in the future.
     */
    AutoPtr<IBinder> mCurToken;

    /**
     * If non-NULL, this is the input method service we are currently connected
     * to.
     */
    AutoPtr<IInputMethod> mCurMethod;

    /**
     * Time that we last initiated a bind to the input method, to determine
     * if we should try to disconnect and reconnect to it.
     */
    Int64 mLastBindTime;

    /**
     * Have we called mCurMethod.bindInput()?
     */
    Boolean mBoundToMethod;

    /**
     * Currently enabled session.  Only touched by service thread, not
     * protected by a lock.
     */
    AutoPtr<SessionState> mEnabledSession;

    /**
     * True if the screen is on.  The value is true initially.
     */
    Boolean mScreenOn;

    AutoPtr<IAlertDialogBuilder> mDialogBuilder;
    AutoPtr<IAlertDialog> mSwitchingDialog;

    ArrayOf< AutoPtr<IInputMethodInfo> >* mIms;
    ArrayOf< AutoPtr<ICharSequence> >* mItems;

    friend class ScreenOnOffReceiver;
};

#endif  //__CINPUTMETHODMANAGERSERVICE_H__
