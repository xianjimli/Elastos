
#ifndef __CBITMAPREGIONDECODER_H__
#define __CBITMAPREGIONDECODER_H__

#include "_CBitmapRegionDecoder.h"
#include <skia/images/SkBitmapRegionDecoder.h>

CarClass(CBitmapRegionDecoder)
{
public:
    ~CBitmapRegionDecoder();
    CARAPI newInstance(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder ** ppIBrd);

    CARAPI newInstanceEx(
        /* [in] */ IFileDescriptor * pFd,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder ** ppIBrd);

    CARAPI newInstanceEx2(
        /* [in] */ IInputStream * pIs,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder ** ppIBrd);

    CARAPI newInstanceEx3(
        /* [in] */ const String& pathName,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder ** ppIBrd);

    CARAPI decodeRegion(
        /* [in] */ IRect * pRect,
        /* [in] */ IBitmapFactoryOptions * pOptions,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI getWidth(
        /* [out] */ Int32 * pWidth);

    CARAPI getHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI recycle();

    CARAPI isRecycled(
        /* [out] */ Boolean * pResult);

    CARAPI checkRecycled(
        /* [in] */ const String& errorMessage);

    CARAPI constructor(
        /* [in] */ Int32 decoder);

private:
    SkBitmapRegionDecoder* mNativeBitmapRegionDecoder;
    Boolean mRecycled;

    static IBitmap* nativeDecodeRegion(
                        SkBitmapRegionDecoder* brd,
                        Int32 start_x,
                        Int32 start_y,
                        Int32 width,
                        Int32 height,
                        IBitmapFactoryOptions* options);
    static Int32 nativeGetWidth(SkBitmapRegionDecoder* brd);
    static Int32 nativeGetHeight(SkBitmapRegionDecoder* brd);
    static void nativeClean(SkBitmapRegionDecoder* brd);

    static IBitmapRegionDecoder* nativeNewInstance(
                        const ArrayOf<Byte> & data,
                        Int32 offset,
                        Int32 length,
                        Boolean isShareable);
    static IBitmapRegionDecoder* nativeNewInstance(
                        IFileDescriptor* fd,
                        Boolean isShareable);
    static IBitmapRegionDecoder* nativeNewInstance(
                        IInputStream* is,
                        //ArrayOf<Byte> & storage,
                        Byte* storage,
                        Boolean isShareable);
    static IBitmapRegionDecoder* nativeNewInstance(
                        Int32 asset,
                        Boolean isShareable);

};

#endif // __CBITMAPREGIONDECODER_H__
