
#include "graphics/CPointF.h"
#include "graphics/CPoint.h"

/**
* Set the point's x and y coordinates
*/
ECode CPointF::set(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    this->x = x;
    this->y = y;
    return NOERROR;
}

ECode CPointF::setEx(
    /* [in] */ IPointF * pP)
{
    this->x = ((CPoint*)pP)->mX;
    this->y = ((CPoint*)pP)->mY;
    return NOERROR;
}

ECode CPointF::negate()
{
    x = -x;
    y = -y;
    return NOERROR;
}

ECode CPointF::offset(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    x += dx;
    y += dy;
    return NOERROR;
}

ECode CPointF::equals(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ Boolean * pResult)
{
    *pResult = ((this->x == x) && (this->y == y));
    return NOERROR;
}

ECode CPointF::lengthEx(
    /* [out] */ Float * pLength)
{
    length(x, y, pLength);
    return NOERROR;
}

ECode CPointF::length(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ Float * pLength)
{
    //*pLength = FloatMath.sqrt(x * x + y * y); FloatMath can not found
    return NOERROR;
}

ECode CPointF::constructor()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CPointF::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    this->x = x;
    this->y = y;
    return NOERROR;
}

ECode CPointF::constructor(
    /* [in] */ IPoint * pPoint)
{
    this->x = ((CPoint*)pPoint)->mX;
    this->y = ((CPoint*)pPoint)->mY;
    return NOERROR;
}

