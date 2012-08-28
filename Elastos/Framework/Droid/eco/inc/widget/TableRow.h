
#ifndef __TABLEROW_H__
#define __TABLEROW_H__


#include "ext/frameworkext.h"
#include "widget/LinearLayout.h"
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>


extern "C" const InterfaceID EIID_TableRow;

/**
 * <p>A layout that arranges its children horizontally. A TableRow should
 * always be used as a child of a {@link android.widget.TableLayout}. If a
 * TableRow's parent is not a TableLayout, the TableRow will behave as
 * an horizontal {@link android.widget.LinearLayout}.</p>
 *
 * <p>The children of a TableRow do not need to specify the
 * <code>layout_width</code> and <code>layout_height</code> attributes in the
 * XML file. TableRow always enforces those values to be respectively
 * {@link android.widget.TableLayout.LayoutParams#MATCH_PARENT} and
 * {@link android.widget.TableLayout.LayoutParams#WRAP_CONTENT}.</p>
 *
 * <p>
 * Also see {@link TableRow.LayoutParams android.widget.TableRow.LayoutParams}
 * for layout attributes </p>
 */
class TableRow : public LinearLayout
{
private:
    // special transparent hierarchy change listener
    class ChildrenTracker
        : public ElRefBase
        , public IViewGroupOnHierarchyChangeListener
    {
        friend class TableRow;

    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChildViewAdded(
            /* [in] */ IView* parent,
            /* [in] */ IView* child);

        CARAPI OnChildViewRemoved(
            /* [in] */ IView* parent,
            /* [in] */ IView* child);

    private:
        CARAPI_(void) SetOnHierarchyChangeListener(
            /* [in] */ IViewGroupOnHierarchyChangeListener* listener);

    private:
        AutoPtr<IViewGroupOnHierarchyChangeListener> mListener;
        TableRow* mOwner;
    };

public:
    /**
     * <p>Creates a new TableRow for the given context.</p>
     *
     * @param context the application environment
     */
    TableRow(
        /* [in] */ IContext* context);

    /**
     * <p>Creates a new TableRow for the given context and with the
     * specified set attributes.</p>
     *
     * @param context the application environment
     * @param attrs a collection of attributes
     */
    TableRow(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI SetOnHierarchyChangeListener(
        /* [in] */ IViewGroupOnHierarchyChangeListener* listener);

    /**
     * <p>Collapses or restores a given column.</p>
     *
     * @param columnIndex the index of the column
     * @param collapsed true if the column must be collapsed, false otherwise
     * {@hide}
     */
    virtual CARAPI SetColumnCollapsed(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Boolean collapsed);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(AutoPtr<IView>) GetVirtualChildAt(
        /* [in] */ Int32 i);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(Int32) GetVirtualChildCount();

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(Int32) MeasureNullChild(
        /* [in] */ Int32 childIndex);

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
    CARAPI_(Int32) GetChildrenSkipCount(
        /* [in] */ IView* child,
        /* [in] */ Int32 index);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(Int32) GetLocationOffset(
        /* [in] */ IView* child);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(Int32) GetNextLocationOffset(
        /* [in] */ IView* child);

    /**
     * <p>Measures the preferred width of each child, including its margins.</p>
     *
     * @param widthMeasureSpec the width constraint imposed by our parent
     *
     * @return an array of integers corresponding to the width of each cell, or
     *         column, in this row
     * {@hide}
     */
    virtual CARAPI_(ArrayOf<Int32>*) GetColumnsWidths(
        /* [in] */ Int32 widthMeasureSpec);

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
    virtual CARAPI SetColumnsWidthConstraints(
        /* [in] */ ArrayOf<Int32>* columnWidths);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);


protected:
    TableRow();

    /**
     * <p>Creates a new TableRow for the given context.</p>
     *
     * @param context the application environment
     */
    CARAPI Init(
        /* [in] */ IContext* context);

    /**
     * <p>Creates a new TableRow for the given context and with the
     * specified set attributes.</p>
     *
     * @param context the application environment
     * @param attrs a collection of attributes
     */
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
     * a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT} and no spanning.
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
    CARAPI_(void) InitTableRow();

    CARAPI_(void) MapIndexAndColumns();


private:
    Int32 mNumColumns;
    ArrayOf<Int32>* mColumnWidths;
    ArrayOf<Int32>* mConstrainedColumnWidths;
    HashMap<Int32, Int32>* mColumnToChildIndex;

    AutoPtr<ChildrenTracker> mChildrenTracker;
};

#endif
