
#ifndef __FRAMELAYOUTLAYOUTPARAMS_H__
#define __FRAMELAYOUTLAYOUTPARAMS_H__


#include <elastos.h>
#include "view/ViewGroupMarginLayoutParams.h"

using namespace Elastos;

class FrameLayoutLayoutParams : public ViewGroupMarginLayoutParams
{
public:
    FrameLayoutLayoutParams();

    /**
     * {@inheritDoc}
     */
    FrameLayoutLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    FrameLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Creates a new set of layout parameters with the specified width, height
     * and weight.
     *
     * @param width the width, either {@link #MATCH_PARENT},
     *        {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param height the height, either {@link #MATCH_PARENT},
     *        {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param gravity the gravity
     *
     * @see android.view.Gravity
     */
    FrameLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 gravity);

    /**
     * {@inheritDoc}
     */
    FrameLayoutLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    /**
     * {@inheritDoc}
     */
    FrameLayoutLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 gravity);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* source);

public:
    /**
     * Gravity for the view associated with these LayoutParams.
     *
     * @see android.view.Gravity
     */
    Int32 mGravity;

};


#endif // __FRAMELAYOUTLAYOUTPARAMS_H__
