
#include <ext/frameworkext.h>
#include "server/connectivity/CTethering.h"
#include <Elastos.Framework.Core.h>
//import com.android.internal.app.IBatteryStats;
//import com.android.server.am.BatteryStatsService;
#include "server/CBatteryStatsService.h"
#include "app/ActivityManagerNative.h"
#include "server/CActivityManagerService.h"
//#include "content/ContentResolver.h"
//#include "content/Context.h"
//#include <content/CIntent.h>
//import android.content.pm.PackageManager;
//import android.os.BatteryManager;
//import android.os.Binder;
#include "os/FileUtils.h"
#include "os/Environment.h"
//import android.os.IBinder;
//import android.os.DropBoxManager;
//import android.os.RemoteException;
//import android.os.ServiceManager;
#include "os/ServiceManager.h"
#include "os/SystemClock.h"
//import android.os.UEventObserver;
//#include "provider/CSettingsSecure.h"
//import android.provider.Settings;
//import android.util.EventLog;
//import android.util.Slog;
#include <Slogger.h>
#include <Logger.h>
#include <StringBuffer.h>
#include <elastos/Mutex.h>

//import java.io.File;
//import java.io.FileDescriptor;
//import java.io.FileInputStream;
//import java.io.FileOutputStream;
//import java.io.IOException;
//import java.io.PrintWriter;

//for jni
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

/*
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.hardware.usb.UsbManager;
import android.net.ConnectivityManager;
import android.net.InterfaceConfiguration;
import android.net.IConnectivityManager;
import android.net.INetworkManagementEventObserver;
import android.net.NetworkInfo;
import android.net.NetworkUtils;
import android.os.Binder;
import android.os.Environment;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.INetworkManagementService;
import android.os.Looper;
import android.os.Message;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.provider.Settings;
import android.util.Log;

import com.android.internal.telephony.Phone;
import com.android.internal.util.HierarchicalState;
import com.android.internal.util.HierarchicalStateMachine;

import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;
*/

const CString CTethering::TAG = CString("Tethering");
const String CTethering::USB_NEAR_IFACE_ADDR = String("192.168.42.129");
const String CTethering::USB_NETMASK = String("255.255.255.0");
// FYI - the default wifi is 192.168.43.1 and 255.255.255.0
const String CTethering::DHCP_DEFAULT_RANGE1_START = String("192.168.42.2");
const String CTethering::DHCP_DEFAULT_RANGE1_STOP  = String("192.168.42.254");
const String CTethering::DHCP_DEFAULT_RANGE2_START = String("192.168.43.2");
const String CTethering::DHCP_DEFAULT_RANGE2_STOP  = String("192.168.43.254");
const String CTethering::DNS_DEFAULT_SERVER1 = String("8.8.8.8");
const String CTethering::DNS_DEFAULT_SERVER2 = String("4.2.2.2");

const Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TRY_TO_SETUP_MOBILE_CONNECTION = TRUE;
const Boolean CTethering::TetherMasterSM::TetherMasterUtilState::WAIT_FOR_NETWORK_TO_SETTLE = FALSE;

// notification from the master SM that it's not in tether mode
const Int32 CTethering::TetherInterfaceSM::CMD_TETHER_MODE_DEAD;
// request from the user that it wants to tether
const Int32 CTethering::TetherInterfaceSM::CMD_TETHER_REQUESTED;
// request from the user that it wants to untether
const Int32 CTethering::TetherInterfaceSM::CMD_TETHER_UNREQUESTED;
// notification that this interface is down
const Int32 CTethering::TetherInterfaceSM::CMD_INTERFACE_DOWN;
// notification that this interface is up
const Int32 CTethering::TetherInterfaceSM::CMD_INTERFACE_UP;
// notification from the master SM that it had an error turning on cellular dun
const Int32 CTethering::TetherInterfaceSM::CMD_CELL_DUN_ERROR;
// notification from the master SM that it had trouble enabling IP Forwarding
const Int32 CTethering::TetherInterfaceSM::CMD_IP_FORWARDING_ENABLE_ERROR;
// notification from the master SM that it had trouble disabling IP Forwarding
const Int32 CTethering::TetherInterfaceSM::CMD_IP_FORWARDING_DISABLE_ERROR;
// notification from the master SM that it had trouble staring tethering
const Int32 CTethering::TetherInterfaceSM::CMD_START_TETHERING_ERROR;
// notification from the master SM that it had trouble stopping tethering
const Int32 CTethering::TetherInterfaceSM::CMD_STOP_TETHERING_ERROR;
// notification from the master SM that it had trouble setting the DNS forwarders
const Int32 CTethering::TetherInterfaceSM::CMD_SET_DNS_FORWARDERS_ERROR;
// the upstream connection has changed
const Int32 CTethering::TetherInterfaceSM::CMD_TETHER_CONNECTION_CHANGED;

/**
 * @hide
 *
 * Timeout
 *
 * TODO - look for parent classes and code sharing
 */

ECode CTethering::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IApartment* looper)
{
    Boolean mBooted = FALSE;
    //used to remember if we got connected before boot finished
    Boolean mDeferedUsbConnection = FALSE;

    mContext = context;
    //??
    mLooper = looper;

    // register for notifications from NetworkManagement Service
    //??
    //IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    //INetworkManagementService service = INetworkManagementService.Stub.asInterface(b);
    //try {
    //    service.registerObserver(this);
    //} catch (RemoteException e) {
    //    Logger::E(TAG, "Error registering observer :" + e);
    //}

    //AutoPtr<INetworkManagementService> service = ServiceManager::GetService(String(Context_NETWORKMANAGEMENT_SERVICE));

    //mIfaces = new HashMap<String, TetherInterfaceSM *>();

    //??
    // make our own thread so we don't anr the system
/*
    mThread = new HandlerThread(String("Tethering"));
    mThread.start();
    mLooper = mThread.getLooper();
    mTetherMasterSM = new TetherMasterSM("TetherMaster", mLooper);
    mTetherMasterSM.start();
*/

    CApartment::New(TRUE, (IApartment**) &mThread);
    mThread->Start(ApartmentAttr_New);
    mTetherMasterSM = new TetherMasterSM(String("TetherMaster"), mThread, this);

    //??
    //mTetherMasterSM->Start();

    mStateReceiver = new StateReceiver(this);
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter **)&filter);
    //??
    //filter->AddAction(String(UsbManager_ACTION_USB_STATE));
    filter->AddAction(String(ConnectivityManager_CONNECTIVITY_ACTION));
    filter->AddAction(String(Intent_ACTION_BOOT_COMPLETED));

    //?? IContext no registerReceiver API available
    //mContext.registerReceiver(mStateReceiver, filter);

    AutoPtr<IIntentFilter> filter2;
    CIntentFilter::New((IIntentFilter **)&filter2);
    filter2->AddAction(String(Intent_ACTION_MEDIA_SHARED));
    filter2->AddAction(String(Intent_ACTION_MEDIA_UNSHARED));
    filter2->AddDataScheme(String("file"));

    //?? IContext no registerReceiver API available
    //mContext.registerReceiver(mStateReceiver, filter);

    //??
    mUsbMassStorageOff = !Environment::MEDIA_SHARED.Equals(
            Environment::GetExternalStorageState());

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);

    mDhcpRange = NULL;
    res->GetStringArray(
            0x0107000e, //com.android.internal.R.array.config_tether_dhcp_range);
            &mDhcpRange);
    if (mDhcpRange != NULL) {
        if ((mDhcpRange->GetLength() == 0) || (mDhcpRange->GetLength() % 2 ==1)) {
            NewDhcpRange();
        }
    } else {
        NewDhcpRange();
    }

    mDunRequired = FALSE; // resample when we turn on

    mTetherableUsbRegexs = NULL;
    res->GetStringArray(
            0x0107000c,//com.android.internal.R.array.config_tether_usb_regexs);
            &mTetherableUsbRegexs);
    mTetherableWifiRegexs = NULL;
    res->GetStringArray(
            0x0107000d,//com.android.internal.R.array.config_tether_wifi_regexs);
            &mTetherableWifiRegexs);
    mUpstreamIfaceRegexs = NULL;
    res->GetStringArray(
            0x0107000f,//com.android.internal.R.array.config_tether_upstream_regexs);
            &mUpstreamIfaceRegexs);

    // TODO - remove and rely on real notifications of the current iface
    mDnsServers = ArrayOf<String>::Alloc(2);
    (*mDnsServers)[0] = DNS_DEFAULT_SERVER1;
    (*mDnsServers)[1] = DNS_DEFAULT_SERVER2;

    return NOERROR;
}

