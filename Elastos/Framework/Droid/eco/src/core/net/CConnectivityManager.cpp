
#include "ext/frameworkext.h"
#include "net/CConnectivityManager.h"

ECode CConnectivityManager::SetNetworkPreference(
    /* [in] */ Int32 preference)
{
    // try {
    return mService->SetNetworkPreference(preference);
    // } catch (RemoteException e) {
    // }
}

ECode CConnectivityManager::GetNetworkPreference(
    /* [out] */ Int32* preference)
{
    VALIDATE_NOT_NULL(preference);
    // try {
    return mService->GetNetworkPreference(preference);
    // } catch (RemoteException e) {
    //    return -1;
    // }
}

ECode CConnectivityManager::GetActiveNetworkInfo(
    /* [out] */ INetworkInfo** networkInfo)
{
    VALIDATE_NOT_NULL(networkInfo);
    // try {
    return mService->GetActiveNetworkInfo(networkInfo);
    // } catch (RemoteException e) {
    //    return null;
    // }
}

ECode CConnectivityManager::GetNetworkInfo(
    /* [in] */ Int32 networkType,
    /* [out] */ INetworkInfo** networkInfo)
{
    VALIDATE_NOT_NULL(networkInfo);
    // try {
    return mService->GetNetworkInfo(networkType, networkInfo);
    // } catch (RemoteException e) {
    //    return null;
    // }
}

ECode CConnectivityManager::GetAllNetworkInfo(
    /* [out] */ IObjectContainer** networkInfo)
{
    VALIDATE_NOT_NULL(networkInfo);
    // try {
    return mService->GetAllNetworkInfo(networkInfo);
    // } catch (RemoteException e) {
    //    return null;
    // }
}

ECode CConnectivityManager::SetRadios(
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    return mService->SetRadios(turnOn, result);
    // } catch (RemoteException e) {
    //    return false;
    // }
}

ECode CConnectivityManager::SetRadio(
    /* [in] */ Int32 networkType, 
    /* [in] */ Boolean turnOn, 
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    return mService->SetRadio(networkType, turnOn, result);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CConnectivityManager::StartUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ Int32* outcome)
{
    VALIDATE_NOT_NULL(outcome);
    // try {
    // TODO: ALEX
    // return mService->StartUsingNetworkFeature(networkType, feature, new Binder(), outcome);
    // } catch (RemoteException e) {
    //     return -1;
    // }
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConnectivityManager::StopUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ Int32* outcome)
{
    VALIDATE_NOT_NULL(outcome);
    // try {
    return mService->StopUsingNetworkFeature(networkType, feature, outcome);
    // } catch (RemoteException e) {
    //     return -1;
    // }
}

ECode CConnectivityManager::RequestRouteToHost(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    return mService->RequestRouteToHost(networkType, hostAddress, result);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CConnectivityManager::GetBackgroundDataSetting(
    /* [out] */ Boolean* isAllowed)
{
    VALIDATE_NOT_NULL(isAllowed);
    // try {
    return mService->GetBackgroundDataSetting(isAllowed);
    // } catch (RemoteException e) {
        // Err on the side of safety
    //     return false;
    // }
}

ECode CConnectivityManager::constructor(
    /* [in] */ IConnectivityManager* service)
{
    if (!service) {
        // throw new IllegalArgumentException(
        //    "ConnectivityManager() cannot be constructed with null service");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
 
    mService = service;
    
    return NOERROR;
}

ECode CConnectivityManager::SetBackgroundDataSetting(
    /* [in] */ Boolean allowBackgroundData)
{
    // try {
    return mService->SetBackgroundDataSetting(allowBackgroundData);
    // } catch (RemoteException e) {
    // }
}

ECode CConnectivityManager::GetMobileDataEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    return mService->GetMobileDataEnabled(result);
    // } catch (RemoteException e) {
    //     return true;
    // }
}

ECode CConnectivityManager::SetMobileDataEnabled(
    /* [in] */ Boolean enabled)
{
    // try {
    return mService->SetMobileDataEnabled(enabled);
    // } catch (RemoteException e) {
    // }
}

ECode CConnectivityManager::GetTetherableIfaces(
    /* [out, callee] */ ArrayOf<String>** result) 
{
    VALIDATE_NOT_NULL(result);
    // try {
    return mService->GetTetherableIfaces(result);
    // } catch (RemoteException e) {
    //    return new String[0];
    // }
}

ECode CConnectivityManager::GetTetheredIfaces(
    /* [out, callee] */ ArrayOf<String>** result) 
{
    // try {
    return mService->GetTetheredIfaces(result);
    // } catch (RemoteException e) {
    //     return new String[0];
    // }
}

ECode CConnectivityManager::GetTetheringErroredIfaces(
    /* [out, callee] */ ArrayOf<String>** result) 
{
    // try {
    return mService->GetTetheringErroredIfaces(result);
    // } catch (RemoteException e) {
    //     return new String[0];
    // }
}

ECode CConnectivityManager::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result) 
{
    // try {
    return mService->Tether(iface, result);
    // } catch (RemoteException e) {
    //     return TETHER_ERROR_SERVICE_UNAVAIL;
    // }
}

ECode CConnectivityManager::Untether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result) 
{
    // try {
    return mService->Untether(iface, result);
    // } catch (RemoteException e) {
    //     return TETHER_ERROR_SERVICE_UNAVAIL;
    // }
}

ECode CConnectivityManager::IsTetheringSupported(
    /* [out] */ Boolean* result) 
{
    // try {
    return mService->IsTetheringSupported(result);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CConnectivityManager::GetTetherableUsbRegexs(
    /* [out, callee] */ ArrayOf<String>** result) 
{
    // try {
    return mService->GetTetherableUsbRegexs(result);
    // } catch (RemoteException e) {
    //     return new String[0];
    // }
}

ECode CConnectivityManager::GetTetherableWifiRegexs(
    /* [out, callee] */ ArrayOf<String>** result) 
{
    // try {
    return mService->GetTetherableWifiRegexs(result);
    // } catch (RemoteException e) {
    //     return new String[0];
    // }
}

/**
 * @param iface The name of the interface we're interested in
 * @return error The error code of the last error tethering or untethering the named
 *               interface
 * {@hide}
 */
ECode CConnectivityManager::GetLastTetherError(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result) 
{
    // try {
    return mService->GetLastTetherError(iface, result);
    // } catch (RemoteException e) {
    //     return TETHER_ERROR_SERVICE_UNAVAIL;
    // }
}

/**
 * @param networkType The type of network you want to report on
 * @param percentage The quality of the connection 0 is bad, 100 is good
 * {@hide}
 */
ECode CConnectivityManager::ReportInetCondition(
    /* [in] */ Int32 networkType, 
    /* [in] */ Int32 percentage) 
{
    // try {
    return mService->ReportInetCondition(networkType, percentage);
    // } catch (RemoteException e) {
    // }
}

Boolean CConnectivityManager::IsNetworkTypeValid(
    /* [in] */ Int32 networkType)
{
    return ((networkType >= 0) 
        && (networkType <= ConnectivityManager_MAX_NETWORK_TYPE));
}

CConnectivityManager::CConnectivityManager() : mService(NULL)
{
}

