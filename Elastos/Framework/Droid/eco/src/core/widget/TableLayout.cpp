
#include "widget/TableLayout.h"
#include "widget/TableLayoutLayoutParams.h"
#include "widget/CTableLayoutLayoutParams.h"
#include "widget/TableRow.h"
#include <elastos/Math.h>


TableLayout::PassThroughHierarchyChangeListener::PassThroughHierarchyChangeListener(
    /* [in] */ TableLayout* owner)
    : mOwner(owner)
{
}

PInterface TableLayout::PassThroughHierarchyChangeListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IViewGroupOnHierarchyChangeListener) {
        return (IViewGroupOnHierarchyChangeListener*)this;
    }

    return NULL;
}

UInt32 TableLayout::PassThroughHierarchyChangeListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TableLayout::PassThroughHierarchyChangeListener::Release()
{
    return ElRefBase::Release();
}

ECode TableLayout::PassThroughHierarchyChangeListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pObject == (IInterface*)this) {
        *pIID = EIID_IViewGroupOnHierarchyChangeListener;
        return NOERROR;
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

/**
 * {@inheritDoc}
 */
ECode TableLayout::PassThroughHierarchyChangeListener::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    mOwner->TrackCollapsedColumns(child);

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewAdded(parent, child);
    }

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
ECode TableLayout::PassThroughHierarchyChangeListener::OnChildViewRemoved(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewRemoved(parent, child);
    }

    return NOERROR;
}

TableLayout::TableLayout()
    : mMaxWidths(NULL)
    , mStretchableColumns(NULL)
    , mShrinkableColumns(NULL)
    , mCollapsedColumns(NULL)
    , mShrinkAllColumns(FALSE)
    , mStretchAllColumns(FALSE)
    , mInitialized(FALSE)
{}

/**
 * <p>Creates a new TableLayout for the given context.</p>
 *
 * @param context the application environment
 */
TableLayout::TableLayout(
    /* [in] */ IContext* context)
    : LinearLayout(context)
    , mMaxWidths(NULL)
    , mStretchableColumns(NULL)
    , mShrinkableColumns(NULL)
    , mCollapsedColumns(NULL)
    , mShrinkAllColumns(FALSE)
    , mStretchAllColumns(FALSE)
    , mInitialized(FALSE)
{
    InitTableLayout();
}

/**
 * <p>Creates a new TableLayout for the given context and with the
 * specified set attributes.</p>
 *
 * @param context the application environment
 * @param attrs a collection of attributes
 */
TableLayout::TableLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : LinearLayout(context, attrs)
    , mMaxWidths(NULL)
    , mStretchableColumns(NULL)
    , mShrinkableColumns(NULL)
    , mCollapsedColumns(NULL)
    , mShrinkAllColumns(FALSE)
    , mStretchAllColumns(FALSE)
    , mInitialized(FALSE)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
    InitTableLayout();
}

TableLayout::~TableLayout()
{
    if (mMaxWidths != NULL) {
        ArrayOf<Int32>::Free(mMaxWidths);
    }
    if (mStretchableColumns != NULL) {
        mStretchableColumns->Clear();
        delete mStretchableColumns;
    }
    if (mShrinkableColumns != NULL) {
        mShrinkableColumns->Clear();
        delete mShrinkableColumns;
    }
    if (mCollapsedColumns != NULL) {
        mCollapsedColumns->Clear();
        delete mCollapsedColumns;
    }
}

static Int32 R_Styleable_TableLayout[] = {
    0x01010149, 0x0101014a, 0x0101014b
};

