
#ifndef __GALLERY_H__
#define __GALLERY_H__

#include "widget/AbsSpinner.h"
#include "widget/Scroller.h"
#include "view/GestureDetector.h"

/**
 * A view that shows items in a center-locked, horizontally scrolling list.
 * <p>
 * The default values for the Gallery assume you will be using
 * {@link android.R.styleable#Theme_galleryItemBackground} as the background for
 * each View given to the Gallery from the Adapter. If you are not doing this,
 * you may need to adjust some Gallery properties, such as the spacing.
 * <p>
 * Views given to the Gallery should use {@link Gallery.LayoutParams} as their
 * layout parameters type.
 *
 * <p>See the <a href="{@docRoot}resources/tutorials/views/hello-gallery.html">Gallery
 * tutorial</a>.</p>
 *
 * @attr ref android.R.styleable#Gallery_animationDuration
 * @attr ref android.R.styleable#Gallery_spacing
 * @attr ref android.R.styleable#Gallery_gravity
 */

class Gallery : public AbsSpinner, public GestureDetector::OnGestureListener
{
private:
    /**
     * Responsible for fling behavior. Use {@link #startUsingVelocity(Int32)} to
     * initiate a fling. Each frame of the fling is handled in {@link #run()}.
     * A FlingRunnable will keep re-posting itself until the fling is done.
     *
     */
    class FlingRunnable : public Runnable
    {
        friend class Gallery;

    public:
        FlingRunnable(
            /* [in] */ Gallery* host);

        ~FlingRunnable();

        CARAPI_(void) StartUsingVelocity(
            /* [in] */ Int32 initialVelocity);

        CARAPI_(void) StartUsingDistance(
            /* [in] */ Int32 distance);

        CARAPI_(void) Stop(
            /* [in] */ Boolean scrollIntoSlots);

        CARAPI_(void) StartCommon();

        CARAPI_(void) EndFling(
            /* [in] */ Boolean scrollIntoSlots);

        CARAPI Run();

    private:

        /**
         * Tracks the decay of a fling scroll
         */
        Scroller* mScroller;

        /**
         * X value reported by mScroller on the previous fling
         */
        Int32 mLastFlingX;

        Gallery* mHost;
    };

    class DisableSuppressSelectionChangedRunnable : public Runnable
    {
    public:
        DisableSuppressSelectionChangedRunnable(
            /* [in] */ Gallery* host);

        CARAPI Run();

    private:
        Gallery* mHost;
    };

    class KeyUpRunnable : public Runnable
    {
    public:
        KeyUpRunnable(
            /* [in] */ Gallery* host);

        CARAPI Run();

    private:
        Gallery* mHost;
    };

public:
    Gallery(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010070/*R.attr.galleryStyle*/);

    ~Gallery();

    /**
     * Whether or not to callback on any {@link #getOnItemSelectedListener()}
     * while the items are being flinged. If FALSE, only the final selected item
     * will cause the callback. If TRUE, all items between the first and the
     * final will cause callbacks.
     *
     * @param shouldCallback Whether or not to callback on the listener while
     *            the items are being flinged.
     */
    virtual CARAPI SetCallbackDuringFling(
        /* [in] */ Boolean shouldCallback);

    /**
     * Whether or not to callback when an item that is not selected is clicked.
     * If FALSE, the item will become selected (and re-centered). If TRUE, the
     * {@link #getOnItemClickListener()} will get the callback.
     *
     * @param shouldCallback Whether or not to callback on the listener when a
     *            item that is not selected is clicked.
     * @hide
     */
    virtual CARAPI SetCallbackOnUnselectedItemClick(
        /* [in] */ Boolean shouldCallback);

    /**
     * Sets how long the transition animation should run when a child view
     * changes position. Only relevant if animation is turned on.
     *
     * @param animationDurationMillis The duration of the transition, in
     *        milliseconds.
     *
     * @attr ref android.R.styleable#Gallery_animationDuration
     */
    virtual CARAPI SetAnimationDuration(
        /* [in] */ Int32 animationDurationMillis);

    /**
     * Sets the spacing between items in a Gallery
     *
     * @param spacing The spacing in pixels between items in the Gallery
     *
     * @attr ref android.R.styleable#Gallery_spacing
     */
    virtual CARAPI SetSpacing(
        /* [in] */ Int32 spacing);

