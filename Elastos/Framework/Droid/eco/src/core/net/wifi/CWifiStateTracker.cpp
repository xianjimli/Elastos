
#include "ext/frameworkdef.h"
#include "net/wifi/CWifiStateTracker.h"
#include "net/wifi/WifiNative.h"

/**
 * Load the driver and firmware
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::LoadDriver(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    *succeeded = WifiNative::LoadDriver();
    return NOERROR;
}

/**
 * Unload the driver and firmware
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::UnloadDriver(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    *succeeded = WifiNative::UnloadDriver();
    return NOERROR;
}

/**
 * Check the supplicant config and
 * start the supplicant daemon
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::StartSupplicant(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    *succeeded = WifiNative::StartSupplicant();
    return NOERROR;
}

/**
 * Stop the supplicant daemon
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::StopSupplicant(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    *succeeded = WifiNative::StopSupplicant();
    return NOERROR;
}

/**
 * Establishes two channels - control channel for commands
 * and monitor channel for notifying WifiMonitor
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::ConnectToSupplicant(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    *succeeded = WifiNative::ConnectToSupplicant();
    return NOERROR;
}

/**
 * Close the control/monitor channels to supplicant
 */
ECode CWifiStateTracker::CloseSupplicantConnection()
{
    Mutex::Autolock lock(_m_syncLock);

    WifiNative::CloseSupplicantConnection();
    return NOERROR;
}

/**
 * Check if the supplicant is alive
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::Ping(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = WifiNative::PingCommand();
    return NOERROR;
}

/**
 * initiate an active or passive scan
 *
 * @param forceActive true if it is a active scan
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::Scan(
    /* [in] */ Boolean forceActive,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = WifiNative::ScanCommand(forceActive);
    return NOERROR;
}

/**
 * Specifies whether the supplicant or driver
 * take care of initiating scan and doing AP selection
 *
 * @param mode
 *    SUPPL_SCAN_HANDLING_NORMAL
 *    SUPPL_SCAN_HANDLING_LIST_ONLY
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::SetScanResultHandling(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = WifiNative::SetScanResultHandlingCommand(mode);
    return NOERROR;
}

/**
 * Fetch the scan results from the supplicant
 *
 * @return example result string
 * 00:bb:cc:dd:cc:ee       2427    166     [WPA-EAP-TKIP][WPA2-EAP-CCMP]   Net1
 * 00:bb:cc:dd:cc:ff       2412    165     [WPA-EAP-TKIP][WPA2-EAP-CCMP]   Net2
 */
ECode CWifiStateTracker::ScanResults(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *result = NULL;
    }
    *result = WifiNative::ScanResultsCommand();
    return NOERROR;
}

/**
 * Set the scan mode - active or passive
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::SetScanMode(
    /* [in] */ Boolean isScanModeActive,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
        return NOERROR;
    }
    if (mIsScanModeActive != isScanModeActive) {
        *succeeded = WifiNative::SetScanModeCommand(mIsScanModeActive = isScanModeActive);
        return NOERROR;
    }
    *succeeded = TRUE;
    return NOERROR;
}

/**
 * Disconnect from Access Point
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::Disconnect(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
    }
    *succeeded = WifiNative::DisconnectCommand();
    return NOERROR;
}

/**
 * Initiate a reconnection to AP
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::ReconnectCommand(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
    }
    *succeeded = WifiNative::ReconnectCommand();
    return NOERROR;
}

/**
 * Add a network
 *
 * @return network id of the new network
 */
ECode CWifiStateTracker::AddNetwork(
    /* [out] */ Int32* netId)
{
    VALIDATE_NOT_NULL(netId);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *netId = -1;
        return NOERROR;
    }
    *netId = WifiNative::AddNetworkCommand();
    return NOERROR;
}

/**
 * Delete a network
 *
 * @param networkId id of the network to be removed
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::RemoveNetwork(
    /* [in] */ Int32 networkId,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = mDisconnectExpected = WifiNative::RemoveNetworkCommand(networkId);
    return NOERROR;
}

