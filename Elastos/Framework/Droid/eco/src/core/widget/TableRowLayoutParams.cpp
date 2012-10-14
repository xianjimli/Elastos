
#include "widget/TableRowLayoutParams.h"
#include <elastos/AutoPtr.h>


const Int32 TableRowLayoutParams::LOCATION;
const Int32 TableRowLayoutParams::LOCATION_NEXT;

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : LinearLayoutLayoutParams(c, attrs)
{
    ASSERT_SUCCEEDED(InitFromAttributes(c, attrs));
}

static Int32 R_Styleable_TableRow_Cell[] = {
    0x0101014c, 0x0101014d
};

ECode TableRowLayoutParams::InitFromAttributes(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    c->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_TableRow_Cell,
                sizeof(R_Styleable_TableRow_Cell) / sizeof(Int32)),
            (ITypedArray**)&a);

    a->GetInt32(0/*com.android.internal.R.styleable.TableRow_Cell_layout_column*/,
            -1, &mColumn);
    a->GetInt32(1/*com.android.internal.R.styleable.TableRow_Cell_layout_span*/,
            1, &mSpan);
    if (mSpan <= 1) {
        mSpan = 1;
    }

    a->Recycle();

    return NOERROR;
}

/**
 * <p>Sets the child width and the child height.</p>
 *
 * @param w the desired width
 * @param h the desired height
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
    : LinearLayoutLayoutParams(w, h)
    , mColumn(-1)
    , mSpan(1)
{
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
    /* [in] */ Float initWeight)
    : LinearLayoutLayoutParams(w, h, initWeight)
    , mColumn(-1)
    , mSpan(1)
{
}

/**
 * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams}
 * and the child height to
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
 */
TableRowLayoutParams::TableRowLayoutParams()
//    : LinearLayoutLayoutParams(MATCH_PARENT, WRAP_CONTENT)
//    , mColumn(-1)
//    , mSpan(1)
{
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
    /* [in] */ Int32 column)
    : LinearLayoutLayoutParams(MATCH_PARENT, WRAP_CONTENT)
    , mColumn(column)
    , mSpan(1)
{
}

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ ViewGroupLayoutParams* p)
    : LinearLayoutLayoutParams(p)
{
}

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : LinearLayoutLayoutParams(source)
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
    }
    else {
        mWidth = MATCH_PARENT;
    }

    // We don't want to force users to specify a layout_height
    a->HasValue(heightAttr, &res);
    if (res) {
        a->GetLayoutDimension(heightAttr, String("layout_height"), &mHeight);
    }
    else {
        mHeight = WRAP_CONTENT;
    }
}

ECode TableRowLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(LinearLayoutLayoutParams::Init(c, attrs));
    return InitFromAttributes(c, attrs);
}

ECode TableRowLayoutParams::Init(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    FAIL_RETURN(LinearLayoutLayoutParams::Init(w, h));
    mColumn = -1;
    mSpan = -1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Float initWeight)
{
    FAIL_RETURN(LinearLayoutLayoutParams::Init(w, h, initWeight));
    mColumn = -1;
    mSpan = -1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init()
{
    FAIL_RETURN(LinearLayoutLayoutParams::Init(MATCH_PARENT, WRAP_CONTENT));
    mColumn = -1;
    mSpan = -1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init(
    /* [in] */ Int32 column)
{
    FAIL_RETURN(LinearLayoutLayoutParams::Init(MATCH_PARENT, WRAP_CONTENT));
    mColumn = column;
    mSpan = -1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return LinearLayoutLayoutParams::Init(p);
}

ECode TableRowLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return LinearLayoutLayoutParams::Init(source);
}
