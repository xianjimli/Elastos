
#ifndef __CCONNECTIVITYMANAGER_H__
#define __CCONNECTIVITYMANAGER_H__

#include "_CConnectivityManager.h"
#include <elastos/AutoPtr.h>

CarClass(CConnectivityManager)
{
public:
    CARAPI SetNetworkPreference(
        /* [in] */ Int32 preference);

    CARAPI GetNetworkPreference(
        /* [out] */ Int32* preference);

    CARAPI GetActiveNetworkInfo(
        /* [out] */ INetworkInfo** networkInfo);

    CARAPI GetNetworkInfo(
        /* [in] */ Int32 networkType,
        /* [out] */ INetworkInfo ** ppNetworkInfo);

    CARAPI GetAllNetworkInfo(
        /* [out] */ IObjectContainer** networkInfo);

    /** {@hide} */
    CARAPI SetRadios(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    /** {@hide} */
    CARAPI SetRadio(
        /* [in] */ Int32 networkType,
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    /**
     * Tells the underlying networking system that the caller wants to
     * begin using the named feature. The interpretation of {@code feature}
     * is completely up to each networking implementation.
     * @param networkType specifies which network the request pertains to
     * @param feature the name of the feature to be used
     * @return an integer value representing the outcome of the request.
     * The interpretation of this value is specific to each networking
     * implementation+feature combination, except that the value {@code -1}
     * always indicates failure.
     */
    CARAPI StartUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature,
        /* [out] */ Int32* outcome);

    CARAPI StopUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature,
        /* [out] */ Int32* outcome);

    CARAPI RequestRouteToHost(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 hostAddress,
        /* [out] */ Boolean* result);

    CARAPI GetBackgroundDataSetting(
        /* [out] */ Boolean* isAllowed);

    /**
     * Sets the value of the setting for background data usage.
     *
     * @param allowBackgroundData Whether an application should use data while
     *            it is in the background.
     *
     * @attr ref android.Manifest.permission#CHANGE_BACKGROUND_DATA_SETTING
     * @see #getBackgroundDataSetting()
     * @hide
     */
    CARAPI SetBackgroundDataSetting(
        /* [in] */ Boolean allowBackgroundData);

    /**
     * Gets the value of the setting for enabling Mobile data.
     *
     * @return Whether mobile data is enabled.
     * @hide
     */
    CARAPI GetMobileDataEnabled(
        /* [out] */ Boolean* result);

    /**
     * Sets the persisted value for enabling/disabling Mobile data.
     *
     * @param enabled Whether the mobile data connection should be
     *            used or not.
     * @hide
     */
    CARAPI SetMobileDataEnabled(
        /* [in] */ Boolean enabled);

    CARAPI constructor(
        /* [in] */ IConnectivityManager* service);

    /**
     * {@hide}
     */
    CARAPI GetTetherableIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    /**
     * {@hide}
     */
    CARAPI GetTetheredIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    /**
     * {@hide}
     */
    CARAPI GetTetheringErroredIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    /**
     * @return error A TETHER_ERROR value indicating success or failure type
     * {@hide}
     */
    CARAPI Tether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

    /**
     * @return error A TETHER_ERROR value indicating success or failure type
     * {@hide}
     */
    CARAPI Untether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

    /**
     * {@hide}
     */
    CARAPI IsTetheringSupported(
        /* [out] */ Boolean* result);

    /**
     * {@hide}
     */
    CARAPI GetTetherableUsbRegexs(
        /* [out, callee] */ ArrayOf<String>** result);

    /**
     * {@hide}
     */
    CARAPI GetTetherableWifiRegexs(
        /* [out, callee] */ ArrayOf<String>** result);

    /**
     * @param iface The name of the interface we're interested in
     * @return error The error code of the last error tethering or untethering the named
     *               interface
     * {@hide}
     */
    CARAPI GetLastTetherError(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

    /**
     * @param networkType The type of network you want to report on
     * @param percentage The quality of the connection 0 is bad, 100 is good
     * {@hide}
     */
    CARAPI ReportInetCondition(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 percentage);

public:
    static CARAPI_(Boolean) IsNetworkTypeValid(
        /* [in] */ Int32 networkType);

public:
    CConnectivityManager();

private:
    AutoPtr<IConnectivityManager> mService;
};

#endif // __CCONNECTIVITYMANAGER_H__

