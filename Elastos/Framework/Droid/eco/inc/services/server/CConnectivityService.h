
#ifndef __CCONNECTIVITYSERVICE_H__
#define __CCONNECTIVITYSERVICE_H__

#include <ext/frameworkext.h>
#include "_CConnectivityService.h"
#include <Elastos.Framework.Core.h>
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Set.h>
#include <elastos/ElRefBase.h>
#include <elastos/Thread.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

/*
import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.ContentResolver;
import android.content.ContextWrapper;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.content.res.Resources.NotFoundException;
import android.net.ConnectivityManager;
import android.net.IConnectivityManager;
import android.net.MobileDataStateTracker;
import android.net.NetworkInfo;
import android.net.NetworkStateTracker;
import android.net.wifi.WifiStateTracker;
import android.net.wimax.WimaxManagerConstants;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemProperties;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.EventLog;
import android.util.Slog;
import com.android.Int32ernal.telephony.Phone;
import com.android.server.connectivity.Tethering;
import dalvik.system.DexClassLoader;
import java.io.FileDescriptor;
import java.io.PrInt32Writer;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.GregorianCalendar;
import java.util.List;
*/


/**
 * @hide
 */
//public class ConnectivityService extends IConnectivityManager.Stub {
CarClass(CConnectivityService)
{
public:
    class StringUtil
    {
    public:
        static CARAPI_(List<String> *) Split(
            /* [in] */ const String & str,
            /* [in] */ const String & delimeter);

        static CARAPI_(AutoPtr<ICharSequence>) StringToCharSeuence(
            /* [in] */ const String & str);
    };

    //private static
    /**
     * Class for holding settings read from resources.
     */
    class NetworkAttributes
    {
    public:
        String mName;
        Int32 mType;
        Int32 mRadio;
        Int32 mPriority;
        NetworkInfoState mLastState;

        NetworkAttributes(
            /* [in] */ const String& init);

        CARAPI_(Boolean) IsDefault();

    };

    //private static
    class RadioAttributes
    {
    public:
        Int32 mSimultaneity;
        Int32 mType;

        RadioAttributes(
            /* [in] */ const String& init);
    };

    //private static
    class ConnectivityThread : public ElRefBase, public Thread, public IThread
    {
    public:
        CARAPI constructor(
            /* [in] */ IContext *context);

        ConnectivityThread(
            /* [in] */ IContext *context);

        virtual ~ConnectivityThread();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *object,
            /* [out] */ InterfaceID *iID);

        CARAPI CheckAccess();

        CARAPI CountStackFrames(
            /* [out] */ Int32* number);

        CARAPI Destroy();

        CARAPI GetId(
            /* [out] */ Int64* id);

        CARAPI GetName(
            /* [out] */ String* name);

        CARAPI GetPriority(
            /* [out] */ Int32* priority);

        CARAPI GetState(
            /* [out] */ ThreadState* state);

        CARAPI GetThreadGroup(
            /* [out] */ IThreadGroup** group);

        CARAPI Interrupt();

        CARAPI IsAlive(
            /* [out] */ Boolean* isAlive);

        CARAPI IsDaemon(
            /* [out] */ Boolean* isDaemon);

        CARAPI IsInterrupted(
            /* [out] */ Boolean* isInterrupted);

        CARAPI Join();

        CARAPI JoinEx(
            /* [in] */ Int64 millis);

        CARAPI JoinEx2(
            /* [in] */ Int64 millis,
            /* [in] */ Int32 nanos);

        CARAPI Resume();

        CARAPI SetDaemon(
            /* [in] */ Boolean isDaemon);

        CARAPI SetName(
            /* [in] */ const String& threadName);

        CARAPI SetPriority(
            /* [in] */ Int32 priority);

        CARAPI Start();

        CARAPI Stop();

        CARAPI Suspend();

        CARAPI Wait(
            /* [in] */ Int64 time,
            /* [in] */ Int32 frac);

        CARAPI Detach();

        CARAPI Lock();

        CARAPI Unlock();

        CARAPI Notify();

        CARAPI NotifyAll();

        CARAPI Wait();

        CARAPI WaitEx(
            /* [in] */ Int64 millis);

        CARAPI WaitEx2(
            /* [in] */ Int64 millis,
            /* [in] */ Int32 nanos);

        //@Override
        CARAPI Run();

        static CARAPI_(IConnectivityService *) GetServiceInstance(
            /* [in] */ IContext* context);

    private:
        AutoPtr<IContext> mContext;

        //static Mutex * mMutex;

        //for IThread
        Mutex mLock;

        CARAPI_(Mutex*) GetSelfLock();
    };

    /**
     * Used to notice when the calling process dies so we can self-expire
     *
     * Also used to know if the process has cleaned up after itself when
     * our auto-expire timer goes off.  The timer has a link to an object.
     *
     */
    //private
    class FeatureUser // : public IBinder.DeathRecipient
    {
    public:
        Int32 mNetworkType;
        String mFeature;
        IBinder* mBinder;
        Int32 mPid;
        Int32 mUid;
        Int64 mCreateTime;

        FeatureUser(
            /* [in] */ Int32 type,
            /* [in] */ const String& feature,
            /* [in] */ IBinder *binder,
            /* [in] */ CConnectivityService *connectivityService);

        CARAPI_(void) UnlinkDeathRecipient();

        CARAPI_(void) BinderDied();

        CARAPI_(void) Expire();

        CARAPI_(String) ToString();

    protected:
        CConnectivityService *mConnectivityService;
    };

    // must be stateless - things change under us.
    class MyHandler
    {
    public:
        //@Override
        CARAPI HandleMessage(
            /* [in] */ IInterface * obj,
            /* [in] */ Int32 what,
            /* [in] */ Int32 arg1,
            /* [in] */ Int32 arg2);

    };

    static CARAPI_(IConnectivityService *) GetServiceInstance(
        /* [in] */ IContext* context);

    static CARAPI_(IConnectivityService *) GetInstance(
        /* [in] */ IContext* context);

    CConnectivityService();

    virtual ~CConnectivityService();

    CARAPI constructor(
        /* [in] */ IContext * context);

    /**
     * Sets the preferred network.
     * @param preference the new preference
     */
    CARAPI SetNetworkPreference(
        /* [in] */ Int32 preference);

    CARAPI GetNetworkPreference(
        /* [out] */ Int32* preference);

    /**
     * Return NetworkInfo for the active (i.e., connected) network Int32erface.
     * It is assumed that at most one network is active at a time. If more
     * than one is active, it is indeterminate which will be returned.
     * @return the info for the active network, or {@code null} if none is
     * active
     */
    CARAPI GetActiveNetworkInfo(
        /* [out] */ INetworkInfo** networkInfo);

    CARAPI GetNetworkInfo(
        /* [in] */ Int32 networkType,
        /* [out] */ INetworkInfo** networkInfo);

    CARAPI GetAllNetworkInfo(
        /* [out] */ IObjectContainer** networkInfo);

    CARAPI SetRadios(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    CARAPI SetRadio(
        /* [in] */ Int32 networkType,
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    // javadoc from Int32erface
    CARAPI StartUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature,
        /* [in] */ IBinder* binder,
        /* [out] */ Int32* outcome);

    // javadoc from Int32erface
    CARAPI StopUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature,
        /* [out] */ Int32* outcome);

    /**
     * Ensure that a network route exists to deliver traffic to the specified
     * host via the specified network interface.
     * @param networkType the type of the network over which traffic to the
     * specified host is to be routed
     * @param hostAddress the IP address of the host to which the route is
     * desired
     * @return {@code TRUE} on success, {@code FALSE} on failure
     */
    CARAPI RequestRouteToHost(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 hostAddress,
        /* [out] */ Boolean* result);

    /**
     * @see ConnectivityManager#getBackgroundDataSetting()
     */
    CARAPI GetBackgroundDataSetting(
        /* [out] */ Boolean* isAllowed);

    /**
     * @see ConnectivityManager#setBackgroundDataSetting(Boolean)
     */
    CARAPI SetBackgroundDataSetting(
        /* [in] */ Boolean allowBackgroundData);

    /**
     * @see ConnectivityManager#getMobileDataEnabled()
     */
    CARAPI GetMobileDataEnabled(
        /* [out] */ Boolean* result);

    /**
     * @see ConnectivityManager#setMobileDataEnabled(Boolean)
     */
    CARAPI SetMobileDataEnabled(
        /* [in] */ Boolean enabled);

    CARAPI SystemReady();

    // javadoc from Int32erface
    CARAPI Tether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

    // javadoc from Int32erface
    CARAPI Untether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

    // javadoc from Int32erface
    CARAPI GetLastTetherError(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

    // TODO - proper iface API for selection by property, inspection, etc
    CARAPI GetTetherableUsbRegexs(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI GetTetherableWifiRegexs(
        /* [out, callee] */ ArrayOf<String>** result);

    // TODO - move iface listing, queries, etc to new module
    // javadoc from Int32erface
    CARAPI GetTetherableIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI GetTetheredIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI GetTetheringErroredIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    // if ro.tether.denied = TRUE we default to no tethering
    // gservices could set the secure setting to 1 though to enable it on a build where it
    // had previously been turned off.
    CARAPI IsTetheringSupported(
        /* [out] */ Boolean* result);

    // 100 percent is full good, 0 is full bad.
    CARAPI ReportInetCondition(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 percentage);

public:
    List<NetworkAttributes*> * mNetAttributes;

    Int32 mNetworksDefined;

    List<RadioAttributes*> * mRadioAttributes;

protected:
    //@Override
    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor * fd,
        /* [in] */ IPrintWriter * pw,
        /* [in] */ const ArrayOf<String> &args);

private:
    static const Boolean DBG;// = FALSE;
    static const CString TAG;// = "ConnectivityService";

    // how Int64 to wait before switching back to a radio's default network
    static const Int32 RESTORE_DEFAULT_NETWORK_DELAY;// = 1 * 60 * 1000;
    // system property that can override the above value
    static const CString NETWORK_RESTORE_DELAY_PROP_NAME;//= "android.telephony.apn-restore";

    //ITethering * mTethering;
    Boolean mTetheringConfigValid;// = FALSE;

    /**
     * Sometimes we want to refer to the individual network state
     * trackers separately, and sometimes we just want to treat them
     * abstractly.
     */
    List<INetworkStateTracker*>* mNetTrackers;

    /**
     * A per Net list of the PID's that requested access to the net
     * used both as a refcount and for per-PID DNS selection
     */
    List<List<Int32>*>* mNetRequestersPids;

    // priority order of the nettrackers
    // (excluding dynamically set mNetworkPreference)
    // TODO - move mNetworkTypePreference into this
    ArrayOf<Int32>* mPriorityList; //int[]

    AutoPtr<IContext> mContext;
    Int32 mNetworkPreference;
    Int32 mActiveDefaultNetwork;// = -1;
    // 0 is full bad, 100 is full good
    Int32 mDefaultInetCondition;// = 0;
    Int32 mDefaultInetConditionPublished;// = 0;
    Boolean mInetConditionChangeInFlight;// = FALSE;
    Int32 mDefaultConnectionSequence;// = 0;

    Int32 mNumDnsEntries;

    Boolean mTestMode;
    static IConnectivityService * sServiceInstance;
    static const Int32 ENABLED;//  = 1;
    static const Int32 DISABLED;// = 0;

    // Share the event space with NetworkStateTracker (which can't see this
    // internal class but sends us events).  If you change these, change
    // NetworkStateTracker.java too.
    static const Int32 MIN_NETWORK_STATE_TRACKER_EVENT;// = 1;
    static const Int32 MAX_NETWORK_STATE_TRACKER_EVENT;// = 100;

    /**
     * used Int32ernally as a delayed event to make us switch back to the
     * default network
     */
    static const Int32 EVENT_RESTORE_DEFAULT_NETWORK;// = MAX_NETWORK_STATE_TRACKER_EVENT + 1;

    /**
     * used Int32ernally to change our mobile data enabled flag
     */
    static const Int32 EVENT_CHANGE_MOBILE_DATA_ENABLED;// = MAX_NETWORK_STATE_TRACKER_EVENT + 2;

    /**
     * used internally to change our network preference setting
     * arg1 = networkType to prefer
     */
    static const Int32 EVENT_SET_NETWORK_PREFERENCE;// = MAX_NETWORK_STATE_TRACKER_EVENT + 3;

    /**
     * used internally to synchronize inet condition reports
     * arg1 = networkType
     * arg2 = condition (0 bad, 100 good)
     */
    static const Int32 EVENT_INET_CONDITION_CHANGE;// = MAX_NETWORK_STATE_TRACKER_EVENT + 4;

    /**
     * used internally to mark the end of inet condition hold periods
     * arg1 = networkType
     */
    static const Int32 EVENT_INET_CONDITION_HOLD_END;// = MAX_NETWORK_STATE_TRACKER_EVENT + 5;

    /**
     * used internally to set the background data preference
     * arg1 = TRUE for enabled, FALSE for disabled
     */
    static const Int32 EVENT_SET_BACKGROUND_DATA;// = MAX_NETWORK_STATE_TRACKER_EVENT + 6;

    /**
     * used internally to set enable/disable cellular data
     * arg1 = ENBALED or DISABLED
     */
    static const Int32 EVENT_SET_MOBILE_DATA;// = MAX_NETWORK_STATE_TRACKER_EVENT + 7;

//    private IHandler* mHandler;

    // list of DeathRecipients used to make sure features are turned off when
    // a process dies
    List<FeatureUser *> * mFeatureUsers;

    Boolean mSystemReady;
    AutoPtr<IIntent> mInitialBroadcast;

    // used in DBG mode to track inet condition reports
    static const Int32 INET_CONDITION_LOG_MAX_SIZE;// = 15;
    List<String>* mInetLog;

    CARAPI_(INetworkStateTracker *) MakeWimaxStateTracker();

    CARAPI_(void) HandleSetNetworkPreference(
        /* [in] */ Int32 preference);

    CARAPI_(Int32) GetPersistedNetworkPreference();

    /**
     * Make the state of network connectivity conform to the preference settings
     * In this method, we only tear down a non-preferred network. Establishing
     * a connection to the preferred network is taken care of when we handle
     * the disconnect event from the non-preferred network
     * (see {@link #handleDisconnect(NetworkInfo)}).
     */
    CARAPI_(void) EnforcePreference();

    CARAPI_(Boolean) Teardown(
        /* [in] */ INetworkStateTracker* netTracker);

    CARAPI_(Int32) StopUsingNetworkFeature(
        /* [in] */ FeatureUser * u,
        /* [in] */ Boolean ignoreDups);

    CARAPI_(void) HandleSetBackgroundData(
        /* [in] */ Boolean enabled);

    CARAPI_(void) HandleSetMobileData(
        /* [in] */ Boolean enabled);

    CARAPI_(Int32) GetNumConnectedNetworks();

    CARAPI_(void) EnforceAccessPermission();

    CARAPI_(void) EnforceChangePermission();

    // TODO Make this a special check when it goes public
    CARAPI_(void) EnforceTetherChangePermission();

    CARAPI_(void) EnforceTetherAccessPermission();

    /**
     * Handle a {@code DISCONNECTED} event. If this pertains to the non-active
     * network, we ignore it. If it is for the active network, we send out a
     * broadcast. But first, we check whether it might be possible to connect
     * to a different network.
     * @param info the {@code NetworkInfo} for the network
     */
    CARAPI_(void) HandleDisconnect(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) SendConnectedBroadcast(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) SendInetConditionBroadcast(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) SendGeneralBroadcast(
        /* [in] */ INetworkInfo* info,
        /* [ijn] */ const String & bcastType);

    /**
     * Called when an attempt to fail over to another network has failed.
     * @param info the {@link NetworkInfo} for the failed network
     */
    CARAPI_(void) HandleConnectionFailure(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) SendStickyBroadcast(
        /* [in] */ IIntent * intent);

    CARAPI_(void) HandleConnect(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) HandleScanResultsAvailable(
        /* [in] */ INetworkInfo * info);

    CARAPI_(void) HandleNotificationChange(
        /* [in] */ Boolean visible,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification);

    /**
     * After a change in the connectivity state of any network, We're mainly
     * concerned with making sure that the list of DNS servers is setupup
     * according to which networks are connected, and ensuring that the
     * right routing table entries exist.
     */
    CARAPI_(void) HandleConnectivityChange(
        /* [in] */ Int32 netType);

    /**
     * Adjust the per-process dns entries (net.dns<x>.<pid>) based
     * on the highest priority active net which this process requested.
     * If there aren't any, clear it out
     */
    CARAPI_(void) ReassessPidDns(
        /* [in] */ Int32 myPid,
        /* [in] */ Boolean doBump);

    CARAPI_(void) WritePidDns(
        /* [in] */ const ArrayOf<String> &dnsList,
        /* [in] */ Int32 pid);

    CARAPI_(void) BumpDns();

    CARAPI_(void) HandleDnsConfigurationChange(
        /* [in] */ Int32 netType);

    CARAPI_(Int32) GetRestoreDefaultNetworkDelay();

    CARAPI_(void) HandleInetConditionChange(
        /* [in] */ Int32 netType,
        /* [in] */ Int32 condition);

    CARAPI_(void) HandleInetConditionHoldEnd(
        /* [in] */ Int32 netType,
        /* [in] */ Int32 sequence);

    CARAPI_(void) TryFailover(
        /* [in] */ Int32 prevNetType);

private:
    Mutex mutex;

    CARAPI_(Boolean) IsNetRequestersPidContains(
        /* [in] */ List<Int32> *l,
        /* [in] */ Int32 pid);

    CARAPI_(Boolean) IsFeatureUsereContains(
        /* [in] */ FeatureUser * u);

    CARAPI_(Boolean) IsTetheringSupported();

    static CARAPI_(AutoPtr<ISettingsSecure>) SettingsSecure();

    CARAPI_(AutoPtr<ILocalConnectivityManagerHelper>) ConnectivityManager();

    CARAPI_(Boolean) ConnectivityManagerIsNetworkTypeValid(
        /* [in] */ Int32 networkType);

    CARAPI_(AutoPtr<INetworkInfo>) NetworkStateTrackerGetNetworkInfo(
        /* [in] */ INetworkStateTracker* nst);

    CARAPI_(Boolean) NetworkStateTrackerIsAvailable(
        /* [in] */ INetworkStateTracker* nst);

    CARAPI_(Boolean) NetworkStateTrackerTeardown(
        /* [in] */ INetworkStateTracker* nst);

    CARAPI_(Boolean) NetworkStateTrackerIsTeardownRequested(
        /* [in] */ INetworkStateTracker* nst);

    CARAPI_(Boolean) NetworkInfoIsConnected(
        /* [in] */ INetworkInfo *ni);

    CARAPI_(Boolean) NetworkInfoIsConnectedOrConnecting(
        /* [in] */ INetworkInfo *ni);

    CARAPI_(Boolean) NetworkInfoIsAvailable(
        /* [in] */ INetworkInfo *ni);

    CARAPI_(String) NetworkInfoToString(
        /* [in] */ INetworkInfo *ni);

    CARAPI_(String) NetworkStateTrackerToString(
        /* [in] */ INetworkStateTracker* nst);
};

#endif //__CCONNECTIVITYSERVICE_H__
