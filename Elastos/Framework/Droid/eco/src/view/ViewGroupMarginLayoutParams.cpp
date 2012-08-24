
#include "view/ViewGroupMarginLayoutParams.h"
#include <elastos/AutoPtr.h>


ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams()
{}

ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

/**
 * {@inheritDoc}
 */
ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupLayoutParams(width, height)
{}

/**
 * Copy constructor. Clones the width, height and margin values of the source.
 *
 * @param source The layout params to copy from.
 */
ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : ViewGroupLayoutParams(source->mWidth, source->mHeight)
    , mLeftMargin(source->mLeftMargin)
    , mTopMargin(source->mTopMargin)
    , mRightMargin(source->mRightMargin)
    , mBottomMargin(source->mBottomMargin)
{}

/**
 * {@inheritDoc}
 */
ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupLayoutParams(source)
{}

/**
 * Sets the margins, in pixels.
 *
 * @param left the left margin size
 * @param top the top margin size
 * @param right the right margin size
 * @param bottom the bottom margin size
 *
 * @attr ref android.R.styleable#ViewGroup_MarginLayout_layout_marginLeft
 * @attr ref android.R.styleable#ViewGroup_MarginLayout_layout_marginTop
 * @attr ref android.R.styleable#ViewGroup_MarginLayout_layout_marginRight
 * @attr ref android.R.styleable#ViewGroup_MarginLayout_layout_marginBottom
 */
ECode ViewGroupMarginLayoutParams::SetMargins(
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

static Int32 R_Styleable_ViewGroup_MarginLayout[] = {
    0x010100f4, 0x010100f5, 0x010100f6, 0x010100f7,
    0x010100f8, 0x010100f9, 0x010100fa
};

ECode ViewGroupMarginLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(c->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_ViewGroup_MarginLayout, 7)/*R.styleable.ViewGroup_MarginLayout*/,
            (ITypedArray**)&a));
    SetBaseAttributes(a,
            0/*R.styleable.ViewGroup_MarginLayout_layout_width*/,
            1/*R.styleable.ViewGroup_MarginLayout_layout_height*/);

    Int32 margin;
    a->GetDimensionPixelSize(
            2/*com.android.internal.R.styleable.ViewGroup_MarginLayout_layout_margin*/,
            -1, &margin);
    if (margin >= 0) {
        mLeftMargin = margin;
        mTopMargin = margin;
        mRightMargin= margin;
        mBottomMargin = margin;
    }
    else {
        a->GetDimensionPixelSize(
                3/*R.styleable.ViewGroup_MarginLayout_layout_marginLeft*/,
                0, &mLeftMargin);
        a->GetDimensionPixelSize(
                4/*R.styleable.ViewGroup_MarginLayout_layout_marginTop*/,
                0, &mTopMargin);
        a->GetDimensionPixelSize(
                5/*R.styleable.ViewGroup_MarginLayout_layout_marginRight*/,
                0, &mRightMargin);
        a->GetDimensionPixelSize(
                6/*R.styleable.ViewGroup_MarginLayout_layout_marginBottom*/,
                0, &mBottomMargin);
    }

    a->Recycle();
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupLayoutParams::Init(width, height);
}

ECode ViewGroupMarginLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    source->GetWidth(&mWidth);
    source->GetHeight(&mHeight);
    source->GetMargins(&mLeftMargin, &mTopMargin, &mRightMargin, &mBottomMargin);
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
}
