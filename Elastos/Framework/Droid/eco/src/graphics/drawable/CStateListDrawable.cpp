
#include "graphics/drawable/CStateListDrawable.h"

IDRAWABLE_METHODS_IMPL(CStateListDrawable, StateListDrawable, StateListDrawable);

ECode CStateListDrawable::AddState(
    /* [in] */ const ArrayOf<Int32> & stateSet,
    /* [in] */ IDrawable * pDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStateListDrawable::GetStateListState(
    /* [out] */ IStateListState ** ppState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStateListDrawable::GetStateCount(
    /* [out] */ Int32 * pCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStateListDrawable::GetStateSet(
    /* [in] */ Int32 index,
    /* [out, callee] */ ArrayOf<Int32> ** ppStateSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStateListDrawable::GetStateDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable ** ppDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStateListDrawable::GetStateDrawableIndex(
    /* [in] */ const ArrayOf<Int32> & stateSet,
    /* [out] */ Int32 * pIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStateListDrawable::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStateListDrawable::constructor(
    /* [in] */ IStateListState* state,
    /* [in] */ IResources* res)
{
    return NOERROR;
}

