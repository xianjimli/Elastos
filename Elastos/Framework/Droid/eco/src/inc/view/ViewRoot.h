
#ifndef __VIEWROOT_H__
#define __VIEWROOT_H__

#include "ext/frameworkext.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/View.h"
#include "view/CSurface.h"
#include "view/CViewRootW.h"
#include "view/CInputMethodCallback.h"
#include "view/BaseSurfaceHolder.h"
#include "view/InputQueue.h"
#include "view/ViewTreeObserver.h"
#include "graphics/CRect.h"
#include "graphics/CRegion.h"
#include <elastos/ElRefBase.h>
#include "content/CConfiguration.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Map.h>

using namespace Elastos;

extern "C" const InterfaceID EIID_ViewRoot;

class ViewConfiguration;
class Scroller;

class ViewRoot :
    public ElRefBase,
    public IViewParent,
    public View::AttachInfo::Callbacks
{
    friend class View;
    friend class CWindowManagerImpl;

private:
    static const char* TAG;
    static const Boolean DBG = TRUE;
    static const Boolean SHOW_FPS = TRUE;
    static const Boolean LOCAL_LOGV = TRUE; //FALSE ? Config.LOGD : Config.LOGV;
    static const Boolean DEBUG_DRAW = TRUE || LOCAL_LOGV;
    static const Boolean DEBUG_LAYOUT = TRUE || LOCAL_LOGV;
    static const Boolean DEBUG_INPUT = TRUE || LOCAL_LOGV;
    static const Boolean DEBUG_INPUT_RESIZE = TRUE || LOCAL_LOGV;
    static const Boolean DEBUG_ORIENTATION = TRUE || LOCAL_LOGV;
    static const Boolean DEBUG_TRACKBALL = TRUE || LOCAL_LOGV;
    static const Boolean DEBUG_IMF = TRUE || LOCAL_LOGV;
    static const Boolean DEBUG_CONFIGURATION = TRUE || LOCAL_LOGV;
    static const Boolean WATCH_POINTER = FALSE;

    static const Boolean MEASURE_LATENCY = FALSE;

    /**
    * Maximum time we allow the user to roll the trackball enough to generate
    * a key event, before resetting the counters.
    */
    static const Int32 MAX_TRACKBALL_DELAY = 250;

    //static LatencyTimer lt;

    static Int64 sInstanceCount;

    static AutoPtr<IWindowSession> sWindowSession;
    static Mutex sStaticInitLock;

    static Boolean sInitialized;

    static List<AutoPtr<IRunnable> > sFirstDrawHandlers;
    static Mutex sFirstDrawHandlersLock;

    static Boolean sFirstDrawComplete;

    static List<AutoPtr<IComponentCallbacks> > sConfigCallbacks;
    static Mutex sConfigCallbacksLock;

    static Int32 sDrawTime;

    static pthread_key_t sKeyRunQueues;

private:
    class CSurfaceHolder : public ElRefBase, public ISurfaceHolder
    {
    public:
        CSurfaceHolder(
            /* [in] */ ViewRoot* viewRoot);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        // we only need a SurfaceHolder for opengl. it would be nice
        // to implement everything else though, especially the callback
        // support (opengl doesn't make use of it right now, but eventually
        // will).
        CARAPI GetSurface(
            /* [out] */ ISurface** surface);

        CARAPI IsCreating(
            /* [out] */ Boolean* result);

        CARAPI AddCallback(
            /* [in] */ ISurfaceHolderCallback* cback);

        CARAPI RemoveCallback(
            /* [in] */ ISurfaceHolderCallback* cback);

        CARAPI SetFixedSize(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SetSizeFromLayout();

        CARAPI SetFormat(
            /* [in] */ Int32 format);

        CARAPI SetType(
            /* [in] */ Int32 type);

        CARAPI SetKeepScreenOn(
            /* [in] */ Boolean screenOn);

        CARAPI LockCanvas(
            /* [out] */ ICanvas** canvas);

        CARAPI LockCanvasEx(
            /* [in] */ IRect* dirty,
            /* [out] */ ICanvas** canvas);

        CARAPI UnlockCanvasAndPost(
            /* [in] */ ICanvas* canvas);

        CARAPI GetSurfaceFrame(
            /* [out] */ IRect** rect);

    private:
        AutoPtr<ViewRoot> mViewRoot;
    };

    class _InputHandler : public InputHandler
    {
    public:
        _InputHandler(
            /* [in] */ ViewRoot* root);

        ~_InputHandler();

        CARAPI HandleKey(
            /* [in] */ IKeyEvent* event,
            /* [in] */ IRunnable* finishedCallback);

        CARAPI HandleMotion(
            /* [in] */ IMotionEvent* event,
            /* [in] */ IRunnable* finishedCallback);

    private:
        AutoPtr<ViewRoot> mViewRoot;
    };

    class ResizedInfo
    {
    public:
        AutoPtr<IRect> mCoveredInsets;
        AutoPtr<IRect> mVisibleInsets;
        AutoPtr<IConfiguration> mNewConfig;
    };

    class TakenSurfaceHolder : public BaseSurfaceHolder
    {
    public:
        TakenSurfaceHolder(
            /* [in] */ ViewRoot* viewRoot);

        CARAPI_(Boolean) OnAllowLockCanvas();

        CARAPI_(void) OnRelayoutContainer();

        CARAPI SetFormat(
            /* [in] */ Int32 format);

        CARAPI SetType(
            /* [in] */ Int32 type);

        CARAPI OnUpdateSurface();

        CARAPI IsCreating(
            /* [out] */ Boolean* result);

        CARAPI SetFixedSize(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SetKeepScreenOn(
            /* [in] */ Boolean screenOn);

    private:
        AutoPtr<ViewRoot> mViewRoot;
    };

    /**
     * Maintains state information for a single trackball axis, generating
     * discrete (DPAD) movements based on raw trackball motion.
     */
    class TrackballAxis
    {
        friend class ViewRoot;
    public:
        TrackballAxis();

        CARAPI_(void) Reset(
            /* [in] */ Int32 _step);

        /**
         * Add trackball movement into the state.  If the direction of movement
         * has been reversed, the state is reset before adding the
         * movement (so that you don't have to compensate for any previously
         * collected movement before see the result of the movement in the
         * new direction).
         *
         * @return Returns the absolute value of the amount of movement
         * collected so far.
         */
        CARAPI_(Float) Collect(
            /* [in] */ Float off,
            /* [in] */ Int64 time,
            /* [in] */ const char* axis);

        /**
         * Generate the number of discrete movement events appropriate for
         * the currently collected trackball movement.
         *
         * @param precision The minimum movement required to generate the
         * first discrete movement.
         *
         * @return Returns the number of discrete movements, either positive
         * or negative, or 0 if there is not enough trackball movement yet
         * for a discrete movement.
         */
        CARAPI_(Int32) Generate(
            /* [in] */ Float precision);

    private:
        /**
        * The maximum amount of acceleration we will apply.
        */
        static const Float MAX_ACCELERATION = 20;

        /**
        * The maximum amount of time (in milliseconds) between events in order
        * for us to consider the user to be doing fast trackball movements,
        * and thus apply an acceleration.
        */
        static const Int64 FAST_MOVE_TIME = 150;

        /**
        * Scaling factor to the time (in milliseconds) between events to how
        * much to multiple/divide the current acceleration.  When movement
        * is < FAST_MOVE_TIME this multiplies the acceleration; when >
        * FAST_MOVE_TIME it divides it.
        */
        static const Float ACCEL_MOVE_SCALING_FACTOR = (1.0f/40);

        Float mPosition;
        Float mAbsPosition;
        Float mAcceleration;
        Int64 mLastMoveTime;
        Int32 mStep;
        Int32 mDir;
        Int32 mNonAccelMovement;
    };

    class RunQueue
    {
    public:
        CARAPI_(void) Post(
            /* [in] */ IRunnable* action);

        CARAPI_(void) PostDelayed(
            /* [in] */ IRunnable* action,
            /* [in] */ Int32 delayMillis);

        CARAPI_(void) RemoveCallbacks(
            /* [in] */ IRunnable* action);

        CARAPI_(void) ExecuteActions(
            /* [in] */ IApartment* apartment);

    private:
        class HandlerAction : public ElRefBase
        {
        public:
            AutoPtr<IRunnable> mAction;
            Int32 mDelay;
        };

    private:
        List<AutoPtr<HandlerAction> > mActions;
        Mutex mLock;
    };
public:
    static CARAPI_(IWindowSession*) GetWindowSession(
        /* [in] */ /*Looper mainLooper*/);

    static CARAPI_(Int64) GetInstanceCount();

    static CARAPI_(void) AddFirstDrawHandler(
        /* [in] */ IRunnable* callback);

    static CARAPI_(void) AddConfigCallback(
        /* [in] */ IComponentCallbacks* callback);

    /**
    * Indicates whether we are in touch mode. Calling this method triggers an IPC
    * call and should be avoided whenever possible.
    *
    * @return True, if the device is in touch mode, FALSE otherwise.
    *
    * @hide
    */
    static CARAPI_(Boolean) IsInTouchMode();

    static CARAPI_(RunQueue*) GetRunQueue();

private:
    /**
    * Return TRUE if child is an ancestor of parent, (or equal to the parent).
    */
    static CARAPI_(Boolean) IsViewDescendantOf(
        /* [in] */ IView* child,
        /* [in] */ IView* parent);

    static CARAPI_(void) ForceLayout(
        /* [in] */ IView* view);

    /**
    * @param keyCode The key code
    * @return True if the key is directional.
    */
    static CARAPI_(Boolean) IsDirectional(
        /* [in] */ Int32 keyCode);

    /**
    * Returns TRUE if this key is a keyboard key.
    * @param keyEvent The key event.
    * @return whether this key is a keyboard key.
    */
    static CARAPI_(Boolean) IsKeyboardKey(
        /* [in] */ IKeyEvent* keyEvent);

    /**
    * log motion events
    */
    static CARAPI_(void) CaptureMotionLog(
        /* [in] */ const char* subTag,
        /* [in] */ IMotionEvent* ev);

    /**
    * log motion events
    */
    static CARAPI_(void) CaptureKeyLog(
        /* [in] */ const char* subTag,
        /* [in] */ IKeyEvent* ev);

public:
    ViewRoot(
        /* [in] */ IContext* ctx);

    ~ViewRoot();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
    * Call this to profile the next traversal call.
    * FIXME for perf testing only. Remove eventually
    */
    CARAPI_(void) Profile();

    CARAPI SetView(
        /* [in] */ IView* view,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ IView* panelParentView);

    CARAPI_(IView*) GetView();

    CARAPI RequestLayout();

    //in IViewParent
    CARAPI RequestLayoutEx();

    CARAPI IsLayoutRequested(
        /* [out] */ Boolean* result);

    //in IViewParent
    CARAPI IsLayoutRequestedEx(
        /* [out] */ Boolean* result);

    CARAPI InvalidateChild(
        /* [in] */ IView* child,
        /* [in] */ IRect* dirty);

    CARAPI GetParent(
        /* [out] */ IViewParent** parent);

    //in IViewParent
    CARAPI GetParentEx(
        /* [out] */ IViewParent** parent);

    CARAPI InvalidateChildInParent(
        /* [in] */ ArrayOf<Int32>* location,
        /* [in] */ IRect* dirty,
        /* [out] */ IViewParent** parent);

    CARAPI GetChildVisibleRect(
        /* [in] */ IView* child,
        /* [in] */ IRect* r,
        /* [in] */ IPoint* offset,
        /* [out] */ Boolean* result);

    CARAPI BringChildToFront(
        /* [in] */ IView* child);

    CARAPI_(void) ScheduleTraversals();

    CARAPI_(void) UnscheduleTraversals();

    CARAPI RequestTransparentRegion(
        /* [in] */ IView* child);

    CARAPI RequestChildFocus(
        /* [in] */ IView* child,
        /* [in] */ IView* focused);

    CARAPI ClearChildFocus(
        /* [in] */ IView* child);

    CARAPI FocusableViewAvailable(
        /* [in] */ IView* v);

    CARAPI RecomputeViewAttributes(
        /* [in] */ IView* child);

    CARAPI RetrievePendingEvent(
        /* [in] */Int32 seq,
        /* [out] */ IInterface** event);

    CARAPI_(void) PlaySoundEffect(
        /* [in] */ Int32 effectId);

    CARAPI PerformHapticFeedback(
        /* [in] */ Int32 effectId,
        /* [in] */ Boolean always,
        /* [out] */ Boolean* result);

    CARAPI FocusSearch(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction,
        /* [out */ IView** result);

    //in IViewParent
    CARAPI FocusSearchEx(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction,
        /* [out */ IView** result);

    CARAPI_(void) Debug();

    CARAPI Die(
        /* [in] */ Boolean immediate);

    CARAPI_(void) DispatchFinishedEvent(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI DispatchResized(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ IRect* coveredInsets,
        /* [in] */ IRect* visibleInsets,
        /* [in] */ Boolean reportDraw,
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(void) DispatchKey(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) DispatchMotion(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) DispatchPointer(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) DispatchTrackball(
        /* [in] */ IMotionEvent* event);

    CARAPI DispatchAppVisibility(
        /* [in] */ Boolean visible);

    CARAPI_(void) DispatchGetNewSurface();

    CARAPI_(void) WindowFocusChanged(
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean inTouchMode);

    CARAPI_(void) DispatchCloseSystemDialogs(
        /* [in] */ const String& reason);

    CARAPI ShowContextMenuForChild(
        /* [in] */ IView* originalView,
        /* [out] */ Boolean* result);

    CARAPI_(void) CreateContextMenu(
        /* [in] */ IContextMenu* menu);

    CARAPI ChildDrawableStateChanged(
        /* [in] */ IView* child);

    CARAPI RequestDisallowInterceptTouchEvent(
        /* [in] */ Boolean disallowIntercept);

    CARAPI RequestChildRectangleOnScreen(
        /* [in] */ IView* child,
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate,
        /* [out] */ Boolean* result);

protected:
    CARAPI_(void) SetLayoutParams(
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Boolean newView);

    CARAPI_(void) HandleAppVisibility(
        /* [in] */ Boolean visible);

    CARAPI_(void) HandleGetNewSurface();

    CARAPI_(Int32) GetHostVisibility();

    CARAPI_(Boolean) ScrollToRectOrFocus(
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate);

    CARAPI_(void) DispatchDetachedFromWindow();

    CARAPI_(void) UpdateConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ Boolean force);

    /**
    * Something in the current window tells us we need to change the touch mode.  For
    * example, we are not in touch mode, and the user touches the screen.
    *
    * If the touch mode has changed, tell the window manager, and handle it locally.
    *
    * @param inTouchMode Whether we want to be in touch mode.
    * @return True if the touch mode changed and focus changed was changed as a result
    */
    CARAPI_(Boolean) EnsureTouchMode(
        /* [in] */ Boolean inTouchMode);

    CARAPI_(void) HandleFinishedEvent(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI DoDie();

    CARAPI CheckThread();

    CARAPI_(IRect*) GetWindowFrame();

private:
    // TODO:
    CARAPI_(void) TestSurface();

    // TODO:
    CARAPI_(void) TestCanvas(ICanvas* canvas);

    CARAPI_(void) InitializeGL();

    CARAPI_(void) InitializeGLInner();

    CARAPI_(void) DestroyGL();

    CARAPI_(void) CheckEglErrors();

    CARAPI_(void) PerformTraversals();

    /**
    * Figures out the measure spec for the root view in a window based on it's
    * layout params.
    *
    * @param windowSize
    *            The available width or height of the window
    *
    * @param rootDimension
    *            The layout params for one dimension (width or height) of the
    *            window.
    *
    * @return The measure spec to use to measure the root view.
    */
    CARAPI_(Int32) GetRootMeasureSpec(
        /* [in] */ Int32 windowSize,
        /* [in] */ Int32 rootDimension);

    CARAPI_(void) Draw(
        /* [in] */ Boolean fullRedrawNeeded);

    CARAPI_(void) StartInputEvent(
        /* [in] */ IRunnable* finishedCallback);

    CARAPI_(void) FinishInputEvent();

    /**
    * Ensure that the touch mode for this window is set, and if it is changing,
    * take the appropriate action.
    * @param inTouchMode Whether we want to be in touch mode.
    * @return True if the touch mode changed and focus changed was changed as a result
    */
    CARAPI_(Boolean) EnsureTouchModeLocally(
        /* [in] */ Boolean inTouchMode);

    CARAPI_(Boolean) EnterTouchMode();

    /**
    * Find an ancestor of focused that wants focus after its descendants and is
    * focusable in touch mode.
    * @param focused The currently focused view.
    * @return An appropriate view, or NULL if no such view exists.
    */
    CARAPI_(IViewGroup*) FindAncestorToTakeFocusInTouchMode(
        /* [in] */ IView* focused);

    CARAPI_(Boolean) LeaveTouchMode();

    CARAPI_(void) DeliverPointerEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) DeliverTrackballEvent(
        /* [in] */ IMotionEvent* event);

    /**
    * See if the key event means we should leave touch mode (and leave touch
    * mode if so).
    * @param event The key event.
    * @return Whether this key event should be consumed (meaning the act of
    *   leaving touch mode alone is considered the event).
    */
    CARAPI_(Boolean) CheckForLeavingTouchModeAndConsume(
        /* [in] */ IKeyEvent* event);

    CARAPI_(Int32) EnqueuePendingEvent(
        /* [in] */ IInterface* event,
        /* [in] */ Boolean sendDone);

    CARAPI_(void) DeliverKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [in] */Boolean sendDone);

    CARAPI_(void) DeliverKeyEventToViewHierarchy(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Boolean sendDone);

    //AudioManager GetAudioManager();

    CARAPI_(Int32) RelayoutWindow(
        /* [in] */ IWindowManagerLayoutParams* params,
        /* [in] */ Int32 viewVisibility,
        /* [in] */ Boolean insetsPending);

    CARAPI DispatchKey(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Boolean sendDone);

    CARAPI DispatchMotion(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Boolean sendDone);

    CARAPI DispatchPointer(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Boolean sendDone);

    CARAPI DispatchTrackball(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Boolean sendDone);

    /**
    * The window is getting focus so if there is anything focused/selected
    * send an {@link AccessibilityEvent} to announce that.
    */
    CARAPI_(void) SendAccessibilityEvents();

    CARAPI SendMessage(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    CARAPI SendMessageAtTime(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI RemoveMessage(
        /* [in] */ Handle32 func);

    CARAPI HandleResized(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Boolean reportDraw,
        /* [in] */ ResizedInfo* info);

    CARAPI HandleKey(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Boolean sendDone);

    CARAPI HandlePointer(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Boolean sendDone);

    CARAPI HandleTrackball(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Boolean sendDone);

    CARAPI HandleWindowFocusChanged(
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean inTouchMode);

    CARAPI HandleCloseSystemDialogs(
        /* [in] */ const String& reason);

public:
    Int32 mLastTrackballTime;
    TrackballAxis* mTrackballAxisX;
    TrackballAxis* mTrackballAxisY;

    Int32 mTmpLocation[2];

    AutoPtr<CInputMethodCallback> mInputMethodCallback;
    Map<Int32, AutoPtr<IInterface> > mPendingEvents;
    Int32 mPendingEventSeq;

    Int32 mThreadID;

    AutoPtr<CWindowManagerLayoutParams> mWindowAttributes;

    AutoPtr<CViewRootW> mWindow;

    AutoPtr<IView> mView;
    AutoPtr<IView> mFocusedView;
    AutoPtr<IView> mRealFocusedView;  // this is not set to null in touch mode
    Int32 mViewVisibility;
    Boolean mAppVisible;

    AutoPtr<ISurfaceHolderCallback2> mSurfaceHolderCallback;
    AutoPtr<BaseSurfaceHolder> mSurfaceHolder;
    Boolean mIsCreating;
    Boolean mDrawingAllowed;

    AutoPtr<CRegion> mTransparentRegion;
    AutoPtr<CRegion> mPreviousTransparentRegion;

    Int32 mWidth;
    Int32 mHeight;
    AutoPtr<CRect> mDirty;    // will be a graphics.Region soon
    Boolean mIsAnimating;

    AutoPtr<ITranslator> mTranslator;

    View::AttachInfo* mAttachInfo;
    AutoPtr<CInputChannel> mInputChannel;
    InputQueue::Callback* mInputQueueCallback;
    InputQueue* mInputQueue;

    AutoPtr<CRect> mTempRect; // used in the transaction to not thrash the heap.
    AutoPtr<CRect> mVisRect; // used to retrieve visible rect of focused view.

    Boolean mTraversalScheduled;
    Boolean mWillDrawSoon;
    Boolean mLayoutRequested;
    Boolean mFirst;
    Boolean mReportNextDraw;
    Boolean mFullRedrawNeeded;
    Boolean mNewSurfaceNeeded;
    Boolean mHasHadWindowFocus;
    Boolean mLastWasImTarget;

    Boolean mWindowAttributesChanged;

    // These can be accessed by any thread, must be protected with a lock.
    // Surface can never be reassigned or cleared (use Surface.clear()).
    AutoPtr<CSurface> mSurface;

    Boolean mAdded;
    Boolean mAddedTouchMode;

    Int32 mAddNesting;

    // These are accessed by multiple threads.
    AutoPtr<CRect> mWinFrame; // frame given by window manager.

    AutoPtr<CRect> mPendingVisibleInsets;
    AutoPtr<CRect> mPendingContentInsets;

    ViewTreeObserver::InternalInsetsInfo* mLastGivenInsets;

    AutoPtr<CConfiguration> mLastConfiguration;
    AutoPtr<CConfiguration> mPendingConfiguration;

    Boolean mScrollMayChange;
    Int32 mSoftInputMode;
    AutoPtr<IView> mLastScrolledFocus;
    Int32 mScrollY;
    Int32 mCurScrollY;
    Scroller* mScroller;

    //EGL10 mEgl;
    //EGLDisplay mEglDisplay;
    //EGLContext mEglContext;
    //EGLSurface mEglSurface;
    //GL11 mGL;
    //Canvas mGlCanvas;
    Boolean mUseGL;
    Boolean mGlWanted;

    ViewConfiguration* mViewConfiguration;

    /**
    * see {@link #playSoundEffect(int)}
    */
    //AudioManager mAudioManager;

private:
    Int32 mDensity;

    // FIXME for perf testing only
    Boolean mProfile;

    AutoPtr<IRunnable> mFinishedCallback;

    AutoPtr<CSurfaceHolder> mHolder;

    _InputHandler* mInputHandler;

    AutoPtr<IApartment> mApartment;
    Mutex mSyncLock;
};

#endif // __VIEWROOT_H__
