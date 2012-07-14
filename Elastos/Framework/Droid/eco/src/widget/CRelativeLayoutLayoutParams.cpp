
#include "ext/frameworkdef.h"
#include "widget/CRelativeLayoutLayoutParams.h"

ECode CRelativeLayoutLayoutParams::GetDescription(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRelativeLayoutLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;
    return NOERROR;
}

ECode CRelativeLayoutLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;
    return NOERROR;
}

ECode CRelativeLayoutLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth;
    return NOERROR;
}

ECode CRelativeLayoutLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight;
    return NOERROR;
}

ECode CRelativeLayoutLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return RelativeLayoutLayoutParams::SetMargins(left, top, right, bottom);
}

ECode CRelativeLayoutLayoutParams::GetMargins(
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

/**
 * Adds a layout rule to be interpreted by the RelativeLayout. This
 * method should only be used for constraints that don't refer to another sibling
 * (e.g., CENTER_IN_PARENT) or take a boolean value ({@link RelativeLayout#TRUE}
 * for true or - for false). To specify a verb that takes a subject, use
 * {@link #addRule(int, int)} instead.
 *
 * @param verb One of the verbs defined by
 *        {@link android.widget.RelativeLayout RelativeLayout}, such as
 *        ALIGN_WITH_PARENT_LEFT.
 * @see #addRule(int, int)
 */
ECode CRelativeLayoutLayoutParams::AddRule(
    /* [in] */ Int32 verb)
{
    return RelativeLayoutLayoutParams::AddRule(verb);
}

/**
 * Adds a layout rule to be interpreted by the RelativeLayout. Use this for
 * verbs that take a target, such as a sibling (ALIGN_RIGHT) or a boolean
 * value (VISIBLE).
 *
 * @param verb One of the verbs defined by
 *        {@link android.widget.RelativeLayout RelativeLayout}, such as
 *         ALIGN_WITH_PARENT_LEFT.
 * @param anchor The id of another view to use as an anchor,
 *        or a boolean value(represented as {@link RelativeLayout#TRUE})
 *        for true or 0 for false).  For verbs that don't refer to another sibling
 *        (for example, ALIGN_WITH_PARENT_BOTTOM) just use -1.
 * @see #addRule(int)
 */
ECode CRelativeLayoutLayoutParams::AddRuleEx(
    /* [in] */ Int32 verb,
    /* [in] */ Int32 anchor)
{
    return RelativeLayoutLayoutParams::AddRuleEx(verb, anchor);
}

/**
 * Retrieves a complete list of all supported rules, where the index is the rule
 * verb, and the element value is the value specified, or "false" if it was never
 * set.
 *
 * @return the supported rules
 * @see #addRule(int, int)
 */
ECode CRelativeLayoutLayoutParams::GetRules(
    /* [out, callee] */ ArrayOf<Int32>** rules)
{
    VALIDATE_NOT_NULL(rules);

    *rules = RelativeLayoutLayoutParams::GetRules()->Clone();
    return NOERROR;
}

ECode CRelativeLayoutLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return RelativeLayoutLayoutParams::Init(c, attrs);
}

ECode CRelativeLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return RelativeLayoutLayoutParams::Init(width, height);
}

ECode CRelativeLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return RelativeLayoutLayoutParams::Init(p);
}

ECode CRelativeLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return RelativeLayoutLayoutParams::Init(source);
}
