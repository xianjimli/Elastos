
#include "graphics/CLayerRasterizer.h"

ECode CLayerRasterizer::AddLayer(
    /* [in] */ IPaint * pPaint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeAddLayer(mNativeInstance, ((Paint*)pPaint)->mNativePaint, dx, dy);
    return NOERROR;
}

ECode CLayerRasterizer::AddLayerEx(
    /* [in] */ IPaint * pPaint)
{
    NativeAddLayer(mNativeInstance, ((Paint*)pPaint)->mNativePaint, 0, 0);
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
    /* [in] */ SkRasterizer* native_layer,
    /* [in] */ SkPaint* native_paint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkASSERT(native_layer);
    SkASSERT(native_paint);

    SkLayerRasterizer * layer = (SkLayerRasterizer*) native_layer;
    const SkPaint * Paint = native_paint;
    layer->addLayer(*Paint, SkFloatToScalar(dx), SkFloatToScalar(dy));
}