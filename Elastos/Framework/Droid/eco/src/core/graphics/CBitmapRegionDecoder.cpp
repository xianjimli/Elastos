
#include "graphics/CBitmapRegionDecoder.h"
#include "graphics/CRect.h"
#include "content/AssetInputStream.h"
#include "graphics/Graphics.h"
#include "graphics/AutoDecoderCancel.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/Utils.h"
#include <elastos/AutoFree.h>
#include <skia/images/SkImageDecoder.h>
#include <skia/core/SkTemplates.h>
#include <skia/core/SkPixelRef.h>
#include <sys/stat.h>

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
ECode CBitmapRegionDecoder::newInstance(
    /* [in] */ const ArrayOf<Byte> & data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder ** ppIBrd)
{
    if ((offset | length) < 0 || data.GetLength() < offset + length) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *ppIBrd = nativeNewInstance(data, offset, length, isShareable);
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
ECode CBitmapRegionDecoder::newInstanceEx(
    /* [in] */ IFileDescriptor * pFd,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder ** ppIBrd)
{
    *ppIBrd = nativeNewInstance(pFd, isShareable);
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
ECode CBitmapRegionDecoder::newInstanceEx2(
    /* [in] */ IInputStream * pIs,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder ** ppIBrd)
{
    Boolean bSupport;
    pIs->IsMarkSupported(&bSupport);
    if (!bSupport) {
        //new CBufferedInputStream(pIs, 16 * 1024);
    }

    // if (is instanceof AssetManager.AssetInputStream) {
    if (true) {
        Int32 Value;
        ((AssetInputStream *) pIs)->GetAssetInt32(&Value);

        *ppIBrd = nativeNewInstance(Value, isShareable);
    } else {
    // pass some temp storage down to the native code. 1024 is made up,
    // but should be large enough to avoid too many small calls back
    // into is.read(...).

        Byte tempStorage[16 * 1024];
        *ppIBrd = nativeNewInstance(pIs, tempStorage, isShareable);
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
ECode CBitmapRegionDecoder::newInstanceEx3(
    /* [in] */ const String& pathName,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder ** ppIBrd)
{
    IBitmapRegionDecoder* decoder = NULL;
    //CFileInputStream* pCFileInputStream = NULL;
    IFileInputStream* pIFileInputStream = NULL;



    CFileInputStream::New(pathName, &pIFileInputStream);

    IInputStream* stream = (IInputStream*) pIFileInputStream;
    newInstanceEx2(stream, isShareable, &decoder);


    if (pIFileInputStream != NULL) {
        pIFileInputStream->Close();
    }

    *ppIBrd = decoder;
    return NOERROR;
}

/**
* Decodes a rectangle region in the image specified by rect.
*
* @param rect The rectangle that specified the region to be decode.
* @param options null-ok; Options that control downsampling.
*             inPurgeable is not supported.
* @param ppBitmap The decoded bitmap, or null if the image data could not be
*         decoded.
*/
ECode CBitmapRegionDecoder::decodeRegion(
    /* [in] */ IRect * pRect,
    /* [in] */ IBitmapFactoryOptions * pOptions,
    /* [out] */ IBitmap ** ppBitmap)
{

    checkRecycled(String("decodeRegion called on recycled region decoder"));

    Int32 width;
    Int32 height;

    getWidth(&width);
    getHeight(&height);

    if (((CRect*)pRect)->mLeft < 0 || ((CRect*)pRect)->mTop < 0 || ((CRect*)pRect)->mRight > width
        || ((CRect*)pRect)->mBottom > height) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *ppBitmap = nativeDecodeRegion(mNativeBitmapRegionDecoder, ((CRect*)pRect)->mLeft, ((CRect*)pRect)->mTop,
    ((CRect*)pRect)->mRight - ((CRect*)pRect)->mLeft, ((CRect*)pRect)->mBottom - ((CRect*)pRect)->mTop, pOptions);

    return NOERROR;
}

/** @param pWidth the original image's width */
ECode CBitmapRegionDecoder::getWidth(
    /* [out] */ Int32 * pWidth)
{
    checkRecycled(String("getWidth called on recycled region decoder"));
    *pWidth = nativeGetWidth(mNativeBitmapRegionDecoder);

    return NOERROR;
}

/** @param pHeight the original image's height */
ECode CBitmapRegionDecoder::getHeight(
    /* [out] */ Int32 * pHeight)
{
    checkRecycled(String("getHeight called on recycled region decoder"));
    *pHeight = nativeGetHeight(mNativeBitmapRegionDecoder);

    return NOERROR;
}

/**
* Frees up the memory associated with this region decoder, and mark the
* region decoder as "dead", meaning it will throw an exception if decodeRegion(),
* getWidth() or getHeight() is called.
*
* <p>This operation cannot be reversed, so it should only be called if you are
* sure there are no further uses for the region decoder. This is an advanced call,
* and normally need not be called, since the normal GC process will free up this
* memory when there are no more references to this region decoder.
*/
ECode CBitmapRegionDecoder::recycle()
{
    if (!mRecycled) {
        nativeClean(mNativeBitmapRegionDecoder);
        mRecycled = true;
    }
    return NOERROR;
}

/**
* @param pResult true if this region decoder has been recycled.
* If so, then it is an error to try use its method.
*
* @return true if the region decoder has been recycled
*/
ECode CBitmapRegionDecoder::isRecycled(
    /* [out] */ Boolean * pResult)
{
    *pResult = mRecycled;
    return NOERROR;
}

/**
* Called by methods that want to throw an exception if the region decoder
* has already been recycled.
*/
ECode CBitmapRegionDecoder::checkRecycled(
    /* [in] */ const String& errorMessage)
{
    if (mRecycled) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return NOERROR;
}

/*  Private constructor that must receive an already allocated native
region decoder int (pointer).

*/
ECode CBitmapRegionDecoder::constructor(
    /* [in] */ Int32 decoder)
{
    mNativeBitmapRegionDecoder = (SkBitmapRegionDecoder*)decoder;
    mRecycled = false;

    return NOERROR;
}


CBitmapRegionDecoder::~CBitmapRegionDecoder()
{
    recycle();
}

Int32 gOptions_sampleSizeFieldID = 0;
Int32 gOptions_widthFieldID = 0;
Int32 gOptions_heightFieldID = 0;
String gOptions_mimeFieldID ;
Int32 gOptions_configFieldID = 0;
Int32 gFileDescriptor_descriptor = 0;
Boolean gOptions_ditherFieldID = false;
Boolean gOptions_preferQualityOverSpeedFieldID = false;
Boolean gOptions_mCancelID = false;

IBitmap* CBitmapRegionDecoder::nativeDecodeRegion(
                    SkBitmapRegionDecoder* brd,
                    Int32 start_x,
                    Int32 start_y,
                    Int32 width,
                    Int32 height,
                    IBitmapFactoryOptions* options)
{
    SkImageDecoder *decoder = brd->getDecoder();
    Int32 sampleSize = 1;
    SkBitmap::Config prefConfig = SkBitmap::kNo_Config;
    Boolean doDither = true;
    Boolean preferQualityOverSpeed = false;

    if (NULL != options) {
        sampleSize = gOptions_sampleSizeFieldID;
        // initialize these, in case we fail later on
        gOptions_widthFieldID = -1;
        gOptions_heightFieldID = -1;
        gOptions_mimeFieldID = 0;

        Int32 jconfig = gOptions_configFieldID;
        prefConfig = Graphics::GetNativeBitmapConfig(jconfig);
        doDither = gOptions_ditherFieldID;
        preferQualityOverSpeed = gOptions_preferQualityOverSpeedFieldID;
    }

    decoder->setDitherImage(doDither);
    decoder->setPreferQualityOverSpeed(preferQualityOverSpeed);
    SkBitmap* bitmap = new SkBitmap;
    SkAutoTDelete<SkBitmap> adb(bitmap);
    AutoDecoderCancel adc(options, decoder);

    // To fix the race condition in case "requestCancelDecode"
    // happens earlier than AutoDecoderCancel object is added
    // to the gAutoDecoderCancelMutex linked list.
    if (NULL != options && gOptions_mCancelID) {
        return NULL;
    }

    SkIRect region;
    region.fLeft = start_x;
    region.fTop = start_y;
    region.fRight = start_x + width;
    region.fBottom = start_y + height;

    if (!brd->decodeRegion(bitmap, region, prefConfig, sampleSize)) {
        return NULL;
    }

    // update options (if any)
    if (NULL != options) {
        gOptions_widthFieldID = bitmap->width();
        gOptions_heightFieldID = bitmap->height();
        // TODO: set the mimeType field with the data from the codec.
        // but how to reuse a set of strings, rather than allocating new one
        // each time?

        CBitmapFactory::GetMimeTypeString(decoder->getFormat(), &gOptions_mimeFieldID);
    }

    // detach bitmap from its autotdeleter, since we want to own it now
    adb.detach();

    SkPixelRef* pr;
    pr = bitmap->pixelRef();
    // promise we will never change our pixels (great for sharing and pictures)
    pr->setImmutable();
    // now create the java bitmap
    return NULL;//GraphicsJNI::createBitmap(env, bitmap, false, NULL);
}

Int32 CBitmapRegionDecoder::nativeGetWidth(SkBitmapRegionDecoder* brd)
{
    return brd->getWidth();
}

Int32 CBitmapRegionDecoder::nativeGetHeight(SkBitmapRegionDecoder* brd)
{
    return brd->getHeight();
}

void CBitmapRegionDecoder::nativeClean(SkBitmapRegionDecoder* brd)
{
    delete brd;
}

static IBitmapRegionDecoder* doBuildTileIndex(SkStream* stream) {
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

static SkMemoryStream* buildSkMemoryStream(SkStream *stream) {
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

IBitmapRegionDecoder* CBitmapRegionDecoder::nativeNewInstance(
                    const ArrayOf<Byte> & data,
                    Int32 offset,
                    Int32 length,
                    Boolean isShareable)
{
    /*  If isShareable we could decide to just wrap the java array and
    share it, but that means adding a globalref to the java array object
    For now we just always copy the array's data if isShareable.
    */
    //AutoJavaByteArray ar(env, byteArray);
    SkStream* stream = new SkMemoryStream(data.GetPayload() + offset, length, true);
    return doBuildTileIndex(stream);
}

IBitmapRegionDecoder* CBitmapRegionDecoder::nativeNewInstance(
                    IFileDescriptor* fd,
                    Boolean isShareable)
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
        stream = buildSkMemoryStream(fdStream);
        fdStream->unref();
    }

    return doBuildTileIndex(stream);;
}

IBitmapRegionDecoder* CBitmapRegionDecoder::nativeNewInstance(
                    IInputStream* is,
                    //ArrayOf<Byte> & storage,
                    Byte* storage,
                    Boolean isShareable)
{
    IBitmapRegionDecoder* largeBitmap = NULL;

    // SkStream* stream = CreateJavaInputStreamAdaptor(env, is, storage, 1024);

    // if (stream) {
    //     // for now we don't allow shareable with java inputstreams
    //     SkMemoryStream *mStream = buildSkMemoryStream(stream);
    //     largeBitmap = doBuildTileIndex(env, mStream);
    //     stream->unref();
    // }

    return largeBitmap;;
}

IBitmapRegionDecoder* CBitmapRegionDecoder::nativeNewInstance(
                    Int32 asset,
                    Boolean isShareable)
{
    // SkStream* stream, *assStream;
    // Asset* asset = reinterpret_cast<Asset*>(native_asset);
    // assStream = new AssetStreamAdaptor(asset);
    // stream = buildSkMemoryStream(assStream);
    // assStream->unref();
    //return doBuildTileIndex(stream);

    return NULL;
}