    /**
     * Sets the alpha of items that are not selected in the Gallery.
     *
     * @param unselectedAlpha the alpha for the items that are not selected.
     *
     * @attr ref android.R.styleable#Gallery_unselectedAlpha
     */
    virtual CARAPI SetUnselectedAlpha(
        /* [in] */ Float unselectedAlpha);

    //@Override
    CARAPI_(Int32) GetChildHeight(
        /* [in] */ IView* child);

    /**
     * Tracks a motion scroll. In reality, this is used to do just about any
     * movement to items (touch scroll, arrow-key scroll, set an item as selected).
     *
     * @param deltaX Change in X from the previous event.
     */
    virtual CARAPI_(void) TrackMotionScroll(
        /* [in] */ Int32 deltaX);

    virtual CARAPI_(Int32) GetLimitedMotionScrollAmount(
        /* [in] */ Boolean motionToLeft,
        /* [in] */ Int32 deltaX);

    //@Override
    CARAPI_(void) SelectionChanged();

    using AdapterView::Layout;

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
    //@Override
    CARAPI_(void) Layout(
        /* [in] */ Int32 delta,
        /* [in] */ Boolean animate);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(Boolean) OnSingleTapUp(
        /* [in] */ IMotionEvent* e);

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(Boolean) OnFling(
        /* [in] */ IMotionEvent* e1,
        /* [in] */ IMotionEvent* e2,
        /* [in] */ Float velocityX,
        /* [in] */ Float velocityY);

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(Boolean) OnScroll(
        /* [in] */ IMotionEvent* e1,
        /* [in] */ IMotionEvent* e2,
        /* [in] */ Float distanceX,
        /* [in] */ Float distanceY);

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(Boolean) OnDown(
        /* [in] */ IMotionEvent* e);

    /**
     * Called when a touch event's action is MotionEvent.ACTION_UP.
     */
    virtual CARAPI_(void) OnUp();

    /**
     * Called when a touch event's action is MotionEvent.ACTION_CANCEL.
     */
    virtual CARAPI_(void) OnCancel();

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(void) OnLongPress(
        /* [in] */ IMotionEvent* e);

    // Unused methods from GestureDetector.OnGestureListener below

    /**
     * {@inheritDoc}
     */
    CARAPI_(void) OnShowPress(
        /* [in] */ IMotionEvent* e);

    //@Override
    CARAPI_(void) DispatchSetSelected(
        /* [in] */ Boolean selected);

    //@Override
    CARAPI_(Boolean) ShowContextMenuForChild(
        /* [in] */ IView* originalView);

    //@Override
    CARAPI_(Boolean) ShowContextMenu();

    //@Override
    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
     * Handles left, right, and clicking
     * @see android.view.View#onKeyDown
     */
    //@Override
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) MovePrevious();

    virtual CARAPI_(Boolean) MoveNext();

    //@Override
    CARAPI_(void) SetSelectedPositionInt(
        /* [in] */ Int32 position);

    /**
     * Describes how the child views are aligned.
     * @param gravity
     *
     * @attr ref android.R.styleable#Gallery_gravity
     */
    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity);


protected:
    Gallery();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010070/*R.attr.galleryStyle*/);

    //@Override
    CARAPI_(Boolean) GetChildStaticTransformation(
        /* [in] */ IView* child,
        /* [in] */ ITransformation* t);

    //@Override
    CARAPI_(Int32) ComputeHorizontalScrollExtent();

    //@Override
    CARAPI_(Int32) ComputeHorizontalScrollOffset();

    //@Override
    CARAPI_(Int32) ComputeHorizontalScrollRange();

    //@Override
    CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateDefaultLayoutParams();

    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    //@Override
    CARAPI_(void) DispatchSetPressed(
        /* [in] */ Boolean pressed);

    //@Override
    CARAPI_(AutoPtr<IContextMenuInfo>) GetContextMenuInfo();

    //@Override
    CARAPI_(Int32) GetChildDrawingOrder(
        /* [in] */ Int32 childCount,
        /* [in] */ Int32 i);

    //@Override
    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean gainFocus,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);


