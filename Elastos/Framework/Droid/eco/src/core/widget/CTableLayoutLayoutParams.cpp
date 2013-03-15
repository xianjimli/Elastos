
#include "widget/CTableLayoutLayoutParams.h"

ECode CTableLayoutLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayoutLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;
    return NOERROR;
}

ECode CTableLayoutLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;
    return NOERROR;
}

ECode CTableLayoutLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CTableLayoutLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CTableLayoutLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return TableLayoutLayoutParams::SetMargins(left, top, right, bottom);
}

ECode CTableLayoutLayoutParams::GetMargins(
    /* [out] */ Int32 * left,
    /* [out] */ Int32 * top,
    /* [out] */ Int32 * right,
    /* [out] */ Int32 * bottom)
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

ECode CTableLayoutLayoutParams::GetWeight(
    /* [out] */ Float * weight)
{
    VALIDATE_NOT_NULL(weight);

    *weight = mWeight;
    return NOERROR;
}

ECode CTableLayoutLayoutParams::SetWeight(
    /* [in] */ Float weight)
{
    mWeight = weight;
    return NOERROR;
}

ECode CTableLayoutLayoutParams::GetGravity(
    /* [out] */ Int32 * gravity)
{
    VALIDATE_NOT_NULL(gravity);

    *gravity = mGravity;
    return NOERROR;
}

ECode CTableLayoutLayoutParams::constructor(
    /* [in] */ IContext * c,
    /* [in] */ IAttributeSet * attrs)
{
    return LinearLayoutLayoutParams::Init(c, attrs);
}

ECode CTableLayoutLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    return LinearLayoutLayoutParams::Init(w, h);
}

ECode CTableLayoutLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Float initWeight)
{
    return LinearLayoutLayoutParams::Init(w, h, initWeight);
}

ECode CTableLayoutLayoutParams::constructor()
{
    return LinearLayoutLayoutParams::Init(MATCH_PARENT, WRAP_CONTENT);
}

ECode CTableLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return LinearLayoutLayoutParams::Init(p);
}

ECode CTableLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return LinearLayoutLayoutParams::Init(source);
}

