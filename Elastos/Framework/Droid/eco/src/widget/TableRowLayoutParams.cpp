
#include "widget/TableRowLayoutParams.h"
#include <elastos/AutoPtr.h>

/**
 * <p>Set of layout parameters used in table rows.</p>
 *
 * @see android.widget.TableLayout.LayoutParams
 * 
 * @attr ref android.R.styleable#TableRow_Cell_layout_column
 * @attr ref android.R.styleable#TableRow_Cell_layout_span
 */

const Int32 TableRowLayoutParams::LOCATION;
const Int32 TableRowLayoutParams::LOCATION_NEXT;

static Int32 R_Styleable_TableRow_Cell[] = {
    0x0101014c, 0x0101014d
};

static const Int32 R_Styleable_TableRow_Cell_layout_column = 0;
static const Int32 R_Styleable_TableRow_Cell_layout_span = 1;

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ IContext* c, 
    /* [in] */ IAttributeSet* attrs) : LinearLayoutLayoutParams(c, attrs)
{
    AutoPtr<ITypedArray> a;
    c->ObtainStyledAttributesEx2(attrs,
                    ArrayOf<Int32>(R_Styleable_TableRow_Cell, 2), (ITypedArray**)&a);

    a->GetInt32(R_Styleable_TableRow_Cell_layout_column, -1, &mColumn);
    a->GetInt32(R_Styleable_TableRow_Cell_layout_span, 1, &mSpan);
    if (mSpan <= 1) {
        mSpan = 1;
    }

    a->Recycle();
}

/**
 * <p>Sets the child width and the child height.</p>
 *
 * @param w the desired width
 * @param h the desired height
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ Int32 w, 
    /* [in] */ Int32 h) : LinearLayoutLayoutParams(w, h) 
{
    mColumn = -1;
    mSpan = 1;
}

/**
 * <p>Sets the child width, height and weight.</p>
 *
 * @param w the desired width
 * @param h the desired height
 * @param initWeight the desired weight
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ Int32 w, 
    /* [in] */ Int32 h, 
    /* [in] */ Float initWeight) : LinearLayoutLayoutParams(w, h, initWeight)
{
    mColumn = -1;
    mSpan = 1;
}

/**
 * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams}
 * and the child height to
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
 */
TableRowLayoutParams::TableRowLayoutParams() : LinearLayoutLayoutParams(MATCH_PARENT, WRAP_CONTENT)
{
    mColumn = -1;
    mSpan = 1;
}

/**
 * <p>Puts the view in the specified column.</p>
 *
 * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT}
 * and the child height to
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
 *
 * @param column the column index for the view
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ Int32 column) : LinearLayoutLayoutParams(MATCH_PARENT, WRAP_CONTENT)
{
    mSpan = 1;
    mColumn = column;
}

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ ViewGroupLayoutParams* p) : LinearLayoutLayoutParams(p)
{
}

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source) : LinearLayoutLayoutParams(source)
{
}

void TableRowLayoutParams::SetBaseAttributes(
    /* [in] */ ITypedArray* a, 
    /* [in] */ Int32 widthAttr, 
    /* [in] */ Int32 heightAttr) 
{
    // We don't want to force users to specify a layout_width
    Boolean res;
    a->HasValue(widthAttr, &res);

    if (res) {
        a->GetLayoutDimension(widthAttr, String("layout_width"), &mWidth);
    } else {
        mWidth = MATCH_PARENT;
    }

    // We don't want to force users to specify a layout_height
    a->HasValue(heightAttr, &res);
    if (res) {
        a->GetLayoutDimension(heightAttr, String("layout_height"), &mHeight);
    } else {
        mHeight = WRAP_CONTENT;
    }
}
