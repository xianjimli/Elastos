
#include "server/CConnectivityService.h"
#include "ext/frameworkext.h"
#include <Elastos.Framework.Core.h>
#include <Slogger.h>
#include <Logger.h>
#include <StringBuffer.h>
//#include <CStringWrapper.h>
//#include <CGregorianCalendar.h>
#include "os/SystemProperties.h"
#include "os/SystemClock.h"
#include "os/Binder.h"
#include "os/Process.h"
//#include "net/CConnectivityManagerHelper.h"
#include "text/TextUtils.h"
#include <ext/frameworkext.h>
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Set.h>
#include <elastos/ElRefBase.h>
#include <elastos/Thread.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

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
import android.net.INetworkStateTracker; <------ no this class
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
*/
//import java.util.GregorianCalendar;

IConnectivityService* CConnectivityService::sServiceInstance = NULL;

/*static*/ const Boolean CConnectivityService::DBG = FALSE;
/*static*/ const CString CConnectivityService::TAG = "ConnectivityService";

// how Int64 to wait before switching back to a radio's default network
/*static*/ const Int32 CConnectivityService::RESTORE_DEFAULT_NETWORK_DELAY = 1 * 60 * 1000;
// system property that can override the above value
/*static*/ const CString CConnectivityService::NETWORK_RESTORE_DELAY_PROP_NAME = CString("android.telephony.apn-restore");

/*static*/ const Int32 CConnectivityService::ENABLED = 1;
/*static*/ const Int32 CConnectivityService::DISABLED = 0;

// Share the event space with INetworkStateTracker (which can't see this
// Int32ernal class but sends us events).  If you change these, change
// INetworkStateTracker.java too.
/*static*/ const Int32 CConnectivityService::MIN_NETWORK_STATE_TRACKER_EVENT = 1;
/*static*/ const Int32 CConnectivityService::MAX_NETWORK_STATE_TRACKER_EVENT = 100;

/**
 * used Int32ernally as a delayed event to make us switch back to the
 * default network
 */
/*static*/ const Int32 CConnectivityService::EVENT_RESTORE_DEFAULT_NETWORK = MAX_NETWORK_STATE_TRACKER_EVENT + 1;

/**
 * used Int32ernally to change our mobile data enabled flag
 */
/*static*/ const Int32 CConnectivityService::EVENT_CHANGE_MOBILE_DATA_ENABLED = MAX_NETWORK_STATE_TRACKER_EVENT + 2;

/**
 * used Int32ernally to change our network preference setting
 * arg1 = networkType to prefer
 */
/*static*/ const Int32 CConnectivityService::EVENT_SET_NETWORK_PREFERENCE = MAX_NETWORK_STATE_TRACKER_EVENT + 3;

/**
 * used Int32ernally to synchronize inet condition reports
 * arg1 = networkType
 * arg2 = condition (0 bad, 100 good)
 */
/*static*/ const Int32 CConnectivityService::EVENT_INET_CONDITION_CHANGE = MAX_NETWORK_STATE_TRACKER_EVENT + 4;

/**
 * used Int32ernally to mark the end of inet condition hold periods
 * arg1 = networkType
 */
/*static*/ const Int32 CConnectivityService::EVENT_INET_CONDITION_HOLD_END = MAX_NETWORK_STATE_TRACKER_EVENT + 5;

/**
 * used Int32ernally to set the background data preference
 * arg1 = TRUE for enabled, FALSE for disabled
 */
/*static*/ const Int32 CConnectivityService::EVENT_SET_BACKGROUND_DATA = MAX_NETWORK_STATE_TRACKER_EVENT + 6;

/**
 * used Int32ernally to set enable/disable cellular data
 * arg1 = ENBALED or DISABLED
 */
/*static*/ const Int32 CConnectivityService::EVENT_SET_MOBILE_DATA = MAX_NETWORK_STATE_TRACKER_EVENT + 7;

// used in DBG mode to track inet condition reports
/*static*/ const Int32 CConnectivityService::INET_CONDITION_LOG_MAX_SIZE = 15;

/**
 * @hide
 */
CConnectivityService::CConnectivityService()
{
    mTetheringConfigValid = FALSE;
    mActiveDefaultNetwork = -1;
    // 0 is full bad, 100 is full good
    mDefaultInetCondition = 0;
    mDefaultInetConditionPublished = 0;
    mInetConditionChangeInFlight = FALSE;
    mDefaultConnectionSequence = 0;

}

List<String> * CConnectivityService::StringUtil::Split(
    /* [in] */ const String & str,
    /* [in] */ const String & delimeter)
{
    if (!str.IsNullOrEmpty() && !delimeter.IsNullOrEmpty())
    {
        List<String> * ret = NULL;
        Int32 index1 = 0;
        Int32 index2 = -1;
        index2 = str.IndexOf(delimeter,index1);
        if (index2 >= 0) {
            ret = new List<String>();
        }
        while( index2 >= 0 && index2 < (Int32)(str.GetLength()) ) {
            Int32 len = index2 - index1;
            String s = str.Substring(index1,len);
            String sTrim= s.Trim();
            ret->PushBack(sTrim);
            index1 = index2 + 1;
            index2 = str.IndexOf(delimeter,index1);
        }
        return ret;
    }
    return NULL;
}

CConnectivityService::NetworkAttributes::NetworkAttributes(
    /* [in] */ const String& init)
{
    String str=String(",");
    List<String> *fragments = CConnectivityService::StringUtil::Split(init, str);
    if (fragments != NULL)
    {
        String str0 = (*fragments)[0];
        str0.ToLowerCase();
        mName = String(str0.string());
        mType = ((*fragments)[1]).ToInt32();
        mRadio = ((*fragments)[2]).ToInt32();
        mPriority = ((*fragments)[3]).ToInt32();
    }
    mLastState = NetworkInfoState_UNKNOWN;
}
Boolean CConnectivityService::NetworkAttributes::IsDefault()
{
    return (mType == mRadio);
}

CConnectivityService::RadioAttributes::RadioAttributes(
    /* [in] */ const String& init)
{
    String str=String(",");
    List<String> * fragments = CConnectivityService::StringUtil::Split(init,str);
    if (fragments != NULL) {
        mType = (*fragments)[0].ToInt32();
        mSimultaneity = (*fragments)[1].ToInt32();
    }
}

CConnectivityService::ConnectivityThread::ConnectivityThread(
    /* [in] */ IContext *context)
{
    Thread::SetName(String("ConnectivityThread"));
    mContext = context;
}

ECode CConnectivityService::ConnectivityThread::constructor(
    /* [in] */ IContext *context)
{
    Thread::SetName(String("ConnectivityThread"));
    mContext = context;
    return NOERROR;
}

//@Override
ECode CConnectivityService::ConnectivityThread::Run()
{
    //?? currently no Looper class available
    //Looper.prepare();
    CConnectivityService::New(mContext, (IConnectivityService **) &(CConnectivityService::sServiceInstance));
    ISynchronize *tmp = (ISynchronize *) this;
    tmp->NotifyAll();
    //Looper.loop();
    return NOERROR;
}

IConnectivityService * CConnectivityService::ConnectivityThread::GetServiceInstance(
    /* [in] */ IContext* context)
{
    //AutoPtr<IThread> thread;
    //CConnectivityService::ConnectivityThread::New(context, (IThread **) &thread);
    ConnectivityThread * thread = new ConnectivityThread(context);
    thread->Start();
    //synchronized (thread) {
    thread->Lock();
    {
        while (CConnectivityService::sServiceInstance == NULL) {
            //Wait until sServiceInstance has been initialized.
            ECode ec = thread->Wait();
            if (FAILED(ec)) {
                Slogger::E(TAG,StringBuffer("Unexpected InterruptedException while waiting")+
                    String(" for ConnectivityService thread"));
            }
        }
    }
    thread->Unlock();

    return CConnectivityService::sServiceInstance;
}

IConnectivityService * CConnectivityService::GetInstance(
    /* [in] */ IContext* context)
{
    return ConnectivityThread::GetServiceInstance(context);
}

CConnectivityService::~CConnectivityService()
{
    if (mNetTrackers != NULL) {
        for (Int32 i=0; i<(Int32)(mNetTrackers->GetSize()); i++) {
            INetworkStateTracker * nst = (*mNetTrackers)[i];
            nst->Release();
        }
        delete mNetTrackers;
    }

    if (mRadioAttributes != NULL) {
        for (Int32 i=0; i<(Int32)(mRadioAttributes->GetSize()); i++) {
            RadioAttributes * ra = (*mRadioAttributes)[i];
            delete ra;
        }
        delete mRadioAttributes;
    }

    if (mNetAttributes != NULL) {
        for (Int32 i=0; i<(Int32)(mNetAttributes->GetSize()); i++) {
            NetworkAttributes * na = (*mNetAttributes)[i];
            delete na;
        }
        delete mNetAttributes;
    }

    ArrayOf<Int32>::Free(mPriorityList);

    if (mNetRequestersPids != NULL) {
        for (Int32 i = 0; i<(Int32)(mNetRequestersPids->GetSize()); i++) {
            List<Int32>* p = (*mNetRequestersPids)[i];
            delete p;
        }
        delete mNetRequestersPids;
    }

    if (mFeatureUsers != NULL ) {
        for (Int32 i=0; i<(Int32)(mFeatureUsers->GetSize()); i++) {
            FeatureUser * u = (*mFeatureUsers)[i];
            delete u;
        }
        delete mFeatureUsers;
    }

    if (mInetLog != NULL) {
        delete mInetLog;
    }
}

