
#include "view/menu/IconMenuView.h"
#include "view/menu/CIconMenuItemView.h"
#include "view/menu/MenuBuilder.h"
#include "view/menu/CIconMenuViewLayoutParams.h"
#include "view/menu/MenuItemImpl.h"
#include <elastos/Math.h>


/**
 * Constructor called from {@link IconMenuView#onSaveInstanceState()}
 */
IconMenuView::SavedState::SavedState(
    /* [in] */ IParcelable* superState,
    /* [in] */ Int32 focusedPosition)
    : ViewBaseSavedState(superState)
    , mFocusedPosition(focusedPosition)
{}

/**
 * Constructor called from {@link #CREATOR}
 */
IconMenuView::SavedState::SavedState(
    /* [in] */ IParcel* in)
    : ViewBaseSavedState(in)
{
    in->ReadInt32(&mFocusedPosition);
}

UInt32 IconMenuView::SavedState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 IconMenuView::SavedState::Release()
{
    return ElRefBase::Release();
}

//@override
ECode IconMenuView::SavedState::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    ViewBaseSavedState::WriteToParcel(dest);
    dest->WriteInt32(mFocusedPosition);

    return NOERROR;
}


IconMenuView::OnClickListener::OnClickListener(
    /* [in] */ IconMenuView* host)
    : mHost(host)
{}

PInterface IconMenuView::OnClickListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return NULL;
}

UInt32 IconMenuView::OnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 IconMenuView::OnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode IconMenuView::OnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IViewOnClickListener*)this) {
        *pIID = EIID_IViewOnKeyListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode IconMenuView::OnClickListener::OnClick(
    /* [in] */ IView* v)
{
    // Switches the menu to expanded mode
    AutoPtr<IMenuBuilderCallback> cb;
    mHost->mMenu->GetCallback((IMenuBuilderCallback**)&cb);
    if (cb != NULL) {
        // Call callback
        cb->OnMenuModeChange(mHost->mMenu);
    }

    return NOERROR;
}


const Int32 IconMenuView::ITEM_CAPTION_CYCLE_DELAY;

IconMenuView::IconMenuView()
    : mHasStaleChildren(FALSE)
    , mMenuBeingLongpressed(FALSE)
    , mLastChildrenCaptionMode(FALSE)
    , mLayout(NULL)
{}

IconMenuView::~IconMenuView()
{
    if (mLayout != NULL) {
        ArrayOf<Int32>::Free(mLayout);
    }
}

static Int32 R_Styleable_IconMenuView[] = {
    0x01010132, 0x01010133, 0x01010134, 0x01010135,
    0x010102d3
};

static Int32 R_Styleable_MenuView[] = {
    0x010100ae, 0x0101012c, 0x0101012d, 0x0101012e,
    0x0101012f, 0x01010130, 0x01010131
};

ECode IconMenuView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(ViewGroup::Init(context, attrs));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_IconMenuView, sizeof(R_Styleable_IconMenuView) / sizeof(Int32)),
        0, 0, (ITypedArray**)&a);

    a->GetDimensionPixelSize(0/*com.android.internal.R.styleable.IconMenuView_rowHeight*/, 64, &mRowHeight);
    a->GetInt32(1/*com.android.internal.R.styleable.IconMenuView_maxRows*/, 2, &mMaxRows);
    a->GetInt32(4/*com.android.internal.R.styleable.IconMenuView_maxItems*/, 6, &mMaxItems);
    a->GetInt32(2/*com.android.internal.R.styleable.IconMenuView_maxItemsPerRow*/, 3, &mMaxItemsPerRow);
    a->GetDrawable(3/*com.android.internal.R.styleable.IconMenuView_moreIcon*/, (IDrawable**)&mMoreIcon);
    a->Recycle();

    a = NULL;
    context->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_MenuView, sizeof(R_Styleable_MenuView) / sizeof(Int32)),
        0, 0, (ITypedArray**)&a);

    a->GetDrawable(5/*com.android.internal.R.styleable.MenuView_itemBackground*/, (IDrawable**)&mItemBackground);
    a->GetDrawable(2/*com.android.internal.R.styleable.MenuView_horizontalDivider*/, (IDrawable**)&mHorizontalDivider);
    a->GetDrawable(3/*com.android.internal.R.styleable.MenuView_verticalDivider*/, (IDrawable**)&mVerticalDivider);
    a->GetResourceId(0/*com.android.internal.R.styleable.MenuView_windowAnimationStyle*/, 0, &mAnimations);
    a->Recycle();

    if (mHorizontalDivider != NULL) {
        mHorizontalDivider->GetIntrinsicHeight(&mHorizontalDividerHeight);
        // Make sure to have some height for the divider
        if (mHorizontalDividerHeight == -1) mHorizontalDividerHeight = 1;
    }

    if (mVerticalDivider != NULL) {
        mVerticalDivider->GetIntrinsicWidth(&mVerticalDividerWidth);
        // Make sure to have some width for the divider
        if (mVerticalDividerWidth == -1) mVerticalDividerWidth = 1;
    }

    mLayout = ArrayOf<Int32>::Alloc(mMaxRows);

    // This view will be drawing the dividers
    SetWillNotDraw(FALSE);

    // This is so we'll receive the MENU key in touch mode
    SetFocusableInTouchMode(TRUE);
    // This is so our children can still be arrow-key focused
    SetDescendantFocusability(FOCUS_AFTER_DESCENDANTS);

    return NOERROR;
}

