
#ifndef __CWINDOWMANAGERSERVICE_H__
#define __CWINDOWMANAGERSERVICE_H__

#include "ext/frameworkdef.h"
#include <elastos/ElRefBase.h>
#include "_CWindowManagerService.h"
#include "server/CServiceRecord.h"
#include "server/CWindowSession.h"
#include "server/InputManager.h"
#include "server/InputWindowList.h"
#include "server/InputApplication.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/CSurfaceSession.h"
#include "view/CInputChannel.h"
#include "view/View.h"
#include "view/WindowManagerPolicy.h"
#include "graphics/CRect.h"
#include "graphics/CPaint.h"
#include "utils/CDisplayMetrics.h"
#include "utils/AutoStringArray.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/HashSet.h>
#include <elastos/Mutex.h>

class InputManager;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IBinder> >
{
    size_t operator()(AutoPtr<IBinder> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

template<> struct Hash<AutoPtr<IWindowSession> >
{
    size_t operator()(AutoPtr<IWindowSession> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

CarClass(CWindowManagerService)
{
public:
    static const char* TAG;

    /** How much to multiply the policy's type layer, to reserve room
     * for multiple windows of the same type and Z-ordering adjustment
     * with TYPE_LAYER_OFFSET. */
    static const Int32 TYPE_LAYER_MULTIPLIER = 10000;

    /** Offset from TYPE_LAYER_MULTIPLIER for moving a group of windows above
     * or below others in the same layer. */
    static const Int32 TYPE_LAYER_OFFSET = 1000;

    /** How much to increment the layer for each window, to reserve room
     * for effect surfaces between them.
     */
    static const Int32 WINDOW_LAYER_MULTIPLIER = 5;

    /** The maximum length we will accept for a loaded animation duration:
     * this is 10 seconds.
     */
    static const Int32 MAX_ANIMATION_DURATION = 10*1000;

    /** Amount of time (in milliseconds) to animate the dim surface from one
     * value to another, when no window animation is driving it.
     */
    static const Int32 DEFAULT_DIM_DURATION = 200;

    /** Amount of time (in milliseconds) to animate the fade-in-out transition for
     * compatible windows.
     */
    static const Int32 DEFAULT_FADE_IN_OUT_DURATION = 400;

    /** Adjustment to time to perform a dim, to make it more dramatic.
     */
    static const Int32 DIM_DURATION_MULTIPLIER = 6;

    // Maximum number of milliseconds to wait for input event injection.
    // FIXME is this value reasonable?
    static const Int32 INJECTION_TIMEOUT_MILLIS = 30 * 1000;

    static const Int32 UPDATE_FOCUS_NORMAL = 0;
    static const Int32 UPDATE_FOCUS_WILL_ASSIGN_LAYERS = 1;
    static const Int32 UPDATE_FOCUS_PLACING_SURFACES = 2;
    static const Int32 UPDATE_FOCUS_WILL_PLACE_SURFACES = 3;

    static const Int32 ADJUST_WALLPAPER_LAYERS_CHANGED = 2;
    static const Int32 ADJUST_WALLPAPER_VISIBILITY_CHANGED = 4;

    // We give a wallpaper up to 150ms to finish scrolling.
    static const Millisecond64 WALLPAPER_TIMEOUT = 150;
    // Time we wait after a timeout before trying to wait again.
    static const Millisecond64 WALLPAPER_TIMEOUT_RECOVERY = 10000;

private:
    static const Int32 ALLOW_DISABLE_YES = 1;
    static const Int32 ALLOW_DISABLE_NO = 0;
    static const Int32 ALLOW_DISABLE_UNKNOWN = -1; // check with DevicePolicyManager

    // Default input dispatching timeout in nanoseconds.
    static const Int64 DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS = 5000 * 1000000LL;//??????too long?

public:
    interface WindowChangeListener
    {
        virtual CARAPI_(void) WindowsChanged() = 0;

        virtual CARAPI_(void) FocusChanged() = 0;
    };

private:
    class WindowToken;
    class AppWindowToken;
    class WindowState;

    // -------------------------------------------------------------
    // Input Events and Focus Management
    // -------------------------------------------------------------
    /* Tracks the progress of input dispatch and ensures that input dispatch state
     * is kept in sync with changes in window focus, visibility, registration, and
     * other relevant Window Manager state transitions. */
    class InputMonitor
    {
    public:
        InputMonitor(
            /* [in] */ CWindowManagerService* service);

        ~InputMonitor();

        /* Notifies the window manager about a broken input channel.
         *
         * Called by the InputManager.
         */
        CARAPI NotifyInputChannelBroken(
            /* [in] */ CInputChannel* inputChannel);

        /* Notifies the window manager about an application that is not responding.
         * Returns a new timeout to continue waiting in nanoseconds, or 0 to abort dispatch.
         *
         * Called by the InputManager.
         */
        CARAPI NotifyANR(
            /* [in] */ void* token,
            /* [in] */ CInputChannel* inputChannel,
            /* [out] */ Int64* timeout);

        /* Updates the cached window information provided to the input dispatcher. */
        CARAPI_(void) UpdateInputWindowsLw();

        /* Notifies that the lid switch changed state. */
        CARAPI NotifyLidSwitchChanged(
            /* [in] */ Millisecond64 whenNanos,
            /* [in] */ Boolean lidOpen);

        /* Provides an opportunity for the window manager policy to intercept early key
         * processing as soon as the key has been read from the device. */
        CARAPI InterceptKeyBeforeQueueing(
            /* [in] */ Millisecond64 whenNanos,
            /* [in] */ Int32 action,
            /* [in] */ Int32 flags,
            /* [in] */ Int32 keyCode,
            /* [in] */ Int32 scanCode,
            /* [in] */ Int32 policyFlags,
            /* [in] */ Boolean isScreenOn,
            /* [out] */ Int32* result);

        /* Provides an opportunity for the window manager policy to process a key before
         * ordinary dispatch. */
        CARAPI InterceptKeyBeforeDispatching(
            /* [in] */ CInputChannel* focus,
            /* [in] */ Int32 action,
            /* [in] */ Int32 flags,
            /* [in] */ Int32 keyCode,
            /* [in] */ Int32 scanCode,
            /* [in] */ Int32 metaState,
            /* [in] */ Int32 repeatCount,
            /* [in] */ Int32 policyFlags,
            /* [out] */ Boolean* result);

        /* Called when the current input focus changes.
         * Layer assignment is assumed to be complete by the time this is called.
         */
        CARAPI_(void) SetInputFocusLw(
            /* [in] */ WindowState* newWindow);

        CARAPI_(void) SetFocusedAppLw(
            /* [in] */ AppWindowToken* newApp);

        CARAPI_(void) PauseDispatchingLw(
            /* [in] */ WindowToken* window);

        CARAPI_(void) ResumeDispatchingLw(
            /* [in] */ WindowToken* window);

        CARAPI_(void) FreezeInputDispatchingLw();

        CARAPI_(void) ThawInputDispatchingLw();

        CARAPI_(void) SetEventDispatchingLw(
            /* [in] */ Boolean enabled);

    private:
        CARAPI_(AutoPtr<WindowState>) GetWindowStateForInputChannel(
            CInputChannel* inputChannel);

        CARAPI_(AutoPtr<WindowState>) GetWindowStateForInputChannelLocked(
            CInputChannel* inputChannel);

        CARAPI_(void) UpdateInputDispatchModeLw();

    private:
        AutoPtr<CWindowManagerService> mService;

        // Current window with input focus for keys and other non-touch events.  May be null.
        AutoPtr<WindowState> mInputFocus;

        // When true, prevents input dispatch from proceeding until set to false again.
        Boolean mInputDispatchFrozen;

        // When true, input dispatch proceeds normally.  Otherwise all events are dropped.
        Boolean mInputDispatchEnabled;

        // Temporary list of windows information to provide to the input dispatcher.
        InputWindowList* mTempInputWindows;

        // Temporary input application object to provide to the input dispatcher.
        InputApplication* mTempInputApplication;
    };
    friend class InputMonitor;

    class WindowToken
    {
    public:
        WindowToken(
            /* [in] */ IBinder* token,
            /* [in] */ Int32 type,
            /* [in] */ Boolean ex);

        CARAPI_(void) GetDescription(
            /* [out] */ String* description);

    public:
        // The actual token.
        AutoPtr<IBinder> mToken;

        // The type of window this token is for, as per WindowManager.LayoutParams.
        Int32 mWindowType;

        // Set if this token was explicitly added by a client, so should
        // not be removed when all windows are removed.
        Boolean mExplicit;

        // For printing.
        String mStringName;

        // If this is an AppWindowToken, this is non-null.
        AppWindowToken* mAppWindowToken;

        // All of the windows associated with this token.
        List<AutoPtr<WindowState> > mWindows;

        // Is key dispatching paused for this token?
        Boolean mPaused;

        // Should this token's windows be hidden?
        Boolean mHidden;

        // Temporary for finding which tokens no longer have visible windows.
        Boolean mHasVisible;

        // Set to true when this token is in a pending transaction where it
        // will be shown.
        Boolean mWaitingToShow;

        // Set to true when this token is in a pending transaction where it
        // will be hidden.
        Boolean mWaitingToHide;

        // Set to true when this token is in a pending transaction where its
        // windows will be put to the bottom of the list.
        Boolean mSendingToBottom;

        // Set to true when this token is in a pending transaction where its
        // windows will be put to the top of the list.
        Boolean mSendingToTop;
    };

    class StartingData;

    class AppWindowToken : public WindowToken
    {
    public:
        AppWindowToken(
            /* [in] */ CWindowManagerService* wmService,
            /* [in] */ IApplicationToken* token);

//        CARAPI_(void) SetAnimation(
//            /* [in] */ Animation anim);

        CARAPI_(void) SetDummyAnimation();

        CARAPI_(void) ClearAnimation();

        CARAPI_(void) UpdateLayers();

        CARAPI_(void) SendAppVisibilityToClients();

        CARAPI_(void) ShowAllWindowsLocked();

        // This must be called while inside a transaction.
        CARAPI_(Boolean) StepAnimationLocked(
            /* [in] */ Millisecond64 currentTime,
            /* [in] */ Int32 dw,
            /* [in] */ Int32 dh);

        CARAPI_(void) UpdateReportedVisibilityLocked();

        CARAPI_(AutoPtr<WindowState>) FindMainWindow();

        CARAPI_(void) GetDescription(
            /* [out] */ String* des);

    public:
        AutoPtr<CWindowManagerService> mWMService;
        AutoPtr<IApplicationToken> mAppToken;

        // All of the windows and child windows that are included in this
        // application token.  Note this list is NOT sorted!
        List<AutoPtr<WindowState> > mAllAppWindows;

        Int32 mGroupId;
        Boolean mAppFullscreen;
        Int32 mRequestedOrientation;

        // The input dispatching timeout for this application token in nanoseconds.
        Int64 mInputDispatchingTimeoutNanos;

        // These are used for determining when all windows associated with
        // an activity have been drawn, so they can be made visible together
        // at the same time.
        Int32 mLastTransactionSequence;
        Int32 mNumInterestingWindows;
        Int32 mNumDrawnWindows;
        Boolean mInPendingTransaction;
        Boolean mAllDrawn;

        // Is this token going to be hidden in a little while?  If so, it
        // won't be taken into account for setting the screen orientation.
        Boolean mWillBeHidden;

        // Is this window's surface needed?  This is almost like hidden, except
        // it will sometimes be true a little earlier: when the token has
        // been shown, but is still waiting for its app transition to execute
        // before making its windows shown.
        Boolean mHiddenRequested;

        // Have we told the window clients to hide themselves?
        Boolean mClientHidden;

        // Last visibility state we reported to the app token.
        Boolean mReportedVisible;

        // Set to true when the token has been removed from the window mgr.
        Boolean mRemoved;

        // Have we been asked to have this token keep the screen frozen?
        Boolean mFreezingScreen;

        Boolean mAnimating;
        //Animation* mAnimation;
        Boolean mHasTransformation;
        //const Transformation* transformation = new Transformation();

        // Offset to the window of all layers in the token, for use by
        // AppWindowToken animations.
        Int32 mAnimLayerAdjustment;

        // Information about an application starting window if displayed.
        StartingData* mStartingData;
        AutoPtr<WindowState> mStartingWindow;
        AutoPtr<IView> mStartingView;
        Boolean mStartingDisplayed;
        Boolean mStartingMoved;
        Boolean mFirstWindowDrawn;
        String mStringName;
    };

    class WindowState : public ElRefBase, public IWindowState
    {
    private:
//        class DeathRecipient // implements IBinder.DeathRecipient
//        {
//        };

    public:
        WindowState(
            /* [in] */ CWindowManagerService* wmService,
            /* [in] */ CWindowSession* session,
            /* [in] */ IInnerWindow* client,
            /* [in] */ WindowToken* token,
            /* [in] */ WindowState* attachedWindow,
            /* [in] */ CWindowManagerLayoutParams* attrs,
            /* [in] */ Int32 viewVisibility);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetFrameLw(
            /* [out] */ IRect** result);

        CARAPI GetShownFrameLw(
            /* [out] */ IRect** result);

        CARAPI GetDisplayFrameLw(
            /* [out] */ IRect** result);

        CARAPI GetContentFrameLw(
            /* [out] */ IRect** result);

        CARAPI GetVisibleFrameLw(
            /* [out] */ IRect** result);

        CARAPI GetGivenInsetsPendingLw(
            /* [out] */ Boolean* result);

        CARAPI GetGivenContentInsetsLw(
            /* [out] */ IRect** result);

        CARAPI GetGivenVisibleInsetsLw(
            /* [out] */ IRect** result);

        CARAPI GetAttrs(
            /* [out] */ IWindowManagerLayoutParams** params);

        CARAPI GetSurfaceLayer(
            /* [out] */ Int32* result);

        CARAPI GetAppToken(
            /* [out] */ IApplicationToken** result);

        CARAPI HasAppShownWindows(
            /* [out] */ Boolean* result);

        CARAPI IsVisibleLw(
            /* [out] */ Boolean* result);

        CARAPI IsVisibleOrBehindKeyguardLw(
            /* [out] */ Boolean* result);

        CARAPI IsDisplayedLw(
            /* [out] */ Boolean* result);

        CARAPI HasDrawnLw(
            /* [out] */ Boolean* result);

        CARAPI HideLw(
            /* [in] */ Boolean doAnimation,
            /* [out] */ Boolean* result);

        CARAPI ShowLw(
             /* [in] */ Boolean doAnimation,
             /* [out] */ Boolean* result);

        CARAPI_(void) Attach();

        CARAPI ComputeFrameLw(
            /* [in] */ IRect* pf,
            /* [in] */ IRect* df,
            /* [in] */ IRect* cf,
            /* [in] */ IRect* vf);

        CARAPI_(AutoPtr<IRect>) GetFrameLw();

        CARAPI_(AutoPtr<IRect>) GetShownFrameLw();

        CARAPI_(AutoPtr<IRect>) GetDisplayFrameLw();

        CARAPI_(AutoPtr<IRect>) GetContentFrameLw();

        CARAPI_(AutoPtr<IRect>) GetVisibleFrameLw();

        CARAPI_(Boolean) GetGivenInsetsPendingLw();

        CARAPI_(AutoPtr<IRect>) GetGivenContentInsetsLw();

        CARAPI_(AutoPtr<IRect>) GetGivenVisibleInsetsLw();

        CARAPI_(CWindowManagerLayoutParams*) GetAttrs();

        CARAPI_(Int32) GetSurfaceLayer();

        CARAPI_(IApplicationToken*) GetAppToken();

        CARAPI_(Int64) GetInputDispatchingTimeoutNanos();

        CARAPI_(Boolean) HasAppShownWindows();

//        CARAPI_(void) SetAnimation(
//            /* [out] */ Animation anim);

        CARAPI_(void) ClearAnimation();

        CARAPI CreateSurfaceLocked(
            /* [out] */ ISurface** surface);

        CARAPI_(void) DestroySurfaceLocked();

        CARAPI_(Boolean) FinishDrawingLocked();

        // This must be called while inside a transaction.
        CARAPI_(Boolean) CommitFinishDrawingLocked(
            /* [in] */ Millisecond64 currentTime);

        // This must be called while inside a transaction.
        CARAPI_(Boolean) PerformShowLocked();

        // This must be called while inside a transaction.  Returns true if
        // there is more animation to run.
        CARAPI_(Boolean) StepAnimationLocked(
            /* [in] */ Millisecond64 currentTime,
            /* [in] */ Int32 dw,
            /* [in] */ Int32 dh);

        CARAPI_(void) FinishExit();

        CARAPI_(Boolean) IsIdentityMatrix(
            /* [in] */ Float dsdx,
            /* [in] */ Float dtdx,
            /* [in] */ Float dsdy,
            /* [in] */ Float dtdy);

        CARAPI_(void) ComputeShownFrameLocked();

        /**
         * Is this window visible?  It is not visible if there is no
         * surface, or we are in the process of running an exit animation
         * that will remove the surface, or its app token has been hidden.
         */
        CARAPI_(Boolean) IsVisibleLw();

        CARAPI_(Boolean) IsVisibleOrBehindKeyguardLw();

        /**
         * Is this window visible, ignoring its app token?  It is not visible
         * if there is no surface, or we are in the process of running an exit animation
         * that will remove the surface.
         */
        CARAPI_(Boolean) IsWinVisibleLw();

        /**
         * The same as isVisible(), but follows the current hidden state of
         * the associated app token, not the pending requested hidden state.
         */
        CARAPI_(Boolean) IsVisibleNow();

        /**
         * Same as isVisible(), but we also count it as visible between the
         * call to IWindowSession.add() and the first relayout().
         */
        CARAPI_(Boolean) IsVisibleOrAdding();

        /**
         * Is this window currently on-screen?  It is on-screen either if it
         * is visible or it is currently running an animation before no longer
         * being visible.
         */
        CARAPI_(Boolean) IsOnScreen();

        /**
         * Like isOnScreen(), but we don't return true if the window is part
         * of a transition that has not yet been started.
         */
        CARAPI_(Boolean) IsReadyForDisplay();

        /** Is the window or its container currently animating? */
        CARAPI_(Boolean) IsAnimating();

        /** Is this window currently animating? */
        CARAPI_(Boolean) IsWindowAnimating();

        /**
         * Like isOnScreen, but returns false if the surface hasn't yet
         * been drawn.
         */
        CARAPI_(Boolean) IsDisplayedLw();

        /**
         * Returns true if the window has a surface that it has drawn a
         * complete UI in to.  Note that this returns true if the orientation
         * is changing even if the window hasn't redrawn because we don't want
         * to stop things from executing during that time.
         */
        CARAPI_(Boolean) IsDrawnLw();

        /**
         * Return true if the window is opaque and fully drawn.  This indicates
         * it may obscure windows behind it.
         */
        CARAPI_(Boolean) IsOpaqueDrawn();

        CARAPI_(Boolean) NeedsBackgroundFiller(
            /* [in] */ Int32 screenWidth,
            /* [in] */ Int32 screenHeight);

        CARAPI_(Boolean) IsFullscreen(
            /* [in] */ Int32 screenWidth,
            /* [in] */ Int32 screenHeight);

        CARAPI_(void) RemoveLocked();

        CARAPI_(void) DisposeInputChannel();

        /** Returns true if this window desires key events. */
        CARAPI_(Boolean) CanReceiveKeys();

        CARAPI_(Boolean) HasDrawnLw();

        CARAPI_(Boolean) ShowLw(
            /* [in] */ Boolean doAnimation);

        CARAPI_(Boolean) ShowLw(
            /* [in] */ Boolean doAnimation,
            /* [in] */ Boolean requestAnim);

        CARAPI_(Boolean) HideLw(
            /* [in] */ Boolean doAnimation);

        CARAPI_(Boolean) HideLw(
            /* [in] */ Boolean doAnimation,
            /* [in] */ Boolean requestAnim);

        CARAPI_(void) MakeInputChannelName(
            /* [out] */ String* name);

        CARAPI_(void) GetDescription(
            /* [out] */ String* description);

    public:
        AutoPtr<CWindowManagerService> mWMService;
        AutoPtr<CWindowSession> mSession;
        AutoPtr<IInnerWindow> mClient;
        WindowToken* mToken;
        WindowToken* mRootToken;
        AppWindowToken* mAppToken;
        AppWindowToken* mTargetAppToken;
        AutoPtr<CWindowManagerLayoutParams> mAttrs;
//        DeathRecipient mDeathRecipient;
        AutoPtr<WindowState> mAttachedWindow;
        List<AutoPtr<WindowState> > mChildWindows;
        Int32 mBaseLayer;
        Int32 mSubLayer;
        Boolean mLayoutAttached;
        Boolean mIsImWindow;
        Boolean mIsWallpaper;
        Boolean mIsFloatingLayer;
        Int32 mViewVisibility;
        Boolean mPolicyVisibility;
        Boolean mPolicyVisibilityAfterAnim;
        Boolean mAppFreezing;
        AutoPtr<ISurface> mSurface;
        Boolean mReportDestroySurface;
        Boolean mSurfacePendingDestroy;
        Boolean mAttachedHidden;    // is our parent window hidden?
        Boolean mLastHidden;        // was this window last hidden?
        Boolean mWallpaperVisible;  // for wallpaper, what was last vis report?
        Int32 mRequestedWidth;
        Int32 mRequestedHeight;
        Int32 mLastRequestedWidth;
        Int32 mLastRequestedHeight;
        Int32 mLayer;
        Int32 mAnimLayer;
        Int32 mLastLayer;
        Boolean mHaveFrame;
        Boolean mObscured;
        Boolean mTurnOnScreen;

        Int32 mLayoutSeq;

        AutoPtr<CConfiguration> mConfiguration;

        // Actual frame shown on-screen (may be modified by animation)
        AutoPtr<IRect> mShownFrame;
        AutoPtr<IRect> mLastShownFrame;

        /**
         * Set when we have changed the size of the surface, to know that
         * we must tell them application to resize (and thus redraw itself).
         */
        Boolean mSurfaceResized;

        /**
         * Insets that determine the actually visible area
         */
        AutoPtr<IRect> mVisibleInsets;
        AutoPtr<IRect> mLastVisibleInsets;
        Boolean mVisibleInsetsChanged;

        /**
         * Insets that are covered by system windows
         */
        AutoPtr<IRect> mContentInsets;
        AutoPtr<IRect> mLastContentInsets;
        Boolean mContentInsetsChanged;

        /**
         * Set to true if we are waiting for this window to receive its
         * given internal insets before laying out other windows based on it.
         */
        Boolean mGivenInsetsPending;

        /**
         * These are the content insets that were given during layout for
         * this window, to be applied to windows behind it.
         */
        AutoPtr<IRect> mGivenContentInsets;

        /**
         * These are the visible insets that were given during layout for
         * this window, to be applied to windows behind it.
         */
        AutoPtr<IRect> mGivenVisibleInsets;

        /**
         * Flag indicating whether the touchable region should be adjusted by
         * the visible insets; if false the area outside the visible insets is
         * NOT touchable, so we must use those to adjust the frame during hit
         * tests.
         */
        Int32 mTouchableInsets;// = ViewTreeObserver.InternalInsetsInfo.TOUCHABLE_INSETS_FRAME;

        // Current transformation being applied.
        Float mDsDx;
        Float mDtDx;
        Float mDsDy;
        Float mDtDy;
        Float mLastDsDx;
        Float mLastDtDx;
        Float mLastDsDy;
        Float mLastDtDy;
        Float mHScale;
        Float mVScale;
        Float mLastHScale;
        Float mLastVScale;
//        Matrix mTmpMatrix = new Matrix();

        // "Real" frame that the application sees.
        AutoPtr<IRect> mFrame;
        AutoPtr<IRect> mLastFrame;

        AutoPtr<IRect> mContainingFrame;
        AutoPtr<IRect> mDisplayFrame;
        AutoPtr<IRect> mContentFrame;
        AutoPtr<IRect> mVisibleFrame;

        Float mShownAlpha;
        Float mAlpha;
        Float mLastAlpha;

        // Set to true if, when the window gets displayed, it should perform
        // an enter animation.
        Boolean mEnterAnimationPending;

        // Currently running animation.
        Boolean mAnimating;
        Boolean mLocalAnimating;
//        AutoPtr<IAnimation> mAnimation;
        Boolean mAnimationIsEntrance;
        Boolean mHasTransformation;
        Boolean mHasLocalTransformation;
//        final Transformation mTransformation = new Transformation();

        // If a window showing a wallpaper: the requested offset for the
        // wallpaper; if a wallpaper window: the currently applied offset.
        Float mWallpaperX;
        Float mWallpaperY;

        // If a window showing a wallpaper: what fraction of the offset
        // range corresponds to a full virtual screen.
        Float mWallpaperXStep;
        Float mWallpaperYStep;

        // Wallpaper windows: pixels offset based on above variables.
        Int32 mXOffset;
        Int32 mYOffset;

        // This is set after IWindowSession.relayout() has been called at
        // least once for the window.  It allows us to detect the situation
        // where we don't yet have a surface, but should have one soon, so
        // we can give the window focus before waiting for the relayout.
        Boolean mRelayoutCalled;

//        AutoPtr<IAnimation> mAnimation;

        // This is set after the Surface has been created but before the
        // window has been drawn.  During this time the surface is hidden.
        Boolean mDrawPending;

        // This is set after the window has finished drawing for the first
        // time but before its surface is shown.  The surface will be
        // displayed when the next layout is run.
        Boolean mCommitDrawPending;

        // This is set during the time after the window's drawing has been
        // committed, and before its surface is actually shown.  It is used
        // to delay showing the surface until all windows in a token are ready
        // to be shown.
        Boolean mReadyToShow;

        // Set when the window has been shown in the screen the first time.
        Boolean mHasDrawn;

        // Currently running an exit animation?
        Boolean mExiting;

        // Currently on the mDestroySurface list?
        Boolean mDestroying;

        // Completely remove from window manager after exit animation?
        Boolean mRemoveOnExit;

        // Set when the orientation is changing and this window has not yet
        // been updated for the new orientation.
        Boolean mOrientationChanging;

        // Is this window now (or just being) removed?
        Boolean mRemoved;

        // For debugging, this is the last information given to the surface flinger.
        Boolean mSurfaceShown;
        Int32 mSurfaceX;
        Int32 mSurfaceY;
        Int32 mSurfaceW;
        Int32 mSurfaceH;
        Int32 mSurfaceLayer;
        Float mSurfaceAlpha;

        // Input channel
        AutoPtr<CInputChannel> mInputChannel;

        // Used to improve performance of toString()
        String mStringNameCache;
        ArrayOf<Char8>* mLastTitle;
        Boolean mWasPaused;
    };

//    class DummyAnimation /*extends Animation*/
//    {
//    public:
//        CARAPI_(Boolean) GetTransformation(
//        /* [in] */ Int64 currentTime /* [in]  Transformation outTransformation*/);
//    };

    class StartingData
    {
    public:
        StartingData(
            /* [in] */ const String& cap,
            /* [in] */ Int32 theme,
            /* [in] */ const ArrayOf<Char8> & nonLocalizedLabel,
            /* [in] */ Int32 labelRes,
            /* [in] */ Int32 icon);

        ~StartingData();

    public:
        String mCap;
        Int32 mTheme;
        ArrayOf<Char8>* mNonLocalizedLabel;
        Int32 mLabelRes;
        Int32 mIcon;
    };

    class Watermark
    {
    public:
        Watermark(
            /* [in] */ CWindowManagerService* service,
            /* [in] */ CSurfaceSession* session,
            /* [in] */ ArrayOf<String>* tokens);

        ~Watermark();

        CARAPI_(void) DrawIfNeeded();

    public:
        AutoPtr<CWindowManagerService> mWMService;
        AutoStringArray mTokens;
        String mText;
        AutoPtr<CPaint> mTextPaint;
        Int32 mTextWidth;
        Int32 mTextHeight;
        Int32 mTextAscent;
        Int32 mTextDescent;
        Int32 mDeltaX;
        Int32 mDeltaY;

        AutoPtr<ISurface> mSurface;
        Int32 mLastDW;
        Int32 mLastDH;
        Boolean mDrawNeeded;
    };

    /**
     * DimAnimator class that controls the dim animation. This holds the surface and
     * all state used for dim animation.
     */
    class DimAnimator
    {
    public:
        DimAnimator (
            /* [in] */ CSurfaceSession* session);

        /**
         * Show the dim surface.
         */
        CARAPI_(void) Show(
            /* [in] */ Int32 dw,
            /* [in] */ Int32 dh);

        /**
         * Set's the dim surface's layer and update dim parameters that will be used in
         * {@link updateSurface} after all windows are examined.
         */
        CARAPI_(void) UpdateParameters(
            /* [in] */ WindowState* w,
            /* [in] */ Millisecond64 currentTime);

        /**
         * Updating the surface's alpha. Returns true if the animation continues, or returns
         * false when the animation is finished and the dim surface is hidden.
         */
        CARAPI_(Boolean) UpdateSurface(
            /* [in] */ Boolean dimming,
            /* [in] */ Millisecond64 currentTime,
            /* [in] */ Boolean displayFrozen);

    public:
        AutoPtr<ISurface> mDimSurface;
        Boolean mDimShown;
        Float mDimCurrentAlpha;
        Float mDimTargetAlpha;
        Float mDimDeltaPerMs;
        Millisecond64 mLastDimAnimTime;
        Int32 mLastDimWidth;
        Int32 mLastDimHeight;
    };

//    /**
//     * Animation that fade in after 0.5 interpolate time, or fade out in reverse order.
//     * This is used for opening/closing transition for apps in compatible mode.
//     */
//    class FadeInOutAnimation /*extends Animation*/{
//    public:
//        FadeInOutAnimation(
//            /* [in] */ Boolean fadeIn);
//
//        CARAPI_(void) Initialize(
//            /* [in] */ Int32 width,
//            /* [in] */ Int32 height,
//            /* [in] */ Int32 parentWidth,
//            /* [in] */ Int32 parentHeight);
//
//        CARAPI_(Int32) GetZAdjustment();
//
//    protected:
//        CARAPI_(void) ApplyTransformation(
//            /* [in] */ Float interpolatedTime
//            /* [in]  Transformation t)*/);
//
//    public:
//        Int32 mWidth;
//        Boolean mFadeIn;
//    };

public:
//    CWindowManagerService(
//        /* [in] */ IContext* context,
//        /* [in] */ /*PowerManagerService pm,*/
//        /* [in] */ Boolean haveInputMethods);

    CWindowManagerService();

    static CARAPI_(Boolean) CanBeImeTarget(
        /* [in] */ WindowState* w);

    CARAPI_(List<AutoPtr<WindowState> >::Iterator) FindDesiredInputMethodWindowIndexLocked(
        /* [in] */ Boolean willMove);

    CARAPI_(void) AddInputMethodWindowToListLocked(
        /* [in] */ WindowState* win);

    CARAPI_(void) LogWindowList(
        /* [in] */ const String& prefix);

    CARAPI_(void) SetInputMethodAnimLayerAdjustment(
        /* [in] */ Int32 adj);

    CARAPI_(void) MoveInputMethodDialogsLocked(
        /* [in] */ List<AutoPtr<WindowState> >::Iterator posIt);

    CARAPI_(Boolean) MoveInputMethodWindowsIfNeededLocked(
        /* [in] */ Boolean needAssignLayers);

    CARAPI_(void) AdjustInputMethodDialogsLocked();

    CARAPI_(Boolean) IsWallpaperVisible(
        /* [in] */ WindowState* wallpaperTarget);

    CARAPI_(Int32) AdjustWallpaperWindowsLocked();

    CARAPI_(void) SetWallpaperAnimLayerAdjustmentLocked(
        /* [in] */ Int32 adj);

    CARAPI_(Boolean) UpdateWallpaperOffsetLocked(
        /* [in] */ WindowState* wallpaperWin,
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh,
        /* [in] */ Boolean sync);

    CARAPI_(void) WallpaperOffsetsComplete(
        /* [in] */ IBinder* window);

    CARAPI_(Boolean) UpdateWallpaperOffsetLocked(
        /* [in] */ WindowState* changingTarget,
        /* [in] */ Boolean sync);

    CARAPI_(void) UpdateWallpaperVisibilityLocked();

    CARAPI_(Int32) AddWindow(
        /* [in] */ CWindowSession* session,
        /* [in] */ IInnerWindow* client,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Int32 viewVisibility,
        /* [in] */ IRect* inContentInsets,
        /* [in] */ IInputChannel* inInputChannel,
        /* [out] */ IRect** outContentInsets,
        /* [out] */ IInputChannel** outInputChannel);

    CARAPI_(void) RemoveWindow(
        /* [in] */ CWindowSession* session,
        /* [in] */ IInnerWindow* client);

    CARAPI_(void) RemoveWindowLocked(
        /* [in] */ CWindowSession* session,
        /* [in] */ WindowState* win);

    CARAPI_(void) SetInsetsWindow(
        /* [in] */ CWindowSession* session,
        /* [in] */ IInnerWindow* client,
        /* [in] */ Int32 touchableInsets,
        /* [in] */ IRect* contentInsets,
        /* [in] */ IRect* visibleInsets);

    CARAPI_(void) GetWindowDisplayFrame(
        /* [in] */ CWindowSession* session,
        /* [in] */ IInnerWindow* client,
        /* [in] */ IRect* outDisplayFrame);

    CARAPI_(void) SetWindowWallpaperPositionLocked(
        /* [in] */ WindowState* window,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float xStep,
        /* [in] */ Float yStep);

    CARAPI_(void) WallpaperCommandComplete(
        /* [in] */ IBinder* window,
        /* [in] */ IBundle* result);

    CARAPI_(AutoPtr<IBundle>) SendWindowWallpaperCommandLocked(
        /* [in] */ WindowState* window,
        /* [in] */ const String& action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean sync);

    CARAPI_(Int32) RelayoutWindow(
        /* [in] */ CWindowSession* session,
        /* [in] */ IInnerWindow* client,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Int32 requestedWidth,
        /* [in] */ Int32 requestedHeight,
        /* [in] */ Int32 viewVisibility,
        /* [in] */ Boolean insetsPending,
        /* [in] */ IRect* inFrame,
        /* [in] */ IRect* inContentInsets,
        /* [in] */ IRect* inVisibleInsets,
        /* [in] */ IConfiguration* inConfig,
        /* [in] */ ISurface* inSurface,
        /* [out] */ IRect** outFrame,
        /* [out] */ IRect** outContentInsets,
        /* [out] */ IRect** outVisibleInsets,
        /* [out] */ IConfiguration** outConfig,
        /* [out] */ ISurface** outSurface);

    CARAPI_(void) FinishDrawingWindow(
        /* [in] */ CWindowSession* session,
        /* [in] */ IInnerWindow* client);

    CARAPI_(void) ValidateAppTokens(
        /* [in] */ const List<AutoPtr<CActivityRecord> > & tokens);

    CARAPI_(Boolean) CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& func);

    CARAPI_(AppWindowToken*) FindAppWindowToken(
        /* [in] */ IBinder* token);

    CARAPI AddWindowToken(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 type);

    CARAPI RemoveWindowToken(
        /* [in] */ IBinder* token);

    CARAPI AddAppToken(
        /* [in] */ Int32 addPos,
        /* [in] */ IApplicationToken* token,
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 requestedOrientation,
        /* [in] */ Boolean fullscreen);

    CARAPI SetAppGroupId(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 groupId);

    CARAPI_(Int32) GetOrientationFromWindowsLocked();

    CARAPI_(Int32) GetOrientationFromAppTokensLocked();

    // Re-evaluate the current orientation from the caller's state.
    // If there is a change, the new Configuration is returned and the
    // caller must call setNewConfiguration() sometime later.
    CARAPI UpdateOrientationFromAppTokens(
        /* [in] */ IConfiguration* currentConfig,
        /* [in] */ IBinder* freezeThisOneIfNeeded,
        /* [out] */ IConfiguration** config);

    /*
     * Determine the new desired orientation of the display, returning
     * a non-null new Configuration if it has changed from the current
     * orientation.  IF TRUE IS RETURNED SOMEONE MUST CALL
     * setNewConfiguration() TO TELL THE WINDOW MANAGER IT CAN UNFREEZE THE
     * SCREEN.  This will typically be done for you if you call
     * sendNewConfiguration().
     *
     * The orientation is computed from non-application windows first. If none of
     * the non-application windows specify orientation, the orientation is computed from
     * application tokens.
     * @see android.view.IWindowManager#updateOrientationFromAppTokens(
     * android.os.IBinder)
     */
    CARAPI_(Boolean) UpdateOrientationFromAppTokensLocked();

    CARAPI_(Int32) ComputeForcedAppOrientationLocked();

    CARAPI SetNewConfiguration(
        /* [in] */ IConfiguration* config);

    CARAPI SetAppOrientation(
        /* [in] */ IApplicationToken* token,
        /* [in] */ Int32 requestedOrientation);

    CARAPI GetAppOrientation(
        /* [in] */ IApplicationToken* token,
        /* [out] */ Int32* orientation);

    CARAPI SetFocusedApp(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean moveFocusNow);

    CARAPI PrepareAppTransition(
        /* [in] */ Int32 transit);

    CARAPI GetPendingAppTransition(
        /* [out] */ Int32* transit);

    CARAPI OverridePendingAppTransition(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 enterAnim,
        /* [in] */ Int32 exitAnim);

    CARAPI ExecuteAppTransition();

    CARAPI SetAppStartingWindow(
        /* [in] */ IBinder* token,
        /* [in] */ const String& cap,
        /* [in] */ Int32 theme,
        /* [in] */ const ArrayOf<Char8> & nonLocalizedLabel,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 icon,
        /* [in] */ IBinder* transferFrom,
        /* [in] */ Boolean createIfNeeded);

    CARAPI SetAppWillBeHidden(
        /* [in] */ IBinder* token);

    CARAPI_(Boolean) SetTokenVisibilityLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ CWindowManagerLayoutParams* lp,
        /* [in] */ Boolean visible,
        /* [in] */ Int32 transit,
        /* [in] */ Boolean performLayout);

    CARAPI SetAppVisibility(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean visible);

    CARAPI_(void) UnsetAppFreezingScreenLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ Boolean unfreezeSurfaceNow,
        /* [in] */ Boolean force);

    CARAPI_(void) StartAppFreezingScreenLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ Int32 configChanges);

    CARAPI StartAppFreezingScreen(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 configChanges);

    CARAPI StopAppFreezingScreen(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean force);

    CARAPI RemoveAppToken(
        /* [in] */ IBinder* token);

    CARAPI_(void) DumpAppTokensLocked();

    CARAPI_(void) DumpWindowsLocked();

    CARAPI MoveAppToken(
        /* [in] */ Int32 index,
        /* [in] */ IBinder* token);

    CARAPI MoveAppTokensToTop(
        /* [in] */ IObjectContainer* tokens);

    CARAPI MoveAppTokensToBottom(
        /* [in] */ IObjectContainer* tokens);

    // these require DISABLE_KEYGUARD permission
    CARAPI DisableKeyguard(
        /* [in] */ IBinder* token,
        /* [in] */ const String& tag);

    CARAPI ReenableKeyguard(
        /* [in] */ IBinder* token);

    CARAPI ExitKeyguardSecurely(
        /* [in] */ IOnKeyguardExitResult* keyguardCallback);

    CARAPI InKeyguardRestrictedInputMode(
        /* [out] */ Boolean* result);

    CARAPI CloseSystemDialogs(
        /* [in] */ const String& reason);

    static CARAPI_(Float) FixScale(
        /* [in] */ Float scale);

    CARAPI SetAnimationScale(
        /* [in] */ Int32 which,
        /* [in] */ Float scale);

//    SetAnimationScales(
//        [in] float[] scales);

