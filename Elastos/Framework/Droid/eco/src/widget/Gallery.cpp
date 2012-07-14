
#include "widget/Gallery.h"
#include "widget/CGalleryLayoutParams.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/HapticFeedbackConstants.h"
#include <elastos/Math.h>

const Int32 R_Attr_GalleryStyle = 0x01010070;

const String Gallery::TAG = String("Gallery");

const Boolean Gallery::localLOGV;

/**
 * Duration in milliseconds from the start of a scroll during which we're
 * unsure whether the user is scrolling or flinging.
 */
const Int32 Gallery::SCROLL_TO_FLING_UNCERTAINTY_TIMEOUT;

static Int32 R_Styleable_Gallery[] = {
    0x010100af, 0x01010112, 0x01010113, 0x0101020e
};

static const Int32 R_Styleable_Gallery_gravity = 0;
static const Int32 R_Styleable_Gallery_animationDuration = 1;
static const Int32 R_Styleable_Gallery_spacing = 2;
static const Int32 R_Styleable_Gallery_unselectedAlpha = 3;

Gallery::Gallery()
{

}

Gallery::Gallery(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) : AbsSpinner(context, attrs, defStyle)
{
    Init(context, attrs, defStyle);
}

Gallery::~Gallery()
{
    delete mFlingRunnable;
}

void Gallery::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs /* = NULL */,
    /* [in] */ Int32 defStyle /* = R_Attr_GalleryStyle */)
{
    mSpacing = 0;

    mAnimationDuration = 400;

    mFlingRunnable = new FlingRunnable(this);

    mShouldCallbackDuringFling = TRUE;

    /**
     * Whether to callback when an item that is not selected is clicked.
     */
    mShouldCallbackOnUnselectedItemClick = TRUE;

    //mGestureDetector = new GestureDetector(context, this);
    //mGestureDetector.setIsLongpressEnabled(TRUE);

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
        attrs, ArrayOf<Int32>(R_Styleable_Gallery, sizeof(R_Styleable_Gallery) / sizeof(Int32)), defStyle, 0, (ITypedArray**)&a);

    Int32 index;
    a->GetInt32(R_Styleable_Gallery_gravity, -1, &index);

    if (index >= 0) {
        SetGravity(index);
    }

    Int32 animationDuration;
    a->GetInt32(R_Styleable_Gallery_animationDuration, -1, &animationDuration);
    if (animationDuration > 0) {
        SetAnimationDuration(animationDuration);
    }

    Int32 spacing;
    a->GetDimensionPixelOffset(R_Styleable_Gallery_spacing, 0, &spacing);
    SetSpacing(spacing);

    Float unselectedAlpha;
    a->GetFloat(
        R_Styleable_Gallery_unselectedAlpha, 0.5f, &unselectedAlpha);
    SetUnselectedAlpha(unselectedAlpha);

    a->Recycle();

    // We draw the selected item last (because otherwise the item to the
    // right overlaps it)
    mGroupFlags |= FLAG_USE_CHILD_DRAWING_ORDER;

    mGroupFlags |= FLAG_SUPPORT_STATIC_TRANSFORMATIONS;
}

/**
 * Whether or not to callback on any {@link #getOnItemSelectedListener()}
 * while the items are being flinged. If FALSE, only the final selected item
 * will cause the callback. If TRUE, all items between the first and the
 * final will cause callbacks.
 *
 * @param shouldCallback Whether or not to callback on the listener while
 *            the items are being flinged.
 */
ECode Gallery::SetCallbackDuringFling(
    /* [in] */ Boolean shouldCallback)
{
    mShouldCallbackDuringFling = shouldCallback;

    return NOERROR;
}

/**
 * Whether or not to callback when an item that is not selected is clicked.
 * If FALSE, the item will become selected (and re-centered). If TRUE, the
 * {@link #getOnItemClickListener()} will get the callback.
 *
 * @param shouldCallback Whether or not to callback on the listener when a
 *            item that is not selected is clicked.
 * @hide
 */
ECode Gallery::SetCallbackOnUnselectedItemClick(
    /* [in] */ Boolean shouldCallback)
{
    mShouldCallbackOnUnselectedItemClick = shouldCallback;

    return NOERROR;
}

/**
 * Sets how long the transition animation should run when a child view
 * changes position. Only relevant if animation is turned on.
 *
 * @param animationDurationMillis The duration of the transition, in
 *        milliseconds.
 *
 * @attr ref android.R.styleable#Gallery_animationDuration
 */
ECode Gallery::SetAnimationDuration(
    /* [in] */ Int32 animationDurationMillis)
{
    mAnimationDuration = animationDurationMillis;

    return NOERROR;
}

/**
 * Sets the spacing between items in a Gallery
 *
 * @param spacing The spacing in pixels between items in the Gallery
 *
 * @attr ref android.R.styleable#Gallery_spacing
 */
ECode Gallery::SetSpacing(
    /* [in] */ Int32 spacing)
{
    mSpacing = spacing;

    return NOERROR;
}

