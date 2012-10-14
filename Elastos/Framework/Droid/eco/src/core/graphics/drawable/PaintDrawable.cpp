
#include "graphics/drawable/PaintDrawable.h"
#include "graphics/drawable/shapes/CRoundRectShape.h"


PaintDrawable::PaintDrawable()
{}

PaintDrawable::PaintDrawable(
    /* [in] */ Int32 color)
{
    GetPaint()->SetColor(color);
}

/**
 * Specify radius for the corners of the rectangle. If this is > 0, then the
 * drawable is drawn in a round-rectangle, rather than a rectangle.
 * @param radius the radius for the corners of the rectangle
 */
ECode PaintDrawable::SetCornerRadius(
    /* [in] */ Float radius)
{
    ArrayOf<Float>* radii = NULL;
    if (radius > 0) {
        radii = ArrayOf<Float>::Alloc(8);
        for (Int32 i = 0; i < 8; i++) {
            (*radii)[i] = radius;
        }
    }
    SetCornerRadii(radii);
    ArrayOf<Float>::Free(radii);
    return NOERROR;
}

/**
 * Specify radii for each of the 4 corners. For each corner, the array
 * contains 2 values, [X_radius, Y_radius]. The corners are ordered
 * top-left, top-right, bottom-right, bottom-left
 * @param radii the x and y radii of the corners
 */
ECode PaintDrawable::SetCornerRadii(
    /* [in] */ ArrayOf<Float>* radii)
{
    if (radii == NULL) {
        if (GetShape() != NULL) {
            SetShape(NULL);
        }
    }
    else {
        AutoPtr<IShape> shape;
        CRoundRectShape::New(radii, NULL, NULL, (IShape**)&shape);
        SetShape(shape);
    }

    return NOERROR;
}

static Int32 R_Styleable_DrawableCorners[] = {
    0x010101a8, 0x010101a9, 0x010101aa, 0x010101ab,
    0x010101ac
};

//@Override
Boolean PaintDrawable::InflateTag(
    /* [in] */ const String& name,
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    if (name.Equals("corners")) {
        AutoPtr<ITypedArray> a;
        r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_DrawableCorners,
            sizeof(R_Styleable_DrawableCorners) / sizeof(Int32)),/*com.android.internal.R.styleable.DrawableCorners*/
            (ITypedArray**)&a);
        Int32 radius;
        a->GetDimensionPixelSize(
                0/*com.android.internal.R.styleable.DrawableCorners_radius*/,
                0, &radius);
        SetCornerRadius(radius);

        // now check of they have any per-corner radii

        Int32 topLeftRadius, topRightRadius, bottomLeftRadius, bottomRightRadius;
        a->GetDimensionPixelSize(
                1/*com.android.internal.R.styleable.DrawableCorners_topLeftRadius*/,
                radius, &topLeftRadius);
        a->GetDimensionPixelSize(
                2/*com.android.internal.R.styleable.DrawableCorners_topRightRadius*/,
                radius, &topRightRadius);
        a->GetDimensionPixelSize(
                3/*com.android.internal.R.styleable.DrawableCorners_bottomLeftRadius*/,
                radius, &bottomLeftRadius);
        a->GetDimensionPixelSize(
                4/*com.android.internal.R.styleable.DrawableCorners_bottomRightRadius*/,
                radius, &bottomRightRadius);

        if (topLeftRadius != radius || topRightRadius != radius ||
                bottomLeftRadius != radius || bottomRightRadius != radius) {
            ArrayOf_<Float, 8> radii;
            radii[0] = topLeftRadius; radii[1] = topLeftRadius;
            radii[2] = topRightRadius; radii[3] = topRightRadius;
            radii[4] = bottomLeftRadius; radii[5] = bottomLeftRadius;
            radii[6] = bottomRightRadius; radii[7] = bottomRightRadius;
            SetCornerRadii(&radii);
        }
        a->Recycle();
        return TRUE;
    }
    return ShapeDrawable::InflateTag(name, r, parser, attrs);
}

ECode PaintDrawable::Init()
{
    return ShapeDrawable::Init();
}

ECode PaintDrawable::Init(
    /* [in] */ Int32 color)
{
    FAIL_RETURN(ShapeDrawable::Init());
    GetPaint()->SetColor(color);
    return NOERROR;
}