ECode TableLayout::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_TableLayout,
                    sizeof(R_Styleable_TableLayout) / sizeof(Int32)),
            (ITypedArray**)&a);

    String stretchedColumns;
    a->GetString(0/*R.styleable.TableLayout_stretchColumns*/, &stretchedColumns);
    if (!stretchedColumns.IsNull()) {
        if (stretchedColumns.GetChar(0) == '*') {
            mStretchAllColumns = TRUE;
        }
        else {
            mStretchableColumns = ParseColumns(stretchedColumns);
        }
    }

    String shrinkedColumns;
    a->GetString(1/*R.styleable.TableLayout_shrinkColumns*/, &shrinkedColumns);
    if (!shrinkedColumns.IsNull()) {
        if (shrinkedColumns.GetChar(0) == '*') {
            mShrinkAllColumns = TRUE;
        }
        else {
            mShrinkableColumns = ParseColumns(shrinkedColumns);
        }
    }

    String collapsedColumns;
    a->GetString(2/*R.styleable.TableLayout_collapseColumns*/, &collapsedColumns);
    if (!collapsedColumns.IsNull()) {
        mCollapsedColumns = ParseColumns(collapsedColumns);
    }

    a->Recycle();

    return NOERROR;
}

/**
 * <p>Parses a sequence of columns ids defined in a CharSequence with the
 * following pattern (regex): \d+(\s*,\s*\d+)*</p>
 *
 * <p>Examples: "1" or "13, 7, 6" or "".</p>
 *
 * <p>The result of the parsing is stored in a sparse Boolean array. The
 * parsed column ids are used as the keys of the sparse array. The values
 * are always TRUE.</p>
 *
 * @param sequence a sequence of column ids, can be empty but not NULL
 * @return a sparse array of Boolean mapping column indexes to the columns
 *         collapse state
 */
HashMap<Int32, Boolean>* TableLayout::ParseColumns(
    /* [in] */ const String& sequence)
{
    HashMap<Int32, Boolean>* columns = new HashMap<Int32, Boolean>(5);
    /*Pattern pattern = Pattern.compile("\\s*,\\s*");
    String[] columnDefs = pattern.split(sequence);*/

    //for (String columnIdentifier : columnDefs) {
    //    try {
    //        Int32 columnIndex = Integer.parseInt(columnIdentifier);
    //        // only valid, i.e. positive, columns indexes are handled
    //        if (columnIndex >= 0) {
    //            // putting TRUE in this sparse array indicates that the
    //            // column index was defined in the XML file
    //            columns.put(columnIndex, TRUE);
    //        }
    //    } catch (NumberFormatException e) {
    //        // we just ignore columns that don't exist
    //    }
    //}

    return columns;
}

/**
 * <p>Performs initialization common to prorgrammatic use and XML use of
 * this widget.</p>
 */
void TableLayout::InitTableLayout()
{
    if (mCollapsedColumns == NULL) {
        mCollapsedColumns = new HashMap<Int32, Boolean>(5);
    }
    if (mStretchableColumns == NULL) {
        mStretchableColumns = new HashMap<Int32, Boolean>(5);
    }
    if (mShrinkableColumns == NULL) {
        mShrinkableColumns = new HashMap<Int32, Boolean>(5);
    }

    mPassThroughListener = new PassThroughHierarchyChangeListener(this);
    // make sure to call the parent class method to avoid potential
    // infinite loops
    LinearLayout::SetOnHierarchyChangeListener(mPassThroughListener);

    mInitialized = TRUE;
}

/**
 * {@inheritDoc}
 */
ECode TableLayout::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    // the user listener is delegated to our pass-through listener
    mPassThroughListener->mOnHierarchyChangeListener = listener;

    return NOERROR;
}

void TableLayout::RequestRowsLayout()
{
    if (mInitialized) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            GetChildAt(i)->RequestLayout();
        }
    }
}

/**
 * {@inheritDoc}
 */

ECode TableLayout::RequestLayout()
{
    if (mInitialized) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            GetChildAt(i)->ForceLayout();
        }
    }

    return LinearLayout::RequestLayout();
}

/**
 * <p>Indicates whether all columns are shrinkable or not.</p>
 *
 * @return TRUE if all columns are shrinkable, FALSE otherwise
 */
