
#include "widget/CAbsListViewLayoutParams.h"

ECode CAbsListViewLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsListViewLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CAbsListViewLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CAbsListViewLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CAbsListViewLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CAbsListViewLayoutParams::GetViewType(
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);
    *viewType = mViewType;

    return NOERROR;
}

ECode CAbsListViewLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return AbsListViewLayoutParams::Init(c, attrs);
}

ECode CAbsListViewLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return AbsListViewLayoutParams::Init(width, height);
}

ECode CAbsListViewLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 viewType)
{
    return AbsListViewLayoutParams::Init(width, height, viewType);
}

ECode CAbsListViewLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return AbsListViewLayoutParams::Init(source);
}
