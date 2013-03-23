

#include "view/accessibility/CAccessibilityManager.h"
#include "view/accessibility/CAccessibilityManagerClient.h"
#include "os/CApartment.h"
#include "os/CServiceManager.h"

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
    return NOERROR;
}

/**
 * Requests interruption of the accessibility feedback from all accessibility services.
 */
ECode CAccessibilityManager::Interrupt()
{
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
    return NOERROR;
}

ECode CAccessibilityManager::constructor(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(CAccessibilityManagerClient::New(
            (ILocalAccessibilityManager*)this->Probe(EIID_ILocalAccessibilityManager),
            (IAccessibilityManagerClient**)&mClient));

    // ASSERT_SUCCEEDED(CApartment::GetDefaultApartment((IApartment**)&mHandler)
    //         && (mHandler != NULL));

    // AutoPtr<IServiceManager> serviceManager;
    // CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
    // AutoPtr<IAccessibilityManager> accessibilityManager;
    // serviceManager->GetService(String("AccessibilityService"),
    //        (IInterface**)&mService);

    // //try {
    // mService->AddClient(mClient);
    // } catch (RemoteException re) {
    //     Log.e(LOG_TAG, "AccessibilityManagerService is dead", re);
    // }

    return NOERROR;
}

void CAccessibilityManager::HandleSetEnabled(
    /* [in] */ Boolean enabled)
{}
