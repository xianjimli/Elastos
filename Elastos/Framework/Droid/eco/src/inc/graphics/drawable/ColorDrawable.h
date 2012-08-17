
#ifndef __COLORDRAWABLE_H__
#define __COLORDRAWABLE_H__

#include "graphics/drawable/Drawable.h"
#include <elastos/ElRefBase.h>

class ColorDrawable : public Drawable
{
protected:
    class ColorState : public ElRefBase, public IColorState
    {
        friend class ColorDrawable;
    public:
        ColorState(
            /* [in] */ IColorState* state);

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

    protected:
        Int32 mBaseColor; // initial color. never changes
        Int32 mUseColor;  // basecolor modulated by setAlpha()
        Int32 mChangingConfigurations;
    };

public:
    /**
     * Creates a new black ColorDrawable.
     */
    ColorDrawable();

    /**
     * Creates a new ColorDrawable with the specified color.
     *
     * @param color The color to draw.
     */
    ColorDrawable(
        /* [in] */ Int32 color);

    ColorDrawable(
        /* [in] */ IColorState* state);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    /**
     * Returns the alpha value of this drawable's color.
     *
     * @return A value between 0 and 255.
     */
    virtual CARAPI_(Int32) GetAlpha();

    /**
     * Sets the color's alpha value.
     *
     * @param alpha The alpha value to set, between 0 and 255.
     */
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    /**
     * Setting a color filter on a ColorDrawable has no effect.
     *
     * @param colorFilter Ignore.
     */
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* colorFilter);

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

protected:
    CARAPI Init(
        /* [in] */ Int32 color);

    CARAPI InitEx(
        /* [in] */ IColorState* state);

private:
    AutoPtr<ColorState> mState;
};

#endif //__COLORDRAWABLE_H__

