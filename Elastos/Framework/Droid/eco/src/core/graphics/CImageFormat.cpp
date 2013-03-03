
#include "graphics/CImageFormat.h"

/**
 * Use this function to retrieve the number of bits per pixel of an
 * ImageFormat.
 *
 * @param format
 * @return the number of bits per pixel of the given format or -1 if the
 *         format doesn't exist or is not supported.
*/
ECode CImageFormat::GetBitsPerPixel(
    /* [in] */ Int32 format,
    /* [out] */ Int32* bits)
{
    switch (format) {
        case IMAGEFORMAT_RGB_565:
            *bits = 16;
            break;
        case IMAGEFORMAT_NV16:
            *bits = 16;
            break;
        case IMAGEFORMAT_YUY2:
            *bits = 16;
            break;
        case IMAGEFORMAT_YV12:
            *bits = 12;
            break;
        case IMAGEFORMAT_NV21:
            *bits = 12;
            break;
        default:
            *bits = -1;
    }

    return NOERROR;
}
