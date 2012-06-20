
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "ext/frameworkext.h"
#include <skia/core/SkRect.h>
#include <skia/core/SkBitmap.h>

class Graphics
{
public:
    class PixelAllocator : public SkBitmap::Allocator
    {
    public:
        // overrides
        virtual bool allocPixelRef(
            /* [in] */ SkBitmap* bitmap,
            /* [in] */ SkColorTable* ctable);
    };

public:
    static CARAPI_(void) SkIRect2IRect(
        /* [in] */ const SkIRect& ir,
        /* [in] */ IRect* obj);

    static CARAPI_(SkRect*) IRectF2SkRect(
        /* [in] */ IRectF* obj,
        /* [in] */ SkRect* r);

    static CARAPI_(void) SkRect2IRectF(
        /* [in] */ const SkRect& r,
        /* [in] */ IRectF* obj);

    static CARAPI SetPixelRef(
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ SkColorTable* ctable);

    static CARAPI_(Boolean) SetPixels(
        /* [in] */ const ArrayOf<Int32>& srcColors,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 srcStride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ const SkBitmap& dstBitmap);

private:
    static const String TAG;
};

#endif //__GRAPHICS_H__
