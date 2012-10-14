
#ifndef __View_h__
#define __View_h__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include "graphics/Interpolator.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include "view/CSurface.h"
#include "view/ViewTreeObserver.h"
#include "view/ViewConfiguration.h"
#include "os/Runnable.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <StringBuffer.h>

using namespace Elastos;
using namespace Elastos::Core;

extern "C" const InterfaceID EIID_View;

class ViewRoot;
class TouchDelegate;

class View
{
    friend class ViewRoot;
    friend class ViewGroup;
public:
    static const Boolean DBG = FALSE;

    /**
     * This view does not want keystrokes. Use with TAKES_FOCUS_MASK when
     * calling setFlags.
     */
    static const Int32 NOT_FOCUSABLE = 0x00000000;

    /**
     * This view wants keystrokes. Use with TAKES_FOCUS_MASK when calling
     * setFlags.
     */
    static const Int32 FOCUSABLE = 0x00000001;

    /**
     * Mask for use with setFlags indicating bits used for focus.
     */
    static const Int32 FOCUSABLE_MASK = 0x00000001;

    /**
     * This view will adjust its padding to fit sytem windows (e.g. status bar)
     */
    static const Int32 FITS_SYSTEM_WINDOWS = 0x00000002;

    static const Int32 VISIBILITY_FLAGS[];

    static const Int32 DRAWING_CACHE_QUALITY_FLAGS[];

    /**
     * The order here is very important to {@link #getDrawableState()}
     */
    static const ArrayOf<ArrayOf<Int32>*>* VIEW_STATE_SETS;
    static CARAPI_(const ArrayOf<ArrayOf<Int32>*>*) InitViewStateSets();

protected:
    /**
     * The logging tag used by this class with android.util.Log.
     */
    static const char* VIEW_LOG_TAG;

public:
    /**
     * Used to mark a View that has no ID.
     */
    static const Int32 NO_ID = -1;

    /**
     * This view is visible.  Use with {@link #setVisibility}.
     */
    static const Int32 VISIBLE = 0x00000000;

    /**
     * This view is invisible, but it still takes up space for layout purposes.
     * Use with {@link #setVisibility}.
     */
    static const Int32 INVISIBLE = 0x00000004;

    /**
     * This view is invisible, and it doesn't take any space for layout
     * purposes. Use with {@link #setVisibility}.
     */
    static const Int32 GONE = 0x00000008;

    /**
     * Mask for use with setFlags indicating bits used for visibility.
     * {@hide}
     */
    static const Int32 VISIBILITY_MASK = 0x0000000C;

     /**
     * This view is enabled. Intrepretation varies by subclass.
     * Use with ENABLED_MASK when calling setFlags.
     * {@hide}
     */
    static const Int32 ENABLED = 0x00000000;

    /**
     * This view is disabled. Intrepretation varies by subclass.
     * Use with ENABLED_MASK when calling setFlags.
     * {@hide}
     */
    static const Int32 DISABLED = 0x00000020;

   /**
    * Mask for use with setFlags indicating bits used for indicating whether
    * this view is enabled
    * {@hide}
    */
    static const Int32 ENABLED_MASK = 0x00000020;

    /**
     * This view won't draw. {@link #onDraw} won't be called and further
     * optimizations
     * will be performed. It is okay to have this flag set and a background.
     * Use with DRAW_MASK when calling setFlags.
     * {@hide}
     */
    static const Int32 WILL_NOT_DRAW = 0x00000080;

    /**
     * Mask for use with setFlags indicating bits used for indicating whether
     * this view is will draw
     * {@hide}
     */
    static const Int32 DRAW_MASK = 0x00000080;

    /**
     * <p>This view doesn't show scrollbars.</p>
     * {@hide}
     */
    static const Int32 SCROLLBARS_NONE = 0x00000000;

    /**
     * <p>This view shows horizontal scrollbars.</p>
     * {@hide}
     */
    static const Int32 SCROLLBARS_HORIZONTAL = 0x00000100;

    /**
     * <p>This view shows vertical scrollbars.</p>
     * {@hide}
     */
    static const Int32 SCROLLBARS_VERTICAL = 0x00000200;

    /**
     * <p>Mask for use with setFlags indicating bits used for indicating which
     * scrollbars are enabled.</p>
     * {@hide}
     */
    static const Int32 SCROLLBARS_MASK = 0x00000300;

    /**
     * Indicates that the view should filter touches when its window is obscured.
     * Refer to the class comments for more information about this security feature.
     * {@hide}
     */
    static const Int32 FILTER_TOUCHES_WHEN_OBSCURED = 0x00000400;

    /**
     * <p>This view doesn't show fading edges.</p>
     * {@hide}
     */
    static const Int32 FADING_EDGE_NONE = 0x00000000;

    /**
     * <p>Indicates this view can be clicked. When clickable, a View reacts
     * to clicks by notifying the OnClickListener.<p>
     * {@hide}
     */
    static const Int32 CLICKABLE = 0x00004000;

    /**
     * <p>Indicates this view is caching its drawing into a bitmap.</p>
     * {@hide}
     */
    static const Int32 DRAWING_CACHE_ENABLED = 0x00008000;

    /**
     * <p>Indicates that no icicle should be saved for this view.<p>
     * {@hide}
     */
    static const Int32 SAVE_DISABLED = 0x000010000;

    /**
     * <p>Mask for use with setFlags indicating bits used for the saveEnabled
     * property.</p>
     * {@hide}
     */
    static const Int32 SAVE_DISABLED_MASK = 0x000010000;

    /**
     * <p>Indicates that no drawing cache should ever be created for this view.<p>
     * {@hide}
     */
    static const Int32 WILL_NOT_CACHE_DRAWING = 0x000020000;

    /**
     * <p>Indicates this view can take / keep focus when int touch mode.</p>
     * {@hide}
     */
    static const Int32 FOCUSABLE_IN_TOUCH_MODE = 0x00040000;

    /**
     * <p>Enables low quality mode for the drawing cache.</p>
     */
    static const Int32 DRAWING_CACHE_QUALITY_LOW = 0x00080000;

    /**
     * <p>Enables high quality mode for the drawing cache.</p>
     */
    static const Int32 DRAWING_CACHE_QUALITY_HIGH = 0x00100000;

    /**
     * <p>Enables automatic quality mode for the drawing cache.</p>
     */
    static const Int32 DRAWING_CACHE_QUALITY_AUTO = 0x00000000;

    /*private static final int[] DRAWING_CACHE_QUALITY_FLAGS = {
            DRAWING_CACHE_QUALITY_AUTO, DRAWING_CACHE_QUALITY_LOW, DRAWING_CACHE_QUALITY_HIGH
    };*/

    /**
     * <p>Mask for use with setFlags indicating bits used for the cache
     * quality property.</p>
     * {@hide}
     */
    static const Int32 DRAWING_CACHE_QUALITY_MASK = 0x00180000;

    /**
     * <p>
     * Indicates this view can be long clicked. When long clickable, a View
     * reacts to long clicks by notifying the OnLongClickListener or showing a
     * context menu.
     * </p>
     * {@hide}
     */
    static const Int32 LONG_CLICKABLE = 0x00200000;

    /**
     * <p>Indicates that this view gets its drawable states from its direct parent
     * and ignores its original internal states.</p>
     *
     * @hide
     */
    static const Int32 DUPLICATE_PARENT_STATE = 0x00400000;

    /**
     * <p>This view shows horizontal fading edges.</p>
     * {@hide}
     */
    static const Int32 FADING_EDGE_HORIZONTAL = 0x00001000;

    /**
     * <p>This view shows vertical fading edges.</p>
     * {@hide}
     */
    static const Int32 FADING_EDGE_VERTICAL = 0x00002000;

    /**
     * <p>Mask for use with setFlags indicating bits used for indicating which
     * fading edges are enabled.</p>
     * {@hide}
     */
    static const Int32 FADING_EDGE_MASK = 0x00003000;

    /**
     * The scrollbar style to display the scrollbars inside the content area,
     * without increasing the padding. The scrollbars will be overlaid with
     * translucency on the view's content.
     */
    static const Int32 SCROLLBARS_INSIDE_OVERLAY = 0;

