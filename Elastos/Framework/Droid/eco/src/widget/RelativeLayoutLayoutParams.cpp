
#include "widget/RelativeLayoutLayoutParams.h"
#include <elastos/AutoPtr.h>

RelativeLayoutLayoutParams::RelativeLayoutLayoutParams()
    : mAlignWithParent(FALSE)
{}

/**
 * {@inheritDoc}
 */
RelativeLayoutLayoutParams::RelativeLayoutLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupMarginLayoutParams(c, attrs)
    , mAlignWithParent(FALSE)
{
}

/**
 * {@inheritDoc}
 */
RelativeLayoutLayoutParams::RelativeLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupMarginLayoutParams(width, height)
    , mAlignWithParent(FALSE)
{}

/**
 * {@inheritDoc}
 */
RelativeLayoutLayoutParams::RelativeLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* p)
    : ViewGroupMarginLayoutParams(p)
    , mAlignWithParent(FALSE)
{}

/**
 * {@inheritDoc}
 */
RelativeLayoutLayoutParams::RelativeLayoutLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : ViewGroupMarginLayoutParams(source)
    , mAlignWithParent(FALSE)
{}

static Int32 R_Styleable_RelativeLayout_Layout[] = {
    0x01010182, 0x01010183, 0x01010184, 0x01010185,
    0x01010186, 0x01010187, 0x01010188, 0x01010189,
    0x0101018a, 0x0101018b, 0x0101018c, 0x0101018d,
    0x0101018e, 0x0101018f, 0x01010190, 0x01010191,
    0x01010192
};

ECode RelativeLayoutLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(c, attrs));

    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(c->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_RelativeLayout_Layout, 17)/*R.styleable.RelativeLayout_Layout*/,
            (ITypedArray**)&a));

    ArrayOf<Int32>* rules = mRules.Clone();
    Int32 N = rules->GetLength();
    for (Int32 i = 0; i < N; i++) {
        Int32 attr = (*rules)[i];
        Boolean val = FALSE;
        switch (attr) {
            case 16:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignWithParentIfMissing*/
                a->GetBoolean(attr, FALSE, &mAlignWithParent);
                break;
            case 0:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_toLeftOf*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_LEFT_OF]);
                break;
            case 1:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_toRightOf*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_RIGHT_OF]);
                break;
            case 2:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_above*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_ABOVE]);
                break;
            case 3:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_below*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_BELOW]);
                break;
            case 4:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignBaseline*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_ALIGN_BASELINE]);
                break;
            case 5:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignLeft*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_ALIGN_LEFT]);
                break;
            case 6:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignTop*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_ALIGN_TOP]);
                break;
            case 7:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignRight*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_ALIGN_RIGHT]);
                break;
            case 8:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignBottom*/
                a->GetResourceId(attr, 0, &(*rules)[RelativeLayout_ALIGN_BOTTOM]);
                break;
            case 9:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignParentLeft*/
                a->GetBoolean(attr, FALSE, &val);
                if (val) {
                    (*rules)[RelativeLayout_ALIGN_PARENT_LEFT] = 1;
                }
                else {
                    (*rules)[RelativeLayout_ALIGN_PARENT_LEFT] = 0;
                }
                break;
            case 10:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignParentTop*/
                a->GetBoolean(attr, FALSE, &val);
                if (val) {
                    (*rules)[RelativeLayout_ALIGN_PARENT_TOP] = 1;
                }
                else {
                    (*rules)[RelativeLayout_ALIGN_PARENT_TOP] = 0;
                }
                break;
            case 11:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignParentRight*/
                a->GetBoolean(attr, FALSE, &val);
                if (val) {
                    (*rules)[RelativeLayout_ALIGN_PARENT_RIGHT] = 1;
                }
                else {
                    (*rules)[RelativeLayout_ALIGN_PARENT_RIGHT] = 0;
                }
                break;
            case 12:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_alignParentBottom*/
                a->GetBoolean(attr, FALSE, &val);
                if (val) {
                    (*rules)[RelativeLayout_ALIGN_PARENT_BOTTOM] = 1;
                }
                else {
                    (*rules)[RelativeLayout_ALIGN_PARENT_BOTTOM] = 0;
                }
                break;
            case 13:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_centerInParent*/
                a->GetBoolean(attr, FALSE, &val);
                if (val) {
                    (*rules)[RelativeLayout_CENTER_IN_PARENT] = 1;
                }
                else {
                    (*rules)[RelativeLayout_CENTER_IN_PARENT] = 0;
                }
                break;
            case 14:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_centerHorizontal*/
                a->GetBoolean(attr, FALSE, &val);
                if (val) {
                    (*rules)[RelativeLayout_CENTER_HORIZONTAL] = 1;
                }
                else {
                    (*rules)[RelativeLayout_CENTER_HORIZONTAL] = 0;
                }
                break;
            case 15:/*com.android.internal.R.styleable.RelativeLayout_Layout_layout_centerVertical*/
                a->GetBoolean(attr, FALSE, &val);
                if (val) {
                    (*rules)[RelativeLayout_CENTER_VERTICAL] = 1;
                }
                else {
                    (*rules)[RelativeLayout_CENTER_VERTICAL] = 0;
                }
                break;
        }
    }

    a->Recycle();

    ArrayOf<Int32>::Free(rules);

    return NOERROR;
}

ECode RelativeLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(width, height));

    return NOERROR;
}

ECode RelativeLayoutLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(source));

    return NOERROR;
}

ECode RelativeLayoutLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(source));

    return NOERROR;
}

ECode RelativeLayoutLayoutParams::AddRule(
    /* [in] */ Int32 verb)
{
    mRules[verb] = TRUE;

    return NOERROR;
}

ECode RelativeLayoutLayoutParams::AddRuleEx(
    /* [in] */ Int32 verb,
    /* [in] */ Int32 anchor)
{
    mRules[verb] = anchor;

    return NOERROR;
}

ArrayOf<Int32>* RelativeLayoutLayoutParams::GetRules()
{
    return &mRules;
}