Boolean TableLayout::IsShrinkAllColumns()
{
    return mShrinkAllColumns;
}

/**
 * <p>Convenience method to mark all columns as shrinkable.</p>
 *
 * @param shrinkAllColumns TRUE to mark all columns shrinkable
 *
 * @attr ref android.R.styleable#TableLayout_shrinkColumns
 */
ECode TableLayout::SetShrinkAllColumns(
    /* [in] */ Boolean shrinkAllColumns)
{
    mShrinkAllColumns = shrinkAllColumns;

    return NOERROR;
}

/**
 * <p>Indicates whether all columns are stretchable or not.</p>
 *
 * @return TRUE if all columns are stretchable, FALSE otherwise
 */
Boolean TableLayout::IsStretchAllColumns()
{
    return mStretchAllColumns;
}

/**
 * <p>Convenience method to mark all columns as stretchable.</p>
 *
 * @param stretchAllColumns TRUE to mark all columns stretchable
 *
 * @attr ref android.R.styleable#TableLayout_stretchColumns
 */
ECode TableLayout::SetStretchAllColumns(
    /* [in] */ Boolean stretchAllColumns)
{
    mStretchAllColumns = stretchAllColumns;

    return NOERROR;
}

/**
 * <p>Collapses or restores a given column. When collapsed, a column
 * does not appear on screen and the extra space is reclaimed by the
 * other columns. A column is collapsed/restored only when it belongs to
 * a {@link android.widget.TableRow}.</p>
 *
 * <p>Calling this method requests a layout operation.</p>
 *
 * @param columnIndex the index of the column
 * @param isCollapsed TRUE if the column must be collapsed, FALSE otherwise
 *
 * @attr ref android.R.styleable#TableLayout_collapseColumns
 */
ECode TableLayout::SetColumnCollapsed(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isCollapsed)
{
    // update the collapse status of the column
    (*mCollapsedColumns)[columnIndex] = isCollapsed;

    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> view = GetChildAt(i);
        if (ITableRow::Probe(view) != NULL) {
            ((TableRow*)view->Probe(EIID_TableRow))->SetColumnCollapsed(columnIndex, isCollapsed);
        }
    }

    RequestRowsLayout();

    return NOERROR;
}

/**
 * <p>Returns the collapsed state of the specified column.</p>
 *
 * @param columnIndex the index of the column
 * @return TRUE if the column is collapsed, FALSE otherwise
 */
Boolean TableLayout::IsColumnCollapsed(
    /* [in] */ Int32 columnIndex)
{
    return (*mCollapsedColumns)[columnIndex];
}

/**
 * <p>Makes the given column stretchable or not. When stretchable, a column
 * takes up as much as available space as possible in its row.</p>
 *
 * <p>Calling this method requests a layout operation.</p>
 *
 * @param columnIndex the index of the column
 * @param isStretchable TRUE if the column must be stretchable,
 *                      FALSE otherwise. Default is FALSE.
 *
 * @attr ref android.R.styleable#TableLayout_stretchColumns
 */
ECode TableLayout::SetColumnStretchable(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isStretchable)
{
    (*mStretchableColumns)[columnIndex] = isStretchable;
    RequestRowsLayout();

    return NOERROR;
}

/**
 * <p>Returns whether the specified column is stretchable or not.</p>
 *
 * @param columnIndex the index of the column
 * @return TRUE if the column is stretchable, FALSE otherwise
 */
Boolean TableLayout::IsColumnStretchable(
    /* [in] */ Int32 columnIndex)
{
    return mStretchAllColumns || (*mStretchableColumns)[columnIndex];
}

/**
 * <p>Makes the given column shrinkable or not. When a row is too wide, the
 * table can reclaim extra space from shrinkable columns.</p>
 *
 * <p>Calling this method requests a layout operation.</p>
 *
 * @param columnIndex the index of the column
 * @param isShrinkable TRUE if the column must be shrinkable,
 *                     FALSE otherwise. Default is FALSE.
 *
 * @attr ref android.R.styleable#TableLayout_shrinkColumns
 */
