
#include "graphics/CLargeBitmap.h"
#include "graphics/CRect.h"

/**
* Decodes a rectangle region in the image specified by rect.
*
* @param rect The rectangle that specified the region to be decode.
* @param opts null-ok; Options that control downsampling.
*             inPurgeable is not supported.
* @return The decoded bitmap, or null if the image data could not be
*         decoded.
*/
ECode CLargeBitmap::DecodeRegion(
    /* [in] */ IRect * pRect,
    /* [in] */ IBitmapFactoryOptions * pOptions,
    /* [out] */ IBitmap ** ppBitmap)
{
    CheckRecycled((Int32)"decodeRegion called on recycled large bitmap");

    Int32 width;
    Int32 height;

    GetWidth(&width);
    GetHeight(&height);

    if (((CRect*)pRect)->mLeft < 0 || ((CRect*)pRect)->mTop < 0 ||
        ((CRect*)pRect)->mRight > width || ((CRect*)pRect)->mBottom > height)
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    *ppBitmap = NativeDecodeRegion(
                    (SkBitmapRegionDecoder *)mNativeLargeBitmap,
                    ((CRect*)pRect)->mLeft,
                    ((CRect*)pRect)->mTop,
                    ((CRect*)pRect)->mRight -((CRect*)pRect)->mLeft,
                    ((CRect*)pRect)->mBottom - ((CRect*)pRect)->mTop, pOptions);

    return NOERROR;
}

/** @param pLength the original image's width */
ECode CLargeBitmap::GetWidth(
    /* [out] */ Int32 * pLength)
{
    CheckRecycled((Int32)"getWidth called on recycled large bitmap");
    *pLength = NativeGetWidth(mNativeLargeBitmap);
    return NOERROR;
}

/** @param pHeight the original image's height */
ECode CLargeBitmap::GetHeight(
    /* [out] */ Int32 * pHeight)
{
    CheckRecycled((Int32)"getHeight called on recycled large bitmap");
    *pHeight =  NativeGetHeight(mNativeLargeBitmap);
    return NOERROR;
}

/**
* Frees up the memory associated with this large bitmap, and mark the
* large bitmap as "dead", meaning it will throw an exception if decodeRegion(),
* getWidth() or getHeight() is called.
* This operation cannot be reversed, so it should only be called if you are
* sure there are no further uses for the large bitmap. This is an advanced call,
* and normally need not be called, since the normal GC process will free up this
* memory when there are no more references to this bitmap.
*/
ECode CLargeBitmap::Recycle()
{
    if (!mRecycled) {
        NativeClean(mNativeLargeBitmap);
        mRecycled = true;
    }
    return NOERROR;
}

/**
* Returns true if this large bitmap has been recycled.
* If so, then it is an error to try use its method.
*
* @return true if the large bitmap has been recycled
*/
ECode CLargeBitmap::IsRecycled(
    /* [out] */ Boolean * pResult)
{
    *pResult = mRecycled;
    return NOERROR;
}


/**
* Called by methods that want to throw an exception if the bitmap
* has already been recycled.
*/
ECode CLargeBitmap::CheckRecycled(
    /* [in] */ Int32 errorMessage)
{
    if (mRecycled) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return NOERROR;
}

ECode CLargeBitmap::constructor(
    /* [in] */ Int32 ibm)
{
    mNativeLargeBitmap = ibm;
    mRecycled = false;
    return NOERROR;
}

IBitmap* CLargeBitmap::NativeDecodeRegion(
    /* [in] */ SkBitmapRegionDecoder * brd,
    /* [in] */ Int32 start_x,
    /* [in] */ Int32 start_y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IBitmapFactoryOptions * options)
{
#if 0
    SkImageDecoder *decoder = brd->getDecoder();
    Int32 sampleSize = 1;
    SkBitmap::Config prefConfig = SkBitmap::kNo_Config;
    Boolean doDither = true;
    Boolean preferQualityOverSpeed = false;

    if (NULL != options) {
        sampleSize = env->GetIntField(options, gOptions_sampleSizeFieldID);
        // initialize these, in case we fail later on
        env->SetIntField(options, gOptions_widthFieldID, -1);
        env->SetIntField(options, gOptions_heightFieldID, -1);
        env->SetObjectField(options, gOptions_mimeFieldID, 0);

        jobject jconfig = env->GetObjectField(options, gOptions_configFieldID);
        prefConfig = GraphicsJNI::getNativeBitmapConfig(env, jconfig);
        doDither = env->GetBooleanField(options, gOptions_ditherFieldID);
        preferQualityOverSpeed = env->GetBooleanField(options,
                gOptions_preferQualityOverSpeedFieldID);
    }

    decoder->setDitherImage(doDither);
    decoder->setPreferQualityOverSpeed(preferQualityOverSpeed);
    SkBitmap*           bitmap = new SkBitmap;
    SkAutoTDelete<SkBitmap>       adb(bitmap);
    AutoDecoderCancel   adc(options, decoder);

    // To fix the race condition in case "requestCancelDecode"
    // happens earlier than AutoDecoderCancel object is added
    // to the gAutoDecoderCancelMutex linked list.
    if (NULL != options && env->GetBooleanField(options, gOptions_mCancelID)) {
        return nullObjectReturn("gOptions_mCancelID");;
    }

    SkIRect region;
    region.fLeft = start_x;
    region.fTop = start_y;
    region.fRight = start_x + width;
    region.fBottom = start_y + height;

    if (!brd->decodeRegion(bitmap, region, prefConfig, sampleSize)) {
        return nullObjectReturn("decoder->decodeRegion returned false");
    }

    // update options (if any)
    if (NULL != options) {
        env->SetIntField(options, gOptions_widthFieldID, bitmap->width());
        env->SetIntField(options, gOptions_heightFieldID, bitmap->height());
        // TODO: set the mimeType field with the data from the codec.
        // but how to reuse a set of strings, rather than allocating new one
        // each time?
        env->SetObjectField(options, gOptions_mimeFieldID,
                            getMimeTypeString(env, decoder->getFormat()));
    }

    // detach bitmap from its autotdeleter, since we want to own it now
    adb.detach();

    SkPixelRef* pr;
    pr = bitmap->pixelRef();
    // promise we will never change our pixels (great for sharing and pictures)
    pr->setImmutable();
    // now create the java bitmap
    return GraphicsJNI::createBitmap(env, bitmap, false, NULL);
#endif
    return NULL;
}

Int32 CLargeBitmap::NativeGetWidth(
    /* [in] */ Int32 lbm)
{
    return NULL;
}

Int32 CLargeBitmap::NativeGetHeight(
    /* [in] */ Int32 lbm)
{
    return NULL;
}

void CLargeBitmap::NativeClean(
    /* [in] */ Int32 lbm)
{

}