ECode CConnectivityService::constructor(
    /* [in] */ IContext * context)
{
    if (DBG) {
        Slogger::V(TAG, CString("ConnectivityService starting up"));
    }
    // setup our unique device name
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver **) &cr);
    String id;
    SettingsSecure()->GetString(cr, String(SettingsSecure_ANDROID_ID),&id);
    if (!id.IsNullOrEmpty()) {
        String name = String("android_") + id;
        SystemProperties::Set(String("net.hostname"), name);
    }

    mContext = context;
    mNetTrackers = new List<INetworkStateTracker*>();

    //??
    //mHandler = new MyHandler();

    mNetworkPreference = GetPersistedNetworkPreference();

    mRadioAttributes = new List<RadioAttributes*>();//[ConnectivityManager.MAX_RADIO_TYPE+1];
    mNetAttributes = new List<NetworkAttributes*>();//[ConnectivityManager.MAX_NETWORK_TYPE+1];

    // Load device network attributes from resources
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);

    ArrayOf<String>* raStrings = NULL;
    res->GetStringArray(
            0x0107000b, //com.android.internal.R.array.radioAttributes,
            &raStrings);
    for (Int32 i=0; i< raStrings->GetLength(); i++) {
        RadioAttributes *r = new RadioAttributes((*raStrings)[i]);
        if (r->mType > ConnectivityManager_MAX_RADIO_TYPE) {
            Slogger::E(TAG, StringBuffer("Error in radioAttributes - ignoring attempt to define type ") +
                            String::FromInt32(r->mType));
            continue;
        }
        if ((*mRadioAttributes)[r->mType] != NULL) {
            Slogger::E(TAG, StringBuffer("Error in radioAttributes - ignoring attempt to redefine type ") +
                    String::FromInt32(r->mType));
            continue;
        }
        (*mRadioAttributes)[r->mType] = r;
    }

    ArrayOf<String> * naStrings = NULL;
    res->GetStringArray(
            0x0107000a, //com.android.internal.R.array.networkAttributes);
            &naStrings);
    for (Int32 i=0; i<naStrings->GetLength();i++) { //String naString : naStrings) {
        NetworkAttributes *n = new NetworkAttributes((*naStrings)[i]);
        if (n->mType > ConnectivityManager_MAX_NETWORK_TYPE) {
            Slogger::E(TAG, StringBuffer("Error in networkAttributes - ignoring attempt to define type ") +
                    String::FromInt32(n->mType));
            continue;
        }
        if ((*mNetAttributes)[n->mType] != NULL) {
            Slogger::E(TAG, StringBuffer("Error in networkAttributes - ignoring attempt to redefine type ") +
                    String::FromInt32(n->mType));
            continue;
        }
        if ((*mRadioAttributes)[n->mRadio] == NULL) {
            Slogger::E(TAG, StringBuffer("Error in networkAttributes - ignoring attempt to use undefined ") +
                    String("radio ") +
                    String::FromInt32(n->mRadio) +
                    String(" in network type ") +
                    String::FromInt32(n->mType));
            continue;
        }
        (*mNetAttributes)[n->mType] = n;
        mNetworksDefined++;

    }
    ArrayOf<String>::Free(naStrings);

    // high priority first
    mPriorityList = ArrayOf<Int32>::Alloc(mNetworksDefined);

    {
        Int32 insertionPoInt32 = mNetworksDefined-1;
        Int32 currentLowest = 0;
        Int32 nextLowest = 0;
        while (insertionPoInt32 > -1) {
            //for (NetworkAttributes na : mNetAttributes) {
            Int32 count = mNetAttributes->GetSize();
            for (Int32 i = 0; i<count; i++) {
                NetworkAttributes * na = (*mNetAttributes)[i];
                if (na == NULL) continue;
                if (na->mPriority < currentLowest) continue;
                if (na->mPriority > currentLowest) {
                    if (na->mPriority < nextLowest || nextLowest == 0) {
                        nextLowest = na->mPriority;
                    }
                    continue;
                }
                (*mPriorityList)[insertionPoInt32--] = na->mType;
            }
            currentLowest = nextLowest;
            nextLowest = 0;
        }
    }

    mNetRequestersPids = new List<List<Int32>*>();//new ArrayList[ConnectivityManager.MAX_NETWORK_TYPE+1];
    for (Int32 i = 0; i<mPriorityList->GetLength(); i++) {
        (*mNetRequestersPids)[i] = new List<Int32>();
    }

    mFeatureUsers = new List<FeatureUser *>();

    mNumDnsEntries = 0;

    mTestMode = SystemProperties::Get(CString("cm.test.mode")).Equals(String("TRUE"))
            && SystemProperties::Get(CString("ro.build.type")).Equals(String("eng"));
    /*
     * Create the network state trackers for Wi-Fi and mobile
     * data. Maybe this could be done with a factory class,
     * but it's not clear that it's worth it, given that
     * the number of different network types is not going
     * to change very often.
     */
    Boolean MobileData;
    GetMobileDataEnabled(&MobileData);
    Boolean noMobileData = !MobileData;
    for (Int32 i = 0; i<mPriorityList->GetLength(); i++) {
        Int32 netType = (*mPriorityList)[i];
        switch ((*mNetAttributes)[netType]->mRadio) {
        case ConnectivityManager_TYPE_WIFI:
            if (DBG) Slogger::V(TAG, CString("Starting Wifi Service."));
            //??
            /*
            WifiStateTracker wst = new WifiStateTracker(context, mHandler);
            WifiService wifiService = new WifiService(context, wst);
            ServiceManager.addService(Context.WIFI_SERVICE, wifiService);
            wifiService.startWifi();
            (*mNetTrackers)[ConnectivityManager.TYPE_WIFI] = wst;
            wst.startMonitoring();
            */
            break;
        case ConnectivityManager_TYPE_MOBILE:
            //??
            /*
            (*mNetTrackers)[netType] = new MobileDataStateTracker(context, mHandler,
                netType, (*mNetAttributes)[netType].mName);
            (*mNetTrackers)[netType].startMonitoring();
            if (noMobileData) {
                if (DBG) Slogger::d(TAG, "tearing down Mobile networks due to setting");
                (*mNetTrackers)[netType].teardown();
            }
            */
            break;
        case ConnectivityManager_TYPE_WIMAX:
            //??
            /*
            INetworkStateTracker nst = makeWimaxStateTracker();
            if (nst != NULL) {
                nst.startMonitoring();
            }
            (*mNetTrackers)[netType] = nst;
            if (noMobileData) {
                if (DBG) Slogger::d(TAG, "tearing down WiMAX networks due to setting");
                (*mNetTrackers)[netType].teardown();
            }
            */
            break;
        default:
            Slogger::E(TAG, StringBuffer("Trying to create a DataStateTracker for an unknown radio type ") +
                    String::FromInt32((*mNetAttributes)[netType]->mRadio));
            continue;
        }
    }

    //??
    /*
    mTethering = new Tethering(mContext, mHandler.getLooper());
    mTetheringConfigValid = ((((*mNetTrackers)[ConnectivityManager.TYPE_MOBILE_DUN] != NULL) ||
                              !mTethering.isDunRequired()) &&
                             (mTethering.getTetherableUsbRegexs().length != 0 ||
                              mTethering.getTetherableWifiRegexs().length != 0) &&
                             mTethering.getUpstreamIfaceRegexs().length != 0);
    */

    if (DBG) {
        mInetLog = new List<String>();
    }
    return NOERROR;
}

INetworkStateTracker * CConnectivityService::MakeWimaxStateTracker()
{
    return NULL;
    //Initialize Wimax
    //??
    /*
    DexClassLoader wimaxClassLoader;
    Class wimaxStateTrackerClass = NULL;
    Class wimaxServiceClass = NULL;
    Class wimaxManagerClass;
    String wimaxJarLocation;
    String wimaxLibLocation;
    String wimaxManagerClassName;
    String wimaxServiceClassName;
    String wimaxStateTrackerClassName;

    INetworkStateTracker wimaxStateTracker = NULL;

    Boolean isWimaxEnabled = mContext.getResources().getBoolean(
            com.android.Int32ernal.R.bool.config_wimaxEnabled);

    if (isWimaxEnabled) {
        try {
            wimaxJarLocation = mContext.getResources().getString(
                    com.android.Int32ernal.R.string.config_wimaxServiceJarLocation);
            wimaxLibLocation = mContext.getResources().getString(
                    com.android.Int32ernal.R.string.config_wimaxNativeLibLocation);
            wimaxManagerClassName = mContext.getResources().getString(
                    com.android.Int32ernal.R.string.config_wimaxManagerClassname);
            wimaxServiceClassName = mContext.getResources().getString(
                    com.android.Int32ernal.R.string.config_wimaxServiceClassname);
            wimaxStateTrackerClassName = mContext.getResources().getString(
                    com.android.Int32ernal.R.string.config_wimaxStateTrackerClassname);

            wimaxClassLoader =  new DexClassLoader(wimaxJarLocation,
                    new ContextWrapper(mContext).getCacheDir().getAbsolutePath(),
                    wimaxLibLocation,ClassLoader.getSystemClassLoader());

            try {
                wimaxManagerClass = wimaxClassLoader.loadClass(wimaxManagerClassName);
                wimaxStateTrackerClass = wimaxClassLoader.loadClass(wimaxStateTrackerClassName);
                wimaxServiceClass = wimaxClassLoader.loadClass(wimaxServiceClassName);
            } catch (ClassNotFoundException ex) {
                ex.prInt32StackTrace();
                return NULL;
            }
        } catch(Resources.NotFoundException ex) {
            Slogger::e(TAG, "Wimax Resources does not exist!!! ");
            return NULL;
        }

        try {
            Slogger::v(TAG, "Starting Wimax Service... ");

            Constructor wmxStTrkrConst = wimaxStateTrackerClass.getConstructor
                    (new Class[] {Context.class,Handler.class});
            wimaxStateTracker = (INetworkStateTracker)wmxStTrkrConst.newInstance(mContext,mHandler);

            Constructor wmxSrvConst = wimaxServiceClass.getDeclaredConstructor
                    (new Class[] {Context.class,wimaxStateTrackerClass});
            wmxSrvConst.setAccessible(TRUE);
            IBinder svcInvoker = (IBinder) wmxSrvConst.newInstance(mContext,wimaxStateTracker);
            wmxSrvConst.setAccessible(FALSE);

            ServiceManager.addService(WimaxManagerConstants.WIMAX_SERVICE, svcInvoker);

        } catch(ClassCastException ex) {
            ex.prInt32StackTrace();
            return NULL;
        } catch (NoSuchMethodException ex) {
            ex.prInt32StackTrace();
            return NULL;
        } catch (InstantiationException ex) {
            ex.prInt32StackTrace();
            return NULL;
        } catch(IllegalAccessException ex) {
            ex.prInt32StackTrace();
            return NULL;
        } catch(InvocationTargetException ex) {
            ex.prInt32StackTrace();
            return NULL;
        } catch(Exception ex) {
            ex.prInt32StackTrace();
            return NULL;
        }
    } else {
        Slogger::e(TAG, "Wimax is not enabled or not added to the network attributes!!! ");
        return NULL;
    }

    return wimaxStateTracker;
    */
}

/**
 * Sets the preferred network.
 * @param preference the new preference
 */
ECode CConnectivityService::SetNetworkPreference(
    /* [in] */ Int32 preference)
{
    EnforceChangePermission();
//??
//    mHandler.sendMessage(mHandler.obtainMessage(EVENT_SET_NETWORK_PREFERENCE, preference, 0));
    ECode ec = NOERROR;
    return ec;
}

ECode CConnectivityService::GetNetworkPreference(
    /* [out] */ Int32* preference)
{
    VALIDATE_NOT_NULL(preference);
    EnforceAccessPermission();
    Mutex::Autolock l(mutex);
    //synchronized(this) {
    *preference = mNetworkPreference;
    //}
    return NOERROR;
}

//private
void CConnectivityService::HandleSetNetworkPreference(
    /* [in] */ Int32 preference)
{
    if (ConnectivityManagerIsNetworkTypeValid(preference) &&
        (*mNetAttributes)[preference] != NULL &&
        (*mNetAttributes)[preference]->IsDefault()) {
        if (mNetworkPreference != preference) {
            AutoPtr<IContentResolver> cr;
            mContext->GetContentResolver((IContentResolver **)&cr);
            Boolean result;
            SettingsSecure()->PutInt32((IContentResolver*)cr, String(SettingsSecure_NETWORK_PREFERENCE), preference, &result);
            mutex.Lock();
            //synchronized(this) {
                mNetworkPreference = preference;
            //}
            mutex.Unlock();;
            EnforcePreference();
        }
    }
}

//private
Int32 CConnectivityService::GetPersistedNetworkPreference()
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver **)&cr);

    Int32 networkPrefSetting;
    SettingsSecure()->GetInt32((IContentResolver *)cr, String(SettingsSecure_NETWORK_PREFERENCE), -1, &networkPrefSetting);
    if (networkPrefSetting != -1) {
        return networkPrefSetting;
    }

    return ConnectivityManager_DEFAULT_NETWORK_PREFERENCE;
}

/**
 * Make the state of network connectivity conform to the preference settings
 * In this method, we only tear down a non-preferred network. Establishing
 * a connection to the preferred network is taken care of when we handle
 * the disconnect event from the non-preferred network
 * (see {@link #handleDisconnect(NetworkInfo)}).
 */
//private
void CConnectivityService::EnforcePreference()
{
    if (NetworkInfoIsConnected((INetworkInfo *)
            NetworkStateTrackerGetNetworkInfo(((*mNetTrackers)[mNetworkPreference]))) )
        return;

    if (!NetworkStateTrackerIsAvailable(( (*mNetTrackers)[mNetworkPreference]) ) )
        return;

    for (Int32 t=0; t <= ConnectivityManager_MAX_RADIO_TYPE; t++) {
        if ( t != mNetworkPreference && (*mNetTrackers)[t] != NULL ) {
            AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo((*mNetTrackers)[t]);
            if (NetworkInfoIsConnected((INetworkInfo*) ni)) {
                if (DBG) {
                    Slogger::D(TAG, StringBuffer("tearing down ") +
                        NetworkInfoToString((INetworkInfo*) ni) +
                        String(" in enforcePreference"));

                }
            }
            Teardown((INetworkStateTracker*)((*mNetTrackers)[t]));
        }
    }
}

