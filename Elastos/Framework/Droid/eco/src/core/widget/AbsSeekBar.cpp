
#include "widget/AbsSeekBar.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const Int32 AbsSeekBar::NO_ALPHA;

static Int32 R_Styleable_SeekBar[] = {
    0x01010142, 0x01010143
};

static const Int32 R_Styleable_SeekBar_thumb = 0; //com.android.internal.R.styleable.SeekBar_thumb
static const Int32 R_Styleable_SeekBar_thumbOffset = 1; //com.android.internal.R.styleable.SeekBar_thumbOffset

static Int32 R_Styleable_Theme[] = {
    0x01010030, 0x01010031, 0x01010032, 0x01010033,
    0x01010034, 0x01010035, 0x01010036, 0x01010037,
    0x01010038, 0x01010039, 0x0101003a, 0x0101003b,
    0x0101003c, 0x0101003d, 0x0101003e, 0x0101003f,
    0x01010040, 0x01010041, 0x01010042, 0x01010043,
    0x01010044, 0x01010045, 0x01010046, 0x01010047,
    0x01010048, 0x01010049, 0x0101004a, 0x0101004b,
    0x0101004c, 0x0101004d, 0x0101004e, 0x0101004f,
    0x01010050, 0x01010051, 0x01010052, 0x01010053,
    0x01010054, 0x01010055, 0x01010056, 0x01010057,
    0x01010058, 0x01010059, 0x0101005a, 0x0101005b,
    0x0101005c, 0x0101005d, 0x0101005e, 0x0101005f,
    0x01010060, 0x01010061, 0x01010062, 0x0101006a,
    0x0101006b, 0x0101006c, 0x0101006d, 0x0101006e,
    0x0101006f, 0x01010070, 0x01010071, 0x01010072,
    0x01010073, 0x01010074, 0x01010075, 0x01010076,
    0x01010077, 0x01010078, 0x01010079, 0x0101007a,
    0x0101007b, 0x0101007c, 0x0101007d, 0x0101007e,
    0x01010080, 0x01010081, 0x01010082, 0x01010083,
    0x01010084, 0x01010085, 0x01010086, 0x01010087,
    0x01010088, 0x01010089, 0x0101008a, 0x0101008b,
    0x0101008c, 0x0101008d, 0x0101008e, 0x0101008f,
    0x01010090, 0x01010091, 0x01010092, 0x01010093,
    0x01010094, 0x010100ae, 0x01010206, 0x01010207,
    0x01010208, 0x0101020d, 0x0101020f, 0x01010210,
    0x01010212, 0x01010213, 0x01010214, 0x01010219,
    0x0101021a, 0x0101021e, 0x0101021f, 0x01010222,
    0x0101022b, 0x01010230, 0x01010267, 0x01010287,
    0x01010288, 0x01010289, 0x0101028b, 0x01010292,
    0x010102a0, 0x010102a1, 0x010102ab, 0x010102ae,
    0x010102af, 0x010102b0, 0x010102b1, 0x010102b2,
    0x010102b3, 0x010102b6, 0x010102b9, 0x010102c5,
    0x010102c6, 0x010102c7, 0x010102c8, 0x010102cc,
    0x010102cd, 0x010102ce, 0x010102cf, 0x010102d0
};

static const Int32 R_Styleable_Theme_disabledAlpha = 3;

AbsSeekBar::AbsSeekBar()
{
    Init();
}

AbsSeekBar::AbsSeekBar(
    /* [in] */ IContext* context) : ProgressBar(context)
{
    Init();
    Init(context);
}

AbsSeekBar::AbsSeekBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) : ProgressBar(context, attrs)
{
    Init();
    Init(context, attrs);
}

AbsSeekBar::AbsSeekBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) : ProgressBar(context, attrs, defStyle)
{
    Init();
    Init(context, attrs, defStyle);
}

void AbsSeekBar::Init()
{
    mIsUserSeekable = TRUE;
    mKeyProgressIncrement = 1;
}

ECode AbsSeekBar::Init(
    /* [in] */ IContext* context)
{
    return ProgressBar::Init(context);
}

