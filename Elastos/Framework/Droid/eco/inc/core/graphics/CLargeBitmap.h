
#ifndef __CLARGEBITMAP_H__
#define __CLARGEBITMAP_H__

#include "_CLargeBitmap.h"
#include <skia/images/SkBitmapRegionDecoder.h>


CarClass(CLargeBitmap)
{
public:
    CARAPI DecodeRegion(
        /* [in] */ IRect * pRect,
        /* [in] */ IBitmapFactoryOptions * pOptions,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI GetWidth(
        /* [out] */ Int32 * pLength);

    CARAPI GetHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI Recycle();

    CARAPI IsRecycled(
        /* [out] */ Boolean * pResult);

    CARAPI CheckRecycled(
        /* [in] */ Int32 errorMessage);

    CARAPI constructor(
        /* [in] */ Int32 ibm);

private:
    Int32 mNativeLargeBitmap;
    Boolean mRecycled;

    static IBitmap* NativeDecodeRegion(
        /* [in] */ SkBitmapRegionDecoder * brd,
        /* [in] */ Int32 start_x,
        /* [in] */ Int32 start_y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IBitmapFactoryOptions * options);

    static Int32 NativeGetWidth(
        /* [in] */ Int32 lbm);

    static Int32 NativeGetHeight(
        /* [in] */ Int32 lbm);

    static void NativeClean(
        /* [in] */ Int32 lbm);
};

#endif // __CLARGEBITMAP_H__
