
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
        /* [in] */ IPointF* p);

    CARAPI Negate();

    CARAPI Offset(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI Equals(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Boolean* result);

    CARAPI Length(
        /* [out] */ Float* length);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI constructor(
        /* [in] */ IPoint * point);

    static CARAPI_(Float) Length(
        /* [in] */ Float x,
        /* [in] */ Float y);

public:
    Float mX;
    Float mY;
};

#endif // __CPOINTF_H__
