
#include "graphics/drawable/shapes/CRectShape.h"

ECode CRectShape::GetWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = RectShape::GetWidth();
    return NOERROR;
}

ECode CRectShape::GetHeight(
    /* [out] */ Float* height)
{
    VALIDATE_NOT_NULL(height);
    *height = RectShape::GetHeight();
    return NOERROR;
}

ECode CRectShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return RectShape::Draw(canvas, paint);
}

ECode CRectShape::Resize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    return RectShape::Resize(width, height);
}

ECode CRectShape::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    VALIDATE_NOT_NULL(hasAlpha);
    *hasAlpha = RectShape::HasAlpha();
    return NOERROR;
}

ECode CRectShape::Clone(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<CRectShape> s;
    CRectShape::NewByFriend((CRectShape**)&s);
    RectShape::Clone((RectShape*)s.Get());
    *shape = (IShape*)s.Get();
    (*shape)->AddRef();
    return NOERROR;
}

ECode CRectShape::constructor()
{
    return RectShape::Init();
}
