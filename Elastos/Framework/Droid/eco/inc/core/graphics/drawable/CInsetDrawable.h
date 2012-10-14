
#ifndef __CINSETDRAWABLE_H__
#define __CINSETDRAWABLE_H__

#include "_CInsetDrawable.h"
#include "graphics/drawable/InsetDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"


CarClass(CInsetDrawable), public InsetDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 inset);

    CARAPI constructor(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 insetLeft,
        /* [in] */ Int32 insetTop,
        /* [in] */ Int32 insetRight,
        /* [in] */ Int32 insetBottom);

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    PInterface Probe(
        /* [in] */ REIID riid);
};

#endif //__CINSETDRAWABLE_H__
