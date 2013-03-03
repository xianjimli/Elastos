
#include "ext/frameworkdef.h"
#include "graphics/CPointF.h"
#include "graphics/CPoint.h"
#include <elastos/Math.h>


using namespace Elastos::Core;

ECode CPointF::constructor()
{
    return NOERROR;
}

ECode CPointF::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mX = x;
    mY = y;
    return NOERROR;
}

ECode CPointF::constructor(
    /* [in] */ IPoint* point)
{
    mX = ((CPoint*)point)->mX;
    mY = ((CPoint*)point)->mY;
    return NOERROR;
}

/**
* Set the point's x and y coordinates
*/
ECode CPointF::Set(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mX = x;
    mY = y;
    return NOERROR;
}

ECode CPointF::SetEx(
    /* [in] */ IPointF* p)
{
    mX = ((CPointF*)p)->mX;
    mY = ((CPointF*)p)->mY;
    return NOERROR;
}

ECode CPointF::Negate()
{
    mX = -mX;
    mY = -mY;
    return NOERROR;
}

ECode CPointF::Offset(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    mX += dx;
    mY += dy;
    return NOERROR;
}

ECode CPointF::Equals(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (mX == x) && (mY == y);
    return NOERROR;
}

ECode CPointF::Length(
    /* [out] */ Float* length)
{
    VALIDATE_NOT_NULL(length);

    *length = Length(mX, mY);
    return NOERROR;
}

Float CPointF::Length(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return Math::Sqrt(x * x + y * y);
}
