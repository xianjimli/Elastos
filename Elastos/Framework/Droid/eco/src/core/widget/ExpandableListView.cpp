
#include "widget/ExpandableListView.h"
#include "widget/CExpandableListPosition.h"
#include "widget/CExpandableListConnector.h"
#include "widget/CPositionMetadata.h"
#include "widget/CGroupMetadata.h"
#include "widget/ExpandableListViewSavedState.h"
#include "widget/CExpandableListViewSavedState.h"

static const Int32 R_Attr_ExpandableListViewStyle=0x0101006f;
static const Int32 R_Styleable_ExpandableListView_groupIndicator = 0;
static const Int32 R_Styleable_ExpandableListView_childIndicator = 1;
static const Int32 R_Styleable_ExpandableListView_indicatorLeft = 2;
static const Int32 R_Styleable_ExpandableListView_indicatorRight = 3;
static const Int32 R_Styleable_ExpandableListView_childIndicatorLeft = 4;
static const Int32 R_Styleable_ExpandableListView_childIndicatorRight = 5;
static const Int32 R_Styleable_ExpandableListView_childDivider = 6;

/**
 * The packed position represents a group.
 */
const Int32 ExpandableListView::PACKED_POSITION_TYPE_GROUP;

/**
 * The packed position represents a child.
 */
const Int32 ExpandableListView::PACKED_POSITION_TYPE_CHILD;

/**
 * The packed position represents a neither/null/no preference.
 */
const Int32 ExpandableListView::PACKED_POSITION_TYPE_NULL;

/**
 * The value for a packed position that represents neither/null/no
 * preference. This value is not otherwise possible since a group type
 * (first bit 0) should not have a child position filled.
 */
const Int64 ExpandableListView::PACKED_POSITION_VALUE_NULL;

/**
 * Denotes when a child indicator should inherit this bound from the generic
 * indicator bounds
 */
const Int32 ExpandableListView::CHILD_INDICATOR_INHERIT;


/** The mask (in packed position representation) for the child */
const Int64 ExpandableListView::PACKED_POSITION_MASK_CHILD;

/** The mask (in packed position representation) for the group */
const Int64 ExpandableListView::PACKED_POSITION_MASK_GROUP;

/** The mask (in packed position representation) for the type */
const Int64 ExpandableListView::PACKED_POSITION_MASK_TYPE;

/** The shift amount (in packed position representation) for the group */
const Int64 ExpandableListView::PACKED_POSITION_SHIFT_GROUP;

/** The shift amount (in packed position representation) for the type */
const Int64 ExpandableListView::PACKED_POSITION_SHIFT_TYPE;

/** The mask (in integer child position representation) for the child */
const Int64 ExpandableListView::PACKED_POSITION_INT_MASK_CHILD;

/** The mask (in integer group position representation) for the group */
const Int64 ExpandableListView::PACKED_POSITION_INT_MASK_GROUP;


Int32 ExpandableListView::EMPTY_STATE_SET[] = {};
//
///** State indicating the group is expanded. */
Int32 ExpandableListView::GROUP_EXPANDED_STATE_SET[] =
        {/*R.attr.state_expanded*/};
//
///** State indicating the group is empty (has no children). */
Int32 ExpandableListView::GROUP_EMPTY_STATE_SET[] =
        {/*R.attr.state_empty*/};

///** State indicating the group is expanded and empty (has no children). */
Int32 ExpandableListView::GROUP_EXPANDED_EMPTY_STATE_SET[] =
        {/*R.attr.state_expanded, R.attr.state_empty*/};

/** States for the group where the 0th bit is expanded and 1st bit is empty. */
Int32* ExpandableListView::GROUP_STATE_SETS[]= {
//     EMPTY_STATE_SET, // 00
//     GROUP_EXPANDED_STATE_SET, // 01
//     GROUP_EMPTY_STATE_SET, // 10
//     GROUP_EXPANDED_EMPTY_STATE_SET // 11
};

/** State indicating the child is the last within its group. */
Int32 ExpandableListView::CHILD_LAST_STATE_SET[] =
        {/*R.attr.state_last*/};



static Int32 R_Styleable_ExpandableListView[] = {
    0x0101010b, 0x0101010c, 0x0101010d, 0x0101010e,
    0x0101010f, 0x01010110, 0x01010111
};

ExpandableListView::ExpandableListView(
    /* [in] */ IContext* context) : ListView(context, NULL)
{
}

ExpandableListView::ExpandableListView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) : ListView(context, attrs, R_Attr_ExpandableListViewStyle)
{
}

ExpandableListView::ExpandableListView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) : ListView(context, attrs, defStyle)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs, ArrayOf<Int32>(R_Styleable_ExpandableListView, 7), defStyle,
                0, (ITypedArray**)&a);

    a->GetDrawable(R_Styleable_ExpandableListView_groupIndicator, (IDrawable**)&mGroupIndicator);
    a->GetDrawable(R_Styleable_ExpandableListView_childIndicator, (IDrawable**)&mChildIndicator);
    a->GetDimensionPixelSize(R_Styleable_ExpandableListView_indicatorLeft, 0, &mIndicatorLeft);
    a->GetDimensionPixelSize(R_Styleable_ExpandableListView_indicatorRight, 0, &mIndicatorRight);
    a->GetDimensionPixelSize(R_Styleable_ExpandableListView_childIndicatorLeft, CHILD_INDICATOR_INHERIT, &mChildIndicatorLeft);
    a->GetDimensionPixelSize(
            R_Styleable_ExpandableListView_childIndicatorRight, CHILD_INDICATOR_INHERIT, &mChildIndicatorRight);
    a->GetDrawable(R_Styleable_ExpandableListView_childDivider, (IDrawable**)&mChildDivider);

    a->Recycle();
}

void ExpandableListView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    // Draw children, etc.
    ListView::DispatchDraw(canvas);

    // If we have any indicators to draw, we do it here
    if ((mChildIndicator.Get() == NULL) && (mGroupIndicator.Get() == NULL)) {
        return;
    }

    Int32 saveCount = 0;
    Boolean clipToPadding = (mGroupFlags & CLIP_TO_PADDING_MASK) == CLIP_TO_PADDING_MASK;
    if (clipToPadding) {
        canvas->Save(&saveCount);
        Int32 scrollX = mScrollX;
        Int32 scrollY = mScrollY;
        Boolean isNotEmpty;
        canvas->ClipRectEx6(scrollX + mPaddingLeft, scrollY + mPaddingTop,
                scrollX + mRight - mLeft - mPaddingRight,
                scrollY + mBottom - mTop - mPaddingBottom, &isNotEmpty);
    }

    Int32 headerViewsCount = GetHeaderViewsCount();

    Int32 lastChildFlPos = mItemCount - GetFooterViewsCount() - headerViewsCount - 1;

    Int32 myB = mBottom;

    AutoPtr<IPositionMetadata> pos;
    AutoPtr<IView> item;
    AutoPtr<IDrawable> indicator;
    Int32 t, b;

    // Start at a value that is neither child nor group
    Int32 lastItemType = ~(ExpandableListPosition::CHILD | ExpandableListPosition::GROUP);

    AutoPtr<IRect> indicatorRect = mIndicatorRect;

    // The "child" mentioned in the following two lines is this
    // View's child, not referring to an expandable list's
    // notion of a child (as opposed to a group)
    Int32 childCount = GetChildCount();
    for (Int32 i = 0, childFlPos = mFirstPosition - headerViewsCount; i < childCount;
         i++, childFlPos++) {

        if (childFlPos < 0) {
            // This child is header
            continue;
        } else if (childFlPos > lastChildFlPos) {
            // This child is footer, so are all subsequent children
            break;
        }

        item = GetChildAt(i);
        item->GetTop(&t);
        item->GetBottom(&b);

        // This item isn't on the screen
        if ((b < 0) || (t > myB)) continue;

        // Get more expandable list-related info for this item
        mConnector->GetUnflattenedPos(childFlPos, (IPositionMetadata**)&pos);

        // If this item type and the previous item type are different, then we need to change
        // the left & right bounds
        if (((CExpandableListPosition*)((CPositionMetadata*)pos.Get())->mPosition.Get())->type != lastItemType) {
            if (((CExpandableListPosition*)((CPositionMetadata*)pos.Get())->mPosition.Get())->type == ExpandableListPosition::CHILD) {
                ((CRect*)indicatorRect.Get())->mLeft = (mChildIndicatorLeft == CHILD_INDICATOR_INHERIT) ?
                        mIndicatorLeft : mChildIndicatorLeft;
                ((CRect*)indicatorRect.Get())->mRight = (mChildIndicatorRight == CHILD_INDICATOR_INHERIT) ?
                        mIndicatorRight : mChildIndicatorRight;
            } else {
                ((CRect*)indicatorRect.Get())->mLeft = mIndicatorLeft;
                ((CRect*)indicatorRect.Get())->mRight = mIndicatorRight;
            }

            lastItemType = ((CExpandableListPosition*)((CPositionMetadata*)pos.Get())->mPosition.Get())->type;
        }

        if (((CRect*)indicatorRect.Get())->mLeft != ((CRect*)indicatorRect.Get())->mRight) {
            // Use item's full height + the divider height
            if (mStackFromBottom) {
                // See ListView#dispatchDraw
                ((CRect*)indicatorRect.Get())->mTop = t;// - mDividerHeight;
                ((CRect*)indicatorRect.Get())->mBottom = b;
            } else {
                ((CRect*)indicatorRect.Get())->mTop = t;
                ((CRect*)indicatorRect.Get())->mBottom = b;// + mDividerHeight;
            }

            // Get the indicator (with its state set to the item's state)
            indicator = GetIndicator(pos);
            if (indicator != NULL) {
                // Draw the indicator
                indicator->SetBoundsEx(indicatorRect);
                indicator->Draw(canvas);
            }
        }

        pos->Recycle();
    }

    if (clipToPadding) {
        canvas->RestoreToCount(saveCount);
    }
}

