
#ifndef __CPOINT_H__
#define __CPOINT_H__

#include "_CPoint.h"

CarClass(CPoint)
{
public:
    CPoint();

    /**
     * Set the point's x and y coordinates
     */
    CARAPI Set(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI GetX(
        /* [out] */ Int32* pX);

    CARAPI GetY(
        /* [out] */ Int32* pY);

    /**
     * Negate the point's coordinates
     */
    CARAPI Negate();

    /**
     * Offset the point's coordinates by dx, dy
     */
    CARAPI Offset(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    /**
     * Returns true if the point's coordinates equal (x,y)
     */
    CARAPI Equals(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Boolean* result);

    CARAPI EqualsEx(
        /* [in] */ IInterface* p,
        /* [out] */ Boolean* result);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI constructor(
        /* [in] */ IPoint* p);

public:
    Int32 mX;
    Int32 mY;
};

#endif //__CPOINT_H__
