
#ifndef __CSTATELISTDRAWABLE_H__
#define __CSTATELISTDRAWABLE_H__

#include "_CStateListDrawable.h"
#include "graphics/drawable/StateListDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"

CarClass(CStateListDrawable), public StateListDrawable
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    CARAPI AddState(
        /* [in] */ const ArrayOf<Int32> & stateSet,
        /* [in] */ IDrawable * pDrawable);

    CARAPI GetStateCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetStateSet(
        /* [in] */ Int32 index,
        /* [out, callee] */ ArrayOf<Int32> ** ppStateSet);

    CARAPI GetStateDrawable(
        /* [in] */ Int32 index,
        /* [out] */ IDrawable ** ppDrawable);

    CARAPI GetStateDrawableIndex(
        /* [in] */ const ArrayOf<Int32> & stateSet,
        /* [out] */ Int32 * pIndex);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSTATELISTDRAWABLE_H__
