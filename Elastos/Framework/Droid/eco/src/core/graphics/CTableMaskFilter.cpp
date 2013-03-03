
#include "ext/frameworkdef.h"
#include "graphics/CTableMaskFilter.h"


ECode CTableMaskFilter::constructor(
    /* [in] */ Handle32 Ni)
{
    mNativeInstance = (SkMaskFilter*)Ni;
    return NOERROR;
}

ECode CTableMaskFilter::constructor(
    /* [in] */ const ArrayOf<Byte>& table)
{
    if (table.GetLength() < 256) {
        // throw new RuntimeException("table.length must be >= 256");
        return E_RUNTIME_EXCEPTION;
    }
    mNativeInstance = NativeNewTable(table);
    return NOERROR;
}

PInterface CTableMaskFilter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_MaskFilter) {
        return reinterpret_cast<PInterface>((MaskFilter*)this);
    }
    return _CTableMaskFilter::Probe(riid);
}

ECode CTableMaskFilter::CreateClipTable(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [out] */ ITableMaskFilter** tf)
{
    VALIDATE_NOT_NULL(tf);

    return CTableMaskFilter::New((Handle32)NativeNewClip(min, max), tf);
}

ECode CTableMaskFilter::CreateGammaTable(
    /* [in] */ Float gamma,
    /* [out] */ ITableMaskFilter** tf)
{
    VALIDATE_NOT_NULL(tf);

    return CTableMaskFilter::New((Handle32)NativeNewGamma(gamma), tf);
}

SkMaskFilter* CTableMaskFilter::NativeNewTable(
    /* [in] */ const ArrayOf<Byte>& table)
{
    return new SkTableMaskFilter((const uint8_t*)table.GetPayload());
}

SkMaskFilter* CTableMaskFilter::NativeNewClip(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    return SkTableMaskFilter::CreateClip(min, max);
}

SkMaskFilter* CTableMaskFilter::NativeNewGamma(
    /* [in] */ Float gamma)
{
    return SkTableMaskFilter::CreateGamma(gamma);
}