void IconMenuView::LayoutItems(
    /* [in] */ Int32 width)
{
    Int32 numItems = ViewGroup::GetChildCount();
    if (numItems == 0) {
        mLayoutNumRows = 0;
        return;
    }

    // Start with the least possible number of rows
    Int32 curNumRows =
            Math::Min((Int32) Math::Ceil(numItems / (Float)mMaxItemsPerRow), mMaxRows);

    /*
     * Increase the number of rows until we find a configuration that fits
     * all of the items' titles. Worst case, we use mMaxRows.
     */
    for (; curNumRows <= mMaxRows; curNumRows++) {
        LayoutItemsUsingGravity(curNumRows, numItems);

        if (curNumRows >= numItems) {
            // Can't have more rows than items
            break;
        }

        if (DoItemsFit()) {
            // All the items fit, so this is a good configuration
            break;
        }
    }
}

/**
 * Figures out the layout for the menu items by equally distributing, and
 * adding any excess items equally to lower rows.
 *
 * @param numRows The total number of rows for the menu view
 * @param numItems The total number of items (across all rows) contained in
 *            the menu view
 * @return int[] Where the value of index i contains the number of items for row i
 */
void IconMenuView::LayoutItemsUsingGravity(
    /* [in] */ Int32 numRows,
    /* [in] */ Int32 numItems)
{
    Int32 numBaseItemsPerRow = numItems / numRows;
    Int32 numLeftoverItems = numItems % numRows;
    /**
     * The bottom rows will each get a leftover item. Rows (indexed at 0)
     * that are >= this get a leftover item. Note: if there are 0 leftover
     * items, no rows will get them since this value will be greater than
     * the last row.
     */
    Int32 rowsThatGetALeftoverItem = numRows - numLeftoverItems;

    for (Int32 i = 0; i < numRows; i++) {
        (*mLayout)[i] = numBaseItemsPerRow;

        // Fill the bottom rows with a leftover item each
        if (i >= rowsThatGetALeftoverItem) {
            (*mLayout)[i]++;
        }
    }

    mLayoutNumRows = numRows;
}

/**
 * Checks whether each item's title is fully visible using the current
 * layout.
 *
 * @return True if the items fit (each item's text is fully visible), false
 *         otherwise.
 */