/**
 * Sets the alpha of items that are not selected in the Gallery.
 *
 * @param unselectedAlpha the alpha for the items that are not selected.
 *
 * @attr ref android.R.styleable#Gallery_unselectedAlpha
 */
ECode Gallery::SetUnselectedAlpha(
    /* [in] */ Float unselectedAlpha)
{
    mUnselectedAlpha = unselectedAlpha;

    return NOERROR;
}


Boolean Gallery::GetChildStaticTransformation(
    /* [in] */ IView* child,
    /* [in] */ ITransformation* t)
{
    t->Clear();
    t->SetAlpha(child == mSelectedChild ? 1.0f : mUnselectedAlpha);

    return TRUE;
}

Int32 Gallery::ComputeHorizontalScrollExtent()
{
    // Only 1 item is considered to be selected
    return 1;
}

Int32 Gallery::ComputeHorizontalScrollOffset()
{
    // Current scroll position is the same as the selected position
    return mSelectedPosition;
}

Int32 Gallery::ComputeHorizontalScrollRange()
{
    // Scroll range is the same as the item count
    return mItemCount;
}

Boolean Gallery::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p->Probe(EIID_IGalleryLayoutParams) != NULL;
}

AutoPtr<IViewGroupLayoutParams> Gallery::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IGalleryLayoutParams> lp;
    CGalleryLayoutParams::New(p, (IGalleryLayoutParams**)&lp);
    return lp;
}

AutoPtr<IViewGroupLayoutParams> Gallery::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IGalleryLayoutParams> lp;
    CGalleryLayoutParams::New(GetContext(), attrs, (IGalleryLayoutParams**)&lp);
    return lp;
}


AutoPtr<IViewGroupLayoutParams> Gallery::GenerateDefaultLayoutParams()
{
    /*
     * Gallery expects Gallery.LayoutParams.
     */
    AutoPtr<IGalleryLayoutParams> lp;
    CGalleryLayoutParams::New(ViewGroupLayoutParams_WRAP_CONTENT,
            ViewGroupLayoutParams_WRAP_CONTENT, (IGalleryLayoutParams**)&lp);
    return lp;
}

void Gallery::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    AbsSpinner::OnLayout(changed, l, t, r, b);

    /*
     * Remember that we are in layout to prevent more layout request from
     * being generated.
     */
    mInLayout = TRUE;
    LayoutEx(0, FALSE);
    mInLayout = FALSE;
}


Int32 Gallery::GetChildHeight(
    /* [in] */ IView* child)
{
    Int32 height;
    child->GetMeasuredHeight(&height);

    return height;
}

/**
 * Tracks a motion scroll. In reality, this is used to do just about any
 * movement to items (touch scroll, arrow-key scroll, set an item as selected).
 *
 * @param deltaX Change in X from the previous event.
 */
void Gallery::TrackMotionScroll(
    /* [in] */ Int32 deltaX)
{
    if (GetChildCount() == 0) {
        return;
    }

    Boolean toLeft = deltaX < 0;

    Int32 limitedDeltaX = GetLimitedMotionScrollAmount(toLeft, deltaX);
    if (limitedDeltaX != deltaX) {
        // The above call returned a limited amount, so stop any scrolls/flings
        mFlingRunnable->EndFling(FALSE);
        OnFinishedMovement();
    }

    OffsetChildrenLeftAndRight(limitedDeltaX);

    DetachOffScreenChildren(toLeft);

    if (toLeft) {
        // If moved left, there will be empty space on the right
        FillToGalleryRight();
    } else {
        // Similarly, empty space on the left
        FillToGalleryLeft();
    }

    // Clear unused views
    mRecycler->Clear();

    SetSelectionToCenterChild();

    Invalidate();
}

Int32 Gallery::GetLimitedMotionScrollAmount(
    /* [in] */ Boolean motionToLeft,
    /* [in] */ Int32 deltaX)
{
    Int32 extremeItemPosition = motionToLeft ? mItemCount - 1 : 0;
    AutoPtr<IView> extremeChild = GetChildAt(extremeItemPosition - mFirstPosition);

    if (extremeChild.Get() == NULL) {
        return deltaX;
    }

    Int32 extremeChildCenter = GetCenterOfView(extremeChild);
    Int32 galleryCenter = GetCenterOfGallery();

    if (motionToLeft) {
        if (extremeChildCenter <= galleryCenter) {

            // The extreme child is past his boundary point!
            return 0;
        }
    } else {
        if (extremeChildCenter >= galleryCenter) {

            // The extreme child is past his boundary point!
            return 0;
        }
    }

    Int32 centerDifference = galleryCenter - extremeChildCenter;

    return motionToLeft
        ? Math::Max(centerDifference, deltaX)
        : Math::Min(centerDifference, deltaX);
}

/**
 * Offset the horizontal location of all children of this view by the
 * specified number of pixels.
 *
 * @param offset the number of pixels to offset
 */
void Gallery::OffsetChildrenLeftAndRight(
    /* [in] */ Int32 offset)
{
    for (Int32 i = GetChildCount() - 1; i >= 0; i--) {
        GetChildAt(i)->OffsetLeftAndRight(offset);
    }
}

