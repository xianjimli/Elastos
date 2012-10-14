
#ifndef __GRADIENTDRAWABLE_H__
#define __GRADIENTDRAWABLE_H__

#include "graphics/drawable/Drawable.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


/**
 * A Drawable with a color gradient for buttons, backgrounds, etc.
 *
 * <p>It can be defined in an XML file with the <code>&lt;shape></code> element. For more
 * information, see the guide to <a
 * href="{@docRoot}guide/topics/resources/drawable-resource.html">Drawable Resources</a>.</p>
 *
 * @attr ref android.R.styleable#GradientDrawable_visible
 * @attr ref android.R.styleable#GradientDrawable_shape
 * @attr ref android.R.styleable#GradientDrawable_innerRadiusRatio
 * @attr ref android.R.styleable#GradientDrawable_innerRadius
 * @attr ref android.R.styleable#GradientDrawable_thicknessRatio
 * @attr ref android.R.styleable#GradientDrawable_thickness
 * @attr ref android.R.styleable#GradientDrawable_useLevel
 * @attr ref android.R.styleable#GradientDrawableSize_width
 * @attr ref android.R.styleable#GradientDrawableSize_height
 * @attr ref android.R.styleable#GradientDrawableGradient_startColor
 * @attr ref android.R.styleable#GradientDrawableGradient_centerColor
 * @attr ref android.R.styleable#GradientDrawableGradient_endColor
 * @attr ref android.R.styleable#GradientDrawableGradient_useLevel
 * @attr ref android.R.styleable#GradientDrawableGradient_angle
 * @attr ref android.R.styleable#GradientDrawableGradient_type
 * @attr ref android.R.styleable#GradientDrawableGradient_centerX
 * @attr ref android.R.styleable#GradientDrawableGradient_centerY
 * @attr ref android.R.styleable#GradientDrawableGradient_gradientRadius
 * @attr ref android.R.styleable#GradientDrawableSolid_color
 * @attr ref android.R.styleable#GradientDrawableStroke_width
 * @attr ref android.R.styleable#GradientDrawableStroke_color
 * @attr ref android.R.styleable#GradientDrawableStroke_dashWidth
 * @attr ref android.R.styleable#GradientDrawableStroke_dashGap
 * @attr ref android.R.styleable#GradientDrawablePadding_left
 * @attr ref android.R.styleable#GradientDrawablePadding_top
 * @attr ref android.R.styleable#GradientDrawablePadding_right
 * @attr ref android.R.styleable#GradientDrawablePadding_bottom
 */
class GradientDrawable : public Drawable
{
protected:
    class GradientState : public ElRefBase, public IGradientState
    {
        friend class GradientDrawable;

    public:
        GradientState();

        GradientState(
            /* [in] */ GradientDrawableOrientation orientation,
            /* [in] */ ArrayOf<Int32>* colors);

        GradientState(
            /* [in] */ GradientState* state);

        ~GradientState();

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

        CARAPI SetShape(
            /* [in] */ Int32 shape);

        CARAPI SetGradientType(
            /* [in] */ Int32 gradient);

        CARAPI SetGradientCenter(
            /* [in] */ Float x,
            /* [in] */ Float y);

        CARAPI SetSolidColor(
            /* [in] */ Int32 argb);

        CARAPI SetStroke(
            /* [in] */ Int32 width,
            /* [in] */ Int32 color);

        CARAPI SetStrokeEx(
            /* [in] */ Int32 width,
            /* [in] */ Int32 color,
            /* [in] */ Float dashWidth,
            /* [in] */ Float dashGap);

        CARAPI SetCornerRadius(
            /* [in] */ Float radius);

        CARAPI SetCornerRadii(
            /* [in] */ ArrayOf<Float>* radii);

        CARAPI SetSize(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SetGradientRadius(
            /* [in] */ Float gradientRadius);

    public:
        Int32 mChangingConfigurations;
        Int32 mShape;
        Int32 mGradient;
        GradientDrawableOrientation mOrientation;
        ArrayOf<Int32>* mColors;
        ArrayOf<Int32>* mTempColors; // no need to copy
        ArrayOf<Float>* mTempPositions; // no need to copy
        ArrayOf<Float>* mPositions;
        Boolean mHasSolidColor;
        Int32 mSolidColor;
        Int32 mStrokeWidth;   // if >= 0 use stroking.
        Int32 mStrokeColor;
        Float mStrokeDashWidth;
        Float mStrokeDashGap;
        Float mRadius;    // use this if mRadiusArray is null
        ArrayOf<Float>* mRadiusArray;
        AutoPtr<CRect> mPadding;
        Int32 mWidth;
        Int32 mHeight;
        Float mInnerRadiusRatio;
        Float mThicknessRatio;
        Int32 mInnerRadius;
        Int32 mThickness;

    private:
        Float mCenterX;
        Float mCenterY;
        Float mGradientRadius;
        Boolean mUseLevel;
        Boolean mUseLevelForShape;
    };

public:
    GradientDrawable();

