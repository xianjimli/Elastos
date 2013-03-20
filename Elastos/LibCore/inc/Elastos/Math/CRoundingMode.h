
#ifndef __CROUNDINGMODE_H__
#define __CROUNDINGMODE_H__

#include "_CRoundingMode.h"
#include "RoundingMode.h"
CarClass(CRoundingMode), public RoundingMode
{
public:
    CARAPI constructor(
        /* [in] */ Int32 rm);

    CARAPI ValueOf(
        /* [in] */ Int32 mode,
        /* [out] */ RRoundingMode * pNewObj);

//    CARAPI Func(
//        /* [in] */ RoundingMode roundingModeEnum);

private:
    // TODO: Add your private member variables here.
};

#endif // __CROUNDINGMODE_H__
