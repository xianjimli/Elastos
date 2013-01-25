
#ifndef __CDATESORTER_H__
#define __CDATESORTER_H__

#include "_CDateSorter.h"

CarClass(CDateSorter)
{
public:
    CARAPI GetIndex(
        /* [out] */ Int32 * pLastDay,
        /* [in] */ Int64 time);

    CARAPI GetLabel(
        /* [out] */ String * pLabel,
        /* [in] */ Int32 index);

    CARAPI GetBoundary(
        /* [out] */ Int64 * pBoundary,
        /* [in] */ Int32 index);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    // TODO: Add your private member variables here.
};

#endif // __CDATESORTER_H__
