
#include "graphics/CBitmap.h"
#include "graphics/Graphics.h"
#include "graphics/CPaint.h"
#include "graphics/CCanvas.h"
#include "graphics/CreateOutputStreamAdaptor.h"
#include "graphics/CMatrix.h"
#include "ext/frameworkdef.h"
#include "utils/CDisplayMetrics.h"
#include "utils/elastos_nio_utils.h"
#include <elastos/Math.h>
#include <Logger.h>
#include <StringBuffer.h>
#include <skia/core/SkTypes.h>
#include <skia/core/SkUnPreMultiply.h>
#include <skia/core/SkColorPriv.h>
#include <skia/images/SkImageEncoder.h>
#include <skia/core/SkDither.h>

using namespace Elastos::System;
using namespace Elastos::Utility::Logging;

const String CBitmap::TAG = "CBitmap";
Mutex CBitmap::sClsLock;
AutoPtr<IMatrix> CBitmap::sScaleMatrix;
Int32 CBitmap::sDefaultDensity = -1;
Int32 CBitmap::WORKING_COMPRESS_STORAGE = 4096;

CBitmap::CBitmap()
    : mNativeBitmap(NULL)
    , mIsMutable(FALSE)
    , mNinePatchChunk(NULL)
    , mWidth(-1)
    , mHeight(-1)
    , mRecycled(FALSE)
    , mDensity(-1)
{
}

void CBitmap::SetDefaultDensity(
    /* [in] */ Int32 density)
{
    sDefaultDensity = density;
}

Int32 CBitmap::GetDefaultDensity()
{
    if (sDefaultDensity >= 0) {
        return sDefaultDensity;
    }
    sDefaultDensity = CDisplayMetrics::DENSITY_DEVICE;
    return sDefaultDensity;
}

ECode CBitmap::GetDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);
    *density = mDensity;
    return NOERROR;
}

ECode CBitmap::SetDensity(
    /* [in] */ Int32 density)
{
    mDensity = density;
    return NOERROR;
}

ECode CBitmap::SetNinePatchChunk(
    /* [in] */ const ArrayOf<Byte> & chunk)
{
    mNinePatchChunk = chunk.Clone();
    return NOERROR;
}

ECode CBitmap::Recycle()
{
    if (!mRecycled) {
        NativeRecycle(mNativeBitmap);
        mNinePatchChunk = NULL;
        mRecycled = TRUE;
    }
    return NOERROR;
}

ECode CBitmap::IsRecycled(
    /* [out] */ Boolean* isRecycled)
{
    VALIDATE_NOT_NULL(isRecycled);

    *isRecycled = mRecycled;
    return NOERROR;
}

ECode CBitmap::CopyPixelsToBuffer(
    /* [in] */ IBuffer* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 elements = 0;
    dst->Remaining(&elements);
    Int32 shift = 0;
    if (dst->Probe(EIID_IByteBuffer)) {
        shift = 0;
    }
    else if (dst->Probe(EIID_IShortBuffer)) {
        shift = 1;
    }
    else if (dst->Probe(EIID_IIntBuffer)) {
        shift = 2;
    }
    else {
        Logger::E(TAG, "unsupported Buffer subclass");
        return E_RUNTIME_EXCEPTION;
    }

    Int64 bufferSize = (Int64)elements << shift;
    Int32 rowBytes = 0;
    Int32 height = 0;
    GetRowBytes(&rowBytes);
    GetHeight(&height);
    Int64 pixelSize = (Int64)rowBytes * height;

    if (bufferSize < pixelSize) {
        Logger::E(TAG, "Buffer not large enough for pixels");
        return E_RUNTIME_EXCEPTION;
    }

    NativeCopyPixelsToBuffer(mNativeBitmap, dst);

    // now update the buffer's position
    Int32 position = 0;
    dst->Position(&position);
    position += pixelSize >> shift;
    dst->PositionEx(position);
    return NOERROR;
}

