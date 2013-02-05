
#ifndef __CLARGEBITMAP_H__
#define __CLARGEBITMAP_H__

#include "_CLargeBitmap.h"
#include <skia/images/SkBitmapRegionDecoder.h>


CarClass(CLargeBitmap)
{
public:
    CARAPI decodeRegion(
        /* [in] */ IRect * pRect,
        /* [in] */ IBitmapFactoryOptions * pOptions,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI getWidth(
        /* [out] */ Int32 * pLength);

    CARAPI getHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI recycle();

    CARAPI isRecycled(
        /* [out] */ Boolean * pResult);

    CARAPI checkRecycled(
        /* [in] */ Int32 errorMessage);

    CARAPI constructor(
        /* [in] */ Int32 ibm);

private:
    Int32 mNativeLargeBitmap;
    Boolean mRecycled;

    static IBitmap* nativeDecodeRegion(
                                    SkBitmapRegionDecoder * brd,
                                    Int32 start_x,
                                    Int32 start_y,
                                    Int32 width,
                                    Int32 height,
                                    IBitmapFactoryOptions * options);

    static Int32 nativeGetWidth(Int32 lbm);

    static Int32 nativeGetHeight(Int32 lbm);

    static void nativeClean(Int32 lbm);

};

#endif // __CLARGEBITMAP_H__
