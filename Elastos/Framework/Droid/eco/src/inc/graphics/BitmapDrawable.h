
#ifndef __HH_BITMAPDRAWABLE_H
#define __HH_BITMAPDRAWABLE_H

#include <elastos.h>
using namespace Elastos;
#include <ext/frameworkext.h>
#include <elastos/AutoPtr.h>
#include "graphics/Drawable.h"
#include <elastos/ElRefBase.h>

class BitmapDrawable : public Drawable
{
public:
    class BitmapState : public ElRefBase, public IBitmapState
    {
    public:
        BitmapState(
            /* [in] */ IBitmap* bitmap);

        BitmapState(
            /* [in] */ IBitmapState* bitmap);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* config);

    public:
        AutoPtr<IBitmap> mBitmap;
        Int32 mChangingConfigurations;
        Int32 mGravity;
        AutoPtr<IPaint> mPaint;
        ShaderTileMode mTileModeX;
        ShaderTileMode mTileModeY;
        Int32 mTargetDensity;
    };

public:
    BitmapDrawable();

    BitmapDrawable(
        /* [in] */ IResources* res);

    BitmapDrawable(
        /* [in] */ IBitmap* bitmap);

    BitmapDrawable(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap);

    BitmapDrawable(
        /* [in] */ const String& filepath);

    BitmapDrawable(
        /* [in] */ IResources* res,
        /* [in] */ const String& filepath);

    BitmapDrawable(
        /* [in] */ IInputStream* is);

    BitmapDrawable(
        /* [in] */ IResources* res,
        /* [in] */ IInputStream* is);

    CARAPI_(AutoPtr<IPaint>) GetPaint();

    CARAPI_(AutoPtr<IBitmap>) GetBitmap();

    /**
     * Set the density scale at which this drawable will be rendered. This
     * method assumes the drawable will be rendered at the same density as the
     * specified canvas.
     *
     * @param canvas The Canvas from which the density scale must be obtained.
     *
     * @see android.graphics.Bitmap#setDensity(Int32)
     * @see android.graphics.Bitmap#getDensity()
     */
    virtual CARAPI SetTargetDensity(
        /* [in] */ ICanvas* canvas);

    /**
     * Set the density scale at which this drawable will be rendered.
     *
     * @param metrics The DisplayMetrics indicating the density scale for this drawable.
     *
     * @see android.graphics.Bitmap#setDensity(Int32)
     * @see android.graphics.Bitmap#getDensity()
     */
    virtual CARAPI SetTargetDensity(
        /* [in] */ IDisplayMetrics* metrics);

    /**
     * Set the density at which this drawable will be rendered.
     *
     * @param density The density scale for this drawable.
     *
     * @see android.graphics.Bitmap#setDensity(Int32)
     * @see android.graphics.Bitmap#getDensity()
     */
    virtual CARAPI SetTargetDensity(
        /* [in] */ Int32 density);

    /** Get the gravity used to position/stretch the bitmap within its bounds.
     * See android.view.Gravity
     * @return the gravity applied to the bitmap
     */
    virtual CARAPI_(Int32) GetGravity();

    /** Set the gravity used to position/stretch the bitmap within its bounds.
     *  See android.view.Gravity
     * @param gravity the gravity
     */
    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    virtual CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    //@Override
    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    //@Override
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    virtual CARAPI_(ShaderTileMode) GetTileModeX();

    virtual CARAPI_(ShaderTileMode) GetTileModeY();

    virtual CARAPI SetTileModeX(
        /* [in] */ ShaderTileMode mode);

    CARAPI SetTileModeY(
        /* [in] */ ShaderTileMode mode);

    virtual CARAPI SetTileModeXY(
        /* [in] */ ShaderTileMode xmode,
        /* [in] */ ShaderTileMode ymode);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

protected:
    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ IResources* res);

    CARAPI Init(
        /* [in] */ IBitmap* bitmap);

    CARAPI Init(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap);

    CARAPI Init(
        /* [in] */ const String& filepath);

    CARAPI Init(
        /* [in] */ IResources* res,
        /* [in] */ const String& filepath);

    CARAPI Init(
        /* [in] */ IInputStream* is);

    CARAPI Init(
        /* [in] */ IResources* res,
        /* [in] */ IInputStream* is);

    CARAPI Init(
        /* [in] */ BitmapState* state,
        /* [in] */ IResources* res);

private:
    CARAPI_(void) SetBitmap(
        /* [in] */ IBitmap* bitmap);

    CARAPI_(void) ComputeBitmapSize();

private:
    static const Int32 DEFAULT_PAINT_FLAGS;

    AutoPtr<BitmapState> mBitmapState;
    AutoPtr<IBitmap>  mBitmap;
    Int32 mTargetDensity;

    AutoPtr<IRect> mDstRect;

    Boolean mApplyGravity;
    Boolean mRebuildShader;
    Boolean mMutated;

    // These are scaled to match the target density.
    Int32 mBitmapWidth;
    Int32 mBitmapHeight;
};

#endif // __HH_BITMAPDRAWABLE_H

