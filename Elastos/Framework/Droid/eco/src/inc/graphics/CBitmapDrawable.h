
#ifndef __CBITMAPDRAWABLE_H__
#define __CBITMAPDRAWABLE_H__

#include "_CBitmapDrawable.h"
#include "graphics/BitmapDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

CarClass(CBitmapDrawable), public BitmapDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IResources* res);

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap);

    CARAPI constructor(
        /* [in] */ String filepath);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ String filepath);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IBitmapState* state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetPaint(
        /* [out] */ IPaint** paint);

    CARAPI GetBitmap(
        /* [out] */ IBitmap** bitmap);

    CARAPI SetTargetDensity(
        /* [in] */ ICanvas* canvas);

    CARAPI SetTargetDensityEx(
        /* [in] */ IDisplayMetrics* metrics);

    CARAPI SetTargetDensityEx2(
        /* [in] */ Int32 density);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    CARAPI GetTileModeX(
        /* [out] */ ShaderTileMode* tileModeX);

    CARAPI GetTileModeY(
        /* [out] */ ShaderTileMode* tileModeY);

    CARAPI SetTileModeX(
        /* [in] */ ShaderTileMode mode);

    CARAPI SetTileModeY(
        /* [in] */ ShaderTileMode mode);

    CARAPI SetTileModeXY(
        /* [in] */ ShaderTileMode xmode,
        /* [in] */ ShaderTileMode ymode);
};

#endif //__CBITMAPDRAWABLE_H__