    /**
     * The scrollbar style to display the scrollbars inside the padded area,
     * increasing the padding of the view. The scrollbars will not overlap the
     * content area of the view.
     */
    static const Int32 SCROLLBARS_INSIDE_INSET = 0x01000000;

    /**
     * The scrollbar style to display the scrollbars at the edge of the view,
     * without increasing the padding. The scrollbars will be overlaid with
     * translucency.
     */
    static const Int32 SCROLLBARS_OUTSIDE_OVERLAY = 0x02000000;

    /**
     * The scrollbar style to display the scrollbars at the edge of the view,
     * increasing the padding of the view. The scrollbars will only overlap the
     * background, if any.
     */
    static const Int32 SCROLLBARS_OUTSIDE_INSET = 0x03000000;

    /**
     * Mask to check if the scrollbar style is overlay or inset.
     * {@hide}
     */
    static const Int32 SCROLLBARS_INSET_MASK = 0x01000000;

    /**
     * Mask to check if the scrollbar style is inside or outside.
     * {@hide}
     */
    static const Int32 SCROLLBARS_OUTSIDE_MASK = 0x02000000;

    /**
     * Mask for scrollbar style.
     * {@hide}
     */
    static const Int32 SCROLLBARS_STYLE_MASK = 0x03000000;

    /**
     * View flag indicating that the screen should remain on while the
     * window containing this view is visible to the user.  This effectively
     * takes care of automatically setting the WindowManager's
     * {@link WindowManager.LayoutParams#FLAG_KEEP_SCREEN_ON}.
     */
    static const Int32 KEEP_SCREEN_ON = 0x04000000;

    /**
     * View flag indicating whether this view should have sound effects enabled
     * for events such as clicking and touching.
     */
    static const Int32 SOUND_EFFECTS_ENABLED = 0x08000000;

    /**
     * View flag indicating whether this view should have haptic feedback
     * enabled for events such as long presses.
     */
    static const Int32 HAPTIC_FEEDBACK_ENABLED = 0x10000000;

    /**
     * View flag indicating whether {@link #addFocusables(ArrayList, int, int)}
     * should add all focusable Views regardless if they are focusable in touch mode.
     */
    static const Int32 FOCUSABLES_ALL = 0x00000000;

    /**
     * View flag indicating whether {@link #addFocusables(ArrayList, int, int)}
     * should add only Views focusable in touch mode.
     */
    static const Int32 FOCUSABLES_TOUCH_MODE = 0x00000001;

    /**
     * Use with {@link #focusSearch}. Move focus to the previous selectable
     * item.
     */
    static const Int32 FOCUS_BACKWARD = 0x00000001;

    /**
     * Use with {@link #focusSearch}. Move focus to the next selectable
     * item.
     */
    static const Int32 FOCUS_FORWARD = 0x00000002;

    /**
     * Use with {@link #focusSearch}. Move focus to the left.
     */
    static const Int32 FOCUS_LEFT = 0x00000011;

    /**
     * Use with {@link #focusSearch}. Move focus up.
     */
    static const Int32 FOCUS_UP = 0x00000021;

    /**
     * Use with {@link #focusSearch}. Move focus to the right.
     */
    static const Int32 FOCUS_RIGHT = 0x00000042;

    /**
     * Use with {@link #focusSearch}. Move focus down.
     */
    static const Int32 FOCUS_DOWN = 0x00000082;

    /**
     * Temporary Rect currently for use in setBackground().  This will probably
     * be extended in the future to hold our own class with more than just
     * a Rect. :)
     */
    static pthread_key_t sKey;

    // for mPrivateFlags:
    /** {@hide} */
    static const Int32 WANTS_FOCUS = 0x00000001;
    /** {@hide} */
    static const Int32 FOCUSED = 0x00000002;
    /** {@hide} */
    static const Int32 SELECTED = 0x00000004;
    /** {@hide} */
    static const Int32 IS_ROOT_NAMESPACE = 0x00000008;
    /** {@hide} */
    static const Int32 HAS_BOUNDS = 0x00000010;
    /** {@hide} */
    static const Int32 DRAWN = 0x00000020;
    /**
     * When this flag is set, this view is running an animation on behalf of its
     * children and should therefore not cancel invalidate requests, even if they
     * lie outside of this view's bounds.
     *
     * {@hide}
     */
    static const Int32 DRAW_ANIMATION = 0x00000040;
    /** {@hide} */
    static const Int32 SKIP_DRAW = 0x00000080;
    /** {@hide} */
    static const Int32 ONLY_DRAWS_BACKGROUND = 0x00000100;
    /** {@hide} */
    static const Int32 REQUEST_TRANSPARENT_REGIONS = 0x00000200;
    /** {@hide} */
    static const Int32 DRAWABLE_STATE_DIRTY = 0x00000400;
    /** {@hide} */
    static const Int32 MEASURED_DIMENSION_SET = 0x00000800;
    /** {@hide} */
    static const Int32 FORCE_LAYOUT = 0x00001000;
    /** {@hide} */
    static const Int32 LAYOUT_REQUIRED = 0x00002000;

    static const Int32 PRESSED = 0x00004000;

    /** {@hide} */
    static const Int32 DRAWING_CACHE_VALID = 0x00008000;

    /**
     * Flag used to indicate that this view should be drawn once more (and only once
     * more) after its animation has completed.
     * {@hide}
     */
    static const Int32 ANIMATION_STARTED = 0x00010000;

    static const Int32 SAVE_STATE_CALLED = 0x00020000;

    /**
     * Indicates that the View returned true when onSetAlpha() was called and that
     * the alpha must be restored.
     * {@hide}
     */
    static const Int32 ALPHA_SET = 0x00040000;

    /**
     * Set by {@link #setScrollContainer(boolean)}.
     */
    static const Int32 SCROLL_CONTAINER = 0x00080000;

    /**
     * Set by {@link #setScrollContainer(boolean)}.
     */
    static const Int32 SCROLL_CONTAINER_ADDED = 0x00100000;

    /**
     * View flag indicating whether this view was invalidated (fully or partially.)
     *
     * @hide
     */
    static const Int32 DIRTY = 0x00200000;

    /**
     * View flag indicating whether this view was invalidated by an opaque
     * invalidate request.
     *
     * @hide
     */
    static const Int32 DIRTY_OPAQUE = 0x00400000;

    /**
     * Mask for {@link #DIRTY} and {@link #DIRTY_OPAQUE}.
     *
     * @hide
     */
    static const Int32 DIRTY_MASK = 0x00600000;

    /**
     * Indicates whether the background is opaque.
     *
     * @hide
     */
    static const Int32 OPAQUE_BACKGROUND = 0x00800000;

    /**
     * Indicates whether the scrollbars are opaque.
     *
     * @hide
     */
    static const Int32 OPAQUE_SCROLLBARS = 0x01000000;

    /**
     * Indicates whether the view is opaque.
     *
     * @hide
     */
    static const Int32 OPAQUE_MASK = 0x01800000;

    /**
     * Indicates a prepressed state;
     * the short time between ACTION_DOWN and recognizing
     * a 'real' press. Prepressed is used to recognize quick taps
     * even when they are shorter than ViewConfiguration.getTapTimeout().
     *
     * @hide
     */
    static const Int32 PREPRESSED = 0x02000000;

    /**
     * Indicates whether the view is temporarily detached.
     *
     * @hide
     */
    static const Int32 CANCEL_NEXT_UP_EVENT = 0x04000000;

    /**
     * Indicates that we should awaken scroll bars once attached
     *
     * @hide
     */
    static const Int32 AWAKEN_SCROLL_BARS_ON_ATTACH = 0x08000000;

    /**
     * Always allow a user to over-scroll this view, provided it is a
     * view that can scroll.
     *
     * @see #getOverScrollMode()
     * @see #setOverScrollMode(int)
     */
    static const Int32 OVER_SCROLL_ALWAYS = 0;

    /**
     * Allow a user to over-scroll this view only if the content is large
     * enough to meaningfully scroll, provided it is a view that can scroll.
     *
     * @see #getOverScrollMode()
     * @see #setOverScrollMode(int)
     */
    static const Int32 OVER_SCROLL_IF_CONTENT_SCROLLS = 1;