Boolean IconMenuView::DoItemsFit()
{
    Int32 itemPos = 0;

    Int32 numRows = mLayoutNumRows;
    for (Int32 row = 0; row < numRows; row++) {
        Int32 numItemsOnRow = (*mLayout)[row];

        /*
         * If there is only one item on this row, increasing the
         * number of rows won't help.
         */
        if (numItemsOnRow == 1) {
            itemPos++;
            continue;
        }

        for (Int32 itemsOnRowCounter = numItemsOnRow; itemsOnRowCounter > 0;
                itemsOnRowCounter--) {
            AutoPtr<IView> child = ViewGroup::GetChildAt(itemPos++);
            AutoPtr<IViewGroupLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**) &lp);
            IIconMenuViewLayoutParams* ilp = IIconMenuViewLayoutParams::Probe(lp);
            if (((CIconMenuViewLayoutParams*)ilp)->mMaxNumItemsOnRow < numItemsOnRow) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/**
 * Adds an CIconMenuItemView to this icon menu view.
 * @param itemView The item's view to add
 */
void IconMenuView::AddItemView(
    /* [in] */ IIconMenuItemView* itemView)
{
    // Set ourselves on the item view
    ((CIconMenuItemView*)itemView)->SetIconMenuView((IIconMenuView*)this->Probe(EIID_IIconMenuView));

    AutoPtr<IDrawableConstantState> state;
    mItemBackground->GetConstantState((IDrawableConstantState**) &state);

    AutoPtr<IResources> res;
    GetContext()->GetResources((IResources**) &res);

    AutoPtr<IDrawable> drawable;
    state->NewDrawableEx(res, (IDrawable**) &drawable);
    // Apply the background to the item view
    itemView->SetBackgroundDrawable(drawable);

    // This class is the invoker for all its item views
    itemView->SetItemInvoker((IMenuBuilderItemInvoker*)this->Probe(EIID_IMenuBuilderItemInvoker));

    AutoPtr<IIconMenuViewLayoutParams> lp = ((CIconMenuItemView*)itemView)->GetTextAppropriateLayoutParams();
    AddView(IView::Probe(itemView), lp);
}

/**
 * Creates the item view for the 'More' button which is used to switch to
 * the expanded menu view. This button is a special case since it does not
 * have a MenuItemData backing it.
 * @return The CIconMenuItemView for the 'More' button
 */
AutoPtr<IIconMenuItemView> IconMenuView::CreateMoreItemView()
{
    AutoPtr<ILayoutInflater> inflater = ((MenuBuilderBase*)mMenu->Probe(
            EIID_MenuBuilderBase))->GetMenuType(MenuBuilder::TYPE_ICON)->GetInflater();

    AutoPtr<IView> view;
    inflater->Inflate(0x01090028/*com.android.internal.R.layout.icon_menu_item_layout*/, NULL, (IView**)&view);
    AutoPtr<IIconMenuItemView> itemView = IIconMenuItemView::Probe(view);

    AutoPtr<IResources> r;
    GetContext()->GetResources((IResources**)&r);
    AutoPtr<ICharSequence> text;
    r->GetText(0x010402cd/*com.android.internal.R.string.more_item_label*/, (ICharSequence**)&text);
    ((CIconMenuItemView*)itemView.Get())->Initialize(text, mMoreIcon);

    // Set up a click listener on the view since there will be no invocation sequence
    // due to the lack of a MenuItemData this view
    AutoPtr<IViewOnClickListener> onClick = new OnClickListener(this);
    itemView->SetOnClickListener(onClick);

    return itemView;
}

ECode IconMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 menuType)
{
    mMenu = menu;
    return UpdateChildren(TRUE);
}

