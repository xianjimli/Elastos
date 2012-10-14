
#ifndef __CMOTIONEVENTHELPER_H__
#define __CMOTIONEVENTHELPER_H__

#include "_CMotionEventHelper.h"

CarClass(CMotionEventHelper)
{
public:
    /**
     * Create a new MotionEvent, copying from an existing one.
     */
    CARAPI Obtain(
        /* [in] */ IMotionEvent* o,
        /* [out] */ IMotionEvent** event);
};

#endif //__CMOTIONEVENTHELPER_H__
