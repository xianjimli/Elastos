
#ifndef __FIXEDSIZEDRAWABLE_H__
#define __FIXEDSIZEDRAWABLE_H__

#include "graphics/drawable/Drawable.h"

class FixedSizeDrawable : public Drawable
{
public:
    FixedSizeDrawable();

    FixedSizeDrawable(
        /* [in] */ IDrawable* that);

    CARAPI SetFixedBounds(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI SetBounds(
        /* [in] */ IRect* bounds);

    CARAPI SetBounds(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    Int32 GetOpacity();

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

protected:
    CARAPI Init(
        /* [in] */ IDrawable* that);

protected:
    AutoPtr<IDrawable> mDrawable;
    Int32 mLeft;
    Int32 mTop;
    Int32 mRight;
    Int32 mBottom;
};

#endif // __FIXEDSIZEDRAWABLE_H__

