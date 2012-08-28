
#ifndef __TABLELAYOUT_H__
#define __TABLELAYOUT_H__

#include "ext/frameworkext.h"
#include "widget/LinearLayout.h"
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>


/**
 * <p>A layout that arranges its children into rows and columns.
 * A TableLayout consists of a number of {@link android.widget.TableRow} objects,
 * each defining a row (actually, you can have other children, which will be
 * explained below). TableLayout containers do not display border lines for
 * their rows, columns, or cells. Each row has zero or more cells; each cell can
 * hold one {@link android.view.View View} object. The table has as many columns
 * as the row with the most cells. A table can leave cells empty. Cells can span
 * columns, as they can in HTML.</p>
 *
 * <p>The width of a column is defined by the row with the widest cell in that
 * column. However, a TableLayout can specify certain columns as shrinkable or
 * stretchable by calling
 * {@link #setColumnShrinkable(Int32, Boolean) setColumnShrinkable()}
 * or {@link #setColumnStretchable(Int32, Boolean) setColumnStretchable()}. If
 * marked as shrinkable, the column width can be shrunk to fit the table into
 * its parent object. If marked as stretchable, it can expand in width to fit
 * any extra space. The total width of the table is defined by its parent
 * container. It is important to remember that a column can be both shrinkable
 * and stretchable. In such a situation, the column will change its size to
 * always use up the available space, but never more. Finally, you can hide a
 * column by calling
 * {@link #setColumnCollapsed(Int32,Boolean) setColumnCollapsed()}.</p>
 *
 * <p>The children of a TableLayout cannot specify the <code>layout_width</code>
 * attribute. Width is always <code>MATCH_PARENT</code>. However, the
 * <code>layout_height</code> attribute can be defined by a child; default value
 * is {@link android.widget.TableLayout.LayoutParams#WRAP_CONTENT}. If the child
 * is a {@link android.widget.TableRow}, then the height is always
 * {@link android.widget.TableLayout.LayoutParams#WRAP_CONTENT}.</p>
 *
 * <p> Cells must be added to a row in increasing column order, both in code and
 * XML. Column numbers are zero-based. If you don't specify a column number for
 * a child cell, it will autoincrement to the next available column. If you skip
 * a column number, it will be considered an empty cell in that row. See the
 * TableLayout examples in ApiDemos for examples of creating tables in XML.</p>
 *
 * <p>Although the typical child of a TableLayout is a TableRow, you can
 * actually use any View subclass as a direct child of TableLayout. The View
 * will be displayed as a single row that spans all the table columns.</p>
 *
 * <p>See the <a href="{@docRoot}resources/tutorials/views/hello-tablelayout.html">Table
 * Layout tutorial</a>.</p>
 */

class TableLayout : public LinearLayout
{
private:
    /**
     * <p>A pass-through listener acts upon the events and dispatches them
     * to another listener. This allows the table layout to set its own internal
     * hierarchy change listener without preventing the user to setup his.</p>
     */
    class PassThroughHierarchyChangeListener
        : public ElRefBase
        , public IViewGroupOnHierarchyChangeListener
    {
        friend class TableLayout;

    public:
        PassThroughHierarchyChangeListener(
            /* [in] */ TableLayout* owner);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        /**
         * {@inheritDoc}
         */
        CARAPI OnChildViewAdded(
            /* [in] */ IView* parent,
            /* [in] */ IView* child);

        /**
         * {@inheritDoc}
         */
        CARAPI OnChildViewRemoved(
            /* [in] */ IView* parent,
            /* [in] */ IView* child);

    private:
        AutoPtr<IViewGroupOnHierarchyChangeListener> mOnHierarchyChangeListener;
        TableLayout* mOwner;
    };

public:
    /**
     * <p>Creates a new TableLayout for the given context.</p>
     *
     * @param context the application environment
     */
    TableLayout(
        /* [in] */ IContext* context);

    /**
     * <p>Creates a new TableLayout for the given context and with the
     * specified set attributes.</p>
     *
     * @param context the application environment
     * @param attrs a collection of attributes
     */
    TableLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ~TableLayout();

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI SetOnHierarchyChangeListener(
        /* [in] */ IViewGroupOnHierarchyChangeListener* listener);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI RequestLayout();

    /**
     * <p>Indicates whether all columns are shrinkable or not.</p>
     *
     * @return true if all columns are shrinkable, false otherwise
     */
    virtual CARAPI_(Boolean) IsShrinkAllColumns();

    /**
     * <p>Convenience method to mark all columns as shrinkable.</p>
     *
     * @param shrinkAllColumns true to mark all columns shrinkable
     *
     * @attr ref android.R.styleable#TableLayout_shrinkColumns
     */
    virtual CARAPI SetShrinkAllColumns(
        /* [in] */ Boolean shrinkAllColumns);

    /**
     * <p>Indicates whether all columns are stretchable or not.</p>
     *
     * @return true if all columns are stretchable, false otherwise
     */
    virtual CARAPI_(Boolean) IsStretchAllColumns();

    /**
     * <p>Convenience method to mark all columns as stretchable.</p>
     *
     * @param stretchAllColumns true to mark all columns stretchable
     *
     * @attr ref android.R.styleable#TableLayout_stretchColumns
     */
    virtual CARAPI SetStretchAllColumns(
        /* [in] */ Boolean stretchAllColumns);

