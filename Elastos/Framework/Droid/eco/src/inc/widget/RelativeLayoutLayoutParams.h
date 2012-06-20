
#ifndef __RELATIVELAYOUTLAYOUTPARAMS_H__
#define __RELATIVELAYOUTLAYOUTPARAMS_H__


#include <elastos.h>
#include "view/ViewGroupMarginLayoutParams.h"

using namespace Elastos;

class RelativeLayoutLayoutParams : public ViewGroupMarginLayoutParams
{
public:
    RelativeLayoutLayoutParams();

    /**
     * {@inheritDoc}
     */
    RelativeLayoutLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    RelativeLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * {@inheritDoc}
     */
    RelativeLayoutLayoutParams(
        /* [in] */ ViewGroupLayoutParams* p);

    /**
     * {@inheritDoc}
     */
    RelativeLayoutLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI AddRule(
        /* [in] */ Int32 verb);

    CARAPI AddRuleEx(
        /* [in] */ Int32 verb,
        /* [in] */ Int32 anchor);

    CARAPI_(ArrayOf<Int32>*) GetRules();

private:
    const static Int32 VERB_COUNT = 16;

public:
    /**
     * When true, uses the parent as the anchor if the anchor doesn't exist or if
     * the anchor's visibility is GONE.
     */
    Boolean mAlignWithParent;
    ArrayOf_<Int32, VERB_COUNT> mRules;

    Int32 mLeft;
    Int32 mTop;
    Int32 mRight;
    Int32 mBottom;

};


#endif // __RelativeLayoutLayoutParams_H__
