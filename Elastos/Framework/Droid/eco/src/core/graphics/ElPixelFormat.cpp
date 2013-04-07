
#include "ext/frameworkdef.h"
#include "graphics/ElPixelFormat.h"
#include <ui/PixelFormat.h>

const Int32 ElPixelFormat::UNKNOWN;
const Int32 ElPixelFormat::TRANSLUCENT;
const Int32 ElPixelFormat::TRANSPARENT;
const Int32 ElPixelFormat::OPAQUE;
const Int32 ElPixelFormat::RGBA_8888;
const Int32 ElPixelFormat::RGBX_8888;
const Int32 ElPixelFormat::RGB_888;
const Int32 ElPixelFormat::RGB_565;
const Int32 ElPixelFormat::RGBA_5551;
const Int32 ElPixelFormat::RGBA_4444;
const Int32 ElPixelFormat::A_8;
const Int32 ElPixelFormat::L_8;
const Int32 ElPixelFormat::LA_88;
const Int32 ElPixelFormat::RGB_332;
const Int32 ElPixelFormat::YCbCr_422_SP;
const Int32 ElPixelFormat::YCbCr_420_SP;
const Int32 ElPixelFormat::YCbCr_422_I;
const Int32 ElPixelFormat::JPEG;

using namespace android;

ECode ElPixelFormat::GetPixelFormatInfo(
    /* [in] */ Int32 format,
    /* [in] */ ElPixelFormat* pixelInfo)
{
    PixelFormatInfo info;
    status_t err;

    // we need this for backward compatibility with PixelFormat's
    // deprecated constants
    switch (format) {
        case HAL_PIXEL_FORMAT_YCbCr_422_SP:
            // defined as the bytes per pixel of the Y plane
            info.bytesPerPixel = 1;
            info.bitsPerPixel = 16;
            goto done;
        case HAL_PIXEL_FORMAT_YCrCb_420_SP:
            // defined as the bytes per pixel of the Y plane
            info.bytesPerPixel = 1;
            info.bitsPerPixel = 12;
            goto done;
        case HAL_PIXEL_FORMAT_YCbCr_422_I:
            // defined as the bytes per pixel of the Y plane
            info.bytesPerPixel = 1;
            info.bitsPerPixel = 16;
            goto done;
    }

    err = getPixelFormatInfo(format, &info);
    if (err < 0) {
        //doThrow(env, "java/lang/IllegalArgumentException");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

done:
    pixelInfo->mBytesPerPixel = info.bytesPerPixel;
    pixelInfo->mBitsPerPixel = info.bitsPerPixel;

    return NOERROR;
}


Boolean ElPixelFormat::FormatHasAlpha(
    /* [in] */ Int32 format)
{
    switch (format) {
        case A_8:
        case LA_88:
        case RGBA_4444:
        case RGBA_5551:
        case RGBA_8888:
        case TRANSLUCENT:
        case TRANSPARENT:
            return TRUE;
    }

    return FALSE;
}
