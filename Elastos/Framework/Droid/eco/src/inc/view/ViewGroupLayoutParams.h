
#ifndef __VIEWGROUPLAYOUTPARAMS_H__
#define __VIEWGROUPLAYOUTPARAMS_H__

#include "ext/frameworkext.h"
#include "view/animation/LayoutAnimationController.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class ViewGroupLayoutParams
{
public:
    /**
     * Special value for the height or width requested by a View.
     * FILL_PARENT means that the view wants to be as big as its parent,
     * minus the parent's padding, if any. This value is deprecated
     * starting in API Level 8 and replaced by {@link #MATCH_PARENT}.
     */
    static const Int32 FILL_PARENT = -1;

    /**
     * Special value for the height or width requested by a View.
     * MATCH_PARENT means that the view wants to be as big as its parent,
     * minus the parent's padding, if any. Introduced in API Level 8.
     */
    static const Int32 MATCH_PARENT = -1;

    /**
     * Special value for the height or width requested by a View.
     * WRAP_CONTENT means that the view wants to be just large enough to fit
     * its own internal content, taking its own padding into account.
     */
    static const Int32 WRAP_CONTENT = -2;

public:
    /**
     * Creates a new set of layout parameters. The values are extracted from
     * the supplied attributes set and context. The XML attributes mapped
     * to this set of layout parameters are:
     *
     * <ul>
     *   <li><code>layout_width</code>: the width, either an exact value,
     *   {@link #WRAP_CONTENT}, or {@link #FILL_PARENT} (replaced by
     *   {@link #MATCH_PARENT} in API Level 8)</li>
     *   <li><code>layout_height</code>: the height, either an exact value,
     *   {@link #WRAP_CONTENT}, or {@link #FILL_PARENT} (replaced by
     *   {@link #MATCH_PARENT} in API Level 8)</li>
     * </ul>
     *
     * @param c the application environment
     * @param attrs the set of attributes from which to extract the layout
     *              parameters' values
     */
    ViewGroupLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Creates a new set of layout parameters with the specified width
     * and height.
     *
     * @param width the width, either {@link #WRAP_CONTENT},
     *        {@link #FILL_PARENT} (replaced by {@link #MATCH_PARENT} in
     *        API Level 8), or a fixed size in pixels
     * @param height the height, either {@link #WRAP_CONTENT},
     *        {@link #FILL_PARENT} (replaced by {@link #MATCH_PARENT} in
     *        API Level 8), or a fixed size in pixels
     */
    ViewGroupLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Copy constructor. Clones the width and height values of the source.
     *
     * @param source The layout params to copy from.
     */
    ViewGroupLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    /**
     * Used internally by MarginLayoutParams.
     * @hide
     */
    ViewGroupLayoutParams();

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

protected:
    /**
     * Extracts the layout parameters from the supplied attributes.
     *
     * @param a the style attributes to extract the parameters from
     * @param widthAttr the identifier of the width attribute
     * @param heightAttr the identifier of the height attribute
     */
    CARAPI_(void) SetBaseAttributes(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 widthAttr,
        /* [in] */ Int32 heightAttr);

    /**
     * Converts the specified size to a readable String.
     *
     * @param size the size to convert
     * @return a String instance representing the supplied size
     *
     * @hide
     */
    static CARAPI SizeToString(
        /* [in] */ Int32 size,
        /* [out] */ String* des);

public:
    /**
     * Information about how wide the view wants to be. Can be one of the
     * constants FILL_PARENT (replaced by MATCH_PARENT ,
     * in API Level 8) or WRAP_CONTENT. or an exact size.
     */
    Int32 mWidth;

    /**
     * Information about how tall the view wants to be. Can be one of the
     * constants FILL_PARENT (replaced by MATCH_PARENT ,
     * in API Level 8) or WRAP_CONTENT. or an exact size.
     */
    Int32 mHeight;

    /**
     * Used to animate layouts.
     */
    AutoPtr<LayoutAnimationController::AnimationParameters> mLayoutAnimationParameters;
};

#endif //__VIEWGROUPLAYOUTPARAMS_H__
