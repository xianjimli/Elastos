
#include "graphics/drawable/CNinePatchDrawable.h"
#include <stdio.h>

IDRAWABLE_METHODS_IMPL(CNinePatchDrawable, NinePatchDrawable, NinePatchDrawable);

ECode CNinePatchDrawable::constructor()
{
    return NOERROR;
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte> & chunk,
    /* [in] */ IRect* padding,
    /* [in] */ String srcName)
{
    return NinePatchDrawable::Init(bitmap, chunk, padding, srcName);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte> & chunk,
    /* [in] */ IRect* padding,
    /* [in] */ String srcName)
{
    return NinePatchDrawable::Init(res, bitmap, chunk, padding, srcName);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ INinePatch* patch)
{
    return NinePatchDrawable::Init(patch);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ INinePatch* patch)
{
    return NinePatchDrawable::Init(res, patch);
}

ECode CNinePatchDrawable::constructor(
    /* [in] */ INinePatchState* state,
    /* [in] */ IResources* res)
{
    return NinePatchDrawable::Init(state, res);
}

PInterface CNinePatchDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CNinePatchDrawable::Probe(riid);
}

ECode CNinePatchDrawable::SetTargetDensity(
    /* [in] */ ICanvas* canvas)
{
    return NinePatchDrawable::SetTargetDensity(canvas);
}

ECode CNinePatchDrawable::SetTargetDensityEx(
    /* [in] */ IDisplayMetrics* metrics)
{
    return NinePatchDrawable::SetTargetDensity(metrics);
}

ECode CNinePatchDrawable::SetTargetDensityEx2(
    /* [in] */ Int32 density)
{
    return NinePatchDrawable::SetTargetDensity(density);
}

ECode CNinePatchDrawable::GetPaint(
    /* [out] */ IPaint** paint)
{
    VALIDATE_NOT_NULL(paint);

    AutoPtr<IPaint> temp = NinePatchDrawable::GetPaint();
    *paint = temp;
    if (*paint) {
        (*paint)->AddRef();
    }

    return NOERROR;
}
