

#ifndef __HH_CANVAS_H
#define __HH_CANVAS_H

#include <ext/frameworkext.h>
#include "graphics/CBitmap.h"
#include <elastos/AutoPtr.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkPaint.h>
#include <skia/core/SkRect.h>
#include <skia/effects/SkPorterDuff.h>
#include <utils/RefBase.h>

extern "C" const InterfaceID EIID_Canvas;

class CSurface;

class Canvas
{
    friend class CSurface;

public:
    Canvas();

    virtual ~Canvas();

public:
    virtual CARAPI GetGL(
        /* [out] */ IGL** gl);

    static CARAPI_(void) FreeGlCaches();

    virtual CARAPI SetBitmap(
        /* [in] */ IBitmap* bitmap);

    virtual CARAPI SetViewport(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    virtual CARAPI GetWidth(
        /* [out] */ Int32* width);

    virtual CARAPI GetHeight(
        /* [out] */ Int32* height);

    virtual CARAPI GetDensity(
        /* [out] */ Int32* density);

    virtual CARAPI SetDensity(
        /* [in] */ Int32 density);

    virtual CARAPI SetScreenDensity(
        /* [in] */ Int32 density);

    virtual CARAPI Save(
        /* [out] */ Int32* count);

    virtual CARAPI SaveEx(
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI SaveLayer(
        /* [in] */ IRectF* bounds,
        /* [in] */ IPaint* paint,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI SaveLayerEx(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ IPaint* paint,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI SaveLayerAlpha(
        /* [in] */ IRectF* bounds,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI SaveLayerAlphaEx(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI Restore();

    virtual CARAPI GetSaveCount(
        /* [out] */ Int32* count);

    virtual CARAPI RestoreToCount(
        /* [in] */ Int32 saveCount);

    virtual CARAPI Translate(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    virtual CARAPI Scale(
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    CARAPI ScaleEx(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);

    virtual CARAPI Rotate(
        /* [in] */ Float degrees);

    CARAPI RotateEx(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);

    virtual CARAPI Skew(
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    virtual CARAPI Concat(
        /* [in] */ IMatrix* matrix);

    virtual CARAPI SetMatrix(
        /* [in] */ IMatrix* matrix);

    virtual CARAPI GetMatrix(
        /* [in] */ IMatrix* ctm);

    virtual CARAPI GetMatrixEx(
        /* [out] */ IMatrix** metrix);

    virtual CARAPI ClipRect(
        /* [in] */ IRectF* rect,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRectEx(
        /* [in] */ IRect* rect,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRectEx2(
        /* [in] */ IRectF* rect,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRectEx3(
        /* [in] */ IRect* rect,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRectEx4(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRectEx5(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRectEx6(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipPath(
        /* [in] */ IPath* path,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipPathEx(
        /* [in] */ IPath* path,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRegion(
        /* [in] */ IRegion* region,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRegionEx(
        /* [in] */ IRegion* region,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI GetDrawFilter(
        /* [out] */ IDrawFilter** filter);

    virtual CARAPI SetDrawFilter(
        /* [in] */ IDrawFilter* filter);

    virtual CARAPI QuickRejectWithRectF(
        /* [in] */ IRectF* rect,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    virtual CARAPI QuickRejectWithPath(
        /* [in] */ IPath* path,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    virtual CARAPI QuickReject(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    virtual CARAPI GetClipBounds(
        /* [in] */ IRect* bounds,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI GetClipBoundsEx(
        /* [out] */ IRect** bounds);

    virtual CARAPI DrawRGB(
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    virtual CARAPI DrawARGB(
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    virtual CARAPI DrawColor(
        /* [in] */ Int32 color);

    virtual CARAPI DrawColorEx(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode);

    virtual CARAPI DrawPaint(
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPoints(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPointsEx(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPoint(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawLine(
        /* [in] */ Float startX,
        /* [in] */ Float startY,
        /* [in] */ Float stopX,
        /* [in] */ Float stopY,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawLines(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawLinesEx(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawRect(
        /* [in] */ IRectF* rect,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawRectEx(
        /* [in] */ IRect* r,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawRectEx2(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawOval(
        /* [in] */ IRectF* oval,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawCircle(
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Float radius,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawArc(
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle,
        /* [in] */ Boolean useCenter,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawRoundRect(
        /* [in] */ IRectF* rect,
        /* [in] */ Float rx,
        /* [in] */ Float ry,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPath(
        /* [in] */ IPath* path,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmap(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmapEx(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRectF* dst,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmapEx2(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRect* dst,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmapEx3(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean hasAlpha,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmapEx4(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean hasAlpha,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmapEx5(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IMatrix* matrix,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmapMesh(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ Int32 meshWidth,
        /* [in] */ Int32 meshHeight,
        /* [in] */ const ArrayOf<Float>& verts,
        /* [in] */ Int32 vertOffset,
        /* [in] */ ArrayOf<Int32>* colors,
        /* [in] */ Int32 colorOffset,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawVertices(
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
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextInBuffer(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextInString(
        /* [in] */ String text,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextInStringEx(
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextInCharSequence(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPosTextInBuffer(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPosTextInString(
        /* [in] */ String text,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextOnPathInBuffer(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IPath* path,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextOnPathInString(
        /* [in] */ String text,
        /* [in] */ IPath* path,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPicture(
        /* [in] */ IPicture* picture);

    virtual CARAPI DrawPictureEx(
        /* [in] */ IPicture* picture,
        /* [in] */ IRectF* dst);

    virtual CARAPI DrawPictureEx2(
        /* [in] */ IPicture* picture,
        /* [in] */ IRect* dst);

    static CARAPI_(void) FreeCaches();

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ IBitmap* bitmap);

    CARAPI Init(
        /* [in] */ SkCanvas* nativeCanvas);

    CARAPI Init(
        /* [in] */ IGL* gl);

private:
    static CARAPI ThrowIfRecycled(
        /* [in] */ IBitmap* bitmap);

    CARAPI_(SkCanvas*) InitRaster(
        /* [in] */ SkBitmap* bitmap);

    CARAPI_(SkCanvas*) InitGL();

    static CARAPI CheckRange(
        /* [in] */ Int32 length,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    static CARAPI_(void) NativeSetBitmap(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkBitmap* nativeBitmap);

    static CARAPI_(void) NativeSetViewport(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    static CARAPI_(Int32) NativeSaveLayer(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ IRectF* bounds,
        /* [in] */ SkPaint* nativePaint,
        /* [in] */ Int32 flags);

    static CARAPI_(Int32) NativeSaveLayer(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Float l,
        /* [in] */ Float t,
        /* [in] */ Float r,
        /* [in] */ Float b,
        /* [in] */ SkPaint* nativePaint,
        /* [in] */ Int32 flags);

    static CARAPI_(Int32) NativeSaveLayerAlpha(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ IRectF* bounds,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 flags);

    static CARAPI_(Int32) NativeSaveLayerAlpha(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Float l,
        /* [in] */ Float t,
        /* [in] */ Float r,
        /* [in] */ Float b,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 flags);

    static CARAPI_(void) NativeConcat(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkMatrix* nativeMatrix);

    static CARAPI_(void) NativeSetMatrix(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkMatrix* nativeMatrix);

    static CARAPI_(Boolean) NativeClipRect(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ RegionOp op);

    static CARAPI_(Boolean) NativeClipPath(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkPath* nativePath,
        /* [in] */ RegionOp op);

    static CARAPI_(Boolean) NativeClipRegion(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkRegion* nativeRegion,
        /* [in] */ RegionOp op);

    static CARAPI_(void) NativeSetDrawFilter(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkDrawFilter* nativeFilter);

    static CARAPI_(Boolean) NativeGetClipBounds(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ IRect* bounds);

    static CARAPI_(void) NativeGetCTM(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkMatrix* nativeMatrix);

    static CARAPI_(Boolean) NativeQuickReject(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ IRectF* rect,
        /* [in] */ CanvasEdgeType edgeType);

    static CARAPI_(Boolean) NativeQuickReject(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkPath* nativePath,
        /* [in] */ CanvasEdgeType edgeType);

    static CARAPI_(Boolean) NativeQuickReject(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ CanvasEdgeType edgeType);

    static CARAPI_(void) NativeDrawRGB(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    static CARAPI_(void) NativeDrawARGB(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    static CARAPI_(void) NativeDrawColor(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Int32 color);

    static CARAPI_(void) NativeDrawColor(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Int32 color,
        /* [in] */ SkPorterDuff::Mode mode);

    static CARAPI_(void) NativeDrawPaint(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawLine(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Float startX,
        /* [in] */ Float startY,
        /* [in] */ Float stopX,
        /* [in] */ Float stopY,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawRect(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ IRectF* rect,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawRect(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawOval(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ IRectF* oval,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawCircle(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Float radius,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawArc(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle,
        /* [in] */ Boolean useCenter,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawRoundRect(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ IRectF* rect,
        /* [in] */ Float rx,
        /* [in] */ Float ry,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawPath(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkPath* nativePath,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawBitmap(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ SkPaint* nativePaint,
        /* [in] */ Int32 canvasDensity,
        /* [in] */ Int32 screenDensity,
        /* [in] */ Int32 bitmapDensity);

    static CARAPI_(void) NativeDrawBitmap(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRectF* dst,
        /* [in] */ SkPaint* nativePaint,
        /* [in] */ Int32 screenDensity,
        /* [in] */ Int32 bitmapDensity);

    static CARAPI_(void) NativeDrawBitmap(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRect* dst,
        /* [in] */ SkPaint* nativePaint,
        /* [in] */ Int32 screenDensity,
        /* [in] */ Int32 bitmapDensity);

    static CARAPI_(void) NativeDrawBitmap(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean hasAlpha,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawBitmapMatrix(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ SkMatrix* nativeMatrix,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawBitmapMesh(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ Int32 meshWidth,
        /* [in] */ Int32 meshHeight,
        /* [in] */ const ArrayOf<Float>& verts,
        /* [in] */ Int32 vertIndex,
        /* [in] */ ArrayOf<Int32>* colors,
        /* [in] */ Int32 colorIndex,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawVertices(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkCanvas::VertexMode mode,
        /* [in] */ Int32 vertexCount,
        /* [in] */ const ArrayOf<Float>& verts,
        /* [in] */ Int32 vertIndex,
        /* [in] */ ArrayOf<Float>* texs,
        /* [in] */ Int32 texIndex,
        /* [in] */ ArrayOf<Int32>* colors,
        /* [in] */ Int32 colorIndex,
        /* [in] */ ArrayOf<Int16>* indices,
        /* [in] */ Int32 indexIndex,
        /* [in] */ Int32 indexCount,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawText(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawText(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawPosText(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawPosText(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ String text,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawTextOnPath(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ SkPath* nativePath,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawTextOnPath(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ String text,
        /* [in] */ SkPath* nativePath,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ SkPaint* nativePaint);

    static CARAPI_(void) NativeDrawPicture(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkPicture* nativePicture);

    static CARAPI_(void) Finalizer(
        /* [in] */ SkCanvas* nativeCanvas);

    static CARAPI DoPoints(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ IPaint* paint,
        /* [in] */ SkCanvas::PointMode mode);

    static CARAPI_(void) DoDrawBitmap(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkBitmap* nativeBitmap,
        /* [in] */ IRect* srcIRect,
        /* [in] */ const SkRect& dst,
        /* [in] */ SkPaint* paint,
        /* [in] */ Int32 screenDensity,
        /* [in] */ Int32 bitmapDensity);

public:
    Int32 mDensity;
    SkCanvas* mNativeCanvas;

private:
    Int32 mSurfaceFormat;
    AutoPtr<CBitmap> mBitmap;
    AutoPtr<IGL> mGL;

    // optional field set by the caller
    AutoPtr<IDrawFilter> mDrawFilter;

    Int32 mScreenDensity;
};

#endif // __HH_CANVAS_H