/**
 * Gets the indicator for the item at the given position. If the indicator
 * is stateful, the state will be given to the indicator.
 *
 * @param pos The flat list position of the item whose indicator
 *            should be returned.
 * @return The indicator in the proper state.
 */
AutoPtr<IDrawable> ExpandableListView::GetIndicator(
    /* [in] */ IPositionMetadata* pos)
{
    AutoPtr<IDrawable> indicator;

    if (((CExpandableListPosition*)(((CPositionMetadata*)pos)->mPosition.Get()))->type == ExpandableListPosition::GROUP) {
        indicator = mGroupIndicator;

        Boolean stateful;
        indicator->IsStateful(&stateful);
        if (indicator.Get() != NULL && stateful) {
            // Empty check based on availability of data.  If the groupMetadata isn't NULL,
            // we do a check on it. Otherwise, the group is collapsed so we consider it
            // empty for performance reasons.
            Boolean isEmpty = (((CPositionMetadata*)pos)->mGroupMetadata == NULL) ||
                    (((CGroupMetadata*)((CPositionMetadata*)pos)->mGroupMetadata.Get())->mLastChildFlPos == ((CGroupMetadata*)((CPositionMetadata*)pos)->mGroupMetadata.Get())->mFlPos);

            Boolean expanded;
            pos->IsExpanded(&expanded);
            Int32 stateSetIndex =
                (expanded ? 1 : 0) | // Expanded?
                (isEmpty ? 2 : 0); // Empty?

            Boolean res;
            //indicator->SetState(ArrayOf<Int32>(GROUP_STATE_SETS[stateSetIndex]), &res);
        }
    } else {
        indicator = mChildIndicator;

        Boolean stateful;
        indicator->IsStateful(&stateful);
        if (indicator != NULL && stateful) {
            // No need for a state sets array for the child since it only has two states
            Int32* stateSet = ((CExpandableListPosition*)((CPositionMetadata*)pos)->mPosition.Get())->flatListPos == ((CGroupMetadata*)(((CPositionMetadata*)pos)->mGroupMetadata.Get()))->mLastChildFlPos
                    ? CHILD_LAST_STATE_SET
                    : EMPTY_STATE_SET;

            Boolean res;
            //indicator->SetState(ArrayOf<Int32>(stateSet, ), &res);
        }
    }

    return indicator;
}

/**
 * Sets the drawable that will be drawn adjacent to every child in the list. This will
 * be drawn using the same height as the normal divider ({@link #setDivider(Drawable)}) or
 * if it does not have an intrinsic height, the height set by {@link #setDividerHeight(Int32)}.
 *
 * @param childDivider The drawable to use.
 */
ECode ExpandableListView::SetChildDivider(
    /* [in] */ IDrawable* childDivider)
{
    mChildDivider = childDivider;
    mClipChildDivider = childDivider != NULL && childDivider->Probe(EIID_IColorDrawable) != NULL;
    return NOERROR;
}

void ExpandableListView::DrawDivider(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* bounds,
    /* [in] */ Int32 childIndex)
{
    Int32 flatListPosition = childIndex + mFirstPosition;

    // Only proceed as possible child if the divider isn't above all items (if it is above
    // all items, then the item below it has to be a group)
    if (flatListPosition >= 0) {
        Int32 adjustedPosition = GetFlatPositionForConnector(flatListPosition);
        AutoPtr<IPositionMetadata> pos;
        mConnector->GetUnflattenedPos(adjustedPosition, (IPositionMetadata**)&pos);
        // If this item is a child, or it is a non-empty group that is expanded

        Boolean expanded;
        pos->IsExpanded(&expanded);
        if ((((CExpandableListPosition*)((CPositionMetadata*)pos.Get())->mPosition.Get())->type == ExpandableListPosition::CHILD) || (expanded &&
                ((CGroupMetadata*)((CPositionMetadata*)pos.Get())->mGroupMetadata.Get())->mLastChildFlPos != ((CGroupMetadata*)((CPositionMetadata*)pos.Get())->mGroupMetadata.Get())->mFlPos)) {
            // These are the cases where we draw the child divider
            AutoPtr<IDrawable> divider = mChildDivider;
            Boolean clip = mClipChildDivider;
            if (!clip) {
                divider->SetBoundsEx(bounds);
            } else {
                Int32 res;
                canvas->Save(&res);
                Boolean empty;
                canvas->ClipRectEx3(bounds, &empty);
            }
            divider->Draw(canvas);
            if (clip) {
                canvas->Restore();
            }
            pos->Recycle();
            return;
        }
        pos->Recycle();
    }

    // Otherwise draw the default divider
    ListView::DrawDivider(canvas, bounds, flatListPosition);
}

/**
 * This overloaded method should not be used, instead use
 * {@link #setAdapter(ExpandableListAdapter)}.
 * <p>
 * {@inheritDoc}
 */
ECode ExpandableListView::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    /*throw new RuntimeException(
            "For ExpandableListView, use setAdapter(ExpandableListAdapter) instead of " +
            "setAdapter(ListAdapter)");*/
    return E_RUNTIME_EXCEPTION;
}

