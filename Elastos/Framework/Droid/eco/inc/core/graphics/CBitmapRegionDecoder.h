
#ifndef __CBITMAPREGIONDECODER_H__
#define __CBITMAPREGIONDECODER_H__

#include "_CBitmapRegionDecoder.h"
#include <skia/images/SkBitmapRegionDecoder.h>

CarClass(CBitmapRegionDecoder)
{
public:
    ~CBitmapRegionDecoder();

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
        /* [in] */ Handle32 decoder);

    /**
     * Create a BitmapRegionDecoder from the specified byte array.
     * Currently only the JPEG and PNG formats are supported.
     *
     * @param data byte array of compressed image data.
     * @param offset offset into data for where the decoder should begin
     *               parsing.
     * @param length the number of bytes, beginning at offset, to parse
     * @param isShareable If this is true, then the BitmapRegionDecoder may keep a
     *                    shallow reference to the input. If this is false,
     *                    then the BitmapRegionDecoder will explicitly make a copy of the
     *                    input data, and keep that. Even if sharing is allowed,
     *                    the implementation may still decide to make a deep
     *                    copy of the input data. If an image is progressively encoded,
     *                    allowing sharing may degrade the decoding speed.
     * @return BitmapRegionDecoder, or null if the image data could not be decoded.
     * @throws IOException if the image format is not supported or can not be decoded.
     */
    static CARAPI NewInstance(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NewInstance(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NewInstance(
        /* [in] */ IInputStream* is,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NewInstance(
        /* [in] */ const String& pathName,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

private:
    CARAPI CheckRecycled(
        /* [in] */ CString errorMessage);

    static CARAPI NativeDecodeRegion(
        /* [in] */ SkBitmapRegionDecoder* brd,
        /* [in] */ Int32 start_x,
        /* [in] */ Int32 start_y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IBitmapFactoryOptions* options,
        /* [out] */ IBitmap** bitmap);

    static CARAPI_(Int32) NativeGetWidth(
        /* [in] */ SkBitmapRegionDecoder* brd);

    static CARAPI_(Int32) NativeGetHeight(
        /* [in] */ SkBitmapRegionDecoder* brd);

    static CARAPI_(void) NativeClean(
        /* [in] */ SkBitmapRegionDecoder* brd);

    static CARAPI NativeNewInstance(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NativeNewInstance(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NativeNewInstance(
        /* [in] */ IInputStream* is,
        /* [in] */ //ArrayOf<Byte> & storage,
        /* [in] */ Byte* storage,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NativeNewInstance(
        /* [in] */ Int32 asset,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

private:
    SkBitmapRegionDecoder* mNativeBitmapRegionDecoder;
    Boolean mRecycled;
};

#endif // __CBITMAPREGIONDECODER_H__