CTethering::~CTethering()
{
    if (mDhcpRange != NULL)
        ArrayOf<String>::Free(mDhcpRange);
    if (mTetherableUsbRegexs != NULL)
        ArrayOf<String>::Free(mTetherableUsbRegexs);
    if (mTetherableWifiRegexs != NULL)
        ArrayOf<String>::Free(mTetherableWifiRegexs);
    if (mUpstreamIfaceRegexs != NULL)
        ArrayOf<String>::Free(mUpstreamIfaceRegexs);
    ArrayOf<String>::Free(mDnsServers);

    //delete mIfaces;
    delete mTetherMasterSM;
    delete mStateReceiver;
}

ECode CTethering::InterfaceLinkStatusChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean link)
{
    Logger::D(TAG, StringBuffer("interfaceLinkStatusChanged ") +
            iface + String(", ") + String::FromBoolean(link));
    Boolean found = FALSE;
    Boolean usb = FALSE;
    if (IsWifi(iface)) {
        found = TRUE;
    } else if (IsUsb(iface)) {
        found = TRUE;
        usb = TRUE;
    }
    if (found == FALSE) return NOERROR;

    //synchronized (mIfaces) {
    mIfacesMutex.Lock();
    {
        TetherInterfaceSM* sm = mIfaces[iface];
        if (link) {
            if (sm == NULL) {
                sm = new TetherInterfaceSM(iface,(IApartment *) mThread, usb, this);
                mIfaces[iface] = sm;
                //??
                //sm->Start();
            }
        } else {
            if (sm != NULL) {
                //??
                //sm->SendMessage(TetherInterfaceSM_CMD_INTERFACE_DOWN);
                mIfaces.Erase(iface);
            }
        }
    }
    mIfacesMutex.Unlock();
    return NOERROR;
}

Boolean CTethering::IsUsb(
    /* [in] */ const String& iface)
{
    //for (String regex : mTetherableUsbRegexs)
    for (Int32 i=0;i<mTetherableUsbRegexs->GetLength(); i++)
    {
        String regex = (*mTetherableUsbRegexs)[i];
        if (iface.Compare(regex) == 0) return TRUE;
    }
    return FALSE;
}

Boolean CTethering::IsWifi(
    /* [in] */ const String &iface)
{
    for (Int32 i=0;i< mTetherableWifiRegexs->GetLength();i++) {
        String regex = (*mTetherableWifiRegexs)[i];
        if (iface.Compare(regex) == 0) return TRUE;
    }
    return FALSE;
}

ECode CTethering::IsWifi(
    /* [in] */ const String& iface,
    /* [out] */ Boolean *  ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = IsWifi(iface);
    return NOERROR;
}

ECode CTethering::InterfaceAdded(
    /* [in] */ const String& iface)
{
//??
/*
    IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    INetworkManagementService service = INetworkManagementService.Stub.asInterface(b);
*/
    Boolean found = FALSE;
    Boolean usb = FALSE;
    if (IsWifi(iface)) {
        found = TRUE;
    }
    if (IsUsb(iface)) {
        found = TRUE;
        usb = TRUE;
    }
    if (found == FALSE) {
        Logger::D(TAG, StringBuffer(iface) + String(" is not a tetherable iface, ignoring"));
        return NOERROR;
    }

//    synchronized (mIfaces) {
    mIfacesMutex.Lock();
    {
        TetherInterfaceSM *sm = mIfaces[iface];
        if (sm != NULL) {
            Logger::E(TAG, StringBuffer("active iface (") + iface + String(") reported as added, ignoring"));
            return NOERROR;
        }
        sm = new TetherInterfaceSM(iface, (IApartment *)mThread, usb, this);
        mIfaces[iface] = sm;
        //??
        //sm->Start();
    }
    mIfacesMutex.Unlock();
    Logger::D(TAG, StringBuffer("interfaceAdded :") + iface);
    return NOERROR;
}

ECode CTethering::InterfaceRemoved(
    /* [in] */ const String& iface)
{
//    synchronized (mIfaces) {
    mIfacesMutex.Lock();
    {
        TetherInterfaceSM *sm = mIfaces[iface];
        if (sm == NULL) {
            Logger::E(TAG, StringBuffer("attempting to remove unknown iface (") +
                    iface + String("), ignoring"));
            return NOERROR;
        }
        //??
        //sm->SendMessage(TetherInterfaceSM_CMD_INTERFACE_DOWN);
        mIfaces.Erase(iface);
    }
    mIfacesMutex.Unlock();
    return NOERROR;
}

Int32 CTethering::Tether(
    /* [in] */ const String& iface)
{
    Logger::D(TAG, StringBuffer("Tethering ") + iface);
    TetherInterfaceSM *sm = NULL;
//    synchronized (mIfaces) {
    mIfacesMutex.Lock();
    {
        sm = mIfaces[iface];
    }
    mIfacesMutex.Unlock();
    if (sm == NULL) {
        Logger::E(TAG, StringBuffer("Tried to Tether an unknown iface :") + iface + String(", ignoring"));
        return ConnectivityManager_TETHER_ERROR_UNKNOWN_IFACE;
    }
    if (!sm->IsAvailable() && !sm->IsErrored()) {
        Logger::E(TAG, StringBuffer("Tried to Tether an unavailable iface :") + iface + String(", ignoring"));
        return ConnectivityManager_TETHER_ERROR_UNAVAIL_IFACE;
    }
    //??
    //sm->SendMessage(TetherInterfaceSM_CMD_TETHER_REQUESTED);
    return ConnectivityManager_TETHER_ERROR_NO_ERROR;
}

ECode CTethering::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32 *ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Tether(iface);
    return NOERROR;
}

ECode CTethering::Untether(
    /* [in] */ const String& iface)
{
    Logger::D(TAG, "Untethering " + iface);
    TetherInterfaceSM *sm = NULL;
    mIfacesMutex.Lock();
    {
        sm = mIfaces[iface];
    }
    mIfacesMutex.Unlock();
    if (sm == NULL) {
        Logger::E(TAG, StringBuffer("Tried to Untether an unknown iface :") + iface + String(", ignoring"));
        return ConnectivityManager_TETHER_ERROR_UNKNOWN_IFACE;
    }
    //??
    /*
    if (sm.IsErrored()) {
        Logger::E(TAG, StringBuffer("Tried to Untethered an errored iface :") + iface + String(", ignoring"));
        return ConnectivityManager_TETHER_ERROR_UNAVAIL_IFACE;
    }
    */

    //??
    //sm->SendMessage(TetherInterfaceSM_CMD_TETHER_UNREQUESTED);
    return ConnectivityManager_TETHER_ERROR_NO_ERROR;
}

ECode CTethering::Untether(
    /* [in] */ const String& iface,
    /* [out] */ Int32 *ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Untether(iface);
    return NOERROR;
}

Int32 CTethering::GetLastTetherError(
    /* [in] */ const String& iface)
{
    TetherInterfaceSM *sm = NULL;
    mIfacesMutex.Lock();
    {
        sm = mIfaces[iface];
    }
    mIfacesMutex.Unlock();
    if (sm == NULL) {
        Logger::E(TAG, StringBuffer("Tried to getLastTetherError on an unknown iface :") + iface + String(", ignoring"));
        return ConnectivityManager_TETHER_ERROR_UNKNOWN_IFACE;
    }
    return sm->GetLastError();
}

ECode CTethering::GetLastTetherError(
    /* [in] */ const String& iface,
    /* [out] */ Int32 *ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = GetLastTetherError(iface);
    return NOERROR;
}

