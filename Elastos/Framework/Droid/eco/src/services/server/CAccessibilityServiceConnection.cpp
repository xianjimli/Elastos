
#include "server/CAccessibilityServiceConnection.h"

CAccessibilityServiceConnection::CAccessibilityServiceConnection()
    : mId(0)
    , mEventTypes(0)
    , mFeedbackType(0)
    , mIsDefault(FALSE)
    , mNotificationTimeout(0)
    , mIsActive(FALSE)
{}

ECode CAccessibilityServiceConnection::constructor(
    /* [in] */ IAccessibilityManager* manager,
    /* [in] */ IComponentName* componentName)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Binds to the accessibility service.
 */
void CAccessibilityServiceConnection::Bind()
{}

/**
 * Unbinds form the accessibility service and removes it from the data
 * structures for service management.
 */
void CAccessibilityServiceConnection::Unbind()
{}

/**
 * Returns if the service is configured i.e. at least event types of interest
 * and feedback type must be set.
 *
 * @return True if the service is configured, false otherwise.
 */
Boolean CAccessibilityServiceConnection::IsConfigured()
{
    return FALSE;
}

ECode CAccessibilityServiceConnection::SetServiceInfo(
    /* [in] */ IAccessibilityServiceInfo* info)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccessibilityServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccessibilityServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return E_NOT_IMPLEMENTED;
}
