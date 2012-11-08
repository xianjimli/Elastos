
#include "ext/frameworkext.h"
#include "view/menu/CIconMenuViewLayoutParams.h"


ECode CIconMenuViewLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return IconMenuViewLayoutParams::Init(c, attrs);
}

ECode CIconMenuViewLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return IconMenuViewLayoutParams::Init(width, height);
}

ECode CIconMenuViewLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);

    return NOERROR;
}

ECode CIconMenuViewLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mLeft = left;
    mTop = top;
    mRight = right;
    mBottom = bottom;

    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetMargins(
    /* [out] */ Int32* left,
    /* [out] */ Int32* top,
    /* [out] */ Int32* right,
    /* [out] */ Int32* bottom)
{
    VALIDATE_NOT_NULL(left);
    VALIDATE_NOT_NULL(top);
    VALIDATE_NOT_NULL(right);
    VALIDATE_NOT_NULL(bottom);

    *left = mLeft;
    *top = mTop;
    *right = mRight;
    *bottom = mBottom;

    return NOERROR;
}