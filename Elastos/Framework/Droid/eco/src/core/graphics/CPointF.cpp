
#include "graphics/CPointF.h"
#include "graphics/CPoint.h"
#include "graphics/CPointFHelper.h"


/**
* Set the point's x and y coordinates
*/
ECode CPointF::Set(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    this->x = x;
    this->y = y;
    return NOERROR;
}

ECode CPointF::SetEx(
    /* [in] */ IPointF * pP)
{
    this->x = ((CPoint*)pP)->mX;
    this->y = ((CPoint*)pP)->mY;
    return NOERROR;
}

ECode CPointF::Negate()
{
    x = -x;
    y = -y;
    return NOERROR;
}

ECode CPointF::Offset(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    x += dx;
    y += dy;
    return NOERROR;
}

ECode CPointF::Equals(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ Boolean * pResult)
{
    *pResult = ((this->x == x) && (this->y == y));
    return NOERROR;
}

ECode CPointF::Length(
    /* [out] */ Float * pLength)
{
    IPointFHelper* pPointFHelper = NULL;
    CPointFHelper::AcquireSingleton(&pPointFHelper);

    pPointFHelper->Length(x, y, pLength);
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

