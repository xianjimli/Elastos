
#include "systemui/statusbar/FixedSizeDrawable.h"

FixedSizeDrawable::FixedSizeDrawable()
{
}

FixedSizeDrawable::FixedSizeDrawable(
    /* [in] */ IDrawable* that)
{
    mDrawable = that;
}

ECode FixedSizeDrawable::SetFixedBounds(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    mLeft = l;
    mTop = t;
    mRight = r;
    mBottom = b;

    return NOERROR;
}

ECode FixedSizeDrawable::SetBounds(
    /* [in] */ IRect* bounds)
{
    return mDrawable->SetBounds(mLeft, mTop, mRight, mBottom);
}

ECode FixedSizeDrawable::SetBounds(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    return mDrawable->SetBounds(mLeft, mTop, mRight, mBottom);
}

ECode FixedSizeDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    return mDrawable->Draw(canvas);
}

Int32 FixedSizeDrawable::GetOpacity()
{
    Int32 opacity;
    mDrawable->GetOpacity(&opacity);

    return opacity;
}

ECode FixedSizeDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mDrawable->SetAlpha(alpha);
}

ECode FixedSizeDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mDrawable->SetColorFilter(cf);
}

ECode FixedSizeDrawable::Init(
    /* [in] */ IDrawable* that)
{
    mDrawable = that;

    return NOERROR;
}
