
#include "ext/frameworkdef.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CBitmap.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CCanvas.h"
#include "graphics/CPaint.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include "graphics/InputStreamAdaptor.h"
#include "graphics/Graphics.h"
#include "graphics/Utils.h"
#include "graphics/NinePatch.h"
#include "graphics/AutoDecoderCancel.h"
#include "utils/CTypedValue.h"
#include "utils/CDisplayMetrics.h"
#include "utils/ResourceTypes.h"
#include "os/MemoryFile.h"
#include <Elastos.IO.h>
#include <elastos/AutoPtr.h>
#include <skia/core/SkStream.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkTemplates.h>
#include <skia/ports/SkImageRef_ashmem.h>
#include <skia/images/SkImageRef_GlobalPool.h>
#include <skia/images/SkImageDecoder.h>
#include <elastos/Math.h>
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

////////////////////////////////////////////////////////////////////////////////
// NinePatchPeeker

class NinePatchPeeker : public SkImageDecoder::Peeker
{
public:
    NinePatchPeeker(SkImageDecoder* host)
    {
        // the host lives longer than we do, so a raw ptr is safe
        mHost = host;
        mPatchIsValid = false;
    }

    ~NinePatchPeeker()
    {
        if (mPatchIsValid) {
            free(mPatch);
        }
    }

    virtual bool peek(const char tag[], const void* data, size_t length)
    {
        if (strcmp("npTc", tag) == 0 && length >= sizeof(android::Res_png_9patch)) {
            android::Res_png_9patch* patch = (android::Res_png_9patch*) data;
            size_t patchSize = patch->serializedSize();
            assert(length == patchSize);
            // You have to copy the data because it is owned by the png reader
            android::Res_png_9patch* patchNew = (android::Res_png_9patch*) malloc(patchSize);
            memcpy(patchNew, patch, patchSize);
            // this relies on deserialization being done in place
            android::Res_png_9patch::deserialize(patchNew);
            patchNew->fileToDevice();
            if (mPatchIsValid) {
                free(mPatch);
            }
            mPatch = patchNew;
            //printf("9patch: (%d,%d)-(%d,%d)\n",
            //       mPatch.sizeLeft, mPatch.sizeTop,
            //       mPatch.sizeRight, mPatch.sizeBottom);
            mPatchIsValid = true;

            // now update our host to force index or 32bit config
            // 'cause we don't want 565 predithered, since as a 9patch, we know
            // we will be stretched, and therefore we want to dither afterwards.
            static const SkBitmap::Config gNo565Pref[] = {
                SkBitmap::kIndex8_Config,
                SkBitmap::kIndex8_Config,
                SkBitmap::kARGB_8888_Config,
                SkBitmap::kARGB_8888_Config,
                SkBitmap::kARGB_8888_Config,
                SkBitmap::kARGB_8888_Config,
            };
            mHost->setPrefConfigTable(gNo565Pref);
        } else {
            mPatch = NULL;
        }
        return true;    // keep on decoding
    }

public:
    bool    mPatchIsValid;
    android::Res_png_9patch*  mPatch;

private:
    SkImageDecoder* mHost;
};

////////////////////////////////////////////////////////////////////////////////
// CBitmapFactory

const char* CBitmapFactory::TAG = "CBitmapFactory";

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ IBitmap* source,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(source);
    Int32 width = 0, height = 0;
    source->GetWidth(&width);
    source->GetHeight(&height);
    return CreateBitmapEx(source, 0, 0, width, height, bitmap);
}

ECode CBitmapFactory::CreateBitmapEx(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IBitmap** bitmap)
{
    return CreateBitmapEx2(source, x, y, width, height, NULL, FALSE, bitmap);
}