ECode TableLayout::SetColumnShrinkable(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isShrinkable)
{
    (*mShrinkableColumns)[columnIndex] = isShrinkable;
    RequestRowsLayout();

    return NOERROR;
}

/**
 * <p>Returns whether the specified column is shrinkable or not.</p>
 *
 * @param columnIndex the index of the column
 * @return TRUE if the column is shrinkable, FALSE otherwise. Default is FALSE.
 */
Boolean TableLayout::IsColumnShrinkable(
    /* [in] */ Int32 columnIndex)
{
    return mShrinkAllColumns || (*mShrinkableColumns)[columnIndex];
}

/**
 * <p>Applies the columns collapse status to a new row added to this
 * table. This method is invoked by PassThroughHierarchyChangeListener
 * upon child insertion.</p>
 *
 * <p>This method only applies to {@link android.widget.TableRow}
 * instances.</p>
 *
 * @param child the newly added child
 */
void TableLayout::TrackCollapsedColumns(
    /* [in] */ IView* child)
{
    if (ITableRow::Probe(child) != NULL) {
        TableRow* row = (TableRow*)child->Probe(EIID_TableRow);
        HashMap<Int32, Boolean>::Iterator it = mCollapsedColumns->Begin();
        for (; it != mCollapsedColumns->End(); ++it) {
            Int32 columnIndex = it->mFirst;
            Boolean isCollapsed = it->mSecond;
            // the collapse status is set only when the column should be
            // collapsed; otherwise, this might affect the default
            // visibility of the row's children
            if (isCollapsed) {
                row->SetColumnCollapsed(columnIndex, isCollapsed);
            }
        }
    }
}

/**
 * {@inheritDoc}
 */

ECode TableLayout::AddView(
    /* [in] */ IView* child)
{
    FAIL_RETURN(LinearLayout::AddView(child));
    RequestRowsLayout();

    return NOERROR;
}

/**
 * {@inheritDoc}
 */

ECode TableLayout::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    FAIL_RETURN(LinearLayout::AddView(child, index));
    RequestRowsLayout();

    return NOERROR;
}

/**
 * {@inheritDoc}
 */

ECode TableLayout::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    FAIL_RETURN(LinearLayout::AddView(child, params));
    RequestRowsLayout();

    return NOERROR;
}

/**
 * {@inheritDoc}
 */

ECode TableLayout::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    FAIL_RETURN(LinearLayout::AddView(child, index, params));
    RequestRowsLayout();

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
void TableLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // enforce vertical layout
    MeasureVertical(widthMeasureSpec, heightMeasureSpec);
}

/**
 * {@inheritDoc}
 */
void TableLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    // enforce vertical layout
    LayoutVertical();
}

/**
 * {@inheritDoc}
 */
void TableLayout::MeasureChildBeforeLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 childIndex,
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 totalWidth,
    /* [in] */ Int32 heightMeasureSpec,
    /* [in] */ Int32 totalHeight)
{
    // when the measured child is a table row, we force the width of its
    // children with the widths computed in findLargestCells()
    if (ITableRow::Probe(child) != NULL) {
        ((TableRow*)child->Probe(EIID_TableRow))->SetColumnsWidthConstraints(mMaxWidths);
    }

    LinearLayout::MeasureChildBeforeLayout(child, childIndex,
            widthMeasureSpec, totalWidth, heightMeasureSpec, totalHeight);
}

/**
 * {@inheritDoc}
 */
ECode TableLayout::MeasureVertical(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    FindLargestCells(widthMeasureSpec);
    ShrinkAndStretchColumns(widthMeasureSpec);

    return LinearLayout::MeasureVertical(widthMeasureSpec, heightMeasureSpec);
}

/**
 * <p>Finds the largest cell in each column. For each column, the width of
 * the largest cell is applied to all the other cells.</p>
 *
 * @param widthMeasureSpec the measure constraint imposed by our parent
 */
