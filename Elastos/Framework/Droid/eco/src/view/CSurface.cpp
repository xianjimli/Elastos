
#include "view/CSurface.h"
#include "view/CSurfaceSession.h"
#include "graphics/CRect.h"
#ifdef _linux
#include <surfaceflinger/SurfaceComposerClient.h>
#include <assert.h>
#include <ui/PixelFormat.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkRegion.h>
#include <utils/String8.h>
#endif
//using namespace Elastos;

#ifdef _linux
static inline SkBitmap::Config ConvertPixelFormat(PixelFormat format)
{
    /* note: if PIXEL_FORMAT_RGBX_8888 means that all alpha bytes are 0xFF, then
        we can map to SkBitmap::kARGB_8888_Config, and optionally call
        bitmap.setIsOpaque(true) on the resulting SkBitmap (as an accelerator)
    */
    switch (format) {
    case android::PIXEL_FORMAT_RGBX_8888: return SkBitmap::kARGB_8888_Config;
    case android::PIXEL_FORMAT_RGBA_8888: return SkBitmap::kARGB_8888_Config;
    case android::PIXEL_FORMAT_RGBA_4444: return SkBitmap::kARGB_4444_Config;
    case android::PIXEL_FORMAT_RGB_565:   return SkBitmap::kRGB_565_Config;
    case android::PIXEL_FORMAT_A_8:       return SkBitmap::kA8_Config;
    default:                              return SkBitmap::kNo_Config;
    }
}
#endif

////////////////////////////////////////////////////////////
// CSurface::CompatibleCanvas
CSurface::CompatibleCanvas::CompatibleCanvas()
{
}

CSurface::CompatibleCanvas::~CompatibleCanvas()
{
}

PInterface CSurface::CompatibleCanvas::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ICanvas*)this;
    }
    else if (riid == EIID_ICanvas) {
        return (ICanvas*)this;
    }
    else if (riid == EIID_Canvas) {
        return reinterpret_cast<PInterface>((Canvas*)this);
    }
    return NULL;
}

UInt32 CSurface::CompatibleCanvas::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSurface::CompatibleCanvas::Release()
{
    return ElRefBase::Release();
}

ECode CSurface::CompatibleCanvas::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ICanvas*)this) {
        *pIID = EIID_ICanvas;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CSurface::CompatibleCanvas::GetGL(
    /* [out] */ IGL** gl)
{
    VALIDATE_NOT_NULL(gl);
    return Canvas::GetGL(gl);
}

ECode CSurface::CompatibleCanvas::FreeGlCaches()
{
    Canvas::FreeGlCaches();
    return NOERROR;
}

ECode CSurface::CompatibleCanvas::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return Canvas::SetBitmap(bitmap);
}

ECode CSurface::CompatibleCanvas::SetViewport(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Canvas::SetViewport(width, height);
}

ECode CSurface::CompatibleCanvas::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);
    return Canvas::IsOpaque(isOpaque);
}

ECode CSurface::CompatibleCanvas::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    //return mCompatibleDisplayMetrics == null ?
    //    super.getWidth() : mCompatibleDisplayMetrics.widthPixels;

    return E_NOT_IMPLEMENTED;
}

ECode CSurface::CompatibleCanvas::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    //return mCompatibleDisplayMetrics == null ?
    //    super.getHeight() : mCompatibleDisplayMetrics.heightPixels;

    return E_NOT_IMPLEMENTED;
}

ECode CSurface::CompatibleCanvas::GetDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);
    return Canvas::GetDensity(density);
}

ECode CSurface::CompatibleCanvas::SetDensity(
    /* [in] */ Int32 density)
{
    return Canvas::SetDensity(density);
}

ECode CSurface::CompatibleCanvas::SetScreenDensity(
    /* [in] */ Int32 density)
{
    return Canvas::SetScreenDensity(density);
}

ECode CSurface::CompatibleCanvas::Save(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::Save(count);
}

ECode CSurface::CompatibleCanvas::SaveEx(
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveEx(saveFlags, count);
}

