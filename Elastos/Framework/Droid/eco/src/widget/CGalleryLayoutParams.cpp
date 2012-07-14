
#include "widget/CGalleryLayoutParams.h"
ECode CGalleryLayoutParams::GetDescription(
    /* [out] */ String * pDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGalleryLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CGalleryLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CGalleryLayoutParams::GetWidth(
    /* [out] */ Int32 * pWidth)
{
    VALIDATE_NOT_NULL(pWidth);

    *pWidth = mWidth;
    return NOERROR;
}

ECode CGalleryLayoutParams::GetHeight(
    /* [out] */ Int32 * pHeight)
{
    VALIDATE_NOT_NULL(pHeight);

    *pHeight = mHeight;
    return NOERROR;
}

ECode CGalleryLayoutParams::constructor(
    /* [in] */ IContext * pC,
    /* [in] */ IAttributeSet * pAttrs)
{
    return ViewGroupLayoutParams::Init(pC, pAttrs);
}

ECode CGalleryLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    return ViewGroupLayoutParams::Init(w, h);
}

ECode CGalleryLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams * pSource)
{
    return ViewGroupLayoutParams::Init(pSource);
}

