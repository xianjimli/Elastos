#ifndef __ELPIXELFORMAT_H__
#define __ELPIXELFORMAT_H__

#include <elastos.h>

using namespace Elastos;

class ElPixelFormat
{
public:
    /* these constants need to match those in hardware/hardware.h */
    static const Int32 UNKNOWN     = 0;

    /** System chooses a format that supports translucency (many alpha bits) */
    static const Int32 TRANSLUCENT = -3;

    /** 
    * System chooses a format that supports transparency
    * (at least 1 alpha bit) 
    */    
    static const Int32 TRANSPARENT = -2;

    /** System chooses an opaque format (no alpha bits required) */
    static const Int32 OPAQUE      = -1;

    static const Int32 RGBA_8888   = 1;
    static const Int32 RGBX_8888   = 2;
    static const Int32 RGB_888     = 3;
    static const Int32 RGB_565     = 4;

    static const Int32 RGBA_5551   = 6;
    static const Int32 RGBA_4444   = 7;
    static const Int32 A_8         = 8;
    static const Int32 L_8         = 9;
    static const Int32 LA_88       = 0xA;
    static const Int32 RGB_332     = 0xB;


    /**
    * @deprecated use {@link android.graphics.ImageFormat#NV16 
    * ImageFormat.NV16} instead.
    */
    static const Int32 YCbCr_422_SP= 0x10;

    /**
    * @deprecated use {@link android.graphics.ImageFormat#NV21 
    * ImageFormat.NV21} instead.
    */
    static const Int32 YCbCr_420_SP= 0x11;

    /**
    * @deprecated use {@link android.graphics.ImageFormat#YUY2 
    * ImageFormat.YUY2} instead.
    */
    static const Int32 YCbCr_422_I = 0x14;

    /**
    * @deprecated use {@link android.graphics.ImageFormat#JPEG 
    * ImageFormat.JPEG} instead.
    */
    static const Int32 JPEG        = 0x100;

    /*
    * We use a class initializer to allow the native code to cache some
    * field offsets.
    */
    //native private static void nativeClassInit();
    //static { nativeClassInit(); }

    //public static native void getPixelFormatInfo(int format, PixelFormat info);

    static Boolean FormatHasAlpha(
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

    Int32 mBytesPerPixel;
    Int32 mBitsPerPixel;
};
#endif //__ELPIXELFORMAT_H__
