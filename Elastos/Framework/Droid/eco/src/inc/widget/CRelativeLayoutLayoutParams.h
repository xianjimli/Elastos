
#ifndef __CRELATIVELAYOUTLAYOUTPARAMS_H__
#define __CRELATIVELAYOUTLAYOUTPARAMS_H__

#include "_CRelativeLayoutLayoutParams.h"
#include "widget/RelativeLayoutLayoutParams.h"

CarClass(CRelativeLayoutLayoutParams), public RelativeLayoutLayoutParams
{
public:
    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI SetMargins(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI GetMargins(
        /* [out] */ Int32* left,
        /* [out] */ Int32* top,
        /* [out] */ Int32* right,
        /* [out] */ Int32* bottom);

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
    CARAPI AddRule(
        /* [in] */ Int32 verb);

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
     CARAPI AddRuleEx(
        /* [in] */ Int32 verb,
        /* [in] */ Int32 anchor);

    using RelativeLayoutLayoutParams::GetRules;

    /**
     * Retrieves a complete list of all supported rules, where the index is the rule
     * verb, and the element value is the value specified, or "false" if it was never
     * set.
     *
     * @return the supported rules
     * @see #addRule(int, int)
     */
    CARAPI GetRules(
        /* [out, callee] */ ArrayOf<Int32>** rules);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CRELATIVELAYOUTLAYOUTPARAMS_H__
