
#ifndef __CTETHERING_H__
#define __CTETHERING_H__

#include "_CTethering.h"
#include <ext/frameworkext.h>
#include "utils/HierarchicalState.h"
#include "utils/HierarchicalStateMachine.h"
#include "content/BroadcastReceiver.h"
#include "os/NativeMessageQueue.h"
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Set.h>
#include <elastos/HashMap.h>


using namespace Elastos;
using namespace Elastos::Core;

/*
package com.android.server.connectivity;

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
/**
 * @hide
 *
 * Timeout
 *
 * TODO - look for parent classes and code sharing
 */

//public class Tethering extends INetworkManagementEventObserver.Stub {
CarClass(CTethering)
{
public:
    class StateReceiver : public BroadcastReceiver
    {
    public:
        virtual CARAPI OnReceive(
            /* [in] */ IContext* content,
            /* [in] */ IIntent* intent);

        StateReceiver(
            /* [in] */ CTethering * t);

    protected:
        CTethering * mT;
    };

    class TetherInterfaceSM : public HierarchicalStateMachine
    {
    public:
        class InitialState : public HierarchicalState
        {
        public:
            //@Override
            CARAPI Enter();

            //@Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage *message,
                /* [out] */ Boolean *ret );

            InitialState(
                /* [in] */ TetherInterfaceSM * tism);

            TetherInterfaceSM* mTism;
        };

        class StartingState : public HierarchicalState
        {
        public:
            //@Override
            CARAPI Enter();

            //@Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage *message,
                /* [out] */ Boolean *ret );

            StartingState(
                /* [in] */ TetherInterfaceSM * tism);

            TetherInterfaceSM* mTism;
        };

        class TetheredState : public HierarchicalState
        {
        public:
            //@Override
            CARAPI Enter();

            //@Override
            CARAPI Exit();

            //@Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage *message,
                /* [out] */ Boolean *ret );

            TetheredState(
                /* [in] */ TetherInterfaceSM * tism);

            TetherInterfaceSM* mTism;

        };

        class UnavailableState : public HierarchicalState
        {
        public:

            //@Override
            CARAPI Enter();

            //@Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage *message,
                /* [out] */ Boolean *ret );

            UnavailableState(
                /* [in] */ TetherInterfaceSM * tism);

            TetherInterfaceSM* mTism;
        };

    public:
        // notification from the master SM that it's not in tether mode
        static const Int32 CMD_TETHER_MODE_DEAD            =  1;
        // request from the user that it wants to tether
        static const Int32 CMD_TETHER_REQUESTED            =  2;
        // request from the user that it wants to untether
        static const Int32 CMD_TETHER_UNREQUESTED          =  3;
        // notification that this interface is down
        static const Int32 CMD_INTERFACE_DOWN              =  4;
        // notification that this interface is up
        static const Int32 CMD_INTERFACE_UP                =  5;
        // notification from the master SM that it had an error turning on cellular dun
        static const Int32 CMD_CELL_DUN_ERROR              =  6;
        // notification from the master SM that it had trouble enabling IP Forwarding
        static const Int32 CMD_IP_FORWARDING_ENABLE_ERROR  =  7;
        // notification from the master SM that it had trouble disabling IP Forwarding
        static const Int32 CMD_IP_FORWARDING_DISABLE_ERROR =  8;
        // notification from the master SM that it had trouble staring tethering
        static const Int32 CMD_START_TETHERING_ERROR       =  9;
        // notification from the master SM that it had trouble stopping tethering
        static const Int32 CMD_STOP_TETHERING_ERROR        = 10;
        // notification from the master SM that it had trouble setting the DNS forwarders
        static const Int32 CMD_SET_DNS_FORWARDERS_ERROR    = 11;
        // the upstream connection has changed
        static const Int32 CMD_TETHER_CONNECTION_CHANGED   = 12;

        Int32 mLastError;

        String mIfaceName;

        String mMyUpstreamIfaceName;  // may change over time

        Boolean mUsb;

        CTethering *mTethering;

        ///*static*/ TetherInterfaceSM *mTism;

    public:
        TetherInterfaceSM(
            /* [in] */ const String& name,
//??
            /* [in] */ IApartment *looper,
            /* [in] */ Boolean usb,
            /* [in] */ CTethering *t);

        ~TetherInterfaceSM();

        CARAPI_(String) ToString();

        //public synchronized
        CARAPI_(Int32) GetLastError();

        // synchronized between this getter and the following setter
        //public synchronized
        CARAPI_(Boolean) IsAvailable();

        // synchronized between this getter and the following setter
        //public synchronized
        CARAPI_(Boolean) IsTethered();

        // synchronized between this getter and the following setter
        //public synchronized
        CARAPI_(Boolean) IsErrored();

        CARAPI_(void) SetLastErrorAndTransitionToInitialState(
            /* [in] */ Int32 error);

    private:
        HierarchicalState * mDefaultState;

        HierarchicalState * mInitialState;

        HierarchicalState * mStartingState;

        HierarchicalState * mTetheredState;

        HierarchicalState * mUnavailableState;

        Boolean mAvailable;

        Boolean mTethered;

    private:
        //private synchronized
        CARAPI_(void) SetLastError(
            /* [in] */ Int32 error);

        //private synchronized
        CARAPI_(void) SetAvailable(
            /* [in] */ Boolean available);

        //private synchronized
        CARAPI_(void) SetTethered(
            /* [in] */ Boolean tethered);

    private:
        Mutex mLock;

        AutoPtr<IApartment> mLooper;

    };

    class TetherMasterSM// : public HierarchicalStateMachine
    {
    public:
        class TetherMasterUtilState : public HierarchicalState
        {
        public:
            //@Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage *message,
                /* [out] */ Boolean *ret );

            //TetherMasterUtilState(
            //    /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;

        protected:
            static const Boolean TRY_TO_SETUP_MOBILE_CONNECTION;// = TRUE;
            static const Boolean WAIT_FOR_NETWORK_TO_SETTLE;//     = FALSE;

        protected:
            CARAPI_(Int32) TurnOnMobileConnection();

            CARAPI_(Boolean) TurnOffMobileConnection();

            CARAPI_(Boolean) TurnOnMasterTetherSettings();

            CARAPI_(Boolean) TurnOffMasterTetherSettings();

            CARAPI_(String) FindActiveUpstreamIface();

            CARAPI_(void) ChooseUpstreamType(
                /* [in] */ Boolean tryCell);

            CARAPI_(void) NotifyTetheredOfNewUpstreamIface(
                /* [in] */ const String& ifaceName);
        };

        class InitialState : public TetherMasterUtilState
        {
        public:
            //@Override
            CARAPI Enter();

            //@Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage *message,
                /* [out] */ Boolean *ret );

            InitialState(
                /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;

        };

        class TetherModeAliveState : public TetherMasterUtilState
        {
        public:
            Boolean mTryCell;// = WAIT_FOR_NETWORK_TO_SETTLE;
            //@Override
            CARAPI Enter();

            //@Override
            CARAPI Exit();

            //@Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage *message,
                /* [out] */ Boolean *ret );

            TetherModeAliveState(
                /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;

        };

        class ErrorState : public HierarchicalState
        {
        public:
            Int32 mErrorNotification;

            //@Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage *message,
                /* [out] */ Boolean *ret );

            CARAPI Notify(
                /* [in] */ Int32 msgType);

            //ErrorState(
            //    /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;
        };

        class SetIpForwardingEnabledErrorState : public ErrorState
        {
        public:
            //@Override
            CARAPI Enter();

            SetIpForwardingEnabledErrorState(
                /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;
        };

        class SetIpForwardingDisabledErrorState : public ErrorState
        {
        public:
            //@Override
            CARAPI Enter();

            SetIpForwardingDisabledErrorState(
                /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;

        };

        class StartTetheringErrorState : public ErrorState
        {
        public:
            //@Override
            CARAPI Enter();

            StartTetheringErrorState(
                /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;
        };

        class StopTetheringErrorState : public ErrorState
        {
        public:
            //@Override
            CARAPI Enter();

            StopTetheringErrorState(
                /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;
        };

        class SetDnsForwardersErrorState : public ErrorState
        {
        public:
            //@Override
            CARAPI Enter();

            SetDnsForwardersErrorState(
                /* [in] */ TetherMasterSM * tmsm);

            TetherMasterSM* mTmsm;
        };

    public:
        // an interface SM has requested Tethering
        static const Int32 CMD_TETHER_MODE_REQUESTED   = 1;
        // an interface SM has unrequested Tethering
        static const Int32 CMD_TETHER_MODE_UNREQUESTED = 2;
        // upstream connection change - do the right thing
        static const Int32 CMD_UPSTREAM_CHANGED        = 3;
        // we received notice that the cellular DUN connection is up
        static const Int32 CMD_CELL_CONNECTION_RENEW   = 4;
        // we don't have a valid upstream conn, check again after a delay
        static const Int32 CMD_RETRY_UPSTREAM          = 5;

        ///*static*/ TetherMasterSM * mTmsm;

        CTethering *mTethering;

    public:
        TetherMasterSM(
            /* [in] */ const String& name,
//??
            /* [in] */ IApartment * looper,
            /* [in] */ CTethering * t);

        virtual ~TetherMasterSM();

        //CARAPI_(void) Init();

    private:
        // This indicates what a timeout event relates to.  A state that
        // sends itself a delayed timeout event and handles incoming timeout events
        // should inc this when it is entered and whenever it sends a new timeout event.
        // We do not flush the old ones.
        Int32 mSequenceNumber;

        HierarchicalState * mInitialState;
        HierarchicalState * mTetherModeAliveState;

        HierarchicalState * mSetIpForwardingEnabledErrorState;
        HierarchicalState * mSetIpForwardingDisabledErrorState;
        HierarchicalState * mStartTetheringErrorState;
        HierarchicalState * mStopTetheringErrorState;
        HierarchicalState * mSetDnsForwardersErrorState;

        List<TetherInterfaceSM *>* mNotifyList;

        Boolean mConnectionRequested;// = FALSE;

        String mUpstreamIfaceName;// = null;

        static const Int32 UPSTREAM_SETTLE_TIME_MS;//     = 10000;
        static const Int32 CELL_CONNECTION_RENEW_MS;//    = 40000;
    };

    virtual ~CTethering();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IApartment* looper);

    CARAPI InterfaceLinkStatusChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean link);

    CARAPI IsWifi(
        /* [in] */ const String& iface,
        /* [out] */ Boolean* ret);

    CARAPI InterfaceAdded(
        /* [in] */ const String& iface);

    CARAPI InterfaceRemoved(
        /* [in] */ const String& iface);

    CARAPI Tether(
        /* [in] */ const String& iface,
        /* [out] */ Int32 * ret);

    CARAPI Untether(
        /* [in] */ const String& iface,
        /* [out] */ Int32 *ret);

    CARAPI GetLastTetherError(
        /* [in] */ const String& iface,
        /* [out] */ Int32 *ret);

    CARAPI GetTetherableUsbRegexs(
        /* [out, callee] */ ArrayOf<String> **ret);

    CARAPI GetTetherableWifiRegexs(
        /* [out, callee] */ ArrayOf<String> **ret);

    CARAPI GetUpstreamIfaceRegexs(
        /* [out, callee] */ ArrayOf<String> **ret);

    CARAPI IsDunRequired(
        /* [out] */ Boolean *ret);

    CARAPI GetTetheredIfaces(
        /* [out, callee] */ ArrayOf<String> **ret);

    CARAPI GetTetherableIfaces(
        /* [out, callee] */ ArrayOf<String> **ret);

    CARAPI GetErroredIfaces(
        /* [out, callee] */ ArrayOf<String> **ret);

    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const ArrayOf<String> & args);

    static CARAPI_(AutoPtr<ISettingsSecure>) SettingsSecure();

    static CARAPI_(Boolean) IsConnected(
        /* [in] */ INetworkInfo *ni);

