
#ifndef __SHAPEDRAWABLE_H__
#define __SHAPEDRAWABLE_H__

#include "ext/frameworkext.h"
#include "graphics/drawable/Drawable.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

class ShapeDrawable : public Drawable
{
protected:
    /**
     * Defines the intrinsic properties of this ShapeDrawable's Shape.
     */
    class ShapeState : public ElRefBase, public IDrawableConstantState
    {
    public:
        ShapeState(
            /* [in] */ ShapeState* orig);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        //@Override
        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* conf);

    public:
        Int32 mChangingConfigurations;
        AutoPtr<IPaint> mPaint;
        AutoPtr<IShape> mShape;
        AutoPtr<IRect> mPadding;
        Int32 mIntrinsicWidth;
        Int32 mIntrinsicHeight;
        Int32 mAlpha;
        AutoPtr<IShaderFactory> mShaderFactory;
    };



public:
    /**
     * ShapeDrawable constructor.
     */
    ShapeDrawable();

    /**
     * Creates a ShapeDrawable with a specified Shape.
     *
     * @param s the Shape that this ShapeDrawable should be
     */
    ShapeDrawable(
        /* [in] */ IShape* s);

    /**
     * Returns the Shape of this ShapeDrawable.
     */
    virtual CARAPI_(AutoPtr<IShape>) GetShape();

    /**
     * Sets the Shape of this ShapeDrawable.
     */
    virtual CARAPI SetShape(
        /* [in] */ IShape* s);

    /**
     * Sets a ShaderFactory to which requests for a
     * {@link android.graphics.Shader} object will be made.
     *
     * @param fact an instance of your ShaderFactory implementation
     */
    virtual CARAPI SetShaderFactory(
        /* [in] */ IShaderFactory* fact);

    /**
     * Returns the ShaderFactory used by this ShapeDrawable for requesting a
     * {@link android.graphics.Shader}.
     */
    virtual CARAPI_(AutoPtr<IShaderFactory>) GetShaderFactory();

    /**
     * Returns the Paint used to draw the shape.
     */
    virtual CARAPI_(AutoPtr<IPaint>) GetPaint();

    /**
     * Sets padding for the shape.
     *
     * @param left    padding for the left side (in pixels)
     * @param top     padding for the top (in pixels)
     * @param right   padding for the right side (in pixels)
     * @param bottom  padding for the bottom (in pixels)
     */
    virtual CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Sets padding for this shape, defined by a Rect object.
     * Define the padding in the Rect object as: left, top, right, bottom.
     */
    virtual CARAPI SetPadding(
        /* [in] */ IRect* padding);

    /**
     * Sets the intrinsic (default) width for this shape.
     *
     * @param width the intrinsic width (in pixels)
     */
    virtual CARAPI SetIntrinsicWidth(
        /* [in] */ Int32 width);

    /**
     * Sets the intrinsic (default) height for this shape.
     *
     * @param height the intrinsic height (in pixels)
     */
    virtual CARAPI SetIntrinsicHeight(
        /* [in] */ Int32 height);

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    /**
     * Set the alpha level for this drawable [0..255]. Note that this drawable
     * also has a color in its paint, which has an alpha as well. These two
     * values are automatically combined during drawing. Thus if the color's
     * alpha is 75% (i.e. 192) and the drawable's alpha is 50% (i.e. 128), then
     * the combined alpha that will be used during drawing will be 37.5%
     * (i.e. 96).
     */
    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ IShape* s);

    CARAPI Init(
        /* [in] */ ShapeState* state);

    /**
     * Called from the drawable's draw() method after the canvas has been set
     * to draw the shape at (0,0). Subclasses can override for special effects
     * such as multiple layers, stroking, etc.
     */
    CARAPI_(void) OnDraw(
        /* [in] */ IShape* shape,
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    /**
     * Subclasses override this to parse custom subelements.
     * If you handle it, return true, else return <em>super.inflateTag(...)</em>.
     */
    virtual CARAPI_(Boolean) InflateTag(
        /* [in] */ const String& name,
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

private:
    ShapeDrawable(
        /* [in] */ ShapeState* state);

    static CARAPI_(Int32) ModulateAlpha(
        /* [in] */ Int32 paintAlpha,
        /* [in] */ Int32 alpha);

    CARAPI_(void) UpdateShape();

private:
    AutoPtr<ShapeState> mShapeState;
    Boolean mMutated;
};

#endif //__SHAPEDRAWABLE_H__