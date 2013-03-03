
#ifndef __CLAYERRASTERIZER_H__
#define __CLAYERRASTERIZER_H__

#include "_CLayerRasterizer.h"
#include "graphics/Rasterizer.h"
#include "graphics/Paint.h"
#include <skia/effects/SkLayerRasterizer.h>

CarClass(CLayerRasterizer), public Rasterizer
{
public:
    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    CARAPI AddLayer(
        /* [in] */ IPaint* paint,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI AddLayerEx(
        /* [in] */ IPaint* paint);

    CARAPI constructor();

private:
    static CARAPI_(SkLayerRasterizer*) NativeConstructor();

    static CARAPI_(void) NativeAddLayer(
        /* [in] */ SkLayerRasterizer* nativeLayer,
        /* [in] */ SkPaint* nativePaint,
        /* [in] */ Float dx,
        /* [in] */ Float dy);
};

#endif // __CLAYERRASTERIZER_H__
