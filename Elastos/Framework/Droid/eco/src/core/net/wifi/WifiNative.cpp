
#include "net/wifi/WifiNative.h"
#include <wifi.h>
#include <stdlib.h>
#include <stdio.h>


static Boolean sScanModeActive = FALSE;

static Int32 doCommand(const char *cmd, char *replybuf, Int32 replybuflen)
{
    size_t reply_len = replybuflen - 1;

    if (::wifi_command(cmd, replybuf, &reply_len) != 0) {
        return -1;
    }
    else {
        // Strip off trailing newline
        if (reply_len > 0 && replybuf[reply_len-1] == '\n') {
            replybuf[reply_len-1] = '\0';
        }
        else {
            replybuf[reply_len] = '\0';
        }
        return 0;
    }
}

static Int32 doIntCommand(const char *cmd)
{
    char reply[256];

    if (doCommand(cmd, reply, sizeof(reply)) != 0) {
        return -1;
    }
    else {
        return atoi(reply);
    }
}

static Boolean doBooleanCommand(const char *cmd, const char *expect)
{
    char reply[256];

    if (doCommand(cmd, reply, sizeof(reply)) != 0) {
        return FALSE;
    }
    else {
        return strcmp(reply, expect) == 0;
    }
}

// Send a command to the supplicant, and return the reply as a String
static String doStringCommand(const char *cmd)
{
    char reply[4096];

    if (doCommand(cmd, reply, sizeof(reply)) != 0) {
        return String(NULL);
    }
    else {
        return String((char *)reply);
    }
}

static Boolean doSetScanMode(Boolean setActive)
{
    return doBooleanCommand((setActive ? "DRIVER SCAN-ACTIVE" : "DRIVER SCAN-PASSIVE"), "OK");
}


const Int32 WifiNative::BLUETOOTH_COEXISTENCE_MODE_ENABLED;
const Int32 WifiNative::BLUETOOTH_COEXISTENCE_MODE_DISABLED;
const Int32 WifiNative::BLUETOOTH_COEXISTENCE_MODE_SENSE;

static CARAPI_(String) GetErrorString(
    /* [in] */ Int32 errorCode)
{
    return String(NULL);
}

Boolean WifiNative::LoadDriver()
{
    return ::wifi_load_driver() == 0;
}

Boolean WifiNative::UnloadDriver()
{
    return ::wifi_unload_driver() == 0;
}

Boolean WifiNative::StartSupplicant()
{
    return ::wifi_start_supplicant() == 0;
}

Boolean WifiNative::StopSupplicant()
{
    return ::wifi_stop_supplicant() == 0;
}

Boolean WifiNative::ConnectToSupplicant()
{
    return ::wifi_connect_to_supplicant() == 0;
}

void WifiNative::CloseSupplicantConnection()
{
    ::wifi_close_supplicant_connection();
}

Boolean WifiNative::PingCommand()
{
    return doBooleanCommand("PING", "PONG");
}

Boolean WifiNative::ScanCommand(
    /* [in] */ Boolean forceActive)
{
    Boolean result;

    // Ignore any error from setting the scan mode.
    // The scan will still work.
    if (forceActive && !sScanModeActive) {
        doSetScanMode(TRUE);
    }
    result = doBooleanCommand("SCAN", "OK");
    if (forceActive && !sScanModeActive) {
        doSetScanMode(sScanModeActive);
    }
    return result;
}

Boolean WifiNative::SetScanModeCommand(
    /* [in] */ Boolean setActive)
{
    sScanModeActive = setActive;
    return doSetScanMode(setActive);
}

String WifiNative::ListNetworksCommand()
{
    return doStringCommand("LIST_NETWORKS");
}

Int32 WifiNative::AddNetworkCommand()
{
    return doIntCommand("ADD_NETWORK");
}