/**
 * Enable a network
 *
 * @param netId network id of the network
 * @param disableOthers true, if all other networks have to be disabled
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::EnableNetwork(
    /* [in] */ Int32 netId,
    /* [in] */ Boolean disableOthers,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
    }
    if (disableOthers) mIsAnyNetworkDisabled.set(TRUE);
    *succeeded = WifiNative::EnableNetworkCommand(netId, disableOthers);
    return NOERROR;
}

/**
 * Enable all networks
 *
 * @param networks list of configured networks
 */
//        public synchronized void enableAllNetworks(List<WifiConfiguration> networks)

/**
 * Disable a network
 *
 * @param netId network id of the network
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::DisableNetwork(
    /* [in] */ Int32 netId,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
        return NOERROR;
    }
    mIsAnyNetworkDisabled.set(TRUE);
    *succeeded = WifiNative::DisableNetworkCommand(netId);
    return NOERROR;
}

/**
 * Initiate a re-association in supplicant
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::Reassociate(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
    }
    *succeeded = WifiNative::ReassociateCommand();
    return NOERROR;
}

/**
 * Blacklist a BSSID. This will avoid the AP if there are
 * alternate APs to connect
 *
 * @param bssid BSSID of the network
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::AddToBlacklist(
    /* [in] */ const String& bssid,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
    }
    *succeeded = WifiNative::AddToBlacklistCommand(bssid);
    return NOERROR;
}

/**
 * Clear the blacklist list
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::ClearBlacklist(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
    }
    *succeeded = WifiNative::ClearBlacklistCommand();
    return NOERROR;
}

/**
 * List all configured networks
 *
 * @return list of networks or null on failure
 */
ECode CWifiStateTracker::ListNetworks(
    /* [out] */ String* networks)
{
    VALIDATE_NOT_NULL(networks);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *networks = NULL;
        return NOERROR;
    }
    *networks = WifiNative::ListNetworksCommand();
    return NOERROR;
}

/**
 * Get network setting by name
 *
 * @param netId network id of the network
 * @param name network variable key
 * @return value corresponding to key
 */
ECode CWifiStateTracker::GetNetworkVariable(
    /* [in] */ Int32 netId,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *value = NULL;
        return NOERROR;
    }
    *value = WifiNative::GetNetworkVariableCommand(netId, name);
    return NOERROR;
}

/**
 * Set network setting by name
 *
 * @param netId network id of the network
 * @param name network variable key
 * @param value network variable value
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::SetNetworkVariable(
    /* [in] */ Int32 netId,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = WifiNative::SetNetworkVariableCommand(netId, name, value);
    return NOERROR;
}

/**
 * Get detailed status of the connection
 *
 * @return Example status result
 *  bssid=aa:bb:cc:dd:ee:ff
 *  ssid=TestNet
 *  id=3
 *  pairwise_cipher=NONE
 *  group_cipher=NONE
 *  key_mgmt=NONE
 *  wpa_state=COMPLETED
 *  ip_address=X.X.X.X
 */
ECode CWifiStateTracker::GetStatus(
    /* [out] */ String* status)
{
    VALIDATE_NOT_NULL(status);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *status = NULL;
        return NOERROR;
    }
    *status = WifiNative::StatusCommand();
    return NOERROR;
}

/**
 * Get RSSI to currently connected network
 *
 * @return RSSI value, -1 on failure
 */
ECode CWifiStateTracker::GetRssi(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *value = -1;
        return NOERROR;
    }
    *value = WifiNative::GetRssiApproxCommand();
    return NOERROR;
}

/**
 * Get approx RSSI to currently connected network
 *
 * @return RSSI value, -1 on failure
 */
ECode CWifiStateTracker::GetRssiApprox(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *value = -1;
        return NOERROR;
    }
    *value = WifiNative::GetRssiApproxCommand();
    return NOERROR;
}

/**
 * Get link speed to currently connected network
 *
 * @return link speed, -1 on failure
 */
