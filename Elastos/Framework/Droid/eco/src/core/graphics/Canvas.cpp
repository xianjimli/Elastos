
#include "graphics/Graphics.h"
#include "graphics/Canvas.h"
#include "graphics/CBitmap.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include "graphics/CPaint.h"
#include "graphics/CMatrix.h"
#include "graphics/CPath.h"
#include "graphics/CRegion.h"
#include "graphics/DrawFilter.h"
#include <skia/core/SkDevice.h>
#include <skia/core/SkScalar.h>
#include <skia/core/SkMatrix.h>
#include <skia/utils/SkGLCanvas.h>
#include <skia/core/SkGraphics.h>
#include <skia/images/SkImageRef_GlobalPool.h>
#include <skia/effects/SkPorterDuff.h>
#include <skia/core/SkShader.h>
#include <skia/core/SkTemplates.h>
#include <elastos/Math.h>
#include <elastos/Character.h>
#include <Logger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

// {CC4FB366-48F0-48FF-A6B6-670E64F46A7B}
extern "C" const InterfaceID EIID_Canvas =
    { 0xcc4fb366, 0x48f0, 0x48ff, { 0xa6, 0xb6, 0x67, 0xe, 0x64, 0xf4, 0x6a, 0x7b } };


Canvas::Canvas()
    : mDensity(Bitmap_DENSITY_NONE)
    , mScreenDensity(Bitmap_DENSITY_NONE)
{
    // 0 means no native bitmap
    mNativeCanvas = InitRaster(0);
}

Canvas::~Canvas()
{
    // If the constructor threw an exception before setting mNativeCanvas, the native finalizer
    // must not be invoked.
    if (mNativeCanvas != NULL) {
        Finalizer(mNativeCanvas);
    }
}

/**
 * Return the GL object associated with this canvas, or null if it is not
 * backed by GL.
 */
ECode Canvas::GetGL(
    /* [out] */ IGL** gl)
{
    assert(gl != NULL);
    *gl = mGL;
    if (*gl) (*gl)->AddRef();
    return NOERROR;
}

/**
 * Call this to free up OpenGL resources that may be cached or allocated
 * on behalf of the Canvas. Any subsequent drawing with a GL-backed Canvas
 * will have to recreate those resources.
 */
void Canvas::FreeGlCaches()
{
    FreeCaches();
}

/**
 * Specify a bitmap for the canvas to draw into.  As a side-effect, also
 * updates the canvas's target density to match that of the bitmap.
 *
 * @param bitmap Specifies a mutable bitmap for the canvas to draw into.
 *
 * @see #setDensity(int)
 * @see #getDensity()
 */
ECode Canvas::SetBitmap(
    /* [in] */ IBitmap* _bitmap)
{
    CBitmap* bitmap = (CBitmap*)_bitmap;

    Boolean isMutable;
    bitmap->IsMutable(&isMutable);
    if (!isMutable) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (mGL != NULL) {
//        throw new RuntimeException("Can't set a bitmap device on a GL canvas");
        return E_RUNTIME_EXCEPTION;
    }
    FAIL_RETURN(ThrowIfRecycled(bitmap));

    NativeSetBitmap(mNativeCanvas, bitmap->Ni());
    mBitmap = bitmap;
    mDensity = bitmap->mDensity;
    return NOERROR;
}

/**
 * Set the viewport dimensions if this canvas is GL based. If it is not,
 * this method is ignored and no exception is thrown.
 *
 *  @param width    The width of the viewport
 *  @param height   The height of the viewport
 */
ECode Canvas::SetViewport(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mGL != NULL) {
        NativeSetViewport(mNativeCanvas, width, height);
    }
    return NOERROR;
}

/**
 * Return true if the device that the current layer draws into is opaque
 * (i.e. does not support per-pixel alpha).
 *
 * @return true if the device that the current layer draws into is opaque
 */
ECode Canvas::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    assert(isOpaque != NULL);

    /*
        Currently we cannot support transparency in GL-based canvas' at
        the view level. Therefore we cannot base our answer on the device's
        bitmap, but need to hard-code the answer. If we relax this
        limitation in views, we can simplify the following code as well.

        Use the getViewport() call to find out if we're gl-based...
    */
    if (mNativeCanvas->getViewport(NULL)) {
        *isOpaque = TRUE;
        return NOERROR;
    }

    // normal technique, rely on the device's bitmap for the answer
    *isOpaque = mNativeCanvas->getDevice()->accessBitmap(false).isOpaque();
    return NOERROR;
}

/**
 * Returns the width of the current drawing layer
 *
 * @return the width of the current drawing layer
 */
ECode Canvas::GetWidth(
    /* [out] */ Int32* width)
{
    assert(width != NULL);

    *width = mNativeCanvas->getDevice()->accessBitmap(false).width();
    return NOERROR;
}

/**
 * Returns the height of the current drawing layer
 *
 * @return the height of the current drawing layer
 */
ECode Canvas::GetHeight(
    /* [out] */ Int32* height)
{
    assert(height != NULL);

    *height = mNativeCanvas->getDevice()->accessBitmap(false).height();
    return NOERROR;
}

/**
 * <p>Returns the target density of the canvas.  The default density is
 * derived from the density of its backing bitmap, or
 * {@link Bitmap#DENSITY_NONE} if there is not one.</p>
 *
 * @return Returns the current target density of the canvas, which is used
 * to determine the scaling factor when drawing a bitmap into it.
 *
 * @see #setDensity(int)
 * @see Bitmap#getDensity()
 */
ECode Canvas::GetDensity(
    /* [out] */ Int32* density)
{
    assert(density != NULL);

    *density = mDensity;
    return NOERROR;
}

/**
 * <p>Specifies the density for this Canvas' backing bitmap.  This modifies
 * the target density of the canvas itself, as well as the density of its
 * backing bitmap via {@link Bitmap#setDensity(int) Bitmap.setDensity(int)}.
 *
 * @param density The new target density of the canvas, which is used
 * to determine the scaling factor when drawing a bitmap into it.  Use
 * {@link Bitmap#DENSITY_NONE} to disable bitmap scaling.
 *
 * @see #getDensity()
 * @see Bitmap#setDensity(int)
 */
ECode Canvas::SetDensity(
    /* [in] */ Int32 density)
{
    if (mBitmap != NULL) {
        mBitmap->SetDensity(density);
    }
    mDensity = density;
    return NOERROR;
}

ECode Canvas::SetScreenDensity(
    /* [in] */ Int32 density)
{
    mScreenDensity = density;
    return NOERROR;
}

/**
 * Saves the current matrix and clip onto a private stack. Subsequent
 * calls to translate,scale,rotate,skew,concat or clipRect,clipPath
 * will all operate as usual, but when the balancing call to restore()
 * is made, those calls will be forgotten, and the settings that existed
 * before the save() will be reinstated.
 *
 * @return The value to pass to restoreToCount() to balance this save()
 */
ECode Canvas::Save(
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = mNativeCanvas->save();
    return NOERROR;
}