/**
 * This method should not be used, use {@link #getExpandableListAdapter()}.
 */
AutoPtr<IAdapter> ExpandableListView::GetAdapter()
{
    /*
     * The developer should never really call this method on an
     * ExpandableListView, so it would be nice to throw a RuntimeException,
     * but AdapterView calls this
     */
    return ListView::GetAdapter();
}

/**
 * Register a callback to be invoked when an item has been clicked and the
 * caller prefers to receive a ListView-style position instead of a group
 * and/or child position. In most cases, the caller should use
 * {@link #setOnGroupClickListener} and/or {@link #setOnChildClickListener}.
 * <p />
 * {@inheritDoc}
 */
ECode ExpandableListView::SetOnItemClickListener(
    /* [in] */ IOnItemClickListener* l)
{
    return ListView::SetOnItemClickListener(l);
}

/**
 * Sets the adapter that provides data to this view.
 * @param adapter The adapter that provides data to this view.
 */
ECode ExpandableListView::SetAdapter(
    /* [in] */ IExpandableListAdapter* adapter)
{
    // Set member variable
    mAdapter = adapter;

    if (adapter != NULL) {
        // Create the connector
        //mConnector = new ExpandableListConnector(adapter);

        CExpandableListConnector::New(adapter, (IExpandableListConnector**)&mConnector);
    } else {
        mConnector = NULL;
    }

    // Link the ListView (superclass) to the expandable list data through the connector
    ListView::SetAdapter(mConnector);
    return NOERROR;
}

/**
 * Gets the adapter that provides data to this view.
 * @return The adapter that provides data to this view.
 */
AutoPtr<IExpandableListAdapter> ExpandableListView::GetExpandableListAdapter()
{
    return mAdapter;
}

/**
 * @param position An absolute (including header and footer) flat list position.
 * @return TRUE if the position corresponds to a header or a footer item.
 */
Boolean ExpandableListView::IsHeaderOrFooterPosition(
    /* [in] */ Int32 position)
{
    Int32 footerViewsStart = mItemCount - GetFooterViewsCount();
    return (position < GetHeaderViewsCount() || position >= footerViewsStart);
}

/**
 * Converts an absolute item flat position into a group/child flat position, shifting according
 * to the number of header items.
 *
 * @param flatListPosition The absolute flat position
 * @return A group/child flat position as expected by the connector.
 */
Int32 ExpandableListView::GetFlatPositionForConnector(
    /* [in] */ Int32 flatListPosition)
{
    return flatListPosition - GetHeaderViewsCount();
}

/**
 * Converts a group/child flat position into an absolute flat position, that takes into account
 * the possible headers.
 *
 * @param flatListPosition The child/group flat position
 * @return An absolute flat position.
 */
Int32 ExpandableListView::GetAbsoluteFlatPosition(
    /* [in] */ Int32 flatListPosition)
{
    return flatListPosition + GetHeaderViewsCount();
}

Boolean ExpandableListView::PerformItemClick(
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    // Ignore clicks in header/footers
    if (IsHeaderOrFooterPosition(position)) {
        // Clicked on a header/footer, so ignore pass it on to super
        return ListView::PerformItemClick(v, position, id);
    }

    // Internally handle the item click
    Int32 adjustedPosition = GetFlatPositionForConnector(position);
    return HandleItemClick(v, adjustedPosition, id);
}

/**
 * This will either expand/collapse groups (if a group was clicked) or pass
 * on the click to the proper child (if a child was clicked)
 *
 * @param position The flat list position. This has already been factored to
 *            remove the header/footer.
 * @param id The ListAdapter ID, not the group or child ID.
 */
