
#ifndef __CEVENTLISTENERWRAPPER_H__
#define __CEVENTLISTENERWRAPPER_H__

#include "_CEventListenerWrapper.h"
#include <elastos/AutoPtr.h>

CarClass(CEventListenerWrapper)
{
public:
    CEventListenerWrapper();

    CARAPI SetConnection(
        /* [in] */ IAccessibilityServiceConnection* connection);

    CARAPI OnAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInterrupt();

    CARAPI constructor(
        /* [in] */ IAccessibilityService* context);

private:
    CARAPI_(void) HandleDoSetConnection(
        /* [in] */ IAccessibilityServiceConnection* connection);

    CARAPI_(void) HandleDoOnInterrupt();

    CARAPI_(void) HandleDoOnAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

private:
    //private final HandlerCaller mCaller;
    AutoPtr<IAccessibilityService> mTarget;
    AutoPtr<IApartment> mApartment;
};

#endif //__CEVENTLISTENERWRAPPER_H__
