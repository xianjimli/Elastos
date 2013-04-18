
#include "widget/Chronometer.h"
#include "os/SystemClock.h"
 
const String Chronometer::TAG = String("Chronometer");
const Int32 Chronometer::TICK_WHAT;

static Int32 R_Styleable_Chronometer[] = {
    0x01010105
};

static const Int32 R_Styleable_Chronometer_format = 0;

Chronometer::Chronometer()
{

}

//Chronometer::Chronometer(Context context)
//{
//    this(context, NULL, 0);
//}
//
//Chronometer::Chronometer(Context context, AttributeSet attrs) {
//    this(context, attrs, 0);
//}

Chronometer::Chronometer(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle) : TextView(context, attrs, defStyle)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
            attrs,
            ArrayOf<Int32>(R_Styleable_Chronometer, 1), defStyle, 0, (ITypedArray**)&a);

    String str;
    a->GetString(R_Styleable_Chronometer_format, &str);
    SetFormat(str);
    a->Recycle();

    Init();
}

void Chronometer::Init()
{
    mBase = SystemClock::GetElapsedRealtime();
    UpdateText(mBase);
}

ECode Chronometer::SetBase(Int64 base)
{
    mBase = base;
    DispatchChronometerTick();
    UpdateText(SystemClock::GetElapsedRealtime());

    return NOERROR;
}

Int64 Chronometer::GetBase()
{
    return mBase;
}

ECode Chronometer::SetFormat(
    /* [in] */ String format)
{
    mFormat = format;
    if (format != NULL && mFormatBuilder == NULL) {
        //mFormatBuilder = new StringBuilder(format.length() * 2);
    }

    return NOERROR;
}

String Chronometer::GetFormat()
{
    return mFormat;
}

ECode Chronometer::SetOnChronometerTickListener(
    /* [in] */ IOnChronometerTickListener* listener)
{
    mOnChronometerTickListener = listener;

    return NOERROR;
}

AutoPtr<IOnChronometerTickListener> Chronometer::GetOnChronometerTickListener()
{
    return mOnChronometerTickListener;
}

ECode Chronometer::Start()
{
    mStarted = TRUE;
    UpdateRunning();

    return NOERROR;
}

ECode Chronometer::Stop()
{
    mStarted = FALSE;
    UpdateRunning();

    return NOERROR;
}

ECode Chronometer::SetStarted(
    /* [in] */ Boolean started)
{
    mStarted = started;
    UpdateRunning();

    return NOERROR;
}

void Chronometer::OnDetachedFromWindow()
{
    TextView::OnDetachedFromWindow();
    mVisible = FALSE;
    UpdateRunning();
}

void Chronometer::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    TextView::OnWindowVisibilityChanged(visibility);
    mVisible = visibility == VISIBLE;
    UpdateRunning();
}

void Chronometer::UpdateText(
    /* [in] */ Int64 now)
{
    Int64 seconds = now - mBase;
    seconds /= 1000;
    String text;// = DateUtils.formatElapsedTime(mRecycle, seconds);

    //if (mFormat != NULL) {
        //AutoPtr<ILocale> loc;// = Locale.getDefault();
        //Boolean res;
        //if (mFormatter == NULL || (!loc->Equals(mFormatterLocale, &res), res)) {
        //    mFormatterLocale = loc;
        //    //mFormatter = new Formatter(mFormatBuilder, loc);
        //}
        //mFormatBuilder.setLength(0);
        //mFormatterArgs[0] = text;
        //try {
            //mFormatter.format(mFormat, mFormatterArgs);
            //text = mFormatBuilder.toString();
        /*} catch (IllegalFormatException ex) {
            if (!mLogged) {
                Log.w(TAG, "Illegal format string: " + mFormat);
                mLogged = TRUE;
            }
        }*/
    //}
    //SetText(text);
}

void Chronometer::UpdateRunning() 
{
    Boolean running = mVisible && mStarted;
    if (running != mRunning) {
        if (running) {
            UpdateText(SystemClock::GetElapsedRealtime());
            DispatchChronometerTick();
            //mHandler->SendMessageDelayed(Message.obtain(mHandler, TICK_WHAT), 1000);
        } else {
            //mHandler->RemoveMessages(TICK_WHAT);
        }
        mRunning = running;
    }
}

void Chronometer::DispatchChronometerTick()
{
    if (mOnChronometerTickListener != NULL) {
        mOnChronometerTickListener->OnChronometerTick((IChronometer*)this->Probe(EIID_IChronometer));
    }
}

