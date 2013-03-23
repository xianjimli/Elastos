
#include "view/accessibility/CAccessibilityEventHelper.h"
#include "view/accessibility/CAccessibilityEvent.h"
#include <elastos/Mutex.h>

/**
 * Returns a cached instance if such is available or a new one is
 * instantiated with type property set.
 *
 * @param eventType The event type.
 * @return An instance.
 */
ECode CAccessibilityEventHelper::Obtain(
    /* [in] */ Int32 eventType,
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    ObtainEx(event);
    (*event)->SetEventType(eventType);

    return NOERROR;
}

/**
 * Returns a cached instance if such is available or a new one is
 * instantiated.
 *
 * @return An instance.
 */
ECode CAccessibilityEventHelper::ObtainEx(
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);

    Mutex::Autolock lock(CAccessibilityEvent::sPoolLock);
    if (CAccessibilityEvent::sPool != NULL) {
        *event = CAccessibilityEvent::sPool;
        CAccessibilityEvent::sPool =
                ((CAccessibilityEvent*)CAccessibilityEvent::sPool.Get())->mNext;
        CAccessibilityEvent::sPoolSize--;
        ((CAccessibilityEvent*)(*event))->mNext = NULL;
        ((CAccessibilityEvent*)(*event))->mIsInPool = FALSE;
        return NOERROR;
    }

    return CAccessibilityEvent::New(event);
}
