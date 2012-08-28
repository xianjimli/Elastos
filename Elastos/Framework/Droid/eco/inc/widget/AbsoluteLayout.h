
#ifndef __ABSOLUTELAYOUT_H__
#define __ABSOLUTELAYOUT_H__

#include "view/ViewGroup.h"
/**
 * A layout that lets you specify exact locations (x/y coordinates) of its
 * children. Absolute layouts are less flexible and harder to maintain than
 * other types of layouts without absolute positioning.
 *
 * <p><strong>XML attributes</strong></p> <p> See {@link
 * android.R.styleable#ViewGroup ViewGroup Attributes}, {@link
 * android.R.styleable#View View Attributes}</p>
 *
 * @deprecated Use {@link android.widget.FrameLayout}, {@link android.widget.RelativeLayout}
 *             or a custom layout instead.
 */
class AbsoluteLayout : public ViewGroup
{
public:
    AbsoluteLayout(
        /* [in] */ IContext* context);

    AbsoluteLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AbsoluteLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);

protected:
    AbsoluteLayout();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    /**
     * Returns a set of layout parameters with a width of
     * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT},
     * a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}
     * and with the coordinates (0, 0).
     */
    //@Override
    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** lp);

    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    // Override to allow type-checking of LayoutParams.
    //@Override
    CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);
};

#endif