ECode CWifiStateTracker::GetLinkSpeed(
    /* [out] */ Int32* speed)
{
    VALIDATE_NOT_NULL(speed);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *speed = -1;
        return NOERROR;
    }
    *speed = WifiNative::GetLinkSpeedCommand();
    return NOERROR;
}

/**
 * Start driver
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::StartDriver(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = WifiNative::StartDriverCommand();
    return NOERROR;
}

/**
 * Stop driver
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::StopDriver(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    /* Driver stop should not happen only when supplicant event
     * DRIVER_STOPPED has already been handled */
    if (mWifiState.get() != WIFI_STATE_ENABLED || mRunState == RUN_STATE_STOPPED) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = WifiNative::StopDriverCommand();
    return NOERROR;
}

/**
 * Start packet filtering
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::StartPacketFiltering(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = WifiNative::StartPacketFiltering();
    return NOERROR;
}

/**
 * Stop packet filtering
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::StopPacketFiltering(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
    }
    *succeeded = WifiNative::StopPacketFiltering();
    return NOERROR;
}

/**
 * Get power mode
 * @return power mode
 */
ECode CWifiStateTracker::GetPowerMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED && !IsDriverStopped()) {
        *mode = -1;
        return NOERROR;
    }
    *mode = WifiNative::GetPowerModeCommand();
    return NOERROR;
}

/**
 * Set the number of allowed radio frequency channels from the system
 * setting value, if any.
 * @return {@code true} if the operation succeeds, {@code false} otherwise, e.g.,
 * the number of channels is invalid.
 */
ECode CWifiStateTracker::SetNumAllowedChannels(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || isDriverStopped()) {
        return false;
    }
    try {
        return setNumAllowedChannels(
                Settings.Secure.getInt(mContext.getContentResolver(),
                Settings.Secure.WIFI_NUM_ALLOWED_CHANNELS));
    } catch (Settings.SettingNotFoundException e) {
        if (mNumAllowedChannels != 0) {
            WifiNative.setNumAllowedChannelsCommand(mNumAllowedChannels);
        }
        // otherwise, use the driver default
    }
    return true;
}

/**
 * Set the number of radio frequency channels that are allowed to be used
 * in the current regulatory domain.
 * @param numChannels the number of allowed channels. Must be greater than 0
 * and less than or equal to 16.
 * @return {@code true} if the operation succeeds, {@code false} otherwise, e.g.,
 * {@code numChannels} is outside the valid range.
 */
ECode CWifiStateTracker::SetNumAllowedChannelsEx(
    /* [in] */ Int32 numChannels,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
        return NOERROR;
    }
    mNumAllowedChannels = numChannels;
    *succeeded = WifiNative::SetNumAllowedChannelsCommand(numChannels);
    return NOERROR;
}

/**
 * Get number of allowed channels
 *
 * @return channel count, -1 on failure
 */
ECode CWifiStateTracker::GetNumAllowedChannels(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *count = -1;
        return NOERROR;
    }
    *count = WifiNative::GetNumAllowedChannelsCommand();
    return NOERROR;
}

/**
 * Set bluetooth coex mode:
 *
 * @param mode
 *  BLUETOOTH_COEXISTENCE_MODE_ENABLED
 *  BLUETOOTH_COEXISTENCE_MODE_DISABLED
 *  BLUETOOTH_COEXISTENCE_MODE_SENSE
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::SetBluetoothCoexistenceMode(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        *succeeded = FALSE;
        return NOERROR;
    }
    *succeeded = WifiNative::SetBluetoothCoexistenceModeCommand(mode);
    return NOERROR;
}

/**
 * Enable or disable Bluetooth coexistence scan mode. When this mode is on,
 * some of the low-level scan parameters used by the driver are changed to
 * reduce interference with A2DP streaming.
 *
 * @param isBluetoothPlaying whether to enable or disable this mode
 */
ECode CWifiStateTracker::SetBluetoothScanMode(
    /* [in] */ Boolean isBluetoothPlaying)
{
    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED || IsDriverStopped()) {
        return NOERROR;
    }
    WifiNative::SetBluetoothCoexistenceScanModeCommand(isBluetoothPlaying);
    return NOERROR;
}