ECode AbsSeekBar::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ProgressBar::Init(context, attrs);
}

ECode AbsSeekBar::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(ProgressBar::Init(context, attrs, defStyle));

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs,
                ArrayOf<Int32>(R_Styleable_SeekBar, 2),
                defStyle, 0, (ITypedArray**)&a));
    AutoPtr<IDrawable> thumb;
    FAIL_RETURN(a->GetDrawable(R_Styleable_SeekBar_thumb, (IDrawable**)&thumb));
    FAIL_RETURN(SetThumb(thumb)); // will guess mThumbOffset if thumb != NULL...
    // ...but allow layout to override this
    Int32 thumbOffset;
    FAIL_RETURN(a->GetDimensionPixelOffset(
            R_Styleable_SeekBar_thumbOffset, GetThumbOffset(), &thumbOffset));
    FAIL_RETURN(SetThumbOffset(thumbOffset));
    FAIL_RETURN(a->Recycle());

    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs,
                ArrayOf<Int32>(R_Styleable_Theme,
                sizeof(R_Styleable_Theme) / sizeof(Int32)),
                0, 0, (ITypedArray**)&a));
    FAIL_RETURN(a->GetFloat(
            R_Styleable_Theme_disabledAlpha, 0.5f, &mDisabledAlpha));
    return a->Recycle();
}

/**
 * Sets the thumb that will be drawn at the end of the progress meter within the SeekBar.
 * <p>
 * If the thumb is a valid drawable (i.e. not NULL), half its width will be
 * used as the new thumb offset (@see #setThumbOffset(Int32)).
 *
 * @param thumb Drawable representing the thumb
 */
ECode AbsSeekBar::SetThumb(
    /* [in] */ IDrawable* thumb)
{
    if (thumb != NULL) {
        thumb->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));

        // Assuming the thumb drawable is symmetric, set the thumb offset
        // such that the thumb will hang halfway off either edge of the
        // progress bar.
        Int32 w;
        thumb->GetIntrinsicWidth(&w);
        mThumbOffset = w / 2;
    }
    mThumb = thumb;
    Invalidate();

    return NOERROR;
}

/**
 * @see #setThumbOffset(Int32)
 */
Int32 AbsSeekBar::GetThumbOffset()
{
    return mThumbOffset;
}

/**
 * Sets the thumb offset that allows the thumb to extend out of the range of
 * the track.
 *
 * @param thumbOffset The offset amount in pixels.
 */
ECode AbsSeekBar::SetThumbOffset(
    /* [in] */ Int32 thumbOffset)
{
    mThumbOffset = thumbOffset;
    Invalidate();

    return NOERROR;
}

/**
 * Sets the amount of progress changed via the arrow keys.
 *
 * @param increment The amount to increment or decrement when the user
 *            presses the arrow keys.
 */
ECode AbsSeekBar::SetKeyProgressIncrement(
    /* [in] */ Int32 increment)
{
    mKeyProgressIncrement = increment < 0 ? -increment : increment;

    return NOERROR;
}

/**
 * Returns the amount of progress changed via the arrow keys.
 * <p>
 * By default, this will be a value that is derived from the max progress.
 *
 * @return The amount to increment or decrement when the user presses the
 *         arrow keys. This will be positive.
 */
Int32 AbsSeekBar::GetKeyProgressIncrement()
{
    return mKeyProgressIncrement;
}

ECode AbsSeekBar::SetMax(
    /* [in] */ Int32 max)
{
    ProgressBar::SetMax(max);

    if ((mKeyProgressIncrement == 0) || (GetMax() / mKeyProgressIncrement > 20)) {
        // It will take the user too long to change this via keys, change it
        // to something more reasonable
        SetKeyProgressIncrement(Math::Max(1, Math::Round((Float) GetMax() / 20)));
    }

    return NOERROR;
}

Boolean AbsSeekBar::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return who == mThumb || ProgressBar::VerifyDrawable(who);
}

