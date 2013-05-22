
#include "os/SystemProperties.h"
#include "server/CNetworkManagementService.h"
#include <cutils/log.h>
#include <StringBuffer.h>

const char* CNetworkManagementService::TAG = "NetworkManagmentService";
const char* CNetworkManagementService::NETD_TAG = "NetdConnector";

CNetworkManagementService::CNetworkManagementService()
{
    mConnector = new NativeDaemonConnector();
}

ECode CNetworkManagementService::Create(
    /* [in] */ IContext * pContext,
    /* [out] */ INetworkManagementService ** ppNMService)
{
//    NetworkManagementService service = new NetworkManagementService(context);
    if (DBG) LOGD("%s:Creating NetworkManagementService", TAG);
//    service.mThread.start();
    if (DBG) LOGD("%s:Awaiting socket connection", TAG);
//    service.mConnectedSignal.await();
    if (DBG) LOGD("%s:Connected", TAG);
//    return service;
}

ECode CNetworkManagementService::RegisterObserver(
    /* [in] */ INetworkManagementEventObserver * pObs)
{
    LOGD("%s:Registering observer", TAG);
//    mObservers.add(obs);
}

ECode CNetworkManagementService::UnregisterObserver(
    /* [in] */ INetworkManagementEventObserver * pObs)
{
    LOGD("%s:Unregistering observer", TAG);
//    mObservers.remove(mObservers.indexOf(obs));
}

ECode CNetworkManagementService::IntToIpString(
    /* [in] */ Int32 i,
    /* [out] */ String * pS)
{
//    *pS = ((i >> 24 ) & 0xFF) + "." + ((i >> 16 ) & 0xFF) + "." + ((i >>  8 ) & 0xFF) + "." + (i & 0xFF);
}

ECode CNetworkManagementService::ListInterfaces(
    /* [out, callee] */ ArrayOf<String> ** ppInterfaces)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*ACCESS_NETWORK_STATE*/, String(TAG));
//    try {
//     mConnector->DoListCommand(String("interface list"), NetdResponseCode::InterfaceListResult, ppInterfaces);
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Cannot communicate with native daemon to list interfaces");
//    }
}

ECode CNetworkManagementService::GetInterfaceConfig(
    /* [in] */ const String& iface)
{
    String rsp;
    List<String>* pRsp;
//    try {
        pRsp = mConnector->DoCommand(String(StringBuffer("interface getcfg ") + iface));
        rsp = pRsp->GetFront();
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Cannot communicate with native daemon to get interface config");
//    }
    LOGD("%s: rsp <%s>", TAG, (const char*)rsp);

    // Rsp: 213 xx:xx:xx:xx:xx:xx yyy.yyy.yyy.yyy zzz.zzz.zzz.zzz [flag1 flag2 flag3]
//    StringTokenizer st = new StringTokenizer(rsp);

//    InterfaceConfiguration cfg;
//    try {
//        try {
//            int code = Integer.parseInt(st.nextToken(" "));
//            if (code != NetdResponseCode.InterfaceGetCfgResult) {
//                throw new IllegalStateException(
//                    String.format("Expected code %d, but got %d",
//                            NetdResponseCode.InterfaceGetCfgResult, code));
//            }
//        } catch (NumberFormatException nfe) {
//            throw new IllegalStateException(
//                    String.format("Invalid response from daemon (%s)", rsp));
//        }

//        cfg = new InterfaceConfiguration();
//        cfg.hwAddr = st.nextToken(" ");
//        try {
//            cfg.ipAddr = stringToIpAddr(st.nextToken(" "));
//        } catch (UnknownHostException uhe) {
//            Slog.e(TAG, "Failed to parse ipaddr", uhe);
//            cfg.ipAddr = 0;
//        }

//        try {
//            cfg.netmask = stringToIpAddr(st.nextToken(" "));
//        } catch (UnknownHostException uhe) {
//            Slog.e(TAG, "Failed to parse netmask", uhe);
//            cfg.netmask = 0;
//        }
//        cfg.interfaceFlags = st.nextToken("]").trim() +"]";
//    } catch (NoSuchElementException nsee) {
//        throw new IllegalStateException(
//                String.format("Invalid response from daemon (%s)", rsp));
//    }
//    Slog.d(TAG, String.format("flags <%s>", cfg.interfaceFlags));
//    return cfg;
}