/**
 * Save configuration on supplicant
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::SaveConfig(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
    }
    *succeeded = WifiNative::SaveConfigCommand();
    return NOERROR;
}

/**
 * Reload the configuration from file
 *
 * @return {@code true} if the operation succeeds, {@code false} otherwise
 */
ECode CWifiStateTracker::ReloadConfig(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(_m_syncLock);

    if (mWifiState.get() != WIFI_STATE_ENABLED) {
        *succeeded = FALSE;
    }
    *succeeded = WifiNative::ReloadConfigCommand();
    return NOERROR;
}

public boolean setRadio(boolean turnOn) {
    return mWM.setWifiEnabled(turnOn);
}

/**
 * {@inheritDoc}
 * There are currently no Wi-Fi-specific features supported.
 * @param feature the name of the feature
 * @return {@code -1} indicating failure, always
 */
ECode CWifiStateTracker::StartUsingNetworkFeature(
    /* [in] */ const String& feature,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = -1;
    return NOERROR;
}

/**
 * {@inheritDoc}
 * There are currently no Wi-Fi-specific features supported.
 * @param feature the name of the feature
 * @return {@code -1} indicating failure, always
 */
ECode CWifiStateTracker::StopUsingNetworkFeature(
    /* [in] */ const String& feature,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = -1;
    return NOERROR;
}

/**
 * Display or don't display a notification that there are open Wi-Fi networks.
 * @param visible {@code true} if notification should be visible, {@code false} otherwise
 * @param numNetworks the number networks seen
 * @param force {@code true} to force notification to be shown/not-shown,
 * even if it is already shown/not-shown.
 * @param delay time in milliseconds after which the notification should be made
 * visible or invisible.
 */
ECode CWifiStateTracker::SetNotificationVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Int32 numNetworks,
    /* [in] */ Boolean force,
    /* [in] */ Int32 delay)
{
    // Since we use auto cancel on the notification, when the
    // mNetworksAvailableNotificationShown is true, the notification may
    // have actually been canceled.  However, when it is false we know
    // for sure that it is not being shown (it will not be shown any other
    // place than here)

    // If it should be hidden and it is already hidden, then noop
    if (!visible && !mNotificationShown && !force) {
        return;
    }

    Message message;
    if (visible) {

        // Not enough time has passed to show the notification again
        if (System.currentTimeMillis() < mNotificationRepeatTime) {
            return;
        }

        if (mNotification == null) {
            // Cache the Notification mainly so we can remove the
            // EVENT_NOTIFICATION_CHANGED message with this Notification from
            // the queue later
            mNotification = new Notification();
            mNotification.when = 0;
            mNotification.icon = ICON_NETWORKS_AVAILABLE;
            mNotification.flags = Notification.FLAG_AUTO_CANCEL;
            mNotification.contentIntent = PendingIntent.getActivity(mContext, 0,
                    new Intent(WifiManager.ACTION_PICK_WIFI_NETWORK), 0);
        }

        CharSequence title = mContext.getResources().getQuantityText(
                com.android.internal.R.plurals.wifi_available, numNetworks);
        CharSequence details = mContext.getResources().getQuantityText(
                com.android.internal.R.plurals.wifi_available_detailed, numNetworks);
        mNotification.tickerText = title;
        mNotification.setLatestEventInfo(mContext, title, details, mNotification.contentIntent);

        mNotificationRepeatTime = System.currentTimeMillis() + NOTIFICATION_REPEAT_DELAY_MS;

        message = mTarget.obtainMessage(EVENT_NOTIFICATION_CHANGED, 1,
                ICON_NETWORKS_AVAILABLE, mNotification);

    } else {

        // Remove any pending messages to show the notification
        mTarget.removeMessages(EVENT_NOTIFICATION_CHANGED, mNotification);

        message = mTarget.obtainMessage(EVENT_NOTIFICATION_CHANGED, 0, ICON_NETWORKS_AVAILABLE);
    }

    mTarget.sendMessageDelayed(message, delay);

    mNotificationShown = visible;
}
