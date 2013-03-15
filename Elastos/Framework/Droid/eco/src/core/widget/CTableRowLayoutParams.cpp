
#include "widget/CTableRowLayoutParams.h"

ECode CTableRowLayoutParams::GetDescription(
    /* [out] */ String * pDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableRowLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;
    return NOERROR;
}

ECode CTableRowLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;
    return NOERROR;
}

ECode CTableRowLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CTableRowLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CTableRowLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return TableRowLayoutParams::SetMargins(left, top, right, bottom);
}

ECode CTableRowLayoutParams::GetMargins(
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

ECode CTableRowLayoutParams::GetWeight(
    /* [out] */ Float* weight)
{
    VALIDATE_NOT_NULL(weight);

    *weight = mWeight;
    return NOERROR;
}

ECode CTableRowLayoutParams::SetWeight(
    /* [in] */ Float weight)
{
    mWeight = weight;
    return NOERROR;
}

ECode CTableRowLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);

    *gravity = mGravity;
    return NOERROR;
}

ECode CTableRowLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return TableRowLayoutParams::Init(c, attrs);
}

ECode CTableRowLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    return TableRowLayoutParams::Init(w, h);
}

ECode CTableRowLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Float initWeight)
{
    return TableRowLayoutParams::Init(w, h, initWeight);
}

ECode CTableRowLayoutParams::constructor()
{
    return TableRowLayoutParams::Init();
}

ECode CTableRowLayoutParams::constructor(
    /* [in] */ Int32 column)
{
    return TableRowLayoutParams::Init(column);
}

ECode CTableRowLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return LinearLayoutLayoutParams::Init(p);
}

ECode CTableRowLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return LinearLayoutLayoutParams::Init(source);
}