    /**
     * Create a new gradient drawable given an orientation and an array
     * of colors for the gradient.
     */
    GradientDrawable(
        /* [in] */ GradientDrawableOrientation orientation,
        /* [in] */ ArrayOf<Int32>* colors);

    //@Override
    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    /**
     * Specify radii for each of the 4 corners. For each corner, the array
     * contains 2 values, [X_radius, Y_radius]. The corners are ordered
     * top-left, top-right, bottom-right, bottom-left
     */
    virtual CARAPI SetCornerRadii(
        /* [in] */ ArrayOf<Float>* radii);

    /**
     * Specify radius for the corners of the gradient. If this is > 0, then the
     * drawable is drawn in a round-rectangle, rather than a rectangle.
     */
    virtual CARAPI SetCornerRadius(
        /* [in] */ Float radius);

    /**
     * Set the stroke width and color for the drawable. If width is zero,
     * then no stroke is drawn.
     */
    virtual CARAPI SetStroke(
        /* [in] */ Int32 width,
        /* [in] */ Int32 color);

    virtual CARAPI SetStroke(
        /* [in] */ Int32 width,
        /* [in] */ Int32 color,
        /* [in] */ Float dashWidth,
        /* [in] */ Float dashGap);

    virtual CARAPI SetSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI SetShape(
        /* [in] */ Int32 shape);

    virtual CARAPI SetGradientType(
        /* [in] */ Int32 gradient);

    virtual CARAPI SetGradientCenter(
        /* [in] */ Float x,
        /* [in] */ Float y);

    virtual CARAPI SetGradientRadius(
        /* [in] */ Float gradientRadius);

    virtual CARAPI SetUseLevel(
        /* [in] */ Boolean useLevel);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI SetColor(
        /* [in] */ Int32 argb);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    //@Override
    CARAPI_(Int32) GetOpacity();

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
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* r);

    //@Override
    CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

    CARAPI Init();

    /**
     * Create a new gradient drawable given an orientation and an array
     * of colors for the gradient.
     */
    CARAPI Init(
        /* [in] */ GradientDrawableOrientation orientation,
        /* [in] */ ArrayOf<Int32>* colors);

    CARAPI Init(
        /* [in] */ GradientState* state);

private:
    GradientDrawable(
        /* [in] */ GradientState* state);

    CARAPI_(Int32) ModulateAlpha(
        /* [in] */ Int32 alpha);

    CARAPI_(AutoPtr<IPath>) BuildRing(
        /* [in] */ GradientState* st);

    /**
     * This checks mRectIsDirty, and if it is true, recomputes both our drawing
     * rectangle (mRect) and the gradient itself, since it depends on our
     * rectangle too.
     * @return true if the resulting rectangle is not empty, false otherwise
     */
    CARAPI_(Boolean) EnsureValidRect();

    static CARAPI_(Float) GetFloatOrFraction(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 index,
        /* [in] */ Float defaultValue);

    CARAPI_(void) InitializeWithState(
        /* [in] */ GradientState* state);

private:
    AutoPtr<GradientState> mGradientState;

    AutoPtr<IPaint> mFillPaint;
    AutoPtr<CRect> mPadding;
    AutoPtr<IPaint> mStrokePaint;   // optional, set by the caller
    AutoPtr<IColorFilter> mColorFilter;   // optional, set by the caller
    Int32 mAlpha;  // modified by the caller
    Boolean mDither;

    AutoPtr<IPath> mPath;
    AutoPtr<CRectF> mRect;

    AutoPtr<IPaint> mLayerPaint;    // internal, used if we use saveLayer()
    Boolean mRectIsDirty;   // internal state
    Boolean mMutated;
    AutoPtr<IPath> mRingPath;
    Boolean mPathIsDirty;
};

#endif // __GRADIENTDRAWABLE_H__
