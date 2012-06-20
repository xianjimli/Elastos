
#ifndef __CCANVAS_H__
#define __CCANVAS_H__

#include "_CCanvas.h"
#include "graphics/Canvas.h"

CarClass(CCanvas), public Canvas
{
public:
    // the SAVE_FLAG constants must match their native equivalents

    /** restore the current matrix when restore() is called */
    static const Int32 MATRIX_SAVE_FLAG = 0x01;
    /** restore the current clip when restore() is called */
    static const Int32 CLIP_SAVE_FLAG = 0x02;
    /** the layer needs to per-pixel alpha */
    static const Int32 HAS_ALPHA_LAYER_SAVE_FLAG = 0x04;
    /** the layer needs to 8-bits per color component */
    static const Int32 FULL_COLOR_LAYER_SAVE_FLAG = 0x08;
    /** clip against the layer's bounds */
    static const Int32 CLIP_TO_LAYER_SAVE_FLAG = 0x10;
    /** restore everything when restore() is called */
    static const Int32 ALL_SAVE_FLAG = 0x1F;

public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetGL(
        /* [out] */ IGL** gl);

    CARAPI FreeGlCaches();

    CARAPI SetBitmap(
        /* [in] */ IBitmap* bitmap);

    CARAPI SetViewport(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI GetDensity(
        /* [out] */ Int32* density);

    CARAPI SetDensity(
        /* [in] */ Int32 density);

    CARAPI SetScreenDensity(
        /* [in] */ Int32 density);

    CARAPI Save(
        /* [out] */ Int32* count);

    CARAPI SaveEx(
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    CARAPI SaveLayer(
        /* [in] */ IRectF* bounds,
        /* [in] */ IPaint* paint,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    CARAPI SaveLayerEx(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ IPaint* paint,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    CARAPI SaveLayerAlpha(
        /* [in] */ IRectF* bounds,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    CARAPI SaveLayerAlphaEx(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    CARAPI Restore();

    CARAPI GetSaveCount(
        /* [out] */ Int32* count);

    CARAPI RestoreToCount(
        /* [in] */ Int32 saveCount);

    CARAPI Translate(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI Scale(
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    CARAPI ScaleEx(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);

    CARAPI Rotate(
        /* [in] */ Float degrees);

    CARAPI RotateEx(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);

    CARAPI Skew(
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    CARAPI Concat(
        /* [in] */ IMatrix* matrix);

    CARAPI SetMatrix(
        /* [in] */ IMatrix* matrix);

    CARAPI GetMatrix(
        /* [in] */ IMatrix* ctm);

    CARAPI GetMatrixEx(
        /* [out] */ IMatrix** matrix);

    CARAPI ClipRect(
        /* [in] */ IRectF* rect,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipRectEx(
        /* [in] */ IRect* rect,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipRectEx2(
        /* [in] */ IRectF* rect,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipRectEx3(
        /* [in] */ IRect* rect,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipRectEx4(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipRectEx5(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipRectEx6(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipPath(
        /* [in] */ IPath* path,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipPathEx(
        /* [in] */ IPath* path,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipRegion(
        /* [in] */ IRegion* region,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI ClipRegionEx(
        /* [in] */ IRegion* region,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI GetDrawFilter(
        /* [out] */ IDrawFilter** filter);

    CARAPI SetDrawFilter(
        /* [in] */ IDrawFilter* filter);

    CARAPI QuickRejectWithRectF(
        /* [in] */ IRectF* rect,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    CARAPI QuickRejectWithPath(
        /* [in] */ IPath* path,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    CARAPI QuickReject(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    CARAPI GetClipBounds(
        /* [in] */ IRect* bounds,
        /* [out] */ Boolean* isNotEmpty);

    CARAPI GetClipBoundsEx(
        /* [out] */ IRect** bounds);

    CARAPI DrawRGB(
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    CARAPI DrawARGB(
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    CARAPI DrawColor(
        /* [in] */ Int32 color);

    CARAPI DrawColorEx(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode);

    CARAPI DrawPaint(
        /* [in] */ IPaint* paint);

    CARAPI DrawPoints(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ IPaint* paint);

    CARAPI DrawPointsEx(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ IPaint* paint);

    CARAPI DrawPoint(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    CARAPI DrawLine(
        /* [in] */ Float startX,
        /* [in] */ Float startY,
        /* [in] */ Float stopX,
        /* [in] */ Float stopY,
        /* [in] */ IPaint* paint);

    CARAPI DrawLines(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ IPaint* paint);

    CARAPI DrawLinesEx(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ IPaint* paint);

    CARAPI DrawRect(
        /* [in] */ IRectF* rect,
        /* [in] */ IPaint* paint);

    CARAPI DrawRectEx(
        /* [in] */ IRect* r,
        /* [in] */ IPaint* paint);

    CARAPI DrawRectEx2(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ IPaint* paint);

    CARAPI DrawOval(
        /* [in] */ IRectF* oval,
        /* [in] */ IPaint* paint);

    CARAPI DrawCircle(
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Float radius,
        /* [in] */ IPaint* paint);

    CARAPI DrawArc(
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle,
        /* [in] */ Boolean useCenter,
        /* [in] */ IPaint* paint);

    CARAPI DrawRoundRect(
        /* [in] */ IRectF* rect,
        /* [in] */ Float rx,
        /* [in] */ Float ry,
        /* [in] */ IPaint* paint);

    CARAPI DrawPath(
        /* [in] */ IPath* path,
        /* [in] */ IPaint* paint);

    CARAPI DrawBitmap(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ IPaint* paint);

    CARAPI DrawBitmapEx(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRectF* dst,
        /* [in] */ IPaint* paint);

    CARAPI DrawBitmapEx2(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRect* dst,
        /* [in] */ IPaint* paint);

    CARAPI DrawBitmapEx3(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean hasAlpha,
        /* [in] */ IPaint* paint);

    CARAPI DrawBitmapEx4(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean hasAlpha,
        /* [in] */ IPaint* paint);

    CARAPI DrawBitmapEx5(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IMatrix* matrix,
        /* [in] */ IPaint* paint);

    CARAPI DrawBitmapMesh(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ Int32 meshWidth,
        /* [in] */ Int32 meshHeight,
        /* [in] */ const ArrayOf<Float>& verts,
        /* [in] */ Int32 vertIndex,
        /* [in] */ ArrayOf<Int32>* colors,
        /* [in] */ Int32 colorIndex,
        /* [in] */ IPaint* paint);

    CARAPI DrawVertices(
        /* [in] */ CanvasVertexMode mode,
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
        /* [in] */ IPaint* paint);

    CARAPI DrawTextInBuffer(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    CARAPI DrawTextInString(
        /* [in] */ String text,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    CARAPI DrawTextInStringEx(
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    CARAPI DrawTextInCharSequence(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    CARAPI DrawPosTextInBuffer(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ IPaint* paint);

    CARAPI DrawPosTextInString(
        /* [in] */ String text,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ IPaint* paint);

    CARAPI DrawTextOnPathInBuffer(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IPath* path,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ IPaint* paint);

    CARAPI DrawTextOnPathInString(
        /* [in] */ String text,
        /* [in] */ IPath* path,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ IPaint * paint);

    CARAPI DrawPicture(
        /* [in] */ IPicture* picture);

    CARAPI DrawPictureEx(
        /* [in] */ IPicture* picture,
        /* [in] */ IRectF* dst);

    CARAPI DrawPictureEx2(
        /* [in] */ IPicture* picture,
        /* [in] */ IRect* dst);

    CARAPI FreeCaches();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap);

    CARAPI constructor(
        /* [in] */ Int32 nativeCanvas);

    CARAPI constructor(
        /* [in] */ IGL* gl);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCANVAS_H__
