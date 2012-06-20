
#ifndef __VIEWGROUPMARGINLAYOUTPARAMS_H__
#define __VIEWGROUPMARGINLAYOUTPARAMS_H__


#include <elastos.h>
#include "view/ViewGroupLayoutParams.h"

using namespace Elastos;

class ViewGroupMarginLayoutParams : public ViewGroupLayoutParams
{
public:
    ViewGroupMarginLayoutParams();

    /**
     * Creates a new set of layout parameters. The values are extracted from
     * the supplied attributes set and context.
     *
     * @param c the application environment
     * @param attrs the set of attributes from which to extract the layout
     *              parameters' values
     */
    ViewGroupMarginLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    ViewGroupMarginLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Copy constructor. Clones the width, height and margin values of the source.
     *
     * @param source The layout params to copy from.
     */
    ViewGroupMarginLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

    /**
     * {@inheritDoc}
     */
    ViewGroupMarginLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

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
    CARAPI SetMargins(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

public:
    /**
     * The left margin in pixels of the child.
     */
    Int32 mLeftMargin;

    /**
     * The top margin in pixels of the child.
     */
    Int32 mTopMargin;

    /**
     * The right margin in pixels of the child.
     */
    Int32 mRightMargin;

    /**
     * The bottom margin in pixels of the child.
     */
    Int32 mBottomMargin;

};


#endif // __VIEWGROUPMARGINLAYOUTPARAMS_H__
