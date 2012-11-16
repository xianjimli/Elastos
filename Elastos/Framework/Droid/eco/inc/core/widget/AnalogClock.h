
#ifndef __ANALOGCLOCK_H__
#define __ANALOGCLOCK_H__

#include "view/View.h"

/**
 * This widget display an analogic clock with two hands for hours and
 * minutes.
 */
class AnalogClock : public View
{
public:
    AnalogClock();

    AnalogClock(
        /* [in] */ IContext* context);

    AnalogClock(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs);

    AnalogClock(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

protected:
    
    virtual CARAPI_(void) OnAttachedToWindow();

    
    virtual CARAPI_(void) OnDetachedFromWindow();

    
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec, 
        /* [in] */ Int32 heightMeasureSpec);

    
    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w, 
        /* [in] */ Int32 h, 
        /* [in] */ Int32 oldw, 
        /* [in] */ Int32 oldh);

    
    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

private:
    CARAPI_(void) OnTimeChanged();

private:
    AutoPtr<IBroadcastReceiver> mIntentReceiver;// = new BroadcastReceiver();

    //Time mCalendar;

    AutoPtr<IDrawable> mHourHand;
    AutoPtr<IDrawable> mMinuteHand;
    AutoPtr<IDrawable> mDial;

    Int32 mDialWidth;
    Int32 mDialHeight;

    Boolean mAttached;

    AutoPtr<IHandler> mHandler;// = new Handler();
    Float mMinutes;
    Float mHour;
    Boolean mChanged;
};

#endif
