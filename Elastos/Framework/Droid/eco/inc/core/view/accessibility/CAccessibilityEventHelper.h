
#ifndef __CACCESSIBILITYEVENTHELPER_H__
#define __CACCESSIBILITYEVENTHELPER_H__

#include "_CAccessibilityEventHelper.h"

CarClass(CAccessibilityEventHelper)
{
public:
    /**
     * Returns a cached instance if such is available or a new one is
     * instantiated with type property set.
     *
     * @param eventType The event type.
     * @return An instance.
     */
    CARAPI Obtain(
        /* [in] */ Int32 eventType,
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Returns a cached instance if such is available or a new one is
     * instantiated.
     *
     * @return An instance.
     */
    CARAPI ObtainEx(
        /* [out] */ IAccessibilityEvent** event);
};

#endif //__CACCESSIBILITYEVENTHELPER_H__
