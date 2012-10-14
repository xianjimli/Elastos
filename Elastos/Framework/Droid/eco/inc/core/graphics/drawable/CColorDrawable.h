
#ifndef __CCOLORDRAWABLE_H__
#define __CCOLORDRAWABLE_H__

#include "_CColorDrawable.h"
#include "graphics/drawable/ColorDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

CarClass(CColorDrawable), public ColorDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 color);

    CARAPI constructor(
        /* [in] */ IColorState* state);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetAlpha(
        /* [out] */ Int32* alpha);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCOLORDRAWABLE_H__
