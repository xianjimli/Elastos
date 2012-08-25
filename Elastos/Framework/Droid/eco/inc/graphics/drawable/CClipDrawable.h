
#ifndef __CCLIPDRAWABLE_H__
#define __CCLIPDRAWABLE_H__

#include "_CClipDrawable.h"
#include "graphics/drawable/ClipDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"

CarClass(CClipDrawable), public ClipDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    CARAPI constructor();

    /**
     * @param orientation Bitwise-or of {@link #HORIZONTAL} and/or {@link #VERTICAL}
     */
    CARAPI constructor(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 orientation);

    CARAPI constructor(
        /* [in] */ IClipState* state,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

#endif //__CCLIPDRAWABLE_H__
