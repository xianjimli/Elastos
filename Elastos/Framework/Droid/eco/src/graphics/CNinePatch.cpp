
#include "graphics/CNinePatch.h"
#include "ext/frameworkdef.h"

ECode CNinePatch::SetPaint(
    /* [in] */ IPaint* p)
{
    NinePatch::SetPaint(p);
    return NOERROR;
}

ECode CNinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRectF* location)
{
    return NinePatch::Draw(canvas, location);
}

ECode CNinePatch::DrawEx(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location)
{
    return NinePatch::Draw(canvas, location);
}

ECode CNinePatch::DrawEx2(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location,
    /* [in] */ IPaint* paint)
{
    return NinePatch::Draw(canvas, location, paint);
}

ECode CNinePatch::GetDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);
    return NinePatch::GetDensity(density);
}

ECode CNinePatch::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return NinePatch::GetWidth(width);
}

ECode CNinePatch::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    return NinePatch::GetHeight(height);;
}

ECode CNinePatch::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    VALIDATE_NOT_NULL(hasAlpha);
    return NinePatch::HasAlpha(hasAlpha);
}

ECode CNinePatch::GetTransparentRegion(
    /* [in] */ IRect* location,
    /* [out] */ IRegion** region)
{
    VALIDATE_NOT_NULL(region);
    return NinePatch::GetTransparentRegion(location, region);
}

ECode CNinePatch::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte> & chunk,
    /* [in] */ String srcName)
{
    return NinePatch::constructor(bitmap, chunk, srcName);
}

ECode CNinePatch::constructor(
    /* [in] */ INinePatch* patch)
{
    VALIDATE_NOT_NULL(patch);
    AutoPtr<CNinePatch> cpatch = (CNinePatch*)patch;
    return NinePatch::constructor((NinePatch*)cpatch.Get());
}


