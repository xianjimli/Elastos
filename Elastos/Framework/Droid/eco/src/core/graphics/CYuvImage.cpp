
#include "graphics/CYuvImage.h"
#include <skia/core/SkStream.h>

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

ECode CYuvImage::compressToJpeg(
    /* [in] */ IRect * pRectangle,
    /* [in] */ Int32 quality,
    /* [in] */ IOutputStream * pStream,
    /* [out] */ Boolean * pResult)
{
    CRect wholeImage;
    wholeImage.mRight = mWidth;
    wholeImage.mBottom = mHeight;

    Boolean res;
    wholeImage.ContainsEx2(pRectangle, &res);
    if (!res) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (quality < 0 || quality > 100) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (pStream == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    adjustRectangle(pRectangle);

    AutoFree< ArrayOf<Int32> > pOffsets;
    calculateOffsets(((CRect*)pRectangle)->mLeft, ((CRect*)pRectangle)->mTop,
             (ArrayOf<Int32> **)&pOffsets);


    AutoFree< ArrayOf<Byte> > pNewByte = ArrayOf<Byte>::Alloc(YUVIMAGE_WORKING_COMPRESS_STORAGE);
    if (pNewByte == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    Int32 width;
    Int32 height;

    pRectangle->GetWidth(&width);
    pRectangle->GetHeight(&height);

    *pResult = nativeCompressToJpeg(*mData, mFormat, width,
												             height, *pOffsets, *mStrides, quality, pStream,
												             *pNewByte);

    return E_NOT_IMPLEMENTED;
}

ECode CYuvImage::getYuvData(
    /* [out, callee] */ ArrayOf<Byte> ** ppData)
{
    *ppData = mData->Clone();
    return NOERROR;
}

ECode CYuvImage::getYuvFormat(
    /* [out] */ Int32 * pFormat)
{
    *pFormat = mFormat;
    return NOERROR;
}

ECode CYuvImage::getStrides(
    /* [out, callee] */ ArrayOf<Int32> ** ppStrides)
{
    *ppStrides = mStrides->Clone();
    return NOERROR;
}

ECode CYuvImage::getWidth(
    /* [out] */ Int32 * pWidth)
{
    *pWidth = mWidth;
    return NOERROR;
}

ECode CYuvImage::getHeight(
    /* [out] */ Int32 * pHeight)
{
    *pHeight = mHeight;
    return NOERROR;
}

ECode CYuvImage::calculateOffsets(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [out, callee] */ ArrayOf<Int32> ** ppOffsets)
{
    if (mFormat == IMAGEFORMAT_NV21) {
        *ppOffsets = ArrayOf<Int32>::Alloc((top * (*mStrides)[0] + left) *
            (mHeight * (*mStrides)[0] + top / 2 * (*mStrides)[1] + left / 2 * 2 ));
    }

    if (mFormat == IMAGEFORMAT_YUY2) {
        *ppOffsets = ArrayOf<Int32>::Alloc(top * (*mStrides)[0] + left / 2 * 4);
    }

    return NOERROR;
}

ECode CYuvImage::calculateStrides(
    /* [in] */ Int32 width,
    /* [in] */ Int32 format,
    /* [out, callee] */ ArrayOf<Int32> ** ppStrides)
{
    if (format == IMAGEFORMAT_NV21) {
        *ppStrides = ArrayOf<Int32>::Alloc(width * width);
    }

    if (format == IMAGEFORMAT_YUY2) {
        *ppStrides = ArrayOf<Int32>::Alloc(width * 2);
    }

    return NOERROR;
}

ECode CYuvImage::adjustRectangle(
    /* [in] */ IRect * pRect)
{
    Int32 width;
    Int32 height;

    pRect->GetWidth(&width);
    pRect->GetHeight(&height);

    if (mFormat == IMAGEFORMAT_NV21) {
       // Make sure left, top, width and height are all even.
       width &= ~1;
       height &= ~1;
       ((CRect*)pRect)->mLeft &= ~1;
       ((CRect*)pRect)->mTop &= ~1;
       ((CRect*)pRect)->mRight = ((CRect*)pRect)->mLeft + width;
       ((CRect*)pRect)->mBottom = ((CRect*)pRect)->mTop + height;
    }

    if (mFormat == IMAGEFORMAT_YUY2) {
        // Make sure left and width are both even.
        width &= ~1;
        ((CRect*)pRect)->mLeft &= ~1;
        ((CRect*)pRect)->mRight = ((CRect*)pRect)->mLeft + width;
    }
    return NOERROR;
}

//////////// native methods


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
    /* [in] */ const ArrayOf<Byte> & yuv,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ const ArrayOf<Int32> & strides)
{
    if (format != IMAGEFORMAT_NV21 &&
                format != IMAGEFORMAT_YUY2) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        if (width <= 0  || height <= 0) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        if (yuv.GetPayload() == NULL) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        if (strides.GetPayload() == NULL) {
            calculateStrides(width, format, (ArrayOf<Int32> **)&mStrides);
        } else {
            mStrides = strides.Clone();
        }

        mData = yuv.Clone();
        mFormat = format;
        mWidth = width;
        mHeight = height;

    return NOERROR;
}


Boolean CYuvImage::nativeCompressToJpeg(ArrayOf<Byte> & oriYuv,
																						Int32 format, 
																						Int32 width, 
																						Int32 height, 
																						ArrayOf<Int32> & offsets, 
																						ArrayOf<Int32> & strides,
																						Int32 quality, 
																						IOutputStream* stream, 
																						ArrayOf<Byte> & tempStorage)
{
#if 0	

	Byte* yuv = oriYuv;
    Int32* imgOffsets = offsets;
    Int32* imgStrides = strides;

    SkWStream* strm = CreateJavaOutputStreamAdaptor(env, jstream, jstorage);

    YuvToJpegEncoder* encoder = YuvToJpegEncoder::create(format, imgStrides);
    if (encoder == NULL) {
        return false;
    }
    encoder->encode(strm, yuv, width, height, imgOffsets, jpegQuality);

    delete encoder;
    env->ReleaseByteArrayElements(inYuv, yuv, 0);
    env->ReleaseIntArrayElements(offsets, imgOffsets, 0);
    env->ReleaseIntArrayElements(strides, imgStrides, 0);
#endif
    return true;
}

