
#ifndef __ROUNDRECTSHAPE_H__
#define __ROUNDRECTSHAPE_H__

#include "graphics/drawable/shapes/RectShape.h"
#include "graphics/CRectF.h"
#include <elastos/AutoPtr.h>

/**
 * Creates a rounded-corner rectangle. Optionally, an inset (rounded) rectangle
 * can be included (to make a sort of "O" shape).
 * The rounded rectangle can be drawn to a Canvas with its own draw() method,
 * but more graphical control is available if you instead pass
 * the RoundRectShape to a {@link android.graphics.drawable.ShapeDrawable}.
 */
class RoundRectShape : public RectShape
{
public:
    /**
     * RoundRectShape constructor.
     * Specifies an outer (round)rect and an optional inner (round)rect.
     *
     * @param outerRadii An array of 8 radius values, for the outer roundrect.
     *                   The first two floats are for the
     *                   top-left corner (remaining pairs correspond clockwise).
     *                   For no rounded corners on the outer rectangle,
     *                   pass null.
     * @param inset      A RectF that specifies the distance from the inner
     *                   rect to each side of the outer rect.
     *                   For no inner, pass null.
     * @param innerRadii An array of 8 radius values, for the inner roundrect.
     *                   The first two floats are for the
     *                   top-left corner (remaining pairs correspond clockwise).
     *                   For no rounded corners on the inner rectangle,
     *                   pass null.
     *                   If inset parameter is null, this parameter is ignored.
     */
    RoundRectShape(
        /* [in] */ ArrayOf<Float>* outerRadii,
        /* [in] */ IRectF* inset,
        /* [in] */ ArrayOf<Float>* innerRadii);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

    // @Override
    // public RoundRectShape clone() throws CloneNotSupportedException {
    //     RoundRectShape shape = (RoundRectShape) super.clone();
    //     shape.mOuterRadii = mOuterRadii.clone();
    //     shape.mInnerRadii = mInnerRadii.clone();
    //     shape.mInset = new RectF(mInset);
    //     shape.mInnerRect = new RectF(mInnerRect);
    //     shape.mPath = new Path(mPath);
    //     return shape;
    // }

protected:
    RoundRectShape();

    CARAPI Init(
        /* [in] */ ArrayOf<Float>* outerRadii,
        /* [in] */ IRectF* inset,
        /* [in] */ ArrayOf<Float>* innerRadii);

    //@Override
    CARAPI_(void) OnResize(
        /* [in] */ Float width,
        /* [in] */ Float height);

    CARAPI_(void) Clone(
        /* [in] */ RoundRectShape* other);

private:
    ArrayOf<Float>* mOuterRadii;
    AutoPtr<CRectF> mInset;
    ArrayOf<Float>* mInnerRadii;

    AutoPtr<CRectF> mInnerRect;
    AutoPtr<IPath> mPath;    // this is what we actually draw
};

#endif //__ROUNDRECTSHAPE_H__
