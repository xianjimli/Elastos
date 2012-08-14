
#include "widget/TableLayoutLayoutParams.h"

/**
 * {@inheritDoc}
 */
TableLayoutLayoutParams::TableLayoutLayoutParams(
    /* [in] */ IContext* c, 
    /* [in] */ IAttributeSet* attrs) : LinearLayoutLayoutParams(c, attrs)
{
}

/**
 * {@inheritDoc}
 */
TableLayoutLayoutParams::TableLayoutLayoutParams(
    /* [in] */ Int32 w, 
    /* [in] */ Int32 h) : LinearLayoutLayoutParams(w, h)
{
}

/**
 * {@inheritDoc}
 */
TableLayoutLayoutParams::TableLayoutLayoutParams(
    /* [in] */ Int32 w, 
    /* [in] */ Int32 h, 
    /* [in] */ Float initWeight) : LinearLayoutLayoutParams(LinearLayoutLayoutParams::MATCH_PARENT, h, initWeight) 
{
}

/**
 * <p>Sets the child width to
 * {@link android.view.ViewGroup.LayoutParams} and the child height to
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
 */
TableLayoutLayoutParams::TableLayoutLayoutParams() : LinearLayoutLayoutParams(LinearLayoutLayoutParams::MATCH_PARENT, LinearLayoutLayoutParams::WRAP_CONTENT) 
{
}

/**
 * {@inheritDoc}
 */
TableLayoutLayoutParams::TableLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* p) : LinearLayoutLayoutParams(p)
{
}

/**
 * {@inheritDoc}
 */
TableLayoutLayoutParams::TableLayoutLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source) : LinearLayoutLayoutParams(source)
{
}

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
void TableLayoutLayoutParams::SetBaseAttributes(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 widthAttr, 
    /* [in] */ Int32 heightAttr) 
{
    mWidth = MATCH_PARENT;
    
    Boolean res;
    a->HasValue(heightAttr, &res);
    if (res) {
        a->GetLayoutDimension(heightAttr, String("layout_height"), &mHeight);
    } else {
        mHeight = LinearLayoutLayoutParams::WRAP_CONTENT;
    }
}