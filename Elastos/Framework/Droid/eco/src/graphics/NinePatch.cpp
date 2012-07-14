
#include "graphics/NinePatch.h"
#include "graphics/CBitmap.h"
#include "graphics/Canvas.h"
#include "graphics/CPaint.h"
#include "graphics/CRegion.h"
#include "graphics/Graphics.h"
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

extern ECode NinePatch_Draw(SkCanvas* canvas, const SkRect& bounds,
        const SkBitmap& bitmap, const android::Res_png_9patch& chunk,
        const SkPaint* paint, SkRegion** outRegion);

const char* NinePatch::TAG = "NinePatch";

NinePatch::NinePatch()
{
}

void NinePatch::SetPaint(
    /* [in] */ IPaint* p)
{
    mPaint = p;
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRectF* location)
{
    VALIDATE_NOT_NULL(canvas);

    return NativeDrawF(((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas,
        location,
        ((CBitmap*)mBitmap.Get())->Ni(), *mChunk,
        mPaint != NULL ? ((Paint*)mPaint->Probe(EIID_Paint))->mNativePaint : NULL,
        ((Canvas*)canvas->Probe(EIID_Canvas))->mDensity,
        ((CBitmap*)mBitmap.Get())->mDensity);
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location)
{
    VALIDATE_NOT_NULL(canvas);

    return NativeDrawI(((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas,
        location,
        ((CBitmap*)mBitmap.Get())->Ni(), *mChunk,
        mPaint != NULL ? ((Paint*)mPaint->Probe(EIID_Paint))->mNativePaint : NULL,
        ((Canvas*)canvas->Probe(EIID_Canvas))->mDensity,
        ((CBitmap*)mBitmap.Get())->mDensity);
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(canvas);

    //printf("NinePatch::Draw---name = %s, left = %f, right = %f, top = %f, bottom = %f, destDensity = %d, srcDensity = %d\n",
    //    (const char*)mSrcName, ((CRect*)location)->mLeft, ((CRect*)location)->mRight,
    //    ((CRect*)location)->mTop, ((CRect*)location)->mBottom,
    //    ((Canvas*)canvas->Probe(EIID_Canvas))->mDensity,
    //    ((CBitmap*)mBitmap.Get())->mDensity);

    Logger::D("NinePatch", StringBuffer("Draw") + mSrcName);
    return NativeDrawI(((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas, location,
        ((CBitmap*)mBitmap.Get())->Ni(), *mChunk,
        paint != NULL ? ((Paint*)paint->Probe(EIID_Paint))->mNativePaint : NULL,
        ((Canvas*)canvas->Probe(EIID_Canvas))->mDensity,
        ((CBitmap*)mBitmap.Get())->mDensity);
}

ECode NinePatch::GetDensity(
    /* [out] */ Int32* density)
{
    return mBitmap->GetDensity(density);
}

ECode NinePatch::GetWidth(
    /* [out] */ Int32* width)
{
    return mBitmap->GetWidth(width);
}

ECode NinePatch::GetHeight(
    /* [out] */ Int32* height)
{
    return mBitmap->GetHeight(height);
}

ECode NinePatch::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    return mBitmap->HasAlpha(hasAlpha);
}

ECode NinePatch::GetTransparentRegion(
    /* [in] */ IRect* location,
    /* [out] */ IRegion** region)
{
    Int32 r = NativeGetTransparentRegion(((CBitmap*)mBitmap.Get())->Ni(), *mChunk, location);
    if (r == 0) {
        return E_RUNTIME_EXCEPTION;
    }

    return CRegion::New(r, region);
}

Boolean NinePatch::IsNinePatchChunk(
    /* [in] */ const ArrayOf<Byte>& chunk)
{
    if (chunk.GetLength() < (Int32)sizeof(android::Res_png_9patch)) {
        return FALSE;
    }

    Byte* array = chunk.GetPayload();
    if (array != NULL) {
        const android::Res_png_9patch* chunk =
            reinterpret_cast<const android::Res_png_9patch*>(array);
        int8_t wasDeserialized = chunk->wasDeserialized;
        return wasDeserialized != -1;
    }
    return FALSE;
}

ECode NinePatch::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte> & chunk,
    /* [in] */ const String& srcName)
{
    VALIDATE_NOT_NULL(bitmap);

    if (chunk.GetPayload() == NULL) {
        return E_INVALID_ARGUMENT;
    }

    mBitmap = bitmap;
    mChunk = chunk.Clone();
    mSrcName = srcName;
    return ValidateNinePatchChunk(((CBitmap*)mBitmap.Get())->Ni(), chunk);
}

ECode NinePatch::constructor(
    /* [in] */ NinePatch* patch)
{
    assert(patch);

    mBitmap = patch->mBitmap;
    mChunk = patch->mChunk->Clone();
    mSrcName = patch->mSrcName;
    if (patch->mPaint != NULL) {
        FAIL_RETURN(CPaint::New(patch->mPaint.Get(), (IPaint**)&mPaint));
    }
    return ValidateNinePatchChunk(((CBitmap*)mBitmap.Get())->Ni(), *mChunk);
}

ECode NinePatch::ValidateNinePatchChunk(
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte>& chunk)
{
    if (chunk.GetLength() < (Int32)sizeof(android::Res_png_9patch)) {
        Logger::E(TAG, "Array too small for chunk.");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode NinePatch::NativeDrawF(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRectF* boundsRect,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte>& chunk,
    /* [in] */ SkPaint* paint, // may be null
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    assert(canvas);
    assert(boundsRect);
    assert(bitmap);
    assert(chunk.GetPayload());
    // paint is optional

    SkRect bounds;
    Graphics::IRectF2SkRect(boundsRect, &bounds);

    return NativeDraw(canvas, bounds, bitmap, chunk, paint, destDensity, srcDensity);
}

ECode NinePatch::NativeDrawI(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRect* boundsRect,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte>& chunk,
    /* [in] */ SkPaint* paint, // may be null
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    assert(canvas);
    assert(boundsRect);
    assert(bitmap);
    assert(chunk.GetPayload());
    // paint is optional

    SkRect bounds;
    Graphics::IRect2SkRect(boundsRect, &bounds);

    return NativeDraw(canvas, bounds, bitmap, chunk, paint, destDensity, srcDensity);
}

ECode NinePatch::NativeDraw(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkRect& bounds,
    /* [in] */ const SkBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte>& chunkObj,
    /* [in] */ const SkPaint* paint,
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    ECode ec = NOERROR;
    Int32 chunkSize = chunkObj.GetLength();
    void* storage = chunkObj.GetPayload();
    // need to deserialize the chunk
    android::Res_png_9patch* chunk = static_cast<android::Res_png_9patch*>(storage);
    assert(chunkSize == (Int32)chunk->serializedSize());
    // this relies on deserialization being done in place
    android::Res_png_9patch::deserialize(chunk);

    if (destDensity == srcDensity || destDensity == 0
            || srcDensity == 0) {
        Logger::V(TAG, StringBuffer("Drawing unscaled 9-patch: (")
            + (Int32)SkScalarToFloat(bounds.fLeft) + ","
            + (Int32)SkScalarToFloat(bounds.fTop) + ")-("
            + (Int32)SkScalarToFloat(bounds.fRight)+ ","
            + (Int32)SkScalarToFloat(bounds.fBottom) + ")=");

        ec = NinePatch_Draw(canvas, bounds, *bitmap, *chunk, paint, NULL);
    }
    else {
        canvas->save();

        SkScalar scale = SkFloatToScalar(destDensity / (float)srcDensity);
        canvas->translate(bounds.fLeft, bounds.fTop);
        canvas->scale(scale, scale);

        bounds.fRight = SkScalarDiv(bounds.fRight-bounds.fLeft, scale);
        bounds.fBottom = SkScalarDiv(bounds.fBottom-bounds.fTop, scale);
        bounds.fLeft = bounds.fTop = 0;

        Logger::V(TAG, StringBuffer("Drawing scaled 9-patch: (")
                + (Int32)SkScalarToFloat(bounds.fLeft) + ","
                + (Int32)SkScalarToFloat(bounds.fTop) + ")-("
                + (Int32)SkScalarToFloat(bounds.fRight)+ ","
                + (Int32)SkScalarToFloat(bounds.fBottom) + ") srcDensity="
                + srcDensity + " destDensity="+ destDensity);

        ec = NinePatch_Draw(canvas, bounds, *bitmap, *chunk, paint, NULL);

        canvas->restore();
    }

    return ec;
}

Int32 NinePatch::NativeGetTransparentRegion(
    /* [in] */ const SkBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte>& chunkObj,
    /* [in] */ IRect* boundsRect)
{
    assert(bitmap);
    assert(chunkObj.GetPayload() != NULL);
    assert(boundsRect);

    SkRect bounds;
    Graphics::IRect2SkRect(boundsRect, &bounds);
    Int32 chunkSize = chunkObj.GetLength();
    void* storage = chunkObj.GetPayload();
    // need to deserialize the chunk
    android::Res_png_9patch* chunk = static_cast<android::Res_png_9patch*>(storage);
    assert(chunkSize == (Int32)chunk->serializedSize());
    // this relies on deserialization being done in place
    android::Res_png_9patch::deserialize(chunk);
    SkRegion* region = NULL;
    NinePatch_Draw(NULL, bounds, *bitmap, *chunk, NULL, &region);
    return (Int32)region;
}