/**
 * Based on saveFlags, can save the current matrix and clip onto a private
 * stack. Subsequent calls to translate,scale,rotate,skew,concat or
 * clipRect,clipPath will all operate as usual, but when the balancing
 * call to restore() is made, those calls will be forgotten, and the
 * settings that existed before the save() will be reinstated.
 *
 * @param saveFlags flag bits that specify which parts of the Canvas state
 *                  to save/restore
 * @return The value to pass to restoreToCount() to balance this save()
 */
ECode Canvas::SaveEx(
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = mNativeCanvas->save((SkCanvas::SaveFlags)saveFlags);
    return NOERROR;
}

/**
 * This behaves the same as save(), but in addition it allocates an
 * offscreen bitmap. All drawing calls are directed there, and only when
 * the balancing call to restore() is made is that offscreen transfered to
 * the canvas (or the previous layer). Subsequent calls to translate,
 * scale, rotate, skew, concat or clipRect, clipPath all operate on this
 * copy. When the balancing call to restore() is made, this copy is
 * deleted and the previous matrix/clip state is restored.
 *
 * @param bounds May be null. The maximum size the offscreen bitmap
 *               needs to be (in local coordinates)
 * @param paint  This is copied, and is applied to the offscreen when
 *               restore() is called.
 * @param saveFlags  see _SAVE_FLAG constants
 * @return       value to pass to restoreToCount() to balance this save()
 */
ECode Canvas::SaveLayer(
    /* [in] */ IRectF* bounds,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    SkPaint* nativePaint = NULL;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    *count = NativeSaveLayer(mNativeCanvas, bounds, nativePaint, saveFlags);
    return NOERROR;
}

/**
 * Helper version of saveLayer() that takes 4 values rather than a RectF.
 */
ECode Canvas::SaveLayerEx(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    SkPaint* nativePaint = NULL;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    *count = NativeSaveLayer(mNativeCanvas, left, top, right, bottom,
            nativePaint, saveFlags);
    return NOERROR;
}

ECode Canvas::SaveLayerAlpha(
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    alpha = Math::Min(255, Math::Max(0, alpha));
    *count = NativeSaveLayerAlpha(mNativeCanvas, bounds, alpha, saveFlags);
    return NOERROR;
}

ECode Canvas::SaveLayerAlphaEx(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = NativeSaveLayerAlpha(mNativeCanvas, left, top, right, bottom,
            alpha, saveFlags);
    return NOERROR;
}

ECode Canvas::Restore()
{
    if (mNativeCanvas->getSaveCount() <= 1) {  // cannot restore anymore
        Logger::W("Canvas", "Underflow in restore");
        return E_INVALID_ARGUMENT;
    }
    mNativeCanvas->restore();
    return NOERROR;
}

ECode Canvas::GetSaveCount(
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = mNativeCanvas->getSaveCount();
    return NOERROR;
}

ECode Canvas::RestoreToCount(
    /* [in] */ Int32 saveCount)
{
    if (saveCount < 1) {
        Logger::W("Canvas", "Underflow in restoreToCount");
        return E_INVALID_ARGUMENT;
    }
    mNativeCanvas->restoreToCount(saveCount);
    return NOERROR;
}

ECode Canvas::Translate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    mNativeCanvas->translate(dx_, dy_);
    return NOERROR;
}

ECode Canvas::Scale(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    mNativeCanvas->scale(sx_, sy_);
    mNativeCanvas->scale(sx_, sy_);
    return NOERROR;
}

ECode Canvas::ScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    Translate(px, py);
    Scale(sx, sy);
    Translate(-px, -py);
    return NOERROR;
}

ECode Canvas::Rotate(
    /* [in] */ Float degrees)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    mNativeCanvas->rotate(degrees_);
    return NOERROR;
}

ECode Canvas::RotateEx(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    Translate(px, py);
    Rotate(degrees);
    Translate(-px, -py);
    return NOERROR;
}

ECode Canvas::Skew(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    mNativeCanvas->skew(sx_, sy_);
    return NOERROR;
}

ECode Canvas::Concat(
    /* [in] */ IMatrix* matrix)
{
    assert(matrix != NULL);

    SkMatrix* nativeMatrix = ((CMatrix*)matrix)->mNativeInstance;
    NativeConcat(mNativeCanvas, nativeMatrix);
    return NOERROR;
}

ECode Canvas::SetMatrix(
    /* [in] */ IMatrix* matrix)
{
    SkMatrix* nativeMatrix = NULL;
    if (matrix != NULL) {
        nativeMatrix = ((CMatrix*)matrix)->mNativeInstance;
    }
    NativeSetMatrix(mNativeCanvas, nativeMatrix);
    return NOERROR;
}

ECode Canvas::GetMatrix(
    /* [in, out] */ IMatrix* ctm)
{
    assert(ctm != NULL);

    SkMatrix* nativeMatrix = ((CMatrix*)ctm)->mNativeInstance;
    NativeGetCTM(mNativeCanvas, nativeMatrix);
    return NOERROR;
}

ECode Canvas::GetMatrixEx(
    /* [out] */ IMatrix** metrix)
{
    assert(metrix != NULL);

    FAIL_RETURN(CMatrix::New(metrix));
    return GetMatrix(*metrix);
}

ECode Canvas::ClipRect(
    /* [in] */ IRectF* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(rect != NULL);

    CRectF* rect_ = (CRectF*)rect;
    *isNotEmpty = NativeClipRect(mNativeCanvas,
            rect_->mLeft, rect_->mTop, rect_->mRight, rect_->mBottom, op);
    return NOERROR;
}

ECode Canvas::ClipRectEx(
    /* [in] */ IRect* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(rect != NULL);

    CRect* rect_ = (CRect*)rect;
    *isNotEmpty = NativeClipRect(mNativeCanvas,
            rect_->mLeft, rect_->mTop, rect_->mRight, rect_->mBottom, op);
    return NOERROR;
}

ECode Canvas::ClipRectEx2(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(rect != NULL);

    CRectF* rect_ = (CRectF*)rect;
    SkRect tmp;
    tmp.set(SkFloatToScalar(rect_->mLeft), SkFloatToScalar(rect_->mTop),
            SkFloatToScalar(rect_->mRight), SkFloatToScalar(rect_->mBottom));
    *isNotEmpty = mNativeCanvas->clipRect(tmp);
    return NOERROR;
}

ECode Canvas::ClipRectEx3(
    /* [in] */ IRect* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(rect != NULL);

    CRect* rect_ = (CRect*)rect;
    SkRect tmp;
    tmp.set(SkIntToScalar(rect_->mLeft), SkIntToScalar(rect_->mTop),
            SkIntToScalar(rect_->mRight), SkIntToScalar(rect_->mBottom));
    *isNotEmpty = mNativeCanvas->clipRect(tmp);
    return NOERROR;
}

ECode Canvas::ClipRectEx4(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    *isNotEmpty = NativeClipRect(mNativeCanvas, left, top, right, bottom, op);
    return NOERROR;
}

ECode Canvas::ClipRectEx5(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    SkRect r;
    r.set(SkFloatToScalar(left), SkFloatToScalar(top),
          SkFloatToScalar(right), SkFloatToScalar(bottom));
    *isNotEmpty = mNativeCanvas->clipRect(r);
    return NOERROR;
}