void TableLayout::FindLargestCells(
    /* [in] */ Int32 widthMeasureSpec)
{
    Boolean firstRow = TRUE;

    // find the maximum width for each column
    // the total number of columns is dynamically changed if we find
    // wider rows as we go through the children
    // the array is reused for each layout operation; the array can grow
    // but never shrinks. Unused extra cells in the array are just ignored
    // this behavior avoids to unnecessary grow the array after the first
    // layout operation
    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);

        Int32 visible;
        child->GetVisibility(&visible);
        if (visible == GONE) {
            continue;
        }

        if (ITableRow::Probe(child) != NULL) {
            TableRow* row = (TableRow*)child->Probe(EIID_TableRow);
            // forces the row's height
            AutoPtr<IViewGroupLayoutParams> layoutParams = row->GetLayoutParams();
            layoutParams->SetHeight(TableLayoutLayoutParams::WRAP_CONTENT);

            ArrayOf<Int32>* widths = row->GetColumnsWidths(widthMeasureSpec);
            Int32 newLength = widths->GetLength();
            // this is the first row, we just need to copy the values
            if (firstRow) {
                if (mMaxWidths == NULL || mMaxWidths->GetLength() != newLength) {
                    ArrayOf<Int32>::Free(mMaxWidths);
                    mMaxWidths = ArrayOf<Int32>::Alloc(newLength);
                }
                memcpy(mMaxWidths->GetPayload(), widths->GetPayload(), newLength * sizeof(Int32));
                firstRow = FALSE;
            }
            else {
                Int32 length = mMaxWidths->GetLength();
                Int32 difference = newLength - length;
                // the current row is wider than the previous rows, so
                // we just grow the array and copy the values
                if (difference > 0) {
                    ArrayOf<Int32>* oldMaxWidths = mMaxWidths;
                    mMaxWidths = ArrayOf<Int32>::Alloc(newLength);
                    memcpy(mMaxWidths->GetPayload(), oldMaxWidths->GetPayload(),
                            oldMaxWidths->GetLength() * sizeof(Int32));
                    memcpy(mMaxWidths->GetPayload() + oldMaxWidths->GetLength(),
                            widths->GetPayload() + oldMaxWidths->GetLength(),
                            difference * sizeof(Int32));
                    ArrayOf<Int32>::Free(oldMaxWidths);
                }

                // the row is narrower or of the same width as the previous
                // rows, so we find the maximum width for each column
                // if the row is narrower than the previous ones,
                // difference will be negative
                length = Math::Min(length, newLength);
                for (Int32 j = 0; j < length; j++) {
                    (*mMaxWidths)[j] = Math::Max((*mMaxWidths)[j], (*widths)[j]);
                }
            }
        }
    }
}

/**
 * <p>Shrinks the columns if their total width is greater than the
 * width allocated by widthMeasureSpec. When the total width is less
 * than the allocated width, this method attempts to stretch columns
 * to fill the remaining space.</p>
 *
 * @param widthMeasureSpec the width measure specification as indicated
 *                         by this widget's parent
 */
void TableLayout::ShrinkAndStretchColumns(
    /* [in] */ Int32 widthMeasureSpec)
{
    // when we have no row, mMaxWidths is not initialized and the loop
    // below could cause a NPE
    if (mMaxWidths == NULL) {
        return;
    }

    // should we honor AT_MOST, EXACTLY and UNSPECIFIED?
    Int32 totalWidth = 0;
    for (Int32 i = 0; i < mMaxWidths->GetLength(); ++i) {
        totalWidth += (*mMaxWidths)[i];
    }

    Int32 size = MeasureSpec::GetSize(widthMeasureSpec) - mPaddingLeft - mPaddingRight;

    if ((totalWidth > size) && (mShrinkAllColumns ||
            mShrinkableColumns->Begin() != mShrinkableColumns->End())) {
        // oops, the largest columns are wider than the row itself
        // fairly redistribute the row's widh among the columns
        MutateColumnsWidth(mShrinkableColumns, mShrinkAllColumns, size, totalWidth);
    }
    else if ((totalWidth < size) && (mStretchAllColumns ||
            mStretchableColumns->Begin() != mStretchableColumns->End())) {
        // if we have some space left, we distribute it among the
        // expandable columns
        MutateColumnsWidth(mStretchableColumns, mStretchAllColumns, size, totalWidth);
    }
}

