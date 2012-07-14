
#include "graphics/Graphics.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include <skia/core/SkDither.h>
#include <skia/core/SkMallocPixelRef.h>
#include <Logger.h>

using namespace Elastos::Utility::Logging;

///////////////////////////////////////////////////////////////////////////////
// Conversions to/from SkColor, for get/setPixels, and the create method, which
// is basically like setPixels

typedef void (*FromColorProc)(void* dst, const SkColor src[], int width,
                              int x, int y);

static void FromColor_D32(void* dst, const SkColor src[], int width,
                          int, int)
{
    SkPMColor* d = (SkPMColor*)dst;

    for (int i = 0; i < width; i++) {
        *d++ = SkPreMultiplyColor(*src++);
    }
}

static void FromColor_D565(void* dst, const SkColor src[], int width,
                           int x, int y)
{
    uint16_t* d = (uint16_t*)dst;

    DITHER_565_SCAN(y);
    for (int stop = x + width; x < stop; x++) {
        SkColor c = *src++;
        *d++ = SkDitherRGBTo565(SkColorGetR(c), SkColorGetG(c), SkColorGetB(c),
                                DITHER_VALUE(x));
    }
}

static void FromColor_D4444(void* dst, const SkColor src[], int width,
                            int x, int y)
{
    SkPMColor16* d = (SkPMColor16*)dst;

    DITHER_4444_SCAN(y);
    for (int stop = x + width; x < stop; x++) {
        SkPMColor c = SkPreMultiplyColor(*src++);
        *d++ = SkDitherARGB32To4444(c, DITHER_VALUE(x));
//        *d++ = SkPixel32ToPixel4444(c);
    }
}

// can return NULL
static FromColorProc ChooseFromColorProc(SkBitmap::Config config) {
    switch (config) {
        case SkBitmap::kARGB_8888_Config:
            return FromColor_D32;
        case SkBitmap::kARGB_4444_Config:
            return FromColor_D4444;
        case SkBitmap::kRGB_565_Config:
            return FromColor_D565;
        default:
            break;
    }
    return NULL;
}

bool Graphics::PixelAllocator::allocPixelRef(
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ SkColorTable* ctable)
{
    return SUCCEEDED(Graphics::SetPixelRef(bitmap, ctable));
}

const char* Graphics::TAG = "Graphics";

SkRect* Graphics::IRect2SkRect(
    /* [in] */ IRect* obj,
    /* [in] */ SkRect* sr)
{
    CRect* r = (CRect*)obj;
    sr->set(SkIntToScalar(r->mLeft), SkIntToScalar(r->mTop),
           SkIntToScalar(r->mRight), SkIntToScalar(r->mBottom));
    return sr;
}

void Graphics::SkIRect2IRect(
        /* [in] */ const SkIRect& ir,
        /* [in] */ IRect* obj)
{
    CRect* r = (CRect*)obj;
    r->mLeft = ir.fLeft;
    r->mTop = ir.fTop;
    r->mRight = ir.fRight;
    r->mBottom = ir.fBottom;
}

SkRect* Graphics::IRectF2SkRect(
    /* [in] */ IRectF* obj,
    /* [in] */ SkRect* r)
{
    CRectF* rf = (CRectF*)obj;
    r->set(SkFloatToScalar(rf->mLeft), SkFloatToScalar(rf->mTop),
           SkFloatToScalar(rf->mRight), SkFloatToScalar(rf->mBottom));
    return r;
}

void Graphics::SkRect2IRectF(
    /* [in] */ const SkRect& r,
    /* [in] */ IRectF* obj)
{
    CRectF* rf = (CRectF*)obj;

    rf->mLeft = r.fLeft;
    rf->mTop = r.fTop;
    rf->mRight = r.fRight;
    rf->mBottom = r.fBottom;
}

SkBitmap::Config Graphics::GetNativeBitmapConfig(
        /* [in] */ BitmapConfig config)
{
    if (-1 == config) {
        return SkBitmap::kNo_Config;
    }
    Int32 c = config;
    if (c < 0 || c >= SkBitmap::kConfigCount) {
        c = SkBitmap::kNo_Config;
    }
    return static_cast<SkBitmap::Config>(c);
}

ECode Graphics::SetPixelRef(
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ SkColorTable* ctable)
{
    Sk64 size64 = bitmap->getSize64();
    if (size64.isNeg() || !size64.is32()) {
        Logger::E(TAG, "bitmap size exceeds 32bits");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    size_t size = size64.get32();

    // call the version of malloc that returns null on failure
    void* addr = sk_malloc_flags(size, 0);
    if (!addr) return E_OUT_OF_MEMORY_ERROR;

    SkPixelRef* pr = new SkMallocPixelRef(addr, size, ctable);
    if (!pr) return E_OUT_OF_MEMORY_ERROR;

    bitmap->setPixelRef(pr)->unref();
    // since we're already allocated, we lockPixels right away
    // HeapAllocator behaves this way too
    bitmap->lockPixels();

    return NOERROR;
}

Boolean Graphics::SetPixels(
    /* [in] */ const ArrayOf<Int32>& srcColors,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 srcStride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ const SkBitmap& dstBitmap)
{
    SkAutoLockPixels alp(dstBitmap);
    void* dst = dstBitmap.getPixels();
    FromColorProc proc = ChooseFromColorProc(dstBitmap.config());

    if (NULL == dst || NULL == proc) {
        return FALSE;
    }

    const Int32* array = srcColors.GetPayload();
    if (array == NULL) return FALSE;

    const SkColor* src = (const SkColor*)array + srcOffset;

    // reset to to actual choice from caller
    dst = dstBitmap.getAddr(x, y);
    // now copy/convert each scanline
    for (Int32 y = 0; y < height; y++) {
        proc(dst, src, width, x, y);
        src += srcStride;
        dst = (char*)dst + dstBitmap.rowBytes();
    }

    return TRUE;
}