//private
void CTethering::SendTetherStateChangedBroadcast()
{
//??
/*
    IBinder b = ServiceManager.getService(Context.CONNECTIVITY_SERVICE);
    IConnectivityManager service = IConnectivityManager.Stub.asInterface(b);
    try {
        if (!service.isTetheringSupported()) return;
    } catch (RemoteException e) {
        return;
    }
*/

    List<String>* availableList = new List<String>();
    List<String>* activeList = new List<String>();
    List<String>* erroredList = new List<String>();

    Boolean wifiTethered = FALSE;
    Boolean usbTethered = FALSE;

    mIfacesMutex.Lock();
    {
    //synchronized (mIfaces) {
        HashMap<String, TetherInterfaceSM *>::Iterator iter;
        for (iter = mIfaces.Begin(); iter != mIfaces.End(); iter++) {
            String iface = iter->mFirst;
            TetherInterfaceSM *sm = iter->mSecond;
            if (sm != NULL) {

                if(sm->IsErrored()) {
                    erroredList->PushBack((String)iface);
                } else if (sm->IsAvailable()) {
                    availableList->PushBack((String)iface);
                } else if (sm->IsTethered()) {
                    if (IsUsb((String)iface)) {
                        usbTethered = TRUE;
                    } else if (IsWifi((String)iface)) {
                        wifiTethered = TRUE;
                    }
                    activeList->PushBack(iface);
                }

            }
        }
    }
    mIfacesMutex.Unlock();
    AutoPtr<IIntent> broadcast;

    ArrayOf<String>* availableArray = ArrayOf<String>::Alloc(availableList->GetSize());
    ArrayOf<String>* activeArray = ArrayOf<String>::Alloc(activeList->GetSize());
    ArrayOf<String>* erroredArray = ArrayOf<String>::Alloc(erroredList->GetSize());

    for (Int32 i = 0; i<availableArray->GetLength(); i++) {
        (*availableArray)[i] = (*availableList)[i];
    }

    for (Int32 i = 0; i<availableArray->GetLength(); i++) {
        (*activeArray)[i] = (*activeList)[i];
    }

    for (Int32 i = 0; i<availableArray->GetLength(); i++) {
        (*erroredArray)[i] = (*erroredList)[i];
    }

    /*//??
    CIntent::New(String(ConnectivityManager_ACTION_TETHER_STATE_CHANGED), (IIntent**) &broadcast);
    broadcast->AddFlags(Intent_FLAG_RECEIVER_REPLACE_PENDING);
    broadcast->PutStringArrayExtra(String(ConnectivityManager_EXTRA_AVAILABLE_TETHER),
            availableArray);
    broadcast->PutStringArrayExtra(String(ConnectivityManager_EXTRA_ACTIVE_TETHER, activeArray));
    broadcast->PutStringArrayExtra(String(ConnectivityManager_EXTRA_ERRORED_TETHER),
            erroredArray);
    mContext->SendStickyBroadcast(broadcast);
    */
    Logger::D(TAG, StringBuffer("sendTetherStateChangedBroadcast ") +
            String::FromInt32(availableList->GetSize()) +
            String(", ") +
            String::FromInt32(activeList->GetSize()) +
            String(", ") +
            String::FromInt32(erroredList->GetSize()) );

    if (usbTethered) {
        if (wifiTethered) {
            ShowTetheredNotification(0x010802d6);
                    //com.android.internal.R.drawable.stat_sys_tether_general);
        } else {
            ShowTetheredNotification(0x010802d7);
                    //com.android.internal.R.drawable.stat_sys_tether_usb);
        }
    } else if (wifiTethered) {
        ShowTetheredNotification(0x010802d8);
                //com.android.internal.R.drawable.stat_sys_tether_wifi);
    } else {
        ClearTetheredNotification();
    }
}

//private
void CTethering::ShowTetheredNotification(
    /* [in] */ Int32 icon)
{
    AutoPtr<INotificationManagerProxy> notificationManager = NotificationManager();

    if ((INotificationManagerProxy *)notificationManager == NULL) {
        return;
    }

    Int32 iconTmp;
    mTetheredNotification->GetIcon(&iconTmp);
    if (mTetheredNotification != NULL) {
        if (iconTmp == icon) {
            return;
        }
        notificationManager->Cancel(iconTmp);
    }

    AutoPtr<IIntent> intent;
    CIntent::New((IIntent **) &intent);
    //??
    //intent->SetClassName("com.android.settings", "com.android.settings.TetherSettings");
    intent->SetFlags(Intent_FLAG_ACTIVITY_NO_HISTORY);

    //??
    //PendingIntent pi = PendingIntent.getActivity(mContext, 0, intent, 0);

    AutoPtr<IResources> r;
    AutoPtr<IResourcesFactory> rf;
    CResourcesFactory::AcquireSingleton((IResourcesFactory **)&rf);
    rf->GetSystem((IResources **)&r);
    AutoPtr<ICharSequence> title;
    r->GetText(
            0x0104037d,//com.android.internal.R.string.tethered_notification_title);
            (ICharSequence **)&title);
    AutoPtr<ICharSequence> message;
    r->GetText(
            0x0104037e,//com.android.intrnal.R.string.tethered_notification_message);
            (ICharSequence **) &message);

    if(mTetheredNotification == NULL) {
        CNotification::New((INotification**)&mTetheredNotification);
        mTetheredNotification->SetWhen(0);
    }
    mTetheredNotification->SetIcon(icon);

    Int32 defaults;
    mTetheredNotification->GetDefaults(&defaults);
    defaults &= ~Notification_DEFAULT_SOUND;
    mTetheredNotification->SetDefaults(defaults);
    mTetheredNotification->SetFlags(Notification_FLAG_ONGOING_EVENT);
    mTetheredNotification->SetTickerText(title);

    //??
    //mTetheredNotification.setLatestEventInfo(mContext, title, message, pi);

    Int32 iconTmp1;
    mTetheredNotification->GetIcon(&iconTmp1);
    //?? no Notify API available
    //notificationManager->Notify(iconTmp1, mTetheredNotification);
}

//private
void CTethering::ClearTetheredNotification()
{
    AutoPtr<INotificationManagerProxy> notificationManager = NotificationManager();

    if ((INotificationManagerProxy *)notificationManager != NULL && (INotification *)mTetheredNotification != NULL) {
        Int32 iconTmp;
        mTetheredNotification->GetIcon(&iconTmp);
        notificationManager->Cancel(iconTmp);
        mTetheredNotification->Release();
    }
}

//private
void CTethering::UpdateUsbStatus()
{
    Boolean enable = mUsbConnected && mUsbMassStorageOff;

    if (mBooted) {
        EnableUsbIfaces(enable);
    }
}

CTethering::StateReceiver::StateReceiver(
    /* [in] */ CTethering * t)
{
    assert(t != NULL);
    mT = t;
}

ECode CTethering::StateReceiver::OnReceive(
    /* [in] */ IContext* content,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    //??
    /*if (action.Equals(String(UsbManager_ACTION_USB_STATE))) {
        AutoPtr<IBundle> extras;
        intent->GetExtras((IBundle **) &extras);
        extras->GetBoolean(String(UsbManager_USB_CONNECTED),&mUsbConnected);
        UpdateUsbStatus();
    } else*/ if (action.Equals(String(Intent_ACTION_MEDIA_SHARED))) {
        mT->mUsbMassStorageOff = FALSE;
        mT->UpdateUsbStatus();
    }
    else if (action.Equals(String(Intent_ACTION_MEDIA_UNSHARED))) {
        mT->mUsbMassStorageOff = TRUE;
        mT->UpdateUsbStatus();
    } else if (action.Equals(String(ConnectivityManager_CONNECTIVITY_ACTION))) {
        //??
        //mTetherMasterSM->SendMessage(TetherMasterSM_CMD_UPSTREAM_CHANGED);
    } else if (action.Equals(Intent_ACTION_BOOT_COMPLETED)) {
        mT->mBooted = TRUE;
        mT->UpdateUsbStatus();
    }
    return NOERROR;
}

// used on cable insert/remove
//private
void CTethering::EnableUsbIfaces(
    /* [in] */ Boolean enable)
{
    //??
    //IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    //INetworkManagementService service = INetworkManagementService.Stub.asInterface(b);

    ArrayOf<String>* ifaces = NULL;
    ECode ec;//
    //??
    //ec = service->ListInterfaces(&ifaces);
    if (FAILED(ec)) {
        Logger::E(TAG, StringBuffer("Error listing Interfaces :") + String::FromInt32(ec));
        return;
    }
    for (Int32 i =0; i<ifaces->GetLength(); i++) {
        String iface = (*ifaces)[i];
        if (IsUsb(iface)) {
            if (enable) {
                InterfaceAdded(iface);
            } else {
                InterfaceRemoved(iface);
            }
        }
    }
    return;
}