protected:
    // toggled when we enter/leave the fully teathered state
    CARAPI_(Boolean) EnableUsbRndis(
        /* [in] */ Boolean enabled);

    // configured when we start tethering and unconfig'd on error or conclusion
    CARAPI_(Boolean) ConfigureUsbIface(
        /* [in] */ Boolean enabled);

    CARAPI_(void) SendTetherStateChangedBroadcast();

    static AutoPtr<IConnectivityManager> ConnectivityManager();

private:
    AutoPtr<IContext> mContext;
    static const CString TAG;// = "Tethering";

    Boolean mBooted;// = FALSE;
    //used to remember if we got connected before boot finished
    Boolean mDeferedUsbConnection;// = FALSE;

    // TODO - remove both of these - should be part of interface inspection/selection stuff
    ArrayOf<String>* mTetherableUsbRegexs;
    ArrayOf<String>* mTetherableWifiRegexs;
    ArrayOf<String>* mUpstreamIfaceRegexs;

    //??
    AutoPtr<IApartment> mLooper;
    //AutoPtr<NativeMessageQueue> mLooper;

    //??
    //AutoPtr<HandlerThread> mThread;
    AutoPtr<IApartment> mThread;

    HashMap<String, TetherInterfaceSM*> mIfaces; // all tethered/tetherable ifaces

    //??
    //BroadcastReceiver mStateReceiver;
    StateReceiver * mStateReceiver;

    static const String USB_NEAR_IFACE_ADDR; //= "192.168.42.129";
    static const String USB_NETMASK; //= "255.255.255.0";

    // FYI - the default wifi is 192.168.43.1 and 255.255.255.0

    ArrayOf<String>* mDhcpRange;
    static const String DHCP_DEFAULT_RANGE1_START;// = "192.168.42.2";
    static const String DHCP_DEFAULT_RANGE1_STOP;//  = "192.168.42.254";
    static const String DHCP_DEFAULT_RANGE2_START;// = "192.168.43.2";
    static const String DHCP_DEFAULT_RANGE2_STOP;//  = "192.168.43.254";

    ArrayOf<String>* mDnsServers;
    static const String DNS_DEFAULT_SERVER1;// = "8.8.8.8";
    static const String DNS_DEFAULT_SERVER2;// = "4.2.2.2";

    // resampled each time we turn on tethering - used as cache for settings/config-val
    Boolean mDunRequired;  // configuration info - must use DUN apn on 3g

    //??
    //HierarchicalStateMachine mTetherMasterSM;
    TetherMasterSM *mTetherMasterSM;
    AutoPtr<INotification> mTetheredNotification;

    // whether we can tether is the && of these two - they come in as separate
    // broadcasts so track them so we can decide what to do when either changes
    Boolean mUsbMassStorageOff;  // track the status of USB Mass Storage
    Boolean mUsbConnected;       // track the status of USB connection

private:
    CARAPI_(Boolean) IsUsb(
        /* [in] */ const String& iface);

    CARAPI_(void) ShowTetheredNotification(
        /* [in] */ Int32 icon);

    CARAPI_(void) ClearTetheredNotification();

    CARAPI_(void) UpdateUsbStatus();

    // used on cable insert/remove
    CARAPI_(void) EnableUsbIfaces(
        /* [in] */ Boolean enable);

    CString s;

private:
    Mutex mIfacesMutex;

    Mutex mThisMutex;

    CARAPI_(Boolean) IsWifi(
        /* [in] */ const String& iface);

    CARAPI_(Int32) Tether(
        /* [in] */ const String& iface);

    CARAPI_(Int32) Untether(
        /* [in] */ const String& iface);

    CARAPI_(Int32) GetLastTetherError(
        /* [in] */ const String& iface);

    CARAPI_(void) NewDhcpRange();

    CARAPI_(AutoPtr<INotificationManagerProxy>) NotificationManager();

};

#endif //__CTETHERING_H__