void TableLayout::MutateColumnsWidth(
    /* [in] */ HashMap<Int32, Boolean>* columns,
    /* [in] */ Boolean allColumns,
    /* [in] */ Int32 size,
    /* [in] */ Int32 totalWidth)
{
    Int32 skipped = 0;
    ArrayOf<Int32>* maxWidths = mMaxWidths;
    Int32 length = maxWidths->GetLength();
    Int32 count = allColumns ? length : columns->GetSize();
    Int32 totalExtraSpace = size - totalWidth;
    Int32 extraSpace = totalExtraSpace / count;

    // Column's widths are changed: force child table rows to re-measure.
    // (done by super.measureVertical after shrinkAndStretchColumns.)
    Int32 nbChildren = GetChildCount();
    for (Int32 i = 0; i < nbChildren; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        if (ITableRow::Probe(child) != NULL) {
            child->ForceLayout();
        }
    }

    if (!allColumns) {
        HashMap<Int32, Boolean>::Iterator it = columns->Begin();
        for (; it != columns->End(); ++it) {
            Int32 column = it->mFirst;
            if (it->mSecond) {
                if (column < length) {
                    (*maxWidths)[column] += extraSpace;
                }
                else {
                    skipped++;
                }
            }
        }
    }
    else {
        for (Int32 i = 0; i < count; i++) {
            (*maxWidths)[i] += extraSpace;
        }

        // we don't skip any column so we can return right away
        return;
    }

    if (skipped > 0 && skipped < count) {
        // reclaim any extra space we left to columns that don't exist
        extraSpace = skipped * extraSpace / (count - skipped);
        HashMap<Int32, Boolean>::Iterator it = columns->Begin();
        for (; it != columns->End(); ++it) {
            Int32 column = it->mFirst;
            if (it->mSecond && column < length) {
                if (extraSpace > (*maxWidths)[column]) {
                    (*maxWidths)[column] = 0;
                }
                else {
                    (*maxWidths)[column] += extraSpace;
                }
            }
        }
    }
}

/**
 * {@inheritDoc}
 */
AutoPtr<IViewGroupLayoutParams> TableLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    CTableLayoutLayoutParams::New(GetContext(), attrs, (ITableLayoutLayoutParams**)&lp);
    return lp;
}

/**
 * Returns a set of layout parameters with a width of
 * {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT},
 * and a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.
 */
AutoPtr<ILinearLayoutLayoutParams> TableLayout::GenerateDefaultLayoutParams()
{
    AutoPtr<ILinearLayoutLayoutParams> lp;
    CTableLayoutLayoutParams::New((ITableLayoutLayoutParams**)&lp);
    return lp;
}

/**
 * {@inheritDoc}
 */
Boolean TableLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return ITableLayoutLayoutParams::Probe(p) != NULL;
}

/**
 * {@inheritDoc}
 */
AutoPtr<IViewGroupLayoutParams> TableLayout::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    CTableLayoutLayoutParams::New(p, (ITableLayoutLayoutParams**)&lp);
    return lp;
}

ECode TableLayout::Init(
    /* [in] */ IContext* context)
{
    FAIL_RETURN(LinearLayout::Init(context));
    InitTableLayout();
    return NOERROR;
}

ECode TableLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(LinearLayout::Init(context, attrs));
    FAIL_RETURN(InitFromAttributes(context, attrs));
    InitTableLayout();
    return NOERROR;
}