// toggled when we enter/leave the fully teathered state
//private
Boolean CTethering::EnableUsbRndis(
    /* [in] */ Boolean enabled)
{
    Logger::D(TAG, StringBuffer("enableUsbRndis(") + String::FromBoolean(enabled) + String(")"));
    //??
    /*
    IBinder b = ServiceManager::GetService(Context_NETWORKMANAGEMENT_SERVICE);
    INetworkManagementService service = INetworkManagementService.Stub.asInterface(b);
    */
    if (enabled) {
        //synchronized (this) {
        mThisMutex.Lock();
        {
            Boolean isUsbRNDISStarted;
            ECode ec;//
            //??
            //ec = service->IsUsbRNDISStarted(&isUsbRNDISStarted);
            if (FAILED(ec)) {
                Logger::E(TAG, StringBuffer("Error toggling usb RNDIS :") + String::FromInt32(ec));
                return FALSE;
            }
            if (!isUsbRNDISStarted) {
                //??
                //ec = service->StartUsbRNDIS();
                if (FAILED(ec)) {
                    Logger::E(TAG, StringBuffer("Error toggling usb RNDIS :") + String::FromInt32(ec));
                    return FALSE;
                }
            }
        }
        mThisMutex.Unlock();
    } else {
        Boolean isUsbRNDISStarted;
        ECode ec;//
        //??
        //ec = service->IsUsbRNDISStarted(&isUsbRNDISStarted);
        if (FAILED(ec)) {
            Logger::E(TAG, StringBuffer("Error toggling usb RNDIS :") + String::FromInt32(ec));
            return FALSE;
        }
        if (!isUsbRNDISStarted) {
            //??
            //ec = service->StopUsbRNDIS();
            if (FAILED(ec)) {
                Logger::E(TAG, StringBuffer("Error toggling usb RNDIS :") + String::FromInt32(ec));
                return FALSE;
            }
        }
    }
    return TRUE;
}

// configured when we start tethering and unconfig'd on error or conclusion
//private
Boolean CTethering::ConfigureUsbIface(
    /* [in] */ Boolean enabled)
{
    Logger::D(TAG, StringBuffer("configureUsbIface(") + String::FromBoolean(enabled) + String(")"));
//??
/*
    IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    INetworkManagementService service = INetworkManagementService.Stub.asInterface(b);
*/
    // bring toggle the interfaces
    ArrayOf<String>* ifaces = NULL;
    ECode ec;//
    //??
    //ec = service->ListInterfaces(&ifaces);
    if (FAILED(ec)) {
        Logger::E(TAG, StringBuffer("Error listing Interfaces :") + String::FromInt32(ec));
        return FALSE;
    }
    for (Int32 i = 0; i<ifaces->GetLength(); i++) {
        String iface = (*ifaces)[i];
        if (IsUsb(iface)) {
            //??
            /*
            IInterfaceConfiguration * ifcg = NULL;
            {
                //??
                //ec = service->GetInterfaceConfig(iface, &ifcg);
                if (FAILED(ec)) {
                    Logger::E(TAG, StringBuffer("Error configuring interface ") + iface + String(", :") + String::FromInt32(e));
                    return FALSE;
                }
                if (ifcg != NULL) {
                    ArrayOf<String> * addr;
                    CConnectivityService::StringUtil::Split(String(USB_NEAR_IFACE_ADDR),String("\\."), &addr);
                    ifcg->ipAddr = ( (*addr)[0].ToInt32()) << 24) +
                            ((*addr)[1].ToInt32()) << 16) +
                            ((*addr)[2].ToInt32()) << 8) +
                            ((*addr)[3].ToInt32()));
                    Arrayof<String> * addr1;
                    CConnectivityService::StringUtil::Split(String(USB_NETMASK),String("\\."), &addr1);
                    ifcg->netmask = ((*addr)[0].ToInt32()) << 24) +
                            ((*addr)[1].ToInt32()) << 16) +
                            ((*addr)[2].ToInt32()) << 8) +
                            ((*addr)[3].ToInt32()));
                    if (enabled) {
                        ifcg->interfaceFlags = ifcg->interfaceFlags.Replace("down", "up");
                    } else {
                        ifcg->interfaceFlags = ifcg->interfaceFlags.Replace("up", "down");
                    }
                    ifcg->interfaceFlags = ifcg->interfaceFlags.Replace("running", "");
                    ifcg->interfaceFlags = ifcg->interfaceFlags.Replace("  "," ");
                    //??
                    //ec = service->SetInterfaceConfig(iface, ifcg);
                    if (FAILED(ec)) {
                        Logger::E(TAG, StringBuffer("Error configuring interface ") + iface + String(", :") + String::FromInt32(e));
                        return FALSE;
                    }
                }
            }
            */
        }
    }
    return TRUE;
}

ECode CTethering::GetTetherableUsbRegexs(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mTetherableUsbRegexs;
    return NOERROR;
}

ECode CTethering::GetTetherableWifiRegexs(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret =  mTetherableWifiRegexs;
    return NOERROR;
}

ECode CTethering::GetUpstreamIfaceRegexs(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret =  mUpstreamIfaceRegexs;
    return NOERROR;
}

ECode CTethering::IsDunRequired(
    /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    Boolean defaultVal;
    res->GetBoolean(
            0x010d0003,//com.android.internal.R.bool.config_tether_dun_required);
            &defaultVal);
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver **)&cr);
    Int32 val;
    SettingsSecure()->GetInt32(
            cr,
            String(SettingsSecure_TETHER_DUN_REQUIRED), (defaultVal ? 1 : 0), &val);
    Boolean result = (val == 1);
    *ret = result;
    return NOERROR;
}

ECode CTethering::GetTetheredIfaces(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    List<String>* list = new List<String>();
    mIfacesMutex.Lock();
    {
        HashMap<String, TetherInterfaceSM *>::Iterator iter;
        for (iter = mIfaces.Begin(); iter != mIfaces.End(); iter++) {
            String str = iter->mFirst;
            TetherInterfaceSM *sm = iter->mSecond;
            if (sm->IsTethered()) {
                list->PushBack((String)str);
            }
        }
    }
    mIfacesMutex.Unlock();
    *ret = ArrayOf<String>::Alloc(list->GetSize());
    for (Int32 i=0; i < (Int32)list->GetSize(); i++) {
        (**ret)[i] = (*list)[i];
    }
    return NOERROR;
}

ECode CTethering::GetTetherableIfaces(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    List<String>* list = new List<String>();
    mIfacesMutex.Lock();
    {
        HashMap<String, TetherInterfaceSM *>::Iterator iter;
        for (iter = mIfaces.Begin(); iter != mIfaces.End(); iter++) {
            String str = iter->mFirst;
            TetherInterfaceSM *sm = iter->mSecond;
            if (sm->IsAvailable()) {
                list->PushBack((String)str);
            }
        }
    }
    mIfacesMutex.Unlock();
    *ret = ArrayOf<String>::Alloc(list->GetSize());
    for (Int32 i=0; i <(Int32) list->GetSize(); i++) {
        (**ret)[i] = (*list)[i];
    }
    return NOERROR;
}

ECode CTethering::GetErroredIfaces(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    List<String>* list = new List<String>();
    mIfacesMutex.Lock();
    {
        HashMap<String, TetherInterfaceSM *>::Iterator iter;
        for (iter = mIfaces.Begin(); iter != mIfaces.End(); iter++) {
            String str = iter->mFirst;
            TetherInterfaceSM *sm = iter->mSecond;

            if (sm->IsErrored()) {
                list->PushBack((String)str);
            }
        }
    }
    mIfacesMutex.Unlock();
    *ret = ArrayOf<String>::Alloc(list->GetSize());
    for (Int32 i=0; i < (Int32)list->GetSize(); i++) {
        (**ret)[i] = (*list)[i];
    }
    return NOERROR;
}

CTethering::TetherInterfaceSM::TetherInterfaceSM(
    /* [in] */ const String& name,
    /* [in] */ IApartment * looper,
    /* [in] */ Boolean usb,
    /* [in] */ CTethering *t)
{
    //??
    //super(name, looper);
    //mTism = (TetherInterfaceSM *)this;
    mTethering = t;
    mIfaceName = name;
    mUsb = usb;
    SetLastError(ConnectivityManager_TETHER_ERROR_NO_ERROR);

    mInitialState = new InitialState(this);
    mStartingState = new StartingState(this);
    mTetheredState = new TetheredState(this);
    mUnavailableState = new UnavailableState(this);

    //??
    /*
    AddState(mInitialState);
    AddState(mStartingState);
    AddState(mTetheredState);
    AddState(mUnavailableState);
    SetInitialState(mInitialState);
    */
}

CTethering::TetherInterfaceSM::~TetherInterfaceSM()
{
    delete mInitialState;
    delete mStartingState;
    delete mTetheredState;
    delete mUnavailableState;
}

//public
String CTethering::TetherInterfaceSM::ToString()
{
    String res;// = new String();
    res += mIfaceName + String(" - ");

    //??
    HierarchicalState * current;
    //current= GetCurrentState();
    if (current == mInitialState) res += String("InitialState");
    if (current == mStartingState) res += String("StartingState");
    if (current == mTetheredState) res += String("TetheredState");
    if (current == mUnavailableState) res += String("UnavailableState");
    if (mAvailable) res += String(" - Available");
    if (mTethered) res += String(" - Tethered");
    res += String(" - lastError =") + String::FromInt32(mLastError);
    return res;
}

