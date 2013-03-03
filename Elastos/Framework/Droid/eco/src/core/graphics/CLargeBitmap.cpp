
#include "graphics/CLargeBitmap.h"
#include "graphics/CRect.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/AutoDecoderCancel.h"
#include "graphics/Graphics.h"
#include <skia/core/SkTemplates.h>
#include <skia/core/SkPixelRef.h>


CLargeBitmap::~CLargeBitmap()
{
    Recycle();
}

ECode CLargeBitmap::constructor(
    /* [in] */ Handle32 lbm)
{
    mNativeLargeBitmap = (SkBitmapRegionDecoder*)lbm;
    mRecycled = FALSE;
    return NOERROR;
}

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
    /* [in] */ IRect* rect,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    FAIL_RETURN(CheckRecycled("decodeRegion called on recycled large bitmap"));

    Int32 width;
    Int32 height;
    GetWidth(&width);
    GetHeight(&height);

    if (((CRect*)rect)->mLeft < 0 || ((CRect*)rect)->mTop < 0 ||
        ((CRect*)rect)->mRight > width || ((CRect*)rect)->mBottom > height) {
        // throw new IllegalArgumentException("rectangle is not inside the image");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NativeDecodeRegion(
                    mNativeLargeBitmap,
                    ((CRect*)rect)->mLeft,
                    ((CRect*)rect)->mTop,
                    ((CRect*)rect)->mRight -((CRect*)rect)->mLeft,
                    ((CRect*)rect)->mBottom - ((CRect*)rect)->mTop, options,
                    bitmap);
}

/** @param pLength the original image's width */
ECode CLargeBitmap::GetWidth(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);

    FAIL_RETURN(CheckRecycled("getWidth called on recycled large bitmap"));
    *length = NativeGetWidth(mNativeLargeBitmap);
    return NOERROR;
}

/** @param pHeight the original image's height */
ECode CLargeBitmap::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    FAIL_RETURN(CheckRecycled("getHeight called on recycled large bitmap"));
    *height =  NativeGetHeight(mNativeLargeBitmap);
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
        mRecycled = TRUE;
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
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mRecycled;
    return NOERROR;
}


/**
* Called by methods that want to throw an exception if the bitmap
* has already been recycled.
*/
ECode CLargeBitmap::CheckRecycled(
    /* [in] */ CString errorMessage)
{
    if (mRecycled) {
        // throw new IllegalStateException(errorMessage);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CLargeBitmap::NativeDecodeRegion(
    /* [in] */ SkBitmapRegionDecoder* lbm,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    SkImageDecoder* decoder = lbm->getDecoder();
    Int32 sampleSize = 1;
    SkBitmap::Config prefConfig = SkBitmap::kNo_Config;
    bool doDither = true;
    bool preferQualityOverSpeed = false;
    CBitmapFactoryOptions* optObj = (CBitmapFactoryOptions*)options;

    if (NULL != optObj) {
        sampleSize = optObj->mInSampleSize;
        // initialize these, in case we fail later on
        optObj->mOutWidth = -1;
        optObj->mOutHeight = -1;
        optObj->mOutMimeType = NULL;

        prefConfig = Graphics::GetNativeBitmapConfig(optObj->mInPreferredConfig);
        doDither = (bool)optObj->mInDither;
        preferQualityOverSpeed = (bool)optObj->mInPreferQualityOverSpeed;
    }

    decoder->setDitherImage(doDither);
    decoder->setPreferQualityOverSpeed(preferQualityOverSpeed);
    SkBitmap* nativeBitmap = new SkBitmap;
    SkAutoTDelete<SkBitmap> adb(nativeBitmap);
    AutoDecoderCancel adc(options, decoder);

    // To fix the race condition in case "requestCancelDecode"
    // happens earlier than AutoDecoderCancel object is added
    // to the gAutoDecoderCancelMutex linked list.
    if (NULL != optObj && optObj->mCancel) {
        *bitmap = NULL;
        return NOERROR;
    }

    SkIRect region;
    region.fLeft = startX;
    region.fTop = startY;
    region.fRight = startX + width;
    region.fBottom = startY + height;

    if (!lbm->decodeRegion(nativeBitmap, region, prefConfig, sampleSize)) {
        *bitmap = NULL;
        return NOERROR;
    }

    // update options (if any)
    if (NULL != optObj) {
        optObj->mOutWidth = nativeBitmap->width();
        optObj->mOutHeight = nativeBitmap->height();
        // TODO: set the mimeType field with the data from the codec.
        // but how to reuse a set of strings, rather than allocating new one
        // each time?
        optObj->mOutMimeType = CBitmapFactory::GetMimeTypeString(decoder->getFormat());
    }

    // detach bitmap from its autotdeleter, since we want to own it now
    adb.detach();

    SkPixelRef* pr;
    pr = nativeBitmap->pixelRef();
    // promise we will never change our pixels (great for sharing and pictures)
    pr->setImmutable();
    // now create the java bitmap
    return Graphics::CreateBitmap(nativeBitmap, FALSE, NULL, -1, bitmap);
}

Int32 CLargeBitmap::NativeGetWidth(
    /* [in] */ SkBitmapRegionDecoder* lbm)
{
    return lbm->getWidth();
}

Int32 CLargeBitmap::NativeGetHeight(
    /* [in] */ SkBitmapRegionDecoder* lbm)
{
    return lbm->getHeight();
}

void CLargeBitmap::NativeClean(
    /* [in] */ SkBitmapRegionDecoder* lbm)
{
    delete lbm;
}