ECode IconMenuView::UpdateChildren(
    /* [in] */ Boolean cleared)
{
    // This method does a clear refresh of children
    ViewGroup::RemoveAllViews();

    List< AutoPtr<IMenuItemImpl> >* itemsToShow =
            ((MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase))->GetVisibleItems();
    const Int32 numItems = itemsToShow->GetSize();
    const Int32 numItemsThatCanFit = mMaxItems;
    // Minimum of the num that can fit and the num that we have
    const Int32 minFitMinus1AndNumItems = Math::Min(numItemsThatCanFit - 1, numItems);

    AutoPtr<IMenuItemImpl> itemData;
    // Traverse through all but the last item that can fit since that last item can either
    // be a 'More' button or a sixth item
    List< AutoPtr<IMenuItemImpl> >::Iterator it;
    Int32 i;

    for (it = itemsToShow->Begin(), i = 0; i < minFitMinus1AndNumItems, it != itemsToShow->End(); ++it, ++i) {
        itemData = *it;
        AutoPtr<IView> view = ((MenuItemImpl*)itemData.Get())->GetItemView(
                MenuBuilder::TYPE_ICON, (IViewGroup*)this->Probe(EIID_IViewGroup));

        AddItemView(IIconMenuItemView::Probe(view));
    }

    if (numItems > numItemsThatCanFit) {
        // If there are more items than we can fit, show the 'More' button to
        // switch to expanded mode
        if (mMoreItemView == NULL) {
            mMoreItemView = CreateMoreItemView();
        }

        AddItemView(mMoreItemView);

        // The last view is the more button, so the actual number of items is one less than
        // the number that can fit
        mNumActualItemsShown = numItemsThatCanFit - 1;
    }
    else if (numItems == numItemsThatCanFit) {
        // There are exactly the number we can show, so show the last item
        AutoPtr<IMenuItemImpl> lastItemData = (*itemsToShow)[numItemsThatCanFit - 1];
        AutoPtr<IView> view = ((MenuItemImpl*)lastItemData.Get())->GetItemView(
                MenuBuilder::TYPE_ICON, (IViewGroup*)this->Probe(EIID_IViewGroup));
        AddItemView(IIconMenuItemView::Probe(view));

        // The items shown fit exactly
        mNumActualItemsShown = numItemsThatCanFit;
    }

    return NOERROR;
}

/**
 * The positioning algorithm that gets called from onMeasure.  It
 * just computes positions for each child, and then stores them in the child's layout params.
 * @param menuWidth The width of this menu to assume for positioning
 * @param menuHeight The height of this menu to assume for positioning
 */
void IconMenuView::PositionChildren(
    /* [in] */ Int32 menuWidth,
    /* [in] */ Int32 menuHeight)
{
    // Clear the containers for the positions where the dividers should be drawn
    if (mHorizontalDivider != NULL) mHorizontalDividerRects.Clear();
    if (mVerticalDivider != NULL) mVerticalDividerRects.Clear();

    // Get the minimum number of rows needed
    const Int32 numRows = mLayoutNumRows;
    const Int32 numRowsMinus1 = numRows - 1;
    const ArrayOf<Int32>* numItemsForRow = mLayout;

    // The item position across all rows
    Int32 itemPos = 0;
    AutoPtr<IView> child;
    AutoPtr<CIconMenuViewLayoutParams> childLayoutParams;

    // Use float for this to get precise positions (uniform item widths
    // instead of last one taking any slack), and then convert to ints at last opportunity
    Float itemLeft;
    Float itemTop = 0;
    // Since each row can have a different number of items, this will be computed per row
    Float itemWidth;
    // Subtract the space needed for the horizontal dividers
    Float itemHeight = (menuHeight - mHorizontalDividerHeight * (numRows - 1)) / (Float)numRows;

    for (Int32 row = 0; row < numRows; row++) {
        // Start at the left
        itemLeft = 0;

        // Subtract the space needed for the vertical dividers, and divide by the number of items
        itemWidth = (menuWidth - mVerticalDividerWidth * ((*numItemsForRow)[row] - 1))
                / (Float)(*numItemsForRow)[row];

        for (Int32 itemPosOnRow = 0; itemPosOnRow < (*numItemsForRow)[row]; itemPosOnRow++) {
            // Tell the child to be exactly this size
            child = ViewGroup::GetChildAt(itemPos);
            child->Measure(MeasureSpec::MakeMeasureSpec((Int32)itemWidth, MeasureSpec::EXACTLY),
                    MeasureSpec::MakeMeasureSpec((Int32)itemHeight, MeasureSpec::EXACTLY));

            // Remember the child's position for layout
            AutoPtr<IViewGroupLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            childLayoutParams = (CIconMenuViewLayoutParams*)IIconMenuViewLayoutParams::Probe(lp);
            childLayoutParams->mLeft = (Int32)itemLeft;
            childLayoutParams->mRight = (Int32)(itemLeft + itemWidth);
            childLayoutParams->mTop = (Int32)itemTop;
            childLayoutParams->mBottom = (Int32)(itemTop + itemHeight);

            // Increment by item width
            itemLeft += itemWidth;
            itemPos++;

            // Add a vertical divider to draw
            if (mVerticalDivider != NULL) {
                AutoPtr<IRect> rect;
                CRect::New((Int32)itemLeft,
                        (Int32)itemTop, (Int32)(itemLeft + mVerticalDividerWidth),
                        (Int32)(itemTop + itemHeight), (IRect**)&rect);
                mVerticalDividerRects.PushBack(rect);
            }

            // Increment by divider width (even if we're not computing
            // dividers, since we need to leave room for them when
            // calculating item positions)
            itemLeft += mVerticalDividerWidth;
        }

        // Last child on each row should extend to very right edge
        if (childLayoutParams != NULL) {
            childLayoutParams->mRight = menuWidth;
        }

        itemTop += itemHeight;

        // Add a horizontal divider to draw
        if ((mHorizontalDivider != NULL) && (row < numRowsMinus1)) {
            AutoPtr<IRect> rect;
            CRect::New(0, (Int32)itemTop, menuWidth,
                    (Int32)(itemTop + mHorizontalDividerHeight), (IRect**)&rect);
            mHorizontalDividerRects.PushBack(rect);

            itemTop += mHorizontalDividerHeight;
        }
    }
}