    // These can only be called with the SET_ANIMATON_SCALE permission.
    CARAPI GetAnimationScale(
        /* [in] */ Int32 which,
        /* [out] */ Float* scale);

//    GetAnimationScales(
//        [out] float[] scales);

    // These require the READ_INPUT_STATE permission.
    CARAPI GetSwitchState(
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetSwitchStateForDevice(
        /* [in] */ Int32 devid,
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetScancodeState(
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetScancodeStateForDevice(
        /* [in] */ Int32 devid,
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetTrackballScancodeState(
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetDPadScancodeState(
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetKeycodeState(
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetKeycodeStateForDevice(
        /* [in] */ Int32 devid,
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetTrackballKeycodeState(
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    CARAPI GetDPadKeycodeState(
        /* [in] */ Int32 sw,
        /* [out] */ Int32* state);

    // Report whether the hardware supports the given keys; returns true if successful
//    HasKeys(
//        [in] int[] keycodes,
//        [inout] Boolean[] keyExists,
//        [out] Boolean* result);

    CARAPI MonitorInput(
        /* [in] */ const String& inputChannelName,
        /* [out] */ IInputChannel** channel);

    // Get input device information.
    CARAPI GetInputDevice(
        /* [in] */ Int32 deviceId,
        /* [out] */ IInputDevice** device);

//    GetInputDeviceIds(
//        [out] int[] ids);

    CARAPI_(void) EnableScreenAfterBoot();

    CARAPI_(void) EnableScreenIfNeededLocked();

    CARAPI_(void) PerformEnableScreen();

    //for testing
    CARAPI SetInTouchMode(
        /* [in] */ Boolean showFocus);

    // These can only be called with the SET_ORIENTATION permission.
    /**
     * Change the current screen rotation, constants as per
     * {@link android.view.Surface}.
     * @param rotation the intended rotation.
     * @param alwaysSendConfiguration Flag to force a new configuration to
     * be evaluated.  This can be used when there are other parameters in
     * configuration that are changing.
     * @param animFlags Animation flags as per {@link android.view.Surface}.
     */
    CARAPI SetRotation(
        /* [in] */ Int32 rotation,
        /* [in] */ Boolean alwaysSendConfiguration,
        /* [in] */ Int32 animFlags);

    CARAPI_(void) SetRotationUnchecked(
        /* [in] */ Int32 rotation,
        /* [in] */ Boolean alwaysSendConfiguration,
        /* [in] */ Int32 animFlags);

    /**
     * Apply a new rotation to the screen, respecting the requests of
     * applications.  Use WindowManagerPolicy.USE_LAST_ROTATION to simply
     * re-evaluate the desired rotation.
     *
     * Returns null if the rotation has been changed.  In this case YOU
     * MUST CALL setNewConfiguration() TO UNFREEZE THE SCREEN.
     */
    CARAPI_(Boolean) SetRotationUncheckedLocked(
        /* [in] */ Int32 rotation,
        /* [in] */ Int32 animFlags);

    /**
     * Retrieve the current screen orientation, constants as per
     * {@link android.view.Surface}.
     */
    CARAPI GetRotation(
        /* [out] */ Int32* rotation);

    /**
     * Watch the rotation of the screen.  Returns the current rotation,
     * calls back when it changes.
     */
    CARAPI WatchRotation(
        /* [in] */ IRotationWatcher* watcher,
        /* [out] */ Int32* rotation);

    /**
     * Starts the view server on the specified port.
     *
     * @param port The port to listener to.
     *
     * @return True if the server was successfully started, false otherwise.
     *
     * @see com.android.server.ViewServer
     * @see com.android.server.ViewServer#VIEW_SERVER_DEFAULT_PORT
     */
    CARAPI StartViewServer(
        /* [in] */ Int32 port,
        /* [out] */ Boolean* result);

    /**
     * Stops the view server if it exists.
     *
     * @return True if the server stopped, false if it wasn't started or
     *         couldn't be stopped.
     *
     * @see com.android.server.ViewServer
     */
    CARAPI StopViewServer(
        /* [out] */ Boolean* result);

    /**
     * Indicates whether the view server is running.
     *
     * @return True if the server is running, false otherwise.
     *
     * @see com.android.server.ViewServer
     */
    CARAPI IsViewServerRunning(
        /* [out] */ Boolean* result);

    /**
     * Lists all availble windows in the system. The listing is written in the
     * specified Socket's output stream with the following syntax:
     * windowHashCodeInHexadecimal windowName
     * Each line of the ouput represents a different window.
     *
     * @param client The remote client to send the listing to.
     * @return False if an error occured, true otherwise.
     */
//    CARAPI_(Boolean) ViewServerListWindows(
//        /* [in] */ Socket client);

    /**
     * Returns the focused window in the following format:
     * windowHashCodeInHexadecimal windowName
     *
     * @param client The remote client to send the listing to.
     * @return False if an error occurred, true otherwise.
     */
//    CARAPI_(Boolean) ViewServerGetFocusedWindow(
//        /* [in] */ Socket client);

     /**
     * Sends a command to a target window. The result of the command, if any, will be
     * written in the output stream of the specified socket.
     *
     * The parameters must follow this syntax:
     * windowHashcode extra
     *
     * Where XX is the length in characeters of the windowTitle.
     *
     * The first parameter is the target window. The window with the specified hashcode
     * will be the target. If no target can be found, nothing happens. The extra parameters
     * will be delivered to the target window and as parameters to the command itself.
     *
     * @param client The remote client to sent the result, if any, to.
     * @param command The command to execute.
     * @param parameters The command parameters.
     *
     * @return True if the command was successfully delivered, false otherwise. This does
     *         not indicate whether the command itself was successful.
     */
//    CARAPI_(Boolean) ViewServerWindowCommand(
//        /* [in] */ Socket client,
//        /* [in] */ const String& command,
//        /* [in] */ const String& parameters);

    CARAPI_(void) AddWindowChangeListener(
        /* [in] */ WindowChangeListener* listener);

    CARAPI_(void) RemoveWindowChangeListener(
        /* [in] */ WindowChangeListener* listener);

    /*
     * Instruct the Activity Manager to fetch the current configuration and broadcast
     * that to config-changed listeners if appropriate.
     */
    CARAPI SendNewConfiguration();

    CARAPI_(AutoPtr<CConfiguration>) ComputeNewConfiguration();

    CARAPI_(AutoPtr<CConfiguration>) ComputeNewConfigurationLocked();

    CARAPI_(Boolean) ComputeNewConfigurationLocked(
        /* [in] */ CConfiguration* config);

    // These can only be called when holding the MANAGE_APP_TOKENS permission.
    CARAPI PauseKeyDispatching(
        /* [in] */ IBinder* token);

    CARAPI ResumeKeyDispatching(
        /* [in] */ IBinder* token);

    CARAPI SetEventDispatching(
        /* [in] */ Boolean enabled);

    CARAPI_(Boolean) DetectSafeMode();

    CARAPI_(void) SystemReady();

    // -------------------------------------------------------------
    // IWindowManager API
    // -------------------------------------------------------------

    CARAPI OpenSession(
        /* [in] */ IInputMethodClient* client,
        /* [in] */ IInputContext* inputContext,
        /* [out] */ IWindowSession** session);

    CARAPI InputMethodClientHasFocus(
        /* [in] */ IInputMethodClient* client,
        /* [out] */ Boolean* result);

    // -------------------------------------------------------------
    // Internals
    // -------------------------------------------------------------

    CARAPI_(AutoPtr<WindowState>) WindowForClientLocked(
        /* [in] */ CWindowSession* session,
        /* [in] */ IInnerWindow* client,
        /* [in] */ Boolean throwOnError);

    CARAPI_(AutoPtr<WindowState>) WindowForClientLocked(
        /* [in] */ CWindowSession* session,
        /* [in] */ IBinder* client,
        /* [in] */ Boolean throwOnError);

    CARAPI_(void) RebuildAppWindowListLocked();

    /**
     * Must be called with the main window manager lock held.
     */
    CARAPI_(void) SetHoldScreenLocked(
        /* [in] */ Boolean holding);

    CARAPI RequestAnimationLocked(
        /* [in] */ Millisecond64 delay);

    /**
     * Have the surface flinger show a surface, robustly dealing with
     * error conditions.  In particular, if there is not enough memory
     * to show the surface, then we will try to get rid of other surfaces
     * in order to succeed.
     *
     * @return Returns true if the surface was successfully shown.
     */
    CARAPI_(Boolean) ShowSurfaceRobustlyLocked(
        /* [in] */ WindowState* win);

    CARAPI_(void) ReclaimSomeSurfaceMemoryLocked(
        /* [in] */ WindowState* win,
        /* [in] */ const char* operation);

    // These can only be called when injecting events to your own window,
    // or by holding the INJECT_EVENTS permission.  These methods may block
    // until pending input events are finished being dispatched even when 'sync' is false.
    // Avoid calling these methods on your UI thread or use the 'NoWait' version instead.
    /**
     * Injects a keystroke event into the UI.
     * Even when sync is false, this method may block while waiting for current
     * input events to be dispatched.
     *
     * @param ev A motion event describing the keystroke action.  (Be sure to use
     * {@link SystemClock#uptimeMillis()} as the timebase.)
     * @param sync If true, wait for the event to be completed before returning to the caller.
     * @return Returns true if event was dispatched, false if it was dropped for any reason
     */
    CARAPI InjectKeyEvent(
        /* [in] */ IKeyEvent* ev,
        /* [in] */ Boolean sync,
        /* [out] */ Boolean* result);

    /**
     * Inject a pointer (touch) event into the UI.
     * Even when sync is false, this method may block while waiting for current
     * input events to be dispatched.
     *
     * @param ev A motion event describing the pointer (touch) action.  (As noted in
     * {@link MotionEvent#obtain(long, long, int, float, float, int)}, be sure to use
     * {@link SystemClock#uptimeMillis()} as the timebase.)
     * @param sync If true, wait for the event to be completed before returning to the caller.
     * @return Returns true if event was dispatched, false if it was dropped for any reason
     */
    CARAPI InjectPointerEvent(
        /* [in] */ IMotionEvent* ev,
        /* [in] */ Boolean sync,
        /* [out] */ Boolean* result);

    /**
     * Inject a trackball (navigation device) event into the UI.
     * Even when sync is false, this method may block while waiting for current
     * input events to be dispatched.
     *
     * @param ev A motion event describing the trackball action.  (As noted in
     * {@link MotionEvent#obtain(long, long, int, float, float, int)}, be sure to use
     * {@link SystemClock#uptimeMillis()} as the timebase.)
     * @param sync If true, wait for the event to be completed before returning to the caller.
     * @return Returns true if event was dispatched, false if it was dropped for any reason
     */
    CARAPI InjectTrackballEvent(
        /* [in] */ IMotionEvent* ev,
        /* [in] */ Boolean sync,
        /* [out] */ Boolean* result);

    /**
     * Inject an input event into the UI without waiting for dispatch to commence.
     * This variant is useful for fire-and-forget input event injection.  It does not
     * block any longer than it takes to enqueue the input event.
     *
     * @param ev An input event.  (Be sure to set the input source correctly.)
     * @return Returns true if event was dispatched, false if it was dropped for any reason
     */
    CARAPI InjectInputEventNoWait(
        /* [in] */ IInputEvent* ev,
        /* [out] */ Boolean* result);

private:
    CARAPI_(void) PlaceWindowAfter(
        /* [in] */ WindowState* pos,
        /* [in] */ WindowState* window);

    CARAPI_(void) PlaceWindowBefore(
        /* [in] */ WindowState* pos,
        /* [in] */ WindowState* window);

    //This method finds out the index of a window that has the same app token as
    //win. used for z ordering the windows in mWindows
    CARAPI_(List<AutoPtr<WindowState> >::Iterator) FindIteratorBasedOnAppTokens(
        /* [in] */ WindowState* win);

    CARAPI_(void) AddWindowToListInOrderLocked(
        /* [in] */ WindowState* win,
        /* [in] */ Boolean addToToken);

    CARAPI_(List<AutoPtr<WindowState> >::Iterator) TmpRemoveWindowLocked(
        /* [in] */ List<AutoPtr<WindowState> >::Iterator interestingPosIt,
        /* [in] */ WindowState* win);

    CARAPI_(void) ReAddWindowToListInOrderLocked(
        /* [in] */ WindowState* win);

    CARAPI_(void) RemoveWindowInnerLocked(
        /* [in] */ CWindowSession* session,
        /* [in] */ WindowState* win);

//    CARAPI_(void) LogSurface(
//        /* [in] */ WindowState* w,
//        /* [in] */ const String& msg,
//        /* [in] */ RuntimeException where);

    CARAPI_(void) SetTransparentRegionWindow(
        /* [in] */ CWindowSession* session,
        /* [in] */ IInnerWindow* client,
        /* [in] */ IRegion* region);

    CARAPI_(void) ApplyEnterAnimationLocked(
        /* [in] */ WindowState* win);

    CARAPI_(Boolean) ApplyAnimationLocked(
        /* [in] */ WindowState* win,
        /* [in] */ Int32 transit,
        /* [in] */ Boolean isEntrance);

//    AttributeCache.Entry getCachedAnimations(WindowManager.LayoutParams lp);

//    AttributeCache.Entry getCachedAnimations(String packageName, int resId);

    CARAPI_(Boolean) ApplyAnimationLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ CWindowManagerLayoutParams* lp,
        /* [in] */ Int32 transit,
        /* [in] */ Boolean enter);

//    CARAPI_(AutoPtr<IAnimation>) LoadAnimation(
//        /* [in] */ CWindowManagerLayoutParams* lp,
//        /* [in] */ Int32 animAttr);

//    CARAPI_(AutoPtr<IAnimation>) LoadAnimation(
//        /* [in] */ const String& packageName,
//        /* [in] */ Int32 resId);

    CARAPI_(Boolean) TmpRemoveAppWindowsLocked(
        /* [in] */ WindowToken* token);

    CARAPI_(List<AutoPtr<WindowState> >::Iterator) FindWindowOffsetLocked(
        /* [in] */ List<AppWindowToken*>::Iterator tokenPosIt);

    CARAPI_(List<AutoPtr<WindowState> >::Iterator) ReAddWindowLocked(
        /* [in] */ List<AutoPtr<WindowState> >::Iterator it,
        /* [in] */ WindowState* win);

    CARAPI_(List<AutoPtr<WindowState> >::Iterator) ReAddAppWindowsLocked(
        /* [in] */ List<AutoPtr<WindowState> >::Iterator it,
        /* [in] */ WindowToken* token);

    CARAPI_(void) RemoveAppTokensLocked(
        /* [in] */ IObjectContainer* tokens);

    CARAPI_(void) MoveAppWindowsLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ List<AppWindowToken*>::Iterator tokenPosIt,
        /* [in] */ Boolean updateFocusAndLayout);

    CARAPI_(void) MoveAppWindowsLocked(
        /* [in] */ IObjectContainer* tokens,
        /* [in] */ List<AppWindowToken*>::Iterator tokenPosIt);

    // -------------------------------------------------------------
    // Misc IWindowSession methods
    // -------------------------------------------------------------

    CARAPI_(Boolean) ShouldAllowDisableKeyguard();

    CARAPI_(Boolean) IsSystemSecure();

    CARAPI_(void) NotifyWindowsChanged();

    CARAPI_(void) NotifyFocusChanged();

    CARAPI_(AutoPtr<WindowState>) FindWindow(
        /* [in] */ Int32 hashCode);

    CARAPI ReportInjectionResult(
        /* [in] */ Int32 result,
        /* [out] */ Boolean* succeeded);

    CARAPI_(AutoPtr<WindowState>) GetFocusedWindow();

    CARAPI_(AutoPtr<WindowState>) GetFocusedWindowLocked();

    CARAPI_(void) AssignLayersLocked();

    CARAPI PerformLayoutAndPlaceSurfacesLocked();

    CARAPI_(Int32) PerformLayoutLockedInner();

    CARAPI_(void) PerformLayoutAndPlaceSurfacesLockedInner(
        /* [in] */ Boolean recoveringMemory);

    CARAPI_(Boolean) UpdateFocusedWindowLocked(
        /* [in] */ Int32 mode);

    CARAPI_(void) FinishUpdateFocusedWindowAfterAssignLayersLocked();

    CARAPI_(AutoPtr<WindowState>) ComputeFocusedWindowLocked();

    CARAPI_(void) StartFreezingDisplayLocked();

    CARAPI_(void) StopFreezingDisplayLocked();

    static CARAPI_(Int32) GetPropertyInt(
        /* [in] */ ArrayOf<String>* tokens,
        /* [in] */ Int32 index,
        /* [in] */ Int32 defUnits,
        /* [in] */ Int32 defDps,
        /* [in] */ CDisplayMetrics* dm);

    CARAPI SendMessage(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    CARAPI SendMessageDelayed(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 delayMillis);

    CARAPI RemoveMessage(
        /* [in] */ Handle32 func);

    CARAPI HandleReportFocusChange();

    CARAPI HandleReportLosingFocus();

    CARAPI HandleAnimate();

    CARAPI HandleStarting(
        /* [in] */ AppWindowToken* wtoken);

    CARAPI HandleRemoveStarting(
        /* [in] */ AppWindowToken* wtoken);

    CARAPI HandleFinishedStarting();

    CARAPI HandleReportAppTokenWindows(
        /* [in] */ Boolean nowVisible,
        /* [in] */ Boolean nowGone,
        /* [in] */ AppWindowToken* wtoken);

    CARAPI HandleWindowFreezeTimeout();

    CARAPI HandleHoldScreenChanged(
        /* [in] */ IWindowSession* newHold);

    CARAPI HandleAppTransitionTimeout();

    CARAPI HandlePersistAnimationScale();

    CARAPI HandleForceGc();

    CARAPI HandleEnableScreen();

    CARAPI HandleAppFreezeTimeout();

    CARAPI HandleSendNewConfiguration();

    CARAPI HandleReportWindowsChange();

public:
    CARAPI_(void) CreateWatermark();

private:
    friend class CWindowSession;
    friend class InputManagerCallbacks;

    AutoPtr<IApartment> mApartment;


    /**
     * Condition waited on by {@link #reenableKeyguard} to know the call to
     * the window policy has finished.
     * This is set to true only if mKeyguardTokenWatcher.acquired() has
     * actually disabled the keyguard.
     */
    Boolean mKeyguardDisabled;
    Int32 mAllowDisableKeyguard; // sync'd by mKeyguardTokenWatcher
//    TokenWatcher* mKeyguardTokenWatcher;
    Mutex mKeyguardTokenWatcherLock;

//    BroadcastReceiver mBroadcastReceiver;
    AutoPtr<IContext> mContext;
    Boolean mHaveInputMethods;
    Boolean mLimitedAlphaCompositing;
    AutoPtr<IWindowManagerPolicy> mPolicy;

    AutoPtr<IActivityManager> mActivityManager;

    AutoPtr<IBatteryStats> mBatteryStats;

    /**
     * All currently active sessions with clients.
     */
    HashSet<AutoPtr<IWindowSession> > mSessions;

    /**
     * Mapping from an IWindow IBinder to the server's Window object.
     * This is also used as the lock for all of our state.
     */
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> > mWindowMap;
    Mutex mWindowMapLock;

    /**
     * Mapping from a token IBinder to a WindowToken object.
     */
    HashMap<AutoPtr<IBinder>, WindowToken*> mTokenMap;

    /**
     * The same tokens as mTokenMap, stored in a list for efficient iteration
     * over them.
     */
    List<WindowToken*> mTokenList;

    /**
     * Window tokens that are in the process of exiting, but still
     * on screen for animations.
     */
    List<WindowToken*> mExitingTokens;

    /**
     * Z-ordered (bottom-most first) list of all application tokens, for
     * controlling the ordering of windows in different applications.  This
     * contains WindowToken objects.
     */
    List<AppWindowToken*> mAppTokens;

    /**
     * Application tokens that are in the process of exiting, but still
     * on screen for animations.
     */
    List<AppWindowToken*> mExitingAppTokens;

     /**
     * List of window tokens that have finished starting their application,
     * and now need to have the policy remove their windows.
     */
    List<AppWindowToken*> mFinishedStarting;

    /**
     * This was the app token that was used to retrieve the last enter
     * animation.  It will be used for the next exit animation.
     */
    AppWindowToken* mLastEnterAnimToken;

     /**
     * These were the layout params used to retrieve the last enter animation.
     * They will be used for the next exit animation.
     */
    AutoPtr<CWindowManagerLayoutParams> mLastEnterAnimParams;

    /**
     * Z-ordered (bottom-most first) list of all Window objects.
     */
    List<AutoPtr<WindowState> > mWindows;

    /**
     * Windows that are being resized.  Used so we can tell the client about
     * the resize after closing the transaction in which we resized the
     * underlying surface.
     */
    List<AutoPtr<WindowState> > mResizingWindows;

    /**
     * Windows whose animations have ended and now must be removed.
     */
    List<AutoPtr<WindowState> > mPendingRemove;

    /**
     * Windows whose surface should be destroyed.
     */
    List<AutoPtr<WindowState> > mDestroySurface;

    /**
     * Windows that have lost input focus and are waiting for the new
     * focus window to be displayed before they are told about this.
     */
    List<AutoPtr<WindowState> > mLosingFocus;

     /**
     * This is set when we have run out of memory, and will either be an empty
     * list or contain windows that need to be force removed.
     */
    List<AutoPtr<WindowState> >* mForceRemoves;

    AutoPtr<CSurfaceSession> mFxSession;
    DimAnimator* mDimAnimator;
    AutoPtr<ISurface> mBlurSurface;
    Boolean mBlurShown;
    Watermark* mWatermark;

    Int32 mTransactionSequence;

    Float mTmpFloats[9];

    Boolean mSafeMode;
    Boolean mDisplayEnabled;
    Boolean mSystemBooted;
    Int32 mInitialDisplayWidth;
    Int32 mInitialDisplayHeight;
    Int32 mRotation;
    Int32 mRequestedRotation;
    Int32 mForcedAppOrientation;
    Int32 mLastRotationFlags;
//    ArrayList<IRotationWatcher> mRotationWatchers;

    Boolean mLayoutNeeded;
    Boolean mAnimationPending;
    Boolean mDisplayFrozen;
    Boolean mWaitingForConfig;
    Boolean mWindowsFreezingScreen;
    Int64 mFreezeGcPending;
    Int32 mAppsFreezingScreen;

    Int32 mLayoutSeq;

    // State while inside of layoutAndPlaceSurfacesLocked().
    Boolean mFocusMayChange;

    AutoPtr<CConfiguration> mCurConfiguration;

    // This is held as long as we have the screen frozen, to give us time to
    // perform a rotation animation when turning off shows the lock screen which
    // changes the orientation.
//    PowerManager.WakeLock mScreenFrozenLock;

    // State management of app transitions.  When we are preparing for a
    // transition, mNextAppTransition will be the kind of transition to
    // perform or TRANSIT_NONE if we are not waiting.  If we are waiting,
    // mOpeningApps and mClosingApps are the lists of tokens that will be
    // made visible or hidden at the next transition.
    Int32 mNextAppTransition;// = WindowManagerPolicy.TRANSIT_UNSET;
    String mNextAppTransitionPackage;
    Int32 mNextAppTransitionEnter;
    Int32 mNextAppTransitionExit;
    Boolean mAppTransitionReady;
    Boolean mAppTransitionRunning;
    Boolean mAppTransitionTimeout;
    Boolean mStartingIconInTransition;
    Boolean mSkipAppTransitionAnimation;
    List<AppWindowToken*> mOpeningApps;
    List<AppWindowToken*> mClosingApps;
    List<AppWindowToken*> mToTopApps;
    List<AppWindowToken*> mToBottomApps;

    AutoPtr<IDisplay> mDisplay;

    AutoPtr<WindowState> mCurrentFocus;
    AutoPtr<WindowState> mLastFocus;

    // This just indicates the window the input method is on top of, not
    // necessarily the window its input is going to.
    AutoPtr<WindowState> mInputMethodTarget;
    AutoPtr<WindowState> mUpcomingInputMethodTarget;
    Boolean mInputMethodTargetWaitingAnim;
    Int32 mInputMethodAnimLayerAdjustment;

    AutoPtr<WindowState> mInputMethodWindow;
    List<AutoPtr<WindowState> > mInputMethodDialogs;

    List<WindowToken*> mWallpaperTokens;

    // If non-null, this is the currently visible window that is associated
    // with the wallpaper.
    AutoPtr<WindowState> mWallpaperTarget;
    // If non-null, we are in the middle of animating from one wallpaper target
    // to another, and this is the lower one in Z-order.
    AutoPtr<WindowState> mLowerWallpaperTarget;
    // If non-null, we are in the middle of animating from one wallpaper target
    // to another, and this is the higher one in Z-order.
    AutoPtr<WindowState> mUpperWallpaperTarget;
    Int32 mWallpaperAnimLayerAdjustment;
    Float mLastWallpaperX;
    Float mLastWallpaperY;
    Float mLastWallpaperXStep;
    Float mLastWallpaperYStep;
    // This is set when we are waiting for a wallpaper to tell us it is done
    // changing its scroll position.
    AutoPtr<WindowState> mWaitingOnWallpaper;
    // The last time we had a timeout when waiting for a wallpaper.
    Millisecond64 mLastWallpaperTimeoutTime;

    AppWindowToken* mFocusedApp;

//    PowerManagerService mPowerManager;

    Float mWindowAnimationScale;
    Float mTransitionAnimationScale;

    InputManager* mInputManager;

    // Who is holding the screen on.
    AutoPtr<IWindowSession> mHoldingScreenOn;
//    PowerManager.WakeLock mHoldingScreenWakeLock;

    Boolean mTurnOnScreen;

    /**
     * Whether the UI is currently running in touch mode (not showing
     * navigational focus because the user is directly pressing the screen).
     */
    Boolean mInTouchMode;
    List<WindowChangeListener*> mWindowChangeListeners;
    Boolean mWindowsChanged;

    AutoPtr<CConfiguration> mTempConfiguration;
    Int32 mScreenLayout;

    // The frame use to limit the size of the app running in compatibility mode.
    AutoPtr<IRect> mCompatibleScreenFrame;
    // The surface used to fill the outer rim of the app running in compatibility mode.
    AutoPtr<ISurface> mBackgroundFillerSurface;
    Boolean mBackgroundFillerShown;

    Boolean mInLayout;

    InputMonitor* mInputMonitor;

    AutoPtr<IWindowSession> mLastReportedHold;
};

#endif //__CWINDOWMANAGERSERVICE_H__
