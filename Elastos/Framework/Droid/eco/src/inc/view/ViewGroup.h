
#ifndef __ViewGroup_h__
#define __ViewGroup_h__

#include "ext/frameworkext.h"
#include "view/View.h"
#include <elastos/Vector.h>

using namespace Elastos;

class ViewGroup : public View
{
public:
    ViewGroup();

    ViewGroup(
        /* [in] */ IContext* context);

    ViewGroup(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ViewGroup(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI_(Int32) GetDescendantFocusability();

    virtual CARAPI SetDescendantFocusability(
        /* [in] */ Int32 focusability);

    CARAPI HandleFocusGainInternal(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    virtual CARAPI RequestChildFocus(
        /* [in] */ IView* child,
        /* [in] */ IView* focused);

    virtual CARAPI FocusableViewAvailable(
        /* [in] */ IView* v);

    virtual CARAPI_(Boolean) ShowContextMenuForChild(
        /* [in] */ IView* originalView);

    virtual CARAPI_(AutoPtr<IView>) FocusSearch(
         /* [in] */ IView* focused,
         /* [in] */ Int32 direction);

    virtual CARAPI_(Boolean) RequestChildRectangleOnScreen(
        /* [in] */ IView* child,
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate);

    CARAPI_(Boolean) DispatchUnhandledMove(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction);

    virtual CARAPI ClearChildFocus(
        /* [in] */ IView* child);

    CARAPI ClearFocus();

    CARAPI UnFocus();

    virtual CARAPI_(AutoPtr<IView>) GetFocusedChild();

    CARAPI_(Boolean) HasFocus();

    CARAPI_(AutoPtr<IView>) FindFocus();

    CARAPI_(Boolean) HasFocusable();

    CARAPI AddFocusables(
        /* [in] */ IObjectContainer* views,
        /* [in] */ Int32 direction);

    CARAPI AddFocusables(
        /* [in] */ IObjectContainer* views,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 focusableMode);

    CARAPI AddTouchables(
        /* [in] */ IObjectContainer* views);

    CARAPI DispatchWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    CARAPI DispatchDisplayHint(
        /* [in] */ Int32 hint);

    CARAPI_(void) DispatchVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    CARAPI DispatchWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    CARAPI DispatchConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI RecomputeViewAttributes(
        /* [in] */ IView* child);

    CARAPI DispatchCollectViewAttributes(
        /* [in] */ Int32 visibility);

    virtual CARAPI BringChildToFront(
        /* [in] */ IView* child);

    CARAPI_(Boolean) DispatchKeyEventPreIme(
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) DispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) DispatchTrackballEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) DispatchTouchEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI RequestDisallowInterceptTouchEvent(
        /* [in] */ Boolean disallowIntercept);

    virtual CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(Boolean) RequestFocus(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    CARAPI DispatchStartTemporaryDetach();

    CARAPI DispatchFinishTemporaryDetach();

    CARAPI_(void) DispatchAttachedToWindow(
        /* [in] */ AttachInfo* info,
        /* [in] */ Int32 visibility);

    CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI DispatchDetachedFromWindow();

    CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI_(void) SetClipChildren(
        /* [in] */ Boolean clipChildren);

    CARAPI_(void) SetClipToPadding(
        /* [in] */ Boolean clipToPadding);

    CARAPI_(void) DispatchSetSelected(
        /* [in] */ Boolean selected);

    virtual CARAPI AddView(
        /* [in] */ IView* child);

    virtual CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index);

    virtual CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI UpdateViewLayout(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams*  params);

    /**
     * Register a callback to be invoked when a child is added to or removed
     * from this view.
     *
     * @param listener the callback to invoke on hierarchy change
     */
    virtual CARAPI SetOnHierarchyChangeListener(
        /* [in] */ IViewGroupOnHierarchyChangeListener* listener);

    virtual CARAPI RemoveView(
        /* [in] */ IView* view);

    /**
     * Removes a view during layout. This is useful if in your onLayout() method,
     * you need to remove more views.
     *
     * @param view the view to remove from the group
     */
    virtual CARAPI RemoveViewInLayout(
        /* [in] */ IView* view);

    /**
     * Removes a range of views during layout. This is useful if in your onLayout() method,
     * you need to remove more views.
     *
     * @param start the index of the first view to remove from the group
     * @param count the number of views to remove from the group
     */
    virtual CARAPI RemoveViewsInLayout(
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    /**
     * Removes the view at the specified position in the group.
     *
     * @param index the position in the group of the view to remove
     */
    virtual CARAPI RemoveViewAt(
        /* [in] */ Int32 index);

    /**
     * Removes the specified range of views from the group.
     *
     * @param start the first position in the group of the range of views to remove
     * @param count the number of views to remove
     */
    virtual CARAPI RemoveViews(
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    /**
     * Call this method to remove all child views from the
     * ViewGroup.
     */
    virtual CARAPI RemoveAllViews();

    /**
     * Called by a ViewGroup subclass to remove child views from itself,
     * when it must first know its size on screen before it can calculate how many
     * child views it will render. An example is a Gallery or a ListView, which
     * may "have" 50 children, but actually only render the number of children
     * that can currently fit inside the object on screen. Do not call
     * this method unless you are extending ViewGroup and understand the
     * view measuring and layout pipeline.
     */
    virtual CARAPI RemoveAllViewsInLayout();

    /**
     * Don't call or override this method. It is used for the implementation of
     * the view hierarchy.
     */
    virtual CARAPI InvalidateChild(
        /* [in] */ IView* child,
        /* [in] */ IRect* dirty);

    /**
     * Don't call or override this method. It is used for the implementation of
     * the view hierarchy.
     *
     * This implementation returns NULL if this ViewGroup does not have a parent,
     * if this ViewGroup is already fully invalidated or if the dirty rectangle
     * does not intersect with this ViewGroup's bounds.
     */
    virtual CARAPI_(AutoPtr<IViewParent>) InvalidateChildInParent(
        /* [in] */ ArrayOf<Int32>* location,
        /* [in] */ IRect* dirty);

    /**
     * Offset a rectangle that is in a descendant's coordinate
     * space into our coordinate space.
     * @param descendant A descendant of this view
     * @param rect A rectangle defined in descendant's coordinate space.
     */
    CARAPI OffsetDescendantRectToMyCoords(
        /* [in] */ IView* descendant,
        /* [in] */ IRect* rect);

    /**
     * Offset a rectangle that is in our coordinate space into an ancestor's
     * coordinate space.
     * @param descendant A descendant of this view
     * @param rect A rectangle defined in descendant's coordinate space.
     */
    CARAPI OffsetRectIntoDescendantCoords(
        /* [in] */ IView* descendant,
        /* [in] */ IRect* rect);

    /**
     * Helper method that offsets a rect either from parent to descendant or
     * descendant to parent.
     */
    virtual CARAPI OffsetRectBetweenParentAndChild(
        /* [in] */ IView* descendant,
        /* [in] */ IRect* rect,
        /* [in] */ Boolean offsetFromChildToParent,
        /* [in] */ Boolean clipToBounds);

    /**
     * Offset the vertical location of all children of this view by the specified number of pixels.
     *
     * @param offset the number of pixels to offset
     *
     * @hide
     */
    virtual CARAPI OffsetChildrenTopAndBottom(
        /* [in] */ Int32 offset);

    virtual CARAPI GetChildVisibleRect(
        /* [in] */ IView* child,
        /* [in] */ IRect* r,
        /* [in] */ IPoint* offset,
        /* [out] */ Boolean* result);

    /**
     * Runs the layout animation. Calling this method triggers a relayout of
     * this view group.
     */
    virtual CARAPI StartLayoutAnimation();

    /**
     * Schedules the layout animation to be played after the next layout pass
     * of this view group. This can be used to restart the layout animation
     * when the content of the view group changes or when the activity is
     * paused and resumed.
     */
    virtual CARAPI ScheduleLayoutAnimation();

    /**
     * Sets the layout animation controller used to animate the group's
     * children after the first layout.
     *
     * @param controller the animation controller
     */
    virtual CARAPI SetLayoutAnimation(
        /* [in] */ ILayoutAnimationController* controller);

    /**
     * Returns the layout animation controller used to animate the group's
     * children.
     *
     * @return the current animation controller
     */
    virtual CARAPI_(AutoPtr<ILayoutAnimationController>) GetLayoutAnimation();

    /**
     * Indicates whether the children's drawing cache is used during a layout
     * animation. By default, the drawing cache is enabled but this will prevent
     * nested layout animations from working. To nest animations, you must disable
     * the cache.
     *
     * @return TRUE if the animation cache is enabled, FALSE otherwise
     *
     * @see #setAnimationCacheEnabled(Boolean)
     * @see View#setDrawingCacheEnabled(Boolean)
     */
    virtual CARAPI_(Boolean) IsAnimationCacheEnabled();

    /**
     * Enables or disables the children's drawing cache during a layout animation.
     * By default, the drawing cache is enabled but this will prevent nested
     * layout animations from working. To nest animations, you must disable the
     * cache.
     *
     * @param enabled TRUE to enable the animation cache, FALSE otherwise
     *
     * @see #isAnimationCacheEnabled()
     * @see View#setDrawingCacheEnabled(Boolean)
     */
    virtual CARAPI SetAnimationCacheEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Indicates whether this ViewGroup will always try to draw its children using their
     * drawing cache. By default this property is enabled.
     *
     * @return TRUE if the animation cache is enabled, FALSE otherwise
     *
     * @see #setAlwaysDrawnWithCacheEnabled(Boolean)
     * @see #setChildrenDrawnWithCacheEnabled(Boolean)
     * @see View#setDrawingCacheEnabled(Boolean)
     */
    virtual CARAPI_(Boolean) IsAlwaysDrawnWithCacheEnabled();

    /**
     * Indicates whether this ViewGroup will always try to draw its children using their
     * drawing cache. This property can be set to TRUE when the cache rendering is
     * slightly different from the children's normal rendering. Renderings can be different,
     * for instance, when the cache's quality is set to low.
     *
     * When this property is disabled, the ViewGroup will use the drawing cache of its
     * children only when asked to. It's usually the task of subclasses to tell ViewGroup
     * when to start using the drawing cache and when to stop using it.
     *
     * @param always TRUE to always draw with the drawing cache, FALSE otherwise
     *
     * @see #isAlwaysDrawnWithCacheEnabled()
     * @see #setChildrenDrawnWithCacheEnabled(Boolean)
     * @see View#setDrawingCacheEnabled(Boolean)
     * @see View#setDrawingCacheQuality(Int32)
     */
    virtual CARAPI SetAlwaysDrawnWithCacheEnabled(
        /* [in] */ Boolean always);

    /**
     * Returns an integer indicating what types of drawing caches are kept in memory.
     *
     * @see #setPersistentDrawingCache(Int32)
     * @see #setAnimationCacheEnabled(Boolean)
     *
     * @return one or a combination of {@link #PERSISTENT_NO_CACHE},
     *         {@link #PERSISTENT_ANIMATION_CACHE}, {@link #PERSISTENT_SCROLLING_CACHE}
     *         and {@link #PERSISTENT_ALL_CACHES}
     */
    virtual CARAPI_(Int32) GetPersistentDrawingCache();

    /**
     * Indicates what types of drawing caches should be kept in memory after
     * they have been created.
     *
     * @see #getPersistentDrawingCache()
     * @see #setAnimationCacheEnabled(Boolean)
     *
     * @param drawingCacheToKeep one or a combination of {@link #PERSISTENT_NO_CACHE},
     *        {@link #PERSISTENT_ANIMATION_CACHE}, {@link #PERSISTENT_SCROLLING_CACHE}
     *        and {@link #PERSISTENT_ALL_CACHES}
     */
    virtual CARAPI SetPersistentDrawingCache(
        /* [in] */ Int32 drawingCacheToKeep);

    /**
     * Returns a new set of layout parameters based on the supplied attributes set.
     *
     * @param attrs the attributes to build the layout parameters from
     *
     * @return an instance of {@link android.view.ViewGroup.LayoutParams} or one
     *         of its descendants
     */
    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(Int32) IndexOfChild(
        /* [in] */ IView* child);

    virtual CARAPI_(Int32) GetChildCount();

    virtual CARAPI_(AutoPtr<IView>) GetChildAt(
        /* [in] */ Int32 index);

    static Int32 GetChildMeasureSpec(
        /* [in] */ Int32 spec,
        /* [in] */ Int32 padding,
        /* [in] */ Int32 childDimension);

    /**
     * Removes any pending animations for views that have been removed. Call
     * this if you don't want animations for exiting views to stack up.
     */
    virtual CARAPI ClearDisappearingChildren();

    CARAPI_(Boolean) GatherTransparentRegion(
        /* [in, out] */ IRegion* region);

    virtual CARAPI RequestTransparentRegion(
        /* [in] */ IView* child);

    /**
     * Returns the animation listener to which layout animation events are
     * sent.
     *
     * @return an {@link android.view.animation.Animation.AnimationListener}
     */
    virtual CARAPI_(AutoPtr<IAnimationListener>) GetLayoutAnimationListener();

    /**
     * Sets whether this ViewGroup's drawable states also include
     * its children's drawable states.  This is used, for example, to
     * make a group appear to be focused when its child EditText or button
     * is focused.
     */
    virtual CARAPI SetAddStatesFromChildren(
        /* [in] */ Boolean addsStates);

    /**
     * Returns whether this ViewGroup's drawable states also include
     * its children's drawable states.  This is used, for example, to
     * make a group appear to be focused when its child EditText or button
     * is focused.
     */
    virtual CARAPI_(Boolean) AddStatesFromChildren();

    /**
     * If {link #addStatesFromChildren} is TRUE, refreshes this group's
     * drawable state (to include the states from its children).
     */
    virtual CARAPI ChildDrawableStateChanged(
        /* [in] */ IView* child);

    /**
     * Specifies the animation listener to which layout animation events must
     * be sent. Only
     * {@link android.view.animation.Animation.AnimationListener#onAnimationStart(Animation)}
     * and
     * {@link android.view.animation.Animation.AnimationListener#onAnimationEnd(Animation)}
     * are invoked.
     *
     * @param animationListener the layout animation listener
     */
    virtual CARAPI SetLayoutAnimationListener(
        /* [in] */ IAnimationListener* animationListener);

protected:
    CARAPI_(Boolean) OnRequestFocusInDescendants(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    CARAPI DispatchSaveInstanceState(
        /* [in] */ IObjectIntegerMap* container);

    /**
     * Perform dispatching of a {@link #saveHierarchyState freeze()} to only this view,
     * not to its children.  For use when overriding
     * {@link #dispatchSaveInstanceState dispatchFreeze()} to allow subclasses to freeze
     * their own state but not the state of their children.
     *
     * @param container the container
     */
    virtual CARAPI DispatchFreezeSelfOnly(
        /* [in] */ IObjectIntegerMap* container);

    CARAPI DispatchRestoreInstanceState(
        /* [in] */ IObjectIntegerMap* container);

    /**
     * Perform dispatching of a {@link #restoreHierarchyState thaw()} to only this view,
     * not to its children.  For use when overriding
     * {@link #dispatchRestoreInstanceState dispatchThaw()} to allow subclasses to thaw
     * their own state but not the state of their children.
     *
     * @param container the container
     */
    virtual CARAPI DispatchThawSelfOnly(
        /* [in] */ IObjectIntegerMap* container);

    CARAPI_(void) SetChildrenDrawingCacheEnabled(
        /* [in] */ Boolean enabled);

    CARAPI_(void) OnAnimationStart();

    CARAPI_(void) OnAnimationEnd();

    CARAPI CreateSnapshot(
        /* [in] */ BitmapConfig quality,
        /* [in] */ Int32 backgroundColor,
        /* [in] */ Boolean skipChildren,
        /* [out] */ IBitmap** bitmap);

    CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

    CARAPI_(Int32) GetChildDrawingOrder(
        /* [in] */ Int32 childCount,
        /* [in] */ Int32 i);

    CARAPI_(Boolean) DrawChild(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IView* child,
        /* [in] */ Int64 drawingTime);

    CARAPI_(void) DispatchSetPressed(
        /* [in] */ Boolean pressed);

    CARAPI_(void) SetStaticTransformationsEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) GetChildStaticTransformation(
        /* [in] */ IView* child,
        /* [in] */ ITransformation* t);

    CARAPI_(AutoPtr<IView>) FindViewTraversal(
        /* [in] */ Int32 id);

    //View FindViewWithTagTraversal(Object tag);

    virtual CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    /**
     * Adds a view during layout. This is useful if in your onLayout() method,
     * you need to add more views (as does the list view for example).
     *
     * If index is negative, it means put it at the end of the list.
     *
     * @param child the view to add to the group
     * @param index the index at which the child must be added
     * @param params the layout parameters to associate with the child
     * @return TRUE if the child was added, FALSE otherwise
     */
    virtual CARAPI_(Boolean) AddViewInLayout(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    /**
     * Adds a view during layout. This is useful if in your onLayout() method,
     * you need to add more views (as does the list view for example).
     *
     * If index is negative, it means put it at the end of the list.
     *
     * @param child the view to add to the group
     * @param index the index at which the child must be added
     * @param params the layout parameters to associate with the child
     * @param preventRequestLayout if TRUE, calling this method will not trigger a
     *        layout request on child
     * @return TRUE if the child was added, FALSE otherwise
     */
    virtual CARAPI_(Boolean) AddViewInLayout(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params,
        /* [in] */ Boolean preventRequestLayout);

    /**
     * Prevents the specified child to be laid out during the next layout pass.
     *
     * @param child the child on which to perform the cleanup
     */
    virtual CARAPI_(void) CleanupLayoutState(
        /* [in] */ IView* child);

    /**
     * Subclasses should override this method to set layout animation
     * parameters on the supplied child.
     *
     * @param child the child to associate with animation parameters
     * @param params the child's layout parameters which hold the animation
     *        parameters
     * @param index the index of the child in the view group
     * @param count the number of children in the view group
     */
    virtual CARAPI AttachLayoutAnimationParameters(
        /* [in] */ IView* child,
        /* [in] */ IViewGroupLayoutParams* params,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count);

    /**
     * Finishes the removal of a detached view. This method will dispatch the detached from
     * window event and notify the hierarchy change listener.
     *
     * @param child the child to be definitely removed from the view hierarchy
     * @param animate if TRUE and the view has an animation, the view is placed in the
     *                disappearing views list, otherwise, it is detached from the window
     *
     * @see #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)
     * @see #detachAllViewsFromParent()
     * @see #detachViewFromParent(View)
     * @see #detachViewFromParent(Int32)
     */
    virtual CARAPI_(void) RemoveDetachedView(
        /* [in] */ IView* child,
        /* [in] */ Boolean animate);

    /**
     * Attaches a view to this view group. Attaching a view assigns this group as the parent,
     * sets the layout parameters and puts the view in the list of children so it can be retrieved
     * by calling {@link #getChildAt(Int32)}.
     *
     * This method should be called only for view which were detached from their parent.
     *
     * @param child the child to attach
     * @param index the index at which the child should be attached
     * @param params the layout parameters of the child
     *
     * @see #removeDetachedView(View, Boolean)
     * @see #detachAllViewsFromParent()
     * @see #detachViewFromParent(View)
     * @see #detachViewFromParent(Int32)
     */
    virtual CARAPI_(void) AttachViewToParent(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    /**
     * Detaches a view from its parent. Detaching a view should be temporary and followed
     * either by a call to {@link #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)}
     * or a call to {@link #removeDetachedView(View, Boolean)}. When a view is detached,
     * its parent is NULL and cannot be retrieved by a call to {@link #getChildAt(Int32)}.
     *
     * @param child the child to detach
     *
     * @see #detachViewFromParent(Int32)
     * @see #detachViewsFromParent(Int32, Int32)
     * @see #detachAllViewsFromParent()
     * @see #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)
     * @see #removeDetachedView(View, Boolean)
     */
    virtual CARAPI_(void) DetachViewFromParent(
        /* [in] */ IView* child);

    /**
     * Detaches a view from its parent. Detaching a view should be temporary and followed
     * either by a call to {@link #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)}
     * or a call to {@link #removeDetachedView(View, Boolean)}. When a view is detached,
     * its parent is NULL and cannot be retrieved by a call to {@link #getChildAt(Int32)}.
     *
     * @param index the index of the child to detach
     *
     * @see #detachViewFromParent(View)
     * @see #detachAllViewsFromParent()
     * @see #detachViewsFromParent(Int32, Int32)
     * @see #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)
     * @see #removeDetachedView(View, Boolean)
     */
    virtual CARAPI_(void) DetachViewFromParent(
        /* [in] */ Int32 index);

    /**
     * Detaches a range of view from their parent. Detaching a view should be temporary and followed
     * either by a call to {@link #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)}
     * or a call to {@link #removeDetachedView(View, Boolean)}. When a view is detached, its
     * parent is NULL and cannot be retrieved by a call to {@link #getChildAt(Int32)}.
     *
     * @param start the first index of the childrend range to detach
     * @param count the number of children to detach
     *
     * @see #detachViewFromParent(View)
     * @see #detachViewFromParent(Int32)
     * @see #detachAllViewsFromParent()
     * @see #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)
     * @see #removeDetachedView(View, Boolean)
     */
    virtual CARAPI_(void) DetachViewsFromParent(
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    /**
     * Detaches all views from the parent. Detaching a view should be temporary and followed
     * either by a call to {@link #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)}
     * or a call to {@link #removeDetachedView(View, Boolean)}. When a view is detached,
     * its parent is NULL and cannot be retrieved by a call to {@link #getChildAt(Int32)}.
     *
     * @see #detachViewFromParent(View)
     * @see #detachViewFromParent(Int32)
     * @see #detachViewsFromParent(Int32, Int32)
     * @see #attachViewToParent(View, Int32, android.view.ViewGroup.LayoutParams)
     * @see #removeDetachedView(View, Boolean)
     */
    virtual CARAPI_(void) DetachAllViewsFromParent();

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b) = 0;

    /**
     * Indicates whether the view group has the ability to animate its children
     * after the first layout.
     *
     * @return true if the children can be animated, false otherwise
     */
    virtual CARAPI_(Boolean) CanAnimate();

    /**
     * Indicates whether the ViewGroup is currently drawing its children using
     * their drawing cache.
     *
     * @return TRUE if children should be drawn with their cache, FALSE otherwise
     *
     * @see #setAlwaysDrawnWithCacheEnabled(Boolean)
     * @see #setChildrenDrawnWithCacheEnabled(Boolean)
     */
    virtual CARAPI_(Boolean) IsChildrenDrawnWithCacheEnabled();

    /**
     * Tells the ViewGroup to draw its children using their drawing cache. This property
     * is ignored when {@link #isAlwaysDrawnWithCacheEnabled()} is TRUE. A child's drawing cache
     * will be used only if it has been enabled.
     *
     * Subclasses should call this method to start and stop using the drawing cache when
     * they perform performance sensitive operations, like scrolling or animating.
     *
     * @param enabled TRUE if children should be drawn with their cache, FALSE otherwise
     *
     * @see #setAlwaysDrawnWithCacheEnabled(Boolean)
     * @see #isChildrenDrawnWithCacheEnabled()
     */
    virtual CARAPI_(void) SetChildrenDrawnWithCacheEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Indicates whether the ViewGroup is drawing its children in the order defined by
     * {@link #getChildDrawingOrder(Int32, Int32)}.
     *
     * @return TRUE if children drawing order is defined by {@link #getChildDrawingOrder(Int32, Int32)},
     *         FALSE otherwise
     *
     * @see #setChildrenDrawingOrderEnabled(Boolean)
     * @see #getChildDrawingOrder(Int32, Int32)
     */
    virtual CARAPI_(Boolean) IsChildrenDrawingOrderEnabled();

    /**
     * Tells the ViewGroup whether to draw its children in the order defined by the method
     * {@link #getChildDrawingOrder(Int32, Int32)}.
     *
     * @param enabled TRUE if the order of the children when drawing is determined by
     *        {@link #getChildDrawingOrder(Int32, Int32)}, FALSE otherwise
     *
     * @see #isChildrenDrawingOrderEnabled()
     * @see #getChildDrawingOrder(Int32, Int32)
     */
    virtual CARAPI_(void) SetChildrenDrawingOrderEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Returns a safe set of layout parameters based on the supplied layout params.
     * When a ViewGroup is passed a View whose layout params do not pass the test of
     * {@link #checkLayoutParams(android.view.ViewGroup.LayoutParams)}, this method
     * is invoked. This method should return a new set of layout params suitable for
     * this ViewGroup, possibly by copying the appropriate attributes from the
     * specified set of layout params.
     *
     * @param p The layout parameters to convert into a suitable set of layout parameters
     *          for this ViewGroup.
     *
     * @return an instance of {@link android.view.ViewGroup.LayoutParams} or one
     *         of its descendants
     */
    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    virtual CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** params);

    CARAPI_(Boolean) DispatchConsistencyCheck(
        /* [in] */ Int32 consistency);

    CARAPI_(Boolean) OnConsistencyCheck(
        /* [in] */ Int32 consistency);

    //virtual CARAPI_(void) debug(Int32 depth)

    /**
     * Ask all of the children of this view to measure themselves, taking into
     * account both the MeasureSpec requirements for this view and its padding.
     * We skip children that are in the GONE state The heavy lifting is done in
     * getChildMeasureSpec.
     *
     * @param widthMeasureSpec The width requirements for this view
     * @param heightMeasureSpec The height requirements for this view
     */
    virtual CARAPI_(void) MeasureChildren(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    /**
     * Ask one of the children of this view to measure itself, taking into
     * account both the MeasureSpec requirements for this view and its padding.
     * The heavy lifting is done in getChildMeasureSpec.
     *
     * @param child The child to measure
     * @param parentWidthMeasureSpec The width requirements for this view
     * @param parentHeightMeasureSpec The height requirements for this view
     */
    virtual CARAPI_(void) MeasureChild(
        /* [in] */ IView* child,
        /* [in] */ Int32 parentWidthMeasureSpec,
        /* [in] */ Int32 parentHeightMeasureSpec);

    virtual CARAPI_(void) MeasureChildWithMargins(
        /* [in] */ IView* child,
        /* [in] */ Int32 parentWidthMeasureSpec,
        /* [in] */ Int32 widthUsed,
        /* [in] */ Int32 parentHeightMeasureSpec,
        /* [in] */ Int32 heightUsed);

    CARAPI_(Boolean) FitSystemWindows(
        /* [in] */ IRect* insets);

    CARAPI DrawableStateChanged();

    CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    CARAPI_(void) InitViewGroup();

    CARAPI_(void) InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    //virtual CARAPI_(void) NotifyAnimationListener();

    CARAPI AddViewInner(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params,
        /* [in] */ Boolean preventRequestLayout);

    CARAPI AddInArray(
        /* [in] */ IView* child,
        /* [in] */ Int32 index);

    CARAPI RemoveFromArray(
        /* [in] */ Int32 index);

    CARAPI RemoveFromArray(
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    CARAPI_(void) BindLayoutAnimation(
        /* [in] */ IView* child);

    CARAPI_(void) RemoveViewInternal(
        /* [in] */ IView* view);

    CARAPI_(void) RemoveViewInternal(
        /* [in] */ Int32 index,
        /* [in] */ IView* view);

    CARAPI_(void) RemoveViewsInternal(
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    CARAPI_(void) SetBooleanFlag(
        /* [in] */ Int32 flag,
        /* [in] */ Boolean value);

    /**
     * Add a view which is removed from mChildren but still needs animation
     *
     * @param v View to add
     */
    CARAPI_(void) AddDisappearingView(
        /* [in] */ IView* view);

    /**
     * Cleanup a view when its animation is done. This may mean removing it from
     * the list of disappearing views.
     *
     * @param view The view whose animation has finished
     * @param animation The animation, cannot be NULL
     */
    CARAPI_(void) FinishAnimatingView(
        /* [in] */ IView* view,
        /* [in] */ IAnimation* animation);


public:
    /**
     * This view will get focus before any of its descendants.
     */
    static const Int32 FOCUS_BEFORE_DESCENDANTS = 0x20000;

    /**
     * This view will get focus only if none of its descendants want it.
     */
    static const Int32 FOCUS_AFTER_DESCENDANTS = 0x40000;

    /**
     * This view will block any of its descendants from getting focus, even
     * if they are focusable.
     */
    static const Int32 FOCUS_BLOCK_DESCENDANTS = 0x60000;

    /**
     * Used to indicate that no drawing cache should be kept in memory.
     */
    static const Int32 PERSISTENT_NO_CACHE = 0x0;

    /**
     * Used to indicate that the animation drawing cache should be kept in memory.
     */
    static const Int32 PERSISTENT_ANIMATION_CACHE = 0x1;

    /**
     * Used to indicate that the scrolling drawing cache should be kept in memory.
     */
    static const Int32 PERSISTENT_SCROLLING_CACHE = 0x2;

    /**
     * Used to indicate that all drawing caches should be kept in memory.
     */
    static const Int32 PERSISTENT_ALL_CACHES = 0x3;

private:
    static const Boolean DBG = TRUE;
    static const char* VG_TAG;

    // When set, ViewGroup invalidates only the child's rectangle
    // Set by default
    static const Int32 FLAG_CLIP_CHILDREN = 0x1;

    // When set, ViewGroup excludes the padding area from the invalidate rectangle
    // Set by default
    static const Int32 FLAG_CLIP_TO_PADDING = 0x2;

    // When set, dispatchDraw() will invoke invalidate(); this is set by drawChild() when
    // a child needs to be invalidated and FLAG_OPTIMIZE_INVALIDATE is set
    static const Int32 FLAG_INVALIDATE_REQUIRED  = 0x4;

    // When set, dispatchDraw() will run the layout animation and unset the flag
    static const Int32 FLAG_RUN_ANIMATION = 0x8;

    // When set, there is either no layout animation on the ViewGroup or the layout
    // animation is over
    // Set by default
    static const Int32 FLAG_ANIMATION_DONE = 0x10;

    // If set, this ViewGroup has padding; if unset there is no padding and we don't need
    // to clip it, even if FLAG_CLIP_TO_PADDING is set
    static const Int32 FLAG_PADDING_NOT_NULL = 0x20;

    // When set, this ViewGroup caches its children in a Bitmap before starting a layout animation
    // Set by default
    static const Int32 FLAG_ANIMATION_CACHE = 0x40;

    // When set, this ViewGroup converts calls to invalidate(Rect) to invalidate() during a
    // layout animation; this avoid clobbering the hierarchy
    // Automatically set when the layout animation starts, depending on the animation's
    // characteristics
    static const Int32 FLAG_OPTIMIZE_INVALIDATE = 0x80;

    // When set, the next call to drawChild() will clear mChildTransformation's matrix
    static const Int32 FLAG_CLEAR_TRANSFORMATION = 0x100;

    // When set, this ViewGroup invokes mAnimationListener.onAnimationEnd() and removes
    // the children's Bitmap caches if necessary
    // This flag is set when the layout animation is over (after FLAG_ANIMATION_DONE is set)

    static const Int32 FLAG_NOTIFY_ANIMATION_LISTENER = 0x200;

    // When the previous drawChild() invocation used an alpha value that was lower than
    // 1.0 and set it in mCachePaint
    static const Int32 FLAG_ALPHA_LOWER_THAN_ONE = 0x1000;

    /**
     * When set, this ViewGroup's drawable states also include those
     * of its children.
     */
    static const Int32 FLAG_ADD_STATES_FROM_CHILDREN = 0x2000;

    /**
     * When set, this ViewGroup tries to always draw its children using their drawing cache.
     */
    static const Int32 FLAG_ALWAYS_DRAWN_WITH_CACHE = 0x4000;

    /**
     * When set, and if FLAG_ALWAYS_DRAWN_WITH_CACHE is not set, this ViewGroup will try to
     * draw its children with their drawing cache.
     */
    static const Int32 FLAG_CHILDREN_DRAWN_WITH_CACHE = 0x8000;

    /**
     * When set, this group will go through its list of children to notify them of
     * any drawable state change.
     */
    static const Int32 FLAG_NOTIFY_CHILDREN_ON_DRAWABLE_STATE_CHANGE = 0x10000;

    static const Int32 FLAG_MASK_FOCUSABILITY = 0x60000;

    /**
     * Used to map between enum in attrubutes and flag values.
     */
    static const Int32 DESCENDANT_FOCUSABILITY_FLAGS[] ;/*=
            {FOCUS_BEFORE_DESCENDANTS, FOCUS_AFTER_DESCENDANTS,
                    FOCUS_BLOCK_DESCENDANTS};*/

    static const Int32 ARRAY_INITIAL_CAPACITY = 12;
    static const Int32 ARRAY_CAPACITY_INCREMENT = 12;

protected:
    /**
     * When set, the drawing method will call {@link #getChildDrawingOrder(Int32, Int32)}
     * to get the index of the child to draw for that iteration.
     *
     * @hide
     */
    static const Int32 FLAG_USE_CHILD_DRAWING_ORDER = 0x400;

    /**
     * When set, this ViewGroup supports static transformations on children; this causes
     * {@link #getChildStaticTransformation(View, android.view.animation.Transformation)} to be
     * invoked when a child is drawn.
     *
     * Any subclass overriding
     * {@link #getChildStaticTransformation(View, android.view.animation.Transformation)} should
     * set this flags in {@link #mGroupFlags}.
     *
     * {@hide}
     */
    static const Int32 FLAG_SUPPORT_STATIC_TRANSFORMATIONS = 0x800;

    /**
     * When set, this ViewGroup should not intercept touch events.
     * {@hide}
     */
    static const Int32 FLAG_DISALLOW_INTERCEPT = 0x80000;

    /**
     * We clip to padding when FLAG_CLIP_TO_PADDING and FLAG_PADDING_NOT_NULL
     * are set at the same time.
     */
    static const Int32 CLIP_TO_PADDING_MASK = FLAG_CLIP_TO_PADDING | FLAG_PADDING_NOT_NULL;

    // Index of the child's left position in the mLocation array
    static const Int32 CHILD_LEFT_INDEX = 0;
    // Index of the child's top position in the mLocation array
    static const Int32 CHILD_TOP_INDEX = 1;

    /**
     * Views which have been hidden or removed which need to be animated on
     * their way out.
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    List<AutoPtr<IView> >* mDisappearingChildren;

    /**
     * Listener used to propagate events indicating when children are added
     * and/or removed from a view group.
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IViewGroupOnHierarchyChangeListener> mOnHierarchyChangeListener;

    /**
     * Internal flags.
     *
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Int32 mGroupFlags;

    /**
     * Indicates which types of drawing caches are to be kept in memory.
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Int32 mPersistentDrawingCache;

private:
    // The view contained within this ViewGroup that has or contains focus.
    AutoPtr<IView> mFocused;

    // The current transformation to apply on the child being drawn
    //private Transformation mChildTransformation;
    AutoPtr<CRect> mInvalidateRegion;

    // Target of Motion events
    AutoPtr<IView> mMotionTarget;

    AutoPtr<CRect> mTempRect;

    // Layout animation
    AutoPtr<ILayoutAnimationController> mLayoutAnimationController;
    AutoPtr<IAnimationListener> mAnimationListener;

    // Child views of this ViewGroup
    Vector<AutoPtr<IView> > mChildren;
    // Number of valid children in the mChildren array, the rest should be null or not
    // considered as children
    Int32 mChildrenCount;

    // Used to draw cached views
    AutoPtr<IPaint> mCachePaint;
};

#endif // __ViewGroup_h__
