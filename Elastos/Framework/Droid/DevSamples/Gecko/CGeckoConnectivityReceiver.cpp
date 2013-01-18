
#include "CGeckoConnectivityReceiver.h"
#include "GeckoApp.h"
#include "GeckoAppShell.h"

ECode CGeckoConnectivityReceiver::OnReciever(
    /* [in] */ IContext* pContext,
    /* [in] */ IIntent *pIntent)
{
    String status;
    ECode ec;
    INetworkInfo* pInfo;
    IConnectivityManager* pCm;
    ec = pContext->GetSystemService(
        Context_CONNECTIVITY_SERVICE, (IInterface**)&pCm);
    if (FAILED(ec)) return ec;

    ec = pCm->GetActiveNetworkInfo(&pInfo);
    pCm->Release();
    if (FAILED(ec)) return ec;

    if (!pInfo) {
        status = "unknown";
    }
    else {
        Boolean isConnected;
        ec = pInfo->IsConnected(&isConnected);
        pInfo->Release();
        if (FAILED(ec)) return ec;

        if (!isConnected) {
            status = "down";
        }
        else {
            status = "up";
        }
    }

    Boolean result;
    GeckoApp::CheckLaunchState(GeckoApp::LaunchState_GeckoRunning, &result);
    if (result) {
        GeckoAppShell::OnChangeNetworkLinkStatus(status);
    }
    return NOERROR;
}