void IconMenuView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (mHasStaleChildren) {
        mHasStaleChildren = FALSE;

        // If we have stale data, resync with the menu
        UpdateChildren(FALSE);
    }

    Int32 measuredWidth = ResolveSize(Math::INT32_MAX_VALUE, widthMeasureSpec);
    CalculateItemFittingMetadata(measuredWidth);
    LayoutItems(measuredWidth);

    // Get the desired height of the icon menu view (last row of items does
    // not have a divider below)
    const Int32 layoutNumRows = mLayoutNumRows;
    const Int32 desiredHeight = (mRowHeight + mHorizontalDividerHeight) *
            layoutNumRows - mHorizontalDividerHeight;

    // Maximum possible width and desired height
    ViewGroup::SetMeasuredDimension(measuredWidth,
            ResolveSize(desiredHeight, heightMeasureSpec));

    // Position the children
    if (layoutNumRows > 0) {
        PositionChildren(mMeasuredWidth, mMeasuredHeight);
    }
}

void IconMenuView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    AutoPtr<IView> child;
    AutoPtr<CIconMenuViewLayoutParams> childLayoutParams;

    for (Int32 i = ViewGroup::GetChildCount() - 1; i >= 0; i--) {
        child = ViewGroup::GetChildAt(i);
        AutoPtr<IViewGroupLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        childLayoutParams = (CIconMenuViewLayoutParams*)IIconMenuViewLayoutParams::Probe(lp);

        // Layout children according to positions set during the measure
        child->Layout(childLayoutParams->mLeft, childLayoutParams->mTop, childLayoutParams->mRight,
                childLayoutParams->mBottom);
    }
}

void IconMenuView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IDrawable> drawable = mHorizontalDivider;
    if (drawable != NULL) {
        // If we have a horizontal divider to draw, draw it at the remembered positions
        List<AutoPtr<IRect> >::ReverseIterator riter = mHorizontalDividerRects.RBegin();
        for (; riter != mHorizontalDividerRects.REnd(); ++riter) {
            IRect* rect = *riter;
            drawable->SetBoundsEx(rect);
            drawable->Draw(canvas);
        }
    }

    drawable = mVerticalDivider;
    if (drawable != NULL) {
        // If we have a vertical divider to draw, draw it at the remembered positions
        List<AutoPtr<IRect> >::ReverseIterator riter = mVerticalDividerRects.RBegin();
        for (; riter != mVerticalDividerRects.REnd(); ++riter) {
            IRect* rect = *riter;
            drawable->SetBoundsEx(rect);
            drawable->Draw(canvas);
        }
    }
}

/**
 * Figures out the layout for the menu items.
 *
 * @param width The available width for the icon menu.
 */

