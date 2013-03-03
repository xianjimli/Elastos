
#ifndef __CYUVIMAGE_H__
#define __CYUVIMAGE_H__

#include "_CYuvImage.h"
#include "graphics/CRect.h"
#include <elastos/AutoFree.h>

CarClass(CYuvImage)
{
public:
    CARAPI CompressToJpeg(
        /* [in] */ IRect* rectangle,
        /* [in] */ Int32 quality,
        /* [in] */ IOutputStream* stream,
        /* [out] */ Boolean* result);

    CARAPI GetYuvData(
        /* [out, callee] */ ArrayOf<Byte>** data);

    CARAPI GetYuvFormat(
        /* [out] */ Int32* format);

    CARAPI GetStrides(
        /* [out, callee] */ ArrayOf<Int32>** strides);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& yuv,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ ArrayOf<Int32>* strides);

    // static Boolean nativeCompressToJpeg(ArrayOf<Byte> oriYuv,
    //     int format, int width, int height, ArrayOf<Int32> offsets, ArrayOf<Int32> strides,
    //     int quality, OutputStream stream, ArrayOf<Byte> tempStorage);

private:
    CARAPI_(ArrayOf<Int32>*) CalculateOffsets(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top);

    CARAPI_(ArrayOf<Int32>*) CalculateStrides(
        /* [in] */ Int32 width,
        /* [in] */ Int32 format);

    CARAPI_(void) AdjustRectangle(
        /* [in] */ IRect* rect);

    static CARAPI_(Boolean) NativeCompressToJpeg(
        /* [in] */ ArrayOf<Byte>& oriYuv,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ ArrayOf<Int32> & offsets,
        /* [in] */ ArrayOf<Int32> & strides,
        /* [in] */ Int32 quality,
        /* [in] */ IOutputStream* stream,
        /* [out] */ ArrayOf<Byte> & tempStorage);

private:
    /**
     * Number of bytes of temp storage we use for communicating between the
     * native compressor and the java OutputStream.
     */
    static const Int32 WORKING_COMPRESS_STORAGE = 4096;

    /**
     * The YUV format as defined in {@link PixelFormat}.
     */
    Int32 mFormat;

    /**
     * The raw YUV data.
     * In the case of more than one image plane, the image planes must be
     * concatenated into a single byte array.
     */
    AutoFree< ArrayOf<Byte> > mData;

    /**
     * The number of row bytes in each image plane.
     */
    AutoFree< ArrayOf<Int32> > mStrides;

    /**
     * The width of the image.
     */
    Int32 mWidth;

    /**
     * The height of the the image.
     */
    Int32 mHeight;

};

#endif // __CYUVIMAGE_H__
