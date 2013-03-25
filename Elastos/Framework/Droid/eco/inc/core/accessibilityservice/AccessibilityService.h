
#ifndef  __ACCESSIBILITYSERVICE_H__
#define  __ACCESSIBILITYSERVICE_H__

#include "Service.h"
#include <elastos/AutoPtr.h>

class AccessibilityService : public Service
{
protected:
    /**
     * This method is a part of the {@link AccessibilityService} lifecycle and is
     * called after the system has successfully bound to the service. If is
     * convenient to use this method for setting the {@link AccessibilityServiceInfo}.
     *
     * @see AccessibilityServiceInfo
     * @see #setServiceInfo(AccessibilityServiceInfo)
     */
    virtual CARAPI OnServiceConnected();

    /**
     * Sets the {@link AccessibilityServiceInfo} that describes this service.
     * <p>
     * Note: You can call this method any time but the info will be picked up after
     *       the system has bound to this service and when this method is called thereafter.
     *
     * @param info The info.
     */
    CARAPI SetServiceInfo(
        /* [in] */ IAccessibilityServiceInfo* info);

    CARAPI SetConnection(
        /* [in] */ IAccessibilityServiceConnection* connection);

    /**
     * Implement to return the implementation of the internal accessibility
     * service interface.  Subclasses should not override.
     */
    virtual CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

private:
    /**
     * Sets the {@link AccessibilityServiceInfo} for this service if the latter is
     * properly set and there is an {@link IAccessibilityServiceConnection} to the
     * AccessibilityManagerService.
     */
    CARAPI_(void) SendServiceInfo();

public:
    AutoPtr<IAccessibilityServiceConnection> mConnection;

private:
    static const CString TAG;

    AutoPtr<IAccessibilityServiceInfo> mInfo;
};

#endif  //__ACCESSIBILITYSERVICE_H__
