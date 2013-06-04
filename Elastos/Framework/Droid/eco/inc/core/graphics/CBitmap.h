
#ifndef __CBITMAP_H__
#define __CBITMAP_H__

#include "_CBitmap.h"
#include "elastos/AutoPtr.h"
#include "elastos/AutoFree.h"
#include "elastos/Mutex.h"
#include <graphics/Canvas.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkPaint.h>

class CBitmapFactory;

CarClass(CBitmap)
{
    friend class Canvas;
    friend class CBitmapFactory;

public:
    static const Int32 DENSITY_NONE = 0;

public:
    static CARAPI_(int) ScaleFromDensity(int size, int sdensity, int tdensity);

    CBitmap();

public:
    CARAPI GetDensity(
        /* [out] */ Int32* density);

    CARAPI SetDensity(
        /* [in] */ Int32 density);

    static CARAPI_(void) SetDefaultDensity(
        /* [in] */ Int32 density);

    CARAPI SetNinePatchChunk(
        /* [in] */ const ArrayOf<Byte> & chunk);

    /**
     * Free up the memory associated with this bitmap's pixels, and mark the
     * bitmap as "dead", meaning it will throw an exception if getPixels() or
     * setPixels() is called, and will draw nothing. This operation cannot be
     * reversed, so it should only be called if you are sure there are no
     * further uses for the bitmap. This is an advanced call, and normally need
     * not be called, since the normal GC process will free up this memory when
     * there are no more references to this bitmap.
     */
    CARAPI Recycle();

    CARAPI IsRecycled(
        /* [out] */ Boolean* isRecycled);

    CARAPI CopyPixelsToBuffer(
        /* [in] */ IBuffer* dst);

    CARAPI CopyPixelsFromBuffer(
        /* [in] */ IBuffer* src);

    CARAPI Copy(
        /* [in] */ BitmapConfig config,
        /* [in] */ Boolean isMutable,
        /* [out] */ IBitmap** bitmap);

    CARAPI GetNinePatchChunk(
        /* [out, callee] */ ArrayOf<Byte>** data);

    CARAPI Compress(
        /* [in] */ BitmapCompressFormat format,
        /* [in] */ Int32 quality,
        /* [in] */ IOutputStream* stream,
        /* [out] */ Boolean* isSucceeded);

    CARAPI IsMutable(
        /* [out] */ Boolean* isMutable);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI GetScaledWidth(
        /* [in] */ ICanvas* canvas,
        /* [out] */ Int32* width);

    CARAPI GetScaledHeight(
        /* [in] */ ICanvas* canvas,
        /* [out] */ Int32* height);

    CARAPI GetScaledWidthEx(
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Int32* width);

    CARAPI GetScaledHeightEx(
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Int32* height);

    CARAPI GetScaledWidthEx2(
        /* [in] */ Int32 targetDensity,
        /* [out] */ Int32* width);

    CARAPI GetScaledHeightEx2(
        /* [in] */ Int32 targetDensity,
        /* [out] */ Int32* height);

    CARAPI GetRowBytes(
        /* [out] */ Int32* bytes);

    CARAPI GetConfig(
        /* [out] */ BitmapConfig* config);

    CARAPI HasAlpha(
        /* [out] */ Boolean* isAlpha);

    CARAPI SetHasAlpha(
        /* [in] */ Boolean hasAlpha);

    CARAPI EraseColor(
        /* [in] */ Int32 c);

    CARAPI GetPixel(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Int32* pixel);

    CARAPI GetPixels(
        /* [in] */ const ArrayOf<Int32> & pixels,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetPixel(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 color);

    CARAPI SetPixels(
        /* [in] */ const ArrayOf<Int32>& pixels,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

    CARAPI ExtractAlpha(
        /* [out] */ IBitmap** bitmap);

    CARAPI ExtractAlphaEx(
        /* [in] */ IPaint* paint,
        /* [in] */ const ArrayOf<Int32>& offsetXY,
        /* [out] */ IBitmap** bitmap);

    CARAPI SameAs(
        /* [in] */ IBitmap* other,
        /* [out] */ Boolean* isSame);

    CARAPI PrepareToDraw();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /*private*/ constructor(
        /* [in] */ Handle32 nativeBitmap,
        /* [in] */ Boolean isMutable,
        /* [in] */ ArrayOf<Byte>* ninePatchChunk,
        /* [in] */ Int32 density);

    static CARAPI CreateScaledBitmap(
        /* [in] */ IBitmapFactory* factory,
        /* [in] */ IBitmap* src,
        /* [in] */ Int32 dstWidth,
        /* [in] */ Int32 dstHeight,
        /* [in] */ Boolean filter,
        /* [out] */ IBitmap** bitmap);

    SkBitmap* Ni();

private:
    static CARAPI_(Int32) GetDefaultDensity();

    /*  Private constructor that must received an already allocated native
        bitmap int (pointer).

        This can be called from JNI code.
    */
    CARAPI Init(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Boolean isMutable,
        /* [in] */ ArrayOf<Byte>* ninePatchChunk,
        /* [in] */ Int32 density = -1);

    CARAPI_(BitmapConfig) Config_NativeToConfig(
        /* [in] */ Int32 config);

    static CARAPI_(Boolean) CheckXYSign(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    static CARAPI_(Boolean) CheckWidthHeight(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(Boolean) CheckPixelAccess(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI CheckPixelsAccess(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ const ArrayOf<Int32>& pixels);

    static CARAPI NativeCreate(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ SkBitmap::Config nativeConfig,
        /* [in] */ Boolean isMutable,
        /* [out] */ CBitmap** bitmap);

    static CARAPI NativeCopy(
        /* [in] */ SkBitmap* srcBitmap,
        /* [in] */ Int32 nativeConfig,
        /* [in] */ Boolean isMutable,
        /* [out] */ CBitmap** bitmap);

    static CARAPI_(void) NativeDestructor(
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(void) NativeRecycle(
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(Boolean) NativeCompress(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Int32 format,
        /* [in] */ Int32 quality,
        /* [in] */ IOutputStream* stream,
        /* [in] */ ArrayOf<Byte>* tempStorage);

    static CARAPI_(void) NativeErase(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Int32 color);

    static CARAPI_(Int32) NativeWidth(
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(Int32) NativeHeight(
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(Int32) NativeRowBytes(
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(Int32) NativeConfig(
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(Boolean) NativeHasAlpha(
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(Int32) NativeGetPixel(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    static CARAPI_(void) NativeGetPixels(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ const ArrayOf<Int32>& pixels,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static CARAPI_(void) NativeSetPixel(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 color);

    static CARAPI_(void) NativeSetPixels(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static CARAPI_(void) NativeCopyPixelsToBuffer(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ IBuffer* dst);

    static CARAPI_(void) NativeCopyPixelsFromBuffer(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ IBuffer* src);

    static CARAPI NativeCreateFromParcel(
        /* [in] */ IParcel* p,
        /* [out] */ CBitmap** bitmap);

    static CARAPI_(Boolean) NativeWriteToParcel(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Boolean isMutable,
        /* [in] */ Int32 density,
        /* [in] */ IParcel* p);

    static CARAPI NativeExtractAlpha(
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ SkPaint* nativePaint,
        /* [in] */ ArrayOf<Int32>* offsetXY,
        /* [out] */ CBitmap** bitmap);

    static CARAPI_(void) NativePrepareToDraw(
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(void) NativeSetHasAlpha(
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ Boolean hasAlpha);

    static CARAPI_(Boolean) NativeSameAs(
        /* [in] */ SkBitmap* nb0,
        /* [in] */ SkBitmap* nb1);

public:
    Int32           mDensity;

private:
    static const char* TAG;

    static Mutex sClsLock;

    // Note:  mNativeBitmap is used by FaceDetector_jni.cpp
    // Don't change/rename without updating FaceDetector_jni.cpp
    SkBitmap*       mNativeBitmap;
    Boolean         mIsMutable;
    AutoFree< ArrayOf<Byte> >  mNinePatchChunk;   // may be null
    Int32           mWidth;
    Int32           mHeight;
    Boolean         mRecycled;

    static AutoPtr<IMatrix> sScaleMatrix;

    static Int32 sDefaultDensity;

    /**
     * Number of bytes of temp storage we use for communicating between the
     * native compressor and the java OutputStream.
     */
    static Int32 WORKING_COMPRESS_STORAGE;
};

#endif // __CBITMAP_H__

