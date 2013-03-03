
#include "graphics/CComposePathEffect.h"


ECode CComposePathEffect::constructor(
    /* [in] */ IPathEffect* outerpe,
    /* [in] */ IPathEffect* innerpe)
{
    mNativeInstance = NativeCreate((
                        (PathEffect*)outerpe->Probe(EIID_PathEffect))->Ni(),
                        ((PathEffect*)innerpe->Probe(EIID_PathEffect))->Ni());
    return NOERROR;
}

PInterface CComposePathEffect::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CComposePathEffect::Probe(riid);
}

SkPathEffect* CComposePathEffect::NativeCreate(
    /* [in] */ SkPathEffect* outerpe,
    /* [in] */ SkPathEffect* innerpe)
{
    return new SkComposePathEffect(
            outerpe, innerpe);
}
