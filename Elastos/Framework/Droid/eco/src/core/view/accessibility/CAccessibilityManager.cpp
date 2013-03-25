

#include "view/accessibility/CAccessibilityManager.h"
#include "view/accessibility/CAccessibilityManagerClient.h"
#include "os/CApartment.h"
#include "os/CServiceManager.h"
#include "os/SystemClock.h"
#include "os/Binder.h"

const CString CAccessibilityManager::TAG = "AccessibilityManager";
const Int32 CAccessibilityManager::DO_SET_ENABLED;

Mutex CAccessibilityManager::sInstanceSync;
AutoPtr<ILocalAccessibilityManager> CAccessibilityManager::sInstance;

CAccessibilityManager::CAccessibilityManager()
    : mIsEnabled(FALSE)
{}

/**
 * Returns if the {@link AccessibilityManager} is enabled.
 *
 * @return True if this {@link AccessibilityManager} is enabled, false otherwise.
 */
ECode CAccessibilityManager::IsEnabled(
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);
    Mutex::Autolock lock(mHandlerLock);
    *isEnabled = mIsEnabled;
    return NOERROR;
}

/**
 * Sends an {@link AccessibilityEvent}. If this {@link AccessibilityManager} is not
 * enabled the call is a NOOP.
 *
 * @param event The {@link AccessibilityEvent}.
 *
 * @throws IllegalStateException if a client tries to send an {@link AccessibilityEvent}
 *         while accessibility is not enabled.
 */
ECode CAccessibilityManager::SendAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    if (!mIsEnabled) {
        return E_ILLEGAL_STATE_EXCEPTION;
        //throw new IllegalStateException("Accessibility off. Did you forget to check that?");
    }
    Boolean doRecycle = FALSE;
    //try {
    event->SetEventTime(SystemClock::GetUptimeMillis());
    // it is possible that this manager is in the same process as the service but
    // client using it is called through Binder from another process. Example: MMS
    // app adds a SMS notification and the NotificationManagerService calls this method
    Int64 identityToken = Binder::ClearCallingIdentity();
    ECode ec = mService->SendAccessibilityEvent(event, &doRecycle);
    if (FAILED(ec)) {
        //Log.e(LOG_TAG, "Error during sending " + event + " ", re);
        goto finally;
    }
    Binder::RestoreCallingIdentity(identityToken);
    // if (LOGV) {
    //     Log.i(LOG_TAG, event + " sent");
    // }
    // } catch (RemoteException re) {
    //     Log.e(LOG_TAG, "Error during sending " + event + " ", re);
    // } finally {
    //     if (doRecycle) {
    //         event.recycle();
    //     }
    // }
finally:
    if (doRecycle) {
        event->Recycle();
    }

    return NOERROR;
}

/**
 * Requests interruption of the accessibility feedback from all accessibility services.
 */
ECode CAccessibilityManager::Interrupt()
{
    if (!mIsEnabled) {
        return E_ILLEGAL_STATE_EXCEPTION;
        //throw new IllegalStateException("Accessibility off. Did you forget to check that?");
    }
    //try {
    ECode ec = mService->Interrupt();
    if (FAILED(ec)) {
        //Log.e(LOG_TAG, "Error while requesting interrupt from all services. ", re);
    }
    // if (LOGV) {
    //     Log.i(LOG_TAG, "Requested interrupt from all services");
    // }
    // } catch (RemoteException re) {
    //     Log.e(LOG_TAG, "Error while requesting interrupt from all services. ", re);
    // }

    return NOERROR;
}

/**
 * Returns the {@link ServiceInfo}s of the installed accessibility services.
 *
 * @return An unmodifiable list with {@link ServiceInfo}s.
 */
ECode CAccessibilityManager::GetAccessibilityServiceList(
    /* [out] */ IObjectContainer** container)
{
    VALIDATE_NOT_NULL(container);

    //try {
    ECode ec = mService->GetAccessibilityServiceList(container);
    if (FAILED(ec)) {
        //Log.e(LOG_TAG, "Error while obtaining the installed AccessibilityServices. ", re);
    }
    // if (LOGV) {
    //     Log.i(LOG_TAG, "Installed AccessibilityServices " + services);
    // }
    // } catch (RemoteException re) {
    //     Log.e(LOG_TAG, "Error while obtaining the installed AccessibilityServices. ", re);
    // }
    //return Collections.unmodifiableList(services);

    return NOERROR;
}

ECode CAccessibilityManager::constructor(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(CAccessibilityManagerClient::New(
            (ILocalAccessibilityManager*)this->Probe(EIID_ILocalAccessibilityManager),
            (IAccessibilityManagerClient**)&mClient));

    ASSERT_SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mHandler)
            && (mHandler != NULL));

    AutoPtr<IServiceManager> serviceManager;
    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
    AutoPtr<IAccessibilityManager> accessibilityManager;
    serviceManager->GetService(String("AccessibilityService"),
           (IInterface**)&mService);

    //try {
    mService->AddClient(mClient);
    // } catch (RemoteException re) {
    //     Log.e(LOG_TAG, "AccessibilityManagerService is dead", re);
    // }

    return NOERROR;
}

void CAccessibilityManager::HandleSetEnabled(
    /* [in] */ Boolean enabled)
{
    Mutex::Autolock lock(mHandlerLock);
    mIsEnabled = enabled;
}
