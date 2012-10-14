
#include "graphics/drawable/shapes/OvalShape.h"

/**
 * OvalShape constructor.
 */
OvalShape::OvalShape()
{}

ECode OvalShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawOval(Rect(), paint);
}

ECode OvalShape::Init()
{
    return NOERROR;
}

void OvalShape::Clone(
    /* [in] */ OvalShape* other)
{
    RectShape::Clone((RectShape*)other);
}
