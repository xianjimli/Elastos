
#include "graphics/CTableMaskFilter.h"


ECode CTableMaskFilter::CreateClipTable(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [out] */ ITableMaskFilter ** ppTf)
{

    New(NativeNewClip(min, max), ppTf);

    return NOERROR;
}

ECode CTableMaskFilter::CreateGammaTable(
    /* [in] */ Float gamma,
    /* [out] */ ITableMaskFilter ** ppTf)
{
    New(NativeNewGamma(gamma), ppTf);
    return NOERROR;
}

ECode CTableMaskFilter::constructor(
    /* [in] */ Int32 Ni)
{
    mNativeInstance = (SkMaskFilter*)Ni;
    return NOERROR;
}

ECode CTableMaskFilter::constructor(
    /* [in] */ const ArrayOf<Byte> & table)
{
    if (table.GetLength() < 256) {
        return E_RUNTIME_EXCEPTION;
    }

    mNativeInstance = NativeNewTable(table);
    return NOERROR;
}

SkMaskFilter* CTableMaskFilter::NativeNewTable(
    /* [in] */ const ArrayOf<Byte> & table)
{
    SkASSERT(table);

    if (table.GetLength() < 256) {
        return NULL;
    }

    return new SkTableMaskFilter((const uint8_t*)table.GetPayload());
}

Int32 CTableMaskFilter::NativeNewClip(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    return (Int32)SkTableMaskFilter::CreateClip(min, max);
}

Int32 CTableMaskFilter::NativeNewGamma(
    /* [in] */ Float gamma)
{
    return (Int32)SkTableMaskFilter::CreateGamma(gamma);
}