private:
    /**
     * Offset the horizontal location of all children of this view by the
     * specified number of pixels.
     *
     * @param offset the number of pixels to offset
     */

    CARAPI_(void) OffsetChildrenLeftAndRight(
        /* [in] */ Int32 offset);

    /**
     * @return The center of this Gallery.
     */
    CARAPI_(Int32) GetCenterOfGallery();

    /**
     * @return The center of the given view.
     */
    static CARAPI_(Int32) GetCenterOfView(
        /* [in] */ IView* view);

    /**
     * Detaches children that are off the screen (i.e.: Gallery bounds).
     *
     * @param toLeft Whether to detach children to the left of the Gallery, or
     *            to the right.
     */
    CARAPI_(void) DetachOffScreenChildren(
        /* [in] */ Boolean toLeft);

    /**
     * Scrolls the items so that the selected item is in its 'slot' (its center
     * is the gallery's center).
     */
    CARAPI_(void) ScrollIntoSlots();

    CARAPI_(void) OnFinishedMovement();

    /**
     * Looks for the child that is closest to the center and sets it as the
     * selected child.
     */
    CARAPI_(void) SetSelectionToCenterChild();

    CARAPI_(void) FillToGalleryLeft();

    CARAPI_(void) FillToGalleryRight();

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
    CARAPI_(AutoPtr<IView>) MakeAndAddView(
        /* [in] */ Int32 position,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 x,
        /* [in] */ Boolean fromLeft);

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
    CARAPI_(void) SetUpChild(
        /* [in] */ IView* child,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 x,
        /* [in] */ Boolean fromLeft);

    /**
     * Figure out vertical placement based on mGravity
     *
     * @param child Child to place
     * @return Where the top of the child should be
     */
    CARAPI_(Int32) CalculateTop(
        /* [in] */ IView* child,
        /* [in] */ Boolean duringLayout);

    CARAPI_(void) DispatchPress(
        /* [in] */ IView* child);

    CARAPI_(void) DispatchUnpress();

    CARAPI_(Boolean) DispatchLongPress(
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI_(Boolean) ScrollToChild(
        /* [in] */ Int32 childPosition);

    CARAPI_(void) UpdateSelectedItemMetadata();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);


private:
    static const CString TAG;

    static const Boolean localLOGV = FALSE;

    /**
     * Duration in milliseconds from the start of a scroll during which we're
     * unsure whether the user is scrolling or flinging.
     */
    static const Int32 SCROLL_TO_FLING_UNCERTAINTY_TIMEOUT = 250;

    /**
     * Horizontal spacing between items.
     */
    Int32 mSpacing;

    /**
     * How long the transition animation should run when a child view changes
     * position, measured in milliseconds.
     */
    Int32 mAnimationDuration;

    /**
     * The alpha of items that are not selected.
     */
    Float mUnselectedAlpha;

    /**
     * Left most edge of a child seen so far during layout.
     */
    Int32 mLeftMost;

    /**
     * Right most edge of a child seen so far during layout.
     */
    Int32 mRightMost;

    Int32 mGravity;

    /**
     * Helper for detecting touch gestures.
     */
    GestureDetector* mGestureDetector;

    /**
     * The position of the item that received the user's down touch.
     */
    Int32 mDownTouchPosition;

    /**
     * The view of the item that received the user's down touch.
     */
    AutoPtr<IView> mDownTouchView;

    /**
     * Executes the delta scrolls from a fling or scroll movement.
     */
    AutoPtr<FlingRunnable> mFlingRunnable;

    /**
     * Sets mSuppressSelectionChanged = FALSE. This is used to set it to FALSE
     * in the future. It will also trigger a selection changed.
     */
    AutoPtr<IRunnable> mDisableSuppressSelectionChangedRunnable;

    /**
     * When fling runnable runs, it resets this to FALSE. Any method along the
     * path until the end of its run() can set this to TRUE to abort any
     * remaining fling. For example, if we've reached either the leftmost or
     * rightmost item, we will set this to TRUE.
     */
    Boolean mShouldStopFling;

    /**
     * The currently selected item's child.
     */
    AutoPtr<IView> mSelectedChild;

    /**
     * Whether to continuously callback on the item selected listener during a
     * fling.
     */
    Boolean mShouldCallbackDuringFling;

    /**
     * Whether to callback when an item that is not selected is clicked.
     */
    Boolean mShouldCallbackOnUnselectedItemClick;

    /**
     * If TRUE, do not callback to item selected listener.
     */
    Boolean mSuppressSelectionChanged;

    /**
     * If TRUE, we have received the "invoke" (center or enter buttons) key
     * down. This is checked before we action on the "invoke" key up, and is
     * subsequently cleared.
     */
    Boolean mReceivedInvokeKeyDown;

    AutoPtr<AdapterContextMenuInfo> mContextMenuInfo;

    /**
     * If TRUE, this onScroll is the first for this user's drag (remember, a
     * drag sends many onScrolls).
     */
    Boolean mIsFirstScroll;
};

#endif // __GALLERY_H__
