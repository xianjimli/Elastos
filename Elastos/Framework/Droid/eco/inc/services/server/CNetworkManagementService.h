
#ifndef __CNETWORKMANAGEMENTSERVICE_H__
#define __CNETWORKMANAGEMENTSERVICE_H__

#include "_CNetworkManagementService.h"
//#include "utils/BlockingQueue.h"
#include "server/NativeDaemonConnector.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashSet.h>
#include <elastos/List.h>

CarClass(CNetworkManagementService)
{
public:
    CNetworkManagementService();

    CARAPI Create(
        /* [in] */ IContext * pContext,
        /* [out] */ INetworkManagementService ** ppNMService);

    CARAPI RegisterObserver(
        /* [in] */ INetworkManagementEventObserver * pObs);

    CARAPI UnregisterObserver(
        /* [in] */ INetworkManagementEventObserver * pObs);

    CARAPI IntToIpString(
        /* [in] */ Int32 i,
        /* [out] */ String * pS);

    CARAPI ListInterfaces(
        /* [out, callee] */ ArrayOf<String> ** ppInterfaces);

    CARAPI GetInterfaceConfig(
        /* [in] */ const String& iface);

    CARAPI SetInterfaceConfig(
        /* [in] */ const String& iface);

    CARAPI Shutdown();

    CARAPI GetIpForwardingEnabled(
        /* [out] */ Boolean * pIsEnabled);

    CARAPI SetIpForwardingEnabled(
        /* [in] */ Boolean isEnabled);

    CARAPI StartTethering(
        /* [in] */ const ArrayOf<String> & dhcpRange);

    CARAPI StopTethering();

    CARAPI IsTetheringStarted(
        /* [out] */ Boolean * pIsStarted);

    CARAPI TetherInterface(
        /* [in] */ const String& iface);

    CARAPI UntetherInterface(
        /* [in] */ const String& iface);

    CARAPI ListTetheredInterfaces(
        /* [out, callee] */ ArrayOf<String> ** ppInterFaces);

    CARAPI SetDnsForwarders(
        /* [in] */ const ArrayOf<String> & dns);

    CARAPI GetDnsForwarders(
        /* [out, callee] */ ArrayOf<String> ** ppForwarders);

    CARAPI EnableNat(
        /* [in] */ const String& internalInterface,
        /* [in] */ const String& externalInterface);

    CARAPI DisableNat(
        /* [in] */ const String& internalInterface,
        /* [in] */ const String& externalInterface);

    CARAPI ListTtys(
        /* [out, callee] */ ArrayOf<String> ** ppTtys);

    CARAPI AttachPppd(
        /* [in] */ const String& tty,
        /* [in] */ const String& localAddr,
        /* [in] */ const String& remoteAddr,
        /* [in] */ const String& dns1Addr,
        /* [in] */ const String& dns2Addr);

    CARAPI DetachPppd(
        /* [in] */ const String& tty);

    CARAPI StartUsbRNDIS();

    CARAPI StopUsbRNDIS();

    CARAPI IsUsbRNDISStarted(
        /* [out] */ Boolean * pIsStarted);

    CARAPI StartAccessPoint(
        /* [in] */ const String& wlanIface,
        /* [in] */ const String& softapIface);

    CARAPI ConvertQuotedString(
        /* [in] */ const String& s,
        /* [out] */ String * pQuotedString);

    CARAPI StopAccessPoint();

    CARAPI SetAccessPoint(
        /* [in] */ const String& wlanIface,
        /* [in] */ const String& softapIface);

    CARAPI GetInterfaceCounter(
        /* [in] */ const String& iface,
        /* [in] */ Boolean rx,
        /* [out] */ Int64 * pCounter);

    CARAPI GetInterfaceRxCounter(
        /* [in] */ const String& iface,
        /* [out] */ Int64 * pCounter);

    CARAPI GetInterfaceTxCounter(
        /* [in] */ const String& iface,
        /* [out] */ Int64 * pCounter);

    CARAPI SetInterfaceThrottle(
        /* [in] */ const String& iface,
        /* [in] */ Int32 rxKbps,
        /* [in] */ Int32 txKbps);

    CARAPI GetInterfaceThrottle(
        /* [in] */ const String& iface,
        /* [in] */ Boolean rx,
        /* [out] */ Int32 * pThrottle);

    CARAPI GetInterfaceRxThrottle(
        /* [in] */ const String& iface,
        /* [out] */ Int32 * pRxThrottle);

    CARAPI GetInterfaceTxThrottle(
        /* [in] */ const String& iface,
        /* [out] */ Int32 * pTxThrottle);

    CARAPI constructor();

    class NetdResponseCode
    {
    public:
        static const Int32 InterfaceListResult       = 110;
        static const Int32 TetherInterfaceListResult = 111;
        static const Int32 TetherDnsFwdTgtListResult = 112;
        static const Int32 TtyListResult             = 113;

        static const Int32 TetherStatusResult        = 210;
        static const Int32 IpFwdStatusResult         = 211;
        static const Int32 InterfaceGetCfgResult     = 213;
        static const Int32 SoftapStatusResult        = 214;
        static const Int32 UsbRNDISStatusResult      = 215;
        static const Int32 InterfaceRxCounterResult  = 216;
        static const Int32 InterfaceTxCounterResult  = 217;
        static const Int32 InterfaceRxThrottleResult = 218;
        static const Int32 InterfaceTxThrottleResult = 219;

        static const Int32 InterfaceChange           = 600;
    };

private:
    static const char* TAG;//= "NetworkManagmentService";
    static const Boolean DBG = FALSE;
    static const char* NETD_TAG;// = "NetdConnector";

     /**
     * Binder context for this service
     */
    AutoPtr<IContext> mContext;

    /**
     * connector object for communicating with netd
     */
    NativeDaemonConnector* mConnector;

//    private Thread mThread;
//    private final CountDownLatch mConnectedSignal = new CountDownLatch(1);

//    private ArrayList<INetworkManagementEventObserver> mObservers;
};

#endif // __CNETWORKMANAGEMENTSERVICE_H__
