
#include "graphics/CCanvas.h"
#include "ext/frameworkdef.h"

const Int32 CCanvas::MATRIX_SAVE_FLAG;
const Int32 CCanvas::CLIP_SAVE_FLAG;
const Int32 CCanvas::HAS_ALPHA_LAYER_SAVE_FLAG;
const Int32 CCanvas::FULL_COLOR_LAYER_SAVE_FLAG;
const Int32 CCanvas::CLIP_TO_LAYER_SAVE_FLAG;
const Int32 CCanvas::ALL_SAVE_FLAG;

PInterface CCanvas::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Canvas) {
        return reinterpret_cast<PInterface>((Canvas*)this);
    }
    return _CCanvas::Probe(riid);
}

ECode CCanvas::GetGL(
    /* [out] */ IGL** gl)
{
    VALIDATE_NOT_NULL(gl);
    return Canvas::GetGL(gl);
}

ECode CCanvas::FreeGlCaches()
{
    Canvas::FreeGlCaches();
    return NOERROR;
}

ECode CCanvas::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return Canvas::SetBitmap(bitmap);
}

ECode CCanvas::SetViewport(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Canvas::SetViewport(width, height);
}

ECode CCanvas::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);
    return Canvas::IsOpaque(isOpaque);
}

ECode CCanvas::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return Canvas::GetWidth(width);
}

ECode CCanvas::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    return Canvas::GetHeight(height);
}

ECode CCanvas::GetDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);
    return Canvas::GetDensity(density);
}

ECode CCanvas::SetDensity(
    /* [in] */ Int32 density)
{
    return Canvas::SetDensity(density);
}

ECode CCanvas::SetScreenDensity(
    /* [in] */ Int32 density)
{
    return Canvas::SetScreenDensity(density);
}

ECode CCanvas::Save(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::Save(count);
}

ECode CCanvas::SaveEx(
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveEx(saveFlags, count);
}

ECode CCanvas::SaveLayer(
    /* [in] */ IRectF* bounds,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveLayer(bounds, paint, saveFlags, count);
}

ECode CCanvas::SaveLayerEx(
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

ECode CCanvas::SaveLayerAlpha(
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveLayerAlpha(bounds, alpha, saveFlags, count);
}

ECode CCanvas::SaveLayerAlphaEx(
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

ECode CCanvas::Restore()
{
    return Canvas::Restore();
}

ECode CCanvas::GetSaveCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::GetSaveCount(count);
}

ECode CCanvas::RestoreToCount(
    /* [in] */ Int32 saveCount)
{
    return Canvas::RestoreToCount(saveCount);
}

ECode CCanvas::Translate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    return Canvas::Translate(dx, dy);
}

ECode CCanvas::Scale(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    return Canvas::Scale(sx, sy);
}

ECode CCanvas::ScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Canvas::ScaleEx(sx, sy, px, py);
}

ECode CCanvas::Rotate(
    /* [in] */ Float degrees)
{
    return Canvas::Rotate(degrees);
}

ECode CCanvas::RotateEx(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Canvas::RotateEx(degrees, px, py);
}

ECode CCanvas::Skew(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    return Canvas::Skew(sx, sy);
}

ECode CCanvas::Concat(
    /* [in] */ IMatrix* matrix)
{
    VALIDATE_NOT_NULL(matrix);
    return Canvas::Concat(matrix);
}

ECode CCanvas::SetMatrix(
    /* [in] */ IMatrix* matrix)
{
    return Canvas::SetMatrix(matrix);
}

ECode CCanvas::GetMatrix(
    /* [in] */ IMatrix* ctm)
{
    VALIDATE_NOT_NULL(ctm);
    return Canvas::GetMatrix(ctm);
}

ECode CCanvas::GetMatrixEx(
    /* [out] */ IMatrix** metrix)
{
    VALIDATE_NOT_NULL(metrix);
    return Canvas::GetMatrixEx(metrix);
}

ECode CCanvas::ClipRect(
    /* [in] */ IRectF* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRect(rect, op, isNotEmpty);
}

ECode CCanvas::ClipRectEx(
    /* [in] */ IRect* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx(rect, op, isNotEmpty);
}

ECode CCanvas::ClipRectEx2(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx2(rect, isNotEmpty);
}

ECode CCanvas::ClipRectEx3(
    /* [in] */ IRect* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx3(rect, isNotEmpty);
}

