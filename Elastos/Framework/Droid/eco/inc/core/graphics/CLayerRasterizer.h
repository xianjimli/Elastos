
#ifndef __CLAYERRASTERIZER_H__
#define __CLAYERRASTERIZER_H__

#include "_CLayerRasterizer.h"
#include <skia/effects/SkLayerRasterizer.h>
#include "graphics/Rasterizer.h"
#include "graphics/Paint.h"

CarClass(CLayerRasterizer), public Rasterizer
{
public:
    CARAPI addLayer(
        /* [in] */ IPaint * pPaint,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI addLayerEx(
        /* [in] */ IPaint * pPaint);

    CARAPI constructor();

private:
    static CARAPI_(SkLayerRasterizer*)nativeConstructor();
    static CARAPI_(void)nativeAddLayer(
                                    SkRasterizer* native_layer,
                                    SkPaint* native_paint,
                                    Float dx,
                                    Float dy);

};

#endif // __CLAYERRASTERIZER_H__
