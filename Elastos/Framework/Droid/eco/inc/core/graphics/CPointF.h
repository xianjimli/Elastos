
#ifndef __CPOINTF_H__
#define __CPOINTF_H__

#include "_CPointF.h"

CarClass(CPointF)
{
public:
    CARAPI Set(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI SetEx(
        /* [in] */ IPointF * pP);

    CARAPI Negate();

    CARAPI Offset(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI Equals(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Boolean * pResult);

    CARAPI Length(
        /* [out] */ Float * pLength);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI constructor(
        /* [in] */ IPoint * pPoint);

    Float x;
    Float y;
private:
    // TODO: Add your private member variables here.
};

#endif // __CPOINTF_H__
