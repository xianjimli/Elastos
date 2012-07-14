
#ifndef __ABSSPANNER_H__
#define __ABSSPANNER_H__

/**
 * An abstract base class for spinner widgets. SDK users will probably not
 * need to use this class.
 *
 * @attr ref android.R.styleable#AbsSpinner_entries
 */

#include "widget/AdapterView.h"
#include "utils/SparseArray.h"

class AbsSpinner : public AdapterView
{
public:

    AbsSpinner(
        /* [in] */ IContext* context = NULL);

    AbsSpinner(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AbsSpinner(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    ~AbsSpinner();

    /**
     * Common code for different constructor flavors
     */
private:
    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    CARAPI_(void) InitAbsSpinner();

public:
    /**
     * The Adapter is used to provide the data which backs this Spinner.
     * It also provides methods to transform spinner items based on their position
     * relative to the selected item.
     * @param adapter The SpinnerAdapter to use for this Spinner
     */
    virtual CARAPI SetAdapter(
        /* [in] */ IAdapter* adapter);

    /**
     * Clear out all children from the list
     */
    virtual CARAPI_(void) ResetList();

    /**
     * @see android.view.View#measure(int, int)
     *
     * Figure out the dimensions of this Spinner. The width comes from
     * the widthMeasureSpec as Spinnners can't have their width set to
     * UNSPECIFIED. The height is based on the height of the selected item
     * plus padding.
     */
protected:
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

public:
    virtual CARAPI_(Int32) GetChildHeight(
        /* [in] */ IView* child);

    virtual CARAPI_(Int32) GetChildWidth(
        /* [in] */ IView* child);

protected:
    virtual CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** params);

public:
    virtual CARAPI_(void) RecycleAllViews();

    /**
     * Jump directly to a specific item in the adapter data.
     */
    virtual CARAPI SetSelection(
        /* [in] */ Int32 position,
        /* [in] */ Boolean animate);

    virtual CARAPI SetSelection(
        /* [in] */ Int32 position);


    /**
     * Makes the item at the supplied position selected.
     *
     * @param position Position to select
     * @param animate Should the transition be animated
     *
     */
    CARAPI_(void) SetSelectionInt(
        /* [in] */ Int32 position,
        /* [in] */ Boolean animate);

    virtual CARAPI LayoutEx(
        /* [in] */ Int32 delta,
        /* [in] */ Boolean animate) = 0;

public:
    virtual CARAPI_(AutoPtr<IView>) GetSelectedView();

    /**
     * Override to prevent spamming ourselves with layout requests
     * as we place views
     *
     * @see android.view.View#requestLayout()
     */
    virtual CARAPI RequestLayout();

    virtual CARAPI_(AutoPtr<IAdapter>) GetAdapter();

    virtual CARAPI_(Int32) GetCount();

    /**
     * Maps a point to a position in the list.
     *
     * @param x X in local coordinate
     * @param y Y in local coordinate
     * @return The position of the item which contains the specified point, or
     *         {@link #INVALID_POSITION} if the point does not intersect an item.
     */
    virtual CARAPI_(Int32) PointToPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    class RecycleBin {
    public:
        RecycleBin(
            /* [in] */ AbsSpinner* mHost);

        CARAPI_(void) Put(
            /* [in] */ Int32 position,
            /* [in] */ IView* v);

        CARAPI_(IView*) Get(
            /* [in] */ Int32 position);

        CARAPI_(void) Clear();

    private:
        SparseArray mScrapHeap;

        AbsSpinner* mHost;
    };

public:
    AutoPtr<IAdapter> mAdapter;

    Int32 mHeightMeasureSpec;
    Int32 mWidthMeasureSpec;
    Boolean mBlockLayoutRequests;

    Int32 mSelectionLeftPadding;
    Int32 mSelectionTopPadding;
    Int32 mSelectionRightPadding;
    Int32 mSelectionBottomPadding;
    AutoPtr<CRect> mSpinnerPadding;

    RecycleBin* mRecycler;

private:
    AutoPtr<IDataSetObserver> mDataSetObserver;

    /** Temporary frame to hold a child View's frame rectangle */
    AutoPtr<IRect> mTouchFrame;

};

#endif //__ABSSPANNER_H__

