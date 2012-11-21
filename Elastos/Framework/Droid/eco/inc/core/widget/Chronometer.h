
#ifndef __CHRONOMETER_H__
#define __CHRONOMETER_H__

#include "widget/TextView.h"

/**
 * Class that implements a simple timer.
 * <p>
 * You can give it a start time in the {@link SystemClock#elapsedRealtime} timebase,
 * and it counts up from that, or if you don't give it a base time, it will use the
 * time at which you call {@link #start}.  By default it will display the current
 * timer value in the form "MM:SS" or "H:MM:SS", or you can use {@link #setFormat}
 * to format the timer value into an arbitrary string.
 *
 * @attr ref android.R.styleable#Chronometer_format
 */
class Chronometer : public TextView
{
public:
    Chronometer();
    /**
     * Initialize this Chronometer object.
     * Sets the base to the current time.
     */
    //Chronometer(
    //    /* [in] */ IContext* context);

    ///**
    // * Initialize with standard view layout information.
    // * Sets the base to the current time.
    // */
    //Chronometer(
    //    /* [in] */ IContext* context, 
    //    /* [in] */ IAttributeSet* attrs);

    /**
     * Initialize with standard view layout information and style.
     * Sets the base to the current time.
     */
    Chronometer(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs = NULL, 
        /* [in] */ Int32 defStyle = 0);

    /**
     * Set the time that the count-up timer is in reference to.
     *
     * @param base Use the {@link SystemClock#elapsedRealtime} time base.
     */
    virtual CARAPI SetBase(
        /* [in] */ Int64 base);

    /**
     * Return the base time as set through {@link #setBase}.
     */
    virtual CARAPI_(Int64) GetBase();

    /**
     * Sets the format string used for display.  The Chronometer will display
     * this string, with the first "%s" replaced by the current timer value in
     * "MM:SS" or "H:MM:SS" form.
     *
     * If the format string is null, or if you never call setFormat(), the
     * Chronometer will simply display the timer value in "MM:SS" or "H:MM:SS"
     * form.
     *
     * @param format the format string.
     */
    virtual CARAPI SetFormat(
        /* [in] */ String format);

    /**
     * Returns the current format string as set through {@link #setFormat}.
     */
    virtual CARAPI_(String) GetFormat();

    /**
     * Sets the listener to be called when the chronometer changes.
     * 
     * @param listener The listener.
     */
    virtual CARAPI SetOnChronometerTickListener(
        /* [in] */ IOnChronometerTickListener* listener);

    /**
     * @return The listener (may be null) that is listening for chronometer change
     *         events.
     */
    virtual CARAPI_(AutoPtr<IOnChronometerTickListener>) GetOnChronometerTickListener();

    /**
     * Start counting up.  This does not affect the base as set from {@link #setBase}, just
     * the view display.
     * 
     * Chronometer works by regularly scheduling messages to the handler, even when the 
     * Widget is not visible.  To make sure resource leaks do not occur, the user should 
     * make sure that each start() call has a reciprocal call to {@link #stop}. 
     */
    virtual CARAPI Start();

    /**
     * Stop counting up.  This does not affect the base as set from {@link #setBase}, just
     * the view display.
     * 
     * This stops the messages to the handler, effectively releasing resources that would
     * be held as the chronometer is running, via {@link #start}. 
     */
    virtual CARAPI Stop();

    /**
     * The same as calling {@link #start} or {@link #stop}.
     * @hide pending API council approval
     */
    virtual CARAPI SetStarted(
        /* [in] */ Boolean started);

    virtual CARAPI_(void) DispatchChronometerTick();

protected:
    virtual CARAPI_(void) OnDetachedFromWindow();

    
    virtual CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

private:
    CARAPI_(void) Init();

    CARAPI_(void) UpdateText(Int64 now);

    CARAPI_(void) UpdateRunning();

private:
    static const String TAG;

    Int64 mBase;
    Boolean mVisible;
    Boolean mStarted;
    Boolean mRunning;
    Boolean mLogged;
    String mFormat;
    //Formatter mFormatter;
    AutoPtr<ILocale> mFormatterLocale;
    ArrayOf<AutoPtr<IInterface> >* mFormatterArgs;// = new Object[1];
    StringBuffer mFormatBuilder;
    AutoPtr<IOnChronometerTickListener> mOnChronometerTickListener;
    StringBuf_<8> mRecycle;// = new StringBuilder(8);

    static const Int32 TICK_WHAT = 2;

    AutoPtr<IHandler> mHandler;// = new Handler();
};

#endif