    /**
     * <p>Collapses or restores a given column. When collapsed, a column
     * does not appear on screen and the extra space is reclaimed by the
     * other columns. A column is collapsed/restored only when it belongs to
     * a {@link android.widget.TableRow}.</p>
     *
     * <p>Calling this method requests a layout operation.</p>
     *
     * @param columnIndex the index of the column
     * @param isCollapsed true if the column must be collapsed, false otherwise
     *
     * @attr ref android.R.styleable#TableLayout_collapseColumns
     */
    virtual CARAPI SetColumnCollapsed(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Boolean isCollapsed);

    /**
     * <p>Returns the collapsed state of the specified column.</p>
     *
     * @param columnIndex the index of the column
     * @return true if the column is collapsed, false otherwise
     */
    virtual CARAPI_(Boolean) IsColumnCollapsed(
        /* [in] */ Int32 columnIndex);

    /**
     * <p>Makes the given column stretchable or not. When stretchable, a column
     * takes up as much as available space as possible in its row.</p>
     *
     * <p>Calling this method requests a layout operation.</p>
     *
     * @param columnIndex the index of the column
     * @param isStretchable true if the column must be stretchable,
     *                      false otherwise. Default is false.
     *
     * @attr ref android.R.styleable#TableLayout_stretchColumns
     */
    virtual CARAPI SetColumnStretchable(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Boolean isStretchable);

    /**
     * <p>Returns whether the specified column is stretchable or not.</p>
     *
     * @param columnIndex the index of the column
     * @return true if the column is stretchable, false otherwise
     */
    virtual CARAPI_(Boolean) IsColumnStretchable(
        /* [in] */ Int32 columnIndex);

    /**
     * <p>Makes the given column shrinkable or not. When a row is too wide, the
     * table can reclaim extra space from shrinkable columns.</p>
     *
     * <p>Calling this method requests a layout operation.</p>
     *
     * @param columnIndex the index of the column
     * @param isShrinkable true if the column must be shrinkable,
     *                     false otherwise. Default is false.
     *
     * @attr ref android.R.styleable#TableLayout_shrinkColumns
     */
    virtual CARAPI SetColumnShrinkable(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Boolean isShrinkable);

    /**
     * <p>Returns whether the specified column is shrinkable or not.</p>
     *
     * @param columnIndex the index of the column
     * @return true if the column is shrinkable, false otherwise. Default is false.
     */
    virtual CARAPI_(Boolean) IsColumnShrinkable(
        /* [in] */ Int32 columnIndex);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI AddView(
        /* [in] */ IView* child);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index);

    //@Override
    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ IViewGroupLayoutParams* params);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(void) MeasureChildBeforeLayout(
        /* [in] */ IView* child,
        /* [in] */ Int32 childIndex,
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 totalWidth,
        /* [in] */ Int32 heightMeasureSpec,
        /* [in] */ Int32 totalHeight);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI MeasureVertical(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);

protected:
    TableLayout();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    /**
     * Returns a set of layout parameters with a width of
     * {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT},
     * and a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.
     */
    //@Override
    CARAPI_(AutoPtr<ILinearLayoutLayoutParams>) GenerateDefaultLayoutParams();

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

private:
    /**
     * <p>Parses a sequence of columns ids defined in a CharSequence with the
     * following pattern (regex): \d+(\s*,\s*\d+)*</p>
     *
     * <p>Examples: "1" or "13, 7, 6" or "".</p>
     *
     * <p>The result of the parsing is stored in a sparse Boolean array. The
     * parsed column ids are used as the keys of the sparse array. The values
     * are always true.</p>
     *
     * @param sequence a sequence of column ids, can be empty but not null
     * @return a sparse array of Boolean mapping column indexes to the columns
     *         collapse state
     */
    static HashMap<Int32, Boolean>* ParseColumns(
        /* [in] */ const String& sequence);

    /**
     * <p>Performs initialization common to prorgrammatic use and XML use of
     * this widget.</p>
     */
    CARAPI_(void) InitTableLayout();


    CARAPI_(void) RequestRowsLayout();

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
    CARAPI_(void) TrackCollapsedColumns(
        /* [in] */ IView* child);

    /**
     * <p>Finds the largest cell in each column. For each column, the width of
     * the largest cell is applied to all the other cells.</p>
     *
     * @param widthMeasureSpec the measure constraint imposed by our parent
     */
    CARAPI_(void) FindLargestCells(
        /* [in] */ Int32 widthMeasureSpec);

    /**
     * <p>Shrinks the columns if their total width is greater than the
     * width allocated by widthMeasureSpec. When the total width is less
     * than the allocated width, this method attempts to stretch columns
     * to fill the remaining space.</p>
     *
     * @param widthMeasureSpec the width measure specification as indicated
     *                         by this widget's parent
     */
    CARAPI_(void) ShrinkAndStretchColumns(
        /* [in] */ Int32 widthMeasureSpec);

    CARAPI_(void) MutateColumnsWidth(
        /* [in] */ HashMap<Int32, Boolean>* columns,
        /* [in] */ Boolean allColumns,
        /* [in] */ Int32 size,
        /* [in] */ Int32 totalWidth);

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

private:
    ArrayOf<Int32>* mMaxWidths;
    HashMap<Int32, Boolean>* mStretchableColumns;
    HashMap<Int32, Boolean>* mShrinkableColumns;
    HashMap<Int32, Boolean>* mCollapsedColumns;

    Boolean mShrinkAllColumns;
    Boolean mStretchAllColumns;

    AutoPtr<PassThroughHierarchyChangeListener> mPassThroughListener;

    Boolean mInitialized;
};

#endif
