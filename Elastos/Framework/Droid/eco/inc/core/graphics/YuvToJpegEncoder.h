#ifndef __YUVTOJPEGENCODER_H__
#define __YUVTOJPEGENCODER_H__

#include "ext/frameworkext.h"
#include <skia/core/SkStream.h>

class YuvToJpegEncoder
{
public:
    /** Create an encoder based on the YUV format.
     *
     *  @param pixelFormat The yuv pixel format as defined in ui/PixelFormat.h.
     *  @param strides The number of row bytes in each image plane.
     *  @return an encoder based on the pixelFormat.
     */
    static YuvToJpegEncoder* create(
        /* [in] */ Int32 pixelFormat,
        /* [in] */ Int32* strides);

    YuvToJpegEncoder(
        /* [in] */ Int32* strides);

    /** Encode YUV data to jpeg,  which is output to a stream.
     *
     *  @param stream The jpeg output stream.
     *  @param inYuv The input yuv data.
     *  @param width Width of the the Yuv data in terms of pixels.
     *  @param height Height of the Yuv data in terms of pixels.
     *  @param offsets The offsets in each image plane with respect to inYuv.
     *  @param jpegQuality Picture quality in [0, 100].
     *  @return true if successfully compressed the stream.
     */
    bool encode(
        /* [in] */ SkWStream* stream,
        /* [in] */ void* inYuv,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32* offsets,
        /* [in] */ Int32 jpegQuality);

    virtual ~YuvToJpegEncoder() {}

protected:
    Int32 fNumPlanes;
    Int32* fStrides;

    // void setJpegCompressStruct(
    //     /* [in] */ jpeg_compress_struct* cinfo,
    //     /* [in] */ Int32 width,
    //     /* [in] */ Int32 height,
    //     /* [in] */ Int32 quality);

    // virtual void configSamplingFactors(
    //     /* [in] */ jpeg_compress_struct* cinfo) = 0;

    // virtual void compress(
    //     /* [in] */ jpeg_compress_struct* cinfo,
    //     /* [in] */ uint8_t* yuv,
    //     /* [in] */ Int32* offsets) = 0;
};

class Yuv420SpToJpegEncoder : public YuvToJpegEncoder
{
public:
    Yuv420SpToJpegEncoder(
    /* [in] */ Int32* strides);

    virtual ~Yuv420SpToJpegEncoder() {}

private:
    // void configSamplingFactors(
    // /* [in] */ jpeg_compress_struct* cinfo);

    void deinterleaveYuv(
    /* [in] */ uint8_t* yuv,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ uint8_t*& yPlanar,
    /* [in] */ uint8_t*& uPlanar,
    /* [in] */ uint8_t*& vPlanar);

    void deinterleave(
    /* [in] */ uint8_t* vuPlanar,
    /* [in] */ uint8_t* uRows,
    /* [in] */ uint8_t* vRows,
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 width);

    // void compress(
    // /* [in] */ jpeg_compress_struct* cinfo,
    // /* [in] */ uint8_t* yuv,
    // /* [in] */ Int32* offsets);
};

class Yuv422IToJpegEncoder : public YuvToJpegEncoder
{
public:
    Yuv422IToJpegEncoder(
        /* [in] */ Int32* strides);

    virtual ~Yuv422IToJpegEncoder() {}

private:
    // void configSamplingFactors(
    //     /* [in] */ jpeg_compress_struct* cinfo);

    // void compress(
    //     /* [in] */ jpeg_compress_struct* cinfo,
    //     /* [in] */ uint8_t* yuv,
    //     /* [in] */ Int32* offsets);

    void deinterleave(
        /* [in] */ uint8_t* yuv,
        /* [in] */ uint8_t* yRows,
        /* [in] */ uint8_t* uRows,
        /* [in] */ uint8_t* vRows,
        /* [in] */ Int32 rowIndex,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);
};

#endif //__YUVTOJPEGENCODER_H__