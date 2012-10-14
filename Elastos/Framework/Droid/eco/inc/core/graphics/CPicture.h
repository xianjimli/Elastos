
#ifndef __CPICTURE_H__
#define __CPICTURE_H__

#include "_CPicture.h"
#include "graphics/Canvas.h"
#include <SkPicture.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

CarClass(CPicture)
{
private:
    class RecordingCanvas
        : public ElRefBase
        , public Canvas
        , public ICanvas
    {
    public:
        RecordingCanvas(
            /* [in] */ CPicture* pict,
            /* [in] */ SkCanvas* nativeCanvas);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetGL(
            /* [out] */ IGL ** ppGl);

        CARAPI FreeGlCaches();

        //@Override
        CARAPI SetBitmap(
            /* [in] */ IBitmap * pBitmap);

        CARAPI SetViewport(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI IsOpaque(
            /* [out] */ Boolean * pIsOpaque);

        CARAPI GetWidth(
            /* [out] */ Int32 * pWidth);

        CARAPI GetHeight(
            /* [out] */ Int32 * pHeight);

        CARAPI GetDensity(
            /* [out] */ Int32 * pDensity);

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
            /* [in] */ IRectF * pBounds,
            /* [in] */ IPaint * pPaint,
            /* [in] */ Int32 saveFlags,
            /* [out] */ Int32 * pCount);

        CARAPI SaveLayerEx(
            /* [in] */ Float left,
            /* [in] */ Float top,
            /* [in] */ Float right,
            /* [in] */ Float bottom,
            /* [in] */ IPaint* paint,
            /* [in] */ Int32 saveFlags,
            /* [out] */ Int32* count);

        CARAPI SaveLayerAlpha(
            /* [in] */ IRectF * pBounds,
            /* [in] */ Int32 alpha,
            /* [in] */ Int32 saveFlags,
            /* [out] */ Int32 * pCount);

        CARAPI SaveLayerAlphaEx(
            /* [in] */ Float left,
            /* [in] */ Float top,
            /* [in] */ Float right,
            /* [in] */ Float bottom,
            /* [in] */ Int32 alpha,
            /* [in] */ Int32 saveFlags,
            /* [out] */ Int32 * pCount);

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
            /* [in] */ IMatrix * pMatrix);

        CARAPI SetMatrix(
            /* [in] */ IMatrix * pMatrix);

        CARAPI GetMatrix(
            /* [in] */ IMatrix * pCtm);

        CARAPI GetMatrixEx(
            /* [out] */ IMatrix ** ppMetrix);

        CARAPI ClipRect(
            /* [in] */ IRectF * pRect,
            /* [in] */ RegionOp op,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipRectEx(
            /* [in] */ IRect * pRect,
            /* [in] */ RegionOp op,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipRectEx2(
            /* [in] */ IRectF * pRect,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipRectEx3(
            /* [in] */ IRect * pRect,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipRectEx4(
            /* [in] */ Float left,
            /* [in] */ Float top,
            /* [in] */ Float right,
            /* [in] */ Float bottom,
            /* [in] */ RegionOp op,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipRectEx5(
            /* [in] */ Float left,
            /* [in] */ Float top,
            /* [in] */ Float right,
            /* [in] */ Float bottom,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipRectEx6(
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipPath(
            /* [in] */ IPath * pPath,
            /* [in] */ RegionOp op,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipPathEx(
            /* [in] */ IPath * pPath,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipRegion(
            /* [in] */ IRegion * pRegion,
            /* [in] */ RegionOp op,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI ClipRegionEx(
            /* [in] */ IRegion * pRegion,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI GetDrawFilter(
            /* [out] */ IDrawFilter ** ppFilter);

        CARAPI SetDrawFilter(
            /* [in] */ IDrawFilter * pFilter);

        CARAPI QuickRejectWithRectF(
            /* [in] */ IRectF * pRect,
            /* [in] */ CanvasEdgeType type,
            /* [out] */ Boolean * pIsNotIntersect);

        CARAPI QuickRejectWithPath(
            /* [in] */ IPath * pPath,
            /* [in] */ CanvasEdgeType type,
            /* [out] */ Boolean * pIsNotIntersect);

        CARAPI QuickReject(
            /* [in] */ Float left,
            /* [in] */ Float top,
            /* [in] */ Float right,
            /* [in] */ Float bottom,
            /* [in] */ CanvasEdgeType type,
            /* [out] */ Boolean * pIsNotIntersect);

        CARAPI GetClipBounds(
            /* [in] */ IRect * pBounds,
            /* [out] */ Boolean * pIsNotEmpty);

        CARAPI GetClipBoundsEx(
            /* [out] */ IRect ** ppBounds);

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
            /* [in] */ IPaint * pPaint);

        CARAPI DrawPoints(
            /* [in] */ const ArrayOf<Float> & pts,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 count,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawPointsEx(
            /* [in] */ const ArrayOf<Float> & pts,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawPoint(
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawLine(
            /* [in] */ Float startX,
            /* [in] */ Float startY,
            /* [in] */ Float stopX,
            /* [in] */ Float stopY,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawLines(
            /* [in] */ const ArrayOf<Float> & pts,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 count,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawLinesEx(
            /* [in] */ const ArrayOf<Float> & pts,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawRect(
            /* [in] */ IRectF * pRect,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawRectEx(
            /* [in] */ IRect * pR,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawRectEx2(
            /* [in] */ Float left,
            /* [in] */ Float top,
            /* [in] */ Float right,
            /* [in] */ Float bottom,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawOval(
            /* [in] */ IRectF * pOval,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawCircle(
            /* [in] */ Float cx,
            /* [in] */ Float cy,
            /* [in] */ Float radius,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawArc(
            /* [in] */ IRectF * pOval,
            /* [in] */ Float startAngle,
            /* [in] */ Float sweepAngle,
            /* [in] */ Boolean useCenter,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawRoundRect(
            /* [in] */ IRectF * pRect,
            /* [in] */ Float rx,
            /* [in] */ Float ry,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawPath(
            /* [in] */ IPath * pPath,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawBitmap(
            /* [in] */ IBitmap * pBitmap,
            /* [in] */ Float left,
            /* [in] */ Float top,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawBitmapEx(
            /* [in] */ IBitmap * pBitmap,
            /* [in] */ IRect * pSrc,
            /* [in] */ IRectF * pDst,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawBitmapEx2(
            /* [in] */ IBitmap * pBitmap,
            /* [in] */ IRect * pSrc,
            /* [in] */ IRect * pDst,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawBitmapEx3(
            /* [in] */ const ArrayOf<Int32> & colors,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 stride,
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Boolean hasAlpha,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawBitmapEx4(
            /* [in] */ const ArrayOf<Int32> & colors,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 stride,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Boolean hasAlpha,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawBitmapEx5(
            /* [in] */ IBitmap * pBitmap,
            /* [in] */ IMatrix * pMatrix,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawBitmapMesh(
            /* [in] */ IBitmap * pBitmap,
            /* [in] */ Int32 meshWidth,
            /* [in] */ Int32 meshHeight,
            /* [in] */ const ArrayOf<Float> & verts,
            /* [in] */ Int32 vertOffset,
            /* [in] */ ArrayOf<Int32>* colors,
            /* [in] */ Int32 colorOffset,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawVertices(
            /* [in] */ CanvasVertexMode mode,
            /* [in] */ Int32 vertexCount,
            /* [in] */ const ArrayOf<Float> & verts,
            /* [in] */ Int32 vertOffset,
            /* [in] */ ArrayOf<Float>* texs,
            /* [in] */ Int32 texOffset,
            /* [in] */ ArrayOf<Int32>* colors,
            /* [in] */ Int32 colorOffset,
            /* [in] */ ArrayOf<Int16>* indices,
            /* [in] */ Int32 indexOffset,
            /* [in] */ Int32 indexCount,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawTextInBuffer(
            /* [in] */ const ArrayOf<Char8> & text,
            /* [in] */ Int32 index,
            /* [in] */ Int32 count,
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawTextInString(
            /* [in] */ const String& text,
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawTextInStringEx(
            /* [in] */ const String& text,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawTextInCharSequence(
            /* [in] */ ICharSequence* text,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawPosTextInBuffer(
            /* [in] */ const ArrayOf<Char8> & text,
            /* [in] */ Int32 index,
            /* [in] */ Int32 count,
            /* [in] */ const ArrayOf<Float> & pos,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawPosTextInString(
            /* [in] */ const String& text,
            /* [in] */ const ArrayOf<Float> & pos,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawTextOnPathInBuffer(
            /* [in] */ const ArrayOf<Char8> & text,
            /* [in] */ Int32 index,
            /* [in] */ Int32 count,
            /* [in] */ IPath * pPath,
            /* [in] */ Float hOffset,
            /* [in] */ Float vOffset,
            /* [in] */ IPaint * pPaint);

        CARAPI DrawTextOnPathInString(
            /* [in] */ const String& text,
            /* [in] */ IPath * pPath,
            /* [in] */ Float hOffset,
            /* [in] */ Float vOffset,
            /* [in] */ IPaint * pPaint);

        //@Override
        CARAPI DrawPicture(
            /* [in] */ IPicture * pPicture);

        CARAPI DrawPictureEx(
            /* [in] */ IPicture * pPicture,
            /* [in] */ IRectF * pDst);

        CARAPI DrawPictureEx2(
            /* [in] */ IPicture * pPicture,
            /* [in] */ IRect * pDst);

        CARAPI FreeCaches();

    private:
        //Don't use AutoPtr here to avoid cycle reference with CPicture;
        CPicture* mPicture;
    };

public:
    CPicture();

    ~CPicture();

    /**
     * To record a picture, call beginRecording() and then draw into the Canvas
     * that is returned. Nothing we appear on screen, but all of the draw
     * commands (e.g. drawRect(...)) will be recorded. To stop recording, call
     * endRecording(). At this point the Canvas that was returned must no longer
     * be referenced, and nothing should be drawn into it.
     */
    CARAPI BeginRecording(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ ICanvas** canvas);

    /**
     * Call endRecording when the picture is built. After this call, the picture
     * may be drawn, but the canvas that was returned by beginRecording must not
     * be referenced anymore. This is automatically called if Picture.draw() or
     * Canvas.drawPicture() is called.
     */
    CARAPI EndRecording();

    /**
     * Get the width of the picture as passed to beginRecording. This
     * does not reflect (per se) the content of the picture.
     */
    CARAPI GetWidth(
        /* [out] */ Int32* width);

    /**
     * Get the height of the picture as passed to beginRecording. This
     * does not reflect (per se) the content of the picture.
     */
    CARAPI GetHeight(
        /* [out] */ Int32* height);

    /**
     * Draw this picture on the canvas. The picture may have the side effect
     * of changing the matrix and clip of the canvas.
     *
     * @param canvas  The picture is drawn to this canvas
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    /**
     * Create a new picture (already recorded) from the data in the stream. This
     * data was generated by a previous call to writeToStream().
     */
    static CARAPI_(AutoPtr<IPicture>) CreateFromStream(
        /* [in] */ IInputStream* stream);

    /**
     * Write the picture contents to a stream. The data can be used to recreate
     * the picture in this or another process by calling createFromStream.
     */
    CARAPI WriteToStream(
        /* [in] */ IOutputStream* stream);

    /*package*/ CARAPI_(SkPicture*) Ni();

    CARAPI constructor();

    /**
     * Create a picture by making a copy of what has already been recorded in
     * src. The contents of src are unchanged, and if src changes later, those
     * changes will not be reflected in this picture.
     */
    CARAPI constructor(
        /* [in] */ IPicture* src);

    CARAPI constructor(
        /* [in] */ Handle32 nativePicture);

private:
    // return empty picture if src is 0, or a copy of the native src
    static CARAPI_(SkPicture*) NativeConstructor(
        /* [in] */ SkPicture* nativeSrcOr0);

    static CARAPI_(SkPicture*) NativeCreateFromStream(
        /* [in] */ IInputStream* stream,
        /* [in] */ ArrayOf<Byte>* storage);

    static CARAPI_(SkCanvas*) NativeBeginRecording(
        /* [in] */ SkPicture* nativePicture,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    static CARAPI_(void) NativeEndRecording(
        /* [in] */ SkPicture* nativePicture);

    static CARAPI_(void) NativeDraw(
        /* [in] */ SkCanvas* nativeCanvas,
        /* [in] */ SkPicture* nativePicture);

    static CARAPI_(Boolean) NativeWriteToStream(
        /* [in] */ SkPicture* nativePicture,
        /* [in] */ IOutputStream* stream,
        /* [in] */ ArrayOf<Byte>* storage);

    static CARAPI_(void) NativeDestructor(
        /* [in] */ SkPicture* nativePicture);

private:
    AutoPtr<ICanvas> mRecordingCanvas;
    SkPicture* mNativePicture;

    static const Int32 WORKING_STREAM_STORAGE = 16 * 1024;
};

#endif //__CPICTURE_H__