ECode AbsSeekBar::DrawableStateChanged()
{
    ProgressBar::DrawableStateChanged();

    AutoPtr<IDrawable> progressDrawable = GetProgressDrawable();
    if (progressDrawable != NULL) {
        progressDrawable->SetAlpha(IsEnabled() ? NO_ALPHA : (Int32) (NO_ALPHA * mDisabledAlpha));
    }

    Boolean stateful;
    mThumb->IsStateful(&stateful);

    if (mThumb != NULL && stateful) {
        ArrayOf<Int32>* state = GetDrawableState();

        Boolean res;
        mThumb->SetState(state, &res);
    }

    return NOERROR;
}

void AbsSeekBar::OnProgressRefresh(
    /* [in] */ Float scale,
    /* [in] */ Boolean fromUser)
{
    AutoPtr<IDrawable> thumb = mThumb;
    if (thumb != NULL) {
        SetThumbPos(GetWidth(), thumb, scale, Math::INT32_MIN_VALUE);
        /*
         * Since we draw translated, the drawable's bounds that it signals
         * for invalidation won't be the actual bounds we want invalidated,
         * so just invalidate this whole view.
         */
        Invalidate();
    }
}

void AbsSeekBar::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    AutoPtr<IDrawable> d = GetCurrentDrawable();
    AutoPtr<IDrawable> thumb = mThumb;

    Int32 height;
    Int32 thumbHeight = thumb == NULL ? 0 : (thumb->GetIntrinsicHeight(&height), height);
    // The max height does not incorporate padding, whereas the height
    // parameter does
    Int32 trackHeight = Math::Min(mMaxHeight, h - mPaddingTop - mPaddingBottom);

    Int32 max = GetMax();
    Float scale = max > 0 ? (Float) GetProgress() / (Float) max : 0;

    if (thumbHeight > trackHeight) {
        if (thumb != NULL) {
            SetThumbPos(w, thumb, scale, 0);
        }
        Int32 gapForCenteringTrack = (thumbHeight - trackHeight) / 2;
        if (d != NULL) {
            // Canvas will be translated by the padding, so 0,0 is where we start drawing
            d->SetBounds(0, gapForCenteringTrack,
                    w - mPaddingRight - mPaddingLeft, h - mPaddingBottom - gapForCenteringTrack
                    - mPaddingTop);
        }
    } else {
        if (d != NULL) {
            // Canvas will be translated by the padding, so 0,0 is where we start drawing
            d->SetBounds(0, 0, w - mPaddingRight - mPaddingLeft, h - mPaddingBottom
                    - mPaddingTop);
        }
        Int32 gap = (trackHeight - thumbHeight) / 2;
        if (thumb != NULL) {
            SetThumbPos(w, thumb, scale, gap);
        }
    }
}

/**
 * @param gap If set to {@link Integer#MIN_VALUE}, this will be ignored and
 */
void AbsSeekBar::SetThumbPos(
    /* [in] */ Int32 w,
    /* [in] */ IDrawable* thumb,
    /* [in] */ Float scale,
    /* [in] */ Int32 gap)
{
    Int32 available = w - mPaddingLeft - mPaddingRight;
    Int32 thumbWidth;
    thumb->GetIntrinsicWidth(&thumbWidth);
    Int32 thumbHeight;
    thumb->GetIntrinsicHeight(&thumbHeight);
    available -= thumbWidth;

    // The extra space for the thumb to move on the track
    available += mThumbOffset * 2;

    Int32 thumbPos = (Int32) (scale * available);

    Int32 topBound, bottomBound;
    if (gap == Math::INT32_MIN_VALUE) {
        AutoPtr<IRect> oldBounds;
        thumb->GetBounds((IRect**)&oldBounds);
        topBound = ((CRect*)oldBounds.Get())->mTop;
        bottomBound = ((CRect*)oldBounds.Get())->mBottom;
    } else {
        topBound = gap;
        bottomBound = gap + thumbHeight;
    }

    // Canvas will be translated, so 0,0 is where we start drawing
    thumb->SetBounds(thumbPos, topBound, thumbPos + thumbWidth, bottomBound);
}

