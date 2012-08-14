
#include "widget/TableRow.h"
#include "widget/CTableRowLayoutParams.h"
#include "view/CGravity.h"
#include <elastos/Math.h>

TableRow::TableRow()
{

}

/**
 * <p>Creates a new TableRow for the given context.</p>
 *
 * @param context the application environment
 */
TableRow::TableRow(
    /* [in] */ IContext* context) : LinearLayout(context)
{
    InitTableRow();
}

/**
 * <p>Creates a new TableRow for the given context and with the
 * specified set attributes.</p>
 *
 * @param context the application environment
 * @param attrs a collection of attributes
 */
TableRow::TableRow(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs) : LinearLayout(context, attrs)
{
    InitTableRow();
}

void TableRow::InitTableRow()
{
    AutoPtr<IViewGroupOnHierarchyChangeListener> oldListener = mOnHierarchyChangeListener;
    //mChildrenTracker = new ChildrenTracker();
    if (oldListener != NULL) {
        mChildrenTracker->SetOnHierarchyChangeListener(oldListener);
    }
    LinearLayout::SetOnHierarchyChangeListener(mChildrenTracker);
}

/**
 * {@inheritDoc}
 */
ECode TableRow::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    mChildrenTracker->SetOnHierarchyChangeListener(listener);

    return NOERROR;
}

/**
 * <p>Collapses or restores a given column.</p>
 *
 * @param columnIndex the index of the column
 * @param collapsed true if the column must be collapsed, false otherwise
 * {@hide}
 */
ECode TableRow::SetColumnCollapsed(
    /* [in] */ Int32 columnIndex, 
    /* [in] */ Boolean collapsed)
{
    AutoPtr<IView> child = GetVirtualChildAt(columnIndex);
    if (child.Get() != NULL) {
        child->SetVisibility(collapsed ? GONE : VISIBLE);
    }

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
void TableRow::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec, 
    /* [in] */ Int32 heightMeasureSpec)
{
    // enforce horizontal layout
    MeasureHorizontal(widthMeasureSpec, heightMeasureSpec);
}

/**
 * {@inheritDoc}
 */
void TableRow::OnLayout(
    /* [in] */ Boolean changed, 
    /* [in] */ Int32 l, 
    /* [in] */ Int32 t, 
    /* [in] */ Int32 r, 
    /* [in] */ Int32 b) 
{
    // enforce horizontal layout
    LayoutHorizontal();
}

/**
 * {@inheritDoc}
 */
AutoPtr<IView> TableRow::GetVirtualChildAt(
    /* [in] */ Int32 i) 
{
    if (mColumnToChildIndex == NULL) {
        MapIndexAndColumns();
    }

    Int32 deflectedIndex = mColumnToChildIndex->Get(i, -1);
    if (deflectedIndex != -1) {
        return GetChildAt(deflectedIndex);
    }

    return NULL;
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::GetVirtualChildCount()
{
    if (mColumnToChildIndex == NULL) {
        MapIndexAndColumns();
    }
    return mNumColumns;
}

void TableRow::MapIndexAndColumns() 
{
    if (mColumnToChildIndex == NULL) {
        Int32 virtualCount = 0;
        Int32 count = GetChildCount();

        mColumnToChildIndex = new SparseInt32Array();
        SparseInt32Array* columnToChild = mColumnToChildIndex;

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            AutoPtr<ITableRowLayoutParams> layoutParams;
            child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);

            if (((CTableRowLayoutParams*)layoutParams.Get())->mColumn >= virtualCount) {
                virtualCount = ((CTableRowLayoutParams*)layoutParams.Get())->mColumn;
            }

            for (Int32 j = 0; j < ((CTableRowLayoutParams*)layoutParams.Get())->mSpan; j++) {
                columnToChild->Put(virtualCount++, i);
            }
        }

        mNumColumns = virtualCount;
    }
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::MeasureNullChild(
    /* [in] */ Int32 childIndex) 
{
    return (*mConstrainedColumnWidths)[childIndex];
}

/**
 * {@inheritDoc}
 */
