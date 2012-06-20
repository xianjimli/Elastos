
#include "ext/frameworkdef.h"
#include "graphics/CTypefaceFactory.h"
#include "graphics/CTypeface.h"
#include <elastos/AutoPtr.h>

ECode CTypefaceFactory::Create(
    /* [in] */ String familyName,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    return Typeface::Create(familyName, style, typeface);
}

ECode CTypefaceFactory::CreateFromTypeface(
    /* [in] */ ITypeface* family,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    return Typeface::Create(family, style, typeface);
}

ECode CTypefaceFactory::DefaultFromStyle(
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    return Typeface::DefaultFromStyle(style, typeface);
}

ECode CTypefaceFactory::CreateFromAsset(
    /* [in] */ IAssetManager* mgr,
    /* [in] */ String path,
    /* [out] */ ITypeface** typeface)
{
    return Typeface::CreateFromAsset(mgr, path, typeface);
}

ECode CTypefaceFactory::CreateFromFile(
    /* [in] */ IFile* path,
    /* [out] */ ITypeface** typeface)
{
    return Typeface::CreateFromFile(path, typeface);
}

ECode CTypefaceFactory::CreateFromFileEx(
    /* [in]*/ String path,
    /* [out] */ ITypeface** typeface)
{
    return Typeface::CreateFromFile(path, typeface);
}

ECode CTypefaceFactory::SetGammaForText(
    /* [in] */ Float blackGamma,
    /* [in] */ Float whiteGamma)
{
    return Typeface::SetGammaForText(blackGamma, whiteGamma);
}
