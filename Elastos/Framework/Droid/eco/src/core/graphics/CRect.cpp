
#include "graphics/CRect.h"
#include "ext/frameworkext.h"

ECode CRect::constructor()
{
    mLeft = mRight = mTop = mBottom = 0;

    return NOERROR;
}

ECode CRect::constructor(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mLeft = left;
    mTop = top;
    mRight = right;
    mBottom = bottom;

    return NOERROR;
}

ECode CRect::constructor(
    /* [in] */ IRect* r)
{
    if (!r) {
        return E_INVALID_ARGUMENT;
    }

    CRect* temp = (CRect*)r;

    mLeft = temp->mLeft;
    mTop = temp->mTop;
    mRight = temp->mRight;
    mBottom = temp->mBottom;

    return NOERROR;
}

ECode CRect::Equals(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    if (!result) {
        return E_INVALID_ARGUMENT;
    }

    *result = Equals(r);

    return NOERROR;
}

Boolean CRect::Equals(
    /* [in] */ IRect* r)
{
    if (r != NULL) {
        CRect* temp = (CRect*)r;
        return mLeft == temp->mLeft && mTop == temp->mTop
            && mRight == temp->mRight && mBottom == temp->mBottom;
    }

    return FALSE;
}

ECode CRect::IsEmpty(
    /* [out] */ Boolean* result)
{
    if (!result) {
        return E_INVALID_ARGUMENT;
    }

    *result = IsEmpty();

    return NOERROR;
}

Boolean CRect::IsEmpty()
{
    return mLeft >= mRight || mTop >= mBottom;
}

ECode CRect::GetWidth(
    /* [out] */ Int32* width)
{
    if (!width) {
        return E_INVALID_ARGUMENT;
    }

    *width = mRight - mLeft;

    return NOERROR;
}

Int32 CRect::GetWidth()
{
    return mRight - mLeft;
}

ECode CRect::GetHeight(
    /* [out] */ Int32* height)
{
    if (!height) {
        return E_INVALID_ARGUMENT;
    }

    *height = mBottom - mTop;

    return NOERROR;
}

Int32 CRect::GetHeight()
{
    return mBottom - mTop;
}

ECode CRect::SetEmpty()
{
    mLeft = mRight = mTop = mBottom = 0;

    return NOERROR;
}

ECode CRect::Set(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mLeft = left;
    mTop = top;
    mRight = right;
    mBottom = bottom;

    return NOERROR;
}

ECode CRect::SetEx(
    /* [in] */ IRect* src)
{
    if (!src) {
        return E_INVALID_ARGUMENT;
    }

    CRect* temp = (CRect*)src;

    mLeft = temp->mLeft;
    mTop = temp->mTop;
    mRight = temp->mRight;
    mBottom = temp->mBottom;

    return NOERROR;
}

ECode CRect::Offset(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    mLeft += dx;
    mTop += dy;
    mRight += dx;
    mBottom += dy;

    return NOERROR;
}

ECode CRect::Inset(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    mLeft += dx;
    mTop += dy;
    mRight -= dx;
    mBottom -= dy;

    return NOERROR;
}

/**
 * Returns true if (x,y) is inside the rectangle. The left and top are
 * considered to be inside, while the right and bottom are not. This means
 * that for a x,y to be contained: left <= x < right and top <= y < bottom.
 * An empty rectangle never contains any point.
 *
 * @param x The X coordinate of the point being tested for containment
 * @param y The Y coordinate of the point being tested for containment
 * @return true iff (x,y) are contained by the rectangle, where containment
 *              means left <= x < right and top <= y < bottom
 */
ECode CRect::Contains(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mLeft < mRight && mTop < mBottom  // check for empty first
           && x >= mLeft && x < mRight && y >= mTop && y < mBottom;

    return NOERROR;
}

/**
 * Returns true iff the 4 specified sides of a rectangle are inside or equal
 * to this rectangle. i.e. is this rectangle a superset of the specified
 * rectangle. An empty rectangle never contains another rectangle.
 *
 * @param left The left side of the rectangle being tested for containment
 * @param top The top of the rectangle being tested for containment
 * @param right The right side of the rectangle being tested for containment
 * @param bottom The bottom of the rectangle being tested for containment
 * @return true iff the the 4 specified sides of a rectangle are inside or
 *              equal to this rectangle
 */
ECode CRect::ContainsEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // check for empty first
    *result = mLeft < mRight && mTop < mBottom
           // now check for containment
           && mLeft <= left && mTop <= top
           && mRight >= right && mBottom >= bottom;

    return NOERROR;
}

/**
 * Returns true iff the specified rectangle r is inside or equal to this
 * rectangle. An empty rectangle never contains another rectangle.
 *
 * @param r The rectangle being tested for containment.
 * @return true iff the specified rectangle r is inside or equal to this
 *              rectangle
 */
