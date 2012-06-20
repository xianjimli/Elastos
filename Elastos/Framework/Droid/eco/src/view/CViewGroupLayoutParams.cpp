
#include "ext/frameworkdef.h"
#include "view/CViewGroupLayoutParams.h"

ECode CViewGroupLayoutParams::constructor()
{
    return NOERROR;
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return NOERROR;
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWidth = width;
    mHeight = height;

    return NOERROR;
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    mWidth = ((CViewGroupLayoutParams*)source)->mWidth;
    mHeight = ((CViewGroupLayoutParams*)source)->mHeight;

    return NOERROR;
}

ECode CViewGroupLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CViewGroupLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CViewGroupLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CViewGroupLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CViewGroupLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}
