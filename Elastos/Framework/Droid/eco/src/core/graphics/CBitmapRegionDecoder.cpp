
#include "graphics/CBitmapRegionDecoder.h"
#include "graphics/CRect.h"
#include "graphics/Graphics.h"
#include "graphics/AutoDecoderCancel.h"
#include "graphics/CBitmapFactory.h"
#include <elastos/AutoFree.h>
#include <skia/images/SkImageDecoder.h>
#include <skia/core/SkTemplates.h>
#include <skia/core/SkPixelRef.h>

/**
* Decodes a rectangle region in the image specified by rect.
*
* @param rect The rectangle that specified the region to be decode.
* @param options null-ok; Options that control downsampling.
*             inPurgeable is not supported.
* @param ppBitmap The decoded bitmap, or null if the image data could not be
*         decoded.
*/
ECode CBitmapRegionDecoder::DecodeRegion(
    /* [in] */ IRect * pRect,
    /* [in] */ IBitmapFactoryOptions * pOptions,
    /* [out] */ IBitmap ** ppBitmap)
{

    CheckRecycled(String("decodeRegion called on recycled region decoder"));

    Int32 width;
    Int32 height;

    GetWidth(&width);
    GetHeight(&height);

    if (((CRect*)pRect)->mLeft < 0 || ((CRect*)pRect)->mTop < 0 || ((CRect*)pRect)->mRight > width
        || ((CRect*)pRect)->mBottom > height) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *ppBitmap = NativeDecodeRegion(mNativeBitmapRegionDecoder, ((CRect*)pRect)->mLeft, ((CRect*)pRect)->mTop,
    ((CRect*)pRect)->mRight - ((CRect*)pRect)->mLeft, ((CRect*)pRect)->mBottom - ((CRect*)pRect)->mTop, pOptions);

    return NOERROR;
}

/** @param pWidth the original image's width */
ECode CBitmapRegionDecoder::GetWidth(
    /* [out] */ Int32 * pWidth)
{
    CheckRecycled(String("getWidth called on recycled region decoder"));
    *pWidth = NativeGetWidth(mNativeBitmapRegionDecoder);

    return NOERROR;
}

/** @param pHeight the original image's height */
ECode CBitmapRegionDecoder::GetHeight(
    /* [out] */ Int32 * pHeight)
{
    CheckRecycled(String("getHeight called on recycled region decoder"));
    *pHeight = NativeGetHeight(mNativeBitmapRegionDecoder);

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
ECode CBitmapRegionDecoder::Recycle()
{
    if (!mRecycled) {
        NativeClean(mNativeBitmapRegionDecoder);
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
ECode CBitmapRegionDecoder::IsRecycled(
    /* [out] */ Boolean * pResult)
{
    *pResult = mRecycled;
    return NOERROR;
}

/**
* Called by methods that want to throw an exception if the region decoder
* has already been recycled.
*/
ECode CBitmapRegionDecoder::CheckRecycled(
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
    Recycle();
}

Int32 gOptions_sampleSizeFieldID = 0;
Int32 gOptions_widthFieldID = 0;
Int32 gOptions_heightFieldID = 0;
String gOptions_mimeFieldID ;
Int32 gOptions_configFieldID = 0;

Boolean gOptions_ditherFieldID = false;
Boolean gOptions_preferQualityOverSpeedFieldID = false;
Boolean gOptions_mCancelID = false;

IBitmap* CBitmapRegionDecoder::NativeDecodeRegion(
    /* [in] */ SkBitmapRegionDecoder* brd,
    /* [in] */ Int32 start_x,
    /* [in] */ Int32 start_y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IBitmapFactoryOptions* options)
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

Int32 CBitmapRegionDecoder::NativeGetWidth(
    /* [in] */ SkBitmapRegionDecoder* brd)
{
    return brd->getWidth();
}

Int32 CBitmapRegionDecoder::NativeGetHeight(
    /* [in] */ SkBitmapRegionDecoder* brd)
{
    return brd->getHeight();
}

void CBitmapRegionDecoder::NativeClean(
    /* [in] */ SkBitmapRegionDecoder* brd)
{
    delete brd;
}
