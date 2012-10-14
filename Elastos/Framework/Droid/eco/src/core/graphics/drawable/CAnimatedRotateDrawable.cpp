
#include "graphics/drawable/CAnimatedRotateDrawable.h"

IDRAWABLE_METHODS_IMPL(
    CAnimatedRotateDrawable, AnimatedRotateDrawable, AnimatedRotateDrawable);

IDrawableCallback_METHODS_IMPL(
    CAnimatedRotateDrawable, AnimatedRotateDrawable, AnimatedRotateDrawable);

ECode CAnimatedRotateDrawable::constructor()
{
    return NOERROR;
}

ECode CAnimatedRotateDrawable::constructor(
    /* [in] */ IDrawableConstantState* state,
    /* [in] */ IResources* res)
{
    return AnimatedRotateDrawable::Init((AnimatedRotateState*)state, res);
}

PInterface CAnimatedRotateDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CAnimatedRotateDrawable::Probe(riid);
}

ECode CAnimatedRotateDrawable::Start()
{
    return AnimatedRotateDrawable::Start();
}

ECode CAnimatedRotateDrawable::Stop()
{
    return AnimatedRotateDrawable::Stop();
}

ECode CAnimatedRotateDrawable::IsRunning(
    /* [out] */ Boolean* isRunning)
{
    VALIDATE_NOT_NULL(isRunning);
    *isRunning = AnimatedRotateDrawable::IsRunning();

    return NOERROR;
}

ECode CAnimatedRotateDrawable::Run()
{
    return AnimatedRotateDrawable::Run();
}

ECode CAnimatedRotateDrawable::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = AnimatedRotateDrawable::GetDrawable();
    *drawable = temp;
    if (*drawable) {
        (*drawable)->AddRef();
    }

    return NOERROR;
}
