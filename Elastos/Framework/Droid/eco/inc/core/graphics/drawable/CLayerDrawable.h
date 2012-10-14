
#ifndef __CLAYERDRAWABLE_H__
#define __CLAYERDRAWABLE_H__

#include "_CLayerDrawable.h"
#include "graphics/drawable/LayerDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"

CarClass(CLayerDrawable), LayerDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    CARAPI FindDrawableByLayerId(
        /* [in] */ Int32 id,
        /* [out] */ IDrawable** drawable);

    CARAPI SetId(
        /* [in] */ Int32 index,
        /* [in] */ Int32 id);

    CARAPI GetNumberOfLayers(
        /* [out] */ Int32* number);

    CARAPI GetDrawable(
        /* [in] */ Int32 index,
        /* [out] */ IDrawable** drawable);

    CARAPI GetId(
        /* [in] */ Int32 index,
        /* [out] */ Int32* id);

    CARAPI SetDrawableByLayerId(
        /* [in] */ Int32 id,
        /* [in] */ IDrawable* drawable,
        /* [out] */ Boolean* res);

    CARAPI SetLayerInset(
        /* [in] */ Int32 index,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IObjectContainer* layers);

    CARAPI constructor(
        /* [in] */ IObjectContainer* layers,
        /* [in] */ IDrawableConstantState* state);

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state,
        /* [in] */ IResources* res);

    PInterface Probe(
        /* [in] */ REIID riid);

private:
    // TODO: Add your private member variables here.
};

#endif // __CLAYERDRAWABLE_H__
