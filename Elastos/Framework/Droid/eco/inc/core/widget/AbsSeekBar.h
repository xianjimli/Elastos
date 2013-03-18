
#ifndef __ABSSEEKBAR_H__
#define __ABSSEEKBAR_H__

#include "widget/ProgressBar.h"

class AbsSeekBar : public ProgressBar
{
public:
    AbsSeekBar();

    AbsSeekBar(
        /* [in] */ IContext* context);

    AbsSeekBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AbsSeekBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
     * Sets the thumb that will be drawn at the end of the progress meter within the SeekBar.
     * <p>
     * If the thumb is a valid drawable (i.e. not null), half its width will be
     * used as the new thumb offset (@see #setThumbOffset(Int32)).
     *
     * @param thumb Drawable representing the thumb
     */
    virtual CARAPI SetThumb(
        /* [in] */ IDrawable* thumb);

    /**
     * @see #setThumbOffset(Int32)
     */
    virtual CARAPI_(Int32) GetThumbOffset();

    /**
     * Sets the thumb offset that allows the thumb to extend out of the range of
     * the track.
     *
     * @param thumbOffset The offset amount in pixels.
     */
    virtual CARAPI SetThumbOffset(
        /* [in] */ Int32 thumbOffset);

    /**
     * Sets the amount of progress changed via the arrow keys.
     *
     * @param increment The amount to increment or decrement when the user
     *            presses the arrow keys.
     */
    virtual CARAPI SetKeyProgressIncrement(
        /* [in] */ Int32 increment);

    /**
     * Returns the amount of progress changed via the arrow keys.
     * <p>
     * By default, this will be a value that is derived from the max progress.
     *
     * @return The amount to increment or decrement when the user presses the
     *         arrow keys. This will be positive.
     */
    virtual CARAPI_(Int32) GetKeyProgressIncrement();

    virtual CARAPI SetMax(
        /* [in] */ Int32 max);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);


protected:
    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    virtual CARAPI DrawableStateChanged();

    virtual CARAPI_(void) OnProgressRefresh(
        /* [in] */ Float scale,
        /* [in] */ Boolean fromUser);

    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //java package access permission
    /**
     * This is called when the user has started touching this widget.
     */
    virtual CARAPI_(void) OnStartTrackingTouch();

    /**
     * This is called when the user either releases his touch or the touch is
     * canceled.
     */
    virtual CARAPI_(void) OnStopTrackingTouch();

    /**
     * Called when the user changes the seekbar's progress by using a key event.
     */
    virtual CARAPI_(void) OnKeyChange();

private:
    CARAPI_(void) Init();

    /**
     * @param gap If set to {@link Integer#MIN_VALUE}, this will be ignored and
     */
    virtual CARAPI_(void) SetThumbPos(
        /* [in] */ Int32 w,
        /* [in] */ IDrawable* thumb,
        /* [in] */ Float scale,
        /* [in] */ Int32 gap);

    virtual CARAPI_(void) TrackTouchEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * Tries to claim the user's drag motion, and requests disallowing any
     * ancestors from stealing events in the drag.
     */
    virtual CARAPI_(void) AttemptClaimDrag();

private:

    AutoPtr<IDrawable> mThumb;

    Int32 mThumbOffset;

    /**
     * On touch, this offset plus the scaled value from the position of the
     * touch will form the progress value. Usually 0.
     */
    Float mTouchProgressOffset;

    /**
     * Whether this is user seekable.
     */
    Boolean mIsUserSeekable;

    /**
     * On key presses (right or left), the amount to increment/decrement the
     * progress.
     */
    Int32 mKeyProgressIncrement;

    static const Int32 NO_ALPHA = 0xFF;
    Float mDisabledAlpha;
};

#endif
