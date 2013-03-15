
#ifndef __CCANDIDATEVIEW_H__
#define __CCANDIDATEVIEW_H__

#include "_CCandidateView.h"
#include "view/ViewMacro.h"
#include "view/View.h"
#include "view/GestureDetector.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


class CandidateView: public View
{
protected:
    class SelfSimpleOnGestureListener
        : public ElRefBase
        , public GestureDetector::SimpleOnGestureListener
    {
    public:
        SelfSimpleOnGestureListener(
            /* [in] */ CandidateView* host);

        ~SelfSimpleOnGestureListener();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        Boolean OnScroll(
            /* [in] */ IMotionEvent* e1,
            /* [in] */ IMotionEvent* e2,
            /* [in] */ Float distanceX,
            /* [in] */ Float distanceY);
    private:
        CandidateView* mHost;
    };

public:
    CandidateView();

    virtual ~CandidateView();

    /**
     * A connection back to the service to communicate with the text field
     * @param listener
     */
    CARAPI SetService(
        /* [in] */ ISoftKeyboard* listener);

    CARAPI ComputeHorizontalScrollRange(
        /* [out] */ Int32* range);

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    CARAPI SetSuggestions(
        /* [in] */ ArrayOf<ICharSequence*>* suggestions,
        /* [in] */ Boolean completions,
        /* [in] */ Boolean typedWordValid);

    CARAPI Clear();

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

protected:
    CARAPI Init(
        /* [in] */ IContext* ctx);

    /**
     * For flick through from keyboard, call this method with the x coordinate of the flick
     * gesture.
     * @param x
     */
    CARAPI_(void) TakeSuggestionAt(
        /* [in] */ Float x);

    CARAPI_(void) ScrollToTarget();

    CARAPI_(void) RemoveHighlight();

protected:
    static const Int32 OUT_OF_BOUNDS = -1;
    static const Int32 MAX_SUGGESTIONS = 32;
    static const Int32 SCROLL_PIXELS = 20;
    static const Int32 X_GAP = 10;
    static ArrayOf< AutoPtr<ICharSequence> >* EMPTY_LIST;

    AutoPtr<ISoftKeyboard> mService;
    ArrayOf< AutoPtr<ICharSequence> >* mSuggestions;
    Int32 mSelectedIndex;
    Int32 mTouchX;
    AutoPtr<IDrawable> mSelectionHighlight;
    Boolean mTypedWordValid;

    AutoPtr<IRect> mBgPadding;

    ArrayOf<Int32>* mWordWidth;
    ArrayOf<Int32>* mWordX;

    Int32 mColorNormal;
    Int32 mColorRecommended;
    Int32 mColorOther;
    Int32 mVerticalPadding;
    AutoPtr<IPaint> mPaint;
    Boolean mScrolled;
    Int32 mTargetScrollX;

    Int32 mTotalWidth;
    GestureDetector* mGestureDetector;
};

CarClass(CCandidateView), public CandidateView
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetService(
        /* [in] */ ISoftKeyboard* listener);

    CARAPI SetSuggestions(
        /* [in] */ ArrayOf<ICharSequence*>* suggestions,
        /* [in] */ Boolean completions,
        /* [in] */ Boolean typedWordValid);

    CARAPI Clear();
};

#endif // __CCANDIDATEVIEW_H__