ECode CBitmap::CopyPixelsFromBuffer(
    /* [in] */ IBuffer* src)
{
    VALIDATE_NOT_NULL(src);

    if (mRecycled) {
        Logger::E(TAG, "copyPixelsFromBuffer called on recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 elements;
    src->Remaining(&elements);
    Int32 shift = 0;
    if (src->Probe(EIID_IByteBuffer)) {
        shift = 0;
    }
    else if (src->Probe(EIID_IShortBuffer)) {
        shift = 1;
    }
    else if (src->Probe(EIID_IIntBuffer)) {
        shift = 2;
    }
    else {
        Logger::E(TAG, "unsupported Buffer subclass");
        return E_RUNTIME_EXCEPTION;
    }

    Int64 bufferBytes = (Int64)elements << shift;
    Int32 rowBytes = 0;
    Int32 height = 0;
    GetRowBytes(&rowBytes);
    GetHeight(&height);
    Int64 bitmapBytes = (Int64)rowBytes * height;

    if (bufferBytes < bitmapBytes) {
        Logger::E(TAG, "Buffer not large enough for pixels");
        return E_RUNTIME_EXCEPTION;
    }

    NativeCopyPixelsFromBuffer(mNativeBitmap, src);
    return NOERROR;
}

ECode CBitmap::Copy(
    /* [in] */ BitmapConfig config,
    /* [in] */ Boolean isMutable,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    if (mRecycled) {
        Logger::E(TAG, "Can't copy a recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<CBitmap> b;
    FAIL_RETURN(NativeCopy(mNativeBitmap, (Int32)config, isMutable, (CBitmap**)&b));

    b->mDensity = mDensity;
    *bitmap = (IBitmap*)b.Get();
    (*bitmap)->AddRef();
    return NOERROR;
}

ECode CBitmap::GetNinePatchChunk(
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    VALIDATE_NOT_NULL(data);

    if (mNinePatchChunk == NULL) {
        *data = NULL;
        return NOERROR;
    }

    *data = mNinePatchChunk->Clone();
    return NOERROR;
}

ECode CBitmap::Compress(
    /* [in] */ BitmapCompressFormat format,
    /* [in] */ Int32 quality,
    /* [in] */ IOutputStream* stream,
    /* [out] */ Boolean* isSucceeded)
{
    VALIDATE_NOT_NULL(isSucceeded);

    if (mRecycled) {
        Logger::E(TAG, "Can't compress a recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // do explicit check before calling the native method
    if (stream == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (quality < 0 || quality > 100) {
        Logger::E(TAG, "quality must be 0..100");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoFree< ArrayOf<Byte> > arr = ArrayOf<Byte>::Alloc(WORKING_COMPRESS_STORAGE);
    if (arr == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    *isSucceeded = NativeCompress(
        mNativeBitmap, (Int32)format, quality, stream, arr.Get());
    return NOERROR;
}

ECode CBitmap::IsMutable(
    /* [out] */ Boolean* isMutable)
{
    VALIDATE_NOT_NULL(isMutable);

    *isMutable = mIsMutable;
    return NOERROR;
}

ECode CBitmap::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth == -1 ? (mWidth = mNativeBitmap->width()) : mWidth;
    return NOERROR;
}

ECode CBitmap::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight == -1 ? (mHeight = mNativeBitmap->height()) : mHeight;
    return NOERROR;
}

ECode CBitmap::GetScaledWidth(
    /* [in] */ ICanvas* canvas,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(canvas);
    VALIDATE_NOT_NULL(width);

    Int32 w = 0;
    GetWidth(&w);
    *width = ScaleFromDensity(w, mDensity, ((CCanvas*)canvas)->mDensity);
    return NOERROR;
}

ECode CBitmap::GetScaledHeight(
    /* [in] */ ICanvas* canvas,
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(canvas);
    VALIDATE_NOT_NULL(height);

    Int32 h = 0;
    GetWidth(&h);
    *height = ScaleFromDensity(h, mDensity, ((CCanvas*)canvas)->mDensity);
    return NOERROR;
}

ECode CBitmap::GetScaledWidthEx(
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(metrics);
    VALIDATE_NOT_NULL(width);

    Int32 w = 0;
    GetWidth(&w);
    *width = ScaleFromDensity(w, mDensity, ((CDisplayMetrics*)metrics)->mDensityDpi);
    return NOERROR;
}

ECode CBitmap::GetScaledHeightEx(
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(metrics);
    VALIDATE_NOT_NULL(height);

    Int32 h = 0;
    GetWidth(&h);
    *height = ScaleFromDensity(h, mDensity, ((CDisplayMetrics*)metrics)->mDensityDpi);
    return NOERROR;
}

ECode CBitmap::GetScaledWidthEx2(
    /* [in] */ Int32 targetDensity,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    Int32 w = 0;
    GetWidth(&w);
    *width = ScaleFromDensity(w, mDensity, targetDensity);
    return NOERROR;
}

ECode CBitmap::GetScaledHeightEx2(
    /* [in] */ Int32 targetDensity,
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    Int32 h = 0;
    GetHeight(&h);
    *height = ScaleFromDensity(h, mDensity, targetDensity);
    return NOERROR;
}

Int32 CBitmap::ScaleFromDensity(
    /* [in] */ Int32 size,
    /* [in] */ Int32 sdensity,
    /* [in] */ Int32 tdensity)
{
    if (sdensity == DENSITY_NONE || sdensity == tdensity) {
        return size;
    }

    // Scale by tdensity / sdensity, rounding up.
    return ( (size * tdensity) + (sdensity >> 1) ) / sdensity;
}

ECode CBitmap::GetRowBytes(
    /* [out] */ Int32* bytes)
{
    VALIDATE_NOT_NULL(bytes);

    *bytes = NativeRowBytes(mNativeBitmap);
    return NOERROR;
}

ECode CBitmap::GetConfig(
    /* [out] */ BitmapConfig* config)
{
    VALIDATE_NOT_NULL(config);

    *config = Config_NativeToConfig(NativeConfig(mNativeBitmap));
    return NOERROR;
}

ECode CBitmap::HasAlpha(
    /* [out] */ Boolean* isAlpha)
{
    VALIDATE_NOT_NULL(isAlpha);

    *isAlpha = NativeHasAlpha(mNativeBitmap);
    return NOERROR;
}

ECode CBitmap::SetHasAlpha(
    /* [in] */ Boolean hasAlpha)
{
    NativeSetHasAlpha(mNativeBitmap, hasAlpha);
    return NOERROR;
}

ECode CBitmap::EraseColor(
    /* [in] */ Int32 c)
{
    if (mRecycled) {
        Logger::E(TAG, "Can't erase a recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (!mIsMutable) {
        Logger::E(TAG, "cannot erase immutable bitmaps");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    NativeErase(mNativeBitmap, c);
    return NOERROR;
}

ECode CBitmap::GetPixel(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Int32* pixel)
{
    VALIDATE_NOT_NULL(pixel);
    if (mRecycled) {
        Logger::E(TAG, "Can't call getPixel() on a recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (!CheckPixelAccess(x, y)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *pixel = NativeGetPixel(mNativeBitmap, x, y);
    return NOERROR;
}

ECode CBitmap::GetPixels(
    /* [in] */ const ArrayOf<Int32>& pixels,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mRecycled) {
        Logger::E(TAG, "Can't call getPixels() on a recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (width == 0 || height == 0) {
        return NOERROR; // nothing to do
    }

    FAIL_RETURN(CheckPixelsAccess(
        x, y, width, height, offset, stride, pixels));

    NativeGetPixels(
        mNativeBitmap, pixels, offset, stride, x, y, width, height);
    return NOERROR;
}

ECode CBitmap::SetPixel(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 color)
{
    if (mRecycled) {
        Logger::E(TAG, "Can't call setPixel() on a recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (!mIsMutable) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (!CheckPixelAccess(x, y)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    NativeSetPixel(mNativeBitmap, x, y, color);
    return NOERROR;
}

ECode CBitmap::SetPixels(
    /* [in] */ const ArrayOf<Int32>& pixels,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mRecycled) {
        Logger::E(TAG, "Can't call SetPixels() on a recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (!mIsMutable) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (width == 0 || height == 0) {
        return NOERROR; // nothing to do
    }

    FAIL_RETURN(CheckPixelsAccess(x, y, width, height, offset, stride, pixels));

    NativeSetPixels(
        mNativeBitmap, pixels, offset, stride, x, y, width, height);
    return NOERROR;
}

ECode CBitmap::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);

    *contents = 0;
    return NOERROR;
}

ECode CBitmap::ExtractAlpha(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    ArrayOf<Int32> a(NULL, 0);
    return ExtractAlphaEx(NULL, a, bitmap);
}

ECode CBitmap::ExtractAlphaEx(
    /* [in] */ IPaint* paint,
    /* [in] */ const ArrayOf<Int32>& offsetXY,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    if (mRecycled) {
        Logger::E(TAG, "Can't extractAlpha on a recycled bitmap");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    SkPaint* nativePaint = paint != NULL ? ((Paint*)paint->Probe(EIID_Paint))->mNativePaint : NULL;
    AutoPtr<CBitmap> bm;
    ArrayOf<Int32>* o = offsetXY.GetPayload() ? (ArrayOf<Int32>*)&offsetXY : NULL;
    ECode ec = NativeExtractAlpha(mNativeBitmap, nativePaint, o, (CBitmap**)&bm);
    if (FAILED(ec)) {
        Logger::E(TAG, "Failed to extractAlpha on Bitmap");
        return E_RUNTIME_EXCEPTION;
    }

    bm->mDensity = mDensity;
    *bitmap = (IBitmap*)bm.Get();
    (*bitmap)->AddRef();
    return NOERROR;
}

ECode CBitmap::SameAs(
    /* [in] */ IBitmap* other,
    /* [out] */ Boolean* isSame)
{
    VALIDATE_NOT_NULL(isSame);

    *isSame = ((other != NULL) && (IBitmap*)this == other) || (other != NULL
        && NativeSameAs(mNativeBitmap, ((CBitmap*)other)->mNativeBitmap));
    return NOERROR;
}

ECode CBitmap::PrepareToDraw()
{
    NativePrepareToDraw(mNativeBitmap);
    return NOERROR;
}

ECode CBitmap::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBitmap::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBitmap::constructor(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ Boolean isMutable,
    /* [in] */ const ArrayOf<Byte>& ninePatchChunk,
    /* [in] */ Int32 density)
{
    if (nativeBitmap == 0) {
        Logger::E(TAG, "internal error: native bitmap is 0");
        return E_RUNTIME_EXCEPTION;
    }

    // we delete this in our finalizer
    mNativeBitmap = (SkBitmap*)nativeBitmap;
    mIsMutable = isMutable;
    mNinePatchChunk = ninePatchChunk.Clone();
    if (density >= 0) {
        mDensity = density;
    }
    return NOERROR;
}

ECode CBitmap::Init(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ Boolean isMutable,
    /* [in] */ ArrayOf<Byte>* ninePatchChunk,
    /* [in] */ Int32 density)
{
    if (nativeBitmap == NULL) {
        Logger::E(TAG, "internal error: native bitmap is 0");
        return E_RUNTIME_EXCEPTION;
    }

    // we delete this in our finalizer
    mNativeBitmap = nativeBitmap;
    mIsMutable = isMutable;
    mNinePatchChunk = ninePatchChunk;
    if (density >= 0) {
        mDensity = density;
    }
    return NOERROR;
}

SkBitmap* CBitmap::Ni()
{
    return mNativeBitmap;
}

ECode CBitmap::CreateScaledBitmap(
    /* [in] */ IBitmapFactory* factory,
    /* [in] */ IBitmap* src,
    /* [in] */ Int32 dstWidth,
    /* [in] */ Int32 dstHeight,
    /* [in] */ Boolean filter,
    /* [out] */ IBitmap** bitmap)
{
    assert(factory);
    assert(src);
    assert(bitmap);

    AutoPtr<IMatrix> m;
    {
        Mutex::Autolock lock(sClsLock);
        // small pool of just 1 matrix
        m = sScaleMatrix;
        sScaleMatrix = NULL;
    }

    if (m == NULL) {
        FAIL_RETURN(CMatrix::New((IMatrix**)&m));
    }

    Int32 width = 0, height = 0;
    src->GetWidth(&width);
    src->GetHeight(&height);
    Float sx = dstWidth  / (Float)width;
    Float sy = dstHeight / (Float)height;
    m->SetScaleEx(sx, sy);
    FAIL_RETURN(factory->CreateBitmapEx2(src, 0, 0, width, height, m, filter, bitmap));

    {
        Mutex::Autolock lock(sClsLock);
        // do we need to check for NULL? why not just assign everytime?
        if (sScaleMatrix == NULL) {
            sScaleMatrix = m;
        }
    }

    return NOERROR;
}

BitmapConfig CBitmap::Config_NativeToConfig(
    /* [in] */ Int32 config)
{
    switch(config) {
        case 2:
            return BitmapConfig_ALPHA_8;
        case 4:
            return BitmapConfig_RGB_565;
        case 5:
            return BitmapConfig_ARGB_4444;
        case 6:
            return BitmapConfig_ALPHA_8;
    }
    assert(true && "native config value invalid.");
    return BitmapConfig_ALPHA_8;
}

Boolean CBitmap::CheckXYSign(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (x < 0) {
        Logger::E(TAG, "x must be >= 0");
        return FALSE;
    }
    if (y < 0) {
        Logger::E(TAG, "y must be >= 0");
        return FALSE;
    }
    return TRUE;
}

Boolean CBitmap::CheckPixelAccess(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (!CheckXYSign(x, y)) {
        return FALSE;
    }
    Int32 w = 0, h = 0;
    GetWidth(&w);
    GetHeight(&h);
    if (x >= w) {
        Logger::E(TAG, "x must be < bitmap.width()");
        return FALSE;
    }
    if (y >= h) {
        Logger::E(TAG, "y must be < bitmap.height()");
        return FALSE;
    }
    return TRUE;
}

Boolean CBitmap::CheckWidthHeight(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (width <= 0) {
        Logger::E(TAG, "width must be > 0");
        return FALSE;
    }
    if (height <= 0) {
        Logger::E(TAG, "height must be > 0");
        return FALSE;
    }
    return TRUE;
}

ECode CBitmap::CheckPixelsAccess(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ const ArrayOf<Int32>& pixels)
{
    if (!CheckXYSign(x, y)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!CheckWidthHeight(width, height)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 w = 0, h = 0;
    GetWidth(&w);
    GetHeight(&h);
    if (x + width > w) {
        Logger::E(TAG, "x + width must be <= bitmap.width()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (y + height > h) {
        Logger::E(TAG, "y + height must be <= bitmap.height()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (Math::Abs(stride) < width) {
        Logger::E(TAG, "abs(stride) must be >= width");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 lastScanline = offset + (height - 1) * stride;
    Int32 length = pixels.GetLength();
    if (offset < 0 || (offset + width > length)
            || lastScanline < 0
            || (lastScanline + width > length)) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////////
// Conversions to/from SkColor, for get/setPixels, and the create method, which
// is basically like setPixels

typedef void (*FromColorProc)(void* dst, const SkColor src[],
        int width, int x, int y);

static void FromColor_D32(void* dst, const SkColor src[],
        int width, int, int)
{
    SkPMColor* d = (SkPMColor*)dst;

    for (int i = 0; i < width; i++) {
        *d++ = SkPreMultiplyColor(*src++);
    }
}

static void FromColor_D565(void* dst, const SkColor src[],
        int width, int x, int y)
{
    uint16_t* d = (uint16_t*)dst;

    DITHER_565_SCAN(y);
    for (int stop = x + width; x < stop; x++) {
        SkColor c = *src++;
        *d++ = SkDitherRGBTo565(SkColorGetR(c), SkColorGetG(c), SkColorGetB(c),
                                DITHER_VALUE(x));
    }
}

static void FromColor_D4444(void* dst, const SkColor src[],
        int width, int x, int y)
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
static FromColorProc ChooseFromColorProc(SkBitmap::Config config)
{
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

//////////////////// ToColor procs /////////////////////

typedef void (*ToColorProc)(SkColor dst[], const void* src, int width,
                            SkColorTable*);

static void ToColor_S32_Alpha(SkColor dst[], const void* src, int width,
                              SkColorTable*)
{
    SkASSERT(width > 0);
    const SkPMColor* s = (const SkPMColor*)src;
    do {
        *dst++ = SkUnPreMultiply::PMColorToColor(*s++);
    } while (--width != 0);
}

static void ToColor_S32_Opaque(SkColor dst[], const void* src, int width,
                               SkColorTable*)
{
    SkASSERT(width > 0);
    const SkPMColor* s = (const SkPMColor*)src;
    do {
        SkPMColor c = *s++;
        *dst++ = SkColorSetRGB(SkGetPackedR32(c), SkGetPackedG32(c),
                               SkGetPackedB32(c));
    } while (--width != 0);
}

static void ToColor_S4444_Alpha(SkColor dst[], const void* src, int width,
                                SkColorTable*)
{
    SkASSERT(width > 0);
    const SkPMColor16* s = (const SkPMColor16*)src;
    do {
        *dst++ = SkUnPreMultiply::PMColorToColor(SkPixel4444ToPixel32(*s++));
    } while (--width != 0);
}

static void ToColor_S4444_Opaque(SkColor dst[], const void* src, int width,
                                 SkColorTable*)
{
    SkASSERT(width > 0);
    const SkPMColor* s = (const SkPMColor*)src;
    do {
        SkPMColor c = SkPixel4444ToPixel32(*s++);
        *dst++ = SkColorSetRGB(SkGetPackedR32(c), SkGetPackedG32(c),
                               SkGetPackedB32(c));
    } while (--width != 0);
}

static void ToColor_S565(SkColor dst[], const void* src, int width,
                         SkColorTable*)
{
    SkASSERT(width > 0);
    const uint16_t* s = (const uint16_t*)src;
    do {
        uint16_t c = *s++;
        *dst++ =  SkColorSetRGB(SkPacked16ToR32(c), SkPacked16ToG32(c),
                                SkPacked16ToB32(c));
    } while (--width != 0);
}

static void ToColor_SI8_Alpha(SkColor dst[], const void* src, int width,
                              SkColorTable* ctable)
{
    SkASSERT(width > 0);
    const uint8_t* s = (const uint8_t*)src;
    const SkPMColor* colors = ctable->lockColors();
    do {
        *dst++ = SkUnPreMultiply::PMColorToColor(colors[*s++]);
    } while (--width != 0);
    ctable->unlockColors(false);
}

static void ToColor_SI8_Opaque(SkColor dst[], const void* src, int width,
                               SkColorTable* ctable)
{
    SkASSERT(width > 0);
    const uint8_t* s = (const uint8_t*)src;
    const SkPMColor* colors = ctable->lockColors();
    do {
        SkPMColor c = colors[*s++];
        *dst++ = SkColorSetRGB(SkGetPackedR32(c), SkGetPackedG32(c),
                               SkGetPackedB32(c));
    } while (--width != 0);
    ctable->unlockColors(false);
}

// can return NULL
static ToColorProc ChooseToColorProc(const SkBitmap& src)
{
    switch (src.config()) {
        case SkBitmap::kARGB_8888_Config:
            return src.isOpaque() ? ToColor_S32_Opaque : ToColor_S32_Alpha;
        case SkBitmap::kARGB_4444_Config:
            return src.isOpaque() ? ToColor_S4444_Opaque : ToColor_S4444_Alpha;
        case SkBitmap::kRGB_565_Config:
            return ToColor_S565;
        case SkBitmap::kIndex8_Config:
            if (src.getColorTable() == NULL) {
                return NULL;
            }
            return src.isOpaque() ? ToColor_SI8_Opaque : ToColor_SI8_Alpha;
        default:
            break;
    }
    return NULL;
}

ECode CBitmap::NativeCreate(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ SkBitmap::Config nativeConfig,
    /* [in] */ Boolean isMutable,
    /* [out] */ CBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    if (width <= 0 || height <= 0) {
        Logger::E(TAG, "width and height must be > 0");
        return E_INVALID_ARGUMENT;
    }

    if (colors.GetPayload() != NULL) {
        Int32 n = colors.GetLength();
        if (n < (Int32)SkAbs32(stride) * height) {
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        }
    }

    SkBitmap bmp;

    bmp.setConfig(nativeConfig, width, height);
    FAIL_RETURN(Graphics::SetPixelRef(&bmp, NULL));

    FAIL_RETURN(Graphics::SetPixels(colors, offset, stride, 0, 0, width, height, bmp));

    return CBitmap::NewByFriend(
        (Int32)new SkBitmap(bmp), isMutable, ArrayOf<Byte>(NULL, 0), -1, bitmap);
}

ECode CBitmap::NativeCopy(
    /* [in] */ SkBitmap* srcBitmap,
    /* [in] */ Int32 nativeConfig,
    /* [in] */ Boolean isMutable,
    /* [out] */ CBitmap** bitmap)
{
    assert(srcBitmap);

    SkBitmap                    result;
    Graphics::PixelAllocator    allocator;

    if (!srcBitmap->copyTo(&result, (SkBitmap::Config)nativeConfig, &allocator)) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    return CBitmap::NewByFriend(
        (Int32)new SkBitmap(result), isMutable, ArrayOf<Byte>(NULL, 0), -1, bitmap);
}

void CBitmap::NativeDestructor(
    /* [in] */ SkBitmap* nativeBitmap)
{
    delete nativeBitmap;
}

void CBitmap::NativeRecycle(
    /* [in] */ SkBitmap* nativeBitmap)
{
    assert(nativeBitmap);
    nativeBitmap->setPixels(NULL, NULL);
}

Boolean CBitmap::NativeCompress(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ Int32 format,
    /* [in] */ Int32 quality,
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* tempStorage)
{
    // These must match the int values in Bitmap.java
    enum JavaEncodeFormat {
        kJPEG_JavaEncodeFormat = 0,
        kPNG_JavaEncodeFormat = 1
    };

    SkImageEncoder::Type fm;

    switch (format) {
    case kJPEG_JavaEncodeFormat:
        fm = SkImageEncoder::kJPEG_Type;
        break;
    case kPNG_JavaEncodeFormat:
        fm = SkImageEncoder::kPNG_Type;
        break;
    default:
        return FALSE;
    }

    Boolean success = FALSE;
    SkWStream* strm = CreateOutputStreamAdaptor(stream, tempStorage);
    if (NULL != strm) {
        SkImageEncoder* encoder = SkImageEncoder::Create(fm);
        if (NULL != encoder) {
            success = encoder->encodeStream(strm, *nativeBitmap, quality);
            delete encoder;
        }
        delete strm;
    }
    return success;
}

void CBitmap::NativeErase(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ Int32 color)
{
    assert(nativeBitmap);
    nativeBitmap->eraseColor(color);
}

Int32 CBitmap::NativeWidth(
    /* [in] */ SkBitmap* nativeBitmap)
{
    assert(nativeBitmap);
    return nativeBitmap->width();
}

Int32 CBitmap::NativeHeight(
    /* [in] */ SkBitmap* nativeBitmap)
{
    assert(nativeBitmap);
    return nativeBitmap->height();
}

Int32 CBitmap::NativeRowBytes(
    /* [in] */ SkBitmap* nativeBitmap)
{
    assert(nativeBitmap);
    return nativeBitmap->rowBytes();
}

Int32 CBitmap::NativeConfig(
    /* [in] */ SkBitmap* nativeBitmap)
{
    assert(nativeBitmap);
    return nativeBitmap->config();
}

Boolean CBitmap::NativeHasAlpha(
    /* [in] */ SkBitmap* nativeBitmap)
{
    assert(nativeBitmap);
    return (Boolean)!nativeBitmap->isOpaque();
}

Int32 CBitmap::NativeGetPixel(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    assert(nativeBitmap);

    SkAutoLockPixels alp(*nativeBitmap);

    ToColorProc proc = ChooseToColorProc(*nativeBitmap);
    if (NULL == proc) {
        return 0;
    }
    const void* src = nativeBitmap->getAddr(x, y);
    if (NULL == src) {
        return 0;
    }

    SkColor dst[1];
    proc(dst, src, 1, nativeBitmap->getColorTable());
    return dst[0];
}

void CBitmap::NativeGetPixels(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ const ArrayOf<Int32>& pixels,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    assert(nativeBitmap);

    SkAutoLockPixels alp(*nativeBitmap);

    ToColorProc proc = ChooseToColorProc(*nativeBitmap);
    if (NULL == proc) {
        return;
    }
    const void* src = nativeBitmap->getAddr(x, y);
    if (NULL == src) {
        return;
    }

    SkColorTable* ctable = nativeBitmap->getColorTable();
    Int32* dst = pixels.GetPayload();
    SkColor* d = (SkColor*)dst + offset;
    while (--height >= 0) {
        proc(d, src, width, ctable);
        d += stride;
        src = (void*)((const char*)src + nativeBitmap->rowBytes());
    }
}

void CBitmap::NativeSetPixel(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 color)
{
    assert(nativeBitmap);

    SkAutoLockPixels alp(*nativeBitmap);
    if (NULL == nativeBitmap->getPixels()) {
        return;
    }

    FromColorProc proc = ChooseFromColorProc(nativeBitmap->config());
    if (NULL == proc) {
        return;
    }

    proc(nativeBitmap->getAddr(x, y), (SkColor*)&color, 1, x, y);
}

void CBitmap::NativeSetPixels(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    assert(nativeBitmap);
    Graphics::SetPixels(colors, offset, stride, x, y, width, height, *nativeBitmap);
}

void CBitmap::NativeCopyPixelsToBuffer(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ IBuffer* dst)
{
    assert(nativeBitmap);

    SkAutoLockPixels alp(*nativeBitmap);
    const void* src = nativeBitmap->getPixels();

    if (NULL != src) {
        AutoBufferPointer abp(dst);
        // the java side has already checked that buffer is large enough
        memcpy(abp.Pointer(), src, nativeBitmap->getSize());
    }
}

void CBitmap::NativeCopyPixelsFromBuffer(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ IBuffer* src)
{
    assert(nativeBitmap);

    SkAutoLockPixels alp(*nativeBitmap);
    void* dst = nativeBitmap->getPixels();

    if (NULL != dst) {
        AutoBufferPointer abp(src);
        // the java side has already checked that buffer is large enough
        memcpy(dst, abp.Pointer(), nativeBitmap->getSize());
    }
}

ECode CBitmap::NativeCreateFromParcel(
    /* [in] */ IParcel* p,
    /* [out] */ CBitmap** bitmap)
{
    assert(p);
    assert(bitmap);

    Int32 value = 0;
    p->ReadInt32(&value);
    const Boolean isMutable = value != 0;

    p->ReadInt32(&value);
    const SkBitmap::Config  config = (SkBitmap::Config)value;

    Int32 width = 0;
    p->ReadInt32(&width);

    Int32 height = 0;
    p->ReadInt32(&height);

    Int32 rowBytes = 0;
    p->ReadInt32(&rowBytes);

    Int32 density = 0;
    p->ReadInt32(&density);

    if (SkBitmap::kARGB_8888_Config != config &&
            SkBitmap::kRGB_565_Config != config &&
            SkBitmap::kARGB_4444_Config != config &&
            SkBitmap::kIndex8_Config != config &&
            SkBitmap::kA8_Config != config) {
        Logger::E(TAG, StringBuffer(
            "Bitmap_createFromParcel unknown config: %d\n") + (Int32)config);
        return E_INVALID_ARGUMENT;
    }

    SkBitmap* bmp = new SkBitmap;

    bmp->setConfig(config, width, height, rowBytes);

    SkColorTable* ctable = NULL;
    if (config == SkBitmap::kIndex8_Config) {
        AutoFree< ArrayOf<Int32> > array;
        p->ReadArrayOf((Handle32*)&array);
        Int32 count = array->GetLength();
        if (count > 0) {
            const SkPMColor* src = (const SkPMColor*)array->GetPayload();
            ctable = new SkColorTable(src, count);
        }
    }

    ECode ec = Graphics::SetPixelRef(bmp, ctable);
    if (FAILED(ec)) {
        ctable->safeUnref();
        delete bmp;
        return ec;
    }

    ctable->safeUnref();

    bmp->lockPixels();
    AutoFree< ArrayOf<Int32> > array;
    p->ReadArrayOf((Handle32*)&array);
    memcpy(bmp->getPixels(), (void*)array->GetPayload(), array->GetLength());
    bmp->unlockPixels();

    return CBitmap::NewByFriend(
        (Int32)bmp, isMutable, ArrayOf<Byte>(NULL, 0), density, bitmap);
}

Boolean CBitmap::NativeWriteToParcel(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ Boolean isMutable,
    /* [in] */ Int32 density,
    /* [in] */ IParcel* p)
{
    assert(nativeBitmap);
    assert(p);

    p->WriteInt32(isMutable);
    p->WriteInt32(nativeBitmap->config());
    p->WriteInt32(nativeBitmap->width());
    p->WriteInt32(nativeBitmap->height());
    p->WriteInt32(nativeBitmap->rowBytes());
    p->WriteInt32(density);

    if (nativeBitmap->getConfig() == SkBitmap::kIndex8_Config) {
        SkColorTable* ctable = nativeBitmap->getColorTable();
        if (ctable != NULL) {
            Int32 count = (Int32)ctable->count();
            ArrayOf<Int32> array((Int32*)ctable->lockColors(), count);
            p->WriteArrayOf((Handle32)&array);
            ctable->unlockColors(false);
        }
        else {
            ArrayOf<Int32> array(NULL, 0);
            p->WriteArrayOf((Handle32)&array); // indicate no ctable
        }
    }

    size_t size = nativeBitmap->getSize();
    nativeBitmap->lockPixels();
    ArrayOf<Byte> array((Byte*)nativeBitmap->getPixels(), size);
    p->WriteArrayOf((Handle32)&array);
    nativeBitmap->unlockPixels();
    return TRUE;
}

ECode CBitmap::NativeExtractAlpha(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ SkPaint* nativePaint,
    /* [in] */ ArrayOf<Int32>* offsetXY,
    /* [out] */ CBitmap** bitmap)
{
    assert(nativeBitmap);

    SkIPoint  offset;
    SkBitmap* dst = new SkBitmap;

    nativeBitmap->extractAlpha(dst, nativePaint, &offset);
    if (offsetXY != 0 && offsetXY->GetLength()>= 2) {
        Int32* array = offsetXY->GetPayload();
        array[0] = offset.fX;
        array[1] = offset.fY;
    }

    return CBitmap::NewByFriend(
        (Int32)dst, TRUE, ArrayOf<Byte>(NULL, 0), -1, bitmap);
}

void CBitmap::NativePrepareToDraw(
    /* [in] */ SkBitmap* nativeBitmap)
{
    assert(nativeBitmap);
    nativeBitmap->lockPixels();
    nativeBitmap->unlockPixels();
}

void CBitmap::NativeSetHasAlpha(
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ Boolean hasAlpha)
{
    assert(bitmap);
    bitmap->setIsOpaque(!hasAlpha);
}

Boolean CBitmap::NativeSameAs(
    /* [in] */ SkBitmap* nb0,
    /* [in] */ SkBitmap* nb1)
{
    assert(nb0);
    assert(nb1);

    if (nb0->width() != nb1->width() ||
        nb0->height() != nb1->height() ||
        nb0->config() != nb1->config()) {
        return FALSE;
    }

    SkAutoLockPixels alp0(*nb0);
    SkAutoLockPixels alp1(*nb1);

    // if we can't load the pixels, return FALSE
    if (NULL == nb0->getPixels() || NULL == nb1->getPixels()) {
        return FALSE;
    }

    if (nb0->config() == SkBitmap::kIndex8_Config) {
        SkColorTable* ct0 = nb0->getColorTable();
        SkColorTable* ct1 = nb1->getColorTable();
        if (NULL == ct0 || NULL == ct1) {
            return FALSE;
        }
        if (ct0->count() != ct1->count()) {
            return FALSE;
        }

        SkAutoLockColors alc0(ct0);
        SkAutoLockColors alc1(ct1);
        const size_t size = ct0->count() * sizeof(SkPMColor);
        if (memcmp(alc0.colors(), alc1.colors(), size) != 0) {
            return FALSE;
        }
    }

    // now compare each scanline. We can't do the entire buffer at once,
    // since we don't care about the pixel values that might extend beyond
    // the width (since the scanline might be larger than the logical width)
    const Int32 h = nb0->height();
    const size_t size = nb0->width() * nb0->bytesPerPixel();
    for (Int32 y = 0; y < h; y++) {
        if (memcmp(nb0->getAddr(0, y), nb1->getAddr(0, y), size) != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