ECode CNetworkManagementService::SetInterfaceConfig(
    /* [in] */ const String& iface)
{
//    String cmd = String.format("interface setcfg %s %s %s %s", iface,
//            IntToIpString(cfg.ipAddr), IntToIpString(cfg.netmask), cfg.interfaceFlags);
//    try {
//        mConnector->DoCommand(cmd);
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate with native daemon to interface setcfg");
//    }
}

ECode CNetworkManagementService::Shutdown()
{
//    if (mContext->CheckCallingOrSelfPermission(
//            android.Manifest.permission.SHUTDOWN)
//            != PackageManager.PERMISSION_GRANTED) {
//        throw new SecurityException("Requires SHUTDOWN permission");
//    }

    LOGD("%s:Shutting down", TAG);
}

ECode CNetworkManagementService::GetIpForwardingEnabled(
    /* [out] */ Boolean * pIsEnabled)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*ACCESS_NETWORK_STATE*/, String(TAG));

    List<String>* pRsp;
//    try {
        pRsp = mConnector->DoCommand(String("ipfwd status"));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate with native daemon to ipfwd status");
//    }

//    for (String line : rsp) {
//        String[] tok = line.split(" ");
//        if (tok.length < 3) {
//            Slog.e(TAG, "Malformed response from native daemon: " + line);
//            return false;
//        }

//        int code = Integer.parseInt(tok[0]);
//        if (code == NetdResponseCode.IpFwdStatusResult) {
            // 211 Forwarding <enabled/disabled>
//            return "enabled".equals(tok[2]);
//        } else {
//            throw new IllegalStateException(String.format("Unexpected response code %d", code));
//        }
//    }
//    throw new IllegalStateException("Got an empty response");
}

ECode CNetworkManagementService::SetIpForwardingEnabled(
    /* [in] */ Boolean isEnabled)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
    mConnector->DoCommand(String(StringBuffer("ipfwd %sable")+(isEnabled ? "en" : "dis")));
}

ECode CNetworkManagementService::StartTethering(
    /* [in] */ const ArrayOf<String> & dhcpRange)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));    

    // cmd is "tether start first_start first_stop second_start second_stop ..."
    // an odd number of addrs will fail
    String cmd = String("tether start");
//    for (String d : dhcpRange) {
//        cmd += " " + d;
//    }

//    try {
//        mConnector->DoCommand(cmd);
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException("Unable to communicate to native daemon");
//    }

}

ECode CNetworkManagementService::StopTethering()
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
//    try {
        mConnector->DoCommand(String("tether stop"));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException("Unable to communicate to native daemon to stop tether");
//    }
}

ECode CNetworkManagementService::IsTetheringStarted(
    /* [out] */ Boolean * pIsStarted)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*ACCESS_NETWORK_STATE*/, String(TAG));
    List<String>* pRsp;
//    try {
        pRsp = mConnector->DoCommand(String("tether status"));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate to native daemon to get tether status");
//    }

//    for (String line : rsp) {
//        String[] tok = line.split(" ");
//        if (tok.length < 3) {
//            throw new IllegalStateException("Malformed response for tether status: " + line);
//        }
//        int code = Integer.parseInt(tok[0]);
//        if (code == NetdResponseCode.TetherStatusResult) {
            // XXX: Tethering services <started/stopped> <TBD>...
//            return "started".equals(tok[2]);
//        } else {
//            throw new IllegalStateException(String.format("Unexpected response code %d", code));
//        }
//    }
//    throw new IllegalStateException("Got an empty response");
}

