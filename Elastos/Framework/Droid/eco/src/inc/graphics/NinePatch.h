
#ifndef __HH_NINEPATCH_H
#define __HH_NINEPATCH_H

#include "ext/frameworkext.h"
#include "elastos/AutoPtr.h"
#include "elastos/AutoFree.h"
#include "utils/AutoString.h"
#include <elastos.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkPaint.h>
#include <utils/ResourceTypes.h>

using namespace Elastos;

class NinePatch
{
public:
    NinePatch();

    CARAPI_(void) SetPaint(
        /* [in] */ IPaint* p);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRectF* location);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* location);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     * @param paint     The Paint to draw through.
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* location,
        /* [in] */ IPaint* paint);

    /**
     * Return the underlying bitmap's density, as per
     * {@link Bitmap#getDensity() Bitmap.getDensity()}.
     */
    CARAPI GetDensity(
        /* [out] */ Int32* density);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI HasAlpha(
        /* [out] */ Boolean* hasAlpha);

    CARAPI GetTransparentRegion(
        /* [in] */ IRect* location,
        /* [out] */ IRegion** region);

    static CARAPI_(Boolean) IsNinePatchChunk(
        /* [in] */ const ArrayOf<Byte>& chunk);

protected:
    constructor(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte>& chunk,
        /* [in] */ String srcName);

    constructor(
        /* [in] */ NinePatch* patch);

private:
    static CARAPI ValidateNinePatchChunk(
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte>& chunk);

    static CARAPI NativeDrawF(
        /* [in] */ SkCanvas* canvas,
        /* [in] */ IRectF* boundsRect,
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte>& chunk,
        /* [in] */ SkPaint* paint, // may be null
        /* [in] */ Int32 destDensity,
        /* [in] */ Int32 srcDensity);

    static CARAPI NativeDrawI(
        /* [in] */ SkCanvas* canvas,
        /* [in] */ IRect* boundsRect,
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte>& chunk,
        /* [in] */ SkPaint* paint, // may be null
        /* [in] */ Int32 destDensity,
        /* [in] */ Int32 srcDensity);

    static CARAPI NativeDraw(
        /* [in] */ SkCanvas* canvas,
        /* [in] */ SkRect& bounds,
        /* [in] */ const SkBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte>& chunkObj,
        /* [in] */ const SkPaint* paint,
        /* [in] */ Int32 destDensity,
        /* [in] */ Int32 srcDensity);

    static CARAPI_(Int32) NativeGetTransparentRegion(
        /* [in] */ const SkBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte>& chunkObj,
        /* [in] */ IRect* boundsRect);

private:
    static const String TAG;

    AutoPtr<IBitmap>          mBitmap;
    AutoFree< ArrayOf<Byte> >   mChunk;
    AutoPtr<IPaint>           mPaint;
    AutoString                  mSrcName;  // Useful for debugging
};

#endif // __HH_NINEPATCH_H