ECode CBitmapFactory::CreateBitmapEx2(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IMatrix* m,
    /* [in] */ Boolean filter,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(source);
    VALIDATE_NOT_NULL(bitmap);

    if (!CBitmap::CheckXYSign(x, y)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!CBitmap::CheckWidthHeight(width, height)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 bitmapWidth = 0, bitmapHeight = 0;
    source->GetWidth(&bitmapWidth);
    source->GetHeight(&bitmapHeight);
    if (x + width > bitmapWidth) {
        Logger::E(TAG, "x + width must be <= bitmap->Width()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (y + height > bitmapHeight) {
        Logger::E(TAG, "y + height must be <= bitmap->Height()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // check if we can just return our argument unchanged
    Boolean isBitmapMutable = FALSE;
    source->IsMutable(&isBitmapMutable);
    Boolean isMatrixIdentity = FALSE;
    m->IsIdentity(&isMatrixIdentity);
    if (!isBitmapMutable && x == 0 && y == 0 && width == bitmapWidth &&
        height == bitmapHeight && (m == NULL || isMatrixIdentity)) {
        *bitmap = source;
        (*bitmap)->AddRef();
        return NOERROR;
    }

    Int32 neww = width;
    Int32 newh = height;
    AutoPtr<CCanvas> canvas;
    FAIL_RETURN(CCanvas::NewByFriend((CCanvas**)&canvas));
    AutoPtr<CBitmap> bmp;
    AutoPtr<CPaint> paint;

    AutoPtr<CRect> srcR;
    FAIL_RETURN(CRect::NewByFriend(x, y, x + width, y + height, (CRect**)&srcR));
    AutoPtr<CRectF> dstR;
    FAIL_RETURN(CRectF::NewByFriend(0, 0, width, height, (CRectF**)&dstR));

    Boolean hasBitmapAlpha = FALSE;
    source->HasAlpha(&hasBitmapAlpha);
    if (m == NULL || isMatrixIdentity) {
        AutoPtr<IBitmap> b;
        FAIL_RETURN(CreateBitmapEx3(neww, newh,
            hasBitmapAlpha ? BitmapConfig_ARGB_8888 : BitmapConfig_RGB_565,
            (IBitmap**)&b));
        bmp = (CBitmap*)b.Get();
        paint = NULL; // not needed
    }
    else {
        /*  the dst should have alpha if the src does, or if our matrix
            doesn't preserve rectness
        */
        Boolean isMatrixStays = FALSE;
        FAIL_RETURN(m->RectStaysRect(&isMatrixStays));
        Boolean hasAlpha = hasBitmapAlpha || !isMatrixStays;
        AutoPtr<CRectF> deviceR;
        FAIL_RETURN(CRectF::NewByFriend((CRectF**)&deviceR));
        Boolean result = FALSE;
        FAIL_RETURN(m->MapRect((IRectF*)deviceR.Get(), (IRectF*)dstR.Get(), &result));
        Float w = 0, h = 0;
        FAIL_RETURN(deviceR->Width(&w));
        FAIL_RETURN(deviceR->Height(&h));
        neww = Math::Round(w);
        newh = Math::Round(h);
        AutoPtr<IBitmap> b;
        FAIL_RETURN(CreateBitmapEx3(neww, newh,
            hasAlpha ? BitmapConfig_ARGB_8888 : BitmapConfig_RGB_565,
            (IBitmap**)&b));
        bmp = (CBitmap*)b.Get();
        if (hasAlpha) {
            FAIL_RETURN(bmp->EraseColor(99));
        }
        FAIL_RETURN(canvas->Translate(-deviceR->mLeft, -deviceR->mTop));
        FAIL_RETURN(canvas->Concat(m));
        FAIL_RETURN(CPaint::NewByFriend((CPaint**)&paint));
        FAIL_RETURN(paint->SetFilterBitmap(filter));
        if (!isMatrixStays) {
            FAIL_RETURN(paint->SetAntiAlias(TRUE));
        }
    }

    // The new bitmap was created from a known bitmap source so assume that
    // they use the same density
    bmp->mDensity = ((CBitmap*)source)->mDensity;

    FAIL_RETURN(canvas->SetBitmap((IBitmap*)bmp.Get()));
    FAIL_RETURN(canvas->DrawBitmapEx(source,
        (IRect*)srcR.Get(), (IRectF*)dstR.Get(), (IPaint*)paint.Get()));

    *bitmap = (IBitmap*)bmp.Get();
    (*bitmap)->AddRef();
    return NOERROR;
}

ECode CBitmapFactory::CreateBitmapEx3(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    AutoPtr<CBitmap> bm;
    FAIL_RETURN(CBitmap::NativeCreate(ArrayOf<Int32>(NULL, 0),
        0, width, width, height, (SkBitmap::Config)config, TRUE,
        (CBitmap**)&bm));
    bm->EraseColor(0); // start with black/transparent pixels
    *bitmap = (IBitmap*)bm.Get();
    (*bitmap)->AddRef();
    return NOERROR;
}

ECode CBitmapFactory::CreateBitmapEx4(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    if (!CBitmap::CheckWidthHeight(width, height)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (Math::Abs(stride) < width) {
        Logger::E(TAG, "Abs(stride) must be >= width");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 lastScanline = offset + (height - 1) * stride;
    Int32 length = colors.GetLength();
    if (offset < 0 || (offset + width > length) || lastScanline < 0 ||
        (lastScanline + width > length)) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    AutoPtr<CBitmap> bm;
    FAIL_RETURN(CBitmap::NativeCreate(
        colors, offset, stride, width, height, (SkBitmap::Config)config, FALSE,
        (CBitmap**)&bm));
    *bitmap = (IBitmap*)bm.Get();
    (*bitmap)->AddRef();
    return NOERROR;
}

ECode CBitmapFactory::CreateBitmapEx5(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    return CreateBitmapEx4(colors, 0, width, width, height, config, bitmap);
}

ECode CBitmapFactory::DecodeFile(
    /* [in] */ const String& pathName,
    /* [in] */ IBitmapFactoryOptions * pOpts,
    /* [out] */ IBitmap ** ppBitmap)
{
    if (!ppBitmap) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = NOERROR;
    AutoPtr<IFileInputStream> fstream;

    ec = CFileInputStream::New(pathName, (IFileInputStream**)&fstream);
    if (FAILED(ec)) {
        return ec;
    }

    ec = DecodeStream((IInputStream*)fstream.Get(), NULL, pOpts, ppBitmap);
    if (FAILED(ec)) {
        return ec;
    }

    fstream->Close();
    return ec;
}

ECode CBitmapFactory::DecodeFileEx(
    /* [in] */ const String& pathName,
    /* [out] */ IBitmap ** ppBitmap)
{
    if (!ppBitmap) {
        return E_INVALID_ARGUMENT;
    }
    return DecodeFile(pathName, NULL, ppBitmap);
}

ECode CBitmapFactory::DecodeResourceStream(
    /* [in] */ IResources* res,
    /* [in] */ ITypedValue* value,
    /* [in] */ IInputStream* is,
    /* [in] */ IRect* pad,
    /* [in] */ IBitmapFactoryOptions* _opts,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    AutoPtr<CBitmapFactoryOptions> opts = (CBitmapFactoryOptions*)_opts;
    if (opts == NULL) {
        CBitmapFactoryOptions::NewByFriend((CBitmapFactoryOptions**)&opts);
    }

    if (opts->mInDensity == 0 && value != NULL) {
        Int32 density = ((CTypedValue*)value)->mDensity;
        if (density == TypedValue_DENSITY_DEFAULT) {
            opts->mInDensity = CDisplayMetrics::DENSITY_DEFAULT;
        } else if (density != TypedValue_DENSITY_NONE) {
            opts->mInDensity = density;
        }
    }

    if (opts->mInTargetDensity == 0 && res != NULL) {
        AutoPtr<IDisplayMetrics> dm;
        res->GetDisplayMetrics((IDisplayMetrics**)&dm);
        opts->mInTargetDensity = ((CDisplayMetrics*)dm.Get())->mDensityDpi;
    }

    return DecodeStream(is, pad, opts, bitmap);
}

ECode CBitmapFactory::DecodeResource(
    /* [in] */ IResources* res,
    /* [in] */ Int32 id,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    AutoPtr<IInputStream> is;

    AutoPtr<ITypedValue> value;
    FAIL_RETURN(CTypedValue::New((ITypedValue**)&value));
    FAIL_RETURN(res->OpenRawResourceEx(id, value.Get(), (IInputStream**)&is));

    /*  do nothing.
        If the exception happened on open, bm will be NULL.
        If it happened on close, bm is still valid.
    */
    ECode ec = DecodeResourceStream(res, value, is, NULL, opts, bitmap);
    if (is != NULL) is->Close();
    return ec;
}

ECode CBitmapFactory::DecodeResourceEx(
    /* [in] */ IResources* res,
    /* [in] */ Int32 id,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeResource(res, id, NULL, bitmap);
}

ECode CBitmapFactory::DecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    if ((offset | length) < 0 || data.GetLength() < offset + length) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NativeDecodeByteArray(data, offset, length, opts, bitmap);
}

ECode CBitmapFactory::DecodeByteArrayEx(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeByteArray(data, offset, length, NULL, bitmap);
}

ECode CBitmapFactory::DecodeStream(
    /* [in] */ IInputStream* is,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    // we don't throw in this case, thus allowing the caller to only check
    // the cache, and not force the image to be decoded.
    VALIDATE_NOT_NULL(is);
    VALIDATE_NOT_NULL(bitmap);

    // we need mark/reset to work properly

    Boolean supported = FALSE;
    is->IsMarkSupported(&supported);
    if (!supported) {
//	        is = new BufferedInputStream(is, 16 * 1024);
    }

    // so we can call reset() if a given codec gives up after reading up to
    // this many bytes. FIXME: need to find out from the codecs what this
    // value should be.
    is->Mark(1024);

    AutoPtr<IBitmap> bm;
    AutoPtr<IAssetInputStream> assertIs;
    assertIs = (IAssetInputStream*)is->Probe(EIID_IAssetInputStream);
    if (assertIs != NULL) {
        Int32 a = 0;
        assertIs->GetAssetInt32(&a);
        FAIL_RETURN(NativeDecodeAsset(a, outPadding, opts, (IBitmap**)&bm));
    } else {
        // pass some temp storage down to the native code. 1024 is made up,
        // but should be large enough to avoid too many small calls back
        // into is.read(...) This number is not related to the value passed
        // to mark(...) above.
        AutoFree< ArrayOf<Byte> > tempStorage;
        if (opts != NULL
            && ((CBitmapFactoryOptions*)opts)->mInTempStorage != NULL) {
            tempStorage = ((CBitmapFactoryOptions*)opts)->mInTempStorage->Clone();
        }
        if (tempStorage == NULL) tempStorage = ArrayOf<Byte>::Alloc(16 * 1024);
        if (tempStorage == NULL) return E_OUT_OF_MEMORY_ERROR;

        ArrayOf<Byte> buf(tempStorage->GetPayload(), tempStorage->GetLength());
        FAIL_RETURN(NativeDecodeStream(is, &buf, outPadding, opts, (IBitmap**)&bm));
    }

    return FinishDecode(bm.Get(), outPadding, opts, bitmap);
}

ECode CBitmapFactory::DecodeStreamEx(
    /* [in] */ IInputStream* is,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeStream(is, NULL, NULL, bitmap);
}

ECode CBitmapFactory::DecodeFileDescriptor(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(fd);
    VALIDATE_NOT_NULL(bitmap);

    if (MemoryFile::IsMemoryFile(fd)) {
        Int32 mappedlength = MemoryFile::GetSize(fd);
        MemoryFile* file = new MemoryFile(fd, mappedlength, "r");
        if (!file) return E_OUT_OF_MEMORY_ERROR;
        AutoPtr<IInputStream> is;
        file->GetInputStream((IInputStream**)&is);
        AutoPtr<IBitmap> bm;
        FAIL_RETURN(DecodeStream(is.Get(), outPadding, opts, (IBitmap**)&bm));
        return FinishDecode(bm, outPadding, opts, bitmap);
    }

    AutoPtr<IBitmap> bm;
    NativeDecodeFileDescriptor(fd, outPadding, opts, (IBitmap**)&bm);
    return FinishDecode(bm.Get(), outPadding, opts, bitmap);
}

ECode CBitmapFactory::DecodeFileDescriptorEx(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeFileDescriptor(fd, NULL, NULL, bitmap);
}

ECode CBitmapFactory::SetDefaultConfig(
    /* [in] */ BitmapConfig config)
{
    if (config == -1) {
        // pick this for now, as historically it was our default.
        // However, if we have a smarter algorithm, we can change this.
        config = BitmapConfig_RGB_565;
    }
    NativeSetDefaultConfig(config);
    return NOERROR;
}

ECode CBitmapFactory::FinishDecode(
        /* [in] */ IBitmap* bm,
        /* [in] */ IRect* outPadding,
        /* [in] */ IBitmapFactoryOptions* _opts,
        /* [out] */ IBitmap** bitmap)
{
    AutoPtr<CBitmapFactoryOptions> opts = (CBitmapFactoryOptions*)_opts;

    if (!bm || !opts) {
        *bitmap = bm;
        if (*bitmap) (*bitmap)->AddRef();
        return NOERROR;
    }

    Int32 density = opts->mInDensity;
    if (density == 0) {
        *bitmap = bm;
        (*bitmap)->AddRef();
        return NOERROR;
    }

    bm->SetDensity(density);
    Int32 targetDensity = opts->mInTargetDensity;
    if (targetDensity == 0 || density == targetDensity || density == opts->mInScreenDensity) {
        *bitmap = bm;
        (*bitmap)->AddRef();
        return NOERROR;
    }

    AutoFree< ArrayOf<Byte> > np;
    bm->GetNinePatchChunk((ArrayOf<Byte>**)&np);
    const Boolean isNinePatch = np != NULL && NinePatch::IsNinePatchChunk(*np);
    if (opts->mInScaled || isNinePatch) {
        Float scale = targetDensity / (Float)density;
        // TODO: This is very inefficient and should be done in native by Skia
        AutoPtr<IBitmap> oldBitmap = bm;
        Int32 width = 0, height = 0;
        bm->GetWidth(&width);
        bm->GetHeight(&height);
        FAIL_RETURN(CBitmap::CreateScaledBitmap(
            (IBitmapFactory*)this,
            oldBitmap,
            (Int32)(width * scale + 0.5f),
            (Int32)(height * scale + 0.5f), TRUE, &bm));
        oldBitmap->Recycle();
        if (isNinePatch) {
            FAIL_RETURN(NativeScaleNinePatch(*np, scale, outPadding));
            bm->SetNinePatchChunk(*np);
        }
        bm->SetDensity(targetDensity);
    }

    *bitmap = bm;
    (*bitmap)->AddRef();
    return NOERROR;
}

ECode CBitmapFactory::NativeDecodeStream(
    /* [in] */ IInputStream* pIs,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ IRect* pOutPadding,
    /* [in] */ IBitmapFactoryOptions* pOpts,
    /* [out] */ IBitmap ** ppBitmap)
{
    ECode ec = NOERROR;
    SkStream* stream = CreateInputStreamAdaptor(pIs, storage, 0);

    if (stream) {
        // for now we don't allow purgeable with java inputstreams
        ec = DoDecode(stream, pOutPadding, pOpts, FALSE, FALSE, ppBitmap);
        stream->unref();
    }
    return ec;
}

ECode CBitmapFactory::DoDecode(
    /* [in] */ SkStream* stream,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [in] */ Boolean allowPurgeable,
    /* [in] */ Boolean forcePurgeable, // default is false;
    /* [out] */ IBitmap ** ppBitmap)
{
    Int32 sampleSize = 1;
    SkImageDecoder::Mode mode = SkImageDecoder::kDecodePixels_Mode;
    SkBitmap::Config prefConfig = SkBitmap::kNo_Config;
    Boolean doDither = TRUE;
    AutoPtr<CBitmapFactoryOptions> coptions = (CBitmapFactoryOptions*)options;
    Boolean isPurgeable = forcePurgeable
        || (allowPurgeable && (options != NULL && coptions->mInPurgeable));
    Boolean preferQualityOverSpeed = FALSE;

    if (NULL != options) {
        sampleSize = coptions->mInSampleSize;
        if (coptions->mInJustDecodeBounds) {
            mode = SkImageDecoder::kDecodeBounds_Mode;
        }
        // initialize these, in case we fail later on
        coptions->mOutWidth = -1;
        coptions->mOutHeight = -1;
        coptions->mOutMimeType = String(NULL);

        Int32 config = coptions->mInPreferredConfig;
        prefConfig = Graphics::GetNativeBitmapConfig(config);
        doDither = coptions->mInDither;
        preferQualityOverSpeed = coptions->mInPreferQualityOverSpeed;
    }

    SkImageDecoder* decoder = SkImageDecoder::Factory(stream);
    if (NULL == decoder) {
        Logger::E(TAG, "SkImageDecoder::Factory returned null");
        *ppBitmap = NULL;
        return E_FAIL;
    }

    decoder->setSampleSize(sampleSize);
    decoder->setDitherImage(doDither);
    decoder->setPreferQualityOverSpeed(preferQualityOverSpeed);

    NinePatchPeeker                 peeker(decoder);
    Graphics::PixelAllocator        allocator;
    SkBitmap*                       bitmap = new SkBitmap;
    android::Res_png_9patch         dummy9Patch;

    SkAutoTDelete<SkImageDecoder>   add(decoder);
    SkAutoTDelete<SkBitmap>         adb(bitmap);

    decoder->setPeeker(&peeker);
    if (!isPurgeable) {
        decoder->setAllocator(&allocator);
    }

    AutoDecoderCancel               adc(options, decoder);

    // To fix the race condition in case "requestCancelDecode"
    // happens earlier than AutoDecoderCancel object is added
    // to the gAutoDecoderCancelMutex linked list.
    if (NULL != options && coptions->mCancel) {
        Logger::E(TAG, "options has been cancelled.");
        return E_FAIL;
    }

    SkImageDecoder::Mode decodeMode = mode;
    if (isPurgeable) {
        decodeMode = SkImageDecoder::kDecodeBounds_Mode;
    }
    if (!decoder->decode(stream, bitmap, prefConfig, decodeMode)) {
        Logger::E(TAG, "decoder->decode returned false");
        *ppBitmap = NULL;
        return E_FAIL;
    }

    // update options (if any)
    if (NULL != options) {
        coptions->mOutWidth = bitmap->width();
        coptions->mOutHeight = bitmap->height();
        // TODO: set the mimeType field with the data from the codec.
        // but how to reuse a set of strings, rather than allocating new one
        // each time?
        GetMimeTypeString(decoder->getFormat(), &coptions->mOutMimeType);
    }

    // if we're in justBounds mode, return now (skip the java bitmap)
    if (SkImageDecoder::kDecodeBounds_Mode == mode) {
        Logger::E(TAG, "we're in justBounds mode");
        return E_FAIL;
    }

    AutoFree< ArrayOf<Byte> > ninePatchChunk;
    if (peeker.mPatchIsValid) {
        size_t ninePatchArraySize = peeker.mPatch->serializedSize();
        ninePatchChunk = ArrayOf<Byte>::Alloc(ninePatchArraySize);
        if (NULL == ninePatchChunk) {
            Logger::E(TAG, "ninePatchChunk == null");
            return E_OUT_OF_MEMORY_ERROR;
        }
        Byte* array = ninePatchChunk->GetPayload();
        peeker.mPatch->serialize(array);
    }

    // detach bitmap from its autotdeleter, since we want to own it now
    adb.detach();

    if (padding != NULL) {
        if (peeker.mPatchIsValid) {
            padding->Set(peeker.mPatch->paddingLeft,
                         peeker.mPatch->paddingTop,
                         peeker.mPatch->paddingRight,
                         peeker.mPatch->paddingBottom);
        } else {
            padding->Set(-1, -1, -1, -1);
        }
    }

    SkPixelRef* pr = NULL;
    if (isPurgeable) {
        pr = InstallPixelRef(bitmap, stream, sampleSize, doDither);
    } else {
        // if we get here, we're in kDecodePixels_Mode and will therefore
        // already have a pixelref installed.
        pr = bitmap->pixelRef();
    }
    // promise we will never change our pixels (great for sharing and pictures)
    pr->setImmutable();
    // now create the java bitmap
    CBitmap* bm = NULL;
    FAIL_RETURN(CBitmap::NewByFriend((Int32)bitmap, FALSE, *ninePatchChunk, -1, &bm));
    *ppBitmap = (IBitmap*)bm->Probe(EIID_IBitmap);
    return NOERROR;
}

SkPixelRef* CBitmapFactory::InstallPixelRef(
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ SkStream* stream,
    /* [in] */ Int32 sampleSize,
    /* [in] */ Boolean ditherImage)
{
    SkImageRef* pr;
    // only use ashmem for large images, since mmaps come at a price
    if (bitmap->getSize() >= 32 * 1024) {
        pr = new SkImageRef_ashmem(stream, bitmap->config(), sampleSize);
    } else {
        pr = new SkImageRef_GlobalPool(stream, bitmap->config(), sampleSize);
    }
    pr->setDitherImage(ditherImage);
    bitmap->setPixelRef(pr)->unref();
    pr->isOpaque(bitmap);
    return pr;
}

void CBitmapFactory::GetMimeTypeString(
    /* [in] */ SkImageDecoder::Format format,
    /* [out] */ String* typeStr)
{
    assert(typeStr);

    static const struct {
        SkImageDecoder::Format fFormat;
        const char*            fMimeType;
    } gMimeTypes[] = {
        { SkImageDecoder::kBMP_Format,  "image/bmp" },
        { SkImageDecoder::kGIF_Format,  "image/gif" },
        { SkImageDecoder::kICO_Format,  "image/x-ico" },
        { SkImageDecoder::kJPEG_Format, "image/jpeg" },
        { SkImageDecoder::kPNG_Format,  "image/png" },
        { SkImageDecoder::kWBMP_Format, "image/vnd.wap.wbmp" }
    };

    const char* cstr = NULL;
    for (size_t i = 0; i < SK_ARRAY_COUNT(gMimeTypes); i++) {
        if (gMimeTypes[i].fFormat == format) {
            cstr = gMimeTypes[i].fMimeType;
            break;
        }
    }

    *typeStr = cstr;
}

SkStream* CBitmapFactory::CopyAssetToStream(
    /* [in] */ android::Asset* asset)
{
    // if we could "ref/reopen" the asset, we may not need to copy it here
    off_t size = asset->seek(0, SEEK_SET);
    if ((off_t)-1 == size) {
        Logger::E(TAG, "---- copyAsset: asset rewind failed\n");
        return NULL;
    }

    size = asset->getLength();
    if (size <= 0) {
        Logger::E(TAG, "---- copyAsset: asset->getLength() returned %d\n" + (Int32)size);
        return NULL;
    }

    SkStream* stream = new SkMemoryStream(size);
    void* data = const_cast<void*>(stream->getMemoryBase());
    off_t len = asset->read(data, size);
    if (len != size) {
        Logger::E(TAG, "---- copyAsset: asset->read(%d) returned %d\n" + (Int32)size + (Int32)len);
        delete stream;
        stream = NULL;
    }
    return stream;
}

void CBitmapFactory::NativeSetDefaultConfig(
    /* [in] */ Int32 nativeConfig)
{
    SkBitmap::Config config = static_cast<SkBitmap::Config>(nativeConfig);

    // these are the only default configs that make sense for codecs right now
    static const SkBitmap::Config gValidDefConfig[] = {
        SkBitmap::kRGB_565_Config,
        SkBitmap::kARGB_8888_Config,
    };

    for (size_t i = 0; i < SK_ARRAY_COUNT(gValidDefConfig); i++) {
        if (config == gValidDefConfig[i]) {
            SkImageDecoder::SetDeviceConfig(config);
            break;
        }
    }
}

ECode CBitmapFactory::NativeDecodeFileDescriptor(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    assert(fd);

    Int32 descriptor = 0;
    fd->GetDescriptor(&descriptor);

    CBitmapFactoryOptions* copts = (CBitmapFactoryOptions*)opts;
    Boolean isPurgeable = copts != NULL && copts->mInPurgeable;
    Boolean isShareable = copts != NULL && copts->mInInputShareable;
    Boolean weOwnTheFD = FALSE;
    if (isPurgeable && isShareable) {
        Int32 newFD = ::dup(descriptor);
        if (-1 != newFD) {
            weOwnTheFD = TRUE;
            descriptor = newFD;
        }
    }

    SkFDStream* stream = new SkFDStream(descriptor, weOwnTheFD);
    SkAutoUnref aur(stream);
    if (!stream->isValid()) {
        *bitmap = NULL;
        return E_RUNTIME_EXCEPTION;
    }

    /* Restore our offset when we leave, so we can be called more than once
       with the same descriptor. This is only required if we didn't dup the
       file descriptor, but it is OK to do it all the time.
    */
    AutoFDSeek as(descriptor);

    /* Allow purgeable iff we own the FD, i.e., in the puregeable and
       shareable case.
    */
    return DoDecode(stream, padding, copts, weOwnTheFD, FALSE, bitmap);
}

ECode CBitmapFactory::NativeDecodeAsset(
    /* [in] */ Int32 native_asset,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    SkStream* stream = NULL;
    android::Asset* asset = reinterpret_cast<android::Asset*>(native_asset);
    Boolean forcePurgeable = opts != NULL
        && ((CBitmapFactoryOptions*)opts)->mInPurgeable;
    if (forcePurgeable) {
        // if we could "ref/reopen" the asset, we may not need to copy it here
        // and we could assume optionsShareable, since assets are always RO
        stream = CopyAssetToStream(asset);
        if (NULL == stream) {
            return E_OUT_OF_MEMORY_ERROR;
        }
    } else {
        // since we know we'll be done with the asset when we return, we can
        // just use a simple wrapper
        stream = new AssetStreamAdaptor(asset);
    }
    SkAutoUnref aur(stream);
    return DoDecode(stream, padding, opts, TRUE, forcePurgeable, bitmap);
}

ECode CBitmapFactory::NativeDecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    /*  If optionsShareable() we could decide to just wrap the java array and
        share it, but that means adding a globalref to the java array object
        and managing its lifetime. For now we just always copy the array's data
        if optionsPurgeable().
     */
    Byte* ar = data.GetPayload();
    CBitmapFactoryOptions* copts = (CBitmapFactoryOptions*)opts;
    Boolean isPurgeable = copts != NULL && copts->mInPurgeable;
    SkStream* stream = new SkMemoryStream(ar + offset, length, isPurgeable);
    SkAutoUnref aur(stream);
    return DoDecode(stream, NULL, opts, TRUE, FALSE, bitmap);
}

ECode CBitmapFactory::NativeScaleNinePatch(
    /* [in] */ const ArrayOf<Byte>& chunkObject,
    /* [in] */ Float scale,
    /* [in] */ IRect* padding)
{
    Byte* array = chunkObject.GetPayload();
    if (array != NULL) {
        Int32 chunkSize = chunkObject.GetLength();
        void* storage = alloca(chunkSize);
        android::Res_png_9patch* chunk = static_cast<android::Res_png_9patch*>(storage);
        memcpy(chunk, array, chunkSize);
        android::Res_png_9patch::deserialize(chunk);

        chunk->paddingLeft = int(chunk->paddingLeft * scale + 0.5f);
        chunk->paddingTop = int(chunk->paddingTop * scale + 0.5f);
        chunk->paddingRight = int(chunk->paddingRight * scale + 0.5f);
        chunk->paddingBottom = int(chunk->paddingBottom * scale + 0.5f);

        for (int i = 0; i < chunk->numXDivs; i++) {
            chunk->xDivs[i] = int(chunk->xDivs[i] * scale + 0.5f);
            if (i > 0 && chunk->xDivs[i] == chunk->xDivs[i - 1]) {
                chunk->xDivs[i]++;
            }
        }

        for (int i = 0; i < chunk->numYDivs; i++) {
            chunk->yDivs[i] = int(chunk->yDivs[i] * scale + 0.5f);
            if (i > 0 && chunk->yDivs[i] == chunk->yDivs[i - 1]) {
                chunk->yDivs[i]++;
            }
        }

        memcpy(array, chunk, chunkSize);

        if (padding) {
            padding->Set(chunk->paddingLeft, chunk->paddingTop,
                         chunk->paddingRight, chunk->paddingBottom);
        }
    }
    return NOERROR;
}