//public synchronized
Int32 CTethering::TetherInterfaceSM::GetLastError()
{
    Mutex::Autolock l(mLock);
    return mLastError;
}

//private synchronized
void CTethering::TetherInterfaceSM::SetLastError(
    /* [in] */ Int32 error)
{
    Mutex::Autolock l(mLock);
    mLastError = error;

    if (IsErrored()) {
        if (mUsb) {
            // note everything's been unwound by this point so nothing to do on
            // further error..
            mTethering->ConfigureUsbIface(FALSE);
        }
    }
}

// synchronized between this getter and the following setter
//public synchronized
Boolean CTethering::TetherInterfaceSM::IsAvailable()
{
    Mutex::Autolock l(mLock);
    return mAvailable;
}

//private synchronized
void CTethering::TetherInterfaceSM::SetAvailable(
    /* [in] */ Boolean available)
{
    Mutex::Autolock l(mLock);
    mAvailable = available;
}

// synchronized between this getter and the following setter
//public synchronized
Boolean CTethering::TetherInterfaceSM::IsTethered()
{
    Mutex::Autolock l(mLock);
    return mTethered;
}

//private synchronized
void CTethering::TetherInterfaceSM::SetTethered(
    /* [in] */ Boolean tethered)
{
    Mutex::Autolock l(mLock);
    mTethered = tethered;
}

// synchronized between this getter and the following setter
//public synchronized
Boolean CTethering::TetherInterfaceSM::IsErrored()
{
    Mutex::Autolock l(mLock);
    return (mLastError != ConnectivityManager_TETHER_ERROR_NO_ERROR);
}

CTethering::TetherInterfaceSM::InitialState::InitialState(
    /* [in] */ TetherInterfaceSM * tism)
{
    mTism = tism;
}

//
ECode CTethering::TetherInterfaceSM::InitialState::Enter()
{
    mTism->SetAvailable(TRUE);
    mTism->SetTethered(FALSE);
    mTism->mTethering->SendTetherStateChangedBroadcast();
    return NOERROR;
}

//@Override
ECode CTethering::TetherInterfaceSM::InitialState::ProcessMessage(
    /* [in] */ IMessage *message,
    /* [out] */ Boolean *ret)
{
    return E_NOT_IMPLEMENTED;
//??
/*
    Logger::D(TAG, "InitialState.processMessage what=" + message.what);
    Boolean retValue = TRUE;
    switch (message.what) {
        case CMD_TETHER_REQUESTED:
            setLastError(ConnectivityManager.TETHER_ERROR_NO_ERROR);
            mTetherMasterSM.sendMessage(TetherMasterSM.CMD_TETHER_MODE_REQUESTED,
                    TetherInterfaceSM.this);
            transitionTo(mStartingState);
            break;
        case CMD_INTERFACE_DOWN:
            transitionTo(mUnavailableState);
            break;
        default:
            retValue = FALSE;
            break;
    }
    return retValue;
*/
}

CTethering::TetherInterfaceSM::StartingState::StartingState(
    /* [in] */ TetherInterfaceSM * tism)
{
    mTism = tism;
}

//@Override
ECode CTethering::TetherInterfaceSM::StartingState::Enter()
{
    mTism->SetAvailable(FALSE);
    if (mTism->mUsb) {
        if (!mTism->mTethering->ConfigureUsbIface(TRUE)) {
            //??
            //mTetherMasterSM.sendMessage(TetherMasterSM.CMD_TETHER_MODE_UNREQUESTED,
            //        TetherInterfaceSM.this);
            mTism->SetLastError(ConnectivityManager_TETHER_ERROR_IFACE_CFG_ERROR);

            //??
            //transitionTo(mInitialState);
            return NOERROR;
        }
    }
    mTism->mTethering->SendTetherStateChangedBroadcast();

    // Skipping StartingState
    //??
    //transitionTo(mTetheredState);
    return NOERROR;
}

//@Override public
ECode CTethering::TetherInterfaceSM::StartingState::ProcessMessage(
    /* [in] */ IMessage *message,
    /* [out] */ Boolean *ret)
{
    return E_NOT_IMPLEMENTED;
    //??
    /*
    Logger::D(TAG, "StartingState.processMessage what=" + message.what);
    Boolean retValue = TRUE;
    switch (message.what) {
        // maybe a parent class?
        case CMD_TETHER_UNREQUESTED:
            mTetherMasterSM.sendMessage(TetherMasterSM.CMD_TETHER_MODE_UNREQUESTED,
                    TetherInterfaceSM.this);
            if (mUsb) {
                if (!Tethering.this.configureUsbIface(FALSE)) {
                    setLastErrorAndTransitionToInitialState(
                        ConnectivityManager.TETHER_ERROR_IFACE_CFG_ERROR);
                    break;
                }
            }
            transitionTo(mInitialState);
            break;
        case CMD_CELL_DUN_ERROR:
        case CMD_IP_FORWARDING_ENABLE_ERROR:
        case CMD_IP_FORWARDING_DISABLE_ERROR:
        case CMD_START_TETHERING_ERROR:
        case CMD_STOP_TETHERING_ERROR:
        case CMD_SET_DNS_FORWARDERS_ERROR:
            setLastErrorAndTransitionToInitialState(
                    ConnectivityManager.TETHER_ERROR_MASTER_ERROR);
            break;
        case CMD_INTERFACE_DOWN:
            mTetherMasterSM.sendMessage(TetherMasterSM.CMD_TETHER_MODE_UNREQUESTED,
                    TetherInterfaceSM.this);
            transitionTo(mUnavailableState);
            break;
        default:
            retValue = FALSE;
    }
    return retValue;
    */
}

CTethering::TetherInterfaceSM::TetheredState::TetheredState(
    /* [in] */ TetherInterfaceSM * tism)
{
    mTism = tism;
}

//@Override
ECode CTethering::TetherInterfaceSM::TetheredState::Enter()
{
    //??
    /*
    IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    INetworkManagementService service =
            INetworkManagementService.Stub.asInterface(b);
    try {
        service.tetherInterface(mIfaceName);
    } catch (Exception e) {
        setLastError(ConnectivityManager.TETHER_ERROR_TETHER_IFACE_ERROR);

        transitionTo(mInitialState);
        return;
    }
    */

    if (mTism->mUsb) mTism->mTethering->EnableUsbRndis(TRUE);
    Logger::D(TAG, "Tethered " + mTism->mIfaceName);
    mTism->SetAvailable(FALSE);
    mTism->SetTethered(TRUE);
    mTism->mTethering->SendTetherStateChangedBroadcast();
    return NOERROR;
}

//@Override
ECode CTethering::TetherInterfaceSM::TetheredState::Exit()
{
    if (mTism->mUsb) mTism->mTethering->EnableUsbRndis(FALSE);
    return NOERROR;
}

