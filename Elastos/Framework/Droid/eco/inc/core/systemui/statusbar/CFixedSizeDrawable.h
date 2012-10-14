
#ifndef __CFIXEDSIZEDRAWABLE_H__
#define __CFIXEDSIZEDRAWABLE_H__

#include "_CFixedSizeDrawable.h"
#include "systemui/statusbar/FixedSizeDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

CarClass(CFixedSizeDrawable), FixedSizeDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IDrawable* that);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetFixedBounds(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

private:
    // TODO: Add your private member variables here.
};

#endif // __CFIXEDSIZEDRAWABLE_H__

