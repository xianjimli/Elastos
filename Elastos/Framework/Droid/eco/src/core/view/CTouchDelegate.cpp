
#include "view/CTouchDelegate.h"
#include "view/View.h"

const Int32 CTouchDelegate::ABOVE;
const Int32 CTouchDelegate::BELOW;
const Int32 CTouchDelegate::TO_LEFT;
const Int32 CTouchDelegate::TO_RIGHT;

/**
 * Constructor
 *
 * @param bounds Bounds in local coordinates of the containing view that should be mapped to
 *        the delegate view
 * @param delegateView The view that should receive motion events
 */
ECode CTouchDelegate::constructor(
    /* [in] */ IRect* bounds,
    /* [in] */ IView* delegateView)
{
    mBounds = bounds;

    mSlop = 0;
    //mSlop = ViewConfiguration::Get(delegateView->GetContext())->GetScaledTouchSlop();
    assert(SUCCEEDED(CRect::New(bounds, (IRect**)&mSlopBounds)));
    mSlopBounds->Inset(-mSlop, -mSlop);
    mDelegateView = delegateView;

    return NOERROR;
}

/**
 * Will forward touch events to the delegate view if the event is within the bounds
 * specified in the constructor.
 *
 * @param event The touch event to forward
 * @return True if the event was forwarded to the delegate, FALSE otherwise.
 */
ECode CTouchDelegate::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    Float fx, fy;
    event->GetX(&fx);
    event->GetY(&fy);

    Int32 x = (Int32)fx;
    Int32 y = (Int32)fy;
    Boolean sendToDelegate = FALSE;
    Boolean hit = TRUE;
    Boolean handled = FALSE;

    Int32 action;
    event->GetAction(&action);
    switch (action) {
    case MotionEvent_ACTION_DOWN:
        {
            Boolean isContains;
            mBounds->Contains(x, y, &isContains);
            if (isContains) {
                mDelegateTargeted = TRUE;
                sendToDelegate = TRUE;
            }
        }
        break;
    case MotionEvent_ACTION_UP:
    case MotionEvent_ACTION_MOVE:
        if (mDelegateTargeted) {
            Boolean isContains;
            mSlopBounds->Contains(x, y, &isContains);
            if (!isContains) {
                hit = FALSE;
            }
        }
        break;
    case MotionEvent_ACTION_CANCEL:
        sendToDelegate = mDelegateTargeted;
        mDelegateTargeted = FALSE;
        break;
    default:
        break;
    }

    if (sendToDelegate) {
        View* delegateView = (View*)mDelegateView->Probe(EIID_View);

        if (hit) {
            // Offset event coordinates to be inside the target view
            event->SetLocation(
                delegateView->GetWidth() / 2, delegateView->GetHeight() / 2);
        }
        else {
            // Offset event coordinates to be outside the target view (in case it does
            // something like tracking pressed state)
            event->SetLocation(-(mSlop * 2), -(mSlop * 2));
        }
        handled = delegateView->DispatchTouchEvent(event);
    }

    *result = handled;

    return NOERROR;
}
