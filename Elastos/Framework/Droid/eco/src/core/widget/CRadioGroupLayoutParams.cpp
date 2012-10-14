
#include "ext/frameworkdef.h"
#include "widget/CRadioGroupLayoutParams.h"

ECode CRadioGroupLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioGroupLayoutParams::SetWidth(
                                        /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CRadioGroupLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CRadioGroupLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CRadioGroupLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CRadioGroupLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return RadioGroupLayoutParams::SetMargins(left, top, right, bottom);
}

ECode CRadioGroupLayoutParams::GetMargins(
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

ECode CRadioGroupLayoutParams::GetWeight(
    /* [out] */ Float* weight)
{
    VALIDATE_NOT_NULL(weight);

    *weight = mWeight;

    return NOERROR;
}

ECode CRadioGroupLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);

    *gravity = mGravity;

    return NOERROR;
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return RadioGroupLayoutParams::Init(c, attrs);
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return RadioGroupLayoutParams::Init(width, height);
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
{
    return RadioGroupLayoutParams::Init(width, height, weight);
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return RadioGroupLayoutParams::Init(p);
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return RadioGroupLayoutParams::Init(source);
}
