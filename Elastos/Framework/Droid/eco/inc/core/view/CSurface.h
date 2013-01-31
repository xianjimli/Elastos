
#ifndef __CSURFACE_H__
#define __CSURFACE_H__

#include "_CSurface.h"
#include <elastos/ElRefBase.h>
#include "graphics/Canvas.h"
#include <elastos/AutoPtr.h>
#include <surfaceflinger/Surface.h>

CarClass(CSurface)
{
public:
    class CompatibleCanvas :
        public ElRefBase,
        public Canvas,
        public ICanvas
    {
    public:
        CompatibleCanvas();

        ~CompatibleCanvas();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    public:
        CARAPI GetGL(
            /* [out] */ IGL ** ppGl);

        CARAPI FreeGlCaches();

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
        AutoPtr<IMatrix> mOrigMatrix;
    };

    CSurface();

    ~CSurface();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel * pSource);

    CARAPI WriteToParcel(
        /* [in] */ IParcel * pDest);

    CARAPI SetCompatibleDisplayMetrics(
        /* [in] */ IDisplayMetrics * pMetrics,
        /* [in] */ ITranslator * pTranslator);

    CARAPI CopyFrom(
        /* [in] */ ISurface * pO);

    CARAPI IsValid(
        /* [out] */ Boolean * pIsValid);

    CARAPI Destroy();

    CARAPI ReleaseSurface();

    CARAPI LockCanvas(
        /* [in] */ IRect * pDirty,
        /* [out] */ ICanvas ** ppCanvas);

    CARAPI LockCanvasNative(
        /* [in] */ IRect * pDirty,
        /* [out] */ ICanvas ** ppCanvas);

    CARAPI UnlockCanvasAndPost(
        /* [in] */ ICanvas * pCanvas);

    CARAPI UnlockCanvas(
        /* [in] */ ICanvas * pCanvas);

    CARAPI FreezeDisplay(
        /* [in] */ Int32 display);

    CARAPI UnfreezeDisplay(
        /* [in] */ Int32 display);

    CARAPI SetOrientationEx(
        /* [in] */ Int32 display,
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 flags);

    CARAPI SetOrientation(
        /* [in] */ Int32 display,
        /* [in] */ Int32 orientation);

    CARAPI SetLayer(
        /* [in] */ Int32 zorder);

    CARAPI SetPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI SetSize(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI Hide();

    CARAPI Show();

    CARAPI SetTransparentRegionHint(
        /* [in] */ IRegion * pRegion);

    CARAPI SetAlpha(
        /* [in] */ Float alpha);

    CARAPI SetMatrix(
        /* [in] */ Float dsdx,
        /* [in] */ Float dtdx,
        /* [in] */ Float dsdy,
        /* [in] */ Float dtdy);

    CARAPI Freeze();

    CARAPI Unfreeze();

    CARAPI SetFreezeTint(
        /* [in] */ Int32 tint);

    CARAPI SetFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    CARAPI GetSurface(
        /* [out] */ Handle32* surface);

    CARAPI constructor(
        /* [in] */ ISurfaceSession * pS,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 display,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 format,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ ISurfaceSession * pS,
        /* [in] */ Int32 pid,
        /* [in] */ const String& name,
        /* [in] */ Int32 display,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 format,
        /* [in] */ Int32 flags);

    CARAPI constructor();

public:
    /** start/end a transaction {@hide} */
    static CARAPI_(void) OpenTransaction();

    static CARAPI_(void) CloseTransaction();

private:
    ECode Init(
        /* [in] */ ISurfaceSession * pS,
        /* [in] */ Int32 pid,
        /* [in] */ const char* name,
        /* [in] */ Int32 display,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 format,
        /* [in] */ Int32 flags);

    void SetSurfaceControl(
        /* [in] */ const android::sp<android::SurfaceControl>& surface);

    void GetSurface();

    void SetSurface(
        /* [in] */ const android::sp<android::Surface>& surface);

private:
    String                      mName;
    Int32                       mSaveCount;
    AutoPtr<CompatibleCanvas>   mCanvas;
#ifdef _linux
    android::sp<android::SurfaceControl>    mSurfaceControl;
    android::sp<android::Surface>           mNativeSurface;
#endif
};

#endif // __CSURFACE_H__

