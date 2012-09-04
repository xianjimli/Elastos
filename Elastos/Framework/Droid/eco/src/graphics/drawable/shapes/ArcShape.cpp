
#include "graphics/drawable/shapes/ArcShape.h"


ArcShape::ArcShape()
{}

/**
 * ArcShape constructor.
 *
 * @param startAngle the angle (in degrees) where the arc begins
 * @param sweepAngle the sweep angle (in degrees). Anything equal to or
 *                   greater than 360 results in a complete circle/oval.
 */
ArcShape::ArcShape(
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
    : mStart(startAngle)
    , mSweep(sweepAngle)
{}

ECode ArcShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawArc(Rect(), mStart, mSweep, TRUE, paint);
}

ECode ArcShape::Init(
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    mStart = startAngle;
    mSweep = sweepAngle;
    return NOERROR;
}

void ArcShape::Clone(
    /* [in] */ ArcShape* other)
{
    RectShape::Clone((RectShape*)other);
    other->mStart = mStart;
    other->mSweep = mSweep;
}
