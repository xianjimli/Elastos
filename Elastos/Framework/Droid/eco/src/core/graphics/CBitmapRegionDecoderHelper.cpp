
#include "graphics/CBitmapRegionDecoderHelper.h"
#include "content/AssetInputStream.h"
#include "graphics/Utils.h"
#include <sys/stat.h>
#include <utils/Asset.h>

Int32 gFileDescriptor_descriptor = 0;
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
* @param ppIBrd, or null if the image data could not be decoded.
*
*/
ECode CBitmapRegionDecoderHelper::NewInstance(
    /* [in] */ const ArrayOf<Byte> & data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder ** ppIBrd)
{
    if ((offset | length) < 0 || data.GetLength() < offset + length) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *ppIBrd = NativeNewInstance(data, offset, length, isShareable);
    return NOERROR;
}
/**
* Create a BitmapRegionDecoder from the file descriptor.
* The position within the descriptor will not be changed when
* this returns, so the descriptor can be used again as is.
* Currently only the JPEG and PNG formats are supported.
*
* @param fd The file descriptor containing the data to decode
* @param isShareable If this is true, then the BitmapRegionDecoder may keep a
*                    shallow reference to the input. If this is false,
*                    then the BitmapRegionDecoder will explicitly make a copy of the
*                    input data, and keep that. Even if sharing is allowed,
*                    the implementation may still decide to make a deep
*                    copy of the input data. If an image is progressively encoded,
*                    allowing sharing may degrade the decoding speed.
* @param ppIBrd, or null if the image data could not be decoded.
*/
ECode CBitmapRegionDecoderHelper::NewInstanceEx(
    /* [in] */ IFileDescriptor * pFd,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder ** ppIBrd)
{
    *ppIBrd = NativeNewInstance(pFd, isShareable);
    return NOERROR;
}

/**
* Create a BitmapRegionDecoder from an input stream.
* The stream's position will be where ever it was after the encoded data
* was read.
* Currently only the JPEG and PNG formats are supported.
*
* @param is The input stream that holds the raw data to be decoded into a
*           BitmapRegionDecoder.
* @param isShareable If this is true, then the BitmapRegionDecoder may keep a
*                    shallow reference to the input. If this is false,
*                    then the BitmapRegionDecoder will explicitly make a copy of the
*                    input data, and keep that. Even if sharing is allowed,
*                    the implementation may still decide to make a deep
*                    copy of the input data. If an image is progressively encoded,
*                    allowing sharing may degrade the decoding speed.
* @param ppIBrd, or null if the image data could not be decoded.
*/
ECode CBitmapRegionDecoderHelper::NewInstanceEx2(
    /* [in] */ IInputStream * pIs,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder ** ppIBrd)
{
    Boolean bSupport;
    pIs->IsMarkSupported(&bSupport);
    if (!bSupport) {
        CBufferedInputStream::New(pIs, 16 * 1024, (IBufferedInputStream **)&pIs);
    }

    // if (is instanceof AssetManager.AssetInputStream) {
    if (true) {
        Int32 Value;
        ((AssetInputStream *) pIs)->GetAssetInt32(&Value);

        *ppIBrd = NativeNewInstance(Value, isShareable);
    } else {
    // pass some temp storage down to the native code. 1024 is made up,
    // but should be large enough to avoid too many small calls back
    // into is.read(...).

        Byte tempStorage[16 * 1024];
        *ppIBrd = NativeNewInstance(pIs, tempStorage, isShareable);
    }
    return NOERROR;
}


/**
* Create a BitmapRegionDecoder from a file path.
* Currently only the JPEG and PNG formats are supported.
*
* @param pathName complete path name for the file to be decoded.
* @param isShareable If this is true, then the BitmapRegionDecoder may keep a
*                    shallow reference to the input. If this is false,
*                    then the BitmapRegionDecoder will explicitly make a copy of the
*                    input data, and keep that. Even if sharing is allowed,
*                    the implementation may still decide to make a deep
*                    copy of the input data. If an image is progressively encoded,
*                    allowing sharing may degrade the decoding speed.
* @param ppIBrd, or null if the image data could not be decoded.
*/
ECode CBitmapRegionDecoderHelper::NewInstanceEx3(
    /* [in] */ const String& pathName,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder ** ppIBrd)
{
    IBitmapRegionDecoder* decoder = NULL;
    IInputStream* stream = NULL;



    CFileInputStream::New(pathName, (IFileInputStream**)&stream);
    NewInstanceEx2(stream, isShareable, &decoder);


    if (stream != NULL) {
        stream->Close();
    }

    *ppIBrd = decoder;
    return NOERROR;
}

