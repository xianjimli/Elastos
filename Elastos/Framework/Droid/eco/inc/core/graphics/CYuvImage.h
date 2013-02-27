
#ifndef __CYUVIMAGE_H__
#define __CYUVIMAGE_H__

#include "_CYuvImage.h"
#include "graphics/CRect.h"
#include <elastos/AutoFree.h>

CarClass(CYuvImage)
{
public:
    CARAPI CompressToJpeg(
        /* [in] */ IRect * pRectangle,
        /* [in] */ Int32 quality,
        /* [in] */ IOutputStream * pStream,
        /* [out] */ Boolean * pResult);

    CARAPI GetYuvData(
        /* [out, callee] */ ArrayOf<Byte> ** ppData);

    CARAPI GetYuvFormat(
        /* [out] */ Int32 * pFormat);

    CARAPI GetStrides(
        /* [out, callee] */ ArrayOf<Int32> ** ppStrides);

    CARAPI GetWidth(
        /* [out] */ Int32 * pWidth);

    CARAPI GetHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte> & yuv,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ const ArrayOf<Int32> & strides);

    // static Boolean nativeCompressToJpeg(ArrayOf<Byte> oriYuv,
    //     int format, int width, int height, ArrayOf<Int32> offsets, ArrayOf<Int32> strides,
    //     int quality, OutputStream stream, ArrayOf<Byte> tempStorage);

private:
    CARAPI CalculateOffsets(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [out, callee] */ ArrayOf<Int32> ** ppOffsets);

    CARAPI CalculateStrides(
        /* [in] */ Int32 width,
        /* [in] */ Int32 format,
        /* [out, callee] */ ArrayOf<Int32> ** ppStrides);

    CARAPI AdjustRectangle(
        /* [in] */ IRect * pRect);

    static Boolean NativeCompressToJpeg(
        /* [in] */ ArrayOf<Byte> & oriYuv,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ ArrayOf<Int32> & offsets,
        /* [in] */ ArrayOf<Int32> & strides,
        /* [in] */ Int32 quality,
        /* [in] */ IOutputStream* stream,
        /* [out] */ ArrayOf<Byte> & tempStorage);

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
