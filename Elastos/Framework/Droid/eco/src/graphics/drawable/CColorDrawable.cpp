
#include "graphics/drawable/CColorDrawable.h"

IDRAWABLE_METHODS_IMPL(CColorDrawable, ColorDrawable, ColorDrawable);

ECode CColorDrawable::constructor()
{
    return ColorDrawable::InitEx(NULL);
}

ECode CColorDrawable::constructor(
    /* [in] */ Int32 color)
{
    return ColorDrawable::Init(color);
}

ECode CColorDrawable::constructor(
    /* [in] */ IColorState* state)
{
    return ColorDrawable::InitEx(state);
}

PInterface CColorDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CColorDrawable::Probe(riid);
}

ECode CColorDrawable::GetAlpha(
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = ColorDrawable::GetAlpha();

    return NOERROR;
}