    /**
     * Never allow a user to over-scroll this view.
     *
     * @see #getOverScrollMode()
     * @see #setOverScrollMode(int)
     */
    static const Int32 OVER_SCROLL_NEVER = 2;

public:
    /**
     * A MeasureSpec encapsulates the layout requirements passed from parent to child.
     * Each MeasureSpec represents a requirement for either the width or the height.
     * A MeasureSpec is comprised of a size and a mode. There are three possible
     * modes:
     * <dl>
     * <dt>UNSPECIFIED</dt>
     * <dd>
     * The parent has not imposed any constraint on the child. It can be whatever size
     * it wants.
     * </dd>
     *
     * <dt>EXACTLY</dt>
     * <dd>
     * The parent has determined an exact size for the child. The child is going to be
     * given those bounds regardless of how big it wants to be.
     * </dd>
     *
     * <dt>AT_MOST</dt>
     * <dd>
     * The child can be as large as it wants up to the specified size.
     * </dd>
     * </dl>
     *
     * MeasureSpecs are implemented as ints to reduce object allocation. This class
     * is provided to pack and unpack the &lt;size, mode&gt; tuple into the int.
     */
    class MeasureSpec
    {
    private:
        static const Int32 MODE_SHIFT = 30;
        static const Int32 MODE_MASK  = 0x3 << MODE_SHIFT;

    public:
        /**
         * Measure specification mode: The parent has not imposed any constraint
         * on the child. It can be whatever size it wants.
         */
        static const Int32 UNSPECIFIED = 0 << MODE_SHIFT;

        /**
         * Measure specification mode: The parent has determined an exact size
         * for the child. The child is going to be given those bounds regardless
         * of how big it wants to be.
         */
        static const Int32 EXACTLY     = 1 << MODE_SHIFT;

        /**
         * Measure specification mode: The child can be as large as it wants up
         * to the specified size.
         */
        static const Int32 AT_MOST     = 2 << MODE_SHIFT;

    public:
        /**
         * Creates a measure specification based on the supplied size and mode.
         *
         * The mode must always be one of the following:
         * <ul>
         *  <li>{@link android.view.View.MeasureSpec#UNSPECIFIED}</li>
         *  <li>{@link android.view.View.MeasureSpec#EXACTLY}</li>
         *  <li>{@link android.view.View.MeasureSpec#AT_MOST}</li>
         * </ul>
         *
         * @param size the size of the measure specification
         * @param mode the mode of the measure specification
         * @return the measure specification based on size and mode
         */
        static CARAPI_(Int32) MakeMeasureSpec(
            /* [in] */ Int32 size,
            /* [in] */ Int32 mode)
        {
            return size + mode;
        }

        /**
         * Extracts the mode from the supplied measure specification.
         *
         * @param measureSpec the measure specification to extract the mode from
         * @return {@link android.view.View.MeasureSpec#UNSPECIFIED},
         *         {@link android.view.View.MeasureSpec#AT_MOST} or
         *         {@link android.view.View.MeasureSpec#EXACTLY}
         */
        static CARAPI_(Int32) GetMode(
            /* [in] */ Int32 measureSpec)
        {
            return (measureSpec & MODE_MASK);
        }

        /**
         * Extracts the size from the supplied measure specification.
         *
         * @param measureSpec the measure specification to extract the size from
         * @return the size in pixels defined in the supplied measure specification
         */
        static CARAPI_(Int32) GetSize(
            /* [in] */ Int32 measureSpec)
        {
            return (measureSpec & ~MODE_MASK);
        }

        /**
         * Returns a String representation of the specified measure
         * specification.
         *
         * @param measureSpec the measure specification to convert to a String
         * @return a String with the following format: "MeasureSpec: MODE SIZE"
         */
        static CARAPI GetDescription(
            /* [in] */ Int32 measureSpec,
            /* [out] */ String* description)
        {
            if (description == NULL) return E_INVALID_ARGUMENT;

            Int32 mode = GetMode(measureSpec);
            Int32 size = GetSize(measureSpec);

            StringBuffer sb("MeasureSpec: ");

            if (mode == UNSPECIFIED) {
                sb += "UNSPECIFIED ";
            }
            else if (mode == EXACTLY) {
                sb += "EXACTLY ";
            }
            else if (mode == AT_MOST) {
                sb += "AT_MOST ";
            }
            else {
                sb += mode + " ";
            }

            sb += size;
            *description = (const char*)sb;
            return NOERROR;
        }
    };

    class CheckForLongPress : public Runnable
    {
    public:
        CheckForLongPress(
            /* [in] */ AutoPtr<IView>& view) : mView(view) {}

        CARAPI Run();

        CARAPI_(void) RememberWindowAttachCount();

    private:
        AutoPtr<IView> mView;
        Int32 mOriginalWindowAttachCount;
    };

    class AttachInfo
    {
    public:
        class Callbacks
        {
        public:
            virtual ~Callbacks() {}

            virtual CARAPI_(void) PlaySoundEffect(
                /* [in] */ Int32 effectId) = 0;

            virtual CARAPI PerformHapticFeedback(
                /* [in] */ Int32 effectId,
                /* [in] */ Boolean always,
                /* [out] */ Boolean* result) = 0;
        };

    public:
        AttachInfo(
            /* [in] */ IWindowSession* session,
            /* [in] */ IInnerWindow* window,
            /* [in] */ IApartment* handler,
            /* [in] */ Callbacks* effectPlayer);

        ~AttachInfo();

    public:
        AutoPtr<IWindowSession> mSession;

        AutoPtr<IInnerWindow> mWindow;

        AutoPtr<IBinder> mWindowToken;

        Callbacks* mRootCallbacks;
        /**
        * The top view of the hierarchy.
        */
        AutoPtr<IView> mRootView;

        AutoPtr<IBinder> mPanelParentWindowToken;

        AutoPtr<CSurface> mSurface;

        /**
        * Scale factor used by the compatibility mode
        */
        Float mApplicationScale;

        /**
        * Indicates whether the application is in compatibility mode
        */
        Boolean mScalingRequired;

        /**
        * Left position of this view's window
        */
        Int32 mWindowLeft;

        /**
        * Top position of this view's window
        */
        Int32 mWindowTop;

        /**
        * Indicates whether views need to use 32-bit drawing caches
        */
        Boolean mUse32BitDrawingCache;

        /**
        * For windows that are full-screen but using insets to layout inside
        * of the screen decorations, these are the current insets for the
        * content of the window.
        */
        AutoPtr<CRect> mContentInsets;

        /**
        * For windows that are full-screen but using insets to layout inside
        * of the screen decorations, these are the current insets for the
        * actual visible parts of the window.
        */
        AutoPtr<CRect> mVisibleInsets;

        /**
         * The internal insets given by this window.  This value is
         * supplied by the client (through
         * {@link ViewTreeObserver.OnComputeInternalInsetsListener}) and will
         * be given to the window manager when changed to be used in laying
         * out windows behind it.
         */
        ViewTreeObserver::InternalInsetsInfo* mGivenInternalInsets;

        /**
         * All views in the window's hierarchy that serve as scroll containers,
         * used to determine if the window can be resized or must be panned
         * to adjust for a soft input area.
         */
        List<AutoPtr<IView> > mScrollContainers;

        AutoPtr<IDispatcherState> mKeyDispatchState;
        /**
        * Indicates whether the view's window currently has the focus.
        */
        Boolean mHasWindowFocus;

        /**
        * The current visibility of the window.
        */
        Int32 mWindowVisibility;

        /**
        * Indicates the time at which drawing started to occur.
        */
        Int64 mDrawingTime;

        /**
         * Indicates whether or not ignoring the DIRTY_MASK flags.
         */
        Boolean mIgnoreDirtyState;

        /**
        * Indicates whether the view's window is currently in touch mode.
        */
        Boolean mInTouchMode;

        /**
        * Indicates that ViewRoot should trigger a global layout change
        * the next time it performs a traversal
        */
        Boolean mRecomputeGlobalAttributes;

