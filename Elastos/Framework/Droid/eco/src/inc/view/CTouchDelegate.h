
#ifndef __CTOUCHDELEGATE_H__
#define __CTOUCHDELEGATE_H__

#include "_CTouchDelegate.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

CarClass(CTouchDelegate)
{
public:
    /**
     * The touchable region of the View extends above its actual extent.
     */
    static const Int32 ABOVE = 1;

    /**
     * The touchable region of the View extends below its actual extent.
     */
    static const Int32 BELOW = 2;

    /**
     * The touchable region of the View extends to the left of its
     * actual extent.
     */
    static const Int32 TO_LEFT = 4;

    /**
     * The touchable region of the View extends to the right of its
     * actual extent.
     */
    static const Int32 TO_RIGHT = 8;

public:
    /**
     * Constructor
     * 
     * @param bounds Bounds in local coordinates of the containing view that should be mapped to
     *        the delegate view
     * @param delegateView The view that should receive motion events
     */
    CARAPI constructor(
        /* [in] */ IRect* bounds,
        /* [in] */ IView* delegateView);

    /**
     * Will forward touch events to the delegate view if the event is within the bounds
     * specified in the constructor.
     * 
     * @param event The touch event to forward
     * @return True if the event was forwarded to the delegate, FALSE otherwise.
     */
    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

private:
    /**
     * View that should receive forwarded touch events 
     */
    AutoPtr<IView> mDelegateView;
    
    /**
     * Bounds in local coordinates of the containing view that should be mapped to the delegate
     * view. This rect is used for initial hit testing.
     */
    AutoPtr<IRect> mBounds;
    
    /**
     * mBounds inflated to include some slop. This rect is to track whether the motion events
     * should be considered to be be within the delegate view.
     */
    AutoPtr<IRect> mSlopBounds;
    
    /**
     * True if the delegate had been targeted on a down event (intersected mBounds).
     */
    Boolean mDelegateTargeted;
    
    Int32 mSlop;
};
#endif //__CTOUCHDELEGATE_H__