Boolean IconMenuView::InvokeItem(
        /* [in] */ IMenuItem* item)
{
    Boolean state;
    mMenu->PerformItemAction(item, 0, &state);
    return state;
}

AutoPtr<IViewGroupLayoutParams> IconMenuView::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<CIconMenuViewLayoutParams> lp;
    CIconMenuViewLayoutParams::NewByFriend(GetContext(), attrs, (CIconMenuViewLayoutParams**)&lp);

    return (IViewGroupLayoutParams*)lp->Probe(EIID_IViewGroupLayoutParams);
}

Boolean IconMenuView::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    // Override to allow type-checking of LayoutParams.
    return IIconMenuViewLayoutParams::Probe(p) != NULL;
}

/**
 * Marks as having stale children.
 */
void IconMenuView::MarkStaleChildren()
{
    if (!mHasStaleChildren) {
        mHasStaleChildren = true;
        RequestLayout();
    }
}

/**
 * @return The number of actual items shown (those that are backed by an
 *         {@link MenuView.ItemView} implementation--eg: excludes More
 *         item).
 */
Int32 IconMenuView::GetNumActualItemsShown()
{
    return mNumActualItemsShown;
}

Int32 IconMenuView::GetWindowAnimations()
{
    return mAnimations;
}

/**
 * Returns the number of items per row.
 * <p>
 * This should only be used for testing.
 *
 * @return The length of the array is the number of rows. A value at a
 *         position is the number of items in that row.
 * @hide
 */
ECode IconMenuView::GetLayout(
    /* [out, callee] */ ArrayOf<Int32>** layout)
{
    *layout = mLayout->Clone();
    return NOERROR;
}

/**
{ * Returns the number of rows in the layout.
 * <p>
 * This should only be used for testing.
 *
 * @return The length of the array is the number of rows. A value at a
 *         position is the number of items in that row.
 * @hide
 */
Int32 IconMenuView::GetLayoutNumRows()
{
    return mLayoutNumRows;
}

Boolean IconMenuView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    if (keyCode == KeyEvent_KEYCODE_MENU) {
        Int32 action, repeatCount;
        event->GetAction(&action);
        event->GetRepeatCount(&repeatCount);
        if (action == KeyEvent_ACTION_DOWN && repeatCount == 0) {
            RemoveCallbacks((IRunnable*)this->Probe(EIID_IRunnable));
            PostDelayed((IRunnable*)this->Probe(EIID_IRunnable), ViewConfiguration::GetLongPressTimeout());
        }
        else if (action == KeyEvent_ACTION_UP) {
            if (mMenuBeingLongpressed) {
                // It was in cycle mode, so reset it (will also remove us
                // from being called back)
                SetCycleShortcutCaptionMode(FALSE);
                return TRUE;

            }
            else {
                // Just remove us from being called back
                RemoveCallbacks((IRunnable*)this->Probe(EIID_IRunnable));
                // Fall through to normal processing too
            }
        }
    }

    return ViewGroup::DispatchKeyEvent(event);
}

//@override
void IconMenuView::OnAttachedToWindow()
{
    ViewGroup::OnAttachedToWindow();

    //RequestFocus();
    RequestFocus(0, NULL);
}

//@override
void IconMenuView::OnDetachedFromWindow()
{
    SetCycleShortcutCaptionMode(FALSE);
    ViewGroup::OnDetachedFromWindow();
}

//@override
ECode IconMenuView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    if (!hasWindowFocus) {
        SetCycleShortcutCaptionMode(FALSE);
    }

    return ViewGroup::OnWindowFocusChanged(hasWindowFocus);
}

/**
 * Sets the shortcut caption mode for IconMenuView. This mode will
 * continuously cycle between a child's shortcut and its title.
 *
 * @param cycleShortcutAndNormal Whether to go into cycling shortcut mode,
 *        or to go back to normal.
 */
void IconMenuView::SetCycleShortcutCaptionMode(
    /* [in] */ Boolean cycleShortcutAndNormal)
{
    if (!cycleShortcutAndNormal) {
        /*
         * We're setting back to title, so remove any callbacks for setting
         * to shortcut
         */
        RemoveCallbacks((IRunnable*)this->Probe(EIID_IRunnable));
        SetChildrenCaptionMode(FALSE);
        mMenuBeingLongpressed = FALSE;

    } else {
        // Set it the first time (the cycle will be started in run()).
        SetChildrenCaptionMode(TRUE);
    }
}

