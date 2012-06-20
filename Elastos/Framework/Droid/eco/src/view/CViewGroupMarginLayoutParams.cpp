
#include "ext/frameworkdef.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CViewGroupMarginLayoutParams.h"

ECode CViewGroupMarginLayoutParams::GetDescription(
    /* [out] */ String* pDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CViewGroupMarginLayoutParams::constructor()
{
    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    //super();

    //TypedArray a = c.obtainStyledAttributes(attrs, R.styleable.ViewGroup_MarginLayout);
    //setBaseAttributes(a,
    //        R.styleable.ViewGroup_MarginLayout_layout_width,
    //        R.styleable.ViewGroup_MarginLayout_layout_height);

    //Int32 margin = a.getDimensionPixelSize(
    //        com.android.internal.R.styleable.ViewGroup_MarginLayout_layout_margin, -1);
    //if (margin >= 0) {
    //    leftMargin = margin;
    //    topMargin = margin;
    //    rightMargin= margin;
    //    bottomMargin = margin;
    //} else {
    //    leftMargin = a.getDimensionPixelSize(
    //            R.styleable.ViewGroup_MarginLayout_layout_marginLeft, 0);
    //    topMargin = a.getDimensionPixelSize(
    //            R.styleable.ViewGroup_MarginLayout_layout_marginTop, 0);
    //    rightMargin = a.getDimensionPixelSize(
    //            R.styleable.ViewGroup_MarginLayout_layout_marginRight, 0);
    //    bottomMargin = a.getDimensionPixelSize(
    //            R.styleable.ViewGroup_MarginLayout_layout_marginBottom, 0);
    //}

    //a.recycle();
    // TODO: Add your code here
    //return E_NOT_IMPLEMENTED;

    //todo:
    mWidth = ViewGroupLayoutParams::MATCH_PARENT;
    mHeight = ViewGroupLayoutParams::MATCH_PARENT;

    mLeftMargin = 0;
    mTopMargin = 0;
    mRightMargin = 0;
    mBottomMargin = 0;

    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    VALIDATE_NOT_NULL(source);

    mWidth = ((CViewGroupLayoutParams*)source)->mWidth;
    mHeight = ((CViewGroupLayoutParams*)source)->mHeight;

    mLeftMargin = ((CViewGroupMarginLayoutParams*)source)->mLeftMargin;
    mTopMargin = ((CViewGroupMarginLayoutParams*)source)->mTopMargin;
    mRightMargin = ((CViewGroupMarginLayoutParams*)source)->mRightMargin;
    mBottomMargin = ((CViewGroupMarginLayoutParams*)source)->mBottomMargin;

    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::GetWidth(
    /* [out] */ Int32 * width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::GetHeight(
    /* [out] */ Int32 * height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mLeftMargin = left;
    mTopMargin = top;
    mRightMargin = right;
    mBottomMargin = bottom;

    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::GetMargins(
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