ECode Canvas::ClipRectEx6(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    SkRect  r;
    r.set(SkIntToScalar(left), SkIntToScalar(top),
          SkIntToScalar(right), SkIntToScalar(bottom));
    *isNotEmpty = mNativeCanvas->clipRect(r);
    return NOERROR;
}

ECode Canvas::ClipPath(
    /* [in] */ IPath* path,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(path != NULL);

    SkPath* nativePath = ((CPath*)path)->Ni();
    *isNotEmpty = NativeClipPath(mNativeCanvas, nativePath, op);
    return NOERROR;
}

ECode Canvas::ClipPathEx(
    /* [in] */ IPath* path,
    /* [out] */ Boolean* isNotEmpty)
{
    return ClipPath(path, RegionOp_INTERSECT, isNotEmpty);
}

ECode Canvas::ClipRegion(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(region != NULL);

    SkRegion* nativeRegion = ((CRegion*)region)->Ni();
    *isNotEmpty = NativeClipRegion(mNativeCanvas, nativeRegion, op);
    return NOERROR;
}

ECode Canvas::ClipRegionEx(
    /* [in] */ IRegion* region,
    /* [out] */ Boolean* isNotEmpty)
{
    return ClipRegion(region, RegionOp_INTERSECT, isNotEmpty);
}

ECode Canvas::GetDrawFilter(
    /* [out] */ IDrawFilter** filter)
{
    assert(filter != NULL);

    *filter = mDrawFilter;
    if (*filter != NULL) (*filter)->AddRef();
    return NOERROR;
}

ECode Canvas::SetDrawFilter(
    /* [in] */ IDrawFilter* filter)
{
    SkDrawFilter* nativeFilter = 0;
    if (filter != NULL) {
        nativeFilter = ((DrawFilter*)filter->Probe(EIID_DrawFilter))->Ni();
    }
    mDrawFilter = filter;
    NativeSetDrawFilter(mNativeCanvas, nativeFilter);

    return NOERROR;
}

ECode Canvas::QuickRejectWithRectF(
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    assert(isNotIntersect != NULL);

    *isNotIntersect = NativeQuickReject(mNativeCanvas, rect, type);
    return NOERROR;
}

ECode Canvas::QuickRejectWithPath(
    /* [in] */ IPath* path,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    assert(isNotIntersect != NULL);
    assert(path != NULL);

    SkPath* nativePath = ((CPath*)path)->Ni();
    *isNotIntersect = NativeQuickReject(mNativeCanvas, nativePath, type);
    return NOERROR;
}

ECode Canvas::QuickReject(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    assert(isNotIntersect != NULL);

    *isNotIntersect = NativeQuickReject(mNativeCanvas, left, top, right, bottom, type);
    return NOERROR;
}

ECode Canvas::GetClipBounds(
    /* [in] */ IRect* bounds,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    *isNotEmpty = NativeGetClipBounds(mNativeCanvas, bounds);
    return NOERROR;
}

ECode Canvas::GetClipBoundsEx(
    /* [out] */ IRect** bounds)
{
    assert(bounds != NULL);

    FAIL_RETURN(CRect::New(bounds));
    Boolean isNotEmpty;
    return GetClipBounds(*bounds, &isNotEmpty);
}

ECode Canvas::DrawRGB(
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    NativeDrawRGB(mNativeCanvas, r, g, b);
    return NOERROR;
}

ECode Canvas::DrawARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    NativeDrawARGB(mNativeCanvas, a, r, g, b);
    return NOERROR;
}

ECode Canvas::DrawColor(
    /* [in] */ Int32 color)
{
    NativeDrawColor(mNativeCanvas, color);
    return NOERROR;
}

ECode Canvas::DrawColorEx(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    NativeDrawColor(mNativeCanvas, color, (SkPorterDuff::Mode)mode);
    return NOERROR;
}

ECode Canvas::DrawPaint(
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    NativeDrawPaint(mNativeCanvas, ((Paint*)paint->Probe(EIID_Paint))->mNativePaint);
    return NOERROR;
}

ECode Canvas::DrawPoints(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    return DoPoints(mNativeCanvas, pts, offset, count, paint,
            SkCanvas::kPoints_PointMode);
}

ECode Canvas::DrawPointsEx(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ IPaint* paint)
{
    return DrawPoints(pts, 0, pts.GetLength(), paint);
}

ECode Canvas::DrawPoint(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    const SkPaint& nativePaint = *((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    mNativeCanvas->drawPoint(SkFloatToScalar(x), SkFloatToScalar(y), nativePaint);
    return NOERROR;
}

ECode Canvas::DrawLine(
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ IPaint* paint)
{
    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawLine(mNativeCanvas, startX, startY, stopX, stopY, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawLines(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    return DoPoints(mNativeCanvas, pts, offset, count, paint,
            SkCanvas::kLines_PointMode);
}

ECode Canvas::DrawLinesEx(
    /* [in] */ const ArrayOf<Float> & pts,
    /* [in] */ IPaint* paint)
{
    return DrawLines(pts, 0, pts.GetLength(), paint);
}

ECode Canvas::DrawRect(
    /* [in] */ IRectF* rect,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawRect(mNativeCanvas, rect, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawRectEx(
    /* [in] */ IRect* r,
    /* [in] */ IPaint* paint)
{
    CRect* r_ = (CRect*)r;
    return DrawRectEx2(r_->mLeft, r_->mTop, r_->mRight, r_->mBottom, paint);
}

ECode Canvas::DrawRectEx2(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawRect(mNativeCanvas, left, top, right, bottom, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawOval(
    /* [in] */ IRectF* oval,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (oval == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawOval(mNativeCanvas, oval, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawCircle(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawCircle(mNativeCanvas, cx, cy, radius, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawArc(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (oval == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawArc(mNativeCanvas, oval, startAngle, sweepAngle,
            useCenter, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (rect == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawRoundRect(mNativeCanvas, rect, rx, ry, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawPath(
    /* [in] */ IPath* path,
    /* [in] */ IPaint* paint)
{
    assert(path != NULL);
    assert(paint != NULL);

    SkPath* nativePath = ((CPath*)path)->Ni();
    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawPath(mNativeCanvas, nativePath, nativePaint);
    return NOERROR;
}

ECode Canvas::ThrowIfRecycled(
    /* [in] */ IBitmap* bitmap)
{
    assert(bitmap != NULL);

    Boolean isRecycled;
    bitmap->IsRecycled(&isRecycled);
    if (isRecycled) {
//        throw new RuntimeException(
//                    "Canvas: trying to use a recycled bitmap " + bitmap);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    FAIL_RETURN(ThrowIfRecycled(bitmap));
    SkBitmap* nativeBitmap = ((CBitmap*)bitmap)->Ni();
    SkPaint* nativePaint = NULL;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmap(mNativeCanvas, nativeBitmap, left, top,
            nativePaint, mDensity, mScreenDensity,
            ((CBitmap*)bitmap)->mDensity);
    return NOERROR;
}

ECode Canvas::DrawBitmapEx(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    if (dst == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    FAIL_RETURN(ThrowIfRecycled(bitmap));
    SkBitmap* nativeBitmap = ((CBitmap*)bitmap)->Ni();
    SkPaint* nativePaint = NULL;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmap(mNativeCanvas, nativeBitmap, src, dst,
            nativePaint, mScreenDensity, ((CBitmap*)bitmap)->mDensity);
    return NOERROR;
}

ECode Canvas::DrawBitmapEx2(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dst,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    if (dst == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    FAIL_RETURN(ThrowIfRecycled(bitmap));
    SkBitmap* nativeBitmap = ((CBitmap*)bitmap)->Ni();
    SkPaint* nativePaint = NULL;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmap(mNativeCanvas, nativeBitmap, src, dst,
            nativePaint, mScreenDensity, ((CBitmap*)bitmap)->mDensity);
    return NOERROR;
}

ECode Canvas::DrawBitmapEx3(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ IPaint* paint)
{
    // check for valid input
    if (width < 0) {
//        throw new IllegalArgumentException("width must be >= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (height < 0) {
//        throw new IllegalArgumentException("height must be >= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (Math::Abs(stride) < width) {
//        throw new IllegalArgumentException("abs(stride) must be >= width");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 lastScanline = offset + (height - 1) * stride;
    Int32 length = colors.GetLength();
    if (offset < 0 || (offset + width > length) || lastScanline < 0
            || (lastScanline + width > length)) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // quick escape if there's nothing to draw
    if (width == 0 || height == 0) {
        return NOERROR;
    }
    SkPaint* nativePaint = NULL;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    // punch down to native for the actual draw
    NativeDrawBitmap(mNativeCanvas, colors, offset, stride, x, y, width,
            height, hasAlpha, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawBitmapEx4(
    /* [in] */ const ArrayOf<Int32> & colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ IPaint* paint)
{
    // call through to the common float version
    return DrawBitmapEx3(colors, offset, stride, (Float)x, (Float)y, width, height,
            hasAlpha, paint);
}

ECode Canvas::DrawBitmapEx5(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IMatrix* matrix,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);
    assert(matrix != NULL);

    SkBitmap* nativeBitmap = ((CBitmap*)bitmap)->Ni();
    SkMatrix* nativeMatrix = ((CMatrix*)matrix)->Ni();
    SkPaint* nativePaint = NULL;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmapMatrix(mNativeCanvas, nativeBitmap, nativeMatrix, nativePaint);
    return NOERROR;
}

ECode Canvas::CheckRange(
    /* [in] */ Int32 length,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    if ((offset | count) < 0 || offset + count > length) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

ECode Canvas::DrawBitmapMesh(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Int32 meshWidth,
    /* [in] */ Int32 meshHeight,
    /* [in] */ const ArrayOf<Float>& verts,
    /* [in] */ Int32 vertOffset,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorOffset,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    if ((meshWidth | meshHeight | vertOffset | colorOffset) < 0) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (meshWidth == 0 || meshHeight == 0) {
        return NOERROR;
    }
    Int32 count = (meshWidth + 1) * (meshHeight + 1);
    // we mul by 2 since we need two floats per vertex
    FAIL_RETURN(CheckRange(verts.GetLength(), vertOffset, count * 2));
    if (colors != NULL) {
        // no mul by 2, since we need only 1 color per vertex
        FAIL_RETURN(CheckRange(colors->GetLength(), colorOffset, count));
    }
    SkBitmap* nativeBitmap = ((CBitmap*)bitmap)->Ni();
    SkPaint* nativePaint = NULL;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmapMesh(mNativeCanvas, nativeBitmap, meshWidth, meshHeight,
            verts, vertOffset, colors, colorOffset, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawVertices(
    /* [in] */ CanvasVertexMode mode,
    /* [in] */ Int32 vertexCount,
    /* [in] */ const ArrayOf<Float>& verts,
    /* [in] */ Int32 vertOffset,
    /* [in] */ ArrayOf<Float>* texs,
    /* [in] */ Int32 texOffset,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorOffset,
    /* [in] */ ArrayOf<Int16>* indices,
    /* [in] */ Int32 indexOffset,
    /* [in] */ Int32 indexCount,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    FAIL_RETURN(CheckRange(verts.GetLength(), vertOffset, vertexCount));
    if (texs != NULL) {
        FAIL_RETURN(CheckRange(texs->GetLength(), texOffset, vertexCount));
    }
    if (colors != NULL) {
        FAIL_RETURN(CheckRange(colors->GetLength(), colorOffset, vertexCount));
    }
    if (indices != NULL) {
        FAIL_RETURN(CheckRange(indices->GetLength(), indexOffset, indexCount));
    }
    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawVertices(mNativeCanvas, (SkCanvas::VertexMode)mode,
            vertexCount, verts, vertOffset, texs, texOffset, colors,
            colorOffset, indices, indexOffset, indexCount, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawTextInBuffer(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    Int32 charCount = String((const char*)text.GetPayload()).GetCharCount();
    if ((index | count | (index + count) |
        (charCount - index - count)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawText(mNativeCanvas, text, index, count, x, y, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawTextInString(
    /* [in] */ const String& text,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (text.IsNullOrEmpty()) {
        return NOERROR;
    }

    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    mNativeCanvas->drawText((const char*)text, text.GetLength(), SkFloatToScalar(x),
            SkFloatToScalar(y), *nativePaint);
    return NOERROR;
}

ECode Canvas::DrawTextInStringEx(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawText(mNativeCanvas, text, start, end, x, y, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawTextInCharSequence(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
//    if (text instanceof String || text instanceof SpannedString ||
//        text instanceof SpannableString) {
//        native_drawText(mNativeCanvas, text.toString(), start, end, x, y,
//                        paint.mNativePaint);
//    }
//    else if (text instanceof GraphicsOperations) {
//        ((GraphicsOperations) text).drawText(this, start, end, x, y,
//                                                 paint);
//    }
//    else {
//        char[] buf = TemporaryBuffer.obtain(end - start);
//        TextUtils.getChars(text, start, end, buf, 0);
//        drawText(buf, 0, end - start, x, y, paint);
//        TemporaryBuffer.recycle(buf);
//    }
    //TODO:
    String str;
    text->ToString(&str);

    return DrawTextInStringEx(str, start, end, x, y, paint);
}

ECode Canvas::DrawPosTextInBuffer(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    Int32 charCount = String((const char*)text.GetPayload()).GetCharCount();
    //TODO: count * 2 > pos.GetLength()?
    if (index < 0 || index + count > charCount
            || count * 2 > pos.GetLength()) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawPosText(mNativeCanvas, text, index, count, pos, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawPosTextInString(
    /* [in] */ const String& text,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    //TODO: text.GetLength() > pos.GetLength()?
    if (text.GetLength() > (UInt32)pos.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawPosText(mNativeCanvas, text, pos, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawTextOnPathInBuffer(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    assert(path != NULL);
    assert(paint != NULL);

    //TODO:
    Int32 charCount = String((const char*)text.GetPayload()).GetCharCount();
    if (index < 0 || index + count > charCount) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    SkPath* nativePath = ((CPath*)path)->Ni();
    SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawTextOnPath(mNativeCanvas, text, index, count,
            nativePath, hOffset, vOffset, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawTextOnPathInString(
    /* [in] */ const String& text,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    assert(path != NULL);
    assert(paint != NULL);

    if (text.GetLength() > 0) {
        SkPath* nativePath = ((CPath*)path)->Ni();
        SkPaint* nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
        NativeDrawTextOnPath(mNativeCanvas, text, nativePath,
                hOffset, vOffset, nativePaint);
    }
    return NOERROR;
}

ECode Canvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode Canvas::DrawPictureEx(
    /* [in] */ IPicture* picture,
    /* [in] */ IRectF* dst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode Canvas::DrawPictureEx2(
    /* [in] */ IPicture* picture,
    /* [in] */ IRect* dst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

void Canvas::FreeCaches()
{
    // these are called in no particular order
    SkGLCanvas::DeleteAllTextures();
    SkImageRef_GlobalPool::SetRAMUsed(0);
    SkGraphics::SetFontCacheUsed(0);
}

ECode Canvas::Init()
{
    mNativeCanvas = InitRaster(NULL);
    return mNativeCanvas ? NOERROR : E_OUT_OF_MEMORY;
}

ECode Canvas::Init(
    /* [in] */ IBitmap* bitmap)
{
    Boolean isMutable = FALSE;
    bitmap->IsMutable(&isMutable);
    if (!isMutable) {
        Logger::W("Canvas", "Immutable bitmap passed to Canvas constructor");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    FAIL_RETURN(ThrowIfRecycled(bitmap));
    mNativeCanvas = InitRaster(((CBitmap*)bitmap)->Ni());
    if (!mNativeCanvas) {
        return E_OUT_OF_MEMORY;
    }
    mBitmap = (CBitmap*)bitmap;
    mDensity = ((CBitmap*)bitmap)->mDensity;
    return NOERROR;
}

ECode Canvas::Init(
    /* [in] */ SkCanvas* nativeCanvas)
{
    if (nativeCanvas == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mNativeCanvas = nativeCanvas;
    mDensity = CBitmap::GetDefaultDensity();
    return NOERROR;
}

ECode Canvas::Init(
    /* [in] */ IGL* gl)
{
    mNativeCanvas = InitGL();
    if (!mNativeCanvas) {
        return E_OUT_OF_MEMORY;
    }
    mGL = gl;
    mDensity = CBitmap::GetDefaultDensity();
    return NOERROR;
}

SkCanvas* Canvas::InitRaster(
    /* [in] */ SkBitmap* bitmap)
{
    return bitmap ? new SkCanvas(*bitmap) : new SkCanvas;
}

SkCanvas* Canvas::InitGL()
{
    return new SkCanvas;
}

void Canvas::NativeSetBitmap(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap)
{
    canvas->setBitmapDevice(*bitmap);
}

void Canvas::NativeSetViewport(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    canvas->setViewport(w, h);
}

Int32 Canvas::NativeSaveLayer(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRectF* bounds,
    /* [in] */ SkPaint* paint,
    /* [in] */ Int32 flags)
{
    SkRect* bounds_ = NULL;
    SkRect storage;
    if (bounds != NULL) {
        storage.set(SkFloatToScalar(((CRectF*)bounds)->mLeft),
                    SkFloatToScalar(((CRectF*)bounds)->mTop),
                    SkFloatToScalar(((CRectF*)bounds)->mRight),
                    SkFloatToScalar(((CRectF*)bounds)->mBottom));
        bounds_ = &storage;
    }
    return canvas->saveLayer(bounds_, paint, (SkCanvas::SaveFlags)flags);
}

Int32 Canvas::NativeSaveLayer(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Float l,
    /* [in] */ Float t,
    /* [in] */ Float r,
    /* [in] */ Float b,
    /* [in] */ SkPaint* paint,
    /* [in] */ Int32 flags)
{
    SkRect bounds;
    bounds.set(SkFloatToScalar(l), SkFloatToScalar(t), SkFloatToScalar(r),
               SkFloatToScalar(b));
    return canvas->saveLayer(&bounds, paint, (SkCanvas::SaveFlags)flags);
}

Int32 Canvas::NativeSaveLayerAlpha(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 flags)
{
    SkRect* bounds_ = NULL;
    SkRect storage;
    if (bounds != NULL) {
        storage.set(SkFloatToScalar(((CRectF*)bounds)->mLeft),
                    SkFloatToScalar(((CRectF*)bounds)->mTop),
                    SkFloatToScalar(((CRectF*)bounds)->mRight),
                    SkFloatToScalar(((CRectF*)bounds)->mBottom));
        bounds_ = &storage;
    }
    return canvas->saveLayerAlpha(bounds_, alpha, (SkCanvas::SaveFlags)flags);
}

Int32 Canvas::NativeSaveLayerAlpha(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Float l,
    /* [in] */ Float t,
    /* [in] */ Float r,
    /* [in] */ Float b,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 flags)
{
    SkRect bounds;
    bounds.set(SkFloatToScalar(l), SkFloatToScalar(t),
               SkFloatToScalar(r), SkFloatToScalar(b));
    return canvas->saveLayerAlpha(&bounds, alpha, (SkCanvas::SaveFlags)flags);
}

void Canvas::NativeConcat(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkMatrix* matrix)
{
    canvas->concat(*matrix);
}

void Canvas::NativeSetMatrix(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkMatrix* matrix)
{
    if (NULL == matrix) {
        canvas->resetMatrix();
    } else {
        canvas->setMatrix(*matrix);
    }
}

Boolean Canvas::NativeClipRect(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ RegionOp op)
{
    SkRect rect;
    rect.set(SkFloatToScalar(left), SkFloatToScalar(top),
             SkFloatToScalar(right), SkFloatToScalar(bottom));
    return canvas->clipRect(rect, (SkRegion::Op)op);
}

Boolean Canvas::NativeClipPath(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkPath* path,
    /* [in] */ RegionOp op)
{
    return canvas->clipPath(*path, (SkRegion::Op)op);
}

Boolean Canvas::NativeClipRegion(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkRegion* region,
    /* [in] */ RegionOp op)
{
    return canvas->clipRegion(*region, (SkRegion::Op)op);
}

void Canvas::NativeSetDrawFilter(
    /* [in] */ SkCanvas* nativeCanvas,
    /* [in] */ SkDrawFilter* nativeFilter)
{
    nativeCanvas->setDrawFilter(nativeFilter);
}

Boolean Canvas::NativeGetClipBounds(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRect* bounds)
{
    SkRect r;
    SkIRect ir;
    bool result = canvas->getClipBounds(&r, SkCanvas::kBW_EdgeType);

    r.round(&ir);
    CRect* bounds_ = (CRect*)bounds;
    bounds_->mLeft = ir.fLeft;
    bounds_->mTop = ir.fTop;
    bounds_->mRight = ir.fRight;
    bounds_->mBottom = ir.fBottom;
    return (Boolean)result;
}

void Canvas::NativeGetCTM(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkMatrix* matrix)
{
    *matrix = canvas->getTotalMatrix();
}

Boolean Canvas::NativeQuickReject(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType edgeType)
{
    SkRect rect_;
    rect_.set(SkFloatToScalar(((CRectF*)rect)->mLeft),
              SkFloatToScalar(((CRectF*)rect)->mTop),
              SkFloatToScalar(((CRectF*)rect)->mRight),
              SkFloatToScalar(((CRectF*)rect)->mBottom));
    return canvas->quickReject(rect_, (SkCanvas::EdgeType)edgeType);
}

Boolean Canvas::NativeQuickReject(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkPath* path,
    /* [in] */ CanvasEdgeType edgeType)
{
    return canvas->quickReject(*path, (SkCanvas::EdgeType)edgeType);
}

Boolean Canvas::NativeQuickReject(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ CanvasEdgeType edgeType)
{
    SkRect r;
    r.set(SkFloatToScalar(left), SkFloatToScalar(top),
          SkFloatToScalar(right), SkFloatToScalar(bottom));
    return canvas->quickReject(r, (SkCanvas::EdgeType)edgeType);
}

void Canvas::NativeDrawRGB(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    canvas->drawARGB(0xFF, r, g, b);
}

void Canvas::NativeDrawARGB(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    canvas->drawARGB(a, r, g, b);
}

void Canvas::NativeDrawColor(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Int32 color)
{
    canvas->drawColor(color);
}

void Canvas::NativeDrawColor(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Int32 color,
    /* [in] */ SkPorterDuff::Mode mode)
{
    canvas->drawColor(color, SkPorterDuff::ToXfermodeMode(mode));
}

void Canvas::NativeDrawPaint(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkPaint* paint)
{
    canvas->drawPaint(*paint);
}

void Canvas::NativeDrawLine(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ SkPaint* paint)
{
    canvas->drawLine(SkFloatToScalar(startX), SkFloatToScalar(startY),
                     SkFloatToScalar(stopX), SkFloatToScalar(stopY),
                     *paint);
}

void Canvas::NativeDrawRect(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRectF* rect,
    /* [in] */ SkPaint* paint)
{
    SkRect rect_;
    rect_.set(SkFloatToScalar(((CRectF*)rect)->mLeft),
              SkFloatToScalar(((CRectF*)rect)->mTop),
              SkFloatToScalar(((CRectF*)rect)->mRight),
              SkFloatToScalar(((CRectF*)rect)->mBottom));
    canvas->drawRect(rect_, *paint);
}

void Canvas::NativeDrawRect(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ SkPaint* paint)
{
    SkScalar left_ = SkFloatToScalar(left);
    SkScalar top_ = SkFloatToScalar(top);
    SkScalar right_ = SkFloatToScalar(right);
    SkScalar bottom_ = SkFloatToScalar(bottom);
    canvas->drawRectCoords(left_, top_, right_, bottom_, *paint);
}

void Canvas::NativeDrawOval(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRectF* oval,
    /* [in] */ SkPaint* paint)
{
    SkRect oval_;
    oval_.set(SkFloatToScalar(((CRectF*)oval)->mLeft),
              SkFloatToScalar(((CRectF*)oval)->mTop),
              SkFloatToScalar(((CRectF*)oval)->mRight),
              SkFloatToScalar(((CRectF*)oval)->mBottom));
    canvas->drawOval(oval_, *paint);
}

void Canvas::NativeDrawCircle(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ SkPaint* paint)
{
    canvas->drawCircle(SkFloatToScalar(cx), SkFloatToScalar(cy),
                       SkFloatToScalar(radius), *paint);
}

void Canvas::NativeDrawArc(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ SkPaint* paint)
{
    SkRect oval_;
    oval_.set(SkFloatToScalar(((CRectF*)oval)->mLeft),
              SkFloatToScalar(((CRectF*)oval)->mTop),
              SkFloatToScalar(((CRectF*)oval)->mRight),
              SkFloatToScalar(((CRectF*)oval)->mBottom));
    canvas->drawArc(oval_, SkFloatToScalar(startAngle),
                    SkFloatToScalar(sweepAngle), useCenter, *paint);
}

void Canvas::NativeDrawRoundRect(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ SkPaint* paint)
{
    SkRect rect_;
    rect_.set(SkFloatToScalar(((CRectF*)rect)->mLeft),
              SkFloatToScalar(((CRectF*)rect)->mTop),
              SkFloatToScalar(((CRectF*)rect)->mRight),
              SkFloatToScalar(((CRectF*)rect)->mBottom));
    canvas->drawRoundRect(rect_, SkFloatToScalar(rx), SkFloatToScalar(ry),
                          *paint);
}

void Canvas::NativeDrawPath(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkPath* path,
    /* [in] */ SkPaint* paint)
{
    canvas->drawPath(*path, *paint);
}

void Canvas::NativeDrawBitmap(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ SkPaint* paint,
    /* [in] */ Int32 canvasDensity,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    SkScalar left_ = SkFloatToScalar(left);
    SkScalar top_ = SkFloatToScalar(top);

    if (canvasDensity == bitmapDensity || canvasDensity == 0
            || bitmapDensity == 0) {
        if (screenDensity != 0 && screenDensity != bitmapDensity) {
            SkPaint filteredPaint;
            if (paint) {
                filteredPaint = *paint;
            }
            filteredPaint.setFilterBitmap(true);
            canvas->drawBitmap(*bitmap, left_, top_, &filteredPaint);
        }
        else {
            canvas->drawBitmap(*bitmap, left_, top_, paint);
        }
    } else {
        canvas->save();
        SkScalar scale = SkFloatToScalar(canvasDensity / (float)bitmapDensity);
        canvas->translate(left_, top_);
        canvas->scale(scale, scale);

        SkPaint filteredPaint;
        if (paint) {
            filteredPaint = *paint;
        }
        filteredPaint.setFilterBitmap(true);

        canvas->drawBitmap(*bitmap, 0, 0, &filteredPaint);

        canvas->restore();
    }
}

void Canvas::NativeDrawBitmap(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRectF* dst,
    /* [in] */ SkPaint* paint,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    SkRect dst_;
    dst_.set(SkFloatToScalar(((CRectF*)dst)->mLeft),
              SkFloatToScalar(((CRectF*)dst)->mTop),
              SkFloatToScalar(((CRectF*)dst)->mRight),
              SkFloatToScalar(((CRectF*)dst)->mBottom));
    DoDrawBitmap(canvas, bitmap, src, dst_, paint,
                 screenDensity, bitmapDensity);
}

void Canvas::NativeDrawBitmap(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dst,
    /* [in] */ SkPaint* paint,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    SkRect dst_;
    dst_.set(SkIntToScalar(((CRect*)dst)->mLeft),
             SkIntToScalar(((CRect*)dst)->mTop),
             SkIntToScalar(((CRect*)dst)->mRight),
             SkIntToScalar(((CRect*)dst)->mBottom));
    DoDrawBitmap(canvas, bitmap, src, dst_, paint,
                 screenDensity, bitmapDensity);
}

void Canvas::NativeDrawBitmap(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ SkPaint* paint)
{
    SkBitmap bitmap;

    bitmap.setConfig(hasAlpha? SkBitmap::kARGB_8888_Config :
                     SkBitmap::kRGB_565_Config, width, height);
    if (!bitmap.allocPixels()) {
        return;
    }

    if (!Graphics::SetPixels(colors, offset, stride,
                             0, 0, width, height, bitmap)) {
        return;
    }

    canvas->drawBitmap(bitmap, SkFloatToScalar(x), SkFloatToScalar(y), paint);
}

void Canvas::NativeDrawBitmapMatrix(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ SkMatrix* matrix,
    /* [in] */ SkPaint* paint)
{
    canvas->drawBitmapMatrix(*bitmap, *matrix, paint);
}

void Canvas::NativeDrawBitmapMesh(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ Int32 meshWidth,
    /* [in] */ Int32 meshHeight,
    /* [in] */ const ArrayOf<Float>& _verts,
    /* [in] */ Int32 vertIndex,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorIndex,
    /* [in] */ SkPaint* paint)
{
    const int ptCount = (meshWidth + 1) * (meshHeight + 1);
    const int indexCount = meshWidth * meshHeight * 6;

    if ((_verts.GetLength() < vertIndex + (ptCount << 1)) ||
        (colors != NULL && colors->GetLength() < colorIndex + ptCount)) {
        sk_throw();
    }

    /*  Our temp storage holds 2 or 3 arrays.
        texture points [ptCount * sizeof(SkPoint)]
        optionally vertex points [ptCount * sizeof(SkPoint)] if we need a
            copy to convert from float to fixed
        indices [ptCount * sizeof(uint16_t)]
    */
    ssize_t storageSize = ptCount * sizeof(SkPoint); // texs[]
#ifdef SK_SCALAR_IS_FIXED
    storageSize += ptCount * sizeof(SkPoint);  // storage for verts
#endif
    storageSize += indexCount * sizeof(uint16_t);  // indices[]

    SkAutoMalloc storage(storageSize);
    SkPoint* texs = (SkPoint*)storage.get();
    SkPoint* verts;
    uint16_t* indices;
#ifdef SK_SCALAR_IS_FLOAT
    verts = (SkPoint*)(_verts.GetPayload() + vertIndex);
    indices = (uint16_t*)(texs + ptCount);
#else
    verts = texs + ptCount;
    indices = (uint16_t*)(verts + ptCount);
    // convert floats to fixed
    {
        const float* src = _verts.GetPayload() + vertIndex;
        for (int i = 0; i < ptCount; i++) {
            verts[i].set(SkFloatToFixed(src[0]), SkFloatToFixed(src[1]));
            src += 2;
        }
    }
#endif

    // cons up texture coordinates and indices
    {
        const SkScalar w = SkIntToScalar(bitmap->width());
        const SkScalar h = SkIntToScalar(bitmap->height());
        const SkScalar dx = w / meshWidth;
        const SkScalar dy = h / meshHeight;

        SkPoint* texsPtr = texs;
        SkScalar y = 0;
        for (int i = 0; i <= meshHeight; i++) {
            if (i == meshHeight) {
                y = h;  // to ensure numerically we hit h exactly
            }
            SkScalar x = 0;
            for (int j = 0; j < meshWidth; j++) {
                texsPtr->set(x, y);
                texsPtr += 1;
                x += dx;
            }
            texsPtr->set(w, y);
            texsPtr += 1;
            y += dy;
        }
        SkASSERT(texsPtr - texs == ptCount);
    }

    // cons up indices
    {
        uint16_t* indexPtr = indices;
        int index = 0;
        for (int i = 0; i < meshHeight; i++) {
            for (int j = 0; j < meshWidth; j++) {
                // lower-left triangle
                *indexPtr++ = index;
                *indexPtr++ = index + meshWidth + 1;
                *indexPtr++ = index + meshWidth + 2;
                // upper-right triangle
                *indexPtr++ = index;
                *indexPtr++ = index + meshWidth + 2;
                *indexPtr++ = index + 1;
                // bump to the next cell
                index += 1;
            }
            // bump to the next row
            index += 1;
        }
        SkASSERT(indexPtr - indices == indexCount);
        SkASSERT((char*)indexPtr - (char*)storage.get() == storageSize);
    }

    // double-check that we have legal indices
#ifdef SK_DEBUG
    {
        for (int i = 0; i < indexCount; i++) {
            SkASSERT((unsigned)indices[i] < (unsigned)ptCount);
        }
    }
#endif

    // cons-up a shader for the bitmap
    SkPaint tmpPaint;
    if (paint) {
        tmpPaint = *paint;
    }
    SkShader* shader = SkShader::CreateBitmapShader(*bitmap,
            SkShader::kClamp_TileMode, SkShader::kClamp_TileMode);
    tmpPaint.setShader(shader)->safeUnref();

    canvas->drawVertices(SkCanvas::kTriangles_VertexMode, ptCount, verts, texs,
                         (const SkColor*)(colors != NULL? colors->GetPayload() : NULL),
                         NULL, indices, indexCount, tmpPaint);
}

void Canvas::NativeDrawVertices(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkCanvas::VertexMode mode,
    /* [in] */ Int32 vertexCount,
    /* [in] */ const ArrayOf<Float>& _verts,
    /* [in] */ Int32 vertIndex,
    /* [in] */ ArrayOf<Float>* _texs,
    /* [in] */ Int32 texIndex,
    /* [in] */ ArrayOf<Int32>* _colors,
    /* [in] */ Int32 colorIndex,
    /* [in] */ ArrayOf<Int16>* _indices,
    /* [in] */ Int32 indexIndex,
    /* [in] */ Int32 indexCount,
    /* [in] */ SkPaint* paint)
{
    if (_verts.GetLength() < vertIndex + vertexCount){
        sk_throw();
    }
    if (_texs != NULL && _texs->GetLength() < texIndex + vertexCount) {
        sk_throw();
    }
    if (_colors != NULL && _colors->GetLength() < colorIndex + vertexCount) {
        sk_throw();
    }
    if (_indices != NULL && _indices->GetLength() < indexIndex + indexCount) {
        sk_throw();
    }

    const int ptCount = vertexCount >> 1;

    SkPoint* verts;
    SkPoint* texs = NULL;
#ifdef SK_SCALAR_IS_FLOAT
    verts = (SkPoint*)(_verts.GetPayload() + vertIndex);
    if (_texs != NULL) {
        texs = (SkPoint*)(_texs->GetPayload() + texIndex);
    }
#else
    int count = ptCount;    // for verts
    if (_texs != NULL) {
        count += ptCount;   // += for texs
    }
    SkAutoMalloc storage(count * sizeof(SkPoint));
    verts = (SkPoint*)storage.get();
    const float* src = vertA.GetPayload() + vertIndex;
    for (int i = 0; i < ptCount; i++) {
        verts[i].set(SkFloatToFixed(src[0]), SkFloatToFixed(src[1]));
        src += 2;
    }
    if (_texs != NULL) {
        texs = verts + ptCount;
        src = _texs->GetPayload() + texIndex;
        for (int i = 0; i < ptCount; i++) {
            texs[i].set(SkFloatToFixed(src[0]), SkFloatToFixed(src[1]));
            src += 2;
        }
    }
#endif

    const SkColor* colors = NULL;
    const uint16_t* indices = NULL;
    if (_colors != NULL) {
        colors = (const SkColor*)(_colors->GetPayload() + colorIndex);
    }
    if (_indices != NULL) {
        indices = (const uint16_t*)(_indices->GetPayload() + indexIndex);
    }

    canvas->drawVertices(mode, ptCount, verts, texs, colors, NULL,
                         indices, indexCount, *paint);
}

void Canvas::NativeDrawText(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ SkPaint* paint)
{
    Int32 startOff, endOff;
    Character::GetOffsetByChars(text, 0, text.GetLength(), 0, index, &startOff);
    Character::GetOffsetByChars(text, 0, text.GetLength(), 0, index + count, &endOff);
    SkScalar x_ = SkFloatToScalar(x);
    SkScalar y_ = SkFloatToScalar(y);
    canvas->drawText(text.GetPayload() + startOff, endOff - startOff, x_, y_, *paint);
}

void Canvas::NativeDrawText(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ SkPaint* paint)
{
    Int32 startOff, endOff;
    Character::GetOffsetByChars(text, 0, start, &startOff);
    Character::GetOffsetByChars(text, 0, end, &endOff);
    SkScalar x_ = SkFloatToScalar(x);
    SkScalar y_ = SkFloatToScalar(y);
    canvas->drawText(((const char*)text + startOff), endOff - startOff, x_, y_, *paint);
}

void Canvas::NativeDrawPosText(
    /* [in] */ SkCanvas* nativeCanvas,
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ SkPaint* nativePaint)
{
    char* textArray = text.GetPayload();
    Float* posArray = pos.GetPayload();
    Int32 posCount = pos.GetLength() >> 1;
    SkPoint* posPtr = posCount > 0 ? new SkPoint[posCount] : NULL;

    for (Int32 i = 0; i < posCount; i++) {
        posPtr[i].fX = SkFloatToScalar(posArray[i << 1]);
        posPtr[i].fY = SkFloatToScalar(posArray[(i << 1) + 1]);
    }

    Int32 startOff, endOff;
    Character::GetOffsetByChars(text, 0, text.GetLength(), 0, index, &startOff);
    Character::GetOffsetByChars(text, 0, text.GetLength(), 0, index + count, &endOff);
    nativeCanvas->drawPosText(textArray + startOff, endOff - startOff , posPtr, *nativePaint);

    delete[] posPtr;
}

void Canvas::NativeDrawPosText(
    /* [in] */ SkCanvas* nativeCanvas,
    /* [in] */ const String& text,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ SkPaint* nativePaint)
{
    const void* text_ = text.string();
    Int32 byteLength = text.GetLength();
    Float* posArray = pos.GetPayload();
    Int32 posCount = pos.GetLength();
    SkPoint* posPtr = posCount > 0 ? new SkPoint[posCount] : NULL;

    for (Int32 i = 0; i < posCount; i++) {
       posPtr[i].fX = SkFloatToScalar(posArray[i << 1]);
       posPtr[i].fY = SkFloatToScalar(posArray[(i << 1) + 1]);
    }

    nativeCanvas->drawPosText(text_, byteLength, posPtr, *nativePaint);
    delete[] posPtr;
}

void Canvas::NativeDrawTextOnPath(
    /* [in] */ SkCanvas* nativeCanvas,
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ SkPath* nativePath,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ SkPaint* nativePaint)
{
    char* textArray = text.GetPayload();
    Int32 startOff, endOff;
    Character::GetOffsetByChars(text, 0, text.GetLength(), 0, index, &startOff);
    Character::GetOffsetByChars(text, 0, text.GetLength(), 0, index + count, &endOff);
    nativeCanvas->drawTextOnPathHV(textArray + startOff, endOff - startOff, *nativePath,
               SkFloatToScalar(hOffset), SkFloatToScalar(vOffset), *nativePaint);
}

void Canvas::NativeDrawTextOnPath(
    /* [in] */ SkCanvas* nativeCanvas,
    /* [in] */ const String& text,
    /* [in] */ SkPath* nativePath,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ SkPaint* nativePaint)
{
    const char* text_ = text.string();
    Int32 byteLength = text.GetLength();
    nativeCanvas->drawTextOnPathHV(text_, byteLength, *nativePath,
               SkFloatToScalar(hOffset), SkFloatToScalar(vOffset), *nativePaint);
}

void Canvas::NativeDrawPicture(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkPicture* picture)
{
    SkASSERT(canvas);
    SkASSERT(picture);

#ifdef TIME_DRAW
    SkMSec now = get_thread_msec(); //SkTime::GetMSecs();
#endif
    canvas->drawPicture(*picture);
#ifdef TIME_DRAW
    LOGD("---- picture playback %d ms\n", get_thread_msec() - now);
#endif
}

void Canvas::Finalizer(
    /* [in] */ SkCanvas* canvas)
{
    canvas->unref();
}

ECode Canvas::DoPoints(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Float>& _pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* _paint,
    /* [in] */ SkCanvas::PointMode mode)
{
    if (canvas == NULL || _paint == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    const SkPaint& paint = *((Paint*)_paint->Probe(EIID_Paint))->mNativePaint;

    float* floats = _pts.GetPayload();
    const int length = _pts.GetLength();

    if ((offset | count) < 0 || offset + count > length) {
//        doThrowAIOOBE(env);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    // now convert the floats into SkPoints
    count >>= 1;    // now it is the number of points
    SkAutoSTMalloc<32, SkPoint> storage(count);
    SkPoint* pts = storage.get();
    const float* src = floats + offset;
    for (int i = 0; i < count; i++) {
        pts[i].set(SkFloatToScalar(src[0]), SkFloatToScalar(src[1]));
        src += 2;
    }
    canvas->drawPoints(mode, count, pts, paint);
    return NOERROR;
}

void Canvas::DoDrawBitmap(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ IRect* srcIRect,
    /* [in] */ const SkRect& dst,
    /* [in] */ SkPaint* paint,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    SkIRect src, *srcPtr = NULL;

    if (NULL != srcIRect) {
        src.set(((CRect*)srcIRect)->mLeft,
                ((CRect*)srcIRect)->mTop,
                ((CRect*)srcIRect)->mRight,
                ((CRect*)srcIRect)->mBottom);
        srcPtr = &src;
    }

    if (screenDensity != 0 && screenDensity != bitmapDensity) {
        SkPaint filteredPaint;
        if (paint) {
            filteredPaint = *paint;
        }
        filteredPaint.setFilterBitmap(true);
        canvas->drawBitmapRect(*bitmap, srcPtr, dst, &filteredPaint);
    } else {
        canvas->drawBitmapRect(*bitmap, srcPtr, dst, paint);
    }
}

