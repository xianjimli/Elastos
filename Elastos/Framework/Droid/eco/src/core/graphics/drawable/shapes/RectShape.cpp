
#include "graphics/drawable/shapes/RectShape.h"

/**
 * RectShape constructor.
 */
RectShape::RectShape()
{
    CRectF::NewByFriend((CRectF**)&mRect);
}

ECode RectShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawRect(mRect, paint);
}

void RectShape::OnResize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    mRect->Set(0, 0, width, height);
}

/**
 * Returns the RectF that defines this rectangle's bounds.
 */
AutoPtr<CRectF> RectShape::Rect()
{
    return mRect;
}

void RectShape::Clone(
    /* [in] */ RectShape* other)
{
    Shape::Clone((Shape*)other);
    CRectF::NewByFriend((IRectF*)mRect.Get(), (CRectF**)&other->mRect);
}

ECode RectShape::Init()
{
    return NOERROR;
}