/**
 * @return The center of this Gallery.
 */
Int32 Gallery::GetCenterOfGallery()
{
    return (GetWidth() - mPaddingLeft - mPaddingRight) / 2 + mPaddingLeft;
}

/**
 * @return The center of the given view.
 */
Int32 Gallery::GetCenterOfView(
    /* [in] */ IView* view)
{
    Int32 left, width;
    view->GetLeft(&left);
    view->GetWidth(&width);

    return left + width / 2;
}

/**
 * Detaches children that are off the screen (i.e.: Gallery bounds).
 *
 * @param toLeft Whether to detach children to the left of the Gallery, or
 *            to the right.
 */
void Gallery::DetachOffScreenChildren(
    /* [in] */ Boolean toLeft)
{
    Int32 numChildren = GetChildCount();
    Int32 firstPosition = mFirstPosition;
    Int32 start = 0;
    Int32 count = 0;

    if (toLeft) {
        Int32 galleryLeft = mPaddingLeft;
        for (Int32 i = 0; i < numChildren; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            Int32 right;
            child->GetRight(&right);
            if (right >= galleryLeft) {
                break;
            } else {
                count++;
                mRecycler->Put(firstPosition + i, child);
            }
        }
    } else {
        Int32 galleryRight = GetWidth() - mPaddingRight;
        for (Int32 i = numChildren - 1; i >= 0; i--) {
            AutoPtr<IView> child = GetChildAt(i);
            Int32 left;
            child->GetLeft(&left);
            if (left <= galleryRight) {
                break;
            } else {
                start = i;
                count++;
                mRecycler->Put(firstPosition + i, child);
            }
        }
    }

    DetachViewsFromParent(start, count);

    if (toLeft) {
        mFirstPosition += count;
    }
}

/**
 * Scrolls the items so that the selected item is in its 'slot' (its center
 * is the gallery's center).
 */
void Gallery::ScrollIntoSlots()
{
    if (GetChildCount() == 0 || mSelectedChild == NULL) return;

    Int32 selectedCenter = GetCenterOfView(mSelectedChild);
    Int32 targetCenter = GetCenterOfGallery();

    Int32 scrollAmount = targetCenter - selectedCenter;
    if (scrollAmount != 0) {
        mFlingRunnable->StartUsingDistance(scrollAmount);
    } else {
        OnFinishedMovement();
    }
}

void Gallery::OnFinishedMovement()
{
    if (mSuppressSelectionChanged) {
        mSuppressSelectionChanged = FALSE;

        // We haven't been callbacking during the fling, so do it now
        AbsSpinner::SelectionChanged();
    }
    Invalidate();
}


void Gallery::SelectionChanged()
{
    if (!mSuppressSelectionChanged) {
        AbsSpinner::SelectionChanged();
    }
}

/**
 * Looks for the child that is closest to the center and sets it as the
 * selected child.
 */
void Gallery::SetSelectionToCenterChild()
{

    AutoPtr<IView> selView = mSelectedChild;
    if (mSelectedChild == NULL) return;

    Int32 galleryCenter = GetCenterOfGallery();

    // Common case where the current selected position is correct
    Int32 left, right;
    selView->GetLeft(&left);
    selView->GetRight(&right);
    if (left <= galleryCenter && right >= galleryCenter) {
        return;
    }

    // TODO better search
    Int32 closestEdgeDistance = Math::INT32_MAX_VALUE;
    Int32 newSelectedChildIndex = 0;
    for (Int32 i = GetChildCount() - 1; i >= 0; i--) {

        AutoPtr<IView> child = GetChildAt(i);

        child->GetLeft(&left);
        child->GetRight(&right);
        if (left <= galleryCenter && right >= galleryCenter) {
            // This child is in the center
            newSelectedChildIndex = i;
            break;
        }

        Int32 childClosestEdgeDistance = Math::Min(Math::Abs(left - galleryCenter),
            Math::Abs(right - galleryCenter));
        if (childClosestEdgeDistance < closestEdgeDistance) {
            closestEdgeDistance = childClosestEdgeDistance;
            newSelectedChildIndex = i;
        }
    }

    Int32 newPos = mFirstPosition + newSelectedChildIndex;

    if (newPos != mSelectedPosition) {
        SetSelectedPositionInt(newPos);
        SetNextSelectedPositionInt(newPos);
        CheckSelectionChanged();
    }
}

/**
 * Creates and positions all views for this Gallery.
 * <p>
 * We layout rarely, most of the time {@link #trackMotionScroll(Int32)} takes
 * care of repositioning, adding, and removing children.
 *
 * @param delta Change in the selected position. +1 means the selection is
 *            moving to the right, so views are scrolling to the left. -1
 *            means the selection is moving to the left.
 */

