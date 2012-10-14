
#ifndef __CLEVELLISTDRAWABLE_H__
#define __CLEVELLISTDRAWABLE_H__

#include "_CLevelListDrawable.h"
#include "graphics/drawable/LevelListDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"

CarClass(CLevelListDrawable), public LevelListDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    CARAPI AddLevel(
        /* [in] */ Int32 low,
        /* [in] */ Int32 high,
        /* [in] */ IDrawable* drawable);

    CARAPI SelectDrawable(
        /* [in] */ Int32 idx,
        /* [out] */ Boolean* res);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};


#endif //__CLEVELLISTDRAWABLE_H__
