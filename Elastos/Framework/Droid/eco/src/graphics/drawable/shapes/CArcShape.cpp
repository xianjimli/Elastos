
#include "graphics/drawable/shapes/CArcShape.h"

ECode CArcShape::GetWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = ArcShape::GetWidth();
    return NOERROR;
}

ECode CArcShape::GetHeight(
    /* [out] */ Float* height)
{
    VALIDATE_NOT_NULL(height);
    *height = ArcShape::GetHeight();
    return NOERROR;
}

ECode CArcShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return ArcShape::Draw(canvas, paint);
}

ECode CArcShape::Resize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    return ArcShape::Resize(width, height);
}

ECode CArcShape::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    VALIDATE_NOT_NULL(hasAlpha);
    *hasAlpha = ArcShape::HasAlpha();
    return NOERROR;
}

ECode CArcShape::Clone(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<CArcShape> s;
    CArcShape::NewByFriend((CArcShape**)&s);
    ArcShape::Clone((ArcShape*)s.Get());
    *shape = (IShape*)s.Get();
    (*shape)->AddRef();
    return NOERROR;
}

ECode CArcShape::constructor()
{
    return NOERROR;
}

ECode CArcShape::constructor(
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    return ArcShape::Init(startAngle, sweepAngle);
}
