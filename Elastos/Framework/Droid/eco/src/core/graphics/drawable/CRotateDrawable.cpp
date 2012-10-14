
#include "graphics/drawable/CRotateDrawable.h"

IDRAWABLE_METHODS_IMPL(
    CRotateDrawable, RotateDrawable, RotateDrawable);

IDrawableCallback_METHODS_IMPL(
    CRotateDrawable, RotateDrawable, RotateDrawable);

/**
 * Returns the drawable rotated by this RotateDrawable.
 */
ECode CRotateDrawable::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> d = RotateDrawable::GetDrawable();
    *drawable = d;
    if (*drawable != NULL) (*drawable)->AddRef();
    return NOERROR;
}

/**
 * <p>Create a new rotating drawable with an empty state.</p>
 */
ECode CRotateDrawable::constructor()
{
    return RotateDrawable::Init();
}

/**
 * <p>Create a new rotating drawable with the specified state. A copy of
 * this state is used as the internal state for the newly created
 * drawable.</p>
 *
 * @param rotateState the state for this drawable
 */
ECode CRotateDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return RotateDrawable::Init((RotateState*)state, res);
}

PInterface CRotateDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CRotateDrawable::Probe(riid);
}