Boolean WifiNative::SetNetworkVariableCommand(
    /* [in] */ Int32 netId,
    /* [in] */ CString name,
    /* [in] */ CString value)
{
    char cmdstr[256];

    const char *nameStr = (const char*)name;
    const char *valueStr = (const char*)value;

    if (nameStr == NULL || valueStr == NULL) {
        return FALSE;
    }

    Boolean cmdTooLong = snprintf(cmdstr, sizeof(cmdstr), "SET_NETWORK %d %s %s",
                 netId, nameStr, valueStr) >= (Int32)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

String WifiNative::GetNetworkVariableCommand(
    /* [in] */ Int32 netId,
    /* [in] */ CString name)
{
    char cmdstr[256];

    const char *nameStr = (const char*)name;

    if (nameStr == NULL) {
        return String(NULL);
    }

    Boolean cmdTooLong = snprintf(cmdstr, sizeof(cmdstr), "GET_NETWORK %d %s",
                             netId, nameStr) >= (Int32)sizeof(cmdstr);

    return cmdTooLong ? String(NULL) : doStringCommand(cmdstr);
}

Boolean WifiNative::RemoveNetworkCommand(
    /* [in] */ Int32 netId)
{
    char cmdstr[256];

    Int32 numWritten = snprintf(cmdstr, sizeof(cmdstr), "REMOVE_NETWORK %d", netId);
    Boolean cmdTooLong = numWritten >= (Int32)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Boolean WifiNative::EnableNetworkCommand(
    /* [in] */ Int32 netId,
    /* [in] */ Boolean disableOthers)
{
    char cmdstr[256];
    const char *cmd = disableOthers ? "SELECT_NETWORK" : "ENABLE_NETWORK";

    Int32 numWritten = snprintf(cmdstr, sizeof(cmdstr), "%s %d", cmd, netId);
    Boolean cmdTooLong = numWritten >= (Int32)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Boolean WifiNative::DisableNetworkCommand(
    /* [in] */ Int32 netId)
{
    char cmdstr[256];

    Int32 numWritten = snprintf(cmdstr, sizeof(cmdstr), "DISABLE_NETWORK %d", netId);
    Boolean cmdTooLong = numWritten >= (Int32)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Boolean WifiNative::ReconnectCommand()
{
    return doBooleanCommand("RECONNECT", "OK");
}

Boolean WifiNative::ReassociateCommand()
{
    return doBooleanCommand("REASSOCIATE", "OK");
}

Boolean WifiNative::DisconnectCommand()
{
    return doBooleanCommand("DISCONNECT", "OK");
}

String WifiNative::StatusCommand()
{
    return doStringCommand("STATUS");
}

Int32 WifiNative::GetRssiHelper(
    /* [in] */ CString cmd)
{
    char reply[256];
    int rssi = -200;

    if (doCommand((const char*)cmd, reply, sizeof(reply)) != 0) {
        return -1;
    }

    // reply comes back in the form "<SSID> rssi XX" where XX is the
    // number we're interested in.  if we're associating, it returns "OK".
    // beware - <SSID> can contain spaces.
    if (strcmp(reply, "OK") != 0) {
        // beware of trailing spaces
        char* end = reply + strlen(reply);
        while (end > reply && end[-1] == ' ') {
            end--;
        }
        *end = 0;

        char* lastSpace = strrchr(reply, ' ');
        // lastSpace should be preceded by "rssi" and followed by the value
        if (lastSpace && !strncmp(lastSpace - 4, "rssi", 4)) {
            sscanf(lastSpace + 1, "%d", &rssi);
        }
    }
    return rssi;
}

Int32 WifiNative::GetRssiCommand()
{
    return GetRssiHelper("DRIVER RSSI");
}

Int32 WifiNative::GetRssiApproxCommand()
{
    return GetRssiHelper("DRIVER RSSI-APPROX");
}

Int32 WifiNative::GetLinkSpeedCommand()
{
    char reply[256];
    int linkspeed;

    if (doCommand("DRIVER LINKSPEED", reply, sizeof(reply)) != 0) {
        return -1;
    }
    // reply comes back in the form "LinkSpeed XX" where XX is the
    // number we're interested in.
    sscanf(reply, "%*s %u", &linkspeed);
    return linkspeed;
}

String WifiNative::GetMacAddressCommand()
{
    char reply[256];
    char buf[256];

    if (doCommand("DRIVER MACADDR", reply, sizeof(reply)) != 0) {
        return String(NULL);
    }
    // reply comes back in the form "Macaddr = XX.XX.XX.XX.XX.XX" where XX
    // is the part of the string we're interested in.
    if (sscanf(reply, "%*s = %255s", buf) == 1) {
        return String(buf);
    }
    else {
        return String(NULL);
    }
}

String WifiNative::ScanResultsCommand()
{
    return doStringCommand("SCAN_RESULTS");
}

Boolean WifiNative::StartDriverCommand()
{
    return doBooleanCommand("DRIVER START", "OK");
}

Boolean WifiNative::StopDriverCommand()
{
    return doBooleanCommand("DRIVER STOP", "OK");
}

Boolean WifiNative::StartPacketFiltering()
{
    return doBooleanCommand("DRIVER RXFILTER-ADD 0", "OK")
            && doBooleanCommand("DRIVER RXFILTER-ADD 1", "OK")
            && doBooleanCommand("DRIVER RXFILTER-ADD 3", "OK")
            && doBooleanCommand("DRIVER RXFILTER-START", "OK");
}

Boolean WifiNative::StopPacketFiltering()
{
    Boolean result = doBooleanCommand("DRIVER RXFILTER-STOP", "OK");
    if (result) {
        doBooleanCommand("DRIVER RXFILTER-REMOVE 3", "OK");
        doBooleanCommand("DRIVER RXFILTER-REMOVE 1", "OK");
        doBooleanCommand("DRIVER RXFILTER-REMOVE 0", "OK");
    }

    return result;
}

Boolean WifiNative::SetPowerModeCommand(
    /* [in] */ Int32 mode)
{
    char cmdstr[256];

    Int32 numWritten = snprintf(cmdstr, sizeof(cmdstr), "DRIVER POWERMODE %d", mode);
    Boolean cmdTooLong = numWritten >= (int)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Int32 WifiNative::GetPowerModeCommand()
{
    char reply[256];
    int power;

    if (doCommand("DRIVER GETPOWER", reply, sizeof(reply)) != 0) {
        return -1;
    }
    // reply comes back in the form "powermode = XX" where XX is the
    // number we're interested in.
    if (sscanf(reply, "%*s = %u", &power) != 1) {
        return -1;
    }
    return (Int32)power;
}

Boolean WifiNative::SetNumAllowedChannelsCommand(
    /* [in] */ Int32 numChannels)
{
    char cmdstr[256];

    Int32 numWritten = snprintf(cmdstr, sizeof(cmdstr), "DRIVER SCAN-CHANNELS %u", numChannels);
    Boolean cmdTooLong = numWritten >= (int)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Int32 WifiNative::GetNumAllowedChannelsCommand()
{
    char reply[256];
    int numChannels;

    if (doCommand("DRIVER SCAN-CHANNELS", reply, sizeof(reply)) != 0) {
        return -1;
    }
    // reply comes back in the form "Scan-Channels = X" where X is the
    // number of channels
    if (sscanf(reply, "%*s = %u", &numChannels) == 1) {
        return (Int32)numChannels;
    }
    else {
        return -1;
    }
}

Boolean WifiNative::SetBluetoothCoexistenceModeCommand(
    /* [in] */ Int32 mode)
{
    char cmdstr[256];

    Int32 numWritten = snprintf(cmdstr, sizeof(cmdstr), "DRIVER BTCOEXMODE %d", mode);
    Boolean cmdTooLong = numWritten >= (int)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Boolean WifiNative::SetBluetoothCoexistenceScanModeCommand(
    /* [in] */ Boolean setCoexScanMode)
{
    char cmdstr[256];

    Int32 numWritten = snprintf(cmdstr, sizeof(cmdstr), "DRIVER BTCOEXSCAN-%s", setCoexScanMode ? "START" : "STOP");
    Boolean cmdTooLong = numWritten >= (int)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Boolean WifiNative::SaveConfigCommand()
{
    // Make sure we never write out a value for AP_SCAN other than 1
    doBooleanCommand("AP_SCAN 1", "OK");
    return doBooleanCommand("SAVE_CONFIG", "OK");
}

Boolean WifiNative::ReloadConfigCommand()
{
    return doBooleanCommand("RECONFIGURE", "OK");
}

Boolean WifiNative::SetScanResultHandlingCommand(
    /* [in] */ Int32 mode)
{
    char cmdstr[256];

    Int32 numWritten = snprintf(cmdstr, sizeof(cmdstr), "AP_SCAN %d", mode);
    Boolean cmdTooLong = numWritten >= (int)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Boolean WifiNative::AddToBlacklistCommand(
    /* [in] */ CString bssid)
{
    char cmdstr[256];

    const char *bssidStr = (const char*)bssid;

    Boolean cmdTooLong = snprintf(cmdstr, sizeof(cmdstr), "BLACKLIST %s", bssidStr) >= (int)sizeof(cmdstr);

    return !cmdTooLong && doBooleanCommand(cmdstr, "OK");
}

Boolean WifiNative::ClearBlacklistCommand()
{
    return doBooleanCommand("BLACKLIST clear", "OK");
}

// Boolean WifiNative::DoDhcpRequest(DhcpInfo results);

String WifiNative::GetDhcpError()
{
    return String(::get_dhcp_error_string());
}

Boolean WifiNative::SetSuspendOptimizationsCommand(
    /* [in] */ Boolean enabled)
{
    char cmdstr[25];

    snprintf(cmdstr, sizeof(cmdstr), "DRIVER SETSUSPENDOPT %d", enabled ? 0 : 1);
    return doBooleanCommand(cmdstr, "OK");
}

/**
 * Wait for the supplicant to send an event, returning the event string.
 * @return the event string sent by the supplicant.
 */
String WifiNative::WaitForEvent()
{
    char buf[256];

    int nread = ::wifi_wait_for_event(buf, sizeof buf);
    if (nread > 0) {
        return String(buf);
    }
    else {
        return  String(NULL);
    }
}
