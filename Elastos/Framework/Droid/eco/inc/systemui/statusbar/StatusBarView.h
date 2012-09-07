
#ifndef __STATUSBARVIEW_H__
#define __STATUSBARVIEW_H__

#include "widget/FrameLayout.h"

class StatusBarView : public FrameLayout
{
protected:
    StatusBarView();

    StatusBarView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

public:
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* event);

protected:
    CARAPI OnFinishInflate();

    CARAPI_(void) OnAttachedToWindow();

    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

private:
    CARAPI_(Int32) GetViewOffset(
        /* [in] */ IView* v);

    CARAPI_(Int32) GetDateSize(
        /* [in] */ IViewGroup* g,
        /* [in] */ Int32 w,
        /* [in] */ Int32 offset);

private:
    static const char* TAG;

protected:
    static const Int32 DIM_ANIM_TIME = 400;

    AutoPtr<IStatusBarService> mService;
    Boolean mTracking;
    Int32 mStartX;
    Int32 mStartY;
    AutoPtr<IViewGroup> mNotificationIcons;
    AutoPtr<IViewGroup> mStatusIcons;
    AutoPtr<IView> mDate;
    AutoPtr<IFixedSizeDrawable> mBackground;
};
#endif //__STATUSBARVIEW_H__