ECode Gallery::LayoutEx(
    /* [in] */ Int32 delta,
    /* [in] */ Boolean animate)
{
    Int32 childrenLeft = mSpinnerPadding->mLeft;
    Int32 childrenWidth = mRight - mLeft - mSpinnerPadding->mLeft - mSpinnerPadding->mRight;

    if (mDataChanged) {
        HandleDataChanged();
    }

    // Handle an empty gallery by removing all views.
    if (mItemCount == 0) {
        ResetList();
        return NOERROR;
    }

    // Update to the new selected position.
    if (mNextSelectedPosition >= 0) {
        SetSelectedPositionInt(mNextSelectedPosition);
    }

    // All views go in recycler while we are in layout
    RecycleAllViews();

    // Clear out old views
    //removeAllViewsInLayout();
    DetachAllViewsFromParent();

    /*
     * These will be used to give initial positions to views entering the
     * gallery as we scroll
     */
    mRightMost = 0;
    mLeftMost = 0;

    // Make selected view and center it

    /*
     * mFirstPosition will be decreased as we add views to the left later
     * on. The 0 for x will be offset in a couple lines down.
     */
    mFirstPosition = mSelectedPosition;
    AutoPtr<IView> sel = MakeAndAddView(mSelectedPosition, 0, 0, TRUE);

    // Put the selected child in the center
    Int32 width;
    sel->GetWidth(&width);
    Int32 selectedOffset = childrenLeft + (childrenWidth / 2) - (width / 2);
    sel->OffsetLeftAndRight(selectedOffset);

    FillToGalleryRight();
    FillToGalleryLeft();

    // Flush any cached views that did not get reused above
    mRecycler->Clear();

    Invalidate();
    CheckSelectionChanged();

    mDataChanged = FALSE;
    mNeedSync = FALSE;
    SetNextSelectedPositionInt(mSelectedPosition);

    UpdateSelectedItemMetadata();

    return NOERROR;
}

void Gallery::FillToGalleryLeft()
{
    Int32 itemSpacing = mSpacing;
    Int32 galleryLeft = mPaddingLeft;

    // Set state for initial iteration
    AutoPtr<IView> prevIterationView = GetChildAt(0);
    Int32 curPosition;
    Int32 curRightEdge;

    Int32 left;
    if (prevIterationView.Get() != NULL) {
        curPosition = mFirstPosition - 1;
        prevIterationView->GetLeft(&left);
        curRightEdge = left - itemSpacing;
    } else {
        // No children available!
        curPosition = 0;
        curRightEdge = mRight - mLeft - mPaddingRight;
        mShouldStopFling = TRUE;
    }

    while (curRightEdge > galleryLeft && curPosition >= 0) {
        prevIterationView = MakeAndAddView(curPosition, curPosition - mSelectedPosition,
                curRightEdge, FALSE);

        // Remember some state
        mFirstPosition = curPosition;

        // Set state for next iteration
        prevIterationView->GetLeft(&left);
        curRightEdge = left - itemSpacing;
        curPosition--;
    }
}

void Gallery::FillToGalleryRight()
{
    Int32 itemSpacing = mSpacing;
    Int32 galleryRight = mRight - mLeft - mPaddingRight;
    Int32 numChildren = GetChildCount();
    Int32 numItems = mItemCount;

    // Set state for initial iteration
    AutoPtr<IView> prevIterationView = GetChildAt(numChildren - 1);
    Int32 curPosition;
    Int32 curLeftEdge;

    Int32 right;
    if (prevIterationView.Get() != NULL) {
        curPosition = mFirstPosition + numChildren;
        prevIterationView->GetRight(&right);
        curLeftEdge = right + itemSpacing;
    } else {
        mFirstPosition = curPosition = mItemCount - 1;
        curLeftEdge = mPaddingLeft;
        mShouldStopFling = TRUE;
    }

    while (curLeftEdge < galleryRight && curPosition < numItems) {
        prevIterationView = MakeAndAddView(curPosition, curPosition - mSelectedPosition,
                curLeftEdge, TRUE);

        // Set state for next iteration
        prevIterationView->GetRight(&right);
        curLeftEdge = right + itemSpacing;
        curPosition++;
    }
}

/**
 * Obtain a view, either by pulling an existing view from the recycler or by
 * getting a new one from the adapter. If we are animating, make sure there
 * is enough information in the view's layout parameters to animate from the
 * old to new positions.
 *
 * @param position Position in the gallery for the view to obtain
 * @param offset Offset from the selected position
 * @param x X-coordintate indicating where this view should be placed. This
 *        will either be the left or right edge of the view, depending on
 *        the fromLeft paramter
 * @param fromLeft Are we posiitoning views based on the left edge? (i.e.,
 *        building from left to right)?
 * @return A view that has been added to the gallery
 */
