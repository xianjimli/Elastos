
#include "widget/ScrollBarDrawable.h"
#include "graphics/CRect.h"
#include "graphics/ElPixelFormat.h"
#include <elastos/Math.h>

using namespace Elastos::System;

ScrollBarDrawable::ScrollBarDrawable()
    : mRange(0)
    , mOffset(0)
    , mExtent(0)
    , mVertical(FALSE)
    , mChanged(FALSE)
    , mRangeChanged(FALSE)
    , mAlwaysDrawHorizontalTrack(FALSE)
    , mAlwaysDrawVerticalTrack(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTempBounds));
}

void ScrollBarDrawable::SetAlwaysDrawHorizontalTrack(
    /* [in] */ Boolean alwaysDrawTrack)
{
    mAlwaysDrawHorizontalTrack = alwaysDrawTrack;
}

void ScrollBarDrawable::SetAlwaysDrawVerticalTrack(
    /* [in] */ Boolean alwaysDrawTrack)
{
    mAlwaysDrawVerticalTrack = alwaysDrawTrack;
}

Boolean ScrollBarDrawable::GetAlwaysDrawHorizontalTrack()
{
    return mAlwaysDrawHorizontalTrack;
}

Boolean ScrollBarDrawable::GetAlwaysDrawVerticalTrack()
{
    return mAlwaysDrawVerticalTrack;
}

void ScrollBarDrawable::SetParameters(
    /* [in] */ Int32 range,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 extent,
    /* [in] */ Boolean vertical)
{
    if (mVertical != vertical) {
        mChanged = TRUE;
    }

    if (mRange != range || mOffset != offset || mExtent != extent) {
        mRangeChanged = TRUE;
    }

    mRange = range;
    mOffset = offset;
    mExtent = extent;
    mVertical = vertical;
}

ECode ScrollBarDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    const Boolean vertical = mVertical;
    const Int32 extent = mExtent;
    const Int32 range = mRange;

    Boolean drawTrack = TRUE;
    Boolean drawThumb = TRUE;
    if (extent <= 0 || range <= extent) {
        drawTrack = vertical ? mAlwaysDrawVerticalTrack : mAlwaysDrawHorizontalTrack;
        drawThumb = FALSE;
    }

    AutoPtr<IRect> _r = GetBounds();
    CRect* r = (CRect*)_r.Get();
    Boolean notIntersected;
    FAIL_RETURN(canvas->QuickReject(r->mLeft, r->mTop, r->mRight,
            r->mBottom, CanvasEdgeType_AA, &notIntersected));
    if (notIntersected) {
        return NOERROR;
    }
    if (drawTrack) {
        DrawTrack(canvas, _r.Get(), vertical);
    }

    if (drawThumb) {
        Int32 height, width;
        _r->GetHeight(&height);
        _r->GetWidth(&width);
        Int32 size = vertical ? height : width;
        Int32 thickness = vertical ? width : height;
        Int32 length = Math::Round((Float)size * extent / range);
        Int32 offset = Math::Round((Float)(size - length) * mOffset / (range - extent));

        // avoid the tiny thumb
        Int32 minLength = thickness * 2;
        if (length < minLength) {
            length = minLength;
        }
        // avoid the too-big thumb
        if (offset + length > size) {
            offset = size - length;
        }

        DrawThumb(canvas, r, offset, length, vertical);
    }
    return NOERROR;
}

void ScrollBarDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    Drawable::OnBoundsChange(bounds);
    mChanged = TRUE;
}

void ScrollBarDrawable::DrawTrack(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* bounds,
    /* [in] */ Boolean vertical)
{
    AutoPtr<IDrawable> track;
    if (vertical) {
        track = mVerticalTrack;
    } else {
        track = mHorizontalTrack;
    }
    if (track != NULL) {
        if (mChanged) {
            track->SetBoundsEx(bounds);
        }
        track->Draw(canvas);
    }
}

void ScrollBarDrawable::DrawThumb(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* _bounds,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean vertical)
{
    const AutoPtr<IRect> thumbRect = mTempBounds;
    const Boolean changed = mRangeChanged || mChanged;
    if (changed) {
        CRect* bounds = (CRect*)_bounds;
        if (vertical) {
            thumbRect->Set(bounds->mLeft,  bounds->mTop + offset,
                    bounds->mRight, bounds->mTop + offset + length);
        } else {
            thumbRect->Set(bounds->mLeft + offset, bounds->mTop,
                    bounds->mLeft + offset + length, bounds->mBottom);
        }
    }

    if (vertical) {
        const AutoPtr<IDrawable> thumb = mVerticalThumb;
        if (changed) thumb->SetBoundsEx(thumbRect);
        thumb->Draw(canvas);
    } else {
        const AutoPtr<IDrawable> thumb = mHorizontalThumb;
        if (changed) thumb->SetBoundsEx(thumbRect);
        thumb->Draw(canvas);
    }
}

void ScrollBarDrawable::SetVerticalThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    if (thumb != NULL) {
        mVerticalThumb = thumb;
    }
}

void ScrollBarDrawable::SetVerticalTrackDrawable(
    /* [in] */ IDrawable* track)
{
    mVerticalTrack = track;
}

void ScrollBarDrawable::SetHorizontalThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    if (thumb != NULL) {
        mHorizontalThumb = thumb;
    }
}

void ScrollBarDrawable::SetHorizontalTrackDrawable(
    /* [in] */ IDrawable* track)
{
    mHorizontalTrack = track;
}

Int32 ScrollBarDrawable::GetSize(
    /* [in] */ Boolean vertical)
{
    Int32 value;
    if (vertical) {
        if (mVerticalTrack != NULL) mVerticalTrack->GetIntrinsicWidth(&value);
        else mVerticalThumb->GetIntrinsicWidth(&value);
    } else {
        if (mHorizontalTrack != NULL) mHorizontalTrack->GetIntrinsicHeight(&value);
        else mHorizontalThumb->GetIntrinsicHeight(&value);
    }
    return value;
}

ECode ScrollBarDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    if (mVerticalTrack != NULL) {
        mVerticalTrack->SetAlpha(alpha);
    }
    mVerticalThumb->SetAlpha(alpha);
    if (mHorizontalTrack != NULL) {
        mHorizontalTrack->SetAlpha(alpha);
    }
    mHorizontalThumb->SetAlpha(alpha);
    return NOERROR;
}

ECode ScrollBarDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    if (mVerticalTrack != NULL) {
        mVerticalTrack->SetColorFilter(cf);
    }
    mVerticalThumb->SetColorFilter(cf);
    if (mHorizontalTrack != NULL) {
        mHorizontalTrack->SetColorFilter(cf);
    }
    mHorizontalThumb->SetColorFilter(cf);
    return NOERROR;
}

Int32 ScrollBarDrawable::GetOpacity()
{
    return ElPixelFormat::TRANSLUCENT;
}