//@Override
ECode CTethering::TetherInterfaceSM::TetheredState::ProcessMessage(
    /* [in] */ IMessage *message,
    /* [out] */ Boolean *ret)
{
    return E_NOT_IMPLEMENTED;
    //??
    /*
    Logger::D(TAG, "TetheredState.processMessage what=" + message.what);
    Boolean retValue = TRUE;
    Boolean error = FALSE;
    switch (message.what) {
        case CMD_TETHER_UNREQUESTED:
        case CMD_INTERFACE_DOWN:
            IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
            INetworkManagementService service =
                    INetworkManagementService.Stub.asInterface(b);
            if (mMyUpstreamIfaceName != NULL) {
                try {
                    service.disableNat(mIfaceName, mMyUpstreamIfaceName);
                    mMyUpstreamIfaceName = NULL;
                } catch (Exception e) {
                    try {
                        service.untetherInterface(mIfaceName);
                    } catch (Exception ee) {}

                    setLastErrorAndTransitionToInitialState(
                            ConnectivityManager.TETHER_ERROR_DISABLE_NAT_ERROR);
                    break;
                }
            }
            try {
                service.untetherInterface(mIfaceName);
            } catch (Exception e) {
                setLastErrorAndTransitionToInitialState(
                        ConnectivityManager.TETHER_ERROR_UNTETHER_IFACE_ERROR);
                break;
            }
            mTetherMasterSM.sendMessage(TetherMasterSM.CMD_TETHER_MODE_UNREQUESTED,
                    TetherInterfaceSM.this);
            if (message.what == CMD_TETHER_UNREQUESTED) {
                if (mUsb) {
                    if (!Tethering.this.configureUsbIface(FALSE)) {
                        setLastError(
                                ConnectivityManager.TETHER_ERROR_IFACE_CFG_ERROR);
                    }
                }
                transitionTo(mInitialState);
            } else if (message.what == CMD_INTERFACE_DOWN) {
                transitionTo(mUnavailableState);
            }
            Logger::D(TAG, "Untethered " + mIfaceName);
            break;
        case CMD_TETHER_CONNECTION_CHANGED:
            String newUpstreamIfaceName = (String)(message.obj);
            b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
            service = INetworkManagementService.Stub.asInterface(b);

            if (mMyUpstreamIfaceName != NULL) {
                try {
                    service.disableNat(mIfaceName, mMyUpstreamIfaceName);
                    mMyUpstreamIfaceName = NULL;
                } catch (Exception e) {
                    try {
                        service.untetherInterface(mIfaceName);
                    } catch (Exception ee) {}

                    setLastErrorAndTransitionToInitialState(
                            ConnectivityManager.TETHER_ERROR_DISABLE_NAT_ERROR);
                    break;
                }
            }
            if (newUpstreamIfaceName != NULL) {
                try {
                    service.enableNat(mIfaceName, newUpstreamIfaceName);
                } catch (Exception e) {
                    try {
                        service.untetherInterface(mIfaceName);
                    } catch (Exception ee) {}

                    setLastError(ConnectivityManager.TETHER_ERROR_ENABLE_NAT_ERROR);
                    transitionTo(mInitialState);
                    return TRUE;
                }
            }
            mMyUpstreamIfaceName = newUpstreamIfaceName;
            break;
        case CMD_CELL_DUN_ERROR:
        case CMD_IP_FORWARDING_ENABLE_ERROR:
        case CMD_IP_FORWARDING_DISABLE_ERROR:
        case CMD_START_TETHERING_ERROR:
        case CMD_STOP_TETHERING_ERROR:
        case CMD_SET_DNS_FORWARDERS_ERROR:
            error = TRUE;
            // fall through
        case CMD_TETHER_MODE_DEAD:
            b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
            service = INetworkManagementService.Stub.asInterface(b);
            if (mMyUpstreamIfaceName != NULL) {
                try {
                    service.disableNat(mIfaceName, mMyUpstreamIfaceName);
                    mMyUpstreamIfaceName = NULL;
                } catch (Exception e) {
                    try {
                        service.untetherInterface(mIfaceName);
                    } catch (Exception ee) {}

                    setLastErrorAndTransitionToInitialState(
                            ConnectivityManager.TETHER_ERROR_DISABLE_NAT_ERROR);
                    break;
                }
            }
            try {
                service.untetherInterface(mIfaceName);
            } catch (Exception e) {
                setLastErrorAndTransitionToInitialState(
                        ConnectivityManager.TETHER_ERROR_UNTETHER_IFACE_ERROR);
                break;
            }
            if (error) {
                setLastErrorAndTransitionToInitialState(
                        ConnectivityManager.TETHER_ERROR_MASTER_ERROR);
                break;
            }
            Logger::D(TAG, "Tether lost upstream connection " + mIfaceName);
            sendTetherStateChangedBroadcast();
            if (mUsb) {
                if (!Tethering.this.configureUsbIface(FALSE)) {
                    setLastError(ConnectivityManager.TETHER_ERROR_IFACE_CFG_ERROR);
                }
            }
            transitionTo(mInitialState);
            break;
        default:
            retValue = FALSE;
            break;
    }
    return retValue;
*/
}

CTethering::TetherInterfaceSM::UnavailableState::UnavailableState(
    /* [in] */ TetherInterfaceSM * tism)
{
    mTism = tism;
}

//@Override
ECode CTethering::TetherInterfaceSM::UnavailableState::Enter()
{
    mTism->SetAvailable(FALSE);
    mTism->SetLastError(ConnectivityManager_TETHER_ERROR_NO_ERROR);
    mTism->SetTethered(FALSE);
    mTism->mTethering->SendTetherStateChangedBroadcast();
    return NOERROR;
}

//@Override
ECode CTethering::TetherInterfaceSM::UnavailableState::ProcessMessage(
    /* [in] */ IMessage *message,
    /* [out] */ Boolean *ret)
{
    return E_NOT_IMPLEMENTED;
    //??
    /*
    Boolean retValue = TRUE;
    switch (message.what) {
        case CMD_INTERFACE_UP:
            transitionTo(mInitialState);
            break;
        default:
            retValue = FALSE;
            break;
    }
    return retValue;
    */
}

void CTethering::TetherInterfaceSM::SetLastErrorAndTransitionToInitialState(
    /* [in] */ Int32 error)
{
    SetLastError(error);
    //??
    //TransitionTo(mInitialState);
}

CTethering::TetherMasterSM::TetherMasterSM(
    /* [in] */ const String& name,
    /* [in] */ IApartment * looper,
    /* [in] */ CTethering * t)
{
    //super(name, looper);

    //mTmsm = (TetherMasterSM *)this;
    mTethering = t;

    //Add states
    mInitialState = new InitialState(this);
    mTetherModeAliveState = new TetherModeAliveState(this);
    mSetIpForwardingEnabledErrorState = new SetIpForwardingEnabledErrorState(this);
    mSetIpForwardingDisabledErrorState = new SetIpForwardingDisabledErrorState(this);
    mStartTetheringErrorState = new StartTetheringErrorState(this);
    mStopTetheringErrorState = new StopTetheringErrorState(this);
    mSetDnsForwardersErrorState = new SetDnsForwardersErrorState(this);

    mNotifyList = new List<TetherInterfaceSM *>();

    //??
    /*
    AddState(mInitialState);
    AddState(mTetherModeAliveState);
    AddState(mSetIpForwardingEnabledErrorState);
    AddState(mSetIpForwardingDisabledErrorState);
    AddState(mStartTetheringErrorState);
    AddState(mStopTetheringErrorState);
    AddState(mSetDnsForwardersErrorState);
    SetInitialState(mInitialState);
    */
}

CTethering::TetherMasterSM::~TetherMasterSM()
{
    delete mNotifyList;
}

//CTethering::TetherMasterSM::TetherMasterUtilState::TetherMasterUtilState(
//    /* [in] */ TetherMasterSM * tmsm)
//{
//    mTmsm = tmsm;
//}

//@Override
ECode CTethering::TetherMasterSM::TetherMasterUtilState::ProcessMessage(
    /* [in] */ IMessage *message,
    /* [out] */ Boolean *ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = FALSE;
    return NOERROR;
}

Int32 CTethering::TetherMasterSM::TetherMasterUtilState::TurnOnMobileConnection()

{
    return E_NOT_IMPLEMENTED;
    //IBinder b = ServiceManager.getService(Context_CONNECTIVITY_SERVICE);
    //IConnectivityManager service = IConnectivityManager.Stub.asInterface(b);
    /*
    Int32 retValue = Phone_APN_REQUEST_FAILED;
    try {
        retValue = service->StartUsingNetworkFeature(ConnectivityManager_TYPE_MOBILE,
                (mDunRequired ? Phone_FEATURE_ENABLE_DUN : Phone_FEATURE_ENABLE_HIPRI),
                new Binder());
    } catch (Exception e) {
    }
    switch (retValue) {
    case Phone_APN_ALREADY_ACTIVE:
    case Phone_APN_REQUEST_STARTED:
        sendMessageDelayed(CMD_CELL_CONNECTION_RENEW, CELL_CONNECTION_RENEW_MS);
        mConnectionRequested = TRUE;
        break;
    case Phone_APN_REQUEST_FAILED:
    default:
        mConnectionRequested = FALSE;
        break;
    }

    return retValue;
    */
}

Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TurnOffMobileConnection()
{
    if (mTmsm->mConnectionRequested) {
        //??
        //IBinder b = ServiceManager.getService(Context.CONNECTIVITY_SERVICE);
        //IConnectivityManager service =
        //        IConnectivityManager.Stub.asInterface(b);

        ECode ec;
        //??
        /*
        ec = service->StopUsingNetworkFeature(ConnectivityManager_TYPE_MOBILE,
                    (mDunRequired? Phone_FEATURE_ENABLE_DUN :
                                 Phone_FEATURE_ENABLE_HIPRI));
        */
        if (FAILED(ec)) {
            return FALSE;
        }
        mTmsm->mConnectionRequested = FALSE;
    }
    return TRUE;
}

