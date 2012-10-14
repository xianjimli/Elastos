
#include "widget/CGalleryLayoutParams.h"
ECode CGalleryLayoutParams::GetDescription(
    /* [out] */ String* description)
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
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CGalleryLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CGalleryLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupLayoutParams::Init(c, attrs);
}

ECode CGalleryLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    return ViewGroupLayoutParams::Init(w, h);
}

ECode CGalleryLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
}

