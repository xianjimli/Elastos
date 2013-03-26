
#include "ext/frameworkdef.h"
#include "view/CMotionEventHelper.h"
#include "view/CMotionEvent.h"

/**
 * Create a new MotionEvent, copying from an existing one.
 */
ECode CMotionEventHelper::Obtain(
    /* [in] */ IMotionEvent* _o,
    /* [out] */ IMotionEvent** _event)
{
    CMotionEvent* o = (CMotionEvent*)_o;
    CMotionEvent* event;
    ECode ec = CMotionEvent::Obtain(o, &event);
    if (FAILED(ec)) {
        return ec;
    }

    *_event = (IMotionEvent*)event;
    return NOERROR;
}
