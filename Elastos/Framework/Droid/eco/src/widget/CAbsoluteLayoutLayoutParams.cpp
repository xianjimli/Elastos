
#include "widget/CAbsoluteLayoutLayoutParams.h"

ECode CAbsoluteLayoutLayoutParams::GetDescription(
    /* [out] */ String * pDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsoluteLayoutLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CAbsoluteLayoutLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CAbsoluteLayoutLayoutParams::GetWidth(
    /* [out] */ Int32 * width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CAbsoluteLayoutLayoutParams::GetHeight(
    /* [out] */ Int32 * height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CAbsoluteLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsoluteLayoutLayoutParams::constructor(
    /* [in] */ IContext * pC,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsoluteLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams * pSource)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