ECode CCanvas::ClipRectEx4(
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

ECode CCanvas::ClipRectEx5(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    return Canvas::ClipRectEx5(left, top, right, bottom, isNotEmpty);
}

ECode CCanvas::ClipRectEx6(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    return Canvas::ClipRectEx6(left, top, right, bottom, isNotEmpty);
}

ECode CCanvas::ClipPath(
    /* [in] */ IPath* path,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(path && isNotEmpty);
    return Canvas::ClipPath(path, op, isNotEmpty);
}

ECode CCanvas::ClipPathEx(
    /* [in] */ IPath* path,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(path && isNotEmpty);
    return Canvas::ClipPathEx(path, isNotEmpty);
}

ECode CCanvas::ClipRegion(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(region && isNotEmpty);
    return Canvas::ClipRegion(region, op, isNotEmpty);
}

ECode CCanvas::ClipRegionEx(
    /* [in] */ IRegion* region,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(region && isNotEmpty);
    return Canvas::ClipRegionEx(region, isNotEmpty);
}

ECode CCanvas::GetDrawFilter(
    /* [out] */ IDrawFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    return Canvas::GetDrawFilter(filter);
}

ECode CCanvas::SetDrawFilter(
    /* [in] */ IDrawFilter* filter)
{
    VALIDATE_NOT_NULL(filter);
    return Canvas::SetDrawFilter(filter);
}

ECode CCanvas::QuickRejectWithRectF(
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(rect && isNotIntersect);
    return Canvas::QuickRejectWithRectF(rect, type, isNotIntersect);
}

ECode CCanvas::QuickRejectWithPath(
    /* [in] */ IPath* path,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(path && isNotIntersect);
    return Canvas::QuickRejectWithPath(path, type, isNotIntersect);
}

ECode CCanvas::QuickReject(
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

ECode CCanvas::GetClipBounds(
    /* [in] */ IRect* bounds,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(bounds && isNotEmpty);
    return Canvas::GetClipBounds(bounds, isNotEmpty);
}

ECode CCanvas::GetClipBoundsEx(
    /* [out] */ IRect** bounds)
{
    VALIDATE_NOT_NULL(bounds);
    return Canvas::GetClipBoundsEx(bounds);
}

ECode CCanvas::DrawRGB(
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Canvas::DrawRGB(r, g, b);
}

ECode CCanvas::DrawARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Canvas::DrawARGB(a, r, g, b);
}

ECode CCanvas::DrawColor(
    /* [in] */ Int32 color)
{
    return Canvas::DrawColor(color);
}

ECode CCanvas::DrawColorEx(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    return Canvas::DrawColorEx(color, mode);
}

ECode CCanvas::DrawPaint(
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPaint(paint);
}

ECode CCanvas::DrawPoints(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPoints(pts, offset, count, paint);
}

ECode CCanvas::DrawPointsEx(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPointsEx(pts, paint);
}

ECode CCanvas::DrawPoint(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPoint(x, y, paint);
}

ECode CCanvas::DrawLine(
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLine(startX, startY, stopX, stopY, paint);
}

ECode CCanvas::DrawLines(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLines(pts, offset, count, paint);
}

ECode CCanvas::DrawLinesEx(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLinesEx(pts, paint);
}

ECode CCanvas::DrawRect(
    /* [in] */ IRectF* rect,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(rect && paint);
    return Canvas::DrawRect(rect, paint);
}

ECode CCanvas::DrawRectEx(
    /* [in] */ IRect* r,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(r && paint);
    return Canvas::DrawRectEx(r, paint);
}

ECode CCanvas::DrawRectEx2(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint * paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawRectEx2(left, top, right, bottom, paint);
}

ECode CCanvas::DrawOval(
    /* [in] */ IRectF* oval,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawOval(oval, paint);
}

ECode CCanvas::DrawCircle(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawCircle(cx, cy, radius, paint);
}

ECode CCanvas::DrawArc(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawArc(oval, startAngle, sweepAngle, useCenter, paint);
}

ECode CCanvas::DrawRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawRoundRect(rect, rx, ry, paint);
}

ECode CCanvas::DrawPath(
    /* [in] */ IPath* path,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(path && paint);
    return Canvas::DrawPath(path, paint);
}

ECode CCanvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmap(bitmap, left, top, paint);
}

ECode CCanvas::DrawBitmapEx(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmapEx(bitmap, src, dst, paint);
}

ECode CCanvas::DrawBitmapEx2(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dst,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmapEx2(bitmap, src, dst, paint);
}

ECode CCanvas::DrawBitmapEx3(
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

ECode CCanvas::DrawBitmapEx4(
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

ECode CCanvas::DrawBitmapEx5(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IMatrix* matrix,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && matrix);
    return Canvas::DrawBitmapEx5(bitmap, matrix, paint);
}

ECode CCanvas::DrawBitmapMesh(
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

ECode CCanvas::DrawVertices(
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

ECode CCanvas::DrawTextInBuffer(
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

ECode CCanvas::DrawTextInString(
    /* [in] */ String text,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawTextInString(text, x, y, paint);
}

ECode CCanvas::DrawTextInStringEx(
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

ECode CCanvas::DrawTextInCharSequence(
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

ECode CCanvas::DrawPosTextInBuffer(
    /* [in] */ const BufferOf<Byte>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPosTextInBuffer(text, index, count, pos, paint);
}

ECode CCanvas::DrawPosTextInString(
    /* [in] */ String text,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    VALIDATE_STRING_NOT_NULL(text);
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPosTextInString(text, pos, paint);
}

ECode CCanvas::DrawTextOnPathInBuffer(
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

ECode CCanvas::DrawTextOnPathInString(
    /* [in] */ String text,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(path && paint);
    return Canvas::DrawTextOnPathInString(text, path, hOffset, vOffset, paint);
}

ECode CCanvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    VALIDATE_NOT_NULL(picture);
    return Canvas::DrawPicture(picture);
}

ECode CCanvas::DrawPictureEx(
    /* [in] */ IPicture* picture,
    /* [in] */ IRectF* dst)
{
    VALIDATE_NOT_NULL(picture && dst);
    return Canvas::DrawPictureEx(picture, dst);
}

ECode CCanvas::DrawPictureEx2(
    /* [in] */ IPicture* picture,
    /* [in] */ IRect* dst)
{
    VALIDATE_NOT_NULL(picture && dst);
    return Canvas::DrawPictureEx2(picture, dst);
}

ECode CCanvas::FreeCaches()
{
    Canvas::FreeCaches();
    return NOERROR;
}

ECode CCanvas::constructor()
{
    return Canvas::Init();
}

ECode CCanvas::constructor(
    /* [in] */ IBitmap* bitmap)
{
    return Canvas::Init(bitmap);
}

ECode CCanvas::constructor(
    /* [in] */ Int32 nativeCanvas)
{
    return Canvas::Init((SkCanvas*)nativeCanvas);
}

ECode CCanvas::constructor(
    /* [in] */ IGL* gl)
{
    return Canvas::Init(gl);
}

