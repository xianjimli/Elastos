
#include "ext/frameworkdef.h"
#include "graphics/CPicture.h"
#include "graphics/SkiaIOStreamAdaptor.h"


////////////////////////////////////////////////////////////
// CPicture::RecordingCanvas
CPicture::RecordingCanvas::RecordingCanvas(
    /* [in] */ CPicture* pict,
    /* [in] */ SkCanvas* nativeCanvas)
{
    Canvas::Init(nativeCanvas);
    mPicture = pict;
}

PInterface CPicture::RecordingCanvas::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ICanvas*)this;
    }
    else if (riid == EIID_Canvas) {
        return reinterpret_cast<PInterface>((Canvas*)this);
    }
    else if (riid == EIID_ICanvas) {
        return (ICanvas*)this;
    }

    return NULL;
}

UInt32 CPicture::RecordingCanvas::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPicture::RecordingCanvas::Release()
{
    return ElRefBase::Release();
}

ECode CPicture::RecordingCanvas::GetInterfaceID(
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

ECode CPicture::RecordingCanvas::GetGL(
    /* [out] */ IGL** gl)
{
    VALIDATE_NOT_NULL(gl);
    return Canvas::GetGL(gl);
}

ECode CPicture::RecordingCanvas::FreeGlCaches()
{
    Canvas::FreeGlCaches();
    return NOERROR;
}

ECode CPicture::RecordingCanvas::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
//    throw new RuntimeException(
//            "Cannot call setBitmap on a picture canvas");
    return E_RUNTIME_EXCEPTION;
}

ECode CPicture::RecordingCanvas::SetViewport(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Canvas::SetViewport(width, height);
}

ECode CPicture::RecordingCanvas::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);
    return Canvas::IsOpaque(isOpaque);
}

ECode CPicture::RecordingCanvas::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    //return mCompatibleDisplayMetrics == null ?
    //    super.getWidth() : mCompatibleDisplayMetrics.widthPixels;

    return E_NOT_IMPLEMENTED;
}

ECode CPicture::RecordingCanvas::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    //return mCompatibleDisplayMetrics == null ?
    //    super.getHeight() : mCompatibleDisplayMetrics.heightPixels;

    return E_NOT_IMPLEMENTED;
}

ECode CPicture::RecordingCanvas::GetDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);
    return Canvas::GetDensity(density);
}

ECode CPicture::RecordingCanvas::SetDensity(
    /* [in] */ Int32 density)
{
    return Canvas::SetDensity(density);
}

ECode CPicture::RecordingCanvas::SetScreenDensity(
    /* [in] */ Int32 density)
{
    return Canvas::SetScreenDensity(density);
}

ECode CPicture::RecordingCanvas::Save(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::Save(count);
}

ECode CPicture::RecordingCanvas::SaveEx(
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveEx(saveFlags, count);
}

ECode CPicture::RecordingCanvas::SaveLayer(
    /* [in] */ IRectF* bounds,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveLayer(bounds, paint, saveFlags, count);
}

ECode CPicture::RecordingCanvas::SaveLayerEx(
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

ECode CPicture::RecordingCanvas::SaveLayerAlpha(
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::SaveLayerAlpha(bounds, alpha, saveFlags, count);
}

ECode CPicture::RecordingCanvas::SaveLayerAlphaEx(
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

ECode CPicture::RecordingCanvas::Restore()
{
    return Canvas::Restore();
}

ECode CPicture::RecordingCanvas::GetSaveCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return Canvas::GetSaveCount(count);
}

ECode CPicture::RecordingCanvas::RestoreToCount(
    /* [in] */ Int32 saveCount)
{
    return Canvas::RestoreToCount(saveCount);
}

ECode CPicture::RecordingCanvas::Translate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    return Canvas::Translate(dx, dy);
}

ECode CPicture::RecordingCanvas::Scale(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    return Canvas::Scale(sx, sy);
}

ECode CPicture::RecordingCanvas::ScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Canvas::ScaleEx(sx, sy, px, py);
}

ECode CPicture::RecordingCanvas::Rotate(
    /* [in] */ Float degrees)
{
    return Canvas::Rotate(degrees);
}

ECode CPicture::RecordingCanvas::RotateEx(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Canvas::RotateEx(degrees, px, py);
}

ECode CPicture::RecordingCanvas::Skew(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    return Canvas::Skew(sx, sy);
}

ECode CPicture::RecordingCanvas::Concat(
    /* [in] */ IMatrix* matrix)
{
    VALIDATE_NOT_NULL(matrix);
    return Canvas::Concat(matrix);
}

