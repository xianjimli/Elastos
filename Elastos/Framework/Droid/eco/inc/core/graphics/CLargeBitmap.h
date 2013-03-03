
#ifndef __CLARGEBITMAP_H__
#define __CLARGEBITMAP_H__

#include "_CLargeBitmap.h"
#include <skia/images/SkBitmapRegionDecoder.h>


CarClass(CLargeBitmap)
{
public:
    ~CLargeBitmap();

    CARAPI DecodeRegion(
        /* [in] */ IRect* rect,
        /* [in] */ IBitmapFactoryOptions* options,
        /* [out] */ IBitmap** bitmap);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI Recycle();

    CARAPI IsRecycled(
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ Handle32 lbm);

private:
    CARAPI CheckRecycled(
        /* [in] */ CString errorMessage);

    static CARAPI NativeDecodeRegion(
        /* [in] */ SkBitmapRegionDecoder* brd,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IBitmapFactoryOptions* options,
        /* [out] */ IBitmap** bitmap);

    static CARAPI_(Int32) NativeGetWidth(
        /* [in] */ SkBitmapRegionDecoder* lbm);

    static CARAPI_(Int32) NativeGetHeight(
        /* [in] */ SkBitmapRegionDecoder* lbm);

    static CARAPI_(void) NativeClean(
        /* [in] */ SkBitmapRegionDecoder* lbm);

private:
    SkBitmapRegionDecoder* mNativeLargeBitmap;
    Boolean mRecycled;
};

#endif // __CLARGEBITMAP_H__
