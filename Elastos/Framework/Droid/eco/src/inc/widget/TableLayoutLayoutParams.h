
#ifndef __TABLELAYOUTLAYOUTPARAMS_H__
#define __TABLELAYOUTLAYOUTPARAMS_H__

#include "widget/LinearLayoutLayoutParams.h"

/**
 * <p>This set of layout parameters enforces the width of each child to be
 * {@link #MATCH_PARENT} and the height of each child to be
 * {@link #WRAP_CONTENT}, but only if the height is not specified.</p>
 */
class TableLayoutLayoutParams : public LinearLayoutLayoutParams 
{
public:
    /**
     * {@inheritDoc}
     */
    TableLayoutLayoutParams(
        /* [in] */ IContext* c, 
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    TableLayoutLayoutParams(
        /* [in] */ Int32 w, 
        /* [in] */ Int32 h);

    /**
     * {@inheritDoc}
     */
    TableLayoutLayoutParams(
        /* [in] */ Int32 w, 
        /* [in] */ Int32 h, 
        /* [in] */ Float initWeight);

    /**
     * <p>Sets the child width to
     * {@link android.view.ViewGroup.LayoutParams} and the child height to
     * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
     */
    TableLayoutLayoutParams();

    /**
     * {@inheritDoc}
     */
    TableLayoutLayoutParams(
        /* [in] */ ViewGroupLayoutParams* p);

    /**
     * {@inheritDoc}
     */
    TableLayoutLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

    /**
     * <p>Fixes the row's width to
     * {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT}; the row's
     * height is fixed to
     * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT} if no layout
     * height is specified.</p>
     *
     * @param a the styled attributes set
     * @param widthAttr the width attribute to fetch
     * @param heightAttr the height attribute to fetch
     */
protected:
    virtual CARAPI_(void) SetBaseAttributes(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 widthAttr, 
        /* [in] */ Int32 heightAttr);
};

#endif
