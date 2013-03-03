
#include "graphics/CLayerRasterizer.h"


PInterface CLayerRasterizer::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_Rasterizer) {
        return reinterpret_cast<PInterface>((Rasterizer*)this);
    }
    return _CLayerRasterizer::Probe(riid);
}

ECode CLayerRasterizer::AddLayer(
    /* [in] */ IPaint* paint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeAddLayer((SkLayerRasterizer*)mNativeInstance, ((Paint*)paint->Probe(EIID_Paint))->mNativePaint, dx, dy);
    return NOERROR;
}

ECode CLayerRasterizer::AddLayerEx(
    /* [in] */ IPaint* paint)
{
    NativeAddLayer((SkLayerRasterizer*)mNativeInstance, ((Paint*)paint->Probe(EIID_Paint))->mNativePaint, 0, 0);
    return NOERROR;
}

ECode CLayerRasterizer::constructor()
{
    mNativeInstance = NativeConstructor();
    return NOERROR;
}

SkLayerRasterizer* CLayerRasterizer::NativeConstructor()
{
    return new SkLayerRasterizer();
}

void CLayerRasterizer::NativeAddLayer(
    /* [in] */ SkLayerRasterizer* nativeLayer,
    /* [in] */ SkPaint* nativePaint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkASSERT(nativeLayer);
    SkASSERT(nativePaint);
    nativeLayer->addLayer(*nativePaint, SkFloatToScalar(dx), SkFloatToScalar(dy));
}
