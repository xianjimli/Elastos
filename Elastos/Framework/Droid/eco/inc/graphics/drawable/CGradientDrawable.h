
#ifndef __CGRADIENTDRAWABLE_H__
#define __CGRADIENTDRAWABLE_H__

#include "_CGradientDrawable.h"
#include "graphics/drawable/GradientDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

CarClass(CGradientDrawable), public  GradientDrawable
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL();

    /**
     * Specify radii for each of the 4 corners. For each corner, the array
     * contains 2 values, [X_radius, Y_radius]. The corners are ordered
     * top-left, top-right, bottom-right, bottom-left
     */
    CARAPI SetCornerRadii(
        /* [in] */ ArrayOf<Float>* radii);

    /**
     * Specify radius for the corners of the gradient. If this is > 0, then the
     * drawable is drawn in a round-rectangle, rather than a rectangle.
     */
    CARAPI SetCornerRadius(
        /* [in] */ Float radius);

    /**
     * Set the stroke width and color for the drawable. If width is zero,
     * then no stroke is drawn.
     */
    CARAPI SetStroke(
        /* [in] */ Int32 width,
        /* [in] */ Int32 color);

    CARAPI SetStrokeEx(
        /* [in] */ Int32 width,
        /* [in] */ Int32 color,
        /* [in] */ Float dashWidth,
        /* [in] */ Float dashGap);

    CARAPI SetSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetShape(
        /* [in] */ Int32 shape);

    CARAPI SetGradientType(
        /* [in] */ Int32 gradient);

    CARAPI SetGradientCenter(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI SetGradientRadius(
        /* [in] */ Float gradientRadius);

    CARAPI SetUseLevel(
        /* [in] */ Boolean useLevel);

    CARAPI SetColor(
        /* [in] */ Int32 argb);

    CARAPI constructor();

    /**
     * Create a new gradient drawable given an orientation and an array
     * of colors for the gradient.
     */
    CARAPI constructor(
        /* [in] */ GradientDrawableOrientation orientation,
        /* [in] */ ArrayOf<Int32>* colors);

    CARAPI constructor(
        /* [in] */ IGradientState* state);
};

#endif //__CGRADIENTDRAWABLE_H__
