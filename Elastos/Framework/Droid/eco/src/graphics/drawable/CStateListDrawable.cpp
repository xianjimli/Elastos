
#include "graphics/drawable/CStateListDrawable.h"

IDRAWABLE_METHODS_IMPL(
    CStateListDrawable, StateListDrawable, StateListDrawable);

IDrawableCallback_METHODS_IMPL(
    CStateListDrawable, StateListDrawable, StateListDrawable);

PInterface CStateListDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CStateListDrawable::Probe(riid);
}

ECode CStateListDrawable::AddState(
    /* [in] */ const ArrayOf<Int32> & stateSet,
    /* [in] */ IDrawable* drawable)
{
    return StateListDrawable::AddState(stateSet, drawable);
}

ECode CStateListDrawable::GetStateCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = StateListDrawable::GetStateCount();
    return NOERROR;
}

ECode CStateListDrawable::GetStateSet(
    /* [in] */ Int32 index,
    /* [out, callee] */ ArrayOf<Int32> ** stateSet)
{
    VALIDATE_NOT_NULL(stateSet);
    *stateSet = StateListDrawable::GetStateSet(index);
    return NOERROR;
}

ECode CStateListDrawable::GetStateDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable ** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = StateListDrawable::GetStateDrawable(index);

    return NOERROR;
}

ECode CStateListDrawable::GetStateDrawableIndex(
    /* [in] */ const ArrayOf<Int32> & stateSet,
    /* [out] */ Int32 * index)
{
    VALIDATE_NOT_NULL(index);
    *index = StateListDrawable::GetStateDrawableIndex(stateSet);

    return NOERROR;
}

ECode CStateListDrawable::SelectDrawable(
    /* [in] */ Int32 idx,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = StateListDrawable::SelectDrawable(idx);

    return NOERROR;
}

ECode CStateListDrawable::constructor()
{
    return StateListDrawable::Init(NULL, NULL);
}

ECode CStateListDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return StateListDrawable::Init((StateListDrawable::StateListState*)state, res);
}

