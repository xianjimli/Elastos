
#include "graphics/CRectF.h"
#include "graphics/CRect.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

ECode CRectF::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    *isEmpty = mLeft >= mRight || mTop >= mBottom;
    return NOERROR;
}

ECode CRectF::Width(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mRight - mLeft;
    return NOERROR;
}

ECode CRectF::Height(
    /* [out] */ Float* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mBottom - mTop;
    return NOERROR;
}

ECode CRectF::CenterX(
    /* [out] */ Float* x)
{
    VALIDATE_NOT_NULL(x);
    *x = (mLeft + mRight) * 0.5f;
    return NOERROR;
}

ECode CRectF::CenterY(
    /* [out] */ Float* y)
{
    VALIDATE_NOT_NULL(y);
    *y = (mTop + mBottom) * 0.5f;
    return NOERROR;
}

ECode CRectF::SetEmpty()
{
    mLeft = mRight = mTop = mBottom = 0;
    return NOERROR;
}

ECode CRectF::Set(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom)
{
    this->mLeft = left;
    this->mTop = top;
    this->mRight = right;
    this->mBottom = bottom;
    return NOERROR;
}

ECode CRectF::SetEx(
    /* [in] */ IRectF* src)
{
    VALIDATE_NOT_NULL(src);
    CRectF* csrc = (CRectF*)src;
    this->mLeft = csrc->mLeft;
    this->mTop = csrc->mTop;
    this->mRight = csrc->mRight;
    this->mBottom = csrc->mBottom;
    return NOERROR;
}

ECode CRectF::SetEx2(
    /* [in] */ IRect* src)
{
    VALIDATE_NOT_NULL(src);
    CRect* csrc = (CRect*)src;
    this->mLeft = csrc->mLeft;
    this->mTop = csrc->mTop;
    this->mRight = csrc->mRight;
    this->mBottom = csrc->mBottom;
    return NOERROR;
}

ECode CRectF::Offset(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    mLeft    += dx;
    mTop     += dy;
    mRight   += dx;
    mBottom  += dy;
    return NOERROR;
}

ECode CRectF::OffsetTo(
    /* [in] */ Float newLeft,
    /* [in] */ Float newTop)
{
    mRight += newLeft - mLeft;
    mBottom += newTop - mTop;
    mLeft = newLeft;
    mTop = newTop;
    return NOERROR;
}

ECode CRectF::Inset(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    mLeft    += dx;
    mTop     += dy;
    mRight   -= dx;
    mBottom  -= dy;
    return NOERROR;
}

ECode CRectF::Contains(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ Boolean* isContained)
{
    VALIDATE_NOT_NULL(isContained);
    *isContained =  mLeft < mRight && mTop < mBottom  // check for empty first
        && x >= mLeft && x < mRight && y >= mTop && y < mBottom;
    return NOERROR;
}

ECode CRectF::ContainsEx(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isContained)
{
    VALIDATE_NOT_NULL(isContained);
        // check for empty first
    *isContained = this->mLeft < this->mRight && this->mTop < this->mBottom
            // now check for containment
            && this->mLeft <= left && this->mTop <= top
            && this->mRight >= right && this->mBottom >= bottom;
    return NOERROR;
}

ECode CRectF::ContainsEx2(
    /* [in] */ IRectF* r,
    /* [out] */ Boolean* isContained)
{
    VALIDATE_NOT_NULL(r);
    VALIDATE_NOT_NULL(isContained);
    CRectF* cr = (CRectF*)r;
            // check for empty first
    *isContained = this->mLeft < this->mRight && this->mTop < this->mBottom
            // now check for containment
            && this->mLeft <= cr->mLeft && this->mTop <= cr->mTop
            && this->mRight >= cr->mRight && this->mBottom >= cr->mBottom;
    return NOERROR;
}

ECode CRectF::Intersect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isIntersected)
{
    VALIDATE_NOT_NULL(isIntersected);
    if (this->mLeft < right && left < this->mRight
            && this->mTop < bottom && top < this->mBottom) {
        if (this->mLeft < left) {
            this->mLeft = left;
        }
        if (this->mTop < top) {
            this->mTop = top;
        }
        if (this->mRight > right) {
            this->mRight = right;
        }
        if (this->mBottom > bottom) {
            this->mBottom = bottom;
        }
        *isIntersected = TRUE;
        return NOERROR;
    }
    *isIntersected = FALSE;
    return NOERROR;
}

ECode CRectF::IntersectEx(
    /* [in] */ IRectF* r,
    /* [out] */ Boolean* isIntersected)
{
    VALIDATE_NOT_NULL(r && isIntersected);
    CRectF* cr = (CRectF*)r;
    return Intersect(cr->mLeft, cr->mTop, cr->mRight, cr->mBottom, isIntersected);
}

ECode CRectF::SetIntersect(
    /* [in] */ IRectF* a,
    /* [in] */ IRectF* b,
    /* [out] */ Boolean* isIntersected)
{
    VALIDATE_NOT_NULL(a && b && isIntersected);
    CRectF* ca = (CRectF*)a;
    CRectF* cb = (CRectF*)b;
    if (ca->mLeft < cb->mRight && cb->mLeft < ca->mRight
            && ca->mTop < cb->mBottom && cb->mTop < ca->mBottom) {
        mLeft = Math::Max(ca->mLeft, cb->mLeft);
        mTop = Math::Max(ca->mTop, cb->mTop);
        mRight = Math::Min(ca->mRight, cb->mRight);
        mBottom = Math::Min(ca->mBottom, cb->mBottom);
        *isIntersected = TRUE;
        return NOERROR;
    }
    *isIntersected = FALSE;
    return NOERROR;
}

ECode CRectF::Intersects(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isIntersected)
{
    VALIDATE_NOT_NULL(isIntersected);
    *isIntersected = this->mLeft < right && left < this->mRight
        && this->mTop < bottom && top < this->mBottom;
    return NOERROR;
}

ECode CRectF::Round(
    /* [in] */ IRect* dst)
{
    VALIDATE_NOT_NULL(dst);
    dst->Set(Math::Round(mLeft), Math::Round(mTop),
             Math::Round(mRight), Math::Round(mBottom));
    return NOERROR;
}

ECode CRectF::RoundOut(
    /* [in] */ IRect* dst)
{
    VALIDATE_NOT_NULL(dst);
    dst->Set((Int32) Math::Floor(mLeft), (Int32) Math::Floor(mTop),
             (Int32) Math::Ceil(mRight), (Int32) Math::Ceil(mBottom));
    return NOERROR;
}

ECode CRectF::Union(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom)
{
    if ((left < right) && (top < bottom)) {
        if ((this->mLeft < this->mRight) && (this->mTop < this->mBottom)) {
            if (this->mLeft > left)
                this->mLeft = left;
            if (this->mTop > top)
                this->mTop = top;
            if (this->mRight < right)
                this->mRight = right;
            if (this->mBottom < bottom)
                this->mBottom = bottom;
        } else {
            this->mLeft = left;
            this->mTop = top;
            this->mRight = right;
            this->mBottom = bottom;
        }
    }
    return NOERROR;
}

ECode CRectF::UnionEx(
    /* [in] */ IRectF* r)
{
    VALIDATE_NOT_NULL(r);
    CRectF* cr = (CRectF*)r;
    return Union(cr->mLeft, cr->mTop, cr->mRight, cr->mBottom);
}

ECode CRectF::UnionEx2(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (x < mLeft) {
        mLeft = x;
    } else if (x > mRight) {
        mRight = x;
    }
    if (y < mTop) {
        mTop = y;
    } else if (y > mBottom) {
        mBottom = y;
    }
    return NOERROR;
}

ECode CRectF::Sort()
{
    if (mLeft > mRight) {
        Float temp = mLeft;
        mLeft = mRight;
        mRight = temp;
    }
    if (mTop > mBottom) {
        Float temp = mTop;
        mTop = mBottom;
        mBottom = temp;
    }
    return NOERROR;
}

ECode CRectF::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    source->ReadFloat(&mLeft);
    source->ReadFloat(&mTop);
    source->ReadFloat(&mRight);
    source->ReadFloat(&mBottom);
    return NOERROR;
}

ECode CRectF::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    dest->WriteFloat(mLeft);
    dest->WriteFloat(mTop);
    dest->WriteFloat(mRight);
    dest->WriteFloat(mBottom);
    return NOERROR;
}

ECode CRectF::constructor()
{
    return NOERROR;
}

ECode CRectF::constructor(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom)
{
    this->mLeft = left;
    this->mTop = top;
    this->mRight = right;
    this->mBottom = bottom;
    return NOERROR;
}

ECode CRectF::constructor(
    /* [in] */ IRectF* rf)
{
    VALIDATE_NOT_NULL(rf);
    CRectF* crf = (CRectF*)rf;
    mLeft = crf->mLeft;
    mTop = crf->mTop;
    mRight = crf->mRight;
    mBottom = crf->mBottom;
    return NOERROR;
}

ECode CRectF::constructor(
    /* [in] */ IRect* r)
{
    VALIDATE_NOT_NULL(r);
    CRect* cr = (CRect*)r;
    mLeft = cr->mLeft;
    mTop = cr->mTop;
    mRight = cr->mRight;
    mBottom = cr->mBottom;
    return NOERROR;
}

Float CRectF::GetWidth()
{
    return mRight - mLeft;
}

Float CRectF::GetHeight()
{
    return mBottom - mTop;
}