AutoPtr<IView> Gallery::MakeAndAddView(
    /* [in] */ Int32 position,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 x,
    /* [in] */ Boolean fromLeft)
{
    AutoPtr<IView> child;

    if (!mDataChanged) {
        child = mRecycler->Get(position);
        if (child.Get() != NULL) {
            // Can reuse an existing view
            Int32 childLeft;
            child->GetLeft(&childLeft);

            Int32 width;
            child->GetMeasuredWidth(&width);
            // Remember left and right edges of where views have been placed
            mRightMost = Math::Max(mRightMost, childLeft
                    + width);
            mLeftMost = Math::Min(mLeftMost, childLeft);

            // Position the view
            SetUpChild(child, offset, x, fromLeft);

            return child;
        }
    }

    // Nothing found in the recycler -- ask the adapter for a view
    mAdapter->GetView(position, NULL, (IViewGroup*)this->Probe(EIID_IViewGroup), (IView**)&child);

    // Position the view
    SetUpChild(child, offset, x, fromLeft);

    return child;
}

/**
 * Helper for makeAndAddView to set the position of a view and fill out its
 * layout paramters.
 *
 * @param child The view to position
 * @param offset Offset from the selected position
 * @param x X-coordintate indicating where this view should be placed. This
 *        will either be the left or right edge of the view, depending on
 *        the fromLeft paramter
 * @param fromLeft Are we posiitoning views based on the left edge? (i.e.,
 *        building from left to right)?
 */
void Gallery::SetUpChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 x,
    /* [in] */ Boolean fromLeft)
{

    // Respect layout params that are already in the view. Otherwise
    // make some up...
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp.Get() == NULL) {
        lp = GenerateDefaultLayoutParams();
    }

    AddViewInLayout(child, fromLeft ? -1 : 0, lp);

    child->SetSelected(offset == 0);

    // Get measure specs
    Int32 childHeightSpec = ViewGroup::GetChildMeasureSpec(mHeightMeasureSpec,
            mSpinnerPadding->mTop + mSpinnerPadding->mBottom, ((CViewGroupLayoutParams*)lp.Get())->mHeight);
    Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(mWidthMeasureSpec,
            mSpinnerPadding->mLeft + mSpinnerPadding->mRight, ((CViewGroupLayoutParams*)lp.Get())->mWidth);

    // Measure child
    child->Measure(childWidthSpec, childHeightSpec);

    Int32 childLeft;
    Int32 childRight;

    // Position vertically based on gravity setting
    Int32 childTop = CalculateTop(child, TRUE);

    Int32 height;
    child->GetMeasuredHeight(&height);
    Int32 childBottom = childTop + height;

    Int32 width;
    child->GetMeasuredWidth(&width);
    if (fromLeft) {
        childLeft = x;
        childRight = childLeft + width;
    } else {
        childLeft = x - width;
        childRight = x;
    }

    child->Layout(childLeft, childTop, childRight, childBottom);
}

/**
 * Figure out vertical placement based on mGravity
 *
 * @param child Child to place
 * @return Where the top of the child should be
 */
Int32 Gallery::CalculateTop(
    /* [in] */ IView* child,
    /* [in] */ Boolean duringLayout)
{
    Int32 myHeight = duringLayout ? mMeasuredHeight : GetHeight();

    Int32 measuredHeight, height;
    child->GetMeasuredHeight(&measuredHeight);
    child->GetHeight(&height);
    Int32 childHeight = duringLayout ? measuredHeight : height;

    Int32 childTop = 0;
    Int32 availableSpace;

    switch (mGravity) {
    case Gravity_TOP:
        childTop = mSpinnerPadding->mTop;
        break;
    case Gravity_CENTER_VERTICAL:
        availableSpace = myHeight - mSpinnerPadding->mBottom
                - mSpinnerPadding->mTop - childHeight;
        childTop = mSpinnerPadding->mTop + (availableSpace / 2);
        break;
    case Gravity_BOTTOM:
        childTop = myHeight - mSpinnerPadding->mBottom - childHeight;
        break;
    }
    return childTop;
}

Boolean Gallery::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    // Give everything to the gesture detector
    Boolean retValue;// = mGestureDetector.onTouchEvent(event);

    Int32 action;
    event->GetAction(&action);
    if (action == MotionEvent_ACTION_UP) {
        // Helper method for lifted finger
        OnUp();
    } else if (action == MotionEvent_ACTION_CANCEL) {
        OnCancel();
    }

    return retValue;
}

/**
 * {@inheritDoc}
 */
Boolean Gallery::OnSingleTapUp(
    /* [in] */ IMotionEvent* e)
{
    if (mDownTouchPosition >= 0) {

        // An item tap should make it selected, so scroll to this child.
        ScrollToChild(mDownTouchPosition - mFirstPosition);

        // Also pass the click so the client knows, if it wants to.
        if (mShouldCallbackOnUnselectedItemClick || mDownTouchPosition == mSelectedPosition) {
            Int64 id;
            mAdapter->GetItemId(mDownTouchPosition, &id);
            PerformItemClick(mDownTouchView, mDownTouchPosition, id);
        }

        return TRUE;
    }

    return FALSE;
}

/**
 * {@inheritDoc}
 */
