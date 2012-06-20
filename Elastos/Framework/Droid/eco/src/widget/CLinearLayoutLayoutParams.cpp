
#include "ext/frameworkdef.h"
#include "widget/CLinearLayoutLayoutParams.h"

ECode CLinearLayoutLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLinearLayoutLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CLinearLayoutLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CLinearLayoutLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CLinearLayoutLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CLinearLayoutLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return LinearLayoutLayoutParams::SetMargins(left, top, right, bottom);
}

ECode CLinearLayoutLayoutParams::GetMargins(
    /* [out] */ Int32* left,
    /* [out] */ Int32* top,
    /* [out] */ Int32* right,
    /* [out] */ Int32* bottom)
{
    VALIDATE_NOT_NULL(left);
    VALIDATE_NOT_NULL(top);
    VALIDATE_NOT_NULL(right);
    VALIDATE_NOT_NULL(bottom);

    *left = mLeftMargin;
    *top = mTopMargin;
    *right = mRightMargin;
    *bottom = mBottomMargin;

    return NOERROR;
}

ECode CLinearLayoutLayoutParams::GetWeight(
    /* [out] */ Float* weight)
{
    VALIDATE_NOT_NULL(weight);

    *weight = mWeight;

    return NOERROR;
}

ECode CLinearLayoutLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);

    *gravity = mGravity;

    return NOERROR;
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return LinearLayoutLayoutParams::Init(c, attrs);
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return LinearLayoutLayoutParams::Init(width, height);
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
{
    return LinearLayoutLayoutParams::Init(width, height, weight);
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return LinearLayoutLayoutParams::Init(p);
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return LinearLayoutLayoutParams::Init(source);
}

