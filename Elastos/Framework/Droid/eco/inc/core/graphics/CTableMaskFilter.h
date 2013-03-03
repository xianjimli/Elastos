
#ifndef __CTABLEMASKFILTER_H__
#define __CTABLEMASKFILTER_H__

#include "_CTableMaskFilter.h"
#include "graphics/MaskFilter.h"
#include <skia/effects/SkTableMaskFilter.h>

CarClass(CTableMaskFilter), public MaskFilter
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 Ni);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& table);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    static CARAPI CreateClipTable(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max,
        /* [out] */ ITableMaskFilter** tf);

    static CARAPI CreateGammaTable(
        /* [in] */ Float gamma,
        /* [out] */ ITableMaskFilter** tf);

private:
    static CARAPI_(SkMaskFilter*) NativeNewTable(
        /* [in] */ const ArrayOf<Byte>& table);

    static CARAPI_(SkMaskFilter*) NativeNewClip(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max);

    static CARAPI_(SkMaskFilter*) NativeNewGamma(
        /* [in] */ Float gamma);
};

#endif // __CTABLEMASKFILTER_H__