Boolean Gallery::OnFling(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY)
{

    if (!mShouldCallbackDuringFling) {
        // We want to suppress selection changes

        // Remove any future code to set mSuppressSelectionChanged = FALSE
        RemoveCallbacks(mDisableSuppressSelectionChangedRunnable);

        // This will get reset once we scroll into slots
        if (!mSuppressSelectionChanged) mSuppressSelectionChanged = TRUE;
    }

    // Fling the gallery!
    mFlingRunnable->StartUsingVelocity((Int32) -velocityX);

    return TRUE;
}

/**
 * {@inheritDoc}
 */
Boolean Gallery::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY)
{

    //if (localLOGV) Log.v(TAG, String.valueOf(e2.getX() - e1.getX()));

    /*
     * Now's a good time to tell our parent to stop intercepting our events!
     * The user has moved more than the slop amount, since GestureDetector
     * ensures this before calling this method. Also, if a parent is more
     * interested in this touch's events than we are, it would have
     * intercepted them by now (for example, we can assume when a Gallery is
     * in the ListView, a vertical scroll would not end up in this method
     * since a ListView would have intercepted it by now).
     */
    mParent->RequestDisallowInterceptTouchEvent(TRUE);

    // As the user scrolls, we want to callback selection changes so related-
    // info on the screen is up-to-date with the gallery's selection
    if (!mShouldCallbackDuringFling) {
        if (mIsFirstScroll) {
            /*
             * We're not notifying the client of selection changes during
             * the fling, and this scroll could possibly be a fling. Don't
             * do selection changes until we're sure it is not a fling.
             */
            if (!mSuppressSelectionChanged) mSuppressSelectionChanged = TRUE;
            PostDelayed(mDisableSuppressSelectionChangedRunnable, SCROLL_TO_FLING_UNCERTAINTY_TIMEOUT);
        }
    } else {
        if (mSuppressSelectionChanged) mSuppressSelectionChanged = FALSE;
    }

    // Track the motion
    TrackMotionScroll(-1 * (Int32) distanceX);

    mIsFirstScroll = FALSE;
    return TRUE;
}

/**
 * {@inheritDoc}
 */
Boolean Gallery::OnDown(
    /* [in] */ IMotionEvent* e)
{

    // Kill any existing fling/scroll
    mFlingRunnable->Stop(FALSE);

    // Get the item's view that was touched
    Float X, Y;
    e->GetX(&X);
    e->GetY(&Y);
    mDownTouchPosition = PointToPosition((Int32) X, (Int32) Y);

    if (mDownTouchPosition >= 0) {
        mDownTouchView = GetChildAt(mDownTouchPosition - mFirstPosition);
        mDownTouchView->SetPressed(TRUE);
    }

    // Reset the multiple-scroll tracking state
    mIsFirstScroll = TRUE;

    // Must return TRUE to get matching events for this down event.
    return TRUE;
}

/**
 * Called when a touch event's action is MotionEvent.ACTION_UP.
 */
void Gallery::OnUp()
{
    if (mFlingRunnable->mScroller->IsFinished()) {
        ScrollIntoSlots();
    }

    DispatchUnpress();
}

/**
 * Called when a touch event's action is MotionEvent.ACTION_CANCEL.
 */
void Gallery::OnCancel()
{
    OnUp();
}

/**
 * {@inheritDoc}
 */
ECode Gallery::OnLongPress(
    /* [in] */ IMotionEvent* e)
{
    if (mDownTouchPosition < 0) {
        return NOERROR;
    }

    PerformHapticFeedback(HapticFeedbackConstants::LONG_PRESS);
    Int64 id = GetItemIdAtPosition(mDownTouchPosition);
    DispatchLongPress(mDownTouchView, mDownTouchPosition, id);

    return NOERROR;
}

// Unused methods from GestureDetector.OnGestureListener below

/**
 * {@inheritDoc}
 */
ECode Gallery::OnShowPress(
    /* [in] */ IMotionEvent* e)
{
    return NOERROR;
}

// Unused methods from GestureDetector.OnGestureListener above

void Gallery::DispatchPress(
    /* [in] */ IView* child)
{
    if (child != NULL) {
        child->SetPressed(TRUE);
    }

    SetPressed(TRUE);
}

void Gallery::DispatchUnpress()
{
    for (Int32 i = GetChildCount() - 1; i >= 0; i--) {
        GetChildAt(i)->SetPressed(FALSE);
    }

    SetPressed(FALSE);
}

void Gallery::DispatchSetSelected(
    /* [in] */ Boolean selected)
{
    /*
     * We don't want to pass the selected state given from its parent to its
     * children since this widget itself has a selected state to give to its
     * children.
     */
}


void Gallery::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
    // Show the pressed state on the selected child
    if (mSelectedChild.Get() != NULL) {
        mSelectedChild->SetPressed(pressed);
    }
}


AutoPtr<IContextMenuInfo> Gallery::GetContextMenuInfo()
{
    return AutoPtr<IContextMenuInfo>(mContextMenuInfo.Get());
}


Boolean Gallery::ShowContextMenuForChild(
    /* [in] */ IView* originalView)
{

    Int32 longPressPosition = GetPositionForView(originalView);
    if (longPressPosition < 0) {
        return FALSE;
    }

    Int64 longPressId;
    mAdapter->GetItemId(longPressPosition, &longPressId);
    return DispatchLongPress(originalView, longPressPosition, longPressId);
}

