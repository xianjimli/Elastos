
#include "widget/AnalogClock.h"
#include <elastos/Math.h>
using namespace Elastos::Core;

#include "content/CIntentFilter.h"

static const Int32 R_Styleable_AnalogClock_dial = 0;
static const Int32 R_Styleable_AnalogClock_hand_hour = 1;
static const Int32 R_Styleable_AnalogClock_hand_minute = 2;

static const Int32 R_Drawable_Clock_dial = 0x01080147;
static const Int32 R_Drawable_Clock_hand_hour=0x01080148;
static const Int32 R_Drawable_Clock_hand_minute=0x01080149;

static Int32 R_Styleable_AnalogClock[] = {
    0x01010102, 0x01010103, 0x01010104
};

AnalogClock::AnalogClock()
{

}

AnalogClock::AnalogClock(
    /* [in] */ IContext* context) : View(context, NULL) 
{

}

AnalogClock::AnalogClock(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs) : View(context, attrs, 0)
{

}

AnalogClock::AnalogClock(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) : View(context, attrs, defStyle)
{
    AutoPtr<IResources> r;
    mContext->GetResources((IResources**)&r);
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
        attrs, ArrayOf<Int32>(R_Styleable_AnalogClock, 3), defStyle, 0, (ITypedArray**)&a);

    a->GetDrawable(R_Styleable_AnalogClock_dial, (IDrawable**)&mDial);
    if (mDial == NULL) {
        r->GetDrawable(R_Drawable_Clock_dial, (IDrawable**)&mDial);
    }

    a->GetDrawable(R_Styleable_AnalogClock_hand_hour, (IDrawable**)&mHourHand);
    if (mHourHand == NULL) {
        r->GetDrawable(R_Drawable_Clock_hand_hour, (IDrawable**)&mHourHand);
    }

    a->GetDrawable(R_Styleable_AnalogClock_hand_minute, (IDrawable**)&mMinuteHand);
    if (mMinuteHand == NULL) {
        r->GetDrawable(R_Drawable_Clock_hand_minute, (IDrawable**)&mMinuteHand);
    }

    //mCalendar = new Time();

    mDial->GetIntrinsicWidth(&mDialWidth);
    mDial->GetIntrinsicHeight(&mDialHeight);
}

void AnalogClock::OnAttachedToWindow()
{
    View::OnAttachedToWindow();

    if (!mAttached) {
        mAttached = TRUE;
        AutoPtr<IIntentFilter> filter;
        CIntentFilter::New((IIntentFilter**)&filter);

        filter->AddAction((String)Intent_ACTION_TIME_TICK);
        filter->AddAction((String)Intent_ACTION_TIME_CHANGED);
        filter->AddAction((String)Intent_ACTION_TIMEZONE_CHANGED);

        //GetContext()->RegisterReceiver(mIntentReceiver, filter, NULL, mHandler);
    }

    // NOTE: It's safe to do these after registering the receiver since the receiver always runs
    // in the main thread, therefore the receiver can't run before this method returns.

    // The time zone may have changed while the receiver wasn't registered, so update the Time
    //mCalendar = new Time();

    // Make sure we update to the current time
    OnTimeChanged();
}

void AnalogClock::OnDetachedFromWindow() 
{
    View::OnDetachedFromWindow();
    if (mAttached) {
        //GetContext()->UnregisterReceiver(mIntentReceiver);
        mAttached = FALSE;
    }
}

void AnalogClock::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec, 
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 widthSize =  MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 heightSize =  MeasureSpec::GetSize(heightMeasureSpec);

    Float hScale = 1.0f;
    Float vScale = 1.0f;

    if (widthMode != MeasureSpec::UNSPECIFIED && widthSize < mDialWidth) {
        hScale = (Float) widthSize / (Float) mDialWidth;
    }

    if (heightMode != MeasureSpec::UNSPECIFIED && heightSize < mDialHeight) {
        vScale = (Float )heightSize / (Float) mDialHeight;
    }

    Float scale = Math::Min(hScale, vScale);

    SetMeasuredDimension(ResolveSize((Int32) (mDialWidth * scale), widthMeasureSpec),
        ResolveSize((Int32) (mDialHeight * scale), heightMeasureSpec));
}

void AnalogClock::OnSizeChanged(
    /* [in] */ Int32 w, 
    /* [in] */ Int32 h, 
    /* [in] */ Int32 oldw, 
    /* [in] */ Int32 oldh)
{
    View::OnSizeChanged(w, h, oldw, oldh);
    mChanged = TRUE;
}

void AnalogClock::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    View::OnDraw(canvas);

    Boolean changed = mChanged;
    if (changed) {
        mChanged = FALSE;
    }

    Int32 availableWidth = mRight - mLeft;
    Int32 availableHeight = mBottom - mTop;

    Int32 x = availableWidth / 2;
    Int32 y = availableHeight / 2;

    AutoPtr<IDrawable> dial = mDial;
    Int32 w;
    dial->GetIntrinsicWidth(&w);
    Int32 h;
    dial->GetIntrinsicHeight(&h);

    Boolean scaled = FALSE;

    Int32 res;
    if (availableWidth < w || availableHeight < h) {
        scaled = TRUE;
        Float scale = Math::Min((Float) availableWidth / (Float) w,
            (Float) availableHeight / (Float) h);
        canvas->Save(&res);
        canvas->ScaleEx(scale, scale, x, y);
    }

    if (changed) {
        dial->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    dial->Draw(canvas);

    canvas->Save(&res);
    canvas->RotateEx(mHour / 12.0f * 360.0f, x, y);
    AutoPtr<IDrawable> hourHand = mHourHand;
    if (changed) {
        hourHand->GetIntrinsicWidth(&w);
        hourHand->GetIntrinsicHeight(&h);
        hourHand->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    hourHand->Draw(canvas);
    canvas->Restore();

    canvas->Save(&res);
    canvas->RotateEx(mMinutes / 60.0f * 360.0f, x, y);

    AutoPtr<IDrawable> minuteHand = mMinuteHand;
    if (changed) {
        minuteHand->GetIntrinsicWidth(&w);
        minuteHand->GetIntrinsicHeight(&h);
        minuteHand->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    minuteHand->Draw(canvas);
    canvas->Restore();

    if (scaled) {
        canvas->Restore();
    }
}

void AnalogClock::OnTimeChanged()
{
    /*mCalendar.setToNow();

    Int32 hour = mCalendar.hour;
    Int32 minute = mCalendar.minute;
    Int32 second = mCalendar.second;

    mMinutes = minute + second / 60.0f;
    mHour = hour + mMinutes / 60.0f;
    mChanged = TRUE;*/
}

//private final BroadcastReceiver mIntentReceiver = new BroadcastReceiver() {
//    @Override
//        public void onReceive(Context context, Intent intent) {
//            if (intent.getAction().equals(Intent.ACTION_TIMEZONE_CHANGED)) {
//                String tz = intent.getStringExtra("time-zone");
//                mCalendar = new Time(TimeZone.getTimeZone(tz).getID());
//            }
//
//            onTimeChanged();
//
//            invalidate();
//    }
//};
