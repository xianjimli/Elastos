
#ifndef __CLAYERRASTERIZER_H__
#define __CLAYERRASTERIZER_H__

#include "_CLayerRasterizer.h"
#include <skia/effects/SkLayerRasterizer.h>
#include "graphics/Rasterizer.h"
#include "graphics/Paint.h"

CarClass(CLayerRasterizer), public Rasterizer
{
public:
    CARAPI AddLayer(
        /* [in] */ IPaint * pPaint,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI AddLayerEx(
        /* [in] */ IPaint * pPaint);

    CARAPI constructor();

private:
    static CARAPI_(SkLayerRasterizer*)NativeConstructor();
    static CARAPI_(void)NativeAddLayer(
        /* [in] */ SkRasterizer* native_layer,
        /* [in] */ SkPaint* native_paint,
        /* [in] */ Float dx,
        /* [in] */ Float dy);
};

#endif // __CLAYERRASTERIZER_H__