ECode CSurface::CompatibleCanvas::SaveLayer(
    /* [in] */ IRectF* bounds,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveLayer(bounds, paint, saveFlags, count);
}

ECode CSurface::CompatibleCanvas::SaveLayerEx(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveLayerEx(left, top, right, bottom, paint, saveFlags, count);
}

ECode CSurface::CompatibleCanvas::SaveLayerAlpha(
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveLayerAlpha(bounds, alpha, saveFlags, count);
}

ECode CSurface::CompatibleCanvas::SaveLayerAlphaEx(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveLayerAlphaEx(left, top, right, bottom, alpha, saveFlags, count);
}

ECode CSurface::CompatibleCanvas::Restore()
{
    return Canvas::Restore();
}

ECode CSurface::CompatibleCanvas::GetSaveCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::GetSaveCount(count);
}

ECode CSurface::CompatibleCanvas::RestoreToCount(
    /* [in] */ Int32 saveCount)
{
    return Canvas::RestoreToCount(saveCount);
}

ECode CSurface::CompatibleCanvas::Translate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    return Canvas::Translate(dx, dy);
}

ECode CSurface::CompatibleCanvas::Scale(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    return Canvas::Scale(sx, sy);
}

ECode CSurface::CompatibleCanvas::ScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Canvas::ScaleEx(sx, sy, px, py);
}

ECode CSurface::CompatibleCanvas::Rotate(
    /* [in] */ Float degrees)
{
    return Canvas::Rotate(degrees);
}

ECode CSurface::CompatibleCanvas::RotateEx(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Canvas::RotateEx(degrees, px, py);
}

ECode CSurface::CompatibleCanvas::Skew(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    return Canvas::Skew(sx, sy);
}

ECode CSurface::CompatibleCanvas::Concat(
    /* [in] */ IMatrix* matrix)
{
    VALIDATE_NOT_NULL(matrix);
    return Canvas::Concat(matrix);
}

ECode CSurface::CompatibleCanvas::SetMatrix(
    /* [in] */ IMatrix* matrix)
{
    //if (mCompatibleMatrix == null || mOrigMatrix == null || mOrigMatrix.equals(matrix)) {
    //    // don't scale the matrix if it's not compatibility mode, or
    //    // the matrix was obtained from getMatrix.
    //    super.setMatrix(matrix);
    //} else {
    //    Matrix m = new Matrix(mCompatibleMatrix);
    //    m.preConcat(matrix);
    //    super.setMatrix(m);
    //}

    return E_NOT_IMPLEMENTED;
}

ECode CSurface::CompatibleCanvas::GetMatrix(
    /* [in] */ IMatrix* ctm)
{
    VALIDATE_NOT_NULL(ctm);

    //super.getMatrix(m);
    //if (mOrigMatrix == null) {
    //    mOrigMatrix = new Matrix();
    //}
    //mOrigMatrix.set(m);

    return E_NOT_IMPLEMENTED;
}

ECode CSurface::CompatibleCanvas::GetMatrixEx(
    /* [out] */ IMatrix** metrix)
{
    VALIDATE_NOT_NULL(metrix);
    return Canvas::GetMatrixEx(metrix);
}

