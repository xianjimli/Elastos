
#ifndef __HH_NINEPATCHDRAWABLE_H
#define __HH_NINEPATCHDRAWABLE_H

#include "graphics/Drawable.h"
#include <elastos/ElRefBase.h>

class NinePatchDrawable : public Drawable
{
public:
    class NinePatchState : public ElRefBase, public IDrawableConstantState
    {
    public:
        NinePatchState(
            /* [in] */ INinePatch* ninePatch,
            /* [in] */ IRect* padding,
            /* [in] */ Boolean dither = DEFAULT_DITHER);

        NinePatchState(
            /* [in] */ NinePatchState* state);

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
        AutoPtr<INinePatch> mNinePatch;
        AutoPtr<IRect> mPadding;
        Boolean mDither;
        Int32 mChangingConfigurations;
        Int32 mTargetDensity;
    };

public:
    NinePatchDrawable();

    NinePatchDrawable(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte> & chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    NinePatchDrawable(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte> & chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    NinePatchDrawable(
        /* [in] */ INinePatch* patch);

    NinePatchDrawable(
        /* [in] */ IResources* res,
        /* [in] */ INinePatch* patch);

    NinePatchDrawable(
        /* [in] */ NinePatchState* state,
        /* [in] */ IResources* res);

    virtual CARAPI SetTargetDensity(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI SetTargetDensity(
        /* [in] */ IDisplayMetrics* metrics);

    virtual CARAPI SetTargetDensity(
        /* [in] */ Int32 density);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    //@Override
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    //@Override
    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(AutoPtr<IPaint>) GetPaint();

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(Int32) GetMinimumWidth();

    //@Override
    CARAPI_(Int32) GetMinimumHeight();

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI_(AutoPtr<IRegion>) GetTransparentRegion();

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI Init(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte> & chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    CARAPI Init(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte> & chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    CARAPI Init(
        /* [in] */ INinePatch* patch);

    CARAPI Init(
        /* [in] */ IResources* res,
        /* [in] */ INinePatch* patch);

    CARAPI Init(
        /* [in] */ NinePatchState* state,
        /* [in] */ IResources* res);

private:
    CARAPI_(void) SetNinePatchState(
        /* [in] */ NinePatchState* state,
        /* [in] */ IResources* res);

    CARAPI ComputeBitmapSize();

private:
    // dithering helps a lot, and is pretty cheap, so default is true
    static const Boolean DEFAULT_DITHER = TRUE;
    AutoPtr<NinePatchState> mNinePatchState;
    AutoPtr<INinePatch> mNinePatch;
    AutoPtr<IRect> mPadding;
    AutoPtr<IPaint> mPaint;
    Boolean mMutated;

    Int32 mTargetDensity;

    // These are scaled to match the target density.
    Int32 mBitmapWidth;
    Int32 mBitmapHeight;

    friend class NinePatchState;
};

#endif // __HH_NINEPATCHDRAWABLE_H