ECode CNetworkManagementService::TetherInterface(
    /* [in] */ const String& iface)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
//    try {
        mConnector->DoCommand(String(StringBuffer("tether interface add ") + iface));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate to native daemon for adding tether interface");
//    }
}

ECode CNetworkManagementService::UntetherInterface(
    /* [in] */ const String& iface)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
//    try {
        mConnector->DoCommand(String(StringBuffer("tether interface remove ") + iface));        
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate to native daemon for removing tether interface");
//    }
}

ECode CNetworkManagementService::ListTetheredInterfaces(
    /* [out, callee] */ ArrayOf<String> ** ppInterFaces)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*ACCESS_NETWORK_STATE*/, String(TAG));
//    try {
//        return mConnector.doListCommand(
//                "tether interface list", NetdResponseCode::TetherInterfaceListResult);
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate to native daemon for listing tether interfaces");
//    }
}

ECode CNetworkManagementService::SetDnsForwarders(
    /* [in] */ const ArrayOf<String> & dns)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));    
//    try {
//        String cmd = "tether dns set";
//        for (String s : dns) {
//            cmd += " " + InetAddress.getByName(s).getHostAddress();
//        }
//        try {
//            mConnector.doCommand(cmd);
//        } catch (NativeDaemonConnectorException e) {
//            throw new IllegalStateException(
//                    "Unable to communicate to native daemon for setting tether dns");
//        }
//    } catch (UnknownHostException e) {
//        throw new IllegalStateException("Error resolving dns name", e);
//    }
}

ECode CNetworkManagementService::GetDnsForwarders(
    /* [out, callee] */ ArrayOf<String> ** ppForwarders)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*ACCESS_NETWORK_STATE*/, String(TAG));       
 //   try {
//        return mConnector.doListCommand(
//                "tether dns list", NetdResponseCode.TetherDnsFwdTgtListResult);
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate to native daemon for listing tether dns");
//    }
}

ECode CNetworkManagementService::EnableNat(
    /* [in] */ const String& internalInterface,
    /* [in] */ const String& externalInterface)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
//    try {
        mConnector->DoCommand(String(
                StringBuffer("nat enable %s %s")+internalInterface+externalInterface));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate to native daemon for enabling NAT interface");
//    }
}

ECode CNetworkManagementService::DisableNat(
    /* [in] */ const String& internalInterface,
    /* [in] */ const String& externalInterface)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
//    try {
        mConnector->DoCommand(String(
                StringBuffer("nat disable %s %s")+internalInterface+externalInterface));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate to native daemon for disabling NAT interface");
//    }
}

ECode CNetworkManagementService::ListTtys(
    /* [out, callee] */ ArrayOf<String> ** ppTtys)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*ACCESS_NETWORK_STATE*/, String(TAG));
//    try {
//        return mConnector.doListCommand("list_ttys", NetdResponseCode.TtyListResult);
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Unable to communicate to native daemon for listing TTYs");
//    }
}

ECode CNetworkManagementService::AttachPppd(
    /* [in] */ const String& tty,
    /* [in] */ const String& localAddr,
    /* [in] */ const String& remoteAddr,
    /* [in] */ const String& dns1Addr,
    /* [in] */ const String& dns2Addr)
{
//    try {
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
//        mConnector.doCommand(String.format("pppd attach %s %s %s %s %s", tty,
//                InetAddress.getByName(localAddr).getHostAddress(),
//                InetAddress.getByName(remoteAddr).getHostAddress(),
//                InetAddress.getByName(dns1Addr).getHostAddress(),
//                InetAddress.getByName(dns2Addr).getHostAddress()));
//    } catch (UnknownHostException e) {
//        throw new IllegalStateException("Error resolving addr", e);
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException("Error communicating to native daemon to attach pppd", e);
//    }
}

ECode CNetworkManagementService::DetachPppd(
    /* [in] */ const String& tty)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
 //   try {
        mConnector->DoCommand(String(StringBuffer("pppd detach %s")+tty));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException("Error communicating to native daemon to detach pppd", e);