Boolean ExpandableListView::HandleItemClick(
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    AutoPtr<IPositionMetadata> posMetadata;
    mConnector->GetUnflattenedPos(position, (IPositionMetadata**)&posMetadata);

    id = GetChildOrGroupId(((CPositionMetadata*)posMetadata.Get())->mPosition);

    Boolean returnValue;
    if (((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->mPosition.Get())->type == ExpandableListPosition::GROUP) {
        /* It's a group, so handle collapsing/expanding */

        /* It's a group click, so pass on event */
        if (mOnGroupClickListener.Get() != NULL) {
            Boolean res;
            mOnGroupClickListener->OnGroupClick((IExpandableListView*)this->Probe(EIID_IExpandableListView), v,
                ((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos, id, &res);
            if (res) {
                posMetadata->Recycle();
                return TRUE;
            }
        }

        Boolean expanded;
        posMetadata->IsExpanded(&expanded);
        if (expanded) {
            /* Collapse it */
            ((CExpandableListConnector*)mConnector.Get())->CollapseGroup(posMetadata);

            PlaySoundEffect(SoundEffectConstants_CLICK);

            if (mOnGroupCollapseListener.Get() != NULL) {
                mOnGroupCollapseListener->OnGroupCollapse(((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos);
            }
        } else {
            /* Expand it */
            ((CExpandableListConnector*)mConnector.Get())->ExpandGroup(posMetadata);

            PlaySoundEffect(SoundEffectConstants_CLICK);

            if (mOnGroupExpandListener.Get() != NULL) {
                mOnGroupExpandListener->OnGroupExpand(((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos);
            }

            Int32 groupPos = ((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos;
            Int32 groupFlatPos = ((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->mPosition.Get())->flatListPos;

            Int32 shiftedGroupPosition = groupFlatPos + GetHeaderViewsCount();

            Int32 count;
            mAdapter->GetChildrenCount(groupPos, &count);
            SmoothScrollToPosition(shiftedGroupPosition + count,
                    shiftedGroupPosition);
        }

        returnValue = TRUE;
    } else {
        /* It's a child, so pass on event */
        if (mOnChildClickListener.Get() != NULL) {
            PlaySoundEffect(SoundEffectConstants_CLICK);
            mOnChildClickListener->OnChildClick((IExpandableListView*)(this->Probe(EIID_IExpandableListView)), v,
                ((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos,
                    ((CExpandableListPosition*)((CPositionMetadata*)posMetadata.Get())->mPosition.Get())->childPos, id, &returnValue);
            return returnValue;
        }

        returnValue = FALSE;
    }

    posMetadata->Recycle();

    return returnValue;
}

/**
 * Expand a group in the grouped list view
 *
 * @param groupPos the group to be expanded
 * @return True if the group was expanded, FALSE otherwise (if the group
 *         was already expanded, this will return FALSE)
 */
Boolean ExpandableListView::ExpandGroup(
    /* [in] */ Int32 groupPos)
{
    Boolean retValue;
    retValue = ((CExpandableListConnector*)mConnector.Get())->ExpandGroup(groupPos);

    if (mOnGroupExpandListener.Get() != NULL) {
        mOnGroupExpandListener->OnGroupExpand(groupPos);
    }

    return retValue;
}

/**
 * Collapse a group in the grouped list view
 *
 * @param groupPos position of the group to collapse
 * @return True if the group was collapsed, FALSE otherwise (if the group
 *         was already collapsed, this will return FALSE)
 */
Boolean ExpandableListView::CollapseGroup(
    /* [in] */ Int32 groupPos)
{
    Boolean retValue;
    retValue = ((CExpandableListConnector*)mConnector.Get())->CollapseGroup(groupPos);

    if (mOnGroupCollapseListener.Get() != NULL) {
        mOnGroupCollapseListener->OnGroupCollapse(groupPos);
    }

    return retValue;
}



ECode ExpandableListView::SetOnGroupCollapseListener(
    /* [in] */ IOnGroupCollapseListener* onGroupCollapseListener)
{
    mOnGroupCollapseListener = onGroupCollapseListener;
    return NOERROR;
}

ECode ExpandableListView::SetOnGroupExpandListener(
    /* [in] */ IOnGroupExpandListener* onGroupExpandListener)
{
    mOnGroupExpandListener = onGroupExpandListener;
    return NOERROR;
}

ECode ExpandableListView::SetOnGroupClickListener(
    /* [in] */ IOnGroupClickListener* onGroupClickListener)
{
    mOnGroupClickListener = onGroupClickListener;
    return NOERROR;
}

ECode ExpandableListView::SetOnChildClickListener(
    /* [in] */ IOnChildClickListener* onChildClickListener)
{
    mOnChildClickListener = onChildClickListener;
    return NOERROR;
}

/**
 * Converts a flat list position (the raw position of an item (child or group)
 * in the list) to an group and/or child position (represented in a
 * packed position). This is useful in situations where the caller needs to
 * use the underlying {@link ListView}'s methods. Use
 * {@link ExpandableListView#getPackedPositionType} ,
 * {@link ExpandableListView#getPackedPositionChild},
 * {@link ExpandableListView#getPackedPositionGroup} to unpack.
 *
 * @param flatListPosition The flat list position to be converted.
 * @return The group and/or child position for the given flat list position
 *         in packed position representation. #PACKED_POSITION_VALUE_NULL if
 *         the position corresponds to a header or a footer item.
 */
Int64 ExpandableListView::GetExpandableListPosition(
    /* [in] */ Int32 flatListPosition)
{
    if (IsHeaderOrFooterPosition(flatListPosition)) {
        return PACKED_POSITION_VALUE_NULL;
    }

    Int32 adjustedPosition = GetFlatPositionForConnector(flatListPosition);
    AutoPtr<IPositionMetadata> pm;
    mConnector->GetUnflattenedPos(adjustedPosition, (IPositionMetadata**)&pm);
    Int64 packedPos;
    ((CExpandableListPosition*)((CPositionMetadata*)pm.Get())->mPosition.Get())->GetPackedPosition(&packedPos);
    pm->Recycle();
    return packedPos;
}

/**
 * Converts a group and/or child position to a flat list position. This is
 * useful in situations where the caller needs to use the underlying
 * {@link ListView}'s methods.
 *
 * @param packedPosition The group and/or child positions to be converted in
 *            packed position representation. Use
 *            {@link #getPackedPositionForChild(Int32, Int32)} or
 *            {@link #getPackedPositionForGroup(Int32)}.
 * @return The flat list position for the given child or group.
 */
Int32 ExpandableListView::GetFlatListPosition(
    /* [in] */ Int64 packedPosition)
{
    AutoPtr<IPositionMetadata> pm;
    pm = ((CExpandableListConnector*)mConnector.Get())->GetFlattenedPos(ExpandableListPosition::ObtainPosition(packedPosition));
    Int32 flatListPosition = ((CExpandableListPosition*)((CPositionMetadata*)pm.Get())->mPosition.Get())->flatListPos;
    pm->Recycle();
    return GetAbsoluteFlatPosition(flatListPosition);
}

/**
 * Gets the position of the currently selected group or child (along with
 * its type). Can return {@link #PACKED_POSITION_VALUE_NULL} if no selection.
 *
 * @return A packed position containing the currently selected group or
 *         child's position and type. #PACKED_POSITION_VALUE_NULL if no selection
 *         or if selection is on a header or a footer item.
 */
Int64 ExpandableListView::GetSelectedPosition()
{
    Int32 selectedPos = GetSelectedItemPosition();

    // The case where there is no selection (selectedPos == -1) is also handled here.
    return GetExpandableListPosition(selectedPos);
}

/**
 * Gets the ID of the currently selected group or child. Can return -1 if no
 * selection.
 *
 * @return The ID of the currently selected group or child. -1 if no
 *         selection.
 */
Int64 ExpandableListView::GetSelectedId()
{
    Int64 packedPos = GetSelectedPosition();
    if (packedPos == PACKED_POSITION_VALUE_NULL) return -1;

    Int32 groupPos = GetPackedPositionGroup(packedPos);

    Int64 id;
    if (GetPackedPositionType(packedPos) == PACKED_POSITION_TYPE_GROUP) {
        // It's a group
        mAdapter->GetGroupId(groupPos, &id);
        return id;
    } else {
        // It's a child
        mAdapter->GetChildId(groupPos, GetPackedPositionChild(packedPos), &id);
        return id;
    }
}

/**
 * Sets the selection to the specified group.
 * @param groupPosition The position of the group that should be selected.
 */
ECode ExpandableListView::SetSelectedGroup(
    /* [in] */ Int32 groupPosition)
{
    AutoPtr<IExpandableListPosition> elGroupPos = ExpandableListPosition::ObtainGroupPosition(groupPosition);
    AutoPtr<IPositionMetadata> pm;
    pm = ((CExpandableListConnector*)mConnector.Get())->GetFlattenedPos(elGroupPos);
    elGroupPos->Recycle();
    Int32 absoluteFlatPosition = GetAbsoluteFlatPosition(((CExpandableListPosition*)((CPositionMetadata*)pm.Get())->mPosition.Get())->flatListPos);
    ListView::SetSelection(absoluteFlatPosition);
    pm->Recycle();
    return NOERROR;
}

/**
 * Sets the selection to the specified child. If the child is in a collapsed
 * group, the group will only be expanded and child subsequently selected if
 * shouldExpandGroup is set to TRUE, otherwise the method will return FALSE.
 *
 * @param groupPosition The position of the group that contains the child.
 * @param childPosition The position of the child within the group.
 * @param shouldExpandGroup Whether the child's group should be expanded if
 *            it is collapsed.
 * @return Whether the selection was successfully set on the child.
 */
Boolean ExpandableListView::SetSelectedChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [in] */ Boolean shouldExpandGroup)
{
    AutoPtr<IExpandableListPosition> elChildPos = ExpandableListPosition::ObtainChildPosition(
            groupPosition, childPosition);
    AutoPtr<IPositionMetadata> flatChildPos;
    flatChildPos = ((CExpandableListConnector*)mConnector.Get())->GetFlattenedPos(elChildPos);

    if (flatChildPos.Get() == NULL) {
        // The child's group isn't expanded

        // Shouldn't expand the group, so return FALSE for we didn't set the selection
        if (!shouldExpandGroup) return FALSE;

        ExpandGroup(groupPosition);

        flatChildPos = ((CExpandableListConnector*)mConnector.Get())->GetFlattenedPos(elChildPos);

        // Sanity check
        if (flatChildPos.Get() == NULL) {
            //throw new IllegalStateException("Could not find child");
        }
    }

    Int32 absoluteFlatPosition = GetAbsoluteFlatPosition(((CExpandableListPosition*)((CPositionMetadata*)flatChildPos.Get())->mPosition.Get())->flatListPos);
    ListView::SetSelection(absoluteFlatPosition);

    elChildPos->Recycle();
    flatChildPos->Recycle();

    return TRUE;
}

/**
 * Whether the given group is currently expanded.
 *
 * @param groupPosition The group to check.
 * @return Whether the group is currently expanded.
 */
Boolean ExpandableListView::IsGroupExpanded(
    /* [in] */ Int32 groupPosition)
{
    Boolean expanded;
    mConnector->IsGroupExpanded(groupPosition, &expanded);
    return expanded;
}

/**
 * Gets the type of a packed position. See
 * {@link #getPackedPositionForChild(Int32, Int32)}.
 *
 * @param packedPosition The packed position for which to return the type.
 * @return The type of the position contained within the packed position,
 *         either {@link #PACKED_POSITION_TYPE_CHILD}, {@link #PACKED_POSITION_TYPE_GROUP}, or
 *         {@link #PACKED_POSITION_TYPE_NULL}.
 */
Int32 ExpandableListView::GetPackedPositionType(
    /* [in] */ Int64 packedPosition)
{
    if (packedPosition == PACKED_POSITION_VALUE_NULL) {
        return PACKED_POSITION_TYPE_NULL;
    }

    return (packedPosition & PACKED_POSITION_MASK_TYPE) == PACKED_POSITION_MASK_TYPE
            ? PACKED_POSITION_TYPE_CHILD
            : PACKED_POSITION_TYPE_GROUP;
}

/**
 * Gets the group position from a packed position. See
 * {@link #getPackedPositionForChild(Int32, Int32)}.
 *
 * @param packedPosition The packed position from which the group position
 *            will be returned.
 * @return The group position portion of the packed position. If this does
 *         not contain a group, returns -1.
 */
Int32 ExpandableListView::GetPackedPositionGroup(
    /* [in] */ Int64 packedPosition)
{
    // Null
    if (packedPosition == PACKED_POSITION_VALUE_NULL) return -1;

    return (Int32) ((packedPosition & PACKED_POSITION_MASK_GROUP) >> PACKED_POSITION_SHIFT_GROUP);
}

/**
 * Gets the child position from a packed position that is of
 * {@link #PACKED_POSITION_TYPE_CHILD} type (use {@link #getPackedPositionType(Int64)}).
 * To get the group that this child belongs to, use
 * {@link #getPackedPositionGroup(Int64)}. See
 * {@link #getPackedPositionForChild(Int32, Int32)}.
 *
 * @param packedPosition The packed position from which the child position
 *            will be returned.
 * @return The child position portion of the packed position. If this does
 *         not contain a child, returns -1.
 */
Int32 ExpandableListView::GetPackedPositionChild(
    /* [in] */ Int64 packedPosition)
{
    // Null
    if (packedPosition == PACKED_POSITION_VALUE_NULL) return -1;

    // Group since a group type clears this bit
    if ((packedPosition & PACKED_POSITION_MASK_TYPE) != PACKED_POSITION_MASK_TYPE) return -1;

    return (Int32) (packedPosition & PACKED_POSITION_MASK_CHILD);
}

/**
 * Returns the packed position representation of a child's position.
 * <p>
 * In general, a packed position should be used in
 * situations where the position given to/returned from an
 * {@link ExpandableListAdapter} or {@link ExpandableListView} method can
 * either be a child or group. The two positions are packed into a single
 * Int64 which can be unpacked using
 * {@link #getPackedPositionChild(Int64)},
 * {@link #getPackedPositionGroup(Int64)}, and
 * {@link #getPackedPositionType(Int64)}.
 *
 * @param groupPosition The child's parent group's position.
 * @param childPosition The child position within the group.
 * @return The packed position representation of the child (and parent group).
 */
Int64 ExpandableListView::GetPackedPositionForChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    return (((Int64)PACKED_POSITION_TYPE_CHILD) << PACKED_POSITION_SHIFT_TYPE)
            | ((((Int64)groupPosition) & PACKED_POSITION_INT_MASK_GROUP)
                    << PACKED_POSITION_SHIFT_GROUP)
            | (childPosition & PACKED_POSITION_INT_MASK_CHILD);
}

/**
 * Returns the packed position representation of a group's position. See
 * {@link #getPackedPositionForChild(Int32, Int32)}.
 *
 * @param groupPosition The child's parent group's position.
 * @return The packed position representation of the group.
 */
Int64 ExpandableListView::GetPackedPositionForGroup(
    /* [in] */ Int32 groupPosition)
{
    // No need to OR a type in because PACKED_POSITION_GROUP == 0
    return ((((Int64)groupPosition) & PACKED_POSITION_INT_MASK_GROUP)
                    << PACKED_POSITION_SHIFT_GROUP);
}

AutoPtr<IContextMenuInfo> ExpandableListView::CreateContextMenuInfo(
    /* [in] */ IView* view,
    /* [in] */ Int32 flatListPosition,
    /* [in] */ Int64 id)
{
    if (IsHeaderOrFooterPosition(flatListPosition)) {
        // Return normal info for header/footer view context menus
        return new AdapterContextMenuInfo(view, flatListPosition, id);
    }

    Int32 adjustedPosition = GetFlatPositionForConnector(flatListPosition);
    AutoPtr<IPositionMetadata> pm;
    mConnector->GetUnflattenedPos(adjustedPosition, (IPositionMetadata**)&pm);
    AutoPtr<IExpandableListPosition> pos = ((CPositionMetadata*)pm.Get())->mPosition;
    pm->Recycle();

    id = GetChildOrGroupId(pos);
    Int64 packedPosition;
    pos->GetPackedPosition(&packedPosition);
    pos->Recycle();

    return new ExpandableListContextMenuInfo(view, packedPosition, id);
}

/**
 * Gets the ID of the group or child at the given <code>position</code>.
 * This is useful since there is no ListAdapter ID -> ExpandableListAdapter
 * ID conversion mechanism (in some cases, it isn't possible).
 *
 * @param position The position of the child or group whose ID should be
 *            returned.
 */
Int64 ExpandableListView::GetChildOrGroupId(
    /* [in] */ IExpandableListPosition* position)
{
    Int64 id;
    if (((CExpandableListPosition*)position)->type == ExpandableListPosition::CHILD) {
        mAdapter->GetChildId(((CExpandableListPosition*)position)->groupPos, ((CExpandableListPosition*)position)->childPos, &id);
        return id;
    } else {
        mAdapter->GetGroupId(((CExpandableListPosition*)position)->groupPos, &id);
        return id;
    }
}

/**
 * Sets the indicator to be drawn next to a child.
 *
 * @param childIndicator The drawable to be used as an indicator. If the
 *            child is the last child for a group, the state
 *            {@link android.R.attr#state_last} will be set.
 */
ECode ExpandableListView::SetChildIndicator(
    /* [in] */ IDrawable* childIndicator)
{
    mChildIndicator = childIndicator;
    return NOERROR;
}

/**
 * Sets the drawing bounds for the child indicator. For either, you can
 * specify {@link #CHILD_INDICATOR_INHERIT} to use inherit from the general
 * indicator's bounds.
 *
 * @see #setIndicatorBounds(Int32, Int32)
 * @param left The left position (relative to the left bounds of this View)
 *            to start drawing the indicator.
 * @param right The right position (relative to the left bounds of this
 *            View) to end the drawing of the indicator.
 */
ECode ExpandableListView::SetChildIndicatorBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    mChildIndicatorLeft = left;
    mChildIndicatorRight = right;
    return NOERROR;
}

/**
 * Sets the indicator to be drawn next to a group.
 *
 * @param groupIndicator The drawable to be used as an indicator. If the
 *            group is empty, the state {@link android.R.attr#state_empty} will be
 *            set. If the group is expanded, the state
 *            {@link android.R.attr#state_expanded} will be set.
 */
ECode ExpandableListView::SetGroupIndicator(
    /* [in] */ IDrawable* groupIndicator)
{
    mGroupIndicator = groupIndicator;
    return NOERROR;
}

/**
 * Sets the drawing bounds for the indicators (at minimum, the group indicator
 * is affected by this; the child indicator is affected by this if the
 * child indicator bounds are set to inherit).
 *
 * @see #setChildIndicatorBounds(Int32, Int32)
 * @param left The left position (relative to the left bounds of this View)
 *            to start drawing the indicator.
 * @param right The right position (relative to the left bounds of this
 *            View) to end the drawing of the indicator.
 */
ECode ExpandableListView::SetIndicatorBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    mIndicatorLeft = left;
    mIndicatorRight = right;
    return NOERROR;
}

ExpandableListView::ExpandableListContextMenuInfo::ExpandableListContextMenuInfo(
    /* [in] */ IView* targetView,
    /* [in] */ Int64 packedPosition,
    /* [in] */ Int64 id)
{
    this->targetView = targetView;
    this->packedPosition = packedPosition;
    this->id = id;
}

PInterface ExpandableListView::ExpandableListContextMenuInfo::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 ExpandableListView::ExpandableListContextMenuInfo::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ExpandableListView::ExpandableListContextMenuInfo::Release()
{
    return ElRefBase::Release();
}

ECode ExpandableListView::ExpandableListContextMenuInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

AutoPtr<IParcelable> ExpandableListView::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = ListView::OnSaveInstanceState();

    ArrayOf<IGroupMetadata*>* expandedGroupMetadataList = NULL;
    if (mConnector.Get() != NULL) {
        expandedGroupMetadataList = ((CExpandableListConnector*)mConnector.Get())->GetExpandedGroupMetadataList();
    }

    AutoPtr<IExpandableListViewSavedState> ss;
    CExpandableListViewSavedState::New(superState, *expandedGroupMetadataList, (IExpandableListViewSavedState**)&ss);

    return (IParcelable*)ss->Probe(EIID_IParcelable);
}

void ExpandableListView::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (!(state->Probe(EIID_IExpandableListViewSavedState))) {
        ListView::OnRestoreInstanceState(state);
    }

    ExpandableListViewSavedState* ss = (ExpandableListViewSavedState*) state;
    ListView::OnRestoreInstanceState(ss->GetSuperState());

    if (mConnector.Get() != NULL && ss->expandedGroupMetadataList != NULL) {
        ((CExpandableListConnector*)mConnector.Get())->SetExpandedGroupMetadataList(ss->expandedGroupMetadataList);
    }
}


