
#include "systemui/statusbar/CFixedSizeDrawable.h"

IDRAWABLE_METHODS_IMPL(
    CFixedSizeDrawable, FixedSizeDrawable, FixedSizeDrawable);

ECode CFixedSizeDrawable::constructor(
    /* [in] */ IDrawable* that)
{
    return FixedSizeDrawable::Init(that);
}

PInterface CFixedSizeDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CFixedSizeDrawable::Probe(riid);
}

ECode CFixedSizeDrawable::SetFixedBounds(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    return FixedSizeDrawable::SetFixedBounds(l, t, r, b);
}