/**
 * When this method is invoked if the menu is currently not being
 * longpressed, it means that the longpress has just been reached (so we set
 * longpress flag, and start cycling). If it is being longpressed, we cycle
 * to the next mode.
 */
ECode IconMenuView::Run()
{
    if (mMenuBeingLongpressed) {

        // Cycle to other caption mode on the children
        SetChildrenCaptionMode(!mLastChildrenCaptionMode);
    }
    else {
        // Switch ourselves to continuously cycle the items captions
        mMenuBeingLongpressed = true;
        SetCycleShortcutCaptionMode(true);
    }

    // We should run again soon to cycle to the other caption mode
    PostDelayed((IRunnable*)this->Probe(EIID_IRunnable), ITEM_CAPTION_CYCLE_DELAY);

    return NOERROR;
}

/**
 * Iterates children and sets the desired shortcut mode. Only
 * {@link #setCycleShortcutCaptionMode(boolean)} and {@link #run()} should call
 * this.
 *
 * @param shortcut Whether to show shortcut or the title.
 */
void IconMenuView::SetChildrenCaptionMode(
    /* [in] */ Boolean shortcut)
{
    // Set the last caption mode pushed to children
    mLastChildrenCaptionMode = shortcut;

    for (Int32 i = ViewGroup::GetChildCount() - 1; i >= 0; i--) {
        AutoPtr<IView> view = ViewGroup::GetChildAt(i);
        ((CIconMenuItemView*)IIconMenuItemView::Probe(view))->SetCaptionMode(shortcut);
    }
}

/**
 * For each item, calculates the most dense row that fully shows the item's
 * title.
 *
 * @param width The available width of the icon menu.
 */
void IconMenuView::CalculateItemFittingMetadata(
    /* [in] */ Int32 width)
{
    Int32 maxNumItemsPerRow = mMaxItemsPerRow;
    Int32 numItems = ViewGroup::GetChildCount();
    for (Int32 i = 0; i < numItems; i++) {
        AutoPtr<IViewGroupLayoutParams> lp;
        ViewGroup::GetChildAt(i)->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        CIconMenuViewLayoutParams* ilp =
                (CIconMenuViewLayoutParams*)IIconMenuViewLayoutParams::Probe(lp);
        // Start with 1, since that case does not get covered in the loop below
        ilp->mMaxNumItemsOnRow = 1;
        for (Int32 curNumItemsPerRow = maxNumItemsPerRow; curNumItemsPerRow > 0;
                curNumItemsPerRow--) {
            // Check whether this item can fit into a row containing curNumItemsPerRow
            if (ilp->mDesiredWidth < width / curNumItemsPerRow) {
                // It can, mark this value as the most dense row it can fit into
                ilp->mMaxNumItemsOnRow = curNumItemsPerRow;
                break;
            }
        }
    }
}

AutoPtr<IParcelable> IconMenuView::OnSaveInstanceState()
{
    //TODO
    // AutoPtr<IParcelable> superState = NULL;
    // ViewGroup::OnSaveInstanceState((IParcelable**) &superState);

    // AutoPtr<IView> focusedView = NULL;
    // GetFocusedChild((IView**) &focusedView);

    // for (Int32 i = GetChildCount() - 1; i >= 0; i--) {
    //     AutoPtr<IView> view = NULL;
    //     GetChildAt(i, (IView**) &view);
    //     if (view == focusedView) {
    //         return new SavedState(superState, i);
    //     }
    // }

    // return new SavedState(superState, -1);

    return NULL;
}

void IconMenuView::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    //TODO
    // SavedState ss = (SavedState) state;
    // super.onRestoreInstanceState(ss.getSuperState());

    // if (ss.focusedPosition >= getChildCount()) {
    //     return;
    // }

    // View v = getChildAt(ss.focusedPosition);
    // if (v != null) {
    //     v.requestFocus();
    // }
}