//    }
}

ECode CNetworkManagementService::StartUsbRNDIS()
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
//    try {
        mConnector->DoCommand(String("usb startrndis"));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Error communicating to native daemon for starting RNDIS", e);
//    }
}

ECode CNetworkManagementService::StopUsbRNDIS()
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
//    try {
        mConnector->DoCommand(String("usb stoprndis"));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException("Error communicating to native daemon", e);
//    }
}

ECode CNetworkManagementService::IsUsbRNDISStarted(
    /* [out] */ Boolean * pIsStarted)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*ACCESS_NETWORK_STATE*/, String(TAG));
    List<String>* pRsp;
//    try {
        pRsp = mConnector->DoCommand(String("usb rndisstatus"));
//    } catch (NativeDaemonConnectorException e) {
//        throw new IllegalStateException(
//                "Error communicating to native daemon to check RNDIS status", e);
//    }

//    for (String line : rsp) {
//        String []tok = line.split(" ");
//        int code = Integer.parseInt(tok[0]);
//        if (code == NetdResponseCode.UsbRNDISStatusResult) {
//            if (tok[3].equals("started"))
//                return true;
//            return false;
//        } else {
//            throw new IllegalStateException(String.format("Unexpected response code %d", code));
//        }
//    }
//    throw new IllegalStateException("Got an empty response");
}

ECode CNetworkManagementService::StartAccessPoint(
    /* [in] */ const String& wlanIface,
    /* [in] */ const String& softapIface)
{
    ArrayOf<String>** response;
//    mConnector->DoCommand(String(StringBuffer("softap stop ")+wlanIface), response);
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNetworkManagementService::ConvertQuotedString(
    /* [in] */ const String& s,
    /* [out] */ String * pQuotedString)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNetworkManagementService::StopAccessPoint()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNetworkManagementService::SetAccessPoint(
    /* [in] */ const String& wlanIface,
    /* [in] */ const String& softapIface)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNetworkManagementService::GetInterfaceCounter(
    /* [in] */ const String& iface,
    /* [in] */ Boolean rx,
    /* [out] */ Int64 * pCounter)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNetworkManagementService::GetInterfaceRxCounter(
    /* [in] */ const String& iface,
    /* [out] */ Int64 * pCounter)
{
    GetInterfaceCounter(iface, TRUE, pCounter);
}

ECode CNetworkManagementService::GetInterfaceTxCounter(
    /* [in] */ const String& iface,
    /* [out] */ Int64 * pCounter)
{
    GetInterfaceCounter(iface, FALSE, pCounter);
}

ECode CNetworkManagementService::SetInterfaceThrottle(
    /* [in] */ const String& iface,
    /* [in] */ Int32 rxKbps,
    /* [in] */ Int32 txKbps)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*CHANGE_NETWORK_STATE*/, String(TAG));
 //   try {
        mConnector->DoCommand(String(
                StringBuffer("interface setthrottle ")+iface+" "+rxKbps+" "+txKbps));
//    } catch (NativeDaemonConnectorException e) {
//        Slog.e(TAG, "Error communicating with native daemon to set throttle", e);
//    }
}

ECode CNetworkManagementService::GetInterfaceThrottle(
    /* [in] */ const String& iface,
    /* [in] */ Boolean rx,
    /* [out] */ Int32 * pThrottle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNetworkManagementService::GetInterfaceRxThrottle(
    /* [in] */ const String& iface,
    /* [out] */ Int32 * pRxThrottle)
{
    GetInterfaceThrottle(iface, TRUE, pRxThrottle);
}

ECode CNetworkManagementService::GetInterfaceTxThrottle(
    /* [in] */ const String& iface,
    /* [out] */ Int32 * pTxThrottle)
{
    GetInterfaceThrottle(iface, FALSE, pTxThrottle);
}

ECode CNetworkManagementService::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