void TableRow::MeasureChildBeforeLayout(
    /* [in] */ IView* child, 
    /* [in] */ Int32 childIndex,
    /* [in] */ Int32 widthMeasureSpec, 
    /* [in] */ Int32 totalWidth,
    /* [in] */ Int32 heightMeasureSpec, 
    /* [in] */ Int32 totalHeight) 
{
    if (mConstrainedColumnWidths != NULL) {
        AutoPtr<ITableRowLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

        Int32 measureMode = MeasureSpec::EXACTLY;
        Int32 columnWidth = 0;

        Int32 span = ((CTableRowLayoutParams*)lp.Get())->mSpan;
        ArrayOf<Int32>* constrainedColumnWidths = mConstrainedColumnWidths;
        for (Int32 i = 0; i < span; i++) {
            columnWidth += (*constrainedColumnWidths)[childIndex + i];
        }

        Int32 gravity = ((CTableRowLayoutParams*)lp.Get())->mGravity;
        Boolean isHorizontalGravity;
        AutoPtr<IGravity> g;
        CGravity::AcquireSingleton((IGravity**)&g);
        g->IsHorizontal(gravity, &isHorizontalGravity);

        if (isHorizontalGravity) {
            measureMode = MeasureSpec::AT_MOST;
        }

        // no need to care about padding here,
        // ViewGroup.getChildMeasureSpec() would get rid of it anyway
        // because of the EXACTLY measure spec we use
        Int32 childWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(
            Math::Max(0, columnWidth - ((CTableRowLayoutParams*)lp.Get())->mLeftMargin - ((CTableRowLayoutParams*)lp.Get())->mRightMargin), measureMode
        );
        Int32 childHeightMeasureSpec = GetChildMeasureSpec(heightMeasureSpec,
                mPaddingTop + mPaddingBottom + ((CTableRowLayoutParams*)lp.Get())->mTopMargin +
                ((CTableRowLayoutParams*)lp.Get())->mBottomMargin + totalHeight, ((CTableRowLayoutParams*)lp.Get())->mHeight);

        child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);

        if (isHorizontalGravity) {
            Int32 childWidth;
            child->GetMeasuredWidth(&childWidth);
            (*((CTableRowLayoutParams*)lp.Get())->mOffset)[TableRowLayoutParams::LOCATION_NEXT] = columnWidth - childWidth;

            switch (gravity & Gravity_HORIZONTAL_GRAVITY_MASK) {
                case Gravity_LEFT:
                    // don't offset on X axis
                    break;
                case Gravity_RIGHT:
                    (*((CTableRowLayoutParams*)lp.Get())->mOffset)[TableRowLayoutParams::LOCATION] = (*((CTableRowLayoutParams*)lp.Get())->mOffset)[TableRowLayoutParams::LOCATION_NEXT];
                    break;
                case Gravity_CENTER_HORIZONTAL:
                    (*((CTableRowLayoutParams*)lp.Get())->mOffset)[TableRowLayoutParams::LOCATION] = (*((CTableRowLayoutParams*)lp.Get())->mOffset)[TableRowLayoutParams::LOCATION_NEXT] / 2;
                    break;
            }
        } else {
            (*((CTableRowLayoutParams*)lp.Get())->mOffset)[TableRowLayoutParams::LOCATION] = (*((CTableRowLayoutParams*)lp.Get())->mOffset)[TableRowLayoutParams::LOCATION_NEXT] = 0;
        }
    } else {
        // fail silently when column widths are not available
        LinearLayout::MeasureChildBeforeLayout(child, childIndex, widthMeasureSpec,
                totalWidth, heightMeasureSpec, totalHeight);
    }
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::GetChildrenSkipCount(
    /* [in] */ IView* child, 
    /* [in] */ Int32 index)
{
    AutoPtr<ITableRowLayoutParams> layoutParams;
    child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);

    // when the span is 1 (default), we need to skip 0 child
    return ((CTableRowLayoutParams*)layoutParams.Get())->mSpan - 1;
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::GetLocationOffset(
    /* [in] */ IView* child)
{
    AutoPtr<ITableRowLayoutParams> layoutParams;
    child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);

    return (*((CTableRowLayoutParams*) layoutParams.Get())->mOffset)[TableRowLayoutParams::LOCATION];
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::GetNextLocationOffset(
    /* [in] */ IView* child)
{
    AutoPtr<ITableRowLayoutParams> layoutParams;
    child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);

    return (*((CTableRowLayoutParams*) layoutParams.Get())->mOffset)[TableRowLayoutParams::LOCATION_NEXT];
}

/**
 * <p>Measures the preferred width of each child, including its margins.</p>
 *
 * @param widthMeasureSpec the width constraint imposed by our parent
 *
 * @return an array of integers corresponding to the width of each cell, or
 *         column, in this row
 * {@hide}
 */