//private
Boolean CConnectivityService::Teardown(
    /* [in] */ INetworkStateTracker* netTracker)
{
    assert( netTracker != NULL);
    if (NetworkStateTrackerTeardown(netTracker)) {
        netTracker->SetTeardownRequested(TRUE);
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Return NetworkInfo for the active (i.e., connected) network Int32erface.
 * It is assumed that at most one network is active at a time. If more
 * than one is active, it is indeterminate which will be returned.
 * @return the info for the active network, or {@code NULL} if none is
 * active
 */
ECode CConnectivityService::GetActiveNetworkInfo(
    /* [out] */ INetworkInfo** networkInfo)
{
    VALIDATE_NOT_NULL(networkInfo);
    EnforceAccessPermission();
    if (mActiveDefaultNetwork != -1) {
        (*mNetTrackers)[mActiveDefaultNetwork]->GetNetworkInfo(networkInfo);
    }
    *networkInfo = NULL;
    return NOERROR;
}

ECode CConnectivityService::GetNetworkInfo(
    /* [in] */ Int32 networkType,
    /* [out] */ INetworkInfo** networkInfo)
{
    VALIDATE_NOT_NULL(networkInfo);
    EnforceAccessPermission();
    if (ConnectivityManagerIsNetworkTypeValid(networkType)) {
        INetworkStateTracker * t = (*mNetTrackers)[networkType];
        if (t != NULL)
            return t->GetNetworkInfo(networkInfo);
    }
    *networkInfo = NULL;
    return NOERROR;
}

ECode CConnectivityService::GetAllNetworkInfo(
    /* [out] */ IObjectContainer** networkInfo)
{
    VALIDATE_NOT_NULL(networkInfo);
    EnforceAccessPermission();
    IObjectContainer* ret = NULL;
    CObjectContainer::New(&ret);
    //NetworkInfo[] result = new NetworkInfo[mNetworksDefined];
    for (Int32 i = 0; i < (Int32)mNetTrackers->GetSize(); i++) {
        INetworkStateTracker * t = (*mNetTrackers)[i];
        if(t != NULL) {
            AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo(t);
            ret->Add(ni);
        }
    }
    *networkInfo = ret;
    return NOERROR;
}

ECode CConnectivityService::SetRadios(
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean ret = TRUE;
    EnforceChangePermission();
    for (Int32 i = 0; i<(Int32)mNetTrackers->GetSize(); i++) {
        INetworkStateTracker * t = (*mNetTrackers)[i];
        if (t != NULL) {
            t->SetRadio(turnOn, &ret);
        }
    }
    *result = ret;
    return NOERROR;
}

ECode CConnectivityService::SetRadio(
    /* [in] */ Int32 netType,
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    EnforceChangePermission();
    if (!ConnectivityManagerIsNetworkTypeValid(netType)) {
        *result = FALSE;
        return NOERROR;
    }
    INetworkStateTracker *tracker = (*mNetTrackers)[netType];
    if (tracker != NULL) {
        Boolean ret = TRUE;
        tracker->SetRadio(turnOn, &ret);
        *result = ret;
        return NOERROR;
    }
    else {
        *result = FALSE;
        return NOERROR;
    }

}

/**
 * Used to notice when the calling process dies so we can self-expire
 *
 * Also used to know if the process has cleaned up after itself when
 * our auto-expire timer goes off.  The timer has a link to an object.
 *
 */
CConnectivityService::FeatureUser::FeatureUser(
        /* [in] */ Int32 type,
        /* [in] */ const String& feature,
        /* [in] */ IBinder *binder,
        /* [in] */ CConnectivityService *connectivityService)
{
    //super();
    mConnectivityService = connectivityService;
    mNetworkType = type;
    mFeature = feature;
    mBinder = binder;
    mPid = Binder::GetCallingPid();
    mUid = Binder::GetCallingUid();
    //??
    mCreateTime = SystemClock::GetUptimeMillis();

    ECode ec;
    //??
    //ec = mBinder->LinkToDeath(this, 0);
    if (FAILED(ec)) {
        //??
        //BinderDied();
    }
}

void CConnectivityService::FeatureUser::UnlinkDeathRecipient()
{
    //??
    //mBinder->UnlinkToDeath(this, 0);
}

void CConnectivityService::FeatureUser::BinderDied()
{
    Slogger::D(TAG, StringBuffer("ConnectivityService FeatureUser binderDied(") +
            String::FromInt32(mNetworkType) + String(", ") +
            mFeature + String(", ") +
            ToString() + String("), created ") +
            String::FromInt64(SystemClock::GetUptimeMillis() - mCreateTime) +
            String(" mSec ago"));
    mConnectivityService->StopUsingNetworkFeature(this, FALSE);
}

void CConnectivityService::FeatureUser::Expire()
{
    Slogger::D(TAG, StringBuffer("ConnectivityService FeatureUser expire(") +
            String::FromInt32(mNetworkType) + String(", ") +
            mFeature + String(", ") +
            ToString() + String("), created ") +
            String::FromInt64(SystemClock::GetUptimeMillis() - mCreateTime) +
            String(" mSec ago"));
    mConnectivityService->StopUsingNetworkFeature(this, FALSE);
}

String CConnectivityService::FeatureUser::ToString()
{
    return String("FeatureUser(")+
           String::FromInt32(mNetworkType)+ String(",")+
           mFeature+String(",")+
           String::FromInt32(mPid)+String(",")+
           String::FromInt32(mUid)+String("), created ") +
           String::FromInt64(SystemClock::GetUptimeMillis() - mCreateTime) +
           String(" mSec ago");
}

AutoPtr<ICharSequence> CConnectivityService::StringUtil::StringToCharSeuence(
    /* [in] */ const String & str)
{
    AutoPtr<ICharSequence> ret;
    CStringWrapper::New(str, (ICharSequence**)&ret);
    return ret;
}

Boolean CConnectivityService::IsNetRequestersPidContains(
    /* [in] */ List<Int32> *l,
    /* [in] */ Int32 pid)
{
    if (l == NULL)
        return FALSE;
    for (Int32 i=0; i<(Int32) l->GetSize(); i++) {
        if ((*l)[i] == pid) {
            return TRUE;
        }
    }
    return FALSE;
}

// javadoc from Int32erface
ECode CConnectivityService::StartUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String & feature,
        /* [in] */ IBinder* binder,
        /* [out] */ Int32* outcome)
{
    VALIDATE_NOT_NULL(outcome);
    if (DBG) {
        Slogger::D(TAG, StringBuffer("startUsingNetworkFeature for net ") +
                String::FromInt32(networkType) + String(": ") + feature);
    }
    EnforceChangePermission();
    if (!ConnectivityManagerIsNetworkTypeValid(networkType) ||
            (*mNetAttributes)[networkType] == NULL) {
        //??
        //*outcome = Phone_APN_REQUEST_FAILED;
        return NOERROR;
    }

    FeatureUser * f = new FeatureUser(networkType, feature, binder, this);

    // TODO - move this Int32o the MobileDataStateTracker
    Int32 usedNetworkType = networkType;
    if(networkType == ConnectivityManager_TYPE_MOBILE) {
        Boolean tmp;
        GetMobileDataEnabled(&tmp);
        if (!tmp) {
            if (DBG) Slogger::D(TAG, CString("requested special network with data disabled - rejected"));
            //??
            //*outcome = Phone_APN_TYPE_NOT_AVAILABLE;
            return NOERROR;
        }

        //?? Phone_* macro not defined
        /*
        if (TextUtils::Equals(StringUtil::StringToCharSeuence(feature),
                StringUtil::StringToCharSeuence(Phone_FEATURE_ENABLE_MMS)) ) {
            usedNetworkType = ConnectivityManager_TYPE_MOBILE_MMS;
        } else if (TextUtils::Equals(StringUtil::StringToCharSeuence(feature),
                StringUtil::StringToCharSeuence(Phone.FEATURE_ENABLE_SUPL)) ) {
            usedNetworkType = ConnectivityManager_TYPE_MOBILE_SUPL;
        } else if (TextUtils::Equals(StringUtil::StringToCharSeuence(feature),
                StringUtil::StringToCharSeuence(Phone.FEATURE_ENABLE_DUN)) ) {
            usedNetworkType = ConnectivityManager_TYPE_MOBILE_DUN;
        } else if (TextUtils::Equals(StringUtil::StringToCharSeuence(feature),
                StringUtil::StringToCharSeuence(Phone.FEATURE_ENABLE_HIPRI)) ) {
            usedNetworkType = ConnectivityManager_TYPE_MOBILE_HIPRI;
        }
        */
    }
    INetworkStateTracker *network = (*mNetTrackers)[usedNetworkType];
    if (network != NULL) {
        if (usedNetworkType != networkType) {
            Int32 currentPid = Binder::GetCallingPid();

            INetworkStateTracker * radio = (*mNetTrackers)[networkType];
            AutoPtr<INetworkInfo> ni =NetworkStateTrackerGetNetworkInfo(radio);

            if (NetworkInfoIsAvailable((INetworkInfo *)ni) == FALSE) {
                if (DBG) Slogger::D(TAG, CString("special network not available"));
                //??
                //*outcome = Phone_APN_TYPE_NOT_AVAILABLE;
                return NOERROR;
            }

            mutex.Lock();
            mFeatureUsers->PushBack(f);
            if (!IsNetRequestersPidContains(
                    (*mNetRequestersPids)[usedNetworkType],
                    currentPid)) {
                // this gets used for per-pid dns when connected
                (*mNetRequestersPids)[usedNetworkType]->PushBack(currentPid);
            }
            mutex.Unlock();
            //??
            //mHandler.sendMessageDelayed(mHandler.obtainMessage(EVENT_RESTORE_DEFAULT_NETWORK,
            //        f), getRestoreDefaultNetworkDelay());

            if (NetworkInfoIsConnectedOrConnecting((INetworkInfo *)ni)  &&
                NetworkStateTrackerIsTeardownRequested(network) == FALSE ) {
                if (NetworkInfoIsConnected((INetworkInfo *)ni) == TRUE) {
                    // add the pid-specific dns
                    HandleDnsConfigurationChange(networkType);
                    if (DBG) Slogger::D(TAG, CString("special network already active"));
                    //??
                    //*outcome = Phone_APN_ALREADY_ACTIVE;
                    return NOERROR;
                }
                if (DBG) Slogger::D(TAG, CString("special network already connecting"));
                //??
                //*outcome = Phone_APN_REQUEST_STARTED;
                return NOERROR;
            }

            // check if the radio in play can make another contact
            // assume if cannot for now

            if (DBG) Slogger::D(TAG, "reconnecting to special network");
            Boolean t;
            network->Reconnect(&t);
            //??
            //*outcome = Phone_APN_REQUEST_STARTED;
            return NOERROR;
        } else {
            //synchronized(this) {
            mutex.Lock();
            mFeatureUsers->PushBack(f);
            //}
            mutex.Unlock();
            //??
            //mHandler.sendMessageDelayed(mHandler.obtainMessage(EVENT_RESTORE_DEFAULT_NETWORK,
            //        f), getRestoreDefaultNetworkDelay());

            network->StartUsingNetworkFeature(feature,
                    Binder::GetCallingPid(), Binder::GetCallingUid(), outcome);
            return NOERROR;
        }
    }
    //??
    //*outcome = Phone_APN_TYPE_NOT_AVAILABLE;
    return NOERROR;
}

// javadoc from Int32erface
ECode CConnectivityService::StopUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String & feature,
    /* [out] */ Int32* outcome)
{
    VALIDATE_NOT_NULL(outcome);
    EnforceChangePermission();

    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();

    FeatureUser *u = NULL;
    Boolean found = FALSE;

    mutex.Lock();
    //synchronized(this) {
        for (Int32 i = 0; i <(Int32) mFeatureUsers->GetSize() ; i++) {
            u = (FeatureUser *)((*mFeatureUsers)[i]);
            if (uid == u->mUid && pid == u->mPid &&
                    networkType == u->mNetworkType &&
                    TextUtils::Equals(StringUtil::StringToCharSeuence(feature),
                            StringUtil::StringToCharSeuence(u->mFeature)) ) {
                found = TRUE;
                break;
            }
        }
    //}
    mutex.Unlock();
    if (found && u != NULL) {
        // stop regardless of how many other time this proc had called start
        *outcome = StopUsingNetworkFeature(u, TRUE);
        return NOERROR;
    } else {
        // none found!
        if (DBG) Slogger::D(TAG, CString("ignoring stopUsingNetworkFeature - not a live request"));
        *outcome = 1;
        return NOERROR;
    }
}

