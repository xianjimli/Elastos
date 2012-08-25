
#ifndef __CANIMATEDROTATEDRAWABLE_H__
#define __CANIMATEDROTATEDRAWABLE_H__

#include "_CAnimatedRotateDrawable.h"
#include "graphics/drawable/AnimatedRotateDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"

CarClass(CAnimatedRotateDrawable), AnimatedRotateDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Start();

    CARAPI Stop();

    CARAPI IsRunning(
        /* [out] */ Boolean* isRunning);

    CARAPI Run();

    CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);

private:
    // TODO: Add your private member variables here.
};

#endif // __CANIMATEDROTATEDRAWABLE_H__