ECode CRect::ContainsEx2(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* temp = (CRect*)r;
           // check for empty first
    *result = mLeft < mRight && mTop < mBottom
           // now check for containment
           && mLeft <= temp->mLeft && mTop <= temp->mTop
           && mRight >= temp->mRight && mBottom >= temp->mBottom;

    return NOERROR;
}

ECode CRect::Intersect(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    if (!result) {
        return E_INVALID_ARGUMENT;
    }

    if (mLeft < right && left < mRight
        && mTop < bottom && top < mBottom) {
        if (mLeft < left) {
            mLeft = left;
        }
        if (mTop < top) {
            mTop = top;
        }
        if (mRight > right) {
            mRight = right;
        }
        if (mBottom > bottom) {
            mBottom = bottom;
        }

        *result = TRUE;
    }
    else {
        *result = FALSE;
    }

    return NOERROR;
}

ECode CRect::IntersectEx(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    if (!r || !result) {
        return E_INVALID_ARGUMENT;
    }

    CRect* temp = (CRect*)r;

    return Intersect(temp->mLeft, temp->mTop, temp->mRight, temp->mBottom, result);
}

ECode CRect::Intersects(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    if (!result) {
        return E_INVALID_ARGUMENT;
    }

    *result = mLeft < right && left < mRight
        && mTop < bottom && top < mBottom;

    return NOERROR;
}

ECode CRect::IntersectsEx(
    /* [in] */ IRect* a,
    /* [in] */ IRect* b,
    /* [out] */ Boolean* result)
{
    if (!a || !b || !result) {
        return E_INVALID_ARGUMENT;
    }

    CRect* tempA = (CRect*)a;
    CRect* tempB = (CRect*)b;

    *result = tempA->mLeft < tempB->mRight && tempB->mLeft < tempA->mRight
        && tempA->mTop < tempB->mBottom && tempB->mTop < tempA->mBottom;

    return NOERROR;
}

ECode CRect::Union(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if ((left < right) && (top < bottom)) {
        if ((mLeft < mRight) && (mTop < mBottom)) {
            if (mLeft > left)
                mLeft = left;
            if (mTop > top)
                mTop = top;
            if (mRight < right)
                mRight = right;
            if (mBottom < bottom)
                mBottom = bottom;
        } else {
            mLeft = left;
            mTop = top;
            mRight = right;
            mBottom = bottom;
        }
    }

    return NOERROR;
}

ECode CRect::UnionEx(
    /* [in] */ IRect* r)
{
    if (!r) {
        return E_INVALID_ARGUMENT;
    }

    CRect* temp = (CRect*)r;

    return Union(temp->mLeft, temp->mTop, temp->mRight, temp->mBottom);
}

ECode CRect::Scale(
    /* [in] */ Float scale)
{
    if (scale != 1.0f) {
        mLeft = (Int32)(mLeft * scale + 0.5f);
        mTop = (Int32)(mTop * scale + 0.5f);
        mRight = (Int32)(mRight * scale + 0.5f);
        mBottom = (Int32)(mBottom * scale + 0.5f);
    }

    return NOERROR;
}

ECode CRect::GetLeft(
    /* [out] */ Int32* left)
{
    VALIDATE_NOT_NULL(left);
    *left = mLeft;
    return NOERROR;
}

ECode CRect::SetLeft(
    /* [in] */ Int32 left)
{
    mLeft = left;
    return NOERROR;
}

ECode CRect::GetTop(
    /* [out] */ Int32* top)
{
    VALIDATE_NOT_NULL(top);
    *top = mTop;
    return NOERROR;
}

ECode CRect::SetTop(
    /* [in] */ Int32 top)
{
    mTop = top;
    return NOERROR;
}

ECode CRect::GetRight(
    /* [out] */ Int32* right)
{
    VALIDATE_NOT_NULL(right);
    *right = mRight;
    return NOERROR;
}

ECode CRect::SetRight(
    /* [in] */ Int32 right)
{
    mRight = right;
    return NOERROR;
}

ECode CRect::GetBottom(
    /* [out] */ Int32* bottom)
{
    VALIDATE_NOT_NULL(bottom);
    *bottom = mBottom;
    return NOERROR;
}

ECode CRect::SetBottom(
    /* [in] */ Int32 bottom)
{
    mBottom = bottom;
    return NOERROR;
}

ECode CRect::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    if (!source) {
        return E_INVALID_ARGUMENT;
    }

    source->ReadInt32(&mLeft);
    source->ReadInt32(&mRight);
    source->ReadInt32(&mTop);
    source->ReadInt32(&mBottom);

    return NOERROR;
}

ECode CRect::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (!dest) {
        return E_INVALID_ARGUMENT;
    }

    dest->WriteInt32(mLeft);
    dest->WriteInt32(mRight);
    dest->WriteInt32(mTop);
    dest->WriteInt32(mBottom);

    return NOERROR;
}
