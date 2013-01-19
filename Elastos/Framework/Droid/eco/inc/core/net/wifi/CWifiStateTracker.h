
#ifndef __CWIFISTATETRACKER_H__
#define __CWIFISTATETRACKER_H__

#include "_CWifiStateTracker.h"


CarClass(CWifiStateTracker)
{
public:
    /**
     * Load the driver and firmware
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI LoadDriver(
        /* [out] */ Boolean* succeeded);

    /**
     * Unload the driver and firmware
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI UnloadDriver(
        /* [out] */ Boolean* succeeded);

    /**
     * Check the supplicant config and
     * start the supplicant daemon
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI StartSupplicant(
        /* [out] */ Boolean* succeeded);

    /**
     * Stop the supplicant daemon
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI StopSupplicant(
        /* [out] */ Boolean* succeeded);

    /**
     * Establishes two channels - control channel for commands
     * and monitor channel for notifying WifiMonitor
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI ConnectToSupplicant(
        /* [out] */ Boolean* succeeded);

    /**
     * Close the control/monitor channels to supplicant
     */
    CARAPI CloseSupplicantConnection();

    /**
     * Check if the supplicant is alive
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI Ping(
        /* [out] */ Boolean* succeeded);

    /**
     * initiate an active or passive scan
     *
     * @param forceActive true if it is a active scan
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI Scan(
        /* [in] */ Boolean forceActive,
        /* [out] */ Boolean* succeeded);

    /**
     * Specifies whether the supplicant or driver
     * take care of initiating scan and doing AP selection
     *
     * @param mode
     *    SUPPL_SCAN_HANDLING_NORMAL
     *    SUPPL_SCAN_HANDLING_LIST_ONLY
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI SetScanResultHandling(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* succeeded);

    /**
     * Fetch the scan results from the supplicant
     *
     * @return example result string
     * 00:bb:cc:dd:cc:ee       2427    166     [WPA-EAP-TKIP][WPA2-EAP-CCMP]   Net1
     * 00:bb:cc:dd:cc:ff       2412    165     [WPA-EAP-TKIP][WPA2-EAP-CCMP]   Net2
     */
    CARAPI ScanResults(
        /* [out] */ String* result);

    /**
     * Set the scan mode - active or passive
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI SetScanMode(
        /* [in] */ Boolean isScanModeActive,
        /* [out] */ Boolean* succeeded);

    /**
     * Disconnect from Access Point
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI Disconnect(
        /* [out] */ Boolean* succeeded);

    /**
     * Initiate a reconnection to AP
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI ReconnectCommand(
        /* [out] */ Boolean* succeeded);

    /**
     * Add a network
     *
     * @return network id of the new network
     */
    CARAPI AddNetwork(
        /* [out] */ Int32* netId);

    /**
     * Delete a network
     *
     * @param networkId id of the network to be removed
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI RemoveNetwork(
        /* [in] */ Int32 networkId,
        /* [out] */ Boolean* succeeded);

    /**
     * Enable a network
     *
     * @param netId network id of the network
     * @param disableOthers true, if all other networks have to be disabled
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI EnableNetwork(
        /* [in] */ Int32 netId,
        /* [in] */ Boolean disableOthers,
        /* [out] */ Boolean* succeeded);

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
    CARAPI DisableNetwork(
        /* [in] */ Int32 netId,
        /* [out] */ Boolean* succeeded);

    /**
     * Initiate a re-association in supplicant
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI Reassociate(
        /* [out] */ Boolean* succeeded);

    /**
     * Blacklist a BSSID. This will avoid the AP if there are
     * alternate APs to connect
     *
     * @param bssid BSSID of the network
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI AddToBlacklist(
        /* [in] */ const String& bssid,
        /* [out] */ Boolean* succeeded);

    /**
     * Clear the blacklist list
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI ClearBlacklist(
        /* [out] */ Boolean* succeeded);

    /**
     * List all configured networks
     *
     * @return list of networks or null on failure
     */
    CARAPI ListNetworks(
        /* [out] */ String* networks);

    /**
     * Get network setting by name
     *
     * @param netId network id of the network
     * @param name network variable key
     * @return value corresponding to key
     */
    CARAPI GetNetworkVariable(
        /* [in] */ Int32 netId,
        /* [in] */ const String& name,
        /* [out] */ String* value);

    /**
     * Set network setting by name
     *
     * @param netId network id of the network
     * @param name network variable key
     * @param value network variable value
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI SetNetworkVariable(
        /* [in] */ Int32 netId,
        /* [in] */ const String& name,
        /* [in] */ const String& value,
        /* [out] */ Boolean* succeeded);

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
    CARAPI GetStatus(
        /* [out] */ String* status);

    /**
     * Get RSSI to currently connected network
     *
     * @return RSSI value, -1 on failure
     */
    CARAPI GetRssi(
        /* [out] */ Int32* value);

    /**
     * Get approx RSSI to currently connected network
     *
     * @return RSSI value, -1 on failure
     */
    CARAPI GetRssiApprox(
        /* [out] */ Int32* value);

    /**
     * Get link speed to currently connected network
     *
     * @return link speed, -1 on failure
     */
    CARAPI GetLinkSpeed(
        /* [out] */ Int32* speed);

    /**
     * Start driver
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI StartDriver(
        /* [out] */ Boolean* succeeded);

    /**
     * Stop driver
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI StopDriver(
        /* [out] */ Boolean* succeeded);

    /**
     * Start packet filtering
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI StartPacketFiltering(
        /* [out] */ Boolean* succeeded);

    /**
     * Stop packet filtering
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI StopPacketFiltering(
        /* [out] */ Boolean* succeeded);

    /**
     * Get power mode
     * @return power mode
     */
    CARAPI GetPowerMode(
        /* [out] */ Int32* mode);

    /**
     * Set the number of allowed radio frequency channels from the system
     * setting value, if any.
     * @return {@code true} if the operation succeeds, {@code false} otherwise, e.g.,
     * the number of channels is invalid.
     */
    CARAPI SetNumAllowedChannels(
        /* [out] */ Boolean* succeeded);

    /**
     * Set the number of radio frequency channels that are allowed to be used
     * in the current regulatory domain.
     * @param numChannels the number of allowed channels. Must be greater than 0
     * and less than or equal to 16.
     * @return {@code true} if the operation succeeds, {@code false} otherwise, e.g.,
     * {@code numChannels} is outside the valid range.
     */
    CARAPI SetNumAllowedChannelsEx(
        /* [in] */ Int32 numChannels,
        /* [out] */ Boolean* succeeded);

    /**
     * Get number of allowed channels
     *
     * @return channel count, -1 on failure
     */
    CARAPI GetNumAllowedChannels(
        /* [out] */ Int32* count);

    /**
     * Set bluetooth coex mode:
     *
     * @param mode
     *  BLUETOOTH_COEXISTENCE_MODE_ENABLED
     *  BLUETOOTH_COEXISTENCE_MODE_DISABLED
     *  BLUETOOTH_COEXISTENCE_MODE_SENSE
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI SetBluetoothCoexistenceMode(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* succeeded);

    /**
     * Enable or disable Bluetooth coexistence scan mode. When this mode is on,
     * some of the low-level scan parameters used by the driver are changed to
     * reduce interference with A2DP streaming.
     *
     * @param isBluetoothPlaying whether to enable or disable this mode
     */
    CARAPI SetBluetoothScanMode(
        /* [in] */ Boolean isBluetoothPlaying);

    /**
     * Save configuration on supplicant
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI SaveConfig(
        /* [out] */ Boolean* succeeded);

    /**
     * Reload the configuration from file
     *
     * @return {@code true} if the operation succeeds, {@code false} otherwise
     */
    CARAPI ReloadConfig(
        /* [out] */ Boolean* succeeded);

    /**
     * {@inheritDoc}
     * There are currently no Wi-Fi-specific features supported.
     * @param feature the name of the feature
     * @return {@code -1} indicating failure, always
     */
    CARAPI StartUsingNetworkFeature(
        /* [in] */ const String& feature,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ Int32* result);

    /**
     * {@inheritDoc}
     * There are currently no Wi-Fi-specific features supported.
     * @param feature the name of the feature
     * @return {@code -1} indicating failure, always
     */
    CARAPI StopUsingNetworkFeature(
        /* [in] */ const String& feature,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ Int32* result);

    /**
     * Display or don't display a notification that there are open Wi-Fi networks.
     * @param visible {@code true} if notification should be visible, {@code false} otherwise
     * @param numNetworks the number networks seen
     * @param force {@code true} to force notification to be shown/not-shown,
     * even if it is already shown/not-shown.
     * @param delay time in milliseconds after which the notification should be made
     * visible or invisible.
     */
    CARAPI SetNotificationVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Int32 numNetworks,
        /* [in] */ Boolean force,
        /* [in] */ Int32 delay);
};

#endif //__CWIFISTATETRACKER_H__