Boolean Gallery::ShowContextMenu()
{
    if (IsPressed() && mSelectedPosition >= 0) {
        Int32 index = mSelectedPosition - mFirstPosition;
        AutoPtr<IView> v = GetChildAt(index);
        return DispatchLongPress(v, mSelectedPosition, mSelectedRowId);
    }

    return FALSE;
}

Boolean Gallery::DispatchLongPress(
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Boolean handled = FALSE;

    if (mOnItemLongClickListener != NULL) {
        mOnItemLongClickListener->OnItemLongClick( (IAdapterView*)this->Probe(EIID_IAdapterView), mDownTouchView,
                mDownTouchPosition, id, &handled);
    }

    if (!handled) {
        mContextMenuInfo = new AdapterContextMenuInfo(view, position, id);
        handled = AbsSpinner::ShowContextMenuForChild((IView*)this->Probe(EIID_IView));
    }

    if (handled) {
        PerformHapticFeedback(HapticFeedbackConstants_LONG_PRESS);
    }

    return handled;
}

Boolean Gallery::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // Gallery steals all key events
    Boolean result;
    event->DispatchEx(
        (IKeyEventCallback*)this->Probe(EIID_IKeyEventCallback),
        NULL, NULL, &result);

    return result;
}

/**
 * Handles left, right, and clicking
 * @see android.view.View#onKeyDown
 */

Boolean Gallery::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    switch (keyCode) {

    case KeyEvent_KEYCODE_DPAD_LEFT:
        if (MovePrevious()) {
            PlaySoundEffect(SoundEffectConstants_NAVIGATION_LEFT);
        }
        return TRUE;

    case KeyEvent_KEYCODE_DPAD_RIGHT:
        if (MoveNext()) {
            PlaySoundEffect(SoundEffectConstants_NAVIGATION_RIGHT);
        }
        return TRUE;

    case KeyEvent_KEYCODE_DPAD_CENTER:
    case KeyEvent_KEYCODE_ENTER:
        mReceivedInvokeKeyDown = TRUE;
        // fallthrough to default handling
    }

    return AbsSpinner::OnKeyDown(keyCode, event);
}


Boolean Gallery::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    switch (keyCode) {
    case KeyEvent_KEYCODE_DPAD_CENTER:
    case KeyEvent_KEYCODE_ENTER: {

        if (mReceivedInvokeKeyDown) {
            if (mItemCount > 0) {

                DispatchPress(mSelectedChild);
                //PostDelayed(new Runnable(), ViewConfiguration::GetPressedStateDuration());

                Int32 selectedIndex = mSelectedPosition - mFirstPosition;

                Int64 id;
                mAdapter->GetItemId(mSelectedPosition, &id);
                PerformItemClick(GetChildAt(selectedIndex), mSelectedPosition, id);
            }
        }

        // Clear the flag
        mReceivedInvokeKeyDown = FALSE;

        return TRUE;
    }
    }

    return AbsSpinner::OnKeyUp(keyCode, event);
}

Boolean Gallery::MovePrevious()
{
    if (mItemCount > 0 && mSelectedPosition > 0) {
        ScrollToChild(mSelectedPosition - mFirstPosition - 1);
        return TRUE;
    } else {
        return FALSE;
    }
}

Boolean Gallery::MoveNext()
{
    if (mItemCount > 0 && mSelectedPosition < mItemCount - 1) {
        ScrollToChild(mSelectedPosition - mFirstPosition + 1);
        return TRUE;
    } else {
        return FALSE;
    }
}

Boolean Gallery::ScrollToChild(
    /* [in] */ Int32 childPosition)
{
    AutoPtr<IView> child = GetChildAt(childPosition);

    if (child.Get() != NULL) {
        Int32 distance = GetCenterOfGallery() - GetCenterOfView(child);
        mFlingRunnable->StartUsingDistance(distance);
        return TRUE;
    }

    return FALSE;
}


void Gallery::SetSelectedPositionInt(
    /* [in] */ Int32 position)
{
    AbsSpinner::SetSelectedPositionInt(position);

    // Updates any metadata we keep about the selected item.
    UpdateSelectedItemMetadata();
}

void Gallery::UpdateSelectedItemMetadata()
{

    AutoPtr<IView> oldSelectedChild = mSelectedChild;

    AutoPtr<IView> child = mSelectedChild = GetChildAt(mSelectedPosition - mFirstPosition);
    if (child.Get() == NULL) {
        return;
    }

    child->SetSelected(TRUE);
    child->SetFocusable(TRUE);

    Boolean result;
    if (HasFocus()) {
        child->RequestFocus(&result);
    }

    // We unfocus the old child down here so the above hasFocus check
    // returns TRUE
    if (oldSelectedChild.Get() != NULL) {

        // Make sure its drawable state doesn't contain 'selected'
        oldSelectedChild->SetSelected(FALSE);

        // Make sure it is not focusable anymore, since otherwise arrow keys
        // can make this one be focused
        oldSelectedChild->SetFocusable(FALSE);
    }

}

