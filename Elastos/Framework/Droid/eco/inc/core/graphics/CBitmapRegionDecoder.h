
#ifndef __CBITMAPREGIONDECODER_H__
#define __CBITMAPREGIONDECODER_H__

#include "_CBitmapRegionDecoder.h"
#include <skia/images/SkBitmapRegionDecoder.h>

CarClass(CBitmapRegionDecoder)
{
public:
    ~CBitmapRegionDecoder();

    CARAPI DecodeRegion(
        /* [in] */ IRect * pRect,
        /* [in] */ IBitmapFactoryOptions * pOptions,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI GetWidth(
        /* [out] */ Int32 * pWidth);

    CARAPI GetHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI Recycle();

    CARAPI IsRecycled(
        /* [out] */ Boolean * pResult);

    CARAPI CheckRecycled(
        /* [in] */ const String& errorMessage);

    CARAPI constructor(
        /* [in] */ Int32 decoder);

private:
    SkBitmapRegionDecoder* mNativeBitmapRegionDecoder;
    Boolean mRecycled;

    static IBitmap* NativeDecodeRegion(
        /* [in] */ SkBitmapRegionDecoder* brd,
        /* [in] */ Int32 start_x,
        /* [in] */ Int32 start_y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IBitmapFactoryOptions* options);

    static Int32 NativeGetWidth(
        /* [in] */ SkBitmapRegionDecoder* brd);

    static Int32 NativeGetHeight(
        /* [in] */ SkBitmapRegionDecoder* brd);

    static void NativeClean(
        /* [in] */ SkBitmapRegionDecoder* brd);
};

#endif // __CBITMAPREGIONDECODER_H__
