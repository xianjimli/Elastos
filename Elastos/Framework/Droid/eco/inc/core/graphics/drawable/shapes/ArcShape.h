
#ifndef __ARCSHAPE_H__
#define __ARCSHAPE_H__

#include "graphics/drawable/shapes/RectShape.h"

/**
 * Creates an arc shape. The arc shape starts at a specified
 * angle and sweeps clockwise, drawing slices of pie.
 * The arc can be drawn to a Canvas with its own draw() method,
 * but more graphical control is available if you instead pass
 * the ArcShape to a {@link android.graphics.drawable.ShapeDrawable}.
 */
class ArcShape : public RectShape
{
public:
    /**
     * ArcShape constructor.
     *
     * @param startAngle the angle (in degrees) where the arc begins
     * @param sweepAngle the sweep angle (in degrees). Anything equal to or
     *                   greater than 360 results in a complete circle/oval.
     */
    ArcShape(
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

protected:
    ArcShape();

    CARAPI Init(
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle);

    CARAPI_(void) Clone(
        /* [in] */ ArcShape* other);

private:
    Float mStart;
    Float mSweep;
};

#endif //__ARCSHAPE_H__
