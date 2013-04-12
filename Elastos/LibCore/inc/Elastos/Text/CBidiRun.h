
#ifndef __CBIDIRUN_H__
#define __CBIDIRUN_H__

#include "_CBidiRun.h"
#include "BidiRun.h"


CarClass(CBidiRun), public BidiRun
{
public:
    CARAPI constructor(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ Int32 level);

    CARAPI GetLevel(
        /* [out] */ Int32 * pLevel);

    CARAPI GetLimit(
        /* [out] */ Int32 * pLimit);

    CARAPI GetStart(
        /* [out] */ Int32 * pStart);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBIDIRUN_H__
