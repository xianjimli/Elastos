
#include "graphics/CImageFormat.h"

/**
 * Use this function to retrieve the number of bits per pixel of an
 * ImageFormat.
 *
 * @param format
 * @return the number of bits per pixel of the given format or -1 if the
 *         format doesn't exist or is not supported.
*/
ECode CImageFormat::getBitsPerPixel(
    /* [in] */ Int32 format,
    /* [out] */ Int32 * pBits)
{
    switch (format) {
        case IMAGEFORMAT_RGB_565:
            *pBits = 16;
        case IMAGEFORMAT_NV16:
            *pBits = 16;
        case IMAGEFORMAT_YUY2:
            *pBits = 16;
        case IMAGEFORMAT_YV12:
            *pBits = 12;
        case IMAGEFORMAT_NV21:
            *pBits = 12;
        default:
        return E_NOT_IMPLEMENTED;
        }

    return NOERROR;
}

ECode CImageFormat::constructor()
{
    // TODO: Add your code here
    return NOERROR;
}

