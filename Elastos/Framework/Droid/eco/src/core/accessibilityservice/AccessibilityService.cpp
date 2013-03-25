
#include "ext/frameworkdef.h"
#include "accessibilityservice/AccessibilityService.h"

const CString AccessibilityService::TAG = "AccessibilityService";

ECode AccessibilityService::OnServiceConnected()
{
    return NOERROR;
}

ECode AccessibilityService::SetServiceInfo(
    /* [in] */ IAccessibilityServiceInfo* info)
{
    mInfo = info;
    SendServiceInfo();
    return NOERROR;
}

void AccessibilityService::SendServiceInfo()
{
    if (mInfo != NULL && mConnection != NULL) {
        //try {
        ECode ec = mConnection->SetServiceInfo(mInfo);
        if (FAILED(ec)) {
            //     Log.w(LOG_TAG, "Error while setting AccessibilityServiceInfo", re);
        }
        // } catch (RemoteException re) {
        //     Log.w(LOG_TAG, "Error while setting AccessibilityServiceInfo", re);
        // }
    }
}

ECode AccessibilityService::SetConnection(
    /* [in] */ IAccessibilityServiceConnection* connection)
{
    mConnection = connection;
    return NOERROR;
}

ECode AccessibilityService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    //return new IEventListenerWrapper(this);
    return NOERROR;
}
