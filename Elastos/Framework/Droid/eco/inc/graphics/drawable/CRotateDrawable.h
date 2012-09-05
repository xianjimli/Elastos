
#ifndef __CROTATEDRAWABLE_H__
#define __CROTATEDRAWABLE_H__

#include "_CRotateDrawable.h"
#include "graphics/drawable/RotateDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"


CarClass(CRotateDrawable), public RotateDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    /**
     * Returns the drawable rotated by this RotateDrawable.
     */
    CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);

    /**
     * <p>Create a new rotating drawable with an empty state.</p>
     */
    CARAPI constructor();

    /**
     * <p>Create a new rotating drawable with the specified state. A copy of
     * this state is used as the internal state for the newly created
     * drawable.</p>
     *
     * @param rotateState the state for this drawable
     */
    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    PInterface Probe(
        /* [in] */ REIID riid);
};

#endif //__CROTATEDRAWABLE_H__