Boolean CConnectivityService::IsFeatureUsereContains(
    /* [in] */ FeatureUser * u)
{
    for (Int32 i = 0; i<(Int32) mFeatureUsers->GetSize(); i++) {
        if ((*mFeatureUsers)[i] == u) {
            return TRUE;
        }
    }
    return FALSE;
}

//private
Int32 CConnectivityService::StopUsingNetworkFeature(
    /* [in] */ FeatureUser * u,
    /* [in] */ Boolean ignoreDups)
{
    Int32 networkType = u->mNetworkType;
    String feature = u->mFeature;
    Int32 pid = u->mPid;
    Int32 uid = u->mUid;

    INetworkStateTracker *tracker = NULL;
    Boolean callTeardown = FALSE;  // used to carry our decision outside of sync block

    if (DBG) {
        String tmp = String("stopUsingNetworkFeature for net ") +
                String::FromInt32(networkType) +
                String(": ") + feature;
        Slogger::V(TAG, CString(tmp.string()));
    }

    if (!ConnectivityManagerIsNetworkTypeValid(networkType)) {
        return -1;
    }

    // need to link the mFeatureUser list with the mNetRequestersPids state in this
    // sync block
    //synchronized(this) {
    mutex.Lock();
    {
    // check if this process still has an outstanding start request
        if (!IsFeatureUsereContains(u)) {
            if (DBG) Slogger::D(TAG, CString("ignoring - this process has no outstanding requests"));
            return 1;
        }
        u->UnlinkDeathRecipient();
        mFeatureUsers->Remove(u);//mFeatureUsers.indexOf(u));
        // If we care about duplicate requests, check for that here.
        //
        // This is done to support the extension of a request - the app
        // can request we start the network feature again and renew the
        // auto-shutoff delay.  Normal "stop" calls from the app though
        // do not pay attention to duplicate requests - in effect the
        // API does not refcount and a single stop will counter multiple starts.
        if (ignoreDups == FALSE) {
            for (Int32 i = 0; i <(Int32) mFeatureUsers->GetSize() ; i++) {
                FeatureUser *x = (FeatureUser *)(*mFeatureUsers)[i];
                if (x->mUid == u->mUid &&
                    x->mPid == u->mPid &&
                    x->mNetworkType == u->mNetworkType &&
                    TextUtils::Equals((ICharSequence *)CConnectivityService::StringUtil::StringToCharSeuence(x->mFeature),
                                      (ICharSequence *)CConnectivityService::StringUtil::StringToCharSeuence(u->mFeature)) ) {
                    if (DBG) Slogger::D(TAG, CString("ignoring stopUsingNetworkFeature as dup is found"));
                    return 1;
                }
            }
        }

        // TODO - move to MobileDataStateTracker
        Int32 usedNetworkType = networkType;
        if (networkType == ConnectivityManager_TYPE_MOBILE) {
            //?? phone_ * is not defined yet
            /*
            if (TextUtils::Equals(
                StringUtil::StringToCharSeuence(feature),
                StringUtil::StringToCharSeuence(Phone_FEATURE_ENABLE_MMS)) ) {
                usedNetworkType = ConnectivityManager_TYPE_MOBILE_MMS;
            } else if (TextUtils::Equals(
                    StringUtil::StringToCharSeuence(feature),
                    StringUtil::StringToCharSeuence(Phone_FEATURE_ENABLE_SUPL)) ) {
                usedNetworkType = ConnectivityManager_TYPE_MOBILE_SUPL;
            } else if (TextUtils::Equals(
                    StringUtil::StringToCharSeuence(feature),
                    StringUtil::StringToCharSeuence(Phone_FEATURE_ENABLE_DUN)) ) {
                usedNetworkType = ConnectivityManager_TYPE_MOBILE_DUN;
            } else if (TextUtils::Equals(
                    StringUtil::StringToCharSeuence(feature),
                    StringUtil::StringToCharSeuence(Phone_FEATURE_ENABLE_HIPRI)) ) {
                usedNetworkType = ConnectivityManager_TYPE_MOBILE_HIPRI;
            }
            */
        }
        tracker = (*mNetTrackers)[usedNetworkType];
        if (tracker == NULL) {
            if (DBG) Slogger::D(TAG, StringBuffer("ignoring - no known tracker for net type ") +
                    String::FromInt32(usedNetworkType));
            return -1;
        }
        if (usedNetworkType != networkType) {
            Int32 currentPid = pid;//new Integer(pid);
            (*mNetRequestersPids)[usedNetworkType]->Remove(currentPid);
            ReassessPidDns(pid, TRUE);
            if ((*mNetRequestersPids)[usedNetworkType]->GetSize() != 0) {
                if (DBG) Slogger::D(TAG, StringBuffer("not tearing down special network - ") +
                       String("others still using it"));
                return 1;
            }
            callTeardown = TRUE;
        }
    }
    mutex.Unlock();
    if (DBG) Slogger::D(TAG, CString("Doing network teardown"));
    if (callTeardown) {
        NetworkStateTrackerTeardown(tracker);
        return 1;
    } else {
        // do it the old fashioned way
        Int32 ret;
        tracker->StopUsingNetworkFeature(feature, pid, uid, &ret);
        return ret;
    }
}

/**
 * Ensure that a network route exists to deliver traffic to the specified
 * host via the specified network Int32erface.
 * @param networkType the type of the network over which traffic to the
 * specified host is to be routed
 * @param hostAddress the IP address of the host to which the route is
 * desired
 * @return {@code TRUE} on success, {@code FALSE} on failure
 */
ECode CConnectivityService::RequestRouteToHost(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    EnforceChangePermission();
    if (!ConnectivityManagerIsNetworkTypeValid(networkType)) {
        *result = FALSE;
        return NOERROR;
    }
    INetworkStateTracker * tracker = (*mNetTrackers)[networkType];

    AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo(tracker);
    if (tracker == NULL ||
        !NetworkInfoIsConnected((INetworkInfo *)ni) ||
        NetworkStateTrackerIsTeardownRequested(tracker)) {
        if (DBG) {
            Slogger::D(TAG, StringBuffer("requestRouteToHost on down network (") +
                    String::FromInt32(networkType) +
                    String(") - dropped"));
        }
        *result = FALSE;
        return NOERROR;
    }
    tracker->RequestRouteToHost(hostAddress, result);
    return NOERROR;
}

/**
 * @see ConnectivityManager#getBackgroundDataSetting()
 */
ECode CConnectivityService::GetBackgroundDataSetting(
    /* [out] */ Boolean* isAllowed)
{
    VALIDATE_NOT_NULL(isAllowed);
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 tmp;
    SettingsSecure()->GetInt32((IContentResolver*)cr,
                String(SettingsSecure_BACKGROUND_DATA), 1, &tmp);
    *isAllowed = (tmp == 1);
    return NOERROR;
}

/**
 * @see ConnectivityManager#setBackgroundDataSetting(Boolean)
 */
ECode CConnectivityService::SetBackgroundDataSetting(
    /* [in] */ Boolean allowBackgroundData)
{
//??
//    mContext.enforceCallingOrSelfPermission(
//            android.Manifest.permission.CHANGE_BACKGROUND_DATA_SETTING,
//            "ConnectivityService");

//??
//    mHandler.sendMessage(mHandler.obtainMessage(EVENT_SET_BACKGROUND_DATA,
//            (allowBackgroundDataUsage ? ENABLED : DISABLED), 0));
    return NOERROR;
}

//private
void CConnectivityService::HandleSetBackgroundData(
    /* [in] */ Boolean enabled)
{
    Boolean tmp;
    GetBackgroundDataSetting(&tmp);
    if (enabled != tmp) {
        AutoPtr<IContentResolver> cr;
        mContext->GetContentResolver((IContentResolver**)&cr);
        Boolean tmp;
        SettingsSecure()->PutInt32((IContentResolver *)cr,
                String(SettingsSecure_BACKGROUND_DATA), enabled ? 1 : 0, &tmp);
        AutoPtr<IIntent> broadcast;
        CIntent::New(
                String(ConnectivityManager_ACTION_BACKGROUND_DATA_SETTING_CHANGED),
                (IIntent **)&broadcast);
        mContext->SendBroadcast(broadcast);
    }
}

/**
 * @see ConnectivityManager#getMobileDataEnabled()
 */
ECode CConnectivityService::GetMobileDataEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    EnforceAccessPermission();
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 tmp;
    SettingsSecure()->GetInt32((IContentResolver*)cr,
                String(SettingsSecure_MOBILE_DATA), 1, &tmp);
    Boolean retVal = (tmp== 1);
    if (DBG) Slogger::D(TAG, StringBuffer("getMobileDataEnabled returning ") +
            String::FromBoolean(retVal));
    *result = retVal;
    return NOERROR;
}

/**
 * @see ConnectivityManager#setMobileDataEnabled(Boolean)
 */
ECode CConnectivityService::SetMobileDataEnabled(
    /* [in] */ Boolean enabled)
{
    EnforceChangePermission();
    if (DBG) Slogger::D(TAG, StringBuffer("setMobileDataEnabled(") +
            String::FromBoolean(enabled) +
            String(")"));

    //??
    //mHandler.sendMessage(mHandler.obtainMessage(EVENT_SET_MOBILE_DATA,
    //    (enabled ? ENABLED : DISABLED), 0));
    return NOERROR;
}

//private
void CConnectivityService::HandleSetMobileData(
    /* [in] */ Boolean enabled)
{
    Boolean tmp;
    GetMobileDataEnabled(&tmp);
    if ( tmp== enabled) return;

    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Boolean t1;
    SettingsSecure()->PutInt32((IContentResolver*)cr,
            String(SettingsSecure_MOBILE_DATA), enabled ? 1 : 0, &t1);

    if (enabled) {
        if ((*mNetTrackers)[ConnectivityManager_TYPE_MOBILE] != NULL) {
            if (DBG) {
                Slogger::D(TAG, StringBuffer("starting up ") +
                        (*mNetTrackers)[ConnectivityManager_TYPE_MOBILE]);
            }
            Boolean t;
            (*mNetTrackers)[ConnectivityManager_TYPE_MOBILE]->Reconnect(&t);
        }
        if ((*mNetTrackers)[ConnectivityManager_TYPE_WIMAX] != NULL) {
            if (DBG) {
                Slogger::D(TAG, StringBuffer("starting up ") +
                        (*mNetTrackers)[ConnectivityManager_TYPE_WIMAX]);
            }
            Boolean t;
            (*mNetTrackers)[ConnectivityManager_TYPE_WIMAX]->Reconnect(&t);
        }
    } else {

        for (Int32 i=0; i<(Int32)mNetTrackers->GetSize(); i++) {
            INetworkStateTracker * nt = (*mNetTrackers)[i];
            if (nt == NULL) continue;
            AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo(nt);
            Int32 netType;
            ni->GetType(&netType);
            if ((*mNetAttributes)[netType]->mRadio == ConnectivityManager_TYPE_MOBILE) {
                if (DBG) Slogger::D(TAG, StringBuffer("tearing down ") + NetworkStateTrackerToString(nt));
                NetworkStateTrackerTeardown(nt);
            }
        }
        if ((*mNetTrackers)[ConnectivityManager_TYPE_WIMAX] != NULL) {
            NetworkStateTrackerTeardown(((*mNetTrackers)[ConnectivityManager_TYPE_WIMAX]));
        }
    }
}

//private
Int32 CConnectivityService::GetNumConnectedNetworks()
{
    Int32 numConnectedNets = 0;

    for (Int32 i = 0; i<(Int32)mNetTrackers->GetSize(); i++) {
        INetworkStateTracker *nt = (*mNetTrackers)[i];
        AutoPtr<INetworkInfo> ni =NetworkStateTrackerGetNetworkInfo(nt);
        if (nt != NULL &&
            NetworkInfoIsConnected((INetworkInfo *)ni) &&
            !NetworkStateTrackerIsTeardownRequested(nt) ) {
            ++numConnectedNets;
        }
    }
    return numConnectedNets;
}

//private
void CConnectivityService::EnforceAccessPermission()
{
//??
//    mContext.enforceCallingOrSelfPermission(
//            android.Manifest.permission.ACCESS_NETWORK_STATE,
//            "ConnectivityService");
}

//private
void CConnectivityService::EnforceChangePermission()
{
//??
//    mContext.enforceCallingOrSelfPermission(
//            android.Manifest.permission.CHANGE_NETWORK_STATE,
//            "ConnectivityService");
}

// TODO Make this a special check when it goes public
//private
void CConnectivityService::EnforceTetherChangePermission()
{
//??
//    mContext.enforceCallingOrSelfPermission(
//            android.Manifest.permission.CHANGE_NETWORK_STATE,
//            "ConnectivityService");
}

//private
void CConnectivityService::EnforceTetherAccessPermission()
{
//??
//    mContext.enforceCallingOrSelfPermission(
//            android.Manifest.permission.ACCESS_NETWORK_STATE,
//            "ConnectivityService");
}

/**
 * Handle a {@code DISCONNECTED} event. If this pertains to the non-active
 * network, we ignore it. If it is for the active network, we send out a
 * broadcast. But first, we check whether it might be possible to connect
 * to a different network.
 * @param info the {@code NetworkInfo} for the network
 */
//private
void CConnectivityService::HandleDisconnect(
    /* [in] */ INetworkInfo* info)
{
    assert(info != NULL);
    Int32 prevNetType;
    info->GetType(&prevNetType);

    (*mNetTrackers)[prevNetType]->SetTeardownRequested(FALSE);
    /*
     * If the disconnected network is not the active one, then don't report
     * this as a loss of connectivity. What probably happened is that we're
     * getting the disconnect for a network that we explicitly disabled
     * in accordance with network preference policies.
     */
    if (!(*mNetAttributes)[prevNetType]->IsDefault()) {
        List<Int32> * pids = (*mNetRequestersPids)[prevNetType];
        for (Int32 i = 0; i<(Int32)pids->GetSize(); i++) {
            Int32 pid = (*pids)[i];
            // will remove them because the net's no Int64er connected
            // need to do this now as only now do we know the pids and
            // can properly NULL things that are no Int64er referenced.
            ReassessPidDns(pid, FALSE);
        }
    }

    IIntent* intent;
    CIntent::New(String(ConnectivityManager_CONNECTIVITY_ACTION),&intent);

    intent->PutParcelableExtra(String(ConnectivityManager_EXTRA_NETWORK_INFO),(IParcelable *) info);
    Boolean failover;
    info->IsFailover(&failover);
    if (failover) {
        intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_IS_FAILOVER), TRUE);
        info->SetFailover(FALSE);
    }
    String reason;
    info->GetReason(&reason);
    if (!reason.IsNullOrEmpty()) {
        intent->PutStringExtra(String(ConnectivityManager_EXTRA_REASON), reason);
    }
    String sinfo;
    info->GetExtraInfo(&sinfo);
    if (sinfo.IsNullOrEmpty()) {
        intent->PutStringExtra(String(ConnectivityManager_EXTRA_EXTRA_INFO),sinfo);
    }

    if ((*mNetAttributes)[prevNetType]->IsDefault()) {

        TryFailover(prevNetType);
        if (mActiveDefaultNetwork != -1) {
            AutoPtr<INetworkInfo> switchTo;
            (*mNetTrackers)[mActiveDefaultNetwork]->GetNetworkInfo((INetworkInfo **)&switchTo);
            INetworkInfo * tmp = (INetworkInfo *)switchTo;
            intent->PutParcelableExtra(String(ConnectivityManager_EXTRA_OTHER_NETWORK_INFO),(IParcelable *)tmp);
        } else {
            mDefaultInetConditionPublished = 0; // we're not connected anymore
            intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_NO_CONNECTIVITY), TRUE);
        }
    }
    //?? ConnectivityManager_EXTRA_INET_CONDITION is not defined
    //intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_INET_CONDITION), mDefaultInetConditionPublished);
    // do this before we broadcast the change
    HandleConnectivityChange(prevNetType);

    SendStickyBroadcast(intent);
    /*
     * If the failover network is already connected, then immediately send
     * out a followup broadcast indicating successful failover
     */
    if (mActiveDefaultNetwork != -1) {
        AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo((*mNetTrackers)[mActiveDefaultNetwork]);
        SendConnectedBroadcast((INetworkInfo *)ni);
    }
}

//private
void CConnectivityService::TryFailover(
    /* [in] */ Int32 prevNetType)
{
    /*
     * If this is a default network, check if other defaults are available
     * or active
     */
    if ((*mNetAttributes)[prevNetType]->IsDefault()) {
        if (mActiveDefaultNetwork == prevNetType) {
            mActiveDefaultNetwork = -1;
        }

        Boolean mobileData;
        GetMobileDataEnabled(&mobileData);
        Boolean noMobileData= !mobileData;
        for (Int32 checkType=0; checkType <= ConnectivityManager_MAX_NETWORK_TYPE; checkType++) {
            if (checkType == prevNetType) continue;
            if ((*mNetAttributes)[checkType] == NULL) continue;
            if ((*mNetAttributes)[checkType]->IsDefault() == FALSE) continue;
            if ((*mNetAttributes)[checkType]->mRadio == ConnectivityManager_TYPE_MOBILE &&
                    noMobileData) {
                Slogger::E(TAG, StringBuffer("not failing over to mobile type ") +
                        String::FromInt32(checkType) +
                        String(" because Mobile Data Disabled"));
                continue;
            }
            if ((*mNetAttributes)[checkType]->mRadio == ConnectivityManager_TYPE_WIMAX &&
                    noMobileData) {
                Slogger::E(TAG, StringBuffer("not failing over to mobile type ") +
                        String::FromInt32(checkType) +
                        String(" because Mobile Data Disabled"));
                continue;
            }
            AutoPtr<INetworkStateTracker> checkTracker = (*mNetTrackers)[checkType];
            AutoPtr<INetworkInfo> checkInfo = NetworkStateTrackerGetNetworkInfo((INetworkStateTracker *)checkTracker);

            if (!NetworkInfoIsConnectedOrConnecting((INetworkInfo *)checkInfo) ||
                 NetworkStateTrackerIsTeardownRequested(checkTracker)) {
                checkInfo->SetFailover(TRUE);
                Boolean t;
                checkTracker->Reconnect(&t);
            }
            if (DBG) {
                String typeName;
                checkInfo->GetTypeName(&typeName);
                Slogger::D(TAG, StringBuffer("Attempting to switch to ") + typeName);
            }
        }
    }
}

//private
void CConnectivityService::SendConnectedBroadcast(
    /* [in] */ INetworkInfo * info)
{
    assert( info != NULL);
    SendGeneralBroadcast(info, String(ConnectivityManager_CONNECTIVITY_ACTION));
}

//private
void CConnectivityService::SendInetConditionBroadcast(
    /* [in] */ INetworkInfo * info)
{
    assert( info != NULL);
    //?? ConnectivityManager_INET_CONDITION_ACTION is not defined
    //SendGeneralBroadcast(info, String(ConnectivityManager_INET_CONDITION_ACTION));
}

//private
void CConnectivityService::SendGeneralBroadcast(
    /* [in] */ INetworkInfo * info,
    /* [in] */ const String& bcastType)
{
    assert( info != NULL);
    AutoPtr<IIntent> intent;
    CIntent::New(bcastType,(IIntent **)&intent);
    intent->PutParcelableExtra(String(ConnectivityManager_EXTRA_NETWORK_INFO),(IParcelable *) info);
    Boolean isFailover;
    info->IsFailover(&isFailover);
    if (isFailover) {
        intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_IS_FAILOVER), TRUE);
        info->SetFailover(FALSE);
    }
    String reason;
    info->GetReason(&reason);
    if (reason.IsNullOrEmpty()) {
        intent->PutStringExtra(String(ConnectivityManager_EXTRA_REASON), reason);
    }
    String sinfo;
    info->GetExtraInfo(&sinfo);
    if (sinfo.IsNullOrEmpty()) {
        intent->PutStringExtra(String(ConnectivityManager_EXTRA_EXTRA_INFO),sinfo);
    }
    //?? no ConnectivityManager_EXTRA_INET_CONDITION defined
    //intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_INET_CONDITION), mDefaultInetConditionPublished);
    SendStickyBroadcast(intent);
}

/**
 * Called when an attempt to fail over to another network has failed.
 * @param info the {@link NetworkInfo} for the failed network
 */
//private
void CConnectivityService::HandleConnectionFailure(
    /* [in] */ INetworkInfo * info)
{
    assert( info != NULL);
    Int32 type;
    info->GetType(&type);
    (*mNetTrackers)[type]->SetTeardownRequested(FALSE);

    String reason;
    info->GetReason(&reason);
    String extraInfo;
    info->GetExtraInfo(&extraInfo);

    String reasonText;
    if (reason.IsNullOrEmpty()) {
        reasonText = ".";
    } else {
        reasonText = " (" + reason + ").";
    }
    String typeName;
    info->GetTypeName(&typeName);
    String tmp = String("Attempt to connect to ") + typeName + String(" failed") + reasonText;
    Slogger::E(TAG, CString(tmp.string()));

    AutoPtr<IIntent> intent;
    CIntent::New(String(ConnectivityManager_CONNECTIVITY_ACTION), (IIntent**) &intent);
    intent->PutParcelableExtra(String(ConnectivityManager_EXTRA_NETWORK_INFO),(IParcelable *) info);

    AutoPtr<INetworkInfo> activeNetworkInfo;
    ECode ec = GetActiveNetworkInfo((INetworkInfo **)&activeNetworkInfo);

    if (FAILED(ec)) {//activeNetworkInfo->Get() == NULL) {
        intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_NO_CONNECTIVITY), TRUE);
    }
    if (reason.IsNullOrEmpty()) {
        intent->PutStringExtra(String(ConnectivityManager_EXTRA_REASON), reason);
    }
    if (extraInfo.IsNullOrEmpty()) {
        intent->PutStringExtra(String(ConnectivityManager_EXTRA_EXTRA_INFO), extraInfo);
    }
    Boolean isFailover;
    info->IsFailover(&isFailover);
    if (isFailover) {
        intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_IS_FAILOVER), TRUE);
        info->SetFailover(FALSE);
    }

    if ((*mNetAttributes)[type]->IsDefault()) {
        TryFailover(type);
        if (mActiveDefaultNetwork != -1) {
            AutoPtr<INetworkInfo> switchTo;
            (*mNetTrackers)[mActiveDefaultNetwork]->GetNetworkInfo((INetworkInfo**) &switchTo);
            INetworkInfo * tmp = (INetworkInfo *) switchTo;
            intent->PutParcelableExtra(String(ConnectivityManager_EXTRA_OTHER_NETWORK_INFO),
                    (IParcelable * )tmp);
        } else {
            mDefaultInetConditionPublished = 0;
            intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_NO_CONNECTIVITY), TRUE);
        }
    }

    //?? ConnectivityManager_EXTRA_INET_CONDITION
    //intent->PutBooleanExtra(String(ConnectivityManager_EXTRA_INET_CONDITION), mDefaultInetConditionPublished);
    SendStickyBroadcast((IIntent *)intent);
    /*
     * If the failover network is already connected, then immediately send
     * out a followup broadcast indicating successful failover
     */
    if (mActiveDefaultNetwork != -1) {
        AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo((*mNetTrackers)[mActiveDefaultNetwork]);
        SendConnectedBroadcast((INetworkInfo*)ni);
    }
}

//private
void CConnectivityService::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    assert( intent != NULL);
    //synchronized(this) {
    Mutex::Autolock l(mutex);
    {
        if (!mSystemReady) {
            CIntent::New(intent, (IIntent **) &mInitialBroadcast);
        }
        intent->AddFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
        //??
        //mContext->SendStickyBroadcast(intent);
    }
}

ECode CConnectivityService::SystemReady()
{
//    synchronized(this) {
    Mutex::Autolock l(mutex);
    {
        mSystemReady = TRUE;
        //??
        //if (mInitialBroadcast->Get() != NULL) {
        //    mContext->SendStickyBroadcast((IIntent*) mInitialBroadcast);
        //    mInitialBroadcast->Release();
        //}
    }
    return NOERROR;
}

//private
void CConnectivityService::HandleConnect(
    /* [in] */ INetworkInfo * info)
{
    assert( info != NULL);
    Int32 type;
    info->GetType(&type);

    // snapshot isFailover, because sendConnectedBroadcast() resets it
    Boolean isFailover;
    info->IsFailover(&isFailover);
    INetworkStateTracker* thisNet = (*mNetTrackers)[type];

    // if this is a default net and other default is running
    // kill the one not preferred
    if ((*mNetAttributes)[type]->IsDefault()) {
        if (mActiveDefaultNetwork != -1 && mActiveDefaultNetwork != type) {
            if ((type != mNetworkPreference &&
                    (*mNetAttributes)[mActiveDefaultNetwork]->mPriority >
                    (*mNetAttributes)[type]->mPriority) ||
                    mNetworkPreference == mActiveDefaultNetwork) {
                    String typeName;
                    info->GetTypeName(&typeName);
                    // don't accept this one
                    if (DBG) Slogger::V(TAG, StringBuffer("Not broadcasting CONNECT_ACTION ") +
                            String("to torn down network ") + typeName);
                    Teardown(thisNet);
                    return;
            } else {
                // tear down the other
                INetworkStateTracker* otherNet =
                        (*mNetTrackers)[mActiveDefaultNetwork];
                AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo(otherNet);
                String typeName;
                ni->GetTypeName(&typeName);

                if (DBG) Slogger::V(TAG, StringBuffer("Policy requires ") + typeName + String(" teardown"));
                if (!Teardown(otherNet)) {
                    Slogger::E(TAG, "Network declined teardown request");
                    Teardown(thisNet);
                    return;
                }
                if (isFailover) {
                    otherNet->ReleaseWakeLock();
                }
            }
        }
        mActiveDefaultNetwork = type;
        // this will cause us to come up initially as unconnected and switching
        // to connected after our normal pause unless somebody reports us as reall
        // disconnected
        mDefaultInetConditionPublished = 0;
        mDefaultConnectionSequence++;
        mInetConditionChangeInFlight = FALSE;
        // Don't do this - if we never sign in stay, grey
        //reportNetworkCondition(mActiveDefaultNetwork, 100);
    }
    thisNet->SetTeardownRequested(FALSE);
    thisNet->UpdateNetworkSettings();
    HandleConnectivityChange(type);
    SendConnectedBroadcast(info);
}

//private
void CConnectivityService::HandleScanResultsAvailable(
    /* [in] */ INetworkInfo* info)
{
    assert( info != NULL);
    Int32 networkType;
    info->GetType(&networkType);
    if (networkType != ConnectivityManager_TYPE_WIFI) {
        String typeName;
        info->GetTypeName(&typeName);
        if (DBG) Slogger::V(TAG, StringBuffer("Got ScanResultsAvailable for ") +
                typeName + String(" network. Don't know how to handle."));
    }

    (*mNetTrackers)[networkType]->InterpretScanResultsAvailable();
}

//private
void CConnectivityService::HandleNotificationChange(
    /* [in] */ Boolean visible,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    AutoPtr<INotificationManager> notificationManager;
    mContext->GetSystemService(Context_NOTIFICATION_SERVICE,(IInterface**)&notificationManager);
    if (visible) {
        //??
        //notificationManager->Notify(id, notification);
    } else {
        //??
        //notificationManager->Cancel(id);
    }
}

/**
 * After a change in the connectivity state of any network, We're mainly
 * concerned with making sure that the list of DNS servers is setupup
 * according to which networks are connected, and ensuring that the
 * right routing table entries exist.
 */
//private
void CConnectivityService::HandleConnectivityChange(
    /* [in] */ Int32 netType)
{
    /*
     * If a non-default network is enabled, add the host routes that
     * will allow it's DNS servers to be accessed.
     */
    HandleDnsConfigurationChange(netType);
    AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo((*mNetTrackers)[netType]);
    if (NetworkInfoIsConnected((INetworkInfo *)ni)) {
        if ((*mNetAttributes)[netType]->IsDefault()) {
            (*mNetTrackers)[netType]->AddDefaultRoute();
        } else {
            // many radios add a default route even when we don't want one.
            // remove the default Int32erface unless we need it for our active network
            if (mActiveDefaultNetwork != -1) {
                String defaultIface;
                (*mNetTrackers)[mActiveDefaultNetwork]->GetInterfaceName(&defaultIface);
                if (!defaultIface.IsNullOrEmpty()) {
                    String defaultIfaceEx;
                    (*mNetTrackers)[netType]->GetInterfaceName(&defaultIfaceEx);
                    if (!defaultIface.Equals(defaultIfaceEx)) {
                        (*mNetTrackers)[netType]->RemoveDefaultRoute();
                    }
                }
            }
            (*mNetTrackers)[netType]->AddPrivateDnsRoutes();
        }
    } else {
        if ((*mNetAttributes)[netType]->IsDefault()) {
            (*mNetTrackers)[netType]->RemoveDefaultRoute();
        } else {
            (*mNetTrackers)[netType]->RemovePrivateDnsRoutes();
        }
    }
}

/**
 * Adjust the per-process dns entries (net.dns<x>.<pid>) based
 * on the highest priority active net which this process requested.
 * If there aren't any, clear it out
 */
//private
void CConnectivityService::ReassessPidDns(
    /* [in] */ Int32 myPid,
    /* [in] */ Boolean doBump)
{
    if (DBG) Slogger::D(TAG, StringBuffer("reassessPidDns for pid ") + String::FromInt32(myPid));
    for(Int32 index = 0; index<(Int32)mPriorityList->GetLength(); index++) {
        Int32 i = (*mPriorityList)[i];
        if ((*mNetAttributes)[i]->IsDefault()) {
            continue;
        }
        INetworkStateTracker *nt = (*mNetTrackers)[i];
        AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo(nt);
        if (NetworkInfoIsConnected(ni) && NetworkStateTrackerIsTeardownRequested(nt)==FALSE) {
            List<Int32>* pids = (*mNetRequestersPids)[i];
            for (Int32 j=0; j<(Int32)pids->GetSize(); j++) {
                Int32 pid = (*pids)[j];
                if (pid == myPid) {
                    //??
                    ArrayOf<String> * dnsList;
                    nt->GetNameServers(&dnsList);
                    WritePidDns(*dnsList, myPid);
                    if (doBump) {
                        BumpDns();
                    }
                    return;
                }
            }
        }
    }
    // nothing found - delete
    for (Int32 i = 1; ; i++) {
        String prop = String("net.dns") + String::FromInt32(i) + String(".") + String::FromInt32(myPid);
        if (SystemProperties::Get(CString(prop.string())).GetLength() == 0) {
            if (doBump) {
                BumpDns();
            }
            return;
        }
        SystemProperties::Set(CString(prop.string()), CString(""));
    }
}

//private
void CConnectivityService::WritePidDns(
    /* [in] */ const ArrayOf<String> &dnsList,
    /*[ in] */ Int32 pid)
{
    Int32 j = 1;

    for (Int32 i = 0; i <(Int32) dnsList.GetLength(); i++) {
        String dns = dnsList[i];
        if (!dns.IsNullOrEmpty()) {
            if (!TextUtils::Equals(
                    (ICharSequence*)StringUtil::StringToCharSeuence(dns),
                    (ICharSequence*)StringUtil::StringToCharSeuence(String("0.0.0.0"))) ) {
                String str1 = String("net.dns") +
                        String::FromInt32(j) +
                        String(".") +
                        String::FromInt32(pid);
                SystemProperties::Set(
                    CString(str1.string()),
                    CString(dns.string()));
                j++;
            }
        }
    }
}

//private
void CConnectivityService::BumpDns()
{
    /*
     * Bump the property that tells the name resolver library to reread
     * the DNS server list from the properties.
     */
    String propVal = SystemProperties::Get(CString("net.dnschange"));
    Int32 n = 0;
    if (propVal.GetLength() != 0) {
        n = propVal.ToInt32();
    }
    String str1 = String("") + String::FromInt32(n+1);
    SystemProperties::Set(CString("net.dnschange"), CString(str1.string()) );
}

//private
void CConnectivityService::HandleDnsConfigurationChange(
    /* [in] */ Int32 netType)
{
    // add default net's dns entries
    INetworkStateTracker* nt = (*mNetTrackers)[netType];
    AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo(nt);
    if (nt != NULL && NetworkInfoIsConnected(ni) && !NetworkStateTrackerIsTeardownRequested(nt)) {
        ArrayOf<String> * dnsList;
        nt->GetNameServers(&dnsList);
        if ((*mNetAttributes)[netType]->IsDefault()) {
            Int32 j = 1;
            for (Int32 i = 0; i< dnsList->GetLength(); i++) {
                String dns = (*dnsList)[i];
                if (!dns.IsNullOrEmpty()) {
                    if (!TextUtils::Equals(
                          (ICharSequence *) StringUtil::StringToCharSeuence(dns),
                          (ICharSequence *) StringUtil::StringToCharSeuence(String("0.0.0.0")) )) {
                        if (DBG) {
                            String typeName;
                            ni->GetTypeName(&typeName);
                            Slogger::D(TAG, StringBuffer("adding dns ") + dns + String(" for ") +
                                typeName);
                        }
                    }
                    String str1 = String("net.dns") + String::FromInt32(j);
                    SystemProperties::Set(CString(str1.string()), CString(dns.string()));
                    j++;
                }
            }
            for (Int32 k=j ; k<mNumDnsEntries; k++) {
                if (DBG) Slogger::D(TAG, StringBuffer("erasing net.dns") + String::FromInt32(k) );
                String str1 = String("net.dns") + String::FromInt32(k);
                SystemProperties::Set(CString(str1.string()), CString(""));
            }
            mNumDnsEntries = j;
        } else {
            // set per-pid dns for attached secondary nets
            List<Int32>* pids = (*mNetRequestersPids)[netType];
            for (Int32 y=0; y< (Int32)pids->GetSize(); y++) {
                Int32 pid = (*pids)[y];
                WritePidDns(*dnsList, pid);
            }
        }
    }
    BumpDns();
}

//private
Int32 CConnectivityService::GetRestoreDefaultNetworkDelay()
{
    String restoreDefaultNetworkDelayStr = SystemProperties::Get(
            CString(NETWORK_RESTORE_DELAY_PROP_NAME));
    if(!restoreDefaultNetworkDelayStr.IsNullOrEmpty() ) {
        if ( restoreDefaultNetworkDelayStr.GetLength() != 0) {
            return restoreDefaultNetworkDelayStr.ToInt32();
        }
    }
    return RESTORE_DEFAULT_NETWORK_DELAY;
}

//@Override
//protected
void CConnectivityService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const ArrayOf<String>& args)
{
    return;
//??
/*
    if (mContext.checkCallingOrSelfPermission(
            android.Manifest.permission.DUMP)
            != PackageManager.PERMISSION_GRANTED) {
        pw.prInt32ln("Permission Denial: can't dump ConnectivityService " +
                "from from pid=" + Binder.getCallingPid() + ", uid=" +
                Binder.getCallingUid());
        return;
    }
    pw.prInt32ln();
    for (INetworkStateTracker nst : mNetTrackers) {
        if (nst != NULL) {
            if (nst.getNetworkInfo().isConnected()) {
                pw.prInt32ln("Active network: " + nst.getNetworkInfo().
                        getTypeName());
            }
            pw.prInt32ln(nst.getNetworkInfo());
            pw.prInt32ln(nst);
            pw.prInt32ln();
        }
    }

    pw.prInt32ln("Network Requester Pids:");
    for (Int32 net : mPriorityList) {
        String pidString = net + ": ";
        for (Object pid : mNetRequestersPids[net]) {
            pidString = pidString + pid.toString() + ", ";
        }
        pw.prInt32ln(pidString);
    }
    pw.prInt32ln();

    pw.prInt32ln("FeatureUsers:");
    for (Object requester : mFeatureUsers) {
        pw.prInt32ln(requester.toString());
    }
    pw.prInt32ln();

    mTethering.dump(fd, pw, args);

    if (mInetLog != NULL) {
        pw.prInt32ln();
        pw.prInt32ln("Inet condition reports:");
        for(Int32 i = 0; i < mInetLog.size(); i++) {
            pw.prInt32ln(mInetLog.get(i));
        }
    }
*/
}

// must be stateless - things change under us.
//??
//@Override
ECode CConnectivityService::MyHandler::HandleMessage(
    /* [in] */ IInterface * obj,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{

    return NOERROR;
/*
    INetworkInfo * info;
    switch (what) {
        case INetworkStateTracker_EVENT_STATE_CHANGED:
            info = (INetworkInfo*) obj;
            Int32 type;
            info->GetType(&type);
            NetworkInfoState state;
            info->GetState(&state);
            String typeName;
            info->GetTypeName(&typeName);

            //??
            //NetworkInfoDetailedState nidstate
            //info->GetDetailedState(&nidstate);

            Int32 subtype;
            info->GetSubtype(&subtype);
            // only do this optimization for wifi.  It going Int32o scan mode for location
            // services generates alot of noise.  Meanwhile the mms apn won't send out
            // subsequent notifications when on default cellular because it never
            // disconnects..  so only do this to wifi notifications.  Fixed better when the
            // APN notifications are standardized.
            if ((*mNetAttributes)[type]->mLastState == state &&
                    (*mNetAttributes)[type]->mRadio == ConnectivityManager_TYPE_WIFI) {
                if (DBG) {
                    // TODO - remove this after we validate the dropping doesn't break
                    // anything
                    Slogger::D(TAG, StringBuffer("Dropping ConnectivityChange for ") +
                            typeName + String(": ") +
                            String::FromInt32(state) + String("/") +String::FromInt32(nidstate));
                }
                return;
            }
            (*mNetAttributes)[type]->mLastState = state;

            if (DBG) Slogger::D(TAG, StringBuffer("ConnectivityChange for ") +
                    typeName + String(": ") +
                    String::FromInt32(state) + String("/") + String::FromInt32(nidstate));

            // Connectivity state changed:
            // [31-13] Reserved for future use
            // [12-9] Network subtype (for mobile network, as defined
            //         by TelephonyManager)
            // [8-3] Detailed state ordinal (as defined by
            //         NetworkInfo.DetailedState)
            // [2-0] Network type (as defined by ConnectivityManager)
            Int32 eventLogParam = (type & 0x7) |
                    //((info.getDetailedState().ordinal() & 0x3f) << 3) |
                    ((nidstate & 0x3f) << 3)|
                    subtype << 9);
            //??
            //EventLog.writeEvent(EventLogTags_CONNECTIVITY_STATE_CHANGED,
            //        eventLogParam);

            if (nidstate ==
                    NetworkInfoDetailedState_FAILED) {
                HandleConnectionFailure(info);
            } else if (state == NetworkInfoState_DISCONNECTED) {
                HandleDisconnect(info);
            } else if (state == NetworkInfoState_SUSPENDED) {
                // TODO: need to think this over.
                // the logic here is, handle SUSPENDED the same as
                // DISCONNECTED. The only difference being we are
                // broadcasting an Int32ent with NetworkInfo that's
                // suspended. This allows the applications an
                // opportunity to handle DISCONNECTED and SUSPENDED
                // differently, or not.
                HandleDisconnect(info);
            } else if (state == NetworkInfoState_CONNECTED) {
                HandleConnect(info);
            }

            break;

        case INetworkStateTracker_EVENT_SCAN_RESULTS_AVAILABLE:
            info = (INetworkInfo*)obj;
            HandleScanResultsAvailable(info);
            break;

        case INetworkStateTracker_EVENT_NOTIFICATION_CHANGED:
            HandleNotificationChange(arg1 == 1, arg2,
                    (INotification*) obj);
            break;

        case INetworkStateTracker_EVENT_CONFIGURATION_CHANGED:
            info = (INetworkInfo*)obj;
            HandleDnsConfigurationChange(type);
            break;

        case INetworkStateTracker_EVENT_ROAMING_CHANGED:
            // fill me in
            break;

        case INetworkStateTracker_EVENT_NETWORK_SUBTYPE_CHANGED:
            // fill me in
            break;
        case EVENT_RESTORE_DEFAULT_NETWORK:
            FeatureUser * u = (FeatureUser*)obj;
            u->Expire();
            break;
        case EVENT_INET_CONDITION_CHANGE:
        {
            Int32 netType = arg1;
            Int32 condition = arg2;
            HandleInetConditionChange(netType, condition);
            break;
        }
        case EVENT_INET_CONDITION_HOLD_END:
        {
            Int32 netType = arg1;
            Int32 sequence = arg2;
            HandleInetConditionHoldEnd(netType, sequence);
            break;
        }
        case EVENT_SET_NETWORK_PREFERENCE:
        {
            Int32 preference = arg1;
            HandleSetNetworkPreference(preference);
            break;
        }
        case EVENT_SET_BACKGROUND_DATA:
        {
            Boolean enabled = (arg1 == ENABLED);
            HandleSetBackgroundData(enabled);
            break;
        }
        case EVENT_SET_MOBILE_DATA:
        {
            Boolean enabled = (arg1 == ENABLED);
            HandleSetMobileData(enabled);
            break;
        }
    }
*/
}

// javadoc from Int32erface
ECode CConnectivityService::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* ret )
{
    VALIDATE_NOT_NULL(ret);
    EnforceTetherChangePermission();

    if (IsTetheringSupported()) {
        //mTethering->Tether(iface, ret);
    } else {
        *ret = ConnectivityManager_TETHER_ERROR_UNSUPPORTED;
    }
    return NOERROR;
}

// javadoc from Int32erface
ECode CConnectivityService::Untether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* ret )
{
    VALIDATE_NOT_NULL(ret);
    EnforceTetherChangePermission();

    if (IsTetheringSupported()) {
        //mTethering->Untether(iface, ret);
    } else {
        *ret = ConnectivityManager_TETHER_ERROR_UNSUPPORTED;
    }
    return NOERROR;
}

// javadoc from Int32erface
ECode CConnectivityService::GetLastTetherError(
    /* [in] */ const String& iface,
    /* [out] */ Int32* ret )
{
    VALIDATE_NOT_NULL(ret);
    EnforceTetherAccessPermission();

    if (IsTetheringSupported()) {
        //mTethering->GetLastTetherError(iface, ret);
    } else {
        *ret = ConnectivityManager_TETHER_ERROR_UNSUPPORTED;
    }
    return NOERROR;
}

// TODO - proper iface API for selection by property, inspection, etc
ECode CConnectivityService::GetTetherableUsbRegexs(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    EnforceTetherAccessPermission();
    if (IsTetheringSupported()) {
        //mTethering->GetTetherableUsbRegexs(ret);
    } else {
        *ret = NULL;
        //return new String[0];
    }
    return NOERROR;
}

ECode CConnectivityService::GetTetherableWifiRegexs(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    EnforceTetherAccessPermission();
    if (IsTetheringSupported()) {
        //mTethering->GetTetherableWifiRegexs(ret);
    } else {
        *ret = NULL;
        //return new String[0];
    }
    return NOERROR;
}

// TODO - move iface listing, queries, etc to new module
// javadoc from Int32erface
ECode CConnectivityService::GetTetherableIfaces(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    EnforceTetherAccessPermission();
    //return mTethering->GetTetherableIfaces(ret);
    return NOERROR;
}

ECode CConnectivityService::GetTetheredIfaces(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    EnforceTetherAccessPermission();
    //return mTethering->etTetheredIfaces(ret);
    return NOERROR;
}

ECode CConnectivityService::GetTetheringErroredIfaces(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    EnforceTetherAccessPermission();
    //return mTethering->GetErroredIfaces(ret);
    return NOERROR;
}

// if ro.tether.denied = TRUE we default to no tethering
// gservices could set the secure setting to 1 though to enable it on a build where it
// had previously been turned off.
ECode CConnectivityService::IsTetheringSupported(
    /* [out] */ Boolean *ret )
{
    VALIDATE_NOT_NULL(ret);
    *ret = IsTetheringSupported();
    return NOERROR;
}

Boolean CConnectivityService::IsTetheringSupported()
{
    EnforceTetherAccessPermission();
    Int32 defaultVal = (SystemProperties::Get(CString("ro.tether.denied")).Equals(String("TRUE")) ? 0 : 1);
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 tmp;
    SettingsSecure()->GetInt32(
                (IContentResolver*) cr,
                String(SettingsSecure_TETHER_SUPPORTED),
                defaultVal,
                &tmp);
    Boolean tetherEnabledInSettings = (tmp!= 0);
    return tetherEnabledInSettings && mTetheringConfigValid;
}

// 100 percent is full good, 0 is full bad.
ECode CConnectivityService::ReportInetCondition(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 percentage)
{
    if (DBG) Slogger::D(TAG, StringBuffer("reportNetworkCondition(") +
            String::FromInt32(networkType) +
            String(", ") +
            String::FromInt32(percentage) +
            String(")") );
    //??
    //mContext->EnforceCallingOrSelfPermission(
    //        CString("elastos.permission.STATUS_BAR")//android.Manifest.permission.STATUS_BAR,
    //        CString("ConnectivityService"));

    if (DBG) {
        Int32 pid = Binder::GetCallingPid();
        Int32 uid = Binder::GetCallingUid();
        AutoPtr<IGregorianCalendar> c;
        CGregorianCalendar::New((IGregorianCalendar **)&c);
        AutoPtr<IDate> dt;
        c->GetTime((IDate **)&dt);
        String strdt;
        dt->ToString(&strdt);
        String str = (percentage > 50 ? String("connected") : String("disconnected") );
        String s = String::FromInt32(pid) + String("(") +
                   String::FromInt32(uid) + String(") reports inet is ") +
                   str + String(" (") +
                   String::FromInt32(percentage) + String(") on ") +
                   String("network Type ") + String::FromInt32(networkType) +
                   String(" at ") + strdt;
        mInetLog->PushBack(s);
        while((Int32)mInetLog->GetSize() > INET_CONDITION_LOG_MAX_SIZE) {
            mInetLog->Remove(0);
        }
    }
    //??
    //mHandler.sendMessage(mHandler.obtainMessage(
    //    EVENT_INET_CONDITION_CHANGE, networkType, percentage));
    return NOERROR;
}

//private
void CConnectivityService::HandleInetConditionChange(
    /* [in] */ Int32 netType,
    /* [in] */ Int32 condition)
{
    if (DBG) {
        Slogger::D(TAG, StringBuffer("Inet connectivity change, net=") +
                String::FromInt32(netType) +
                String(", condition=") +
                String::FromInt32(condition) +
                String(",mActiveDefaultNetwork=") +
                String::FromBoolean(mActiveDefaultNetwork));
    }
    if (mActiveDefaultNetwork == -1) {
        if (DBG) Slogger::D(TAG,CString("no active default network - aborting"));
        return;
    }
    if (mActiveDefaultNetwork != netType) {
        if (DBG) Slogger::D(TAG, CString("given net not default - aborting"));
        return;
    }
    mDefaultInetCondition = condition;
    Int32 delay;
    if (mInetConditionChangeInFlight == FALSE) {
        AutoPtr<IContentResolver> cr;
        mContext->GetContentResolver((IContentResolver**)&cr);
        if (DBG) Slogger::D(TAG, CString("starting a change hold"));
        // setup a new hold to debounce this
        if (mDefaultInetCondition > 50) {

            SettingsSecure()->GetInt32((IContentResolver*) cr,
                    String(SettingsSecure_INET_CONDITION_DEBOUNCE_UP_DELAY), 500, &delay);
        } else {
            SettingsSecure()->GetInt32((IContentResolver*) cr,
            String(SettingsSecure_INET_CONDITION_DEBOUNCE_DOWN_DELAY), 3000, &delay);
        }
        mInetConditionChangeInFlight = TRUE;
        //??
        //mHandler.sendMessageDelayed(mHandler.obtainMessage(EVENT_INET_CONDITION_HOLD_END,
        //        mActiveDefaultNetwork, mDefaultConnectionSequence), delay);
    } else {
        // we've set the new condition, when this hold ends that will get
        // picked up
        if (DBG) Slogger::D(TAG, CString("currently in hold - not setting new end evt"));
    }
}

//private
void CConnectivityService::HandleInetConditionHoldEnd(
    /* [in] */ Int32 netType,
    /* [in] */ Int32 sequence)
{
    if (DBG) {
        Slogger::D(TAG, StringBuffer("Inet hold end, net=") +
                String::FromInt32(netType) +
                String(", condition =") +
                String::FromInt32(mDefaultInetCondition) +
                String(", published condition =") +
                String::FromBoolean(mDefaultInetConditionPublished));
    }
    mInetConditionChangeInFlight = FALSE;

    if (mActiveDefaultNetwork == -1) {
        if (DBG) Slogger::D(TAG, CString("no active default network - aborting"));
        return;
    }
    if (mDefaultConnectionSequence != sequence) {
        if (DBG) Slogger::D(TAG, CString("event hold for obsolete network - aborting"));
        return;
    }
    if (mDefaultInetConditionPublished == mDefaultInetCondition) {
        if (DBG) Slogger::D(TAG, CString("no change in condition - aborting"));
        return;
    }
    AutoPtr<INetworkInfo> ni = NetworkStateTrackerGetNetworkInfo((*mNetTrackers)[mActiveDefaultNetwork]);
    if (NetworkInfoIsConnected((INetworkInfo*)ni) == FALSE) {
        if (DBG) Slogger::D(TAG, CString("default network not connected - aborting"));
        return;
    }
    mDefaultInetConditionPublished = mDefaultInetCondition;
    SendInetConditionBroadcast(ni);
    return;
}


CConnectivityService::ConnectivityThread::~ConnectivityThread()
{

}

/*
Mutex* CConnectivityService::ConnectivityThread::GetSelfLock()
{
    return &_m_syncLock;
}
*/

PInterface CConnectivityService::ConnectivityThread::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IThread) {
        return (IThread*)this;
    }

    return NULL;
}

UInt32 CConnectivityService::ConnectivityThread::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CConnectivityService::ConnectivityThread::Release()
{
    return ElRefBase::Release();
}

ECode CConnectivityService::ConnectivityThread::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *iID)
{
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IThread*)this) {
        *iID = EIID_IThread;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}
ECode CConnectivityService::ConnectivityThread::CheckAccess()
{
    return Thread::CheckAccess();
}

ECode CConnectivityService::ConnectivityThread::CountStackFrames(
    /* [out] */ Int32* number)
{
    return Thread::CountStackFrames(number);
}

ECode CConnectivityService::ConnectivityThread::Destroy()
{
    return Thread::Destroy();
}

ECode CConnectivityService::ConnectivityThread::GetId(
    /* [out] */ Int64* id)
{
    return Thread::GetId(id);
}

ECode CConnectivityService::ConnectivityThread::GetName(
    /* [out] */ String* name)
{
    return Thread::GetName(name);
}

ECode CConnectivityService::ConnectivityThread::GetPriority(
    /* [out] */ Int32* priority)
{
    return Thread::GetPriority(priority);
}

ECode CConnectivityService::ConnectivityThread::GetState(
    /* [out] */ ThreadState* state)
{
    return Thread::GetState(state);
}

ECode CConnectivityService::ConnectivityThread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    return Thread::GetThreadGroup(group);
}

ECode CConnectivityService::ConnectivityThread::Interrupt()
{
    return Thread::Interrupt();
}

ECode CConnectivityService::ConnectivityThread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    return Thread::IsAlive(isAlive);
}

ECode CConnectivityService::ConnectivityThread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return Thread::IsDaemon(isDaemon);
}

ECode CConnectivityService::ConnectivityThread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    return Thread::IsInterrupted(isInterrupted);
}

ECode CConnectivityService::ConnectivityThread::Join()
{
    return Thread::Join();
}

ECode CConnectivityService::ConnectivityThread::JoinEx(
    /* [in] */ Int64 millis)
{
    return Thread::JoinEx(millis);
}

ECode CConnectivityService::ConnectivityThread::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::JoinEx2(millis, nanos);
}

ECode CConnectivityService::ConnectivityThread::Resume()
{
    return Thread::Resume();
}

ECode CConnectivityService::ConnectivityThread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return Thread::SetDaemon(isDaemon);
}

ECode CConnectivityService::ConnectivityThread::SetName(
    /* [in] */ const String& threadName)
{
    return Thread::SetName(threadName);
}

ECode CConnectivityService::ConnectivityThread::SetPriority(
    /* [in] */ Int32 priority)
{
    return Thread::SetPriority(priority);
}

ECode CConnectivityService::ConnectivityThread::Start()
{
    return Thread::Start();
}

ECode CConnectivityService::ConnectivityThread::Stop()
{
    return Thread::Stop();
}

ECode CConnectivityService::ConnectivityThread::Suspend()
{
    return Thread::Suspend();
}

ECode CConnectivityService::ConnectivityThread::Wait(
    /* [in] */ Int64 time,
    /* [in] */ Int32 frac)
{
    return Thread::Wait(time, frac);
}

ECode CConnectivityService::ConnectivityThread::Detach()
{
    return Thread::Detach();
}

ECode CConnectivityService::ConnectivityThread::Lock()
{
    return Thread::Lock();
}

ECode CConnectivityService::ConnectivityThread::Unlock()
{
    return Thread::Unlock();
}

ECode CConnectivityService::ConnectivityThread::Notify()
{
    return Thread::Notify();
}

ECode CConnectivityService::ConnectivityThread::NotifyAll()
{
    return Thread::NotifyAll();
}

ECode CConnectivityService::ConnectivityThread::Wait()
{
    return Thread::Wait();
}

ECode CConnectivityService::ConnectivityThread::WaitEx(
    /* [in] */ Int64 millis)
{
    return Thread::Wait(millis);
}

ECode CConnectivityService::ConnectivityThread::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Wait(millis, nanos);
}

Mutex* CConnectivityService::ConnectivityThread::GetSelfLock()
{
    return &mLock;
}

AutoPtr<ISettingsSecure> CConnectivityService::SettingsSecure()
{
    AutoPtr<ISettingsSecure> ss;
    CSettingsSecure::AcquireSingleton((ISettingsSecure **)&ss);
    return ss;
}

AutoPtr<ILocalConnectivityManagerHelper> CConnectivityService::ConnectivityManager()
{
    AutoPtr<ILocalConnectivityManagerHelper> lmh;
    CConnectivityManagerHelper::AcquireSingleton((ILocalConnectivityManagerHelper **)&lmh);
    return lmh;
}

Boolean CConnectivityService::ConnectivityManagerIsNetworkTypeValid(
    /* [in] */ Int32 networkType)
{
    Boolean ret;
    ConnectivityManager()->IsNetworkTypeValid(
            networkType, &ret);
    return ret;
}

AutoPtr<INetworkInfo> CConnectivityService::NetworkStateTrackerGetNetworkInfo(
    /* [in] */ INetworkStateTracker* nst)
{
    assert(nst != NULL);
    AutoPtr<INetworkInfo> ni;
    nst->GetNetworkInfo((INetworkInfo **)&ni);
    return ni;
}

Boolean CConnectivityService::NetworkStateTrackerIsAvailable(
    /* [in] */ INetworkStateTracker* nst)
{
    assert(nst != NULL);
    Boolean ret;
    nst->IsAvailable(&ret);
    return ret;
}

Boolean CConnectivityService::NetworkStateTrackerTeardown(
    /* [in] */ INetworkStateTracker* nst)
{
    assert(nst != NULL);
    Boolean ret;
    nst->Teardown(&ret);
    return ret;
}

Boolean CConnectivityService::NetworkStateTrackerIsTeardownRequested(
    /* [in] */ INetworkStateTracker* nst)
{
    assert(nst != NULL);
    Boolean ret;
    nst->IsTeardownRequested(&ret);
    return ret;
}

Boolean CConnectivityService::NetworkInfoIsConnected(
   /* [in] */ INetworkInfo *ni)
{
    assert(ni != NULL);
    Boolean ret;
    ni->IsConnected(&ret);
    return ret;
}

Boolean CConnectivityService::NetworkInfoIsConnectedOrConnecting(
   /* [in] */ INetworkInfo *ni)
{
    assert(ni != NULL);
    Boolean ret;
    ni->IsConnectedOrConnecting(&ret);
    return ret;
}

Boolean CConnectivityService::NetworkInfoIsAvailable(
   /* [in] */ INetworkInfo *ni)
{
    assert(ni != NULL);
    Boolean ret;
    ni->IsAvailable(&ret);
    return ret;
}

String CConnectivityService::NetworkInfoToString(
    /* [in] */ INetworkInfo *ni)
{
    assert(ni != NULL);
    String ret;

    String typeName;
    ni->GetTypeName(&typeName);
    String subtypeName;
    ni->GetSubtypeName(&subtypeName);

    ret=String("typeName:")+typeName+String(" subtypeName:")+subtypeName;
    return ret;
}

String CConnectivityService::NetworkStateTrackerToString(
    /* [in] */ INetworkStateTracker* nst)
{
    return NetworkInfoToString((INetworkInfo *)NetworkStateTrackerGetNetworkInfo(nst));
}

