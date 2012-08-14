
#ifndef __TABLEROWLAYOUTPARAMS_H__
#define __TABLEROWLAYOUTPARAMS_H__

#include "widget/LinearLayoutLayoutParams.h"

/**
 * <p>Set of layout parameters used in table rows.</p>
 *
 * @see android.widget.TableLayout.LayoutParams
 * 
 * @attr ref android.R.styleable#TableRow_Cell_layout_column
 * @attr ref android.R.styleable#TableRow_Cell_layout_span
 */
class TableRowLayoutParams : public LinearLayoutLayoutParams
{
public:
    /**
     * {@inheritDoc}
     */
    TableRowLayoutParams(
        /* [in] */ IContext* c, 
        /* [in] */ IAttributeSet* attrs);

    /**
     * <p>Sets the child width and the child height.</p>
     *
     * @param w the desired width
     * @param h the desired height
     */
    TableRowLayoutParams(
        /* [in] */ Int32 w, 
        /* [in] */ Int32 h);

    /**
     * <p>Sets the child width, height and weight.</p>
     *
     * @param w the desired width
     * @param h the desired height
     * @param initWeight the desired weight
     */
    TableRowLayoutParams(
        /* [in] */ Int32 w, 
        /* [in] */ Int32 h, 
        /* [in] */ Float initWeight);

    /**
     * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams}
     * and the child height to
     * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
     */
    TableRowLayoutParams();

    /**
     * <p>Puts the view in the specified column.</p>
     *
     * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT}
     * and the child height to
     * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
     *
     * @param column the column index for the view
     */
    TableRowLayoutParams(
        /* [in] */ Int32 column);

    /**
     * {@inheritDoc}
     */
    TableRowLayoutParams(
        /* [in] */ ViewGroupLayoutParams* p);

    /**
     * {@inheritDoc}
     */
    TableRowLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

protected:
    virtual CARAPI_(void) SetBaseAttributes(
        /* [in] */ ITypedArray* a, 
        /* [in] */ Int32 widthAttr, 
        /* [in] */ Int32 heightAttr);

public:
    /**
     * <p>The column index of the cell represented by the widget.</p>
     */
    Int32 mColumn;

    /**
     * <p>The number of columns the widgets spans over.</p>
     */
    Int32 mSpan;

    static const Int32 LOCATION = 0;
    static const Int32 LOCATION_NEXT = 1;

    ArrayOf<Int32>* mOffset;// = new Int32[2];
};

#endif