ArrayOf<Int32>* TableRow::GetColumnsWidths(
    /* [in] */ Int32 widthMeasureSpec)
{
    Int32 numColumns = GetVirtualChildCount();
    if (mColumnWidths == NULL || numColumns != mColumnWidths->GetLength()) {
        //mColumnWidths = new Int32[numColumns];
        mColumnWidths = ArrayOf<Int32>::Alloc(numColumns);
    }

    ArrayOf<Int32>* columnWidths = mColumnWidths;

    for (Int32 i = 0; i < numColumns; i++) {
        AutoPtr<IView> child = GetVirtualChildAt(i);
        Int32 visible;
        child->GetVisibility(&visible);

        if (child.Get() != NULL && visible != GONE) {
            AutoPtr<ITableRowLayoutParams> layoutParams;
            child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
            CTableRowLayoutParams* lp = (CTableRowLayoutParams*)layoutParams.Get();

            if (lp->mSpan == 1) {
                Int32 spec;
                switch (lp->mWidth) {
                    case TableRowLayoutParams::WRAP_CONTENT:
                        spec = GetChildMeasureSpec(widthMeasureSpec, 0, TableRowLayoutParams::WRAP_CONTENT);
                        break;
                    case TableRowLayoutParams::MATCH_PARENT:
                        spec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);
                        break;
                    default:
                        spec = MeasureSpec::MakeMeasureSpec(lp->mWidth, MeasureSpec::EXACTLY);
                }
                child->Measure(spec, spec);

                Int32 width;
                child->GetMeasuredWidth(&width);

                (*columnWidths)[i] = width + lp->mLeftMargin + lp->mRightMargin;
            } else {
                (*columnWidths)[i] = 0;
            }
        } else {
            (*columnWidths)[i] = 0;
        }
    }

    return columnWidths;
}

/**
 * <p>Sets the width of all of the columns in this row. At layout time,
 * this row sets a fixed width, as defined by <code>columnWidths</code>,
 * on each child (or cell, or column.)</p>
 *
 * @param columnWidths the fixed width of each column that this row must
 *                     honor
 * @throws IllegalArgumentException when columnWidths' length is smaller
 *         than the number of children in this row
 * {@hide}
 */
ECode TableRow::SetColumnsWidthConstraints(
    /* [in] */ ArrayOf<Int32>* columnWidths)
{
    if (columnWidths == NULL || columnWidths->GetLength() < GetVirtualChildCount()) {
        /*throw new IllegalArgumentException(
                "columnWidths should be >= getVirtualChildCount()");*/
    }

    mConstrainedColumnWidths = columnWidths;

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
AutoPtr<IViewGroupLayoutParams> TableRow::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    CTableRowLayoutParams::New(GetContext(), attrs, (ITableRowLayoutParams**)&lp);
    return lp;
}

/**
 * Returns a set of layout parameters with a width of
 * {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT},
 * a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT} and no spanning.
 */
AutoPtr<ILinearLayoutLayoutParams> TableRow::GenerateDefaultLayoutParams()
{
    AutoPtr<ITableRowLayoutParams> lp;
    CTableRowLayoutParams::New((ITableRowLayoutParams**)&lp);
    return (ILinearLayoutLayoutParams*)lp.Get();
}

/**
 * {@inheritDoc}
 */
Boolean TableRow::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p->Probe(EIID_ITableRowLayoutParams) != NULL;
}

/**
 * {@inheritDoc}
 */
AutoPtr<IViewGroupLayoutParams> TableRow::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    CTableRowLayoutParams::New((ITableRowLayoutParams**)&lp);
    return lp;
}

void TableRow::ChildrenTracker::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    mListener = listener;
}

ECode TableRow::ChildrenTracker::OnChildViewAdded(
    /* [in] */ IView* parent, 
    /* [in] */ IView* child)
{
    // dirties the index to column map
    mOwner->mColumnToChildIndex = NULL;

    if (mListener.Get() != NULL) {
        mListener->OnChildViewAdded(parent, child);
    }

    return NOERROR;
}

ECode TableRow::ChildrenTracker::OnChildViewRemoved(
    /* [in] */ IView* parent, 
    /* [in] */ IView* child)
{
    // dirties the index to column map
    mOwner->mColumnToChildIndex = NULL;

    if (mListener.Get() != NULL) {
        mListener->OnChildViewRemoved(parent, child);
    }

    return NOERROR;
}

