
#include "widget/LinearLayoutLayoutParams.h"
#include <elastos/AutoPtr.h>

LinearLayoutLayoutParams::LinearLayoutLayoutParams()
    : mGravity(-1)
{}

/**
 * {@inheritDoc}
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : mGravity(-1)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

/**
 * {@inheritDoc}
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupMarginLayoutParams(width, height)
    , mWeight(0)
    , mGravity(-1)
{}

/**
 * Creates a new set of layout parameters with the specified width, height
 * and weight.
 *
 * @param width the width, either {@link #MATCH_PARENT},
 *        {@link #WRAP_CONTENT} or a fixed size in pixels
 * @param height the height, either {@link #MATCH_PARENT},
 *        {@link #WRAP_CONTENT} or a fixed size in pixels
 * @param weight the weight
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
    : ViewGroupMarginLayoutParams(width, height)
    , mWeight(weight)
    , mGravity(-1)
{}

/**
 * {@inheritDoc}
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* p)
    : ViewGroupMarginLayoutParams(p)
    , mGravity(-1)
{}

/**
 * {@inheritDoc}
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : ViewGroupMarginLayoutParams(source)
    , mGravity(-1)
{}

static Int32 R_Styleable_LinearLayout_Layout[] = {
    0x010100b3, 0x010100f4, 0x010100f5, 0x01010181
};

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(c, attrs));

    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(c->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_LinearLayout_Layout, 4)/*R.styleable.LinearLayout_Layout*/,
            (ITypedArray**)&a));

    a->GetFloat(3/*com.android.internal.R.styleable.LinearLayout_Layout_layout_weight*/,
            0, &mWeight);
    a->GetInt32(0/*com.android.internal.R.styleable.LinearLayout_Layout_layout_gravity*/,
            -1, &mGravity);

    a->Recycle();
    return NOERROR;
}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ViewGroupMarginLayoutParams::Init(width, height);
    mWeight = 0;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
{
    ViewGroupMarginLayoutParams::Init(width, height);
    mWeight = weight;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    ViewGroupMarginLayoutParams::Init(source);
    return NOERROR;
}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    ViewGroupMarginLayoutParams::Init(source);
    return NOERROR;
}
