
#include "graphics/CLayerRasterizer.h"

ECode CLayerRasterizer::addLayer(
    /* [in] */ IPaint * pPaint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    nativeAddLayer(mNativeInstance, ((Paint*)pPaint)->mNativePaint, dx, dy);
    return E_NOT_IMPLEMENTED;
}

ECode CLayerRasterizer::addLayerEx(
    /* [in] */ IPaint * pPaint)
{
    nativeAddLayer(mNativeInstance, ((Paint*)pPaint)->mNativePaint, 0, 0);
    return E_NOT_IMPLEMENTED;
}

ECode CLayerRasterizer::constructor()
{
    mNativeInstance = nativeConstructor();
    return E_NOT_IMPLEMENTED;
}

SkLayerRasterizer* CLayerRasterizer::nativeConstructor()
{
    return new SkLayerRasterizer();
}

void CLayerRasterizer::nativeAddLayer(
                                SkRasterizer* native_layer,
                                SkPaint* native_paint,
                                Float dx,
                                Float dy)
{
    SkASSERT(native_layer);
    SkASSERT(native_paint);

    SkLayerRasterizer * layer = (SkLayerRasterizer*) native_layer;
    const SkPaint * Paint = native_paint;
    layer->addLayer(*Paint, SkFloatToScalar(dx), SkFloatToScalar(dy));
}