
#ifndef __FASTSCROLLER_H__
#define __FASTSCROLLER_H__

#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

class AbsListView;
/**
 * Helper class for AbsListView to draw and control the Fast Scroll thumb
 */
class FastScroller
{
    friend class AbsListView;
private:
    // Minimum number of pages to justify showing a fast scroll thumb
    static Int32 MIN_PAGES;
    // Scroll thumb not showing
    static const Int32 STATE_NONE = 0;
    // Not implemented yet - fade-in transition
    static const Int32 STATE_ENTER = 1;
    // Scroll thumb visible and moving along with the scrollbar
    static const Int32 STATE_VISIBLE = 2;
    // Scroll thumb being dragged by user
    static const Int32 STATE_DRAGGING = 3;
    // Scroll thumb fading out due to inactivity timeout
    static const Int32 STATE_EXIT = 4;

public:
    class ScrollFade : public Runnable
    {
        friend class FastScroller;
    protected:
        static const Int32 ALPHA_MAX = 208;
        static const Int64 FADE_DURATION = 200;

    public:
        ScrollFade(
            /* [in] */ FastScroller* host);

        CARAPI Run();

    protected:
        CARAPI_(CARAPI_(void)) StartFade();

        CARAPI_(Int32) GetAlpha();

    protected:
        Int64 mStartTime;
        Int64 mFadeDuration;

    private:
        FastScroller* mHost;
    };

public:
    FastScroller(
        /* [in] */ IContext* context,
        /* [in] */ AbsListView* listView);

    CARAPI_(void) SetState(
        /* [in] */ Int32 state);

    CARAPI_(Int32) GetState();

    CARAPI_(void) Draw(
        /* [in] */ ICanvas* canvas);

protected:
    CARAPI_(void) Stop();

    CARAPI_(Boolean) IsVisible();

    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    CARAPI_(void) OnScroll(
        /* [in] */ IAbsListView* view,
        /* [in] */ Int32 firstVisibleItem,
        /* [in] */ Int32 visibleItemCount,
        /* [in] */ Int32 totalItemCount);

    CARAPI_(AutoPtr<ISectionIndexer>) GetSectionIndexer();

    CARAPI_(Vector<AutoPtr<IInterface> >&) GetSections();

    CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* me);

    CARAPI_(Boolean) IsPointInside(
        /* [in] */ Float x,
        /* [in] */ Float y);

private:
    CARAPI_(void) ResetThumbPos();

    CARAPI_(void) UseThumbDrawable(
        /* [in] */ IContext* context,
        /* [in] */ IDrawable* drawable);

    CARAPI_(void) Init(
        /* [in] */ IContext* context);

    CARAPI_(void) GetSectionsFromIndexer();

    CARAPI_(void) ScrollTo(
        /* [in] */ Float position);

    CARAPI_(void) CancelFling();

    CARAPI_(Boolean) PostDelayed(
        /* [in] */ Int64 delayMillis);

private:
    AutoPtr<IDrawable> mThumbDrawable;
    AutoPtr<IDrawable> mOverlayDrawable;

    Int32 mThumbH;
    Int32 mThumbW;
    Int32 mThumbY;

    AutoPtr<IRectF> mOverlayPos;
    Int32 mOverlaySize;

    AbsListView* mList;
    Boolean mScrollCompleted;
    Int32 mVisibleItem;
    AutoPtr<IPaint> mPaint;
    Int32 mListOffset;
    Int32 mItemCount;
    Boolean mLongList;

    Vector<AutoPtr<IInterface> > mSections;
    String mSectionText;
    Boolean mDrawOverlay;
    AutoPtr<ScrollFade> mScrollFade;

    Int32 mState;

    //Handler mHandler = new Handler();

    AutoPtr<IBaseAdapter> mListAdapter;

    AutoPtr<ISectionIndexer> mSectionIndexer;

    Boolean mChangedBounds;

    AutoPtr<IApartment> mApartment;
};
#endif //__FASTSCROLLER_H__