        /**
        * Set during a traveral if any views want to keep the screen on.
        */
        Boolean mKeepScreenOn;

        /**
        * Set if the visibility of any views has changed.
        */
        Boolean mViewVisibilityChanged;

        /**
        * Set to true if a view has been scrolled.
        */
        Boolean mViewScrollChanged;

        AutoPtr<IViewTreeObserver> mTreeObserver;

        /**
         * Global to the view hierarchy used as a temporary for dealing with
         * x/y points in the transparent region computations.
         */
        Int32 mTransparentLocation[2];

        /**
         * Global to the view hierarchy used as a temporary for dealing with
         * x/y points in the ViewGroup.invalidateChild implementation.
         */
        Int32 mInvalidateChildLocation[2];

        /**
         * A Canvas used by the view hierarchy to perform bitmap caching.
         */
        AutoPtr<ICanvas> mCanvas;

        /**
         * A Handler supplied by a view's {@link android.view.ViewRoot}. This
         * handler can be used to pump events in the UI events queue.
         */
        AutoPtr<IApartment> mHandler;

        /**
         * Temporary for use in computing invalidate rectangles while
         * calling up the hierarchy.
         */
        AutoPtr<CRect> mTmpInvalRect;
    };

private:
    class CheckForTap : public Runnable
    {
    public:
        CheckForTap(
            /* [in] */ AutoPtr<IView>& view) : mView(view) {}

        CARAPI Run();

    private:
        AutoPtr<IView> mView;
    };

    class ViewPerformClick : public Runnable
    {
    public:
        ViewPerformClick(
            /* [in] */ AutoPtr<IView>& view) : mView(view) {}

        CARAPI Run();

    private:
        AutoPtr<IView> mView;
    };

    class UnsetPressedState : public Runnable
    {
    public:
        UnsetPressedState(
            /* [in] */ AutoPtr<IView>& view) : mView(view) {}

        CARAPI Run();

    private:
        AutoPtr<IView> mView;
    };

    /**
     * <p>ScrollabilityCache holds various fields used by a View when scrolling
     * is supported. This avoids keeping too many unused fields in most
     * instances of View.</p>
     */
    class ScrollabilityCache : public Runnable
    {
    public:
        /**
         * Scrollbars are not visible
         */
        static const Int32 OFF = 0;

        /**
         * Scrollbars are visible
         */
        static const Int32 ON = 1;

        /**
         * Scrollbars are fading away
         */
        static const Int32 FADING = 2;

    public:
        ScrollabilityCache(
            /* [in] */ AutoPtr<ViewConfiguration>& configuration,
            /* [in] */ AutoPtr<IView>& host);

        CARAPI Run();

    public:
        CARAPI_(void) SetFadeColor(
            /* [in] */ Int32 color);

    public:
        Boolean mFadeScrollBars;
        Int32 mFadingEdgeLength;
        Int32 mScrollBarDefaultDelayBeforeFade;
        Int32 mScrollBarFadeDuration;
        Int32 mScrollBarSize;
        AutoPtr<IScrollBarDrawable> mScrollBar;
        ArrayOf<Float>* mInterpolatorValues;
        AutoPtr<IView> mHost;

        AutoPtr<IPaint> mPaint;
        AutoPtr<IMatrix> mMatrix;
        AutoPtr<IShader> mShader;

        Interpolator* mScrollBarInterpolator;

        /**
         * When fading should start. This time moves into the future every time
         * a new scroll happens. Measured based on SystemClock.uptimeMillis()
         */
        Int64 mFadeStartTime;

        /**
         * The current state of the scrollbars: ON, OFF, or FADING
         */
        Int32 mState;

        Int32 mLastColor;
    };

public:
    View();

    View(
        /* [in] */ IContext* context);