//protected
Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TurnOnMasterTetherSettings()
{
    //??
    //IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    //INetworkManagementService service =
    //        INetworkManagementService.Stub.asInterface(b);
    ECode ec;
    //??
    //ec = service->SetIpForwardingEnabled(TRUE);
    if (FAILED(ec)) {
        //??
        //TansitionTo(mSetIpForwardingEnabledErrorState);
        return FALSE;
    }

    //??
    //ec = service->StartTethering(mDhcpRange);
    if (FAILED(ec)) {
        //??
        //TransitionTo(mStartTetheringErrorState);
        return FALSE;
    }
    //??
    //ec = service->SetDnsForwarders(mDnsServers);
    if (FAILED(ec)) {
        //??
        //TransitionTo(mSetDnsForwardersErrorState);
        return FALSE;
    }
    return TRUE;
}

//protected
Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TurnOffMasterTetherSettings()
{
    //IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    //INetworkManagementService service =
    //        INetworkManagementService.Stub.asInterface(b);
    ECode ec;
    //??
    //ec = service->StopTethering();
    if (FAILED(ec)) {
        //??
        //TransitionTo(mStopTetheringErrorState);
        return FALSE;
    }
    //??
    //ec = service->SetIpForwardingEnabled(FALSE);
    if (FAILED(ec)) {
        //??
        //TransitionTo(mSetIpForwardingDisabledErrorState);
        return FALSE;
    }
    //??
    //TransitionTo(mInitialState);
    return TRUE;
}

//protected
String CTethering::TetherMasterSM::TetherMasterUtilState::FindActiveUpstreamIface()
{
    // check for what iface we can use - if none found switch to error.
    //??
    //IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    //INetworkManagementService service = INetworkManagementService.Stub.asInterface(b);

    ArrayOf<String> * ifaces;
    /*
    try {
        ifaces = service.listInterfaces();
    } catch (Exception e) {
        Logger::E(TAG, "Error listing Interfaces :" + e);
        return NULL;
    }
    */
    for (Int32 i=0; i<(Int32) mTmsm->mTethering->mUpstreamIfaceRegexs->GetLength(); i++) {
        String regex = (*(mTmsm->mTethering->mUpstreamIfaceRegexs))[i];
        for (Int32 j=0; (Int32)j<ifaces->GetLength(); j++) {
            String iface = (*ifaces)[j];
            //??
            //if (iface.matches(regex))
            {
                // verify it is active
                //??
                /*
                InterfaceConfiguration ifcg = NULL;
                ifcg = service->GetInterfaceConfig(iface);
                Boolean active;
                ECode ec = ifcg->IsActive(&active);
                if (FAILED(ec)) {
                    Logger::E(TAG, StringBuffer("Error getting iface config :") + String::FromInt32(ec));
                    // ignore - try next
                    continue;
                }
                if (active) {
                        return iface;
                }
                */
            }
        }
    }
    //return NULL;
    return String("");
}

//protected
void CTethering::TetherMasterSM::TetherMasterUtilState::ChooseUpstreamType(
    /* [in] */ Boolean tryCell)
{
    // decide if the current upstream is good or not and if not
    // do something about it (start up DUN if required or HiPri if not)
    String iface = FindActiveUpstreamIface();
    //??
    //IBinder b = ServiceManager.getService(Context.CONNECTIVITY_SERVICE);
    //IConnectivityManager cm = IConnectivityManager.Stub.asInterface(b);
    mTmsm->mConnectionRequested = FALSE;
    Logger::D(TAG, StringBuffer("chooseUpstreamType(") + String::FromBoolean(tryCell) +
            String("),  dunRequired =")
            + String::FromBoolean(mTmsm->mTethering->mDunRequired) + String(", iface=") + iface);
    if (iface.IsNullOrEmpty()) {
        //try {
            if (mTmsm->mTethering->mDunRequired) {
                // check if Dun is on - we can use that
                AutoPtr<INetworkInfo> info;
                ConnectivityManager()->GetNetworkInfo(
                        ConnectivityManager_TYPE_MOBILE_DUN,
                        (INetworkInfo **)&info);
                if (CTethering::IsConnected(info)) {
                    Logger::D(TAG, StringBuffer("setting dun ifacename =") + iface);
                    // even if we're already connected - it may be somebody else's
                    // refcount, so add our own
                    TurnOnMobileConnection();
                } else {
                    // verify the iface is not the default mobile - can't use that!
                    ConnectivityManager()->GetNetworkInfo(ConnectivityManager_TYPE_MOBILE, (INetworkInfo **)&info);
                    if (CTethering::IsConnected(info)) {
                        iface = NULL; // can't accept this one
                    }
                }
            } else {
                Logger::D(TAG, "checking if hipri brought us this connection");
                AutoPtr<INetworkInfo> info;
                ConnectivityManager()->GetNetworkInfo(
                        ConnectivityManager_TYPE_MOBILE_HIPRI, (INetworkInfo **)&info);
                if (CTethering::IsConnected(info)) {
                    Logger::D(TAG, StringBuffer("yes - hipri in use"));
                    // even if we're already connected - it may be sombody else's
                    // refcount, so add our own
                    TurnOnMobileConnection();
                }
            }
        //} catch (RemoteException e) {
        //    Logger::E(TAG, "RemoteException calling ConnectivityManager " + e);
        //    iface = NULL;
        //}
    }
    // may have been set to NULL in the if above
    if (iface == NULL ) {
        if (tryCell == TRY_TO_SETUP_MOBILE_CONNECTION) {
            TurnOnMobileConnection();
        }
        // wait for things to settle and retry
        //??
        //sendMessageDelayed(CMD_RETRY_UPSTREAM, UPSTREAM_SETTLE_TIME_MS);
    }
    NotifyTetheredOfNewUpstreamIface(iface);
}

//protected
void CTethering::TetherMasterSM::TetherMasterUtilState::NotifyTetheredOfNewUpstreamIface(
    const String& ifaceName)
{
    Logger::D(TAG, "notifying tethered with iface =" + ifaceName);
    mTmsm->mUpstreamIfaceName = ifaceName;
    for (Int32 i=0; i<(Int32)mTmsm->mNotifyList->GetSize(); i++) {
        TetherInterfaceSM *sm = (*(mTmsm->mNotifyList))[i];
        //??
        //sm.sendMessage(TetherInterfaceSM_CMD_TETHER_CONNECTION_CHANGED,
        //        ifaceName);
    }
}

CTethering::TetherMasterSM::InitialState::InitialState(
    /* [in] */ TetherMasterSM * tmsm)
{
    mTmsm = tmsm;
}

//@Override
ECode CTethering::TetherMasterSM::InitialState::Enter()
{
    mTmsm->mConnectionRequested = FALSE;
    return NOERROR;
}

//@Override
ECode CTethering::TetherMasterSM::InitialState::ProcessMessage(
    /* [in] */ IMessage *message,
    /* [out] */ Boolean *ret)
{
    return E_NOT_IMPLEMENTED;
    /*
    Logger::D(TAG, "MasterInitialState.processMessage what=" + message.what);
    Boolean retValue = TRUE;
    switch (message.what) {
        case CMD_TETHER_MODE_REQUESTED:
            mDunRequired = isDunRequired();
            TetherInterfaceSM who = (TetherInterfaceSM)message.obj;
            Logger::D(TAG, "Tether Mode requested by " + who.toString());
            mNotifyList.add(who);
            transitionTo(mTetherModeAliveState);
            break;
        case CMD_TETHER_MODE_UNREQUESTED:
            who = (TetherInterfaceSM)message.obj;
            Logger::D(TAG, "Tether Mode unrequested by " + who.toString());
            Int32 index = mNotifyList.indexOf(who);
            if (index != -1) {
                mNotifyList.remove(who);
            }
            break;
        default:
            retValue = FALSE;
            break;
    }
    return retValue;
    */
}

CTethering::TetherMasterSM::TetherModeAliveState::TetherModeAliveState(
    /* [in] */ TetherMasterSM * tmsm)
{
    mTmsm = tmsm;
}

//@Override
ECode CTethering::TetherMasterSM::TetherModeAliveState::Enter()
{
    mTryCell = WAIT_FOR_NETWORK_TO_SETTLE;  // first pass lets just see what we have.
    ChooseUpstreamType(mTryCell);
    mTryCell = !mTryCell;
    TurnOnMasterTetherSettings(); // may transition us out
    return NOERROR;
}

//@Override
ECode CTethering::TetherMasterSM::TetherModeAliveState::Exit()
{
    TurnOffMobileConnection();
    CTethering::TetherMasterSM::TetherMasterUtilState::NotifyTetheredOfNewUpstreamIface(String(""));
    return NOERROR;
}