ECode CPicture::RecordingCanvas::SetMatrix(
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

ECode CPicture::RecordingCanvas::GetMatrix(
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

ECode CPicture::RecordingCanvas::GetMatrixEx(
    /* [out] */ IMatrix** metrix)
{
    VALIDATE_NOT_NULL(metrix);
    return Canvas::GetMatrixEx(metrix);
}

ECode CPicture::RecordingCanvas::ClipRect(
    /* [in] */ IRectF* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRect(rect, op, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipRectEx(
    /* [in] */ IRect* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx(rect, op, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipRectEx2(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx2(rect, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipRectEx3(
    /* [in] */ IRect* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(rect && isNotEmpty);
    return Canvas::ClipRectEx3(rect, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipRectEx4(
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

ECode CPicture::RecordingCanvas::ClipRectEx5(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    return Canvas::ClipRectEx5(left, top, right, bottom, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipRectEx6(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    return Canvas::ClipRectEx6(left, top, right, bottom, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipPath(
    /* [in] */ IPath* path,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(path && isNotEmpty);
    return Canvas::ClipPath(path, op, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipPathEx(
    /* [in] */ IPath* path,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(path && isNotEmpty);
    return Canvas::ClipPathEx(path, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipRegion(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(region && isNotEmpty);
    return Canvas::ClipRegion(region, op, isNotEmpty);
}

ECode CPicture::RecordingCanvas::ClipRegionEx(
    /* [in] */ IRegion* region,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(region && isNotEmpty);
    return Canvas::ClipRegionEx(region, isNotEmpty);
}

ECode CPicture::RecordingCanvas::GetDrawFilter(
    /* [out] */ IDrawFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    return Canvas::GetDrawFilter(filter);
}

ECode CPicture::RecordingCanvas::SetDrawFilter(
    /* [in] */ IDrawFilter* filter)
{
    VALIDATE_NOT_NULL(filter);
    return Canvas::SetDrawFilter(filter);
}

ECode CPicture::RecordingCanvas::QuickRejectWithRectF(
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(rect && isNotIntersect);
    return Canvas::QuickRejectWithRectF(rect, type, isNotIntersect);
}

ECode CPicture::RecordingCanvas::QuickRejectWithPath(
    /* [in] */ IPath* path,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(path && isNotIntersect);
    return Canvas::QuickRejectWithPath(path, type, isNotIntersect);
}

ECode CPicture::RecordingCanvas::QuickReject(
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

ECode CPicture::RecordingCanvas::GetClipBounds(
    /* [in] */ IRect* bounds,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(bounds && isNotEmpty);
    return Canvas::GetClipBounds(bounds, isNotEmpty);
}

ECode CPicture::RecordingCanvas::GetClipBoundsEx(
    /* [out] */ IRect** bounds)
{
    VALIDATE_NOT_NULL(bounds);
    return Canvas::GetClipBoundsEx(bounds);
}

ECode CPicture::RecordingCanvas::DrawRGB(
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Canvas::DrawRGB(r, g, b);
}

ECode CPicture::RecordingCanvas::DrawARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Canvas::DrawARGB(a, r, g, b);
}

ECode CPicture::RecordingCanvas::DrawColor(
    /* [in] */ Int32 color)
{
    return Canvas::DrawColor(color);
}

ECode CPicture::RecordingCanvas::DrawColorEx(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    return Canvas::DrawColorEx(color, mode);
}

ECode CPicture::RecordingCanvas::DrawPaint(
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPaint(paint);
}

ECode CPicture::RecordingCanvas::DrawPoints(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPoints(pts, offset, count, paint);
}

ECode CPicture::RecordingCanvas::DrawPointsEx(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPointsEx(pts, paint);
}

ECode CPicture::RecordingCanvas::DrawPoint(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPoint(x, y, paint);
}

ECode CPicture::RecordingCanvas::DrawLine(
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLine(startX, startY, stopX, stopY, paint);
}

ECode CPicture::RecordingCanvas::DrawLines(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLines(pts, offset, count, paint);
}

ECode CPicture::RecordingCanvas::DrawLinesEx(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawLinesEx(pts, paint);
}

ECode CPicture::RecordingCanvas::DrawRect(
    /* [in] */ IRectF* rect,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(rect && paint);
    return Canvas::DrawRect(rect, paint);
}

ECode CPicture::RecordingCanvas::DrawRectEx(
    /* [in] */ IRect* r,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(r && paint);
    return Canvas::DrawRectEx(r, paint);
}

ECode CPicture::RecordingCanvas::DrawRectEx2(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint * paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawRectEx2(left, top, right, bottom, paint);
}

ECode CPicture::RecordingCanvas::DrawOval(
    /* [in] */ IRectF* oval,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawOval(oval, paint);
}

ECode CPicture::RecordingCanvas::DrawCircle(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawCircle(cx, cy, radius, paint);
}

ECode CPicture::RecordingCanvas::DrawArc(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawArc(oval, startAngle, sweepAngle, useCenter, paint);
}

ECode CPicture::RecordingCanvas::DrawRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawRoundRect(rect, rx, ry, paint);
}

ECode CPicture::RecordingCanvas::DrawPath(
    /* [in] */ IPath* path,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(path && paint);
    return Canvas::DrawPath(path, paint);
}

ECode CPicture::RecordingCanvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmap(bitmap, left, top, paint);
}

ECode CPicture::RecordingCanvas::DrawBitmapEx(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmapEx(bitmap, src, dst, paint);
}

ECode CPicture::RecordingCanvas::DrawBitmapEx2(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dst,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && paint);
    return Canvas::DrawBitmapEx2(bitmap, src, dst, paint);
}

ECode CPicture::RecordingCanvas::DrawBitmapEx3(
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

ECode CPicture::RecordingCanvas::DrawBitmapEx4(
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

ECode CPicture::RecordingCanvas::DrawBitmapEx5(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IMatrix* matrix,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(bitmap && matrix);
    return Canvas::DrawBitmapEx5(bitmap, matrix, paint);
}

ECode CPicture::RecordingCanvas::DrawBitmapMesh(
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

ECode CPicture::RecordingCanvas::DrawVertices(
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

ECode CPicture::RecordingCanvas::DrawTextInBuffer(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawTextInBuffer(text, index, count, x, y, paint);
}

ECode CPicture::RecordingCanvas::DrawTextInString(
    /* [in] */ const String& text,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawTextInString(text, x, y, paint);
}

ECode CPicture::RecordingCanvas::DrawTextInStringEx(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawTextInStringEx(text, start, end, x, y, paint);
}

ECode CPicture::RecordingCanvas::DrawTextInCharSequence(
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

ECode CPicture::RecordingCanvas::DrawPosTextInBuffer(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPosTextInBuffer(text, index, count, pos, paint);
}

ECode CPicture::RecordingCanvas::DrawPosTextInString(
    /* [in] */ const String& text,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    VALIDATE_STRING_NOT_NULL(text);
    VALIDATE_NOT_NULL(paint);
    return Canvas::DrawPosTextInString(text, pos, paint);
}

ECode CPicture::RecordingCanvas::DrawTextOnPathInBuffer(
    /* [in] */ const ArrayOf<Char8>& text,
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

ECode CPicture::RecordingCanvas::DrawTextOnPathInString(
    /* [in] */ const String& text,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(path && paint);
    return Canvas::DrawTextOnPathInString(text, path, hOffset, vOffset, paint);
}

ECode CPicture::RecordingCanvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    VALIDATE_NOT_NULL(picture);
    if (mPicture == picture) {
//        throw new RuntimeException(
//                    "Cannot draw a picture into its recording canvas");
        return E_RUNTIME_EXCEPTION;
    }
    return Canvas::DrawPicture(picture);
}

ECode CPicture::RecordingCanvas::DrawPictureEx(
    /* [in] */ IPicture* picture,
    /* [in] */ IRectF* dst)
{
    VALIDATE_NOT_NULL(picture && dst);
    return Canvas::DrawPictureEx(picture, dst);
}

ECode CPicture::RecordingCanvas::DrawPictureEx2(
    /* [in] */ IPicture* picture,
    /* [in] */ IRect* dst)
{
    VALIDATE_NOT_NULL(picture && dst);
    return Canvas::DrawPictureEx2(picture, dst);
}

ECode CPicture::RecordingCanvas::FreeCaches()
{
    Canvas::FreeCaches();
    return NOERROR;
}



////////////////////////////////////////////////////////////
// CPicture
const Int32 CPicture::WORKING_STREAM_STORAGE;

CPicture::CPicture()
{
//     this(nativeConstructor(0));
}

CPicture::~CPicture()
{
    NativeDestructor(mNativePicture);
}

/**
 * To record a picture, call beginRecording() and then draw into the Canvas
 * that is returned. Nothing we appear on screen, but all of the draw
 * commands (e.g. drawRect(...)) will be recorded. To stop recording, call
 * endRecording(). At this point the Canvas that was returned must no longer
 * be referenced, and nothing should be drawn into it.
 */
ECode CPicture::BeginRecording(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);
    SkCanvas* ni = NativeBeginRecording(mNativePicture, width, height);
    mRecordingCanvas = new RecordingCanvas(this, ni);
    *canvas = (ICanvas*)mRecordingCanvas;
    (*canvas)->AddRef();
    return NOERROR;
}

/**
 * Call endRecording when the picture is built. After this call, the picture
 * may be drawn, but the canvas that was returned by beginRecording must not
 * be referenced anymore. This is automatically called if Picture.draw() or
 * Canvas.drawPicture() is called.
 */
ECode CPicture::EndRecording()
{
    if (mRecordingCanvas != NULL) {
        mRecordingCanvas = NULL;
        NativeEndRecording(mNativePicture);
    }
    return NOERROR;
}

/**
 * Get the width of the picture as passed to beginRecording. This
 * does not reflect (per se) the content of the picture.
 */
ECode CPicture::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mNativePicture->width();
    return NOERROR;
}

/**
 * Get the height of the picture as passed to beginRecording. This
 * does not reflect (per se) the content of the picture.
 */
ECode CPicture::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mNativePicture->height();
    return NOERROR;
}

/**
 * Draw this picture on the canvas. The picture may have the side effect
 * of changing the matrix and clip of the canvas.
 *
 * @param canvas  The picture is drawn to this canvas
 */
ECode CPicture::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mRecordingCanvas != NULL) {
        EndRecording();
    }
    NativeDraw(
        ((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas, mNativePicture);
    return NOERROR;
}

/**
 * Create a new picture (already recorded) from the data in the stream. This
 * data was generated by a previous call to writeToStream().
 */
AutoPtr<IPicture> CPicture::CreateFromStream(
    /* [in] */ IInputStream* stream)
{
    AutoPtr<IPicture> picture;
    ArrayOf_<Byte, WORKING_STREAM_STORAGE> storage;
    SkPicture* ni = NativeCreateFromStream(stream, &storage);
    CPicture::New((Handle32)ni, (IPicture**)&picture);
    return picture;
}

/**
 * Write the picture contents to a stream. The data can be used to recreate
 * the picture in this or another process by calling createFromStream.
 */
ECode CPicture::WriteToStream(
    /* [in] */ IOutputStream* stream)
{
    // do explicit check before calling the native method
    if (stream == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    ArrayOf_<Byte, WORKING_STREAM_STORAGE> storage;
    if (!NativeWriteToStream(mNativePicture, stream, &storage)) {
//        throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

SkPicture* CPicture::Ni()
{
    return mNativePicture;
}

SkPicture* CPicture::NativeConstructor(
    /* [in] */ SkPicture* nativeSrcOr0)
{
    if (nativeSrcOr0) {
        return new SkPicture(*nativeSrcOr0);
    }
    else {
        return new SkPicture;
    }
}

SkPicture* CPicture::NativeCreateFromStream(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    SkPicture* picture = NULL;
    SkStream* strm = CreateSkiaInputStreamAdaptor(stream, storage);
    if (strm) {
        picture = new SkPicture(strm);
        delete strm;
    }
    return picture;
}

SkCanvas* CPicture::NativeBeginRecording(
    /* [in] */ SkPicture* nativePicture,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    // beginRecording does not ref its return value, it just returns it.
    SkCanvas* canvas = nativePicture->beginRecording(w, h);
    // the java side will wrap this guy in a Canvas.java, which will call
    // unref in its finalizer, so we have to ref it here, so that both that
    // Canvas.java and our picture can both be owners
    canvas->ref();
    return canvas;
}

void CPicture::NativeEndRecording(
    /* [in] */ SkPicture* nativePicture)
{
    nativePicture->endRecording();
}

void CPicture::NativeDraw(
    /* [in] */ SkCanvas* nativeCanvas,
    /* [in] */ SkPicture* nativePicture)
{
    SkASSERT(nativeCanvas);
    SkASSERT(nativePicture);
    nativePicture->draw(nativeCanvas);
}

Boolean CPicture::NativeWriteToStream(
    /* [in] */ SkPicture* nativePicture,
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    SkWStream* strm = CreateSkiaOutputStreamAdaptor(stream, storage);

    if (NULL != strm) {
        nativePicture->serialize(strm);
        delete strm;
        return TRUE;
    }
    return FALSE;
}

void CPicture::NativeDestructor(
    /* [in] */ SkPicture* nativePicture)
{
    SkASSERT(nativePicture);
    delete nativePicture;
}

ECode CPicture::constructor()
{
    return constructor((Handle32)NativeConstructor(0));
}

/**
 * Create a picture by making a copy of what has already been recorded in
 * src. The contents of src are unchanged, and if src changes later, those
 * changes will not be reflected in this picture.
 */
ECode CPicture::constructor(
    /* [in] */ IPicture* src)
{
    return constructor((Handle32)NativeConstructor(
            src != NULL ? ((CPicture*)src)->mNativePicture : NULL));
}

ECode CPicture::constructor(
    /* [in] */ Handle32 nativePicture)
{
    if (nativePicture == 0) {
//        throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }
    mNativePicture = (SkPicture*)nativePicture;
    return NOERROR;
}
