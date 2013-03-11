
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "ext/frameworkext.h"
#include <skia/core/SkRect.h>
#include <skia/core/SkBitmap.h>
#include <skia/images/SkBitmapRegionDecoder.h>

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
    static CARAPI_(SkRect*) IRect2SkRect(
        /* [in] */ IRect* obj,
        /* [in] */ SkRect* r);

    static CARAPI_(SkIRect*) IRect2SkIRect(
    /* [in] */ IRect* obj,
    /* [in] */ SkIRect* sr);

    static CARAPI_(void) SkIRect2IRect(
        /* [in] */ const SkIRect& ir,
        /* [in] */ IRect* obj);

    static CARAPI_(SkRect*) IRectF2SkRect(
        /* [in] */ IRectF* obj,
        /* [in] */ SkRect* r);

    static CARAPI_(void) SkRect2IRectF(
        /* [in] */ const SkRect& r,
        /* [in] */ IRectF* obj);

    static CARAPI_(SkBitmap::Config) GetNativeBitmapConfig(
        /* [in] */ BitmapConfig config);

    /** Create a java Bitmap object given the native bitmap (required) and optional
        storage array (may be null). If storage is specified, then it must already be
        locked, and its native address set as the bitmap's pixels. If storage is null,
        then the bitmap must be an owner of its natively allocated pixels (via allocPixels).
        */
    static CARAPI CreateBitmap(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Boolean isMutable,
        /* [in] */ ArrayOf<Byte>* ninePatch,
        /* [in] */ Int32 density,
        /* [out] */ IBitmap** bitmap);

    static CARAPI CreateBitmapRegionDecoder(
        /* [in] */ SkBitmapRegionDecoder* bitmap,
        /* [out] */ IBitmapRegionDecoder** decoder);

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
    static const char* TAG;
};

class CppPixelAllocator : public SkBitmap::Allocator
{
public:
    CppPixelAllocator(
        // JNIEnv* env,
        /* [in] */ bool reportSizeToVM);

    // overrides
    virtual bool allocPixelRef(
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ SkColorTable* ctable);

private:
    // JavaVM* fVM;
    bool fReportSizeToVM;
};

class CppMemoryUsageReporter : public SkVMMemoryReporter
{
public:
    CppMemoryUsageReporter(/*JNIEnv* env*/);

    virtual ~CppMemoryUsageReporter();

    // overrides
    virtual bool reportMemory(size_t memorySize);

private:
    // JavaVM* fVM;
    size_t fTotalSize;
};

#endif //__GRAPHICS_H__
