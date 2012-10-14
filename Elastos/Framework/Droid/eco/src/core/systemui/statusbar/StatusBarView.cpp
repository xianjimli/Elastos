
#include "systemui/statusbar/StatusBarView.h"
#include "systemui/statusbar/CFixedSizeDrawable.h"

const char* StatusBarView::TAG = "StatusBarView";
const Int32 StatusBarView::DIM_ANIM_TIME;

StatusBarView::StatusBarView()
{}

StatusBarView::StatusBarView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : FrameLayout(context, attrs)
{
}

//@Override
ECode StatusBarView::OnFinishInflate()
{
    FrameLayout::OnFinishInflate();

    //TODO: 0 is not value of R.id.notificationIcons
    //
    mNotificationIcons = IViewGroup::Probe(FindViewById(0/*R.id.notificationIcons*/).Get());
    mStatusIcons = IViewGroup::Probe(FindViewById(1/*R.id.statusIcons*/).Get());
    mDate = FindViewById(2/*R.id.date*/);

    AutoPtr<IDrawable> bg;
    mDate->GetBackground((IDrawable**)&bg);
    CFixedSizeDrawable::New(bg, (IFixedSizeDrawable**)&mBackground);
    mBackground->SetFixedBounds(0, 0, 0, 0);
    mDate->SetBackgroundDrawable(mBackground.Get());

    return NOERROR;
}

//@Override
void StatusBarView::OnAttachedToWindow()
{
    FrameLayout::OnAttachedToWindow();
    //mService->OnBarViewAttached();
}

//@Override
void StatusBarView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    FrameLayout::OnSizeChanged(w, h, oldw, oldh);
    //mService->UpdateExpandedViewPos(StatusBarService.EXPANDED_LEAVE_ALONE);
}

//@Override
void StatusBarView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    FrameLayout::OnLayout(changed, l, t, r, b);

    // put the date date view quantized to the icons
    Int32 oldDateRight;
    mDate->GetRight(&oldDateRight);
    Int32 newDateRight;

    newDateRight = GetDateSize(mNotificationIcons, oldDateRight,
            GetViewOffset(mNotificationIcons));
    if (newDateRight < 0) {
        Int32 offset = GetViewOffset(mStatusIcons);
        if (oldDateRight < offset) {
            newDateRight = oldDateRight;
        }
        else {
            newDateRight = GetDateSize(mStatusIcons, oldDateRight, offset);
            if (newDateRight < 0) {
                newDateRight = r;
            }
        }
    }
    Int32 max = r - GetPaddingRight();
    if (newDateRight > max) {
        newDateRight = max;
    }

    Int32 left, top, bottom;
    mDate->GetLeft(&left);
    mDate->GetTop(&top);
    mDate->GetBottom(&bottom);
    mDate->Layout(left, top, newDateRight, bottom);
    mBackground->SetFixedBounds(-left, -top, (r-l), (b-t));
}

/**
 * Gets the left position of v in this view.  Throws if v is not
 * a child of this.
 */
Int32 StatusBarView::GetViewOffset(
    /* [in] */ IView* v)
{
    Int32 offset = 0;
    while (v != (IView*)this->Probe(EIID_IView)) {
        Int32 left;
        v->GetLeft(&left);
        offset += left;
        AutoPtr<IViewParent> p;
        v->GetParent((IViewParent**)&p);
        if (p != NULL && IView::Probe(p)) {
            v = IView::Probe(p);
        }
        else {
            //throw new RuntimeException(v + " is not a child of " + this);
            return E_RUNTIME_EXCEPTION;
        }
    }
    return offset;
}

Int32 StatusBarView::GetDateSize(
    /* [in] */ IViewGroup* g,
    /* [in] */ Int32 w,
    /* [in] */ Int32 offset)
{
    Int32 N;
    g->GetChildCount(&N);
    for (Int32 i=0; i<N; i++) {
        AutoPtr<IView> v;
        g->GetChildAt(i, (IView**)&v);
        Int32 l;
        v->GetLeft(&l);
        l += offset;
        Int32 r;
        v->GetRight(&r);
        r += offset;
        if (w >= l && w <= r) {
            return r;
        }
    }
    return -1;
}

/**
 * Ensure that, if there is no target under us to receive the touch,
 * that we process it ourself.  This makes sure that onInterceptTouchEvent()
 * is always called for the entire gesture.
 */
//@Override
Boolean StatusBarView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    event->GetAction(&action);
    if (action != MotionEvent_ACTION_DOWN) {
        //mService->InterceptTouchEvent(event);
    }
    return TRUE;
}

//@Override
Boolean StatusBarView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Boolean res = FALSE;
    //mService->InterceptTouchEvent(event)
    return res ? TRUE : FrameLayout::OnInterceptTouchEvent(event);
}
