
#ifndef __CNINEPATCHDRAWABLE_H__
#define __CNINEPATCHDRAWABLE_H__

#include "_CNinePatchDrawable.h"
#include "graphics/drawable/NinePatchDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

CarClass(CNinePatchDrawable), NinePatchDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte> & chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte> & chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ INinePatch* patch);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ INinePatch* patch);

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetTargetDensity(
        /* [in] */ ICanvas* canvas);

    CARAPI SetTargetDensityEx(
        /* [in] */ IDisplayMetrics* metrics);

    CARAPI SetTargetDensityEx2(
        /* [in] */ Int32 density);

    CARAPI GetPaint(
        /* [out] */ IPaint** paint);

private:
    // TODO: Add your private member variables here.
};

#endif // __CNINEPATCHDRAWABLE_H__

