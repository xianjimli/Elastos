
#include "graphics/CSumPathEffect.h"


ECode CSumPathEffect::constructor(
    /* [in] */ IPathEffect* first,
    /* [in] */ IPathEffect* second)
{
    mNativeInstance = NativeCreate((
                        (PathEffect*)first->Probe(EIID_PathEffect))->Ni(),
                        ((PathEffect*)second->Probe(EIID_PathEffect))->Ni());
    return NOERROR;
}

PInterface CSumPathEffect::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CSumPathEffect::Probe(riid);
}

SkPathEffect* CSumPathEffect::NativeCreate(
    /* [in] */ SkPathEffect* first,
    /* [in] */ SkPathEffect* second)
{
    return new SkSumPathEffect(first, second);
}