ECode CSurface::CompatibleCanvas::ClipRect(
    /* [in] */ IRectF* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRect(rect, op, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipRectEx(
    /* [in] */ IRect* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx(rect, op, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipRectEx2(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx2(rect, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipRectEx3(
    /* [in] */ IRect* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx3(rect, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipRectEx4(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    return Canvas::ClipRectEx4(left, top, right, bottom, op, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipRectEx5(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    return Canvas::ClipRectEx5(left, top, right, bottom, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipRectEx6(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    return Canvas::ClipRectEx6(left, top, right, bottom, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipPath(
    /* [in] */ IPath* path,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(path && isNotEmpty);
    return Canvas::ClipPath(path, op, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipPathEx(
    /* [in] */ IPath* path,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(path && isNotEmpty);
    return Canvas::ClipPathEx(path, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipRegion(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(region && isNotEmpty);
    return Canvas::ClipRegion(region, op, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::ClipRegionEx(
    /* [in] */ IRegion* region,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(region && isNotEmpty);
    return Canvas::ClipRegionEx(region, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::GetDrawFilter(
    /* [out] */ IDrawFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    return Canvas::GetDrawFilter(filter);
}

ECode CSurface::CompatibleCanvas::SetDrawFilter(
    /* [in] */ IDrawFilter* filter)
{
    VALIDATE_NOT_NULL(filter);
    return Canvas::SetDrawFilter(filter);
}

ECode CSurface::CompatibleCanvas::QuickRejectWithRectF(
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(rect && isNotIntersect);
    return Canvas::QuickRejectWithRectF(rect, type, isNotIntersect);
}

ECode CSurface::CompatibleCanvas::QuickRejectWithPath(
    /* [in] */ IPath* path,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(path && isNotIntersect);
    return Canvas::QuickRejectWithPath(path, type, isNotIntersect);
}

ECode CSurface::CompatibleCanvas::QuickReject(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(isNotIntersect);
    return Canvas::QuickReject(left, top, right, bottom, type, isNotIntersect);
}

ECode CSurface::CompatibleCanvas::GetClipBounds(
    /* [in] */ IRect* bounds,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(bounds && isNotEmpty);
    return Canvas::GetClipBounds(bounds, isNotEmpty);
}

ECode CSurface::CompatibleCanvas::GetClipBoundsEx(
    /* [out] */ IRect** bounds)
{
    VALIDATE_NOT_NULL(bounds);
    return Canvas::GetClipBoundsEx(bounds);
}

ECode CSurface::CompatibleCanvas::DrawRGB(
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Canvas::DrawRGB(r, g, b);
}

ECode CSurface::CompatibleCanvas::DrawARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Canvas::DrawARGB(a, r, g, b);
}

ECode CSurface::CompatibleCanvas::DrawColor(
    /* [in] */ Int32 color)
{
    return Canvas::DrawColor(color);
}

ECode CSurface::CompatibleCanvas::DrawColorEx(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    return Canvas::DrawColorEx(color, mode);
}

ECode CSurface::CompatibleCanvas::DrawPaint(
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPaint(paint);
}

ECode CSurface::CompatibleCanvas::DrawPoints(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPoints(pts, offset, count, paint);
}

ECode CSurface::CompatibleCanvas::DrawPointsEx(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPointsEx(pts, paint);
}

ECode CSurface::CompatibleCanvas::DrawPoint(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPoint(x, y, paint);
}

ECode CSurface::CompatibleCanvas::DrawLine(
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLine(startX, startY, stopX, stopY, paint);
}

ECode CSurface::CompatibleCanvas::DrawLines(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLines(pts, offset, count, paint);
}

ECode CSurface::CompatibleCanvas::DrawLinesEx(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLinesEx(pts, paint);
}

ECode CSurface::CompatibleCanvas::DrawRect(
    /* [in] */ IRectF* rect,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(rect && paint);
    return Canvas::DrawRect(rect, paint);
}

ECode CSurface::CompatibleCanvas::DrawRectEx(
    /* [in] */ IRect* r,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(r && paint);
    return Canvas::DrawRectEx(r, paint);
}

ECode CSurface::CompatibleCanvas::DrawRectEx2(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint * paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawRectEx2(left, top, right, bottom, paint);
}

ECode CSurface::CompatibleCanvas::DrawOval(
    /* [in] */ IRectF* oval,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawOval(oval, paint);
}

ECode CSurface::CompatibleCanvas::DrawCircle(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawCircle(cx, cy, radius, paint);
}

ECode CSurface::CompatibleCanvas::DrawArc(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawArc(oval, startAngle, sweepAngle, useCenter, paint);
}

ECode CSurface::CompatibleCanvas::DrawRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawRoundRect(rect, rx, ry, paint);
}

ECode CSurface::CompatibleCanvas::DrawPath(
    /* [in] */ IPath* path,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(path && paint);
    return Canvas::DrawPath(path, paint);
}

ECode CSurface::CompatibleCanvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmap(bitmap, left, top, paint);
}

ECode CSurface::CompatibleCanvas::DrawBitmapEx(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmapEx(bitmap, src, dst, paint);
}

ECode CSurface::CompatibleCanvas::DrawBitmapEx2(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dst,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmapEx2(bitmap, src, dst, paint);
}

ECode CSurface::CompatibleCanvas::DrawBitmapEx3(
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
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawBitmapEx3(
        colors, offset, stride, x, y, width, height, hasAlpha, paint);
}

ECode CSurface::CompatibleCanvas::DrawBitmapEx4(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawBitmapEx4(
        colors, offset, stride, x, y, width, height, hasAlpha, paint);
}

ECode CSurface::CompatibleCanvas::DrawBitmapEx5(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IMatrix* matrix,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && matrix);
    return Canvas::DrawBitmapEx5(bitmap, matrix, paint);
}

ECode CSurface::CompatibleCanvas::DrawBitmapMesh(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Int32 meshWidth,
    /* [in] */ Int32 meshHeight,
    /* [in] */ const ArrayOf<Float>& verts,
    /* [in] */ Int32 vertOffset,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorOffset,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmapMesh(
        bitmap, meshWidth, meshHeight, verts, vertOffset, colors, colorOffset, paint);
}

ECode CSurface::CompatibleCanvas::DrawVertices(
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
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawVertices(
        mode, vertexCount, verts, vertOffset, texs, texOffset, colors, colorOffset,
        indices, indexOffset, indexCount, paint);
}

ECode CSurface::CompatibleCanvas::DrawTextInBuffer(
    /* [in] */ const BufferOf<Byte>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawTextInBuffer(text, index, count, x, y, paint);
}

ECode CSurface::CompatibleCanvas::DrawTextInString(
    /* [in] */ String text,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawTextInString(text, x, y, paint);
}

ECode CSurface::CompatibleCanvas::DrawTextInStringEx(
    /* [in] */ String text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawTextInStringEx(text, start, end, x, y, paint);
}

ECode CSurface::CompatibleCanvas::DrawTextInCharSequence(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(text && paint);
    return Canvas::DrawTextInCharSequence(text, start, end, x, y, paint);
}

ECode CSurface::CompatibleCanvas::DrawPosTextInBuffer(
    /* [in] */ const BufferOf<Byte>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPosTextInBuffer(text, index, count, pos, paint);
}

ECode CSurface::CompatibleCanvas::DrawPosTextInString(
    /* [in] */ String text,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    VALIDATE_STRING_NOT_NULL(text);
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPosTextInString(text, pos, paint);
}

ECode CSurface::CompatibleCanvas::DrawTextOnPathInBuffer(
    /* [in] */ const BufferOf<Byte>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(path && paint);
    return Canvas::DrawTextOnPathInBuffer(text, index, count, path, hOffset, vOffset, paint);
}

ECode CSurface::CompatibleCanvas::DrawTextOnPathInString(
    /* [in] */ String text,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(path && paint);
    return Canvas::DrawTextOnPathInString(text, path, hOffset, vOffset, paint);
}

ECode CSurface::CompatibleCanvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    VALIDATE_NOT_NULL(picture);
    return Canvas::DrawPicture(picture);
}

ECode CSurface::CompatibleCanvas::DrawPictureEx(
    /* [in] */ IPicture* picture,
    /* [in] */ IRectF* dst)
{
    VALIDATE_NOT_NULL(picture && dst);
    return Canvas::DrawPictureEx(picture, dst);
}

ECode CSurface::CompatibleCanvas::DrawPictureEx2(
    /* [in] */ IPicture* picture,
    /* [in] */ IRect* dst)
{
    VALIDATE_NOT_NULL(picture && dst);
    return Canvas::DrawPictureEx2(picture, dst);
}

ECode CSurface::CompatibleCanvas::FreeCaches()
{
    Canvas::FreeCaches();
    return NOERROR;
}
////////////////////////////////////////////////////////////
// CSurface

CSurface::CSurface()
    : mSaveCount(0)
{
}

CSurface::~CSurface()
{
    if (!mName.IsNull()) {
        String::Free(mName);
    }
}

void CSurface::OpenTransaction()
{
#ifdef _linux
    android::SurfaceComposerClient::openGlobalTransaction();
#endif
}

void CSurface::CloseTransaction()
{
#ifdef _linux
    android::SurfaceComposerClient::closeGlobalTransaction();
#endif
}

ECode CSurface::ReadFromParcel(
    /* [in] */ IParcel * pSource)
{
#ifdef _linux
    Handle32 data;
    pSource->GetElementPayload(&data);
    android::Parcel* parcel = (android::Parcel*)data;

    if (parcel == NULL) {
        //doThrow(env, "java/lang/NullPointerException", NULL);
        return E_FAIL;
    }

    mNativeSurface = android::Surface::readFromParcel(*parcel);
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::WriteToParcel(
    /* [in] */ IParcel * pDest)
{
#ifdef _linux
    Handle32 data;
    pDest->GetElementPayload(&data);
    android::Parcel* parcel = (android::Parcel*)data;

    if (parcel == NULL) {
        //doThrow(env, "java/lang/NullPointerException", NULL);
        return E_FAIL;
    }

    android::SurfaceControl::writeSurfaceToParcel(mSurfaceControl, parcel);
//    if (flags & PARCELABLE_WRITE_RETURN_VALUE) {
//        setSurfaceControl(env, clazz, 0);
//    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::SetCompatibleDisplayMetrics(
    /* [in] */ IDisplayMetrics * pMetrics,
    /* [in] */ ITranslator * pTranslator)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::CopyFrom(
    /* [in] */ ISurface * pO)
{
#ifdef _linux
    if (!pO) {
        return E_INVALID_ARGUMENT;
    }

    /*
     * This is used by the WindowManagerService just after constructing
     * a Surface and is necessary for returning the Surface reference to
     * the caller. At this point, we should only have a SurfaceControl.
     */

    if (!android::SurfaceControl::isSameSurface(
            this->mSurfaceControl,
            ((CSurface *)pO)->mSurfaceControl)) {
        // we reassign the surface only if it's a different one
        // otherwise we would loose our client-side state.
        SetSurfaceControl(((CSurface *)pO)->mSurfaceControl);
    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::IsValid(
    /* [out] */ Boolean * pIsValid)
{
#ifdef _linux
    if (!pIsValid) {
        return E_INVALID_ARGUMENT;
    }
    if (mSurfaceControl != NULL) {
        *pIsValid = android::SurfaceControl::isValid(mSurfaceControl) ? TRUE : FALSE;
        return NOERROR;
    }
    GetSurface();
    *pIsValid =  android::Surface::isValid(mNativeSurface) ? TRUE : FALSE;
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::Destroy()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::LockCanvas(
    /* [in] */ IRect * pDirty,
    /* [out] */ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);

    /* the dirty rectangle may be expanded to the surface's size, if
     * for instance it has been resized or if the bits were lost, since
     * the last call.
     */
    GetSurface();
    if (!android::Surface::isValid(mNativeSurface)) {
        return E_INVALID_OPERATION;
    }

    // get dirty region
    android::Region dirtyRegion;
    if (pDirty) {
        android::Rect dirty;
        dirty.left = ((CRect*)pDirty)->mLeft;
        dirty.top = ((CRect*)pDirty)->mTop;
        dirty.right = ((CRect*)pDirty)->mRight;
        dirty.bottom = ((CRect*)pDirty)->mBottom;

        if (!dirty.isEmpty()) {
            dirtyRegion.set(dirty);
        }
    } else {
        dirtyRegion.set(android::Rect(0x3FFF,0x3FFF));
    }

    android::Surface::SurfaceInfo info;
    android::status_t err = mNativeSurface->lock(&info, &dirtyRegion);
    if (err < 0) {
        return E_FAIL;
    }

    // Associate a SkCanvas object to this surface
    mCanvas->mSurfaceFormat = info.format;

    SkCanvas* nativeCanvas = (SkCanvas*)mCanvas->mNativeCanvas;
    SkBitmap bitmap;
    ssize_t bpr = info.s * android::bytesPerPixel(info.format);
    bitmap.setConfig(ConvertPixelFormat(info.format), info.w, info.h, bpr);
    if (info.format == android::PIXEL_FORMAT_RGBX_8888) {
        bitmap.setIsOpaque(true);
    }
    if (info.w > 0 && info.h > 0) {
        bitmap.setPixels(info.bits);
    } else {
        // be safe with an empty bitmap.
        bitmap.setPixels(NULL);
    }
    nativeCanvas->setBitmapDevice(bitmap);

    SkRegion clipReg;
    if (dirtyRegion.isRect()) { // very common case
        const android::Rect b(dirtyRegion.getBounds());
        clipReg.setRect(b.left, b.top, b.right, b.bottom);
    } else {
        size_t count;
        android::Rect const* r = dirtyRegion.getArray(&count);
        while (count) {
            clipReg.op(r->left, r->top, r->right, r->bottom, SkRegion::kUnion_Op);
            r++, count--;
        }
    }

    nativeCanvas->clipRegion(clipReg);
    mSaveCount = nativeCanvas->save();

    if (pDirty) {
        const android::Rect& bounds(dirtyRegion.getBounds());
        pDirty->Set(bounds.left, bounds.top, bounds.right, bounds.bottom);
    }

    *canvas = (ICanvas*)mCanvas.Get();
    (*canvas)->AddRef();
    return NOERROR;
}

ECode CSurface::LockCanvasNative(
    /* [in] */ IRect * pDirty,
    /* [out] */ ICanvas ** ppCanvas)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::UnlockCanvasAndPost(
    /* [in] */ ICanvas * pCanvas)
{
#ifdef _linux
    if (!android::Surface::isValid(mNativeSurface)) {
        return E_FAIL;
    }

    // detach the canvas from the surface
    SkCanvas* nativeCanvas = (SkCanvas*)mCanvas->mNativeCanvas;
    nativeCanvas->restoreToCount(mSaveCount);
    nativeCanvas->setBitmapDevice(SkBitmap());
    mSaveCount = 0;

    // unlock surface
    android::status_t err = mNativeSurface->unlockAndPost();
    if (err < 0) {
        //doThrow(env, "java/lang/IllegalArgumentException", NULL);
        return E_FAIL;
    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::UnlockCanvas(
    /* [in] */ ICanvas * pCanvas)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::FreezeDisplay(
    /* [in] */ Int32 display)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::UnfreezeDisplay(
    /* [in] */ Int32 display)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::SetOrientationEx(
    /* [in] */ Int32 display,
    /* [in] */ Int32 orientation,
    /* [in] */ Int32 flags)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::SetOrientation(
    /* [in] */ Int32 display,
    /* [in] */ Int32 orientation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::SetLayer(
    /* [in] */ Int32 zorder)
{
#ifdef _linux
    android::status_t err = mSurfaceControl->setLayer(zorder);
    if (err < 0 && err != android::NO_INIT) {
        //doThrow(env, "java/lang/IllegalArgumentException", NULL);
        return E_FAIL;
    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::SetPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
#ifdef _linux
    android::status_t err = mSurfaceControl->setPosition(x, y);
    if (err < 0 && err != android::NO_INIT) {
        // doThrow(env, "java/lang/IllegalArgumentException", NULL);
        return E_FAIL;
    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::SetSize(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
#ifdef _linux
    android::status_t err = mSurfaceControl->setSize(w, h);
    if (err < 0 && err != android::NO_INIT) {
        //doThrow(env, "java/lang/IllegalArgumentException", NULL);
        return E_FAIL;
    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::Hide()
{
#ifdef _linux
    android::status_t err = mSurfaceControl->hide();
    if (err < 0 && err != android::NO_INIT) {
        //doThrow(env, "java/lang/IllegalArgumentException", NULL);
        return E_FAIL;
    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::Show()
{
#ifdef _linux
    android::status_t err = mSurfaceControl->show();
    if (err < 0 && err != android::NO_INIT) {
        //doThrow(env, "java/lang/IllegalArgumentException", NULL);
        return E_FAIL;
    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::SetTransparentRegionHint(
    /* [in] */ IRegion * pRegion)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::SetAlpha(
    /* [in] */ Float alpha)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::SetMatrix(
    /* [in] */ Float dsdx,
    /* [in] */ Float dtdx,
    /* [in] */ Float dsdy,
    /* [in] */ Float dtdy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::Freeze()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::Unfreeze()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::SetFreezeTint(
    /* [in] */ Int32 tint)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurface::SetFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
#ifdef _linux
    android::status_t err = mSurfaceControl->setFlags(flags, mask);
    if (err < 0 && err != android::NO_INIT) {
        //doThrow(env, "java/lang/IllegalArgumentException", NULL);
        return E_FAIL;
    }
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CSurface::constructor(
    /* [in] */ ISurfaceSession * pS,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 display,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 format,
    /* [in] */ Int32 flags)
{
//	    if (DEBUG_RELEASE) {
//	        mCreationStack = new Exception();
//	    }
    mCanvas = new CompatibleCanvas();
    if (!mCanvas.Get()) {
        return E_OUT_OF_MEMORY;
    }
    return Init(pS, pid, NULL, display, w, h, format, flags);
}

ECode CSurface::constructor(
    /* [in] */ ISurfaceSession * pS,
    /* [in] */ Int32 pid,
    /* [in] */ String name,
    /* [in] */ Int32 display,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 format,
    /* [in] */ Int32 flags)
{
//	    if (DEBUG_RELEASE) {
//	        mCreationStack = new Exception();
//	    }
    mCanvas = new CompatibleCanvas();
    if (!mCanvas.Get()) {
        return E_OUT_OF_MEMORY;
    }
    ECode ec = Init(pS, pid, name, display, w, h, format, flags);
    mName = String::Duplicate(name);
    return ec;
}

ECode CSurface::constructor()
{
//	    if (DEBUG_RELEASE) {
//	        mCreationStack = new Exception();
//	    }
    mCanvas = new CompatibleCanvas();
    if (!mCanvas.Get()) {
        return E_OUT_OF_MEMORY;
    }
    return NOERROR;
}

ECode CSurface::Init(
    /* [in] */ ISurfaceSession * pS,
    /* [in] */ Int32 pid,
    /* [in] */ String name,
    /* [in] */ Int32 display,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 format,
    /* [in] */ Int32 flags)
{
#ifdef _linux
    if (!pS) {
        return E_INVALID_ARGUMENT;
    }

    android::SurfaceComposerClient * client =
            ((CSurfaceSession*)pS)->mClient.get();
    assert(client);

    android::sp<android::SurfaceControl> surface;
    if (name.IsNull()) {
        surface = client->createSurface(pid, display, w, h, format, flags);
    } else {
        surface = client->createSurface(pid,
            android::String8((const char *)name), display, w, h, format, flags);
    }

    if (!surface.get()) {
        return E_OUT_OF_MEMORY;
    }

    mSurfaceControl = surface;
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

#ifdef _linux
void CSurface::SetSurfaceControl(
        /* [in] */ android::sp<android::SurfaceControl> & surface)
{
    if (surface.get()) {
        surface->incStrong(this);
    }
    if (mSurfaceControl.get()) {
        mSurfaceControl->decStrong(this);
    }
    mSurfaceControl = surface;
}
#endif

void CSurface::GetSurface()
{
#ifdef _linux
    if (!mNativeSurface.get() && mSurfaceControl.get()) {
        /*
         * if this method is called from the WindowManager's process, it means
         * the client is is not remote, and therefore is allowed to have
         * a Surface (data), so we create it here.
         * If we don't have a SurfaceControl, it means we're in a different
         * process.
         */
        mNativeSurface = mSurfaceControl->getSurface();
        if (mNativeSurface.get()) {
            mNativeSurface->incStrong(this);
        }
    }
#endif
}