    View(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    View(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    ~View();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI_(Int32) GetVerticalFadingEdgeLength();

    virtual CARAPI SetFadingEdgeLength(
        /* [in] */ Int32 length);

    virtual CARAPI_(Int32) GetHorizontalFadingEdgeLength();

    virtual CARAPI_(Int32) GetVerticalScrollbarWidth();

    //virtual CARAPI_(Int32) GetHorizontalScrollbarHeight();

    virtual CARAPI SetOnFocusChangeListener(
        /* [in] */ IViewOnFocusChangeListener* l);

    virtual CARAPI GetOnFocusChangeListener(
        /* [out] */ IViewOnFocusChangeListener** l);

    virtual CARAPI SetOnClickListener(
        /* [in] */ IViewOnClickListener* l);

    virtual CARAPI SetOnLongClickListener(
        /* [in] */ IViewOnLongClickListener* l);

    virtual CARAPI SetOnCreateContextMenuListener(
        /* [in] */ IViewOnCreateContextMenuListener* l);

    virtual CARAPI_(Boolean) PerformClick();

    virtual CARAPI_(Boolean) PerformLongClick();

    virtual CARAPI_(Boolean) ShowContextMenu();

    virtual CARAPI SetOnKeyListener(
        /* [in] */ IViewOnKeyListener* l);

    virtual CARAPI SetOnTouchListener(
        /* [in] */ IViewOnTouchListener* l);

    /**
     * Give this view focus. This will cause {@link #onFocusChanged} to be called.
     *
     * Note: this does not check whether this {@link View} should get focus, it just
     * gives it focus no matter what.  It should only be called internally by framework
     * code that knows what it is doing, namely {@link #requestFocus(int, Rect)}.
     *
     * @param direction values are View.FOCUS_UP, View.FOCUS_DOWN,
     *        View.FOCUS_LEFT or View.FOCUS_RIGHT. This is the direction which
     *        focus moved when requestFocus() is called. It may not always
     *        apply, in which case use the default View.FOCUS_DOWN.
     * @param previouslyFocusedRect The rectangle of the view that had focus
     *        prior in this View's coordinate system.
     */
    virtual CARAPI HandleFocusGainInternal(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    virtual CARAPI_(Boolean) RequestRectangleOnScreen(
        /* [in] */ IRect* rectangle);

    virtual CARAPI_(Boolean) RequestRectangleOnScreen(
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate);

    virtual CARAPI ClearFocus();

    /**
     * Called to clear the focus of a view that is about to be removed.
     * Doesn't call clearChildFocus, which prevents this view from taking
     * focus again before it has been removed from the parent
     */
    virtual CARAPI ClearFocusForRemoval();

    /**
     * Called internally by the view system when a new view is getting focus.
     * This is what clears the old focus.
     */
    virtual CARAPI UnFocus();

    virtual CARAPI_(Boolean) HasFocus();

    virtual CARAPI_(Boolean) HasFocusable();

    virtual CARAPI SendAccessibilityEvent(
        /* [in] */ Int32 eventType);

    virtual CARAPI SendAccessibilityEventUnchecked(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI_(ArrayOf<Char8>*) GetContentDescription();

    virtual CARAPI SetContentDescription(
        /* [in] */ const ArrayOf<Char8>& contentDescription);

    virtual CARAPI_(Boolean) IsFocused();

    virtual CARAPI_(AutoPtr<IView>) FindFocus();

    virtual CARAPI SetScrollContainer(
        /* [in] */ Boolean isScrollContainer);

    virtual CARAPI_(Int32) GetDrawingCacheQuality();

    virtual CARAPI SetDrawingCacheQuality(
        /* [in] */ Int32 quality);

    virtual CARAPI_(Boolean) GetKeepScreenOn();

    virtual CARAPI SetKeepScreenOn(
        /* [in] */ Boolean keepScreenOn);

    virtual CARAPI_(Int32) GetNextFocusLeftId();

    virtual CARAPI SetNextFocusLeftId(
        /* [in] */ Int32 nextFocusLeftId);

    virtual CARAPI_(Int32) GetNextFocusRightId();

    virtual CARAPI SetNextFocusRightId(
        /* [in] */ Int32 nextFocusRightId);

    virtual CARAPI_(Int32) GetNextFocusUpId();

    virtual CARAPI SetNextFocusUpId(
        /* [in] */ Int32 nextFocusUpId);

    virtual CARAPI_(Int32) GetNextFocusDownId();

    virtual CARAPI SetNextFocusDownId(
        /* [in] */ Int32 nextFocusDownId);

    virtual CARAPI_(Boolean) IsShown();

    virtual CARAPI_(Boolean) IsFitsSystemWindowsFlagSet();

    virtual CARAPI_(Int32) GetVisibility();

    virtual CARAPI SetVisibility(
        /* [in] */ Int32 visibility);

    virtual CARAPI_(Boolean) IsEnabled();

    virtual CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI SetFocusable(
        /* [in] */ Boolean focusable);

    virtual CARAPI SetFocusableInTouchMode(
        /* [in] */ Boolean focusableInTouchMode);

    virtual CARAPI SetSoundEffectsEnabled(
        /* [in] */ Boolean soundEffectsEnabled);

    virtual CARAPI_(Boolean) IsSoundEffectsEnabled();

    virtual CARAPI SetHapticFeedbackEnabled(
        /* [in] */ Boolean hapticFeedbackEnabled);

    virtual CARAPI_(Boolean) IsHapticFeedbackEnabled();

    virtual CARAPI SetWillNotDraw(
        /* [in] */ Boolean willNotDraw);

    virtual CARAPI_(Boolean) WillNotDraw();

    virtual CARAPI SetWillNotCacheDrawing(
        /* [in] */ Boolean willNotCacheDrawing);

    virtual CARAPI_(Boolean) WillNotCacheDrawing();

    virtual CARAPI_(Boolean) IsClickable();

    virtual CARAPI SetClickable(
        /* [in] */ Boolean clickable);

    virtual CARAPI_(Boolean) IsLongClickable();

    virtual CARAPI SetLongClickable(
        /* [in] */ Boolean longClickable);

    virtual CARAPI SetPressed(
        /* [in] */ Boolean pressed);

    virtual CARAPI_(Boolean) IsPressed();

    virtual CARAPI_(Boolean) IsSaveEnabled();

    virtual CARAPI SetSaveEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) GetFilterTouchesWhenObscured();

    virtual CARAPI SetFilterTouchesWhenObscured(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsFocusable();

    virtual CARAPI_(Boolean) IsFocusableInTouchMode();

    virtual CARAPI_(AutoPtr<IView>) FocusSearch(
        /* [in] */ Int32 direction);

    virtual CARAPI_(Boolean) DispatchUnhandledMove(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction);

    /**
     * If a user manually specified the next view id for a particular direction,
     * use the root to look up the view.  Once a view is found, it is cached
     * for future lookups.
     * @param root The root view of the hierarchy containing this view.
     * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
     * @return The user specified next view, or NULL if there is none.
     */
    virtual CARAPI_(AutoPtr<IView>) FindUserSetNextFocus(
        /* [in] */ IView* root,
        /* [in] */ Int32 direction);

    virtual CARAPI GetFocusables(
        /* [in] */ Int32 direction,
        /* [out] */ IObjectContainer** views);

    virtual CARAPI AddFocusables(
        /* [in] */ IObjectContainer* views,
        /* [in] */ Int32 direction);

    virtual CARAPI AddFocusables(
        /* [in] */ IObjectContainer* views,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 focusableMode);

    virtual CARAPI GetTouchables(
        /* [out] */ IObjectContainer** views);

    virtual CARAPI AddTouchables(
        /* [in] */ IObjectContainer* views);

    virtual CARAPI_(Boolean) RequestFocus();

    virtual CARAPI_(Boolean) RequestFocus(
        /* [in] */ Int32 direction);

    virtual CARAPI_(Boolean) RequestFocus(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    virtual CARAPI_(Boolean) RequestFocusFromTouch();

    virtual CARAPI DispatchStartTemporaryDetach();

    virtual CARAPI OnStartTemporaryDetach();

    virtual CARAPI DispatchFinishTemporaryDetach();

    virtual CARAPI OnFinishTemporaryDetach();

    virtual CARAPI GetKeyDispatcherState(
        /* [out] */ IDispatcherState** state);

    virtual CARAPI_(Boolean) DispatchKeyEventPreIme(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) DispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) DispatchTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnFilterTouchEventForSecurity(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) DispatchTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI DispatchWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    virtual CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    virtual CARAPI_(Boolean) HasWindowFocus();

    virtual CARAPI DispatchDisplayHint(
        /* [in] */ Int32 hint);

    virtual CARAPI DispatchWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    virtual CARAPI_(Int32) GetWindowVisibility();

    virtual CARAPI GetWindowVisibleDisplayFrame(
        /* [in, out] */ IRect* outRect);

    virtual CARAPI DispatchConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    /**
     * Private function to aggregate all per-view attributes in to the view
     * root.
     */
    virtual CARAPI DispatchCollectViewAttributes(
        /* [in] */ Int32 visibility);

    virtual CARAPI PerformCollectViewAttributes(
        /* [in] */ Int32 visibility);

    virtual CARAPI NeedGlobalAttributesUpdate(
        /* [in] */ Boolean force);

    virtual CARAPI_(Boolean) IsInTouchMode();

    virtual CARAPI_(AutoPtr<IContext>) GetContext();

    virtual CARAPI_(Boolean) OnKeyPreIme(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyLongPress(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnCheckIsTextEditor();

    //virtual CARAPI_(IInputConnection*) OnCreateInputConnection(
    //    /* [in] */ IEditorInfo* outAttrs);

    virtual CARAPI_(Boolean) CheckInputConnectionProxy(
        /* [in] */ IView* view);

    virtual CARAPI CreateContextMenu(
        /* [in] */ IContextMenu* menu);

    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI CancelLongPress();

    virtual CARAPI SetTouchDelegate(
        /* [in] */ ITouchDelegate* touchDelegate);

    virtual CARAPI GetTouchDelegate(
        /* [out] */ ITouchDelegate** touchDelegate);

    virtual CARAPI BringToFront();

    virtual CARAPI_(AutoPtr<IViewParent>) GetParent();

    virtual CARAPI_(Int32) GetScrollX();

    virtual CARAPI_(Int32) GetScrollY();

    virtual CARAPI_(Int32) GetWidth();

    virtual CARAPI_(Int32) GetHeight();

    virtual CARAPI GetDrawingRect(
        /* [out] */ IRect* outRect);

    virtual CARAPI_(Int32) GetMeasuredWidth();

    virtual CARAPI_(Int32) GetMeasuredHeight();

    virtual CARAPI_(Int32) GetTop();

    virtual CARAPI_(Int32) GetBottom();

    virtual CARAPI_(Int32) GetLeft();

    virtual CARAPI_(Int32) GetRight();

    virtual CARAPI GetHitRect(
        /* [in, out] */ IRect* outRect);

    virtual CARAPI GetFocusedRect(
        /* [in, out] */ IRect* outRect);

    virtual CARAPI_(Boolean) GetGlobalVisibleRect(
        /* [in, out] */ IRect* outRect,
        /* [in, out] */ IPoint* outGlobalOffset);

    virtual CARAPI_(Boolean) GetGlobalVisibleRect(
        /* [in, out] */ IRect* outRect);

    virtual CARAPI_(Boolean) GetLocalVisibleRect(
        /* [in, out] */ IRect* outRect);

    virtual CARAPI OffsetTopAndBottom(
        /* [in] */ Int32 offset);

    virtual CARAPI OffsetLeftAndRight(
        /* [in] */ Int32 offset);

    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GetLayoutParams();

    virtual CARAPI SetLayoutParams(
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI ScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    virtual CARAPI ScrollBy(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    virtual CARAPI Invalidate(
        /* [in] */ IRect* dirty);

    virtual CARAPI Invalidate(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI Invalidate();

    virtual CARAPI_(Boolean) IsOpaque();

    virtual CARAPI_(AutoPtr<IApartment>) GetHandler();

    /**
     * Causes the Runnable to be added to the message queue.
     * The runnable will be run on the user interface thread.
     *
     * @param action The Runnable that will be executed.
     *
     * @return Returns true if the Runnable was successfully placed in to the
     *         message queue.  Returns false on failure, usually because the
     *         looper processing the message queue is exiting.
     */
    virtual CARAPI_(Boolean) Post(
        /* [in] */ IRunnable* action);

    /**
     * Causes the Runnable to be added to the message queue, to be run
     * after the specified amount of time elapses.
     * The runnable will be run on the user interface thread.
     *
     * @param action The Runnable that will be executed.
     * @param delayMillis The delay (in milliseconds) until the Runnable
     *        will be executed.
     *
     * @return true if the Runnable was successfully placed in to the
     *         message queue.  Returns false on failure, usually because the
     *         looper processing the message queue is exiting.  Note that a
     *         result of true does not mean the Runnable will be processed --
     *         if the looper is quit before the delivery time of the message
     *         occurs then the message will be dropped.
     */
    virtual CARAPI_(Boolean) PostDelayed(
        /* [in] */ IRunnable* action,
        /* [in] */ Int64 delayMillis);

    /**
     * Removes the specified Runnable from the message queue.
     *
     * @param action The Runnable to remove from the message handling queue
     *
     * @return true if this view could ask the Handler to remove the Runnable,
     *         false otherwise. When the returned value is true, the Runnable
     *         may or may not have been actually removed from the message queue
     *         (for instance, if the Runnable was not in the queue already.)
     */
    virtual CARAPI_(Boolean) RemoveCallbacks(
        /* [in] */ IRunnable* action);

    virtual CARAPI PostInvalidate();

    virtual CARAPI PostInvalidate(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI PostInvalidateDelayed(
        /* [in] */ Int64 delayMilliseconds);

    virtual CARAPI PostInvalidateDelayed(
        /* [in] */ Int64 delayMilliseconds,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI ComputeScroll();

    virtual CARAPI_(Boolean) IsHorizontalFadingEdgeEnabled();

    virtual CARAPI SetHorizontalFadingEdgeEnabled(
        /* [in] */ Boolean horizontalFadingEdgeEnabled);

    virtual CARAPI_(Boolean) IsVerticalFadingEdgeEnabled();

    virtual CARAPI SetVerticalFadingEdgeEnabled(
        /* [in] */ Boolean verticalFadingEdgeEnabled);

    virtual CARAPI_(Boolean) IsHorizontalScrollBarEnabled();

    virtual CARAPI SetHorizontalScrollBarEnabled(
        /* [in] */ Boolean horizontalScrollBarEnabled);

    virtual CARAPI_(Boolean) IsVerticalScrollBarEnabled();

    virtual CARAPI SetVerticalScrollBarEnabled(
        /* [in] */ Boolean verticalScrollBarEnabled);

    virtual CARAPI SetScrollbarFadingEnabled(
        /* [in] */ Boolean fadeScrollbars);

    virtual CARAPI_(Boolean) IsScrollbarFadingEnabled();

    virtual CARAPI SetScrollBarStyle(
        /* [in] */ Int32 style);

    virtual CARAPI_(Int32) GetScrollBarStyle();

    virtual CARAPI AssignParent(
        /* [in] */ IViewParent* parent);

    virtual CARAPI_(AutoPtr<IBinder>) GetWindowToken();

    virtual CARAPI_(AutoPtr<IBinder>) GetApplicationWindowToken();

    virtual CARAPI_(AutoPtr<IWindowSession>) GetWindowSession();

    virtual CARAPI_(void) DispatchAttachedToWindow(
        /* [in] */ AttachInfo* info,
        /* [in] */ Int32 visibility);

    virtual CARAPI DispatchDetachedFromWindow();

    virtual CARAPI SaveHierarchyState(
        /* [in] */ IObjectIntegerMap* container);

    virtual CARAPI RestoreHierarchyState(
        /* [in] */ IObjectIntegerMap* container);

    virtual CARAPI_(Int64) GetDrawingTime();

    virtual CARAPI SetDuplicateParentStateEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsDuplicateParentStateEnabled();

    virtual CARAPI SetDrawingCacheEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsDrawingCacheEnabled();

    virtual CARAPI_(AutoPtr<IBitmap>) GetDrawingCache();

    virtual CARAPI_(AutoPtr<IBitmap>) GetDrawingCache(
        /* [in] */ Boolean autoScale);

    virtual CARAPI DestroyDrawingCache();

    virtual CARAPI SetDrawingCacheBackgroundColor(
        /* [in] */ Int32 color);

    virtual CARAPI_(Int32) GetDrawingCacheBackgroundColor();

    virtual CARAPI BuildDrawingCache();

    virtual CARAPI_(void) BuildDrawingCache(
        /* [in] */ Boolean autoScale);

    virtual CARAPI CreateSnapshot(
        /* [in] */ BitmapConfig quality,
        /* [in] */ Int32 backgroundColor,
        /* [in] */ Boolean skipChildren,
        /* [out] */ IBitmap** bitmap);

    virtual CARAPI_(Boolean) IsInEditMode();

    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Int32) GetSolidColor();

    virtual CARAPI_(Boolean) IsLayoutRequested();

    virtual CARAPI Layout(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    virtual CARAPI_(AutoPtr<IResources>) GetResources();

    virtual CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* drawable);

    virtual CARAPI ScheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    virtual CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what);

    virtual CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who);

    virtual CARAPI RefreshDrawableState();

    virtual CARAPI_(ArrayOf<Int32>*) GetDrawableState();

    virtual CARAPI SetBackgroundColor(
        /* [in] */ Int32 color);

    virtual CARAPI SetBackgroundResource(
        /* [in] */ Int32 resid);

    virtual CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* d);

    virtual CARAPI_(AutoPtr<IDrawable>) GetBackground();

    virtual CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(Int32) GetPaddingTop();

    virtual CARAPI_(Int32) GetPaddingBottom();

    virtual CARAPI_(Int32) GetPaddingLeft();

    virtual CARAPI_(Int32) GetPaddingRight();

    virtual CARAPI SetSelected(
        /* [in] */ Boolean selected);

    virtual CARAPI_(Boolean) IsSelected();

    virtual CARAPI_(AutoPtr<IViewTreeObserver>) GetViewTreeObserver();

    virtual CARAPI_(AutoPtr<IView>) GetRootView();

    virtual CARAPI GetLocationOnScreen(
        /* [out] */ Int32* x,
        /* [out] */ Int32* y);

    virtual CARAPI GetLocationInWindow(
        /* [out] */ Int32* x,
        /* [out] */ Int32* y);

    CARAPI_(AutoPtr<IView>) FindViewById(
        /* [in] */ Int32 id);

    CARAPI_(AutoPtr<IView>) FindViewWithTag(
        /* [in] */ IInterface* tag);

    virtual CARAPI SetId(
        /* [in] */ Int32 id);

    virtual CARAPI SetIsRootNamespace(
        /* [in] */ Boolean isRoot);

    virtual CARAPI_(Boolean) IsRootNamespace();

    virtual CARAPI_(Int32) GetId();

    virtual CARAPI_(AutoPtr<IInterface>) GetTag();

    virtual CARAPI SetTag(
        /* [in] */ IInterface* tag);

    //virtual CARAPI_(IObject*) GetTag(
    //    /* [in] */ Int32 key);

//    virtual CARAPI_(void) SetTag(
//        /* [in] */ Int32 key,
//        /* [in] */ IObject* tag);
//
//    virtual CARAPI_(void) SetTagInternal(
//        /* [in] */ Int32 key,
//        /* [in] */ IObject* tag);

    virtual CARAPI_(void) Debug();

    virtual CARAPI GetBaseline(
        /* [out] */ Int32* baseline);

    virtual CARAPI RequestLayout();

    virtual CARAPI ForceLayout();

    CARAPI Measure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    /**
     * Utility to reconcile a desired size with constraints imposed by a MeasureSpec.
     * Will take the desired size, unless a different size is imposed by the constraints.
     *
     * @param size How big the view wants to be
     * @param measureSpec Constraints imposed by the parent
     * @return The size this view should be.
     */
    static CARAPI_(Int32) ResolveSize(
        /* [in] */ Int32 size,
        /* [in] */ Int32 measureSpec);

    /**
     * Utility to return a default size. Uses the supplied size if the
     * MeasureSpec imposed no contraints. Will get larger if allowed
     * by the MeasureSpec.
     *
     * @param size Default size for this view
     * @param measureSpec Constraints imposed by the parent
     * @return The size this view should be.
     */
    static CARAPI_(Int32) GetDefaultSize(
        /* [in] */ Int32 size,
        /* [in] */ Int32 measureSpec);

    virtual CARAPI SetMinimumHeight(
        /* [in] */ Int32 minHeight);

    virtual CARAPI SetMinimumWidth(
        /* [in] */ Int32 minWidth);

    virtual CARAPI_(AutoPtr<IAnimation>) GetAnimation();

    virtual CARAPI StartAnimation(
        /* [in] */ IAnimation* animation);

    virtual CARAPI ClearAnimation();

    virtual CARAPI SetAnimation(
        /* [in] */ IAnimation* animation);

    virtual CARAPI_(Boolean) GatherTransparentRegion(
        /* [in] */ IRegion* region);

    virtual CARAPI PlaySoundEffect(
        /* [in] */ Int32 soundConstant);

    virtual CARAPI_(Boolean) PerformHapticFeedback(
        /* [in] */ Int32 feedbackConstant);

    virtual CARAPI_(Boolean) PerformHapticFeedback(
        /* [in] */ Int32 feedbackConstant,
        /* [in] */ Int32 flags);

    virtual CARAPI OnCloseSystemDialogs(
        /* [in] */ const String& reason);

    virtual CARAPI ApplyDrawableToTransparentRegion(
        /* [in] */ IDrawable* dr,
        /* [in] */ IRegion* region);

    static CARAPI Inflate(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ IViewGroup* root,
        /* [out] */ IView** view);

    virtual CARAPI_(Int32) GetOverScrollMode();

    virtual CARAPI SetOverScrollMode(
        /* [in] */ Int32 overScrollMode);

protected:
    virtual CARAPI_(void) InitializeFadingEdge(
        /* [in] */ ITypedArray* a);

    virtual CARAPI_(Int32) GetHorizontalScrollbarHeight();

    virtual CARAPI_(void) InitializeScrollbars(
        /* [in] */ ITypedArray* a);

    virtual CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean gainFocus,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    virtual CARAPI_(void) OnFocusLost();

    virtual CARAPI_(Boolean) FitSystemWindows(
        /* [in] */ IRect* insets);

    virtual CARAPI_(void) DispatchSetPressed(
        /* [in] */ Boolean pressed);

    virtual CARAPI_(void) DispatchVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    virtual CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    virtual CARAPI_(void) OnDisplayHint(
        /* [in] */ Int32 hint);

    virtual CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    virtual CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI_(AutoPtr<IContextMenuInfo>) GetContextMenuInfo();

    virtual CARAPI_(void) OnCreateContextMenu(
        /* [in] */ IContextMenu* menu);

    virtual CARAPI_(void) OnScrollChanged(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 oldl,
        /* [in] */ Int32 oldt);

    virtual CARAPI_(void) SetFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Boolean) AwakenScrollBars();

    virtual CARAPI_(Boolean) AwakenScrollBars(
        /* [in] */ Int32 startDelay);

    virtual CARAPI_(Boolean) AwakenScrollBars(
        /* [in] */ Int32 startDelay,
        /* [in] */ Boolean invalidate);

    virtual CARAPI_(Boolean) HasOpaqueScrollbars();

    virtual CARAPI_(Float) GetTopFadingEdgeStrength();

    virtual CARAPI_(Float) GetBottomFadingEdgeStrength();

    virtual CARAPI_(Float) GetLeftFadingEdgeStrength();

    virtual CARAPI_(Float) GetRightFadingEdgeStrength();

    virtual CARAPI_(Int32) ComputeHorizontalScrollRange();

    virtual CARAPI_(Int32) ComputeHorizontalScrollOffset();

    virtual CARAPI_(Int32) ComputeHorizontalScrollExtent();

    virtual CARAPI_(Int32) ComputeVerticalScrollRange();

    virtual CARAPI_(Int32) ComputeVerticalScrollOffset();

    virtual CARAPI_(Int32) ComputeVerticalScrollExtent();

    virtual CARAPI_(void) OnDrawScrollBars(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Boolean) IsVerticalScrollBarHidden();

    virtual CARAPI_(void) OnDrawHorizontalScrollBar(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IDrawable* scrollBar,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) OnDrawVerticalScrollBar(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IDrawable* scrollBar,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(void) OnAttachedToWindow();

    virtual CARAPI_(void) OnDetachedFromWindow();

    virtual CARAPI_(Int32) GetWindowAttachCount();

    virtual CARAPI DispatchSaveInstanceState(
        /* [in] */ IObjectIntegerMap* container);

    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    virtual CARAPI DispatchRestoreInstanceState(
        /* [in] */ IObjectIntegerMap* container);

    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    virtual CARAPI_(Boolean) IsPaddingOffsetRequired();

    virtual CARAPI_(Int32) GetLeftPaddingOffset();

    virtual CARAPI_(Int32) GetRightPaddingOffset();

    virtual CARAPI_(Int32) GetTopPaddingOffset();

    virtual CARAPI_(Int32) GetBottomPaddingOffset();

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI OnFinishInflate();

    virtual CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    virtual CARAPI DrawableStateChanged();

    virtual CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    static CARAPI_(ArrayOf<Int32>*) MergeDrawableStates(
        /* [in] */ ArrayOf<Int32>* baseState,
        /* [in] */ ArrayOf<Int32>* additionalState);

    virtual CARAPI_(void) DispatchSetSelected(
        /* [in] */ Boolean selected);

    virtual CARAPI_(AutoPtr<IView>) FindViewTraversal(
        /* [in] */ Int32 id);

    virtual CARAPI_(AutoPtr<IView>) FindViewWithTagTraversal(
        /* [in] */ IInterface* tag);

    virtual CARAPI_(Boolean) DispatchConsistencyCheck(
        /* [in] */ Int32 consistency);

    virtual CARAPI_(Boolean) OnConsistencyCheck(
        /* [in] */ Int32 consistency);

    //virtual CARAPI_(void) Debug(
    //    /* [in] */ Int32 depth);

    static String DebugIndent(
        /* [in] */ Int32 depth);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) SetMeasuredDimension(
        /* [in] */ Int32 measuredWidth,
        /* [in] */ Int32 measuredHeight);

    virtual CARAPI_(Int32) GetSuggestedMinimumHeight();

    virtual CARAPI_(Int32) GetSuggestedMinimumWidth();

    virtual CARAPI_(void) OnAnimationStart();

    virtual CARAPI_(void) OnAnimationEnd();

    virtual CARAPI_(Boolean) OnSetAlpha(
        /* [in] */ Int32 alpha);

    virtual CARAPI_(Boolean) OverScrollBy(
        /* [in] */ Int32 deltaX,
        /* [in] */ Int32 deltaY,
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Int32 scrollRangeX,
        /* [in] */ Int32 scrollRangeY,
        /* [in] */ Int32 maxOverScrollX,
        /* [in] */ Int32 maxOverScrollY,
        /* [in] */ Boolean isTouchEvent);

    virtual CARAPI_(void) OnOverScrolled(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Boolean clampedX,
        /* [in] */ Boolean clampedY);

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
    CARAPI_(void) InitScrollCache();

    CARAPI_(void) ResetPressedState();

    static CARAPI_(AutoPtr<IView>) FindViewShouldExist(
        /* [in] */ IView* root,
        /* [in] */ Int32 childViewId);

    CARAPI_(Boolean) HasAncestorThatBlocksDescendantFocus();

    static CARAPI_(void) CaptureViewInfo(
        /* [in] */ const char* subTag,
        /* [in] */ IView* v);

    CARAPI_(void) RemoveLongPressCallback();

    CARAPI_(void) RemoveUnsetPressCallback();

    CARAPI_(void) RemoveTapCallback();

    CARAPI_(Boolean) InitialAwakenScrollBars();

    CARAPI_(void) ComputeOpaqueFlags();

    CARAPI_(void) RecomputePadding();

    static CARAPI PrintFlags(
        /* [in] */ Int32 flags,
        /* [out] */ String* output);

    CARAPI PrintPrivateFlags(
        /* [in] */ Int32 privateFlags,
        /* [out] */ String* output);

    static CARAPI_(void) SetTagInternal(
        /* [in] */ IView* view,
        /* [in] */ Int32 key,
        /* [in] */ IObject* tag);

    CARAPI_(void) PostCheckForLongClick(
        /* [in] */ Int32 delayOffset);

    static CARAPI StateSetUnion(
        /* [in] */ ArrayOf<Int32>* stateSet1,
        /* [in] */ ArrayOf<Int32>* stateSet2,
        /* [out] */ ArrayOf<Int32>** newSet);

    CARAPI HandleInvalidate();

    CARAPI HandleInvalidateRect(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

protected:
    /**
     * The animation currently associated with this view.
     * @hide
     */
    AutoPtr<IAnimation> mCurrentAnimation;

    /**
     * Width as measured during measure pass.
     * {@hide}
     */
    Int32 mMeasuredWidth;

    /**
     * Height as measured during measure pass.
     * {@hide}
     */
    Int32 mMeasuredHeight;

    /**
     * The view's identifier.
     * {@hide}
     *
     * @see #setId(int)
     * @see #getId()
     */
    Int32 mID;

    /**
     * The view's tag.
     * {@hide}
     *
     * @see #setTag(Object)
     * @see #getTag()
     */
    AutoPtr<IInterface> mTag;

    /**
     * Controls the over-scroll mode for this view.
     * See {@link #overScrollBy(int, int, int, int, int, int, int, int, boolean)},
     * {@link #OVER_SCROLL_ALWAYS}, {@link #OVER_SCROLL_IF_CONTENT_SCROLLS},
     * and {@link #OVER_SCROLL_NEVER}.
     */
    Int32 mOverScrollMode;
    /**
     * The parent this view is attached to.
     * {@hide}
     *
     * @see #getParent()
     */
    AutoPtr<IViewParent> mParent;

    AttachInfo* mAttachInfo;
    Int32 mPrivateFlags;

    /**
     * Count of how many windows this view has been attached to.
     */
    Int32 mWindowAttachCount;

    /**
     * The layout parameters associated with this view and used by the parent
     * {@link android.view.ViewGroup} to determine how this view should be
     * laid out.
     * {@hide}
     */
    AutoPtr<IViewGroupLayoutParams> mLayoutParams;

    /**
     * The view flags hold various views states.
     * {@hide}
     */
    Int32 mViewFlags;

    /**
     * The distance in pixels from the left edge of this view's parent
     * to the left edge of this view.
     * {@hide}
     */
    Int32 mLeft;
    /**
     * The distance in pixels from the left edge of this view's parent
     * to the right edge of this view.
     * {@hide}
     */
    Int32 mRight;
    /**
     * The distance in pixels from the top edge of this view's parent
     * to the top edge of this view.
     * {@hide}
     */
    Int32 mTop;
    /**
     * The distance in pixels from the top edge of this view's parent
     * to the bottom edge of this view.
     * {@hide}
     */
    Int32 mBottom;

    /**
     * The offset, in pixels, by which the content of this view is scrolled
     * horizontally.
     * {@hide}
     */
    Int32 mScrollX;
    /**
     * The offset, in pixels, by which the content of this view is scrolled
     * vertically.
     * {@hide}
     */
    Int32 mScrollY;

    /**
     * The left padding in pixels, that is the distance in pixels between the
     * left edge of this view and the left edge of its content.
     * {@hide}
     */
    Int32 mPaddingLeft;
    /**
     * The right padding in pixels, that is the distance in pixels between the
     * right edge of this view and the right edge of its content.
     * {@hide}
     */
    Int32 mPaddingRight;
    /**
     * The top padding in pixels, that is the distance in pixels between the
     * top edge of this view and the top edge of its content.
     * {@hide}
     */
    Int32 mPaddingTop;
    /**
     * The bottom padding in pixels, that is the distance in pixels between the
     * bottom edge of this view and the bottom edge of its content.
     * {@hide}
     */
    Int32 mPaddingBottom;

    /**
     * Cache the paddingRight set by the user to append to the scrollbar's size.
     */
    Int32 mUserPaddingRight;

    /**
     * Cache the paddingBottom set by the user to append to the scrollbar's size.
     */
    Int32 mUserPaddingBottom;

    Int32 mOldWidthMeasureSpec;
    /**
     * @hide
     */
    Int32 mOldHeightMeasureSpec;

    /**
     * Briefly describes the view and is primarily used for accessibility support.
     */
    ArrayOf<Char8>* mContentDescription;

    AutoPtr<IResources> mResources;

    AutoPtr<IDrawable> mBGDrawable;

    Int32 mBackgroundResource;
    Boolean mBackgroundSizeChanged;

    /**
     * Listener used to dispatch focus change events.
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IViewOnFocusChangeListener> mOnFocusChangeListener;

    /**
     * Listener used to dispatch click events.
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IViewOnClickListener> mOnClickListener;

    /**
     * Listener used to dispatch long click events.
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IViewOnLongClickListener> mOnLongClickListener;

    /**
     * Listener used to build the context menu.
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IViewOnCreateContextMenuListener> mOnCreateContextMenuListener;

    AutoPtr<IViewOnKeyListener> mOnKeyListener;

    AutoPtr<IViewOnTouchListener> mOnTouchListener;

    /**
     * The application environment this view lives in.
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IContext> mContext;

    AutoPtr<ScrollabilityCache> mScrollCache;

    ArrayOf<Int32>* mDrawableState;

    AutoPtr<IBitmap> mDrawingCache;
    AutoPtr<IBitmap> mUnscaledDrawingCache;

    /**
     * When this view has focus and the next focus is {@link #FOCUS_LEFT},
     * the user may specify which view to go to next.
     */
    Int32 mNextFocusLeftId;// = View.NO_ID;

    /**
     * When this view has focus and the next focus is {@link #FOCUS_RIGHT},
     * the user may specify which view to go to next.
     */
    Int32 mNextFocusRightId;// = View.NO_ID;

    /**
     * When this view has focus and the next focus is {@link #FOCUS_UP},
     * the user may specify which view to go to next.
     */
    Int32 mNextFocusUpId;// = View.NO_ID;

    /**
     * When this view has focus and the next focus is {@link #FOCUS_DOWN},
     * the user may specify which view to go to next.
     */
    Int32 mNextFocusDownId;// = View.NO_ID;

    AutoPtr<CheckForLongPress> mPendingCheckForLongPress;
    AutoPtr<CheckForTap> mPendingCheckForTap;
    AutoPtr<ViewPerformClick> mPerformClick;

    AutoPtr<UnsetPressedState> mUnsetPressedState;

    /**
     * Whether the long press's action has been invoked.  The tap's action is invoked on the
     * up event while a long press is invoked as soon as the long press duration is reached, so
     * a long press could be performed before the tap is checked, in which case the tap's action
     * should not be invoked.
     */
    Boolean mHasPerformedLongPress;

    /**
     * The minimum height of the view. We'll try our best to have the height
     * of this view to at least this amount.
     */
    //@ViewDebug.ExportedProperty(category = "measurement")
    Int32 mMinHeight;

    /**
     * The minimum width of the view. We'll try our best to have the width
     * of this view to at least this amount.
     */
    //@ViewDebug.ExportedProperty(category = "measurement")
    Int32 mMinWidth;

    /**
     * The delegate to handle touch events that are physically in this view
     * but should be handled by another view.
     */
    AutoPtr<ITouchDelegate> mTouchDelegate;

    /**
     * Solid color to use as a background when creating the drawing cache. Enables
     * the cache to use 16 bit bitmaps instead of 32 bit.
     */
    Int32 mDrawingCacheBackgroundColor;

    /**
     * Special tree observer used when mAttachInfo is null.
     */
    AutoPtr<IViewTreeObserver> mFloatingTreeObserver;

    /**
     * Cache the touch slop from the context that created the view.
     */
    Int32 mTouchSlop;

    // Used for debug only
    static Int64 sInstanceCount;

private:
    AutoPtr<IApartment> mApartment;
};

#endif //__View_h__