void AbsSeekBar::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    ProgressBar::OnDraw(canvas);
    if (mThumb != NULL) {

        Int32 count;
        canvas->Save(&count);
        // Translate the padding. For the x, we need to allow the thumb to
        // draw in its extra space
        canvas->Translate(mPaddingLeft - mThumbOffset, mPaddingTop);
        mThumb->Draw(canvas);
        canvas->Restore();
    }
}

void AbsSeekBar::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AutoPtr<IDrawable> d = GetCurrentDrawable();

    Int32 h;
    Int32 thumbHeight = mThumb == NULL ? 0 : (mThumb->GetIntrinsicHeight(&h), h);
    Int32 dw = 0;
    Int32 dh = 0;
    if (d != NULL) {
        dw = Math::Max(mMinWidth, Math::Min(mMaxWidth, (d->GetIntrinsicWidth(&dw), dw)));
        dh = Math::Max(mMinHeight, Math::Min(mMaxHeight, (d->GetIntrinsicHeight(&dh), dh)));
        dh = Math::Max(thumbHeight, dh);
    }
    dw += mPaddingLeft + mPaddingRight;
    dh += mPaddingTop + mPaddingBottom;

    SetMeasuredDimension(ResolveSize(dw, widthMeasureSpec),
            ResolveSize(dh, heightMeasureSpec));
}

Boolean AbsSeekBar::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (!mIsUserSeekable || !IsEnabled()) {
        return FALSE;
    }

    Int32 action;
    switch (event->GetAction(&action), action) {
        case MotionEvent_ACTION_DOWN:
            SetPressed(TRUE);
            OnStartTrackingTouch();
            TrackTouchEvent(event);
            break;

        case MotionEvent_ACTION_MOVE:
            TrackTouchEvent(event);
            AttemptClaimDrag();
            break;

        case MotionEvent_ACTION_UP:
            TrackTouchEvent(event);
            OnStopTrackingTouch();
            SetPressed(FALSE);
            // ProgressBar doesn't know to repaint the thumb drawable
            // in its inactive state when the touch stops (because the
            // value has not apparently changed)
            Invalidate();
            break;

        case MotionEvent_ACTION_CANCEL:
            OnStopTrackingTouch();
            SetPressed(FALSE);
            Invalidate(); // see above explanation
            break;
    }
    return TRUE;
}

void AbsSeekBar::TrackTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 width = GetWidth();
    Int32 available = width - mPaddingLeft - mPaddingRight;
    Float x;
    event->GetX(&x);
    Float scale;
    Float progress = 0;
    if (x < mPaddingLeft) {
        scale = 0.0f;
    } else if (x > width - mPaddingRight) {
        scale = 1.0f;
    } else {
        scale = (Float)(x - mPaddingLeft) / (Float)available;
        progress = mTouchProgressOffset;
    }

    Int32 max = GetMax();
    progress += scale * max;

    SetProgress((Int32) progress, TRUE);
}

/**
 * Tries to claim the user's drag motion, and requests disallowing any
 * ancestors from stealing events in the drag.
 */
void AbsSeekBar::AttemptClaimDrag()
{
    if (mParent != NULL) {
        mParent->RequestDisallowInterceptTouchEvent(TRUE);
    }
}

/**
 * This is called when the user has started touching this widget.
 */
void AbsSeekBar::OnStartTrackingTouch()
{
}

/**
 * This is called when the user either releases his touch or the touch is
 * canceled.
 */
void AbsSeekBar::OnStopTrackingTouch()
{
}

/**
 * Called when the user changes the seekbar's progress by using a key event.
 */
void AbsSeekBar::OnKeyChange()
{
}

Boolean AbsSeekBar::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (IsEnabled()) {
        Int32 progress = GetProgress();
        switch (keyCode) {
            case KeyEvent_KEYCODE_DPAD_LEFT:
                if (progress <= 0) break;
                SetProgress(progress - mKeyProgressIncrement, TRUE);
                OnKeyChange();
                return TRUE;

            case KeyEvent_KEYCODE_DPAD_RIGHT:
                if (progress >= GetMax()) break;
                SetProgress(progress + mKeyProgressIncrement, TRUE);
                OnKeyChange();
                return TRUE;
        }
    }

    return ProgressBar::OnKeyDown(keyCode, event);
}

