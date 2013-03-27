
#include "ext/frameworkdef.h"
#include "graphics/CYuvImage.h"
#include "graphics/CreateOutputStreamAdaptor.h"
#include "graphics/YuvToJpegEncoder.h"
#include <elastos/AutoPtr.h>
#include <skia/core/SkStream.h>


const Int32 CYuvImage::WORKING_COMPRESS_STORAGE;

/**
 * Construct an YuvImage.
 *
 * @param yuv     The YUV data. In the case of more than one image plane, all the planes must be
 *                concatenated into a single byte array.
 * @param format  The YUV data format as defined in {@link PixelFormat}.
 * @param width   The width of the YuvImage.
 * @param height  The height of the YuvImage.
 * @param strides (Optional) Row bytes of each image plane. If yuv contains padding, the stride
 *                of each image must be provided. If strides is null, the method assumes no
 *                padding and derives the row bytes by format and width itself.
 * @return E_ILLEGAL_ARGUMENT_EXCEPTION if format is not support; width or height <= 0; or yuv is
 *                null.
*/
ECode CYuvImage::constructor(
    /* [in] */ const ArrayOf<Byte>& yuv,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ ArrayOf<Int32>* strides)
{
    if (format != IMAGEFORMAT_NV21 &&
                format != IMAGEFORMAT_YUY2) {
        // throw new IllegalArgumentException(
        //             "only support ImageFormat.NV21 " +
        //             "and ImageFormat.YUY2 for now");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (width <= 0  || height <= 0) {
        // throw new IllegalArgumentException(
        //             "width and height must large than 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (strides == NULL) {
        mStrides = CalculateStrides(width, format);
    }
    else {
        mStrides = strides->Clone();
    }

    mData = yuv.Clone();
    mFormat = format;
    mWidth = width;
    mHeight = height;

    return NOERROR;
}

/**
 * Compress a rectangle region in the YuvImage to a jpeg.
 * Only ImageFormat.NV21 and ImageFormat.YUY2
 * are supported for now.
 *
 * @param rectangle The rectangle region to be compressed. The medthod checks if rectangle is
 *                  inside the image. Also, the method modifies rectangle if the chroma pixels
 *                  in it are not matched with the luma pixels in it.
 * @param quality   Hint to the compressor, 0-100. 0 meaning compress for
 *                  small size, 100 meaning compress for max quality.
 * @param stream    OutputStream to write the compressed data.
 * @return          True if the compression is successful.
 * @throws IllegalArgumentException if rectangle is invalid; quality is not within [0,
 *                  100]; or stream is null.
*/
ECode CYuvImage::CompressToJpeg(
    /* [in] */ IRect* rectangle,
    /* [in] */ Int32 quality,
    /* [in] */ IOutputStream* stream,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IRect> wholeImage;
    CRect::New(0, 0, mWidth, mHeight, (IRect**)&wholeImage);

    Boolean res;
    wholeImage->ContainsEx2(rectangle, &res);
    if (!res) {
        // throw new IllegalArgumentException(
        //             "rectangle is not inside the image");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (quality < 0 || quality > 100) {
        // throw new IllegalArgumentException("quality must be 0..100");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (stream == NULL) {
        // throw new IllegalArgumentException("stream cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AdjustRectangle(rectangle);
    ArrayOf<Int32>* offsets = CalculateOffsets(
        ((CRect*)rectangle)->mLeft,
        ((CRect*)rectangle)->mTop);

    Int32 width;
    Int32 height;
    rectangle->GetWidth(&width);
    rectangle->GetHeight(&height);

    ArrayOf_<Byte, WORKING_COMPRESS_STORAGE> tempStorage;
    *result = NativeCompressToJpeg(
                *mData,
                mFormat,
                width,
                height,
                *offsets,
                *mStrides,
                quality,
                stream,
                tempStorage);

    ArrayOf<Int32>::Free(offsets);
    return NOERROR;
}

ECode CYuvImage::GetYuvData(
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    VALIDATE_NOT_NULL(data);

    *data = mData->Clone();
    return NOERROR;
}

ECode CYuvImage::GetYuvFormat(
    /* [out] */ Int32* format)
{
    VALIDATE_NOT_NULL(format);

    *format = mFormat;
    return NOERROR;
}

ECode CYuvImage::GetStrides(
    /* [out, callee] */ ArrayOf<Int32>** strides)
{
    VALIDATE_NOT_NULL(strides);

    *strides = mStrides->Clone();
    return NOERROR;
}

ECode CYuvImage::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CYuvImage::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ArrayOf<Int32>* CYuvImage::CalculateOffsets(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top)
{
    ArrayOf<Int32>* offsets = NULL;
    if (mFormat == IMAGEFORMAT_NV21) {
        offsets = ArrayOf<Int32>::Alloc(2);
        (*offsets)[0] = top * (*mStrides)[0] + left;
        (*offsets)[1] = mHeight * (*mStrides)[0] + top / 2 * (*mStrides)[1] + left / 2 * 2;
        return offsets;
    }

    if (mFormat == IMAGEFORMAT_YUY2) {
        offsets = ArrayOf<Int32>::Alloc(1);
        (*offsets)[0] = top * (*mStrides)[0] + left / 2 * 4;
        return offsets;
    }

    return offsets;
}

ArrayOf<Int32>* CYuvImage::CalculateStrides(
    /* [in] */ Int32 width,
    /* [in] */ Int32 format)
{
    ArrayOf<Int32>* strides = NULL;
    if (format == IMAGEFORMAT_NV21) {
        strides = ArrayOf<Int32>::Alloc(2);
        (*strides)[0] = (*strides)[1] = width;
        return strides;
    }

    if (format == IMAGEFORMAT_YUY2) {
        strides = ArrayOf<Int32>::Alloc(1);
        (*strides)[0] = width * 2;
        return strides;
    }

    return strides;
}

void CYuvImage::AdjustRectangle(
    /* [in] */ IRect* rect)
{
    Int32 width;
    Int32 height;
    rect->GetWidth(&width);
    rect->GetHeight(&height);
    CRect* rectObj = (CRect*)rect;
    if (mFormat == IMAGEFORMAT_NV21) {
       // Make sure left, top, width and height are all even.
       width &= ~1;
       height &= ~1;
       rectObj->mLeft &= ~1;
       rectObj->mTop &= ~1;
       rectObj->mRight = rectObj->mLeft + width;
       rectObj->mBottom = rectObj->mTop + height;
    }

    if (mFormat == IMAGEFORMAT_YUY2) {
        // Make sure left and width are both even.
        width &= ~1;
        rectObj->mLeft &= ~1;
        rectObj->mRight = rectObj->mLeft + width;
    }
}

//////////// native methods

Boolean CYuvImage::NativeCompressToJpeg(
    /* [in] */ ArrayOf<Byte> & oriYuv,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ ArrayOf<Int32> & offsets,
    /* [in] */ ArrayOf<Int32> & strides,
    /* [in] */ Int32 quality,
    /* [in] */ IOutputStream* stream,
    /* [out] */ ArrayOf<Byte> & tempStorage)
{
    Byte* yuv = oriYuv.GetPayload();
    Int32* imgOffsets = offsets.GetPayload();
    Int32* imgStrides = strides.GetPayload();
    SkWStream* strm = CreateOutputStreamAdaptor(stream, &tempStorage);
    YuvToJpegEncoder* encoder = YuvToJpegEncoder::create(format, imgStrides);
    if (encoder == NULL) {
        return FALSE;
    }
	
    encoder->encode(strm, yuv, width, height, imgOffsets, quality);
    delete encoder;
    return TRUE;
}