//@Override
ECode CTethering::TetherMasterSM::TetherModeAliveState::ProcessMessage(
    /* [in] */ IMessage *message,
    /* [out] */ Boolean *ret)
{
    return E_NOT_IMPLEMENTED;
    /*
    Logger::D(TAG, "TetherModeAliveState.processMessage what=" + message.what);
    Boolean retValue = TRUE;
    switch (message.what) {
        case CMD_TETHER_MODE_REQUESTED:
            TetherInterfaceSM who = (TetherInterfaceSM)message.obj;
            mNotifyList.add(who);
            who.sendMessage(TetherInterfaceSM.CMD_TETHER_CONNECTION_CHANGED,
                    mUpstreamIfaceName);
            break;
        case CMD_TETHER_MODE_UNREQUESTED:
            who = (TetherInterfaceSM)message.obj;
            Int32 index = mNotifyList.indexOf(who);
            if (index != -1) {
                mNotifyList.remove(index);
                if (mNotifyList.isEmpty()) {
                    turnOffMasterTetherSettings(); // transitions appropriately
                }
            }
            break;
        case CMD_UPSTREAM_CHANGED:
            mTryCell = WAIT_FOR_NETWORK_TO_SETTLE;
            chooseUpstreamType(mTryCell);
            mTryCell = !mTryCell;
            break;
        case CMD_CELL_CONNECTION_RENEW:
            // make sure we're still using a requested connection - may have found
            // wifi or something since then.
            if (mConnectionRequested) {
                Logger::D(TAG, "renewing mobile connection - requeuing for another " +
                        CELL_CONNECTION_RENEW_MS + "ms");
                turnOnMobileConnection();
            }
            break;
       case CMD_RETRY_UPSTREAM:
           chooseUpstreamType(mTryCell);
           mTryCell = !mTryCell;
           break;
       default:
           retValue = FALSE;
           break;
    }
    return retValue;
    */
}

//CTethering::TetherMasterSM::ErrorState::ErrorState(
//    /* [in] */ TetherMasterSM * tmsm)
//{
//    mTmsm = tmsm;
//}

//@Override
CTethering::TetherMasterSM::ErrorState::ProcessMessage(
    /* [in] */ IMessage *message,
    /* [out] */ Boolean *ret)
{
    return E_NOT_IMPLEMENTED;
    /*
    Boolean retValue = TRUE;
    switch (message.what) {
        case CMD_TETHER_MODE_REQUESTED:
            TetherInterfaceSM who = (TetherInterfaceSM)message.obj;
            who.sendMessage(mErrorNotification);
            break;
        default:
           retValue = FALSE;
    }
    return retValue;
    */
}

ECode CTethering::TetherMasterSM::ErrorState::Notify(
    /* [in] */ Int32 msgType)
{
    mErrorNotification = msgType;
    for (Int32 i=0; i< (Int32)mTmsm->mNotifyList->GetSize();i++) {
        TetherInterfaceSM *sm = (*(mTmsm->mNotifyList))[i];
        //sm.sendMessage(msgType);
    }
    return NOERROR;
}

CTethering::TetherMasterSM::SetIpForwardingEnabledErrorState::SetIpForwardingEnabledErrorState(
    /* [in] */ TetherMasterSM * tmsm)
{
    mTmsm = tmsm;
}

//@Override
ECode CTethering::TetherMasterSM::SetIpForwardingEnabledErrorState::Enter()
{
    Logger::E(TAG, "Error in setIpForwardingEnabled");;
    Notify(TetherInterfaceSM::CMD_IP_FORWARDING_ENABLE_ERROR);
    return NOERROR;
}

CTethering::TetherMasterSM::SetIpForwardingDisabledErrorState::SetIpForwardingDisabledErrorState(
    /* [in] */ TetherMasterSM * tmsm)
{
    mTmsm = tmsm;
}

//@Override
ECode CTethering::TetherMasterSM::SetIpForwardingDisabledErrorState::Enter()
{
    Logger::E(TAG, "Error in setIpForwardingDisabled");
    Notify(TetherInterfaceSM::CMD_IP_FORWARDING_DISABLE_ERROR);
    return NOERROR;
}

CTethering::TetherMasterSM::StartTetheringErrorState::StartTetheringErrorState(
    /* [in] */ TetherMasterSM * tmsm)
{
    mTmsm = tmsm;
}

//@Override
ECode CTethering::TetherMasterSM::StartTetheringErrorState::Enter()
{
    Logger::E(TAG, "Error in startTethering");
    Notify(TetherInterfaceSM::CMD_START_TETHERING_ERROR);
    //??
    //IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    //INetworkManagementService service =
    //        INetworkManagementService.Stub.asInterface(b);
    //try {
    //    service.setIpForwardingEnabled(FALSE);
    //} catch (Exception e) {}
    return NOERROR;
}

CTethering::TetherMasterSM::StopTetheringErrorState::StopTetheringErrorState(
    /* [in] */ TetherMasterSM * tmsm)
{
    mTmsm = tmsm;
}

//@Override
ECode CTethering::TetherMasterSM::StopTetheringErrorState::Enter()
{
    Logger::E(TAG, "Error in stopTethering");
    Notify(TetherInterfaceSM::CMD_STOP_TETHERING_ERROR);
    //??
    //IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    //INetworkManagementService service =
    //         INetworkManagementService.Stub.asInterface(b);
    //try {
    //    service.setIpForwardingEnabled(FALSE);
    //} catch (Exception e) {}
    return NOERROR;
}

CTethering::TetherMasterSM::SetDnsForwardersErrorState::SetDnsForwardersErrorState(
    /* [in] */ TetherMasterSM * tmsm)
{
    mTmsm = tmsm;
}

//@Override
ECode CTethering::TetherMasterSM::SetDnsForwardersErrorState::Enter()
{
    Logger::E(TAG, "Error in setDnsForwarders");
    Notify(TetherInterfaceSM::CMD_SET_DNS_FORWARDERS_ERROR);
    //??
    /*
    IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
    INetworkManagementService service =
            INetworkManagementService.Stub.asInterface(b);
    try {
        service.stopTethering();
    } catch (Exception e) {}
    try {
        service.setIpForwardingEnabled(FALSE);
    } catch (Exception e) {}
    */
    return NOERROR;
}

/*
public void dump(FileDescriptor fd, PrintWriter pw, String[] args) {
    if (mContext.checkCallingOrSelfPermission(
            android.Manifest.permission.DUMP) != PackageManager.PERMISSION_GRANTED) {
        pw.println("Permission Denial: can't dump ConnectivityService.Tether " +
                "from from pid=" + Binder.getCallingPid() + ", uid=" +
                Binder.getCallingUid());
                return;
    }

    pw.println();
    pw.println("Tether state:");
    synchronized (mIfaces) {
        for (Object o : mIfaces.values()) {
            pw.println(" "+o.toString());
        }
    }
    pw.println();
    return;
}
*/

ECode CTethering::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const ArrayOf<String> & args)
{
    return E_NOT_IMPLEMENTED;
}

void CTethering::NewDhcpRange()
{
    if (mDhcpRange != NULL)
        ArrayOf<String>::Free(mDhcpRange);
    mDhcpRange = ArrayOf<String>::Alloc(4);
    (*mDhcpRange)[0] = String(DHCP_DEFAULT_RANGE1_START);
    (*mDhcpRange)[1] = String(DHCP_DEFAULT_RANGE1_STOP);
    (*mDhcpRange)[2] = String(DHCP_DEFAULT_RANGE2_START);
    (*mDhcpRange)[3] = String(DHCP_DEFAULT_RANGE2_STOP);
}

AutoPtr<INotificationManagerProxy> CTethering::NotificationManager()
{
    AutoPtr<INotificationManagerProxy> notificationManager;
    mContext->GetSystemService(Context_NOTIFICATION_SERVICE,
        (IInterface**)&notificationManager);
    return notificationManager;
}

AutoPtr<IConnectivityManager> CTethering::ConnectivityManager()
{
    AutoPtr<IConnectivityManager> ret;
    //IBinder b = ServiceManager.getService(Context.CONNECTIVITY_SERVICE);
    //IConnectivityManager cm = IConnectivityManager.Stub.asInterface(b);
    return ret;
}

AutoPtr<ISettingsSecure> CTethering::SettingsSecure()
{
    AutoPtr<ISettingsSecure> ss;
    CSettingsSecure::AcquireSingleton((ISettingsSecure **)&ss);
    return ss;
}

Boolean CTethering::IsConnected(
    /* [in] */ INetworkInfo *ni)
{
    assert(ni != NULL);
    Boolean ret;
    ni->IsConnected(&ret);
    return ret;
}