IBitmapRegionDecoder* CBitmapRegionDecoderHelper::NativeNewInstance(
    /* [in] */ const ArrayOf<Byte> & data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isShareable)
{
    /*  If isShareable we could decide to just wrap the java array and
    share it, but that means adding a globalref to the java array object
    For now we just always copy the array's data if isShareable.
    */
    //AutoJavaByteArray ar(env, byteArray);
    SkStream* stream = new SkMemoryStream(data.GetPayload() + offset, length, true);
    return DoBuildTileIndex(stream);
}

IBitmapRegionDecoder* CBitmapRegionDecoderHelper::NativeNewInstance(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean isShareable)
{
    if (fd == NULL) {
        return NULL;
    }

    Int32 descriptor = gFileDescriptor_descriptor;
    SkStream *stream = NULL;
    struct stat fdStat;
    Int32 newFD;
    if (fstat(descriptor, &fdStat) == -1) {
        return NULL;
    }

    if (isShareable &&
            S_ISREG(fdStat.st_mode) &&
            (newFD = ::dup(descriptor)) != -1) {
        SkFDStream* fdStream = new SkFDStream(newFD, true);
        if (!fdStream->isValid()) {
            fdStream->unref();
            return NULL;
        }
        stream = fdStream;
    } else {
        /* Restore our offset when we leave, so we can be called more than once
           with the same descriptor. This is only required if we didn't dup the
           file descriptor, but it is OK to do it all the time.
        */
        AutoFDSeek as(descriptor);

        SkFDStream* fdStream = new SkFDStream(descriptor, false);
        if (!fdStream->isValid()) {
            fdStream->unref();
            return NULL;
        }
        stream = BuildSkMemoryStream(fdStream);
        fdStream->unref();
    }

    return DoBuildTileIndex(stream);;
}

IBitmapRegionDecoder* CBitmapRegionDecoderHelper::NativeNewInstance(
    /* [in] */ IInputStream* is,
    /* [in] */ //ArrayOf<Byte> & storage,
    /* [in] */ Byte* storage,
    /* [in] */ Boolean isShareable)
{
    IBitmapRegionDecoder* largeBitmap = NULL;

    SkStream* stream = NULL;//CreateJavaInputStreamAdaptor(env, is, storage, 1024);

    if (stream) {
        // for now we don't allow shareable with java inputstreams
        SkMemoryStream *mStream = BuildSkMemoryStream(stream);
        largeBitmap = DoBuildTileIndex(mStream);
        stream->unref();
    }

    return largeBitmap;;
}

IBitmapRegionDecoder* CBitmapRegionDecoderHelper::NativeNewInstance(
    /* [in] */ Int32 asset,
    /* [in] */ Boolean isShareable)
{
    SkStream* stream, *assStream;
    // android::Asset* asset = NULL;//reinterpret_cast<Asset*>(native_asset);
    // assStream = new AssetStreamAdaptor(asset);
    // stream = BuildSkMemoryStream(assStream);
    // assStream->unref();
    return DoBuildTileIndex(stream);
}

IBitmapRegionDecoder* CBitmapRegionDecoderHelper::DoBuildTileIndex(
    /* [in] */ SkStream* stream)
{
    // SkImageDecoder* decoder = SkImageDecoder::Factory(stream);
    // int width, height;
    // if (NULL == decoder) {
    //     return NULL;
    // }

    // JavaPixelAllocator *javaAllocator = new JavaPixelAllocator(env, true);
    // decoder->setAllocator(javaAllocator);
    // JavaMemoryUsageReporter *javaMemoryReporter = new JavaMemoryUsageReporter(env);
    // decoder->setReporter(javaMemoryReporter);
    // javaAllocator->unref();
    // javaMemoryReporter->unref();

    // if (!decoder->buildTileIndex(stream, &width, &height)) {
    //     char msg[100];
    //     snprintf(msg, sizeof(msg), "Image failed to decode using %s decoder",
    //             decoder->getFormatName());
    //     doThrowIOE(env, msg);
    //     return nullObjectReturn("decoder->buildTileIndex returned false");
    // }

    // SkBitmapRegionDecoder *bm = new SkBitmapRegionDecoder(decoder, width, height);

    // return GraphicsJNI::createBitmapRegionDecoder(env, bm);
    return NULL;
}

SkMemoryStream* CBitmapRegionDecoderHelper::BuildSkMemoryStream(
    /* [in] */ SkStream *stream)
{
    size_t bufferSize = 4096;
    size_t streamLen = 0;
    size_t len;
    char* data = (char*)sk_malloc_throw(bufferSize);

    while ((len = stream->read(data + streamLen,
                    bufferSize - streamLen)) != 0) {
        streamLen += len;
        if (streamLen == bufferSize) {
            bufferSize *= 2;
            data = (char*)sk_realloc_throw(data, bufferSize);
        }
    }
    data = (char*)sk_realloc_throw(data, streamLen);

    SkMemoryStream* streamMem = new SkMemoryStream();
    streamMem->setMemoryOwned(data, streamLen);
    return streamMem;
}

