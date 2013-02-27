
#ifndef __CTABLEMASKFILTER_H__
#define __CTABLEMASKFILTER_H__

#include "_CTableMaskFilter.h"
#include "graphics/MaskFilter.h"
#include <skia/effects/SkTableMaskFilter.h>

CarClass(CTableMaskFilter), public MaskFilter
{
public:
    CARAPI CreateClipTable(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max,
        /* [out] */ ITableMaskFilter ** ppTf);

    CARAPI CreateGammaTable(
        /* [in] */ Float gamma,
        /* [out] */ ITableMaskFilter ** ppTf);

    CARAPI constructor(
        /* [in] */ Int32 Ni);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte> & table);

private:
    static CARAPI_(SkMaskFilter*) NativeNewTable(
        /* [in] */ const ArrayOf<Byte> & table);

    static CARAPI_(Int32) NativeNewClip(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max);

    static CARAPI_(Int32) NativeNewGamma(
        /* [in] */ Float gamma);
};

#endif // __CTABLEMASKFILTER_H__