/**
 * Describes how the child views are aligned.
 * @param gravity
 *
 * @attr ref android.R.styleable#Gallery_gravity
 */
ECode Gallery::SetGravity(
    /* [in] */ Int32 gravity)
{
    if (mGravity != gravity) {
        mGravity = gravity;
        RequestLayout();
    }

    return NOERROR;
}


Int32 Gallery::GetChildDrawingOrder(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 i)
{
    Int32 selectedIndex = mSelectedPosition - mFirstPosition;

    // Just to be safe
    if (selectedIndex < 0) return i;

    if (i == childCount - 1) {
        // Draw the selected child last
        return selectedIndex;
    } else if (i >= selectedIndex) {
        // Move the children to the right of the selected child earlier one
        return i + 1;
    } else {
        // Keep the children to the left of the selected child the same
        return i;
    }
}


void Gallery::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AbsSpinner::OnFocusChanged(gainFocus, direction, previouslyFocusedRect);

    /*
     * The gallery shows focus by focusing the selected item. So, give
     * focus to our selected item instead. We steal keys from our
     * selected item elsewhere.
     */
    if (gainFocus && mSelectedChild.Get() != NULL) {
        Boolean isFocus;
        mSelectedChild->RequestFocusEx(direction, &isFocus);
    }

}

Gallery::FlingRunnable::FlingRunnable(
    /* [in] */ Gallery* host)
{
    mHost = host;
    mScroller = new Scroller(mHost->GetContext());
}

Gallery::FlingRunnable::~FlingRunnable()
{
    delete mScroller;
}

void Gallery::FlingRunnable::StartCommon()
{
    // Remove any pending flings
    mHost->RemoveCallbacks((IRunnable*)mHost->Probe(EIID_IRunnable));
}

void Gallery::FlingRunnable::StartUsingVelocity(
    /* [in] */ Int32 initialVelocity)
{
    if (initialVelocity == 0) return;

    StartCommon();

    Int32 initialX = initialVelocity < 0 ? Math::INT32_MAX_VALUE : 0;
    mLastFlingX = initialX;
    mScroller->Fling(initialX, 0, initialVelocity, 0,
        0, Math::INT32_MAX_VALUE, 0, Math::INT32_MAX_VALUE);
    mHost->Post((IRunnable*)mHost->Probe(EIID_IRunnable));
}

void Gallery::FlingRunnable::StartUsingDistance(
    /* [in] */ Int32 distance)
{
    if (distance == 0) return;

    StartCommon();

    mLastFlingX = 0;
    mScroller->StartScroll(0, 0, -distance, 0, mHost->mAnimationDuration);
    mHost->Post((IRunnable*)mHost->Probe(EIID_IRunnable));
}

void Gallery::FlingRunnable::Stop(
    /* [in] */ Boolean scrollIntoSlots)
{
    mHost->RemoveCallbacks((IRunnable*)mHost->Probe(EIID_IRunnable));
    EndFling(scrollIntoSlots);
}

void Gallery::FlingRunnable::EndFling(
    /* [in] */ Boolean scrollIntoSlots)
{
    /*
     * Force the scroller's status to finished (without setting its
     * position to the end)
     */
    mScroller->ForceFinished(TRUE);

    if (scrollIntoSlots) mHost->ScrollIntoSlots();
}

void Gallery::FlingRunnable::Run()
{

    if (mHost->mItemCount == 0) {
        EndFling(TRUE);
        return;
    }

    mHost->mShouldStopFling = FALSE;

    Scroller* scroller = mScroller;
    Boolean more = scroller->ComputeScrollOffset();
    Int32 x = scroller->GetCurrX();

    // Flip sign to convert finger direction to list items direction
    // (e.g. finger moving down means list is moving towards the top)
    Int32 delta = mLastFlingX - x;

    // Pretend that each frame of a fling scroll is a touch scroll
    if (delta > 0) {
        // Moving towards the left. Use first view as mDownTouchPosition
        mHost->mDownTouchPosition = mHost->mFirstPosition;

        // Don't fling more than 1 screen
        delta = Math::Min(mHost->GetWidth() - mHost->mPaddingLeft - mHost->mPaddingRight - 1, delta);
    } else {
        // Moving towards the right. Use last view as mDownTouchPosition
        Int32 offsetToLast = mHost->GetChildCount() - 1;
        mHost->mDownTouchPosition = mHost->mFirstPosition + offsetToLast;

        // Don't fling more than 1 screen
        delta = Math::Max(-(mHost->GetWidth() - mHost->mPaddingRight - mHost->mPaddingLeft - 1), delta);
    }

    mHost->TrackMotionScroll(delta);

    if (more && !mHost->mShouldStopFling) {
        mLastFlingX = x;
        mHost->Post((IRunnable*)mHost->Probe(EIID_IRunnable));
    } else {
       EndFling(TRUE);
    }
}
