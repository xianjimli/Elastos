
#ifndef __CROUNDRECTSHAPE_H__
#define __CROUNDRECTSHAPE_H__

#include "graphics/drawable/shapes/RoundRectShape.h"
#include "_CRoundRectShape.h"

CarClass(CRoundRectShape), public RoundRectShape
{
public:
    CARAPI GetWidth(
        /* [out] */ Float* width);

    CARAPI GetHeight(
        /* [out] */ Float* height);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

    CARAPI Resize(
        /* [in] */ Float width,
        /* [in] */ Float height);

    CARAPI HasAlpha(
        /* [out] */ Boolean* hasAlpha);

    CARAPI Clone(
        /* [out] */ IShape** shape);

    CARAPI constructor();

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
    CARAPI constructor(
        /* [in] */ ArrayOf<Float>* outerRadii,
        /* [in] */ IRectF* inset,
        /* [in] */ ArrayOf<Float>* innerRadii);
};

#endif //__CROUNDRECTSHAPE_H__
