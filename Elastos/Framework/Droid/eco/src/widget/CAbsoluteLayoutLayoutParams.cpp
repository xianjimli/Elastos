
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
    return AbsoluteLayoutLayoutParams::Init(width, height, x, y);
}

ECode CAbsoluteLayoutLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return AbsoluteLayoutLayoutParams::Init(c, attrs);
}

ECode CAbsoluteLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return AbsoluteLayoutLayoutParams::Init(source);
}

