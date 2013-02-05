
#ifndef __CPOINTF_H__
#define __CPOINTF_H__

#include "_CPointF.h"

CarClass(CPointF)
{
public:
    CARAPI set(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI setEx(
        /* [in] */ IPointF * pP);

    CARAPI negate();

    CARAPI offset(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI equals(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Boolean * pResult);

    CARAPI lengthEx(
        /* [out] */ Float * pLength);

    CARAPI length(
        /* [in] */ Float x,
        /* [in] */ Float y,
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
