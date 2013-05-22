
#include "view/ViewRoot.h"
#include "view/CWindowManagerImpl.h"
#include "view/ViewConfiguration.h"
#include "view/CInputDevice.h"
#include "view/FocusFinder.h"
#include "view/inputmethod/CLocalInputMethodManager.h"
#include "impl/CPhoneWindow.h"
#include "utils/CDisplayMetrics.h"
#include "graphics/ElPixelFormat.h"
#include "graphics/CCanvas.h"
#include "graphics/CPaint.h"
#include "content/CompatibilityInfo.h"
#include "widget/Scroller.h"
#include "os/CApartment.h"
#include "os/SystemClock.h"
#include <Logger.h>
#include <StringBuffer.h>
#include <elastos/Math.h>


using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

extern "C" const InterfaceID EIID_ViewRoot =
        {0xcccccccc,0xcccc,0xcccc,{0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc}};

ViewRoot::CSurfaceHolder::CSurfaceHolder(
    /* [in] */ ViewRoot* viewRoot)
    : mViewRoot(viewRoot)
{}

PInterface ViewRoot::CSurfaceHolder::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ISurfaceHolder*)this;
    }
    else if (riid == EIID_ISurfaceHolder) {
        return (ISurfaceHolder*)this;
    }
    return NULL;
}

UInt32 ViewRoot::CSurfaceHolder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewRoot::CSurfaceHolder::Release()
{
    return ElRefBase::Release();
}

ECode ViewRoot::CSurfaceHolder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ISurfaceHolder*)this) {
        *pIID = EIID_ISurfaceHolder;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::GetSurface(
    /* [out] */ ISurface** surface)
{
    if (surface == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *surface = mViewRoot->mSurface;
    if (*surface) {
        (*surface)->AddRef();
    }

    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::IsCreating(
    /* [out] */ Boolean* result)
{
    if (result == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *result = FALSE;

    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::AddCallback(
    /* [in] */ ISurfaceHolderCallback* cback)
{
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::RemoveCallback(
    /* [in] */ ISurfaceHolderCallback* cback)
{
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::SetFixedSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::SetSizeFromLayout()
{
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::SetFormat(
    /* [in] */ Int32 format)
{
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::SetType(
    /* [in] */ Int32 type)
{
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::SetKeepScreenOn(
    /* [in] */ Boolean screenOn)
{
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::LockCanvas(
    /* [out] */ ICanvas** canvas)
{
    if (canvas == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *canvas = NULL;

    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::LockCanvasEx(
    /* [in] */ IRect* dirty,
    /* [out] */ ICanvas** canvas)
{
    if (canvas == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *canvas = NULL;

    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::UnlockCanvasAndPost(
    /* [in] */ ICanvas* canvas)
{
    return NOERROR;
}

ECode ViewRoot::CSurfaceHolder::GetSurfaceFrame(
    /* [out] */ IRect** rect)
{
    if (rect == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *rect = NULL;

    return NOERROR;
}

ViewRoot::_InputHandler::_InputHandler(
    /* [in] */ ViewRoot* root) :
    mViewRoot(root)
{}

ViewRoot::_InputHandler::~_InputHandler()
{}

ECode ViewRoot::_InputHandler::HandleKey(
    /* [in] */ IKeyEvent* event,
    /* [in] */ IRunnable* finishedCallback)
{
    mViewRoot->StartInputEvent(finishedCallback);
    return mViewRoot->DispatchKey(event, TRUE);
}

ECode ViewRoot::_InputHandler::HandleMotion(
    /* [in] */ IMotionEvent* event,
    /* [in] */ IRunnable* finishedCallback)
{
    mViewRoot->StartInputEvent(finishedCallback);
    return mViewRoot->DispatchMotion(event, TRUE);
}

ViewRoot::TakenSurfaceHolder::TakenSurfaceHolder(
    /* [in] */ ViewRoot* viewRoot) :
    mViewRoot(viewRoot)
{
}

Boolean ViewRoot::TakenSurfaceHolder::OnAllowLockCanvas()
{
    return mViewRoot->mDrawingAllowed;
}

void ViewRoot::TakenSurfaceHolder::OnRelayoutContainer()
{
    // Not currently interesting -- from changing between fixed and layout size.
}

ECode ViewRoot::TakenSurfaceHolder::SetFormat(
    /* [in] */ Int32 format)
{
    assert(mViewRoot->mView.Get());
    RootViewSurfaceTaker* rvst =
        (RootViewSurfaceTaker*)(mViewRoot->mView)->Probe(EIID_RootViewSurfaceTaker);
    assert(rvst);

    return rvst->SetSurfaceFormat(format);
}

ECode ViewRoot::TakenSurfaceHolder::SetType(
    /* [in] */ Int32 type)
{
    assert(mViewRoot->mView.Get());
    RootViewSurfaceTaker* rvst =
        (RootViewSurfaceTaker*)(mViewRoot->mView)->Probe(EIID_RootViewSurfaceTaker);
    assert(rvst);

    return rvst->SetSurfaceType(type);
}

ECode ViewRoot::TakenSurfaceHolder::OnUpdateSurface()
{
    // We take care of format and type changes on our own.
    Logger::E(ViewRoot::TAG, "Shouldn't be here");

    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode ViewRoot::TakenSurfaceHolder::IsCreating(
    /* [out] */ Boolean* result)
{
    if (result == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *result = mViewRoot->mIsCreating;

    return NOERROR;
}

ECode ViewRoot::TakenSurfaceHolder::SetFixedSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    Logger::E(ViewRoot::TAG, "Currently only support sizing from layout");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ViewRoot::TakenSurfaceHolder::SetKeepScreenOn(
    /* [in] */ Boolean screenOn)
{
    assert(mViewRoot->mView.Get());
    RootViewSurfaceTaker* rvst =
        (RootViewSurfaceTaker*)(mViewRoot->mView)->Probe(EIID_RootViewSurfaceTaker);
    assert(rvst);

    return rvst->SetSurfaceKeepScreenOn(screenOn);
}

const Float ViewRoot::TrackballAxis::MAX_ACCELERATION;
const Float ViewRoot::TrackballAxis::ACCEL_MOVE_SCALING_FACTOR;

ViewRoot::TrackballAxis::TrackballAxis() :
    mAcceleration(1),
    mLastMoveTime(0)
{
}

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
void ViewRoot::TrackballAxis::Reset(
    /* [in] */ Int32 _step)
{
    mPosition = 0;
    mAcceleration = 1;
    mLastMoveTime = 0;
    mStep = _step;
    mDir = 0;
}

Float ViewRoot::TrackballAxis::Collect(
    /* [in] */ Float off,
    /* [in] */ Int64 time,
    /* [in] */ const char* axis)
{
    Int64 normTime;
    if (off > 0) {
        normTime = (Int64)(off * FAST_MOVE_TIME);
        if (mDir < 0) {
            if (ViewRoot::DEBUG_TRACKBALL)
                Logger::D(ViewRoot::TAG, StringBuffer(axis) + " reversed to positive!");
            mPosition = 0;
            mStep = 0;
            mAcceleration = 1;
            mLastMoveTime = 0;
        }
        mDir = 1;
    }
    else if (off < 0) {
        normTime = (Int64)((-off) * FAST_MOVE_TIME);
        if (mDir > 0) {
            if (ViewRoot::DEBUG_TRACKBALL)
                Logger::D(ViewRoot::TAG, StringBuffer(axis) + " reversed to negative!");
            mPosition = 0;
            mStep = 0;
            mAcceleration = 1;
            mLastMoveTime = 0;
        }
        mDir = -1;
    }
    else {
        normTime = 0;
    }

    // The number of milliseconds between each movement that is
    // considered "normal" and will not result in any acceleration
    // or deceleration, scaled by the offset we have here.
    if (normTime > 0) {
        Int64 delta = time - mLastMoveTime;
        mLastMoveTime = time;
        Float acc = mAcceleration;
        if (delta < normTime) {
            // The user is scrolling rapidly, so increase acceleration.
            Float scale = (normTime-delta) * ACCEL_MOVE_SCALING_FACTOR;
            if (scale > 1) acc *= scale;
            if (ViewRoot::DEBUG_TRACKBALL) {
                //Logger::D(ViewRoot::TAG, StringBuffer(axis) + " accelerate: off="
                //+ off + " normTime=" + normTime + " delta=" + delta
                //+ " scale=" + scale + " acc=" + acc);
            }
            mAcceleration = acc < MAX_ACCELERATION ? acc : MAX_ACCELERATION;
        }
        else {
            // The user is scrolling slowly, so decrease acceleration.
            Float scale = (delta-normTime) * ACCEL_MOVE_SCALING_FACTOR;
            if (scale > 1)
                acc /= scale;

            if (ViewRoot::DEBUG_TRACKBALL) {
                //Logger::D(ViewRoot::TAG, StringBuffer(axis) + " deccelerate: off="
                //    + off + " normTime=" + normTime + " delta=" + delta
                //    + " scale=" + scale + " acc=" + acc);
            }
            mAcceleration = acc > 1 ? acc : 1;
        }
    }

    mPosition += off;

    return (mAbsPosition = Math::Abs(mPosition));
}

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
Int32 ViewRoot::TrackballAxis::Generate(
    /* [in] */ Float precision)
{
    Int32 movement = 0;
    mNonAccelMovement = 0;
    do {
        const Int32 dir = mPosition >= 0 ? 1 : -1;
        switch (mStep) {
            // If we are going to execute the first step, then we want
            // to do this as soon as possible instead of waiting for
            // a full movement, in order to make things look responsive.
        case 0:
            {
                if (mAbsPosition < precision) {
                    return movement;
                }
                movement += dir;
                mNonAccelMovement += dir;
                mStep = 1;
            }
            break;
            // If we have generated the first movement, then we need
            // to wait for the second complete trackball motion before
            // generating the second discrete movement.
        case 1:
            {
                if (mAbsPosition < 2) {
                    return movement;
                }
                movement += dir;
                mNonAccelMovement += dir;
                mPosition += dir > 0 ? -2 : 2;
                mAbsPosition = Math::Abs(mPosition);
                mStep = 2;
            }
            break;
            // After the first two, we generate discrete movements
            // consistently with the trackball, applying an acceleration
            // if the trackball is moving quickly.  This is a simple
            // acceleration on top of what we already compute based
            // on how quickly the wheel is being turned, to apply
            // a longer increasing acceleration to continuous movement
            // in one direction.
        default:
            {
                if (mAbsPosition < 1) {
                    return movement;
                }
                movement += dir;
                mPosition += dir >= 0 ? -1 : 1;
                mAbsPosition = Math::Abs(mPosition);
                Float acc = mAcceleration;
                acc *= 1.1f;
                mAcceleration = acc < MAX_ACCELERATION ? acc : mAcceleration;
            }
            break;
        }
    } while (TRUE);
}

void ViewRoot::RunQueue::Post(
    /* [in] */ IRunnable* action)
{
    PostDelayed(action, 0);
}

void ViewRoot::RunQueue::PostDelayed(
    /* [in] */ IRunnable* action,
    /* [in] */ Int32 delayMillis)
{
    AutoPtr<HandlerAction> handlerAction = new HandlerAction();
    handlerAction->mAction = action;
    handlerAction->mDelay = delayMillis;

    Mutex::Autolock lock(mLock);

    mActions.PushBack(handlerAction);
}

void ViewRoot::RunQueue::RemoveCallbacks(
    /* [in] */ IRunnable* action)
{
    Mutex::Autolock lock(mLock);

    List<AutoPtr<HandlerAction> >::Iterator iter = mActions.Begin();
    while (iter != mActions.End()) {
        if ((*iter)->mAction.Get() == action) {
            iter = mActions.Erase(iter);
        }
        else ++iter;
    }
}

void ViewRoot::RunQueue::ExecuteActions(
    /* [in] */ IApartment* apartment)
{
    Mutex::Autolock lock(mLock);

    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;

    List<AutoPtr<HandlerAction> >::Iterator iter;
    for (iter = mActions.Begin(); iter != mActions.End(); ++iter) {
        apartment->PostCppCallbackDelayed(
            (Handle32)(*iter)->mAction.Get(), *(Handle32*)&pHandlerFunc,
            NULL, 0, (*iter)->mDelay);
    }

    mActions.Clear();
}

const char* ViewRoot::TAG = "ViewRoot";
const Boolean ViewRoot::DBG;
const Boolean ViewRoot::SHOW_FPS;
const Boolean ViewRoot::LOCAL_LOGV;
const Boolean ViewRoot::DEBUG_DRAW;
const Boolean ViewRoot::DEBUG_LAYOUT;
const Boolean ViewRoot::DEBUG_INPUT;
const Boolean ViewRoot::DEBUG_INPUT_RESIZE;
const Boolean ViewRoot::DEBUG_ORIENTATION;
const Boolean ViewRoot::DEBUG_TRACKBALL;
const Boolean ViewRoot::DEBUG_IMF;
const Boolean ViewRoot::DEBUG_CONFIGURATION;
const Boolean ViewRoot::WATCH_POINTER;
const Boolean ViewRoot::MEASURE_LATENCY;
const Int32 ViewRoot::MAX_TRACKBALL_DELAY;
const Int32 ViewRoot::DISPATCH_KEY_FROM_IME;

Int64 ViewRoot::sInstanceCount = 0;
Boolean ViewRoot::sInitialized = FALSE;
Boolean ViewRoot::sFirstDrawComplete = FALSE;

AutoPtr<IWindowSession> ViewRoot::sWindowSession;
Mutex ViewRoot::sStaticInitLock;

List<AutoPtr<IRunnable> > ViewRoot::sFirstDrawHandlers;
Mutex ViewRoot::sFirstDrawHandlersLock;

List<AutoPtr<IComponentCallbacks> > ViewRoot::sConfigCallbacks;
Mutex ViewRoot::sConfigCallbacksLock;

pthread_key_t ViewRoot::sKeyRunQueues;

ViewRoot::ViewRoot(
    /* [in] */ IContext* context) :
    mLastTrackballTime(0),
    mPendingEventSeq(0),
    mViewVisibility(View_GONE),
    mAppVisible(TRUE),
    mIsCreating(FALSE),
    mDrawingAllowed(FALSE),
    mWidth(-1),
    mHeight(-1),
    mIsAnimating(FALSE),
    mInputQueueCallback(NULL),
    mInputQueue(NULL),
    mTraversalScheduled(FALSE),
    mWillDrawSoon(FALSE),
    mLayoutRequested(FALSE),
    mFirst(TRUE), // TRUE for the first time the view is added
    mReportNextDraw(FALSE),
    mFullRedrawNeeded(FALSE),
    mNewSurfaceNeeded(FALSE),
    mHasHadWindowFocus(FALSE),
    mLastWasImTarget(FALSE),
    mWindowAttributesChanged(FALSE),
    mAdded(FALSE),
    mAddedTouchMode(FALSE),
    mScrollMayChange(FALSE),
    mScrollY(0),
    mCurScrollY(0),
    mScroller(NULL),
    mUseGL(FALSE),
    mGlWanted(FALSE),
    mProfile(FALSE)
{
    mTrackballAxisX = new TrackballAxis();
    mTrackballAxisY = new TrackballAxis();

    mInputHandler = new _InputHandler(this);

    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));

    //if (MEASURE_LATENCY && lt == NULL) {
    //    lt = new LatencyTimer(100, 1000);
    //}

    // For debug only
    //++sInstanceCount;

    // Initialize the statics when this class is first instantiated. This is
    // done here instead of in the static block because Zygote does not
    // allow the spawning of threads.
    GetWindowSession(mApartment/*context.getMainLooper()*/);

    mThreadID = pthread_self();

    assert(SUCCEEDED(CWindowManagerLayoutParams::NewByFriend(
        (CWindowManagerLayoutParams**)&mWindowAttributes)));

    assert(SUCCEEDED(CRegion::NewByFriend((CRegion**)&mTransparentRegion)));
    assert(SUCCEEDED(CRegion::NewByFriend((CRegion**)&mPreviousTransparentRegion)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mDirty)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mVisRect)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mWinFrame)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mPendingVisibleInsets)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mPendingContentInsets)));

    assert(SUCCEEDED(CViewRootW::NewByFriend(
            (Handle32)this, context, (CViewRootW**)&mWindow)));

    assert(SUCCEEDED(CInputMethodCallback::NewByFriend(
            (Handle32)this, (CInputMethodCallback**)&mInputMethodCallback)));

    mAttachInfo = new View::AttachInfo(
        sWindowSession, (IInnerWindow*)mWindow->Probe(EIID_IInnerWindow),
        this, this);

    mViewConfiguration = ViewConfiguration::Get(context);

    AutoPtr<IResources> rc;
    context->GetResources((IResources**)&rc);
    AutoPtr<IDisplayMetrics> dm;
    rc->GetDisplayMetrics((IDisplayMetrics**)&dm);

    mDensity = ((CDisplayMetrics*)dm.Get())->mDensityDpi;

    assert(SUCCEEDED(CSurface::NewByFriend((CSurface**)&mSurface)));

    mLastGivenInsets = new ViewTreeObserver::InternalInsetsInfo();

    assert(SUCCEEDED(CConfiguration::NewByFriend(
        (CConfiguration**)&mLastConfiguration)));
    assert(SUCCEEDED(CConfiguration::NewByFriend(
        (CConfiguration**)&mPendingConfiguration)));

    mHolder = new CSurfaceHolder(this);
}

ViewRoot::~ViewRoot()
{
    delete mTrackballAxisX;
    delete mTrackballAxisY;
    delete mAttachInfo;
    delete mInputHandler;
    delete mScroller;
    delete mLastGivenInsets;
}

PInterface ViewRoot::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IViewParent*)this;
    }
    else if (riid == EIID_IViewParent) {
        return (IViewParent*)this;
    }
    else if (riid == EIID_ViewRoot) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 ViewRoot::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewRoot::Release()
{
    return ElRefBase::Release();
}

ECode ViewRoot::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ViewRoot::RunQueue* ViewRoot::GetRunQueue()
{
    RunQueue* runQueue = (RunQueue*)pthread_getspecific(sKeyRunQueues);
    if (runQueue == NULL) {
        runQueue = new RunQueue();
        assert(pthread_key_create(&sKeyRunQueues, NULL) == 0);
        assert(pthread_setspecific(sKeyRunQueues, runQueue) == 0);

    }
    assert(runQueue);

    return runQueue;
}

IWindowSession* ViewRoot::GetWindowSession(
    /* [in] */ IApartment* mainLooper)
{
    Mutex::Autolock lock(sStaticInitLock);

    if (!sInitialized) {
        AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::GetInstance(mainLooper);
        assert(imm != NULL);
        AutoPtr<IServiceManager> sm;
        GetServiceManager((IServiceManager**)&sm);
        AutoPtr<IWindowManager> wm;
        sm->GetService(String("window"), (IInterface**)(IWindowManager**)&wm);

        AutoPtr<IInputMethodClient> client;
        imm->GetClient((IInputMethodClient**)&client);
        AutoPtr<IInputContext> ctx;
        imm->GetInputContext((IInputContext**)&ctx);
        wm->OpenSession(client, ctx, (IWindowSession**)&sWindowSession);
        sInitialized = TRUE;
    }
    return sWindowSession;
}

Int64 ViewRoot::GetInstanceCount()
{
    return sInstanceCount;
}

void ViewRoot::AddFirstDrawHandler(
    /* [in] */ IRunnable* callback)
{
    Mutex::Autolock lock(sStaticInitLock);

    if (!sFirstDrawComplete) {
        sFirstDrawHandlers.PushBack(callback);
    }
}

void ViewRoot::AddConfigCallback(
    /* [in] */ IComponentCallbacks* callback)
{
    Mutex::Autolock lock(sConfigCallbacksLock);

    sConfigCallbacks.PushBack(callback);
}

/**
* Call this to profile the next traversal call.
* FIXME for perf testing only. Remove eventually
*/
void ViewRoot::Profile()
{
    mProfile = TRUE;
}

/**
 * Indicates whether we are in touch mode. Calling this method triggers an IPC
 * call and should be avoided whenever possible.
 *
 * @return True, if the device is in touch mode, false otherwise.
 *
 * @hide
 */
Boolean ViewRoot::IsInTouchMode()
{
    Boolean result = FALSE;

    if (sInitialized) {
        sWindowSession->GetInTouchMode(&result);
    }

    return result;
}

void ViewRoot::InitializeGL()
{
    //initializeGLInner();
    //Int32 err = mEgl.eglGetError();
    //if (err != EGL10.EGL_SUCCESS) {
    //    // give-up on using GL
    //    destroyGL();
    //    mGlWanted = FALSE;
    //}
}

void ViewRoot::InitializeGLInner()
{
    mSurface = NULL;
    assert(SUCCEEDED(CSurface::NewByFriend((CSurface**)&mSurface)));

    //final EGL10 egl = (EGL10) EGLContext.getEGL();
    //mEgl = egl;

    ///*
    //* Get to the default display.
    //*/
    //final EGLDisplay eglDisplay = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
    //mEglDisplay = eglDisplay;

    ///*
    //* We can now initialize EGL for that display
    //*/
    //Int32[] version = new Int32[2];
    //egl.eglInitialize(eglDisplay, version);

    ///*
    //* Specify a configuration for our opengl session
    //* and grab the first configuration that matches is
    //*/
    //final Int32[] configSpec = {
    //    EGL10.EGL_RED_SIZE,      5,
    //    EGL10.EGL_GREEN_SIZE,    6,
    //    EGL10.EGL_BLUE_SIZE,     5,
    //    EGL10.EGL_DEPTH_SIZE,    0,
    //    EGL10.EGL_NONE
    //};
    //final EGLConfig[] configs = new EGLConfig[1];
    //final Int32[] num_config = new Int32[1];
    //egl.eglChooseConfig(eglDisplay, configSpec, configs, 1, num_config);
    //final EGLConfig config = configs[0];

    ///*
    //* Create an OpenGL ES context. This must be done only once, an
    //* OpenGL context is a somewhat heavy object.
    //*/
    //final EGLContext context = egl.eglCreateContext(eglDisplay, config,
    //    EGL10.EGL_NO_CONTEXT, NULL);
    //mEglContext = context;

    ///*
    //* Create an EGL surface we can render into.
    //*/
    //final EGLSurface surface = egl.eglCreateWindowSurface(eglDisplay, config, mHolder, NULL);
    //mEglSurface = surface;

    ///*
    //* Before we can issue GL commands, we need to make sure
    //* the context is current and bound to a surface.
    //*/
    //egl.eglMakeCurrent(eglDisplay, surface, surface, context);

    ///*
    //* Get to the appropriate GL interface.
    //* This is simply done by casting the GL context to either
    //* GL10 or GL11.
    //*/
    //final GL11 gl = (GL11) context.getGL();
    //mGL = gl;
    //mGlCanvas = new Canvas(gl);
    //mUseGL = TRUE;
}

void ViewRoot::DestroyGL()
{
    //// inform skia that the context is gone
    //nativeAbandonGlCaches();

    //mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
    //    EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
    //mEgl.eglDestroyContext(mEglDisplay, mEglContext);
    //mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
    //mEgl.eglTerminate(mEglDisplay);
    //mEglContext = NULL;
    //mEglSurface = NULL;
    //mEglDisplay = NULL;
    //mEgl = NULL;
    //mGlCanvas = NULL;
    //mGL = NULL;
    //mUseGL = FALSE;
}

void ViewRoot::CheckEglErrors()
{
    //if (mUseGL) {
    //    Int32 err = mEgl.eglGetError();
    //    if (err != EGL10.EGL_SUCCESS) {
    //        // something bad has happened revert to
    //        // normal rendering.
    //        destroyGL();
    //        if (err != EGL11.EGL_CONTEXT_LOST) {
    //            // we'll try again if it was context lost
    //            mGlWanted = FALSE;
    //        }
    //    }
    //}
}

ECode ViewRoot::SetView(
    /* [in] */ IView* view,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ IView* panelParentView)
{
    Mutex::Autolock lock(mSyncLock);

    if (mView == NULL) {
        mView = view;
        Int32 changes;
        mWindowAttributes->CopyFrom(attrs, &changes);

        RootViewSurfaceTaker* rootViewST =
            (RootViewSurfaceTaker*)view->Probe(EIID_RootViewSurfaceTaker);
        if (rootViewST) {
            rootViewST->WillYouTakeTheSurface(
                (ISurfaceHolderCallback2**)&mSurfaceHolderCallback);
            if (mSurfaceHolderCallback != NULL) {
                mSurfaceHolder = new TakenSurfaceHolder(this);
                mSurfaceHolder->SetFormat(ElPixelFormat::UNKNOWN);
            }
        }

        AutoPtr<IContext> ctx;
        mView->GetContext((IContext**)&ctx);
        assert(ctx.Get());

        AutoPtr<IResources> resources;
        ctx->GetResources((IResources**)&resources);
        assert(resources.Get());

        AutoPtr<ICompatibilityInfo> compatibilityInfo;
        resources->GetCompatibilityInfo((ICompatibilityInfo**)&compatibilityInfo);
        assert(compatibilityInfo.Get());

        compatibilityInfo->GetTranslator((ITranslator**)&mTranslator);
        assert(mTranslator.Get());

        Boolean supportsScreen = TRUE;
        compatibilityInfo->SupportsScreen(&supportsScreen);

        if (mTranslator != NULL || !supportsScreen) {
            AutoPtr<IDisplayMetrics> metrics;
            resources->GetDisplayMetrics((IDisplayMetrics**)&metrics);
            mSurface->SetCompatibleDisplayMetrics(metrics, mTranslator);
        }

        Boolean restore = FALSE;
        if (mTranslator != NULL) {
            restore = TRUE;
            attrs->Backup();
            mTranslator->TranslateWindowLayout(attrs);
        }

        //if (DEBUG_LAYOUT) Log.d(TAG, "WindowLayout in setView:" + attrs);

        if (!supportsScreen) {
            mWindowAttributes->mFlags |= WindowManagerLayoutParams_FLAG_COMPATIBLE_WINDOW;
        }

        mSoftInputMode = mWindowAttributes->mSoftInputMode;
        mWindowAttributesChanged = TRUE;
        mAttachInfo->mRootView = view;
        mAttachInfo->mScalingRequired = mTranslator != NULL;
        mAttachInfo->mApplicationScale = (mTranslator == NULL)
            ? 1.0f : ((CompatibilityInfo::Translator*)mTranslator.Get())->mApplicationScale;
        if (panelParentView != NULL) {
            panelParentView->GetApplicationWindowToken(
                (IBinder**)&mAttachInfo->mPanelParentWindowToken);
        }
        mAdded = TRUE;

        // Schedule the first layout -before- adding to the window
        // manager, to make sure we do the relayout before receiving
        // any other events from the system.
        //
        RequestLayout();

        assert(SUCCEEDED(CInputChannel::NewByFriend((CInputChannel**)&mInputChannel)));

        Int32 res;
        ECode ec = sWindowSession->Add(
            (IInnerWindow*)mWindow, (IWindowManagerLayoutParams*)mWindowAttributes,
            GetHostVisibility(),
            (IRect*)mAttachInfo->mContentInsets, (IRect**)&mAttachInfo->mContentInsets,
            (IInputChannel*)mInputChannel, (IInputChannel**)&mInputChannel,
            &res);

        if (FAILED(ec)) {
            mAdded = FALSE;
            mView = NULL;
            mAttachInfo->mRootView = NULL;
            mInputChannel = NULL;
            UnscheduleTraversals();

            if (restore) {
                mWindowAttributes->Restore();
            }

            //throw new RuntimeException("Adding window failed", e);
            return E_RUNTIME_EXCEPTION;
        }

        if (restore) {
            mWindowAttributes->Restore();
        }

        if (mTranslator != NULL) {
            mTranslator->TranslateRectInScreenToAppWindow(mAttachInfo->mContentInsets);
        }

        mPendingContentInsets->SetEx((IRect*)mAttachInfo->mContentInsets);
        mPendingVisibleInsets->Set(0, 0, 0, 0);

        //if (Config.LOGV) Log.v(TAG, "Added window " + mWindow);

        if (res < WindowManagerImpl_ADD_OKAY) {
            mView = NULL;
            mAttachInfo->mRootView = NULL;
            mAdded = FALSE;
            UnscheduleTraversals();

            switch (res) {
                case WindowManagerImpl_ADD_BAD_APP_TOKEN:
                case WindowManagerImpl_ADD_BAD_SUBWINDOW_TOKEN:
//                    throw new WindowManagerImpl.BadTokenException(
//                                "Unable to add window -- token " + attrs.token
//                                + " is not valid; is your activity running?");
                    Logger::E(TAG,
                        StringBuffer("Unable to add window -- token ") +
                        //mWindowAttributes->mToken +
                        "  is not valid; is your activity running?");

                    return E_BAD_TOKEN_EXCEPTION;

                case WindowManagerImpl_ADD_NOT_APP_TOKEN:
//                     throw new WindowManagerImpl.BadTokenException(
//                                "Unable to add window -- token " + attrs.token
//                                + " is not for an application");
                    Logger::E(TAG,
                        StringBuffer("Unable to add window -- token ") +
                        //mWindowAttributes->mToken +
                        "  is not for an application");

                    return E_BAD_TOKEN_EXCEPTION;

                case WindowManagerImpl_ADD_APP_EXITING:
//                     throw new WindowManagerImpl.BadTokenException(
//                                "Unable to add window -- app for token " + attrs.token
//                                + " is exiting");
                    Logger::E(TAG,
                        StringBuffer("Unable to add window -- app for token ") +
                        /*mWindowAttributes->mToken +*/ "  is exiting?");

                    return E_BAD_TOKEN_EXCEPTION;

                case WindowManagerImpl_ADD_DUPLICATE_ADD:
//                    throw new WindowManagerImpl.BadTokenException(
//                                "Unable to add window -- window " + mWindow
//                                + " has already been added");
                    Logger::E(TAG,
                        StringBuffer("Unable to add window -- window ") +
                        /*mWindow +*/ "  has already been added");

                    return E_BAD_TOKEN_EXCEPTION;

                case WindowManagerImpl_ADD_STARTING_NOT_NEEDED:
                    // Silently ignore -- we would have just removed it
                    // right away, anyway.
                    return NOERROR;

                case WindowManagerImpl_ADD_MULTIPLE_SINGLETON:
//                    throw new WindowManagerImpl.BadTokenException(
//                                "Unable to add window " + mWindow +
//                                " -- another window of this type already exists");
                    Logger::E(TAG,
                        StringBuffer("Unable to add window ") + //mWindow +
                        " -- another window of this type already exists");

                    return E_BAD_TOKEN_EXCEPTION;

                case WindowManagerImpl_ADD_PERMISSION_DENIED:
//                    throw new WindowManagerImpl.BadTokenException(
//                                "Unable to add window " + mWindow +
//                                " -- permission denied for this window type");
                    Logger::E(TAG, StringBuffer("Unable to add window ") +
                        //mWindow +
                        " -- permission denied for this window type");

                    return E_BAD_TOKEN_EXCEPTION;

                default:
                    Logger::E(TAG,
                        StringBuffer("Unable to add window -- unknown error code ") +
                        res);

                    return E_RUNTIME_EXCEPTION;
            }
        }

        if (rootViewST) {
            rootViewST->WillYouTakeTheInputQueue((IInputQueueCallback**)&mInputQueueCallback);
        }

        if (mInputQueueCallback != NULL) {
            CInputQueue::New((CInputChannel*)mInputChannel.Get(), (IInputQueue**)&mInputQueue);
            mInputQueueCallback->OnInputQueueCreated(mInputQueue);
        }
        else {
            assert(SUCCEEDED(CInputQueue::RegisterInputChannel((CInputChannel*)mInputChannel.Get(),
                    mInputHandler, CApartment::GetNativeMessageQueue())));
        }

        view->AssignParent(this);
        mAddedTouchMode = (res & WindowManagerImpl_ADD_FLAG_IN_TOUCH_MODE) != 0;
        mAppVisible = (res & WindowManagerImpl_ADD_FLAG_APP_VISIBLE) != 0;
    }

    return NOERROR;
}

IView* ViewRoot::GetView()
{
    return mView;
}

void ViewRoot::SetLayoutParams(
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Boolean newView)
{
    Mutex::Autolock lock(mSyncLock);

    Int32 oldSoftInputMode = mWindowAttributes->mSoftInputMode;

    // preserve compatible window flag if exists.
    //
    Int32 compatibleWindowFlag =
        mWindowAttributes->mFlags & WindowManagerLayoutParams_FLAG_COMPATIBLE_WINDOW;

    Int32 changes;
    mWindowAttributes->CopyFrom(attrs, &changes);
    mWindowAttributes->mFlags |= compatibleWindowFlag;

    if (newView) {
        mSoftInputMode = mWindowAttributes->mSoftInputMode;
        RequestLayout();
    }

    // Don't lose the mode we last auto-computed.
    //
    if ((mWindowAttributes->mSoftInputMode & WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST)
        == WindowManagerLayoutParams_SOFT_INPUT_ADJUST_UNSPECIFIED) {
            mWindowAttributes->mSoftInputMode = (mWindowAttributes->mSoftInputMode
                & ~WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST)
                | (oldSoftInputMode
                & WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST);
    }

    mWindowAttributesChanged = TRUE;
    ScheduleTraversals();
}

void ViewRoot::HandleAppVisibility(
    /* [in] */ Boolean visible)
{
    if (mAppVisible != visible) {
        mAppVisible = visible;
        ScheduleTraversals();
    }
}

void ViewRoot::HandleGetNewSurface()
{
    mNewSurfaceNeeded = TRUE;
    mFullRedrawNeeded = TRUE;
    ScheduleTraversals();
}

ECode ViewRoot::RequestLayout()
{
    ECode ec = CheckThread();
    if (FAILED(ec)) {
        return ec;
    }

    mLayoutRequested = TRUE;
    ScheduleTraversals();

    return NOERROR;
}

ECode ViewRoot::RequestLayoutEx()
{
    return RequestLayout();
}

ECode ViewRoot::IsLayoutRequested(
    /* [out] */ Boolean* result)
{
    assert(result);
    *result = mLayoutRequested;

    return NOERROR;
}

ECode ViewRoot::IsLayoutRequestedEx(
    /* [out] */ Boolean* result)
{
    return IsLayoutRequested(result);
}

ECode ViewRoot::InvalidateChild(
    /* [in] */ IView* child,
    /* [in] */ IRect* dirty)
{
    ECode ec = CheckThread();
    if (FAILED(ec)) {
        return ec;
    }

    if (DEBUG_DRAW) {
        Logger::D(TAG, StringBuffer("Invalidate child: ") /*+ dirty*/);
    }

    if (mCurScrollY != 0 || mTranslator != NULL) {
        mTempRect->SetEx(dirty);
        dirty = mTempRect;

        if (mCurScrollY != 0) {
            dirty->Offset(0, -mCurScrollY);
        }

        if (mTranslator != NULL) {
            mTranslator->TranslateRectInAppWindowToScreen(dirty);
        }

        if (mAttachInfo->mScalingRequired) {
            dirty->Inset(-1, -1);
        }
    }

    mDirty->UnionEx(dirty);

    if (!mWillDrawSoon) {
        ScheduleTraversals();
    }

    return NOERROR;
}

ECode ViewRoot::GetParent(
    /* [out] */ IViewParent** parent)
{
    assert(parent);
    *parent = NULL;

    return NOERROR;
}

ECode ViewRoot::GetParentEx(
    /* [out] */ IViewParent** parent)
{
    return GetParent(parent);
}

ECode ViewRoot::InvalidateChildInParent(
    /* [in] */ ArrayOf<Int32>* location,
    /* [in] */ IRect* dirty,
    /* [out] */ IViewParent** parent)
{
    InvalidateChild(NULL, dirty);

    assert(parent);
    *parent = NULL;

    return NOERROR;
}

ECode ViewRoot::GetChildVisibleRect(
    /* [in] */ IView* child,
    /* [in] */ IRect* r,
    /* [in] */ IPoint* offset,
    /* [out] */ Boolean* result)
{
    if (child != mView) {
        Logger::W(TAG, "child is not mine, honest!");

        return E_RUNTIME_EXCEPTION;
    }

    // Note: don't apply scroll offset, because we want to know its
    // visibility in the virtual canvas being given to the view hierarchy.
    //
    return r->Intersect(0, 0, mWidth, mHeight, result);
}

ECode ViewRoot::BringChildToFront(
    /* [in] */ IView* child)
{
    return NOERROR;
}

void ViewRoot::ScheduleTraversals()
{
    if (!mTraversalScheduled) {
        mTraversalScheduled = TRUE;
        void (STDCALL ViewRoot::*pHandlerFunc)();
        pHandlerFunc = &ViewRoot::PerformTraversals;
        SendMessage(*(Handle32*)&pHandlerFunc, NULL);
    }
}

void ViewRoot::UnscheduleTraversals()
{
    if (mTraversalScheduled) {
        mTraversalScheduled = FALSE;
        void (STDCALL ViewRoot::*pHandlerFunc)();
        pHandlerFunc = &ViewRoot::PerformTraversals;
        RemoveMessage(*(Handle32*)&pHandlerFunc);
    }
}

Int32 ViewRoot::GetHostVisibility()
{
    assert(mView.Get());

    Int32 visibility = View_GONE;
    if (mAppVisible) {
        mView->GetVisibility(&visibility);
    }

    return visibility;
}

void ViewRoot::PerformTraversals()
{
    if (mProfile) {
        //Debug.startMethodTracing("ViewRoot");
    }

    // cache mView since it is used so much below...
    //
    View* host = mView != NULL ? (View*)mView->Probe(EIID_View) : NULL;

    if (DBG) {
        Logger::D(TAG, "======================================");
        Logger::D(TAG, "performTraversals");
        //host->Debug();
    }

    if (host == NULL || !mAdded) {
        return;
    }

    mTraversalScheduled = FALSE;
    mWillDrawSoon = TRUE;

    Boolean windowResizesToFitContent = FALSE;
    Boolean fullRedrawNeeded = mFullRedrawNeeded;
    Boolean newSurface = FALSE;
    Boolean surfaceChanged = FALSE;
    CWindowManagerLayoutParams* lp = mWindowAttributes;

    Int32 desiredWindowWidth;
    Int32 desiredWindowHeight;
    Int32 childWidthMeasureSpec;
    Int32 childHeightMeasureSpec;

    View::AttachInfo* attachInfo = mAttachInfo;

    const Int32 viewVisibility = GetHostVisibility();
    Boolean viewVisibilityChanged = mViewVisibility != viewVisibility
        || mNewSurfaceNeeded;

    //Float appScale = mAttachInfo->mApplicationScale;

    CWindowManagerLayoutParams* params = NULL;
    if (mWindowAttributesChanged) {
        mWindowAttributesChanged = FALSE;
        surfaceChanged = TRUE;
        params = lp;
    }

    CRect* frame = mWinFrame;

    if (mFirst) {
        fullRedrawNeeded = TRUE;
        mLayoutRequested = TRUE;

        AutoPtr<IContext> ctx;
        mView->GetContext((IContext**)&ctx);
        assert(ctx);

        AutoPtr<IResources> resources;
        ctx->GetResources((IResources**)&resources);
        assert(resources);

        AutoPtr<IDisplayMetrics> packageMetrics;
        resources->GetDisplayMetrics((IDisplayMetrics**)&packageMetrics);
        assert(packageMetrics);

        desiredWindowWidth = ((CDisplayMetrics*)packageMetrics.Get())->mWidthPixels;
        desiredWindowHeight = ((CDisplayMetrics*)packageMetrics.Get())->mHeightPixels;

        // For the very first time, tell the view hierarchy that it
        // is attached to the window.  Note that at this point the surface
        // object is not initialized to its backing store, but soon it
        // will be (assuming the window is visible).
        //
        attachInfo->mSurface = mSurface;
        attachInfo->mUse32BitDrawingCache = ElPixelFormat::FormatHasAlpha(lp->mFormat) ||
            lp->mFormat == ElPixelFormat::RGBX_8888;
        attachInfo->mHasWindowFocus = FALSE;
        attachInfo->mWindowVisibility = viewVisibility;
        attachInfo->mRecomputeGlobalAttributes = FALSE;
        attachInfo->mKeepScreenOn = FALSE;
        viewVisibilityChanged = FALSE;

        AutoPtr<IConfiguration> config;
        resources->GetConfiguration((IConfiguration**)&config);
        mLastConfiguration->SetTo(config);

        host->DispatchAttachedToWindow(attachInfo, 0);
        Logger::I(TAG, StringBuffer("Screen on initialized: ")
            + attachInfo->mKeepScreenOn);
    }
    else {
        desiredWindowWidth = frame->GetWidth();
        desiredWindowHeight = frame->GetHeight();
        if (desiredWindowWidth != mWidth || desiredWindowHeight != mHeight) {
            if (DEBUG_ORIENTATION) {
                Logger::V(TAG, StringBuffer("View ")
                    + (Int32)host + " resized to: " + (Int32)frame);
            }

            fullRedrawNeeded = TRUE;
            mLayoutRequested = TRUE;
            windowResizesToFitContent = TRUE;
        }
    }

    if (viewVisibilityChanged) {
        attachInfo->mWindowVisibility = viewVisibility;
        host->DispatchWindowVisibilityChanged(viewVisibility);

        if (viewVisibility != View_VISIBLE || mNewSurfaceNeeded) {
            if (mUseGL) {
                DestroyGL();
            }
        }

        if (viewVisibility == View_GONE) {
            // After making a window gone, we will count it as being
            // shown for the first time the next time it gets focus.
            //
            mHasHadWindowFocus = FALSE;
        }
    }

    Boolean insetsChanged = FALSE;

    if (mLayoutRequested) {
        // Execute enqueued actions on every layout in case a view that was detached
        // enqueued an action after being detached
        //
        GetRunQueue()->ExecuteActions(attachInfo->mHandler);

        if (mFirst) {
            host->FitSystemWindows(mAttachInfo->mContentInsets);

            // make sure touch mode code executes by setting cached value
            // to opposite of the added touch mode.
            //
            mAttachInfo->mInTouchMode = !mAddedTouchMode;
            EnsureTouchModeLocally(mAddedTouchMode);
        }
        else {
            if (!mAttachInfo->mContentInsets->Equals(mPendingContentInsets)) {
                mAttachInfo->mContentInsets->SetEx(mPendingContentInsets);
                host->FitSystemWindows(mAttachInfo->mContentInsets);
                insetsChanged = TRUE;

                if (DEBUG_LAYOUT) {
                    Logger::D(TAG, "Content insets changing to: "
                        /*+ mAttachInfo->mContentInsets*/);
                }
            }

            if (!mAttachInfo->mVisibleInsets->Equals(mPendingVisibleInsets)) {
                mAttachInfo->mVisibleInsets->SetEx(mPendingVisibleInsets);

                if (DEBUG_LAYOUT) {
                    Logger::D(TAG, "Visible insets changing to: "
                        /*+ mAttachInfo->mVisibleInsets*/);
                }
            }

            if (lp->mWidth == ViewGroupLayoutParams_WRAP_CONTENT
                || lp->mHeight == ViewGroupLayoutParams_WRAP_CONTENT) {
                windowResizesToFitContent = TRUE;

                AutoPtr<IContext> ctx;
                mView->GetContext((IContext**)&ctx);
                assert(ctx);

                AutoPtr<IResources> resources;
                ctx->GetResources((IResources**)&resources);
                assert(resources);

                AutoPtr<IDisplayMetrics> packageMetrics;
                resources->GetDisplayMetrics((IDisplayMetrics**)&packageMetrics);
                assert(packageMetrics);

                desiredWindowWidth =
                    ((CDisplayMetrics*)packageMetrics.Get())->mWidthPixels;
                desiredWindowHeight =
                    ((CDisplayMetrics*)packageMetrics.Get())->mHeightPixels;
            }
        }

        childWidthMeasureSpec = GetRootMeasureSpec(desiredWindowWidth, lp->mWidth);
        childHeightMeasureSpec = GetRootMeasureSpec(desiredWindowHeight, lp->mHeight);

        // Ask host how big it wants to be
        //
        if (DEBUG_ORIENTATION || DEBUG_LAYOUT) {
            //Logger::D(TAG,
            //"Measuring " + host + " in display " + desiredWindowWidth
            //+ "x" + desiredWindowHeight + "...");
        }

        host->Measure(childWidthMeasureSpec, childHeightMeasureSpec);

        if (DBG) {
            Logger::D(TAG, "======================================");
            Logger::D(TAG, "performTraversals -- after measure");
            host->Debug();
        }
    }

    if (attachInfo->mRecomputeGlobalAttributes) {
        ////Log.i(TAG, "Computing screen on!");
        attachInfo->mRecomputeGlobalAttributes = FALSE;
        Boolean oldVal = attachInfo->mKeepScreenOn;
        attachInfo->mKeepScreenOn = FALSE;
        host->DispatchCollectViewAttributes(0);
        if (attachInfo->mKeepScreenOn != oldVal) {
            params = lp;
            ////Log.i(TAG, "Keep screen on changed: " + attachInfo->mKeepScreenOn);
        }
    }

    if (mFirst || attachInfo->mViewVisibilityChanged) {
        attachInfo->mViewVisibilityChanged = FALSE;
        Int32 resizeMode = mSoftInputMode &
            WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST;

        // If we are in auto resize mode, then we need to determine
        // what mode to use now.
        //
        if (resizeMode == WindowManagerLayoutParams_SOFT_INPUT_ADJUST_UNSPECIFIED) {
            List<AutoPtr<IView> >::Iterator iter = attachInfo->mScrollContainers.Begin();
            for (; iter!=attachInfo->mScrollContainers.End(); ++iter) {

                if (((View*)(*iter)->Probe(EIID_View))->IsShown()) {
                    resizeMode = WindowManagerLayoutParams_SOFT_INPUT_ADJUST_RESIZE;
                }
            }

            if (resizeMode == 0) {
                resizeMode = WindowManagerLayoutParams_SOFT_INPUT_ADJUST_PAN;
            }

            if ((lp->mSoftInputMode &
                WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST) != resizeMode) {
                    lp->mSoftInputMode = (lp->mSoftInputMode &
                        ~WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST) |
                        resizeMode;
                params = lp;
            }
        }
    }

    if (params != NULL && (host->mPrivateFlags & View::REQUEST_TRANSPARENT_REGIONS) != 0) {
        if (!ElPixelFormat::FormatHasAlpha(params->mFormat)) {
            params->mFormat = ElPixelFormat::TRANSLUCENT;
        }
    }

    Boolean windowShouldResize = mLayoutRequested && windowResizesToFitContent
        && ((mWidth != host->mMeasuredWidth || mHeight != host->mMeasuredHeight)
        || (lp->mWidth == ViewGroupLayoutParams_WRAP_CONTENT &&
        frame->GetWidth() < desiredWindowWidth && frame->GetWidth() != mWidth)
        || (lp->mHeight == ViewGroupLayoutParams_WRAP_CONTENT &&
        frame->GetHeight() < desiredWindowHeight && frame->GetHeight() != mHeight));

    const Boolean computesInternalInsets = FALSE;
        //attachInfo->mTreeObserver->HasComputeInternalInsetsListeners();
    Boolean insetsPending = FALSE;
    Int32 relayoutResult = 0;

    if (mFirst || windowShouldResize || insetsChanged
        || viewVisibilityChanged || params != NULL) {
        if (viewVisibility == View_VISIBLE) {
            // If this window is giving internal insets to the window
            // manager, and it is being added or changing its visibility,
            // then we want to first give the window manager "fake"
            // insets to cause it to effectively ignore the content of
            // the window during layout.  This avoids it briefly causing
            // other windows to resize/move based on the raw frame of the
            // window, waiting until we can finish laying out this window
            // and get back to the window manager with the ultimately
            // computed insets.
            //
            insetsPending = computesInternalInsets
                && (mFirst || viewVisibilityChanged);

            if (mWindowAttributes->mMemoryType
                == WindowManagerLayoutParams_MEMORY_TYPE_GPU) {
                if (params == NULL) {
                    params = mWindowAttributes;
                }
                mGlWanted = TRUE;
            }
        }

        if (mSurfaceHolder != NULL) {
            mSurfaceHolder->mSurfaceLock.Lock();
            mDrawingAllowed = TRUE;
        }

        Boolean initialized = FALSE;
        Boolean contentInsetsChanged = FALSE;
        Boolean visibleInsetsChanged;
        Boolean hadSurface = FALSE;
        mSurface->IsValid(&hadSurface);

        Int32 fl = 0;
        if (params != NULL) {
            fl = params->mFlags;
            if (attachInfo->mKeepScreenOn) {
                params->mFlags |= WindowManagerLayoutParams_FLAG_KEEP_SCREEN_ON;
            }
        }

        //if (DEBUG_LAYOUT) {
        //    Logger::D(TAG, "host=w:" + host->mMeasuredWidth + ", h:" +
        //        host->mMeasuredHeight + ", params=" + params);
        //}

        relayoutResult = RelayoutWindow(params, viewVisibility, insetsPending);

        if (params != NULL) {
            params->mFlags = fl;
        }

        //if (DEBUG_LAYOUT) Log.v(TAG, "relayout: frame=" + frame->toShortString()
        //    + " content=" + mPendingContentInsets.toShortString()
        //    + " visible=" + mPendingVisibleInsets.toShortString()
        //    + " surface=" + mSurface);

        if (mPendingConfiguration->mSeq != 0) {
            if (DEBUG_CONFIGURATION) {
                Logger::D(TAG, "Visible with new config: "
                    /*+ mPendingConfiguration*/);
            }
            UpdateConfiguration(mPendingConfiguration, !mFirst);
            mPendingConfiguration->mSeq = 0;
        }

        contentInsetsChanged = !mPendingContentInsets->Equals(
            mAttachInfo->mContentInsets);
        visibleInsetsChanged = !mPendingVisibleInsets->Equals(
            mAttachInfo->mVisibleInsets);

        if (contentInsetsChanged) {
            mAttachInfo->mContentInsets->SetEx(mPendingContentInsets);
            host->FitSystemWindows(mAttachInfo->mContentInsets);

            if (DEBUG_LAYOUT) {
                Logger::D(TAG, "Content insets changing to: "
                    /*+ mAttachInfo->mContentInsets*/);
            }
        }

        if (visibleInsetsChanged) {
            mAttachInfo->mVisibleInsets->SetEx(mPendingVisibleInsets);

            if (DEBUG_LAYOUT) {
                Logger::D(TAG, "Visible insets changing to: "
                    /*+ mAttachInfo->mVisibleInsets*/);
            }
        }

        Boolean surfaceValid = FALSE;
        mSurface->IsValid(&surfaceValid);
        if (!hadSurface) {
            if (surfaceValid) {
                // If we are creating a new surface, then we need to
                // completely redraw it.  Also, when we get to the
                // point of drawing it we will hold off and schedule
                // a new traversal instead.  This is so we can tell the
                // window manager about all of the windows being displayed
                // before actually drawing them, so it can display then
                // all at once.
                //
                newSurface = TRUE;
                fullRedrawNeeded = TRUE;
                mPreviousTransparentRegion->SetEmpty();

                if (mGlWanted && !mUseGL) {
                    InitializeGL();
                    //initialized = mGlCanvas != NULL;
                }
            }
        }
        else if (!surfaceValid) {
            // If the surface has been removed, then reset the scroll
            // positions.
            mLastScrolledFocus = NULL;
            mScrollY = mCurScrollY = 0;
            if (mScroller != NULL) {
                mScroller->AbortAnimation();
            }
        }

        if (DEBUG_ORIENTATION) {
            Logger::V(TAG, "Relayout returned: frame="
                /*+ frame + ", surface=" + mSurface*/);
        }

        attachInfo->mWindowLeft = frame->mLeft;
        attachInfo->mWindowTop = frame->mTop;

        // !!FIXME!! This next section handles the case where we did not get the
        // window size we asked for. We should avoid this by getting a maximum size from
        // the window session beforehand.
        //
        mWidth = frame->GetWidth();
        mHeight = frame->GetHeight();

        if (mSurfaceHolder != NULL) {
            // The app owns the surface; tell it about what is going on.
            Boolean isValid = FALSE;
            mSurface->IsValid(&isValid);
            if (isValid) {
                mSurfaceHolder->mSurface = mSurface;
            }

            mSurfaceHolder->mSurfaceLock.Unlock();
            if (isValid) {
                if (!hadSurface) {
                    mSurfaceHolder->UngetCallbacks();

                    mIsCreating = TRUE;
                    mSurfaceHolderCallback->SurfaceCreated(mSurfaceHolder);
                    Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks =
                        mSurfaceHolder->GetCallbacks();
                    Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                    for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                        (*iter).Get()->SurfaceCreated(mSurfaceHolder);
                    }
                    surfaceChanged = TRUE;
                }

                if (surfaceChanged) {
                    mSurfaceHolderCallback->SurfaceChanged(mSurfaceHolder,
                        lp->mFormat, mWidth, mHeight);

                    Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks =
                        mSurfaceHolder->GetCallbacks();
                    Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                    for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                        (*iter).Get()->SurfaceChanged(
                            mSurfaceHolder, lp->mFormat, mWidth, mHeight);
                    }
                }
                mIsCreating = FALSE;
            }
            else if (hadSurface) {
                mSurfaceHolder->UngetCallbacks();

                mSurfaceHolderCallback->SurfaceDestroyed(mSurfaceHolder);

                Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks =
                    mSurfaceHolder->GetCallbacks();
                Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                    (*iter).Get()->SurfaceDestroyed(mSurfaceHolder);
                }

                mSurfaceHolder->mSurfaceLock.Lock();
                mSurfaceHolder->mSurface = NULL;
                CSurface::New((ISurface**)&(mSurfaceHolder->mSurface));
                mSurfaceHolder->mSurfaceLock.Unlock();
            }
        }

        if (initialized) {
            //mGlCanvas.setViewport((Int32) (mWidth * appScale + 0.5f),
            //    (Int32) (mHeight * appScale + 0.5f));
        }

        Boolean focusChangedDueToTouchMode = EnsureTouchModeLocally(
            (relayoutResult & WindowManagerImpl_RELAYOUT_IN_TOUCH_MODE) != 0);
        if (focusChangedDueToTouchMode || mWidth != host->mMeasuredWidth
            || mHeight != host->mMeasuredHeight || contentInsetsChanged) {
                childWidthMeasureSpec = GetRootMeasureSpec(mWidth, lp->mWidth);
                childHeightMeasureSpec = GetRootMeasureSpec(mHeight, lp->mHeight);

                if (DEBUG_LAYOUT) {
                    Logger::D(TAG, StringBuffer("Ooops, something changed!  mWidth=")
                        + mWidth + " measuredWidth=" + host->mMeasuredWidth
                        + " mHeight=" + mHeight
                        + " measuredHeight" + host->mMeasuredHeight
                        + " coveredInsetsChanged=" + contentInsetsChanged);
                }

                // Ask host how big it wants to be
                //
                host->Measure(childWidthMeasureSpec, childHeightMeasureSpec);

                // Implementation of weights from WindowManager.LayoutParams
                // We just grow the dimensions as needed and re-measure if
                // needs be
                //
                Int32 width = host->mMeasuredWidth;
                Int32 height = host->mMeasuredHeight;
                Boolean measureAgain = FALSE;

                if (lp->mHorizontalWeight > 0.0f) {
                    width += (Int32) ((mWidth - width) * lp->mHorizontalWeight);
                    childWidthMeasureSpec = View::MeasureSpec::MakeMeasureSpec(
                        width, View::MeasureSpec::EXACTLY);
                    measureAgain = TRUE;
                }
                if (lp->mVerticalWeight > 0.0f) {
                    height += (Int32) ((mHeight - height) * lp->mVerticalWeight);
                    childHeightMeasureSpec = View::MeasureSpec::MakeMeasureSpec(
                        height, View::MeasureSpec::EXACTLY);
                    measureAgain = TRUE;
                }

                if (measureAgain) {
                    if (DEBUG_LAYOUT) {
                        Logger::D(TAG,
                            StringBuffer("And hey let's measure once more: width=")
                            + width + " height=" + height);
                    }
                    host->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
                }

                mLayoutRequested = TRUE;
        }
    }

    const Boolean didLayout = mLayoutRequested;
    Boolean triggerGlobalLayoutListener = didLayout
        || attachInfo->mRecomputeGlobalAttributes;
    if (didLayout) {
        mLayoutRequested = FALSE;
        mScrollMayChange = TRUE;

        //if (DEBUG_ORIENTATION || DEBUG_LAYOUT) Log.v(
        //    TAG, "Laying out " + host + " to (" +
        //    host->mMeasuredWidth + ", " + host->mMeasuredHeight + ")");

        //Int64 startTime = 0;
        //if (Config.DEBUG && ViewDebug.profileLayout) {
        //    startTime = SystemClock.elapsedRealtime();
        //}

        host->Layout(0, 0, host->mMeasuredWidth, host->mMeasuredHeight);

        //if (Config.DEBUG && ViewDebug.consistencyCheckEnabled) {
        //    if (!host->dispatchConsistencyCheck(ViewDebug.CONSISTENCY_LAYOUT)) {
        //        throw new IllegalStateException("The view hierarchy is an inconsistent state,"
        //            + "please refer to the logs with the tag "
        //            + ViewDebug.CONSISTENCY_LOG_TAG + " for more infomation.");
        //    }
        //}

        //if (Config.DEBUG && ViewDebug.profileLayout) {
        //    EventLog.writeEvent(60001, SystemClock.elapsedRealtime() - startTime);
        //}

        // By this point all views have been sized and positionned
        // We can compute the transparent area

        if ((host->mPrivateFlags & View::REQUEST_TRANSPARENT_REGIONS) != 0) {
            // start out transparent
            // TODO: AVOID THAT CALL BY CACHING THE RESULT?
            //
            host->GetLocationInWindow(mTmpLocation, mTmpLocation + 1);
            Boolean result;
            mTransparentRegion->SetEx2(mTmpLocation[0], mTmpLocation[1],
                mTmpLocation[0] + host->mRight - host->mLeft,
                mTmpLocation[1] + host->mBottom - host->mTop, &result);

            host->GatherTransparentRegion(mTransparentRegion);
            if (mTranslator != NULL) {
                mTranslator->TranslateRegionInWindowToScreen(mTransparentRegion);
            }

            Boolean isEqual;
            mTransparentRegion->Equals(mPreviousTransparentRegion, &isEqual);
            if (!isEqual) {
                mPreviousTransparentRegion->Set(mTransparentRegion, &result);
                // reconfigure window manager
                sWindowSession->SetTransparentRegion(mWindow, mTransparentRegion);
            }
        }

        if (DBG) {
            Logger::D(TAG, "======================================");
            Logger::D(TAG, "performTraversals -- after setFrame");
            host->Debug();
        }
    }

    if (triggerGlobalLayoutListener) {
        attachInfo->mRecomputeGlobalAttributes = FALSE;
        //attachInfo->mTreeObserver->DispatchOnGlobalLayout();
    }

    if (computesInternalInsets) {
        ViewTreeObserver::InternalInsetsInfo* insets = attachInfo->mGivenInternalInsets;
        attachInfo->mGivenInternalInsets->mContentInsets->Set(0, 0, 0, 0);
        attachInfo->mGivenInternalInsets->mVisibleInsets->Set(0, 0, 0, 0);

        //attachInfo->mTreeObserver.dispatchOnComputeInternalInsets(insets);

        AutoPtr<IRect> contentInsets = insets->mContentInsets;
        AutoPtr<IRect> visibleInsets = insets->mVisibleInsets;
        if (mTranslator != NULL) {
            contentInsets = NULL;
            mTranslator->GetTranslatedContentInsets(
                insets->mContentInsets, (IRect**)&contentInsets);
            visibleInsets = NULL;
            mTranslator->GetTranslatedVisbileInsets(
                insets->mVisibleInsets, (IRect**)&visibleInsets);
        }

        Boolean equal = FALSE;
        mLastGivenInsets->Equals(insets, &equal);
        if (insetsPending || !equal) {
            mLastGivenInsets->Set(insets);
            //try {
            Int32 tmpInset = 0;
            insets->GetTouchableInsets(&tmpInset);

            sWindowSession->SetInsets(
                mWindow, tmpInset,
                contentInsets, visibleInsets);
            //} catch (RemoteException e) {
            //}
        }
    }

    if (mFirst) {
        // handle first focus request
        //if (DEBUG_INPUT_RESIZE) {
        //    Logger::D(TAG, StringBuffer("First: mView.hasFocus()=")
        //    + mView->HasFocus());
        //}

        if (mView != NULL) {
            Boolean hasFocus;
            mView->HasFocus(&hasFocus);
            if (!hasFocus) {
                Boolean result;
                mView->RequestFocusEx(View::FOCUS_FORWARD, &result);
                AutoPtr<IView> temp;
                mView->FindFocus((IView**)&temp);
                mRealFocusedView = temp;
                mFocusedView = mRealFocusedView;
                if (DEBUG_INPUT_RESIZE) {
                    Logger::D(TAG, StringBuffer("First: requested focused view=")
                        + (Int32)mFocusedView.Get());
                }
            }
            else {
                AutoPtr<IView> temp;
                mView->FindFocus((IView**)&temp);
                mRealFocusedView = temp;
                if (DEBUG_INPUT_RESIZE) {
                    Logger::D(TAG, StringBuffer("First: existing focused view=")
                        + (Int32)mRealFocusedView.Get());
                }
            }
        }
    }

    mFirst = FALSE;
    mWillDrawSoon = FALSE;
    mNewSurfaceNeeded = FALSE;
    mViewVisibility = viewVisibility;
    if (mAttachInfo->mHasWindowFocus) {
        const Boolean imTarget =
            CWindowManagerLayoutParams::MayUseInputMethod(mWindowAttributes->mFlags);
        if (imTarget != mLastWasImTarget) {
            mLastWasImTarget = imTarget;
            AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
            if (imm != NULL && imTarget) {
               imm->StartGettingWindowFocus(mView);
               AutoPtr<IView> focView;
               mView->FindFocus((IView**) &focView);
               imm->OnWindowFocus(mView, focView,
                   mWindowAttributes->mSoftInputMode,
                   !mHasHadWindowFocus, mWindowAttributes->mFlags);
            }
        }
    }

    Boolean cancelDraw = FALSE;
    attachInfo->mTreeObserver->DispatchOnPreDraw(&cancelDraw);

    if (!cancelDraw && !newSurface) {
        mFullRedrawNeeded = FALSE;
        Draw(fullRedrawNeeded);

        if ((relayoutResult & WindowManagerImpl_RELAYOUT_FIRST_TIME) != 0
            || mReportNextDraw) {
            if (LOCAL_LOGV) {
                Logger::D(TAG, StringBuffer("FINISHED DRAWING: ")
                    /*+ mWindowAttributes->mTitle*/);
            }

            mReportNextDraw = FALSE;
            if (mSurfaceHolder != NULL) {
                Boolean isValid = FALSE;
                mSurface->IsValid(&isValid);
                if (isValid) {
                    mSurfaceHolderCallback->SurfaceRedrawNeeded(mSurfaceHolder);

                    Vector<AutoPtr<ISurfaceHolderCallback> > callbacks =
                        mSurfaceHolder->GetCallbacks();
                    Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                    for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                        ISurfaceHolderCallback2* callback2 =
                            (ISurfaceHolderCallback2*)(*iter).Get()->Probe(
                            EIID_ISurfaceHolderCallback2);
                        if (callback2) {
                            callback2->SurfaceRedrawNeeded(mSurfaceHolder);
                        }
                    }
                }
            }

            //try {
                sWindowSession->FinishDrawing(mWindow);
            //} catch (RemoteException e) {
            //}
        }
    }
    else {
        // We were supposed to report when we are done drawing. Since we canceled the
        // draw, remember it here.
        if ((relayoutResult & WindowManagerImpl_RELAYOUT_FIRST_TIME) != 0) {
            mReportNextDraw = TRUE;
        }
        if (fullRedrawNeeded) {
            mFullRedrawNeeded = TRUE;
        }
        // Try again
        ScheduleTraversals();
    }

    if (mProfile) {
        //Debug.stopMethodTracing();
        mProfile = FALSE;
    }
}

void ViewRoot::TestSurface()
{
    printf("=====%s, %d=====\n", __FILE__, __LINE__);
    AutoPtr<ICanvas> canvas;
    CRect * rect = NULL;
    ECode ec = CRect::NewByFriend(0, 0, 320, 480, &rect);
    printf("=====%s, %d=====ec=%p\n", __FILE__, __LINE__, ec);
//      assert(CRect::NewByFriend(0, 0, 320, 480, &rect));
//      assert(SUCCEEDED(mSurface->LockCanvas(NULL, (ICanvas**)&canvas))); // ok
    assert(SUCCEEDED(mSurface->LockCanvas((IRect*)rect, (ICanvas**)&canvas))); // ok
    assert(SUCCEEDED(canvas->DrawColor(0))); // ok
//      ec = canvas->DrawColorWithMode(0, 0);
//      assert(SUCCEEDED(canvas->DrawColorWithMode(0, 0)));
    Int32 saveCount = 0;
    ec = canvas->SaveEx(CCanvas::MATRIX_SAVE_FLAG, &saveCount); // ok
    printf("=====%s, %d=====ec=%p\n", __FILE__, __LINE__, ec);
//      assert(SUCCEEDED(canvas->SaveWithFlags(CCanvas::MATRIX_SAVE_FLAG, &saveCount)));

    AutoPtr<IPaint> paint;
    assert(SUCCEEDED(CPaint::New((IPaint**)&paint))); // ok
    assert(SUCCEEDED(paint->SetColor(0xFFFFFFFF))); // ok
    assert(SUCCEEDED(canvas->DrawLine(0, 0, 240, 320, paint.Get()))); // ok
    assert(SUCCEEDED(canvas->DrawLine(240, 0, 0, 320, paint.Get()))); // ok
    ec = canvas->RestoreToCount(saveCount); // ok
    printf("=====%s, %d=====ec=%p\n", __FILE__, __LINE__, ec);
//      assert(SUCCEEDED(canvas->RestoreToCount(saveCount)));
    assert(SUCCEEDED(mSurface->UnlockCanvasAndPost(canvas))); // ok
}

void ViewRoot::TestCanvas(ICanvas* canvas)
{
    AutoPtr<IPaint> paint;
    assert(SUCCEEDED(CPaint::New((IPaint**)&paint))); // ok
    assert(SUCCEEDED(paint->SetColor(0xFFFFFFFF))); // ok
    assert(SUCCEEDED(canvas->DrawLine(0, 0, 240, 320, paint.Get()))); // ok
    assert(SUCCEEDED(canvas->DrawLine(240, 0, 0, 320, paint.Get()))); // ok
}

ECode ViewRoot::RequestTransparentRegion(
    /* [in] */ IView* child)
{
    // the test below should not fail unless someone is messing with us
    //
    ECode ec = CheckThread();
    if (FAILED(ec)) {
        return ec;
    }

    if (mView.Get() == child) {
        (reinterpret_cast<View*>(mView->Probe(EIID_View)))->mPrivateFlags
            |= View::REQUEST_TRANSPARENT_REGIONS;

        // Need to make sure we re-evaluate the window attributes next
        // time around, to ensure the window has the correct format.
        //
        mWindowAttributesChanged = TRUE;

        RequestLayout();
    }

    return NOERROR;
}

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
Int32 ViewRoot::GetRootMeasureSpec(
    /* [in] */ Int32 windowSize,
    /* [in] */ Int32 rootDimension)
{
    Int32 measureSpec;
    switch (rootDimension) {
        case ViewGroupLayoutParams_MATCH_PARENT:
            // Window can't resize. Force root view to be windowSize.
            measureSpec = View::MeasureSpec::MakeMeasureSpec(
                windowSize, View::MeasureSpec::EXACTLY);
            break;
        case ViewGroupLayoutParams_WRAP_CONTENT:
            // Window can resize. Set max size for root view.
            measureSpec = View::MeasureSpec::MakeMeasureSpec(
                windowSize, View::MeasureSpec::AT_MOST);
            break;
        default:
            // Window wants to be an exact size. Force root view to be that size.
            measureSpec = View::MeasureSpec::MakeMeasureSpec(
                rootDimension, View::MeasureSpec::EXACTLY);
            break;
    }

    return measureSpec;
}

void ViewRoot::Draw(
    /* [in] */ Boolean fullRedrawNeeded)
{
    CSurface* surface = mSurface;
    Boolean surfaceValid = FALSE;
    surface->IsValid(&surfaceValid);

    if (surface == NULL || !surfaceValid) {
        return;
    }

    if (!sFirstDrawComplete) {
        Mutex::Autolock lock(sFirstDrawHandlersLock);

        sFirstDrawComplete = TRUE;

        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

        List<AutoPtr<IRunnable> >::Iterator iter = sFirstDrawHandlers.Begin();
        for (; iter != sFirstDrawHandlers.End(); ++iter) {
            mApartment->PostCppCallback(
                (Handle32)iter->Get(), *(Handle32*)&pHandlerFunc, NULL, 0);
        }
    }

    ScrollToRectOrFocus(NULL, FALSE);

    if (mAttachInfo->mViewScrollChanged) {
        mAttachInfo->mViewScrollChanged = FALSE;
        //mAttachInfo->mTreeObserver.dispatchOnScrollChanged();
    }

    Int32 yoff;
    const Boolean scrolling = mScroller != NULL && mScroller->ComputeScrollOffset();

    if (scrolling) {
        yoff = mScroller->GetCurrY();
    }
    else {
        yoff = mScrollY;
    }

    if (mCurScrollY != yoff) {
        mCurScrollY = yoff;
        fullRedrawNeeded = TRUE;
    }

    Float appScale = mAttachInfo->mApplicationScale;
    Boolean scalingRequired = mAttachInfo->mScalingRequired;

    CRect* dirty = mDirty;
    if (mSurfaceHolder != NULL) {
        // The app owns the surface, we won't draw.
        //
        dirty->SetEmpty();
        return;
    }

    if (mUseGL) {
        //if (!dirty->IsEmpty()) {
        //    Canvas canvas = mGlCanvas;
        //    if (mGL != NULL && canvas != NULL) {
        //        mGL.glDisable(GL_SCISSOR_TEST);
        //        mGL.glClearColor(0, 0, 0, 0);
        //        mGL.glClear(GL_COLOR_BUFFER_BIT);
        //        mGL.glEnable(GL_SCISSOR_TEST);

        //        mAttachInfo->mDrawingTime = SystemClock.uptimeMillis();
        //        mAttachInfo->mIgnoreDirtyState = TRUE;
        //        mView.mPrivateFlags |= View.DRAWN;

        //        Int32 saveCount = canvas.save(Canvas.MATRIX_SAVE_FLAG);
        //        try {
        //            canvas.translate(0, -yoff);
        //            if (mTranslator != NULL) {
        //                mTranslator->TranslateCanvas(canvas);
        //            }
        //            canvas.setScreenDensity(scalingRequired
        //                ? DisplayMetrics.DENSITY_DEVICE : 0);
        //            mView.draw(canvas);
        //            if (Config.DEBUG && ViewDebug.consistencyCheckEnabled) {
        //                mView.dispatchConsistencyCheck(ViewDebug.CONSISTENCY_DRAWING);
        //            }
        //        } finally {
        //            canvas.restoreToCount(saveCount);
        //        }

        //        mAttachInfo->mIgnoreDirtyState = FALSE;

        //        mEgl.eglSwapBuffers(mEglDisplay, mEglSurface);
        //        checkEglErrors();

        //        if (SHOW_FPS || Config.DEBUG && ViewDebug.showFps) {
        //            Int32 now = (Int32)SystemClock.elapsedRealtime();
        //            if (sDrawTime != 0) {
        //                nativeShowFPS(canvas, now - sDrawTime);
        //            }
        //            sDrawTime = now;
        //        }
        //    }
        //}

        if (scrolling) {
            mFullRedrawNeeded = TRUE;
            ScheduleTraversals();
        }
        return;
    }

    if (fullRedrawNeeded) {
        mAttachInfo->mIgnoreDirtyState = TRUE;
        dirty->Union(0, 0, (Int32)(mWidth * appScale + 0.5f), (Int32)(mHeight * appScale + 0.5f));
    }

    //if (DEBUG_ORIENTATION || DEBUG_DRAW) {
    //    Log.v(TAG, "Draw " + mView + "/"
    //        + mWindowAttributes.getTitle()
    //        + ": dirty={" + dirty->mLeft + "," + dirty->top
    //        + "," + dirty->right + "," + dirty->bottom + "} surface="
    //        + surface + " surface.isValid()=" + surface.isValid() + ", appScale:" +
    //        appScale + ", width=" + mWidth + ", height=" + mHeight);
    //}

    if (!dirty->IsEmpty() || mIsAnimating) {
        AutoPtr<ICanvas> canvas;

        Int32 left = dirty->mLeft;
        Int32 top = dirty->mTop;
        Int32 right = dirty->mRight;
        Int32 bottom = dirty->mBottom;

        if (FAILED(surface->LockCanvas(dirty, (ICanvas**)&canvas))) {
            return;
        }

        if (left != dirty->mLeft || top != dirty->mTop || right != dirty->mRight ||
            bottom != dirty->mBottom) {
            mAttachInfo->mIgnoreDirtyState = TRUE;
        }

        // TODO: Do this in native
        canvas->SetDensity(mDensity);

        if (!dirty->IsEmpty() || mIsAnimating) {
            //Int64 startTime = 0L;

            //if (DEBUG_ORIENTATION || DEBUG_DRAW) {
            //    Log.v(TAG, "Surface " + surface + " drawing to bitmap w="
            //        + canvas.getWidth() + ", h=" + canvas.getHeight());
            //    //canvas.drawARGB(255, 255, 0, 0);
            //}

            //if (Config.DEBUG && ViewDebug.profileDrawing) {
            //    startTime = SystemClock.elapsedRealtime();
            //}

            // If this bitmap's format includes an alpha channel, we
            // need to clear it before drawing so that the child will
            // properly re-composite its drawing on a transparent
            // background. This automatically respects the clip/dirty region
            // or
            // If we are applying an offset, we need to clear the area
            // where the offset doesn't appear to avoid having garbage
            // left in the blank areas.
            //
            Boolean isOpaque = FALSE;
            canvas->IsOpaque(&isOpaque);
            if (!isOpaque || yoff != 0) {
                canvas->DrawColorEx(0, PorterDuffMode_CLEAR);
            }

            dirty->SetEmpty();
            mIsAnimating = FALSE;
            mAttachInfo->mDrawingTime = SystemClock::GetUptimeMillis();
            (reinterpret_cast<View*>(mView->Probe(EIID_View)))->mPrivateFlags |= View::DRAWN;

            //if (DEBUG_DRAW) {
            //    AutoPtr<IContext> cxt;
            //    mView->GetContext((IContext**)&cxt);

            //    Log.i(TAG, "Drawing: package:" + cxt.getPackageName() +
            //        ", metrics=" + cxt.getResources().getDisplayMetrics() +
            //        ", compatibilityInfo=" + cxt.getResources().getCompatibilityInfo());
            //}

            Int32 saveCount = 0;
            canvas->SaveEx(CCanvas::MATRIX_SAVE_FLAG, &saveCount);

            canvas->Translate(0, (Float)-yoff);

            if (mTranslator != NULL) {
                mTranslator->TranslateCanvas(canvas);
            }

            canvas->SetScreenDensity(scalingRequired
                ? CDisplayMetrics::DENSITY_DEVICE : 0);
            mView->Draw(canvas);

            mAttachInfo->mIgnoreDirtyState = FALSE;
            canvas->RestoreToCount(saveCount);


            //if (Config.DEBUG && ViewDebug.consistencyCheckEnabled) {
            //    mView.dispatchConsistencyCheck(ViewDebug.CONSISTENCY_DRAWING);
            //}

            //if (SHOW_FPS || Config.DEBUG && ViewDebug.showFps) {
            //    Int32 now = (Int32)SystemClock.elapsedRealtime();
            //    if (sDrawTime != 0) {
            //        nativeShowFPS(canvas, now - sDrawTime);
            //    }
            //    sDrawTime = now;
            //}

            //if (Config.DEBUG && ViewDebug.profileDrawing) {
            //    EventLog.writeEvent(60000, SystemClock.elapsedRealtime() - startTime);
            //}
        }

        assert(SUCCEEDED(surface->UnlockCanvasAndPost(canvas)));
    }

    //if (LOCAL_LOGV) {
    //    Log.v(TAG, "Surface " + surface + " unlockCanvasAndPost");
    //}

    if (scrolling) {
        mFullRedrawNeeded = TRUE;
        ScheduleTraversals();
    }
}

Boolean ViewRoot::ScrollToRectOrFocus(
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    View::AttachInfo* attachInfo = mAttachInfo;
    CRect* ci = attachInfo->mContentInsets;
    CRect* vi = attachInfo->mVisibleInsets;
    Int32 scrollY = 0;
    Boolean handled = FALSE;

    if (vi->mLeft > ci->mLeft || vi->mTop > ci->mTop
        || vi->mRight > ci->mRight || vi->mBottom > ci->mBottom) {
        // We'll assume that we aren't going to change the scroll
        // offset, since we want to avoid that unless it is actually
        // going to make the focus visible...  otherwise we scroll
        // all over the place.
        //
        scrollY = mScrollY;

        // We can be called for two different situations: during a draw,
        // to update the scroll position if the focus has changed (in which
        // case 'rectangle' is NULL), or in response to a
        // requestChildRectangleOnScreen() call (in which case 'rectangle'
        // is non-NULL and we just want to scroll to whatever that
        // rectangle is).
        //
        View* focus = mRealFocusedView != NULL ?
             reinterpret_cast<View*>(mRealFocusedView->Probe(EIID_View)) : NULL;

        // When in touch mode, focus points to the previously focused view,
        // which may have been removed from the view hierarchy. The following
        // line checks whether the view is still in our hierarchy.
        //
        if (focus == NULL || focus->mAttachInfo != mAttachInfo) {
            mRealFocusedView = NULL;
            return FALSE;
        }

        if ((IView*)focus->Probe(EIID_IView) != mLastScrolledFocus.Get()) {
            // If the focus has changed, then ignore any requests to scroll
            // to a rectangle; first we want to make sure the entire focus
            // view is visible.
            //
            rectangle = NULL;
        }

        //if (DEBUG_INPUT_RESIZE) Log.v(TAG, "Eval scroll: focus=" + focus
        //    + " rectangle=" + rectangle + " ci=" + ci
        //    + " vi=" + vi);

        if ((IView*)focus->Probe(EIID_IView) == mLastScrolledFocus.Get()
            && !mScrollMayChange && rectangle == NULL) {
                //// Optimization: if the focus hasn't changed since last
                //// time, and no layout has happened, then just leave things
                //// as they are.
                //if (DEBUG_INPUT_RESIZE) Log.v(TAG, "Keeping scroll y="
                //    + mScrollY + " vi=" + vi->toShortString());
        }
        else if (focus != NULL) {
            // We need to determine if the currently focused view is
            // within the visible part of the window and, if not, apply
            // a pan so it can be seen.
            //
            mLastScrolledFocus = (IView*)focus->Probe(EIID_IView);
            mScrollMayChange = FALSE;

            if (DEBUG_INPUT_RESIZE) {
                Logger::D(TAG, "Need to scroll?");
            }

            // Try to find the rectangle from the focus view.
            //
            if (focus->GetGlobalVisibleRect(mVisRect, NULL)) {
                //if (DEBUG_INPUT_RESIZE) Log.v(TAG, "Root w="
                //    + mView.getWidth() + " h=" + mView.getHeight()
                //    + " ci=" + ci->toShortString()
                //    + " vi=" + vi->toShortString());

                if (rectangle == NULL) {
                    focus->GetFocusedRect(mTempRect);

                    //if (DEBUG_INPUT_RESIZE) Log.v(TAG, "Focus " + focus
                    //    + ": focusRect=" + mTempRect.toShortString());

                    IViewGroup* viewGroup =
                        (IViewGroup*)mView->Probe(EIID_IViewGroup);
                    if (viewGroup) {
                        viewGroup->OffsetDescendantRectToMyCoords(
                            (IView*)focus->Probe(EIID_IView), (IRect*)mTempRect);
                    }

                    //if (DEBUG_INPUT_RESIZE) Log.v(TAG,
                    //    "Focus in window: focusRect="
                    //    + mTempRect.toShortString()
                    //    + " visRect=" + mVisRect.toShortString());
                }
                else {
                    mTempRect->SetEx(rectangle);
                    //if (DEBUG_INPUT_RESIZE) Log.v(TAG,
                    //    "Request scroll to rect: "
                    //    + mTempRect.toShortString()
                    //    + " visRect=" + mVisRect.toShortString());
                }

                Boolean isIntersect;
                mTempRect->IntersectEx(mVisRect, &isIntersect);
                if (isIntersect) {
                    //if (DEBUG_INPUT_RESIZE) Log.v(TAG,
                    //    "Focus window visible rect: "
                    //    + mTempRect.toShortString());

                    Int32 height;
                    mView->GetHeight(&height);
                    if (mTempRect->GetHeight() >
                        (height - vi->mTop - vi->mBottom)) {
                        // If the focus simply is not going to fit, then
                        // best is probably just to leave things as-is.
                        //
                        if (DEBUG_INPUT_RESIZE) {
                            Logger::D(TAG,
                                StringBuffer("Too tall; leaving scrollY=") + scrollY);
                        }
                    }
                    else if ((mTempRect->mTop-scrollY) < vi->mTop) {
                        scrollY -= vi->mTop - (mTempRect->mTop-scrollY);
                        if (DEBUG_INPUT_RESIZE) {
                            Logger::D(TAG,
                                StringBuffer("Top covered; scrollY=") + scrollY);
                        }
                    }
                    else if ((mTempRect->mBottom - scrollY) >
                        (height - vi->mBottom)) {
                        scrollY += (mTempRect->mBottom - scrollY)
                            - (height - vi->mBottom);

                        if (DEBUG_INPUT_RESIZE) {
                            Logger::D(TAG,
                                StringBuffer("Bottom covered; scrollY=") + scrollY);
                        }
                    }
                    handled = TRUE;
                }
            }
        }
    }

    if (scrollY != mScrollY) {
        if (DEBUG_INPUT_RESIZE) {
            Logger::D(TAG, StringBuffer("Pan scroll changed: old=")
                + mScrollY + " , new=" + scrollY);
        }

        if (!immediate) {
            if (mScroller == NULL) {
                AutoPtr<IContext> ctx;
                mView->GetContext((IContext**)&ctx);
                mScroller = new Scroller(ctx);
            }
            mScroller->StartScroll(0, mScrollY, 0, scrollY - mScrollY);
        }
        else if (mScroller != NULL) {
            mScroller->AbortAnimation();
        }
        mScrollY = scrollY;
    }

    return handled;
}

ECode ViewRoot::RequestChildFocus(
    /* [in] */ IView* child,
    /* [in] */ IView* focused)
{
    ECode ec = CheckThread();
    if (FAILED(ec)) {
        return ec;
    }

    if (mFocusedView.Get() != focused) {
        //mAttachInfo->mTreeObserver.dispatchOnGlobalFocusChange(mFocusedView, focused);
        ScheduleTraversals();
    }

    mFocusedView = mRealFocusedView = focused;

    if (DEBUG_INPUT_RESIZE) {
        Logger::D(TAG, "Request child focus: focus now "/* + mFocusedView*/);
    }

    return NOERROR;
}

ECode ViewRoot::ClearChildFocus(
    /* [in] */ IView* child)
{
    ECode ec = CheckThread();
    if (FAILED(ec)) {
        return ec;
    }

    IView* oldFocus = mFocusedView;

    if (DEBUG_INPUT_RESIZE) {
        Logger::D(TAG, "Clearing child focus");
    }

    mFocusedView = mRealFocusedView = NULL;

    Boolean hasFocus = FALSE;
    mView->HasFocus(&hasFocus);

    if (mView != NULL && !hasFocus) {
        // If a view gets the focus, the listener will be invoked from requestChildFocus()
        //
        if (!((View*)mView->Probe(EIID_View))->RequestFocus(View::FOCUS_FORWARD)) {
            //mAttachInfo->mTreeObserver.dispatchOnGlobalFocusChange(oldFocus, NULL);
        }
    }
    else if (oldFocus != NULL) {
        //mAttachInfo->mTreeObserver.dispatchOnGlobalFocusChange(oldFocus, NULL);
    }

    return NOERROR;
}


ECode ViewRoot::FocusableViewAvailable(
    /* [in] */ IView* v)
{
    assert(v);
    ECode ec = CheckThread();
    if (FAILED(ec)) {
        return ec;
    }

    if (mView != NULL && !((View*)mView->Probe(EIID_View))->HasFocus()) {
        Boolean result;
        v->RequestFocus(&result);
    }
    else {
        // the one case where will transfer focus away from the current one
        // is if the current view is a view group that prefers to give focus
        // to its children first AND the view is a descendant of it.
        //
        AutoPtr<IView> temp;
        mView->FindFocus((IView**)&temp);
        mFocusedView = temp;

        Boolean descendantsHaveDibsOnFocus = FALSE;

        if (mFocusedView != NULL) {
            IViewGroup* viewGroup = (IViewGroup*)mFocusedView->Probe(EIID_IViewGroup);
            if (viewGroup) {
                Int32 focusability;
                viewGroup->GetDescendantFocusability(&focusability);
                descendantsHaveDibsOnFocus =
                    focusability == ViewGroup::FOCUS_AFTER_DESCENDANTS;
            }
        }

        if (descendantsHaveDibsOnFocus && IsViewDescendantOf(v, mFocusedView)) {
            // If a view gets the focus, the listener will be invoked from requestChildFocus()
            Boolean result;
            v->RequestFocus(&result);
        }
    }

    return NOERROR;
}

ECode ViewRoot::RecomputeViewAttributes(
    /* [in] */ IView* child)
{
    ECode ec = CheckThread();
    if (FAILED(ec)) {
        return ec;
    }

    if (mView.Get() == child) {
        mAttachInfo->mRecomputeGlobalAttributes = TRUE;
        if (!mWillDrawSoon) {
            ScheduleTraversals();
        }
    }

    return NOERROR;
}

void ViewRoot::DispatchDetachedFromWindow()
{
    if (LOCAL_LOGV) /*(Config.LOGV)*/ {
        Logger::D(TAG,
            StringBuffer("Detaching in ")/* + this + " of " + mSurface*/);
    }

    if (mView != NULL) {
        ((View*)mView->Probe(EIID_View))->DispatchDetachedFromWindow();
    }

    mView = NULL;
    mAttachInfo->mRootView = NULL;
    mAttachInfo->mSurface = NULL;

    if (mUseGL) {
        DestroyGL();
    }
    mSurface->ReleaseSurface();

    if (mInputChannel != NULL) {
        if (mInputQueueCallback != NULL) {
            mInputQueueCallback->OnInputQueueDestroyed(mInputQueue);
            mInputQueueCallback = NULL;
        }
        else {
            CInputQueue::UnregisterInputChannel(mInputChannel);
        }
    }

    sWindowSession->Remove(mWindow);

    // Dispose the input channel after removing the window so the Window Manager
    // doesn't interpret the input channel being closed as an abnormal termination.
    if (mInputChannel != NULL) {
        mInputChannel->Dispose();
        mInputChannel = NULL;
    }
}

void ViewRoot::UpdateConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ Boolean force)
{
    //if (DEBUG_CONFIGURATION) Log.v(TAG,
    //    "Applying new config to window "
    //    + mWindowAttributes.getTitle()
    //    + ": " + config);

    {
        Mutex::Autolock lock(sConfigCallbacksLock);

        List<AutoPtr<IComponentCallbacks> >::Iterator iter;
        for (iter = sConfigCallbacks.Begin(); iter != sConfigCallbacks.End(); ++iter) {
            iter->Get()->OnConfigurationChanged(config);
        }
    }

    if (mView != NULL) {
        // At this point the resources have been updated to
        // have the most recent config, whatever that is.  Use
        // the on in them which may be newer.
        //
        AutoPtr<IResources> resources;
        mView->GetResources((IResources**)&resources);
        assert(resources);

        //resources->GetConfiguration(&config);
        //assert(config);
        //config->Release();

        Int32 result;
        if (FAILED(mLastConfiguration->Diff((CConfiguration*)config, &result))) {
            return;
        }

        if (force || (result != 0)) {
            mLastConfiguration->SetTo(config);
            mView->DispatchConfigurationChanged(config);
        }
    }
}

/**
 * Return true if child is an ancestor of parent, (or equal to the parent).
 */
Boolean ViewRoot::IsViewDescendantOf(
    /* [in] */ IView* child,
    /* [in] */ IView* parent)
{
    assert(child != NULL && parent != NULL);
    if (child == parent) {
        return TRUE;
    }

    AutoPtr<IViewParent> theParent;
    child->GetParent((IViewParent**)&theParent);

    return (theParent->Probe(EIID_IViewGroup))
        && IsViewDescendantOf((IView*)theParent->Probe(EIID_IView), parent);
}

void ViewRoot::ForceLayout(
    /* [in] */ IView* view)
{
    assert(view != NULL);
    view->ForceLayout();
    IViewGroup* group = (IViewGroup*)view->Probe(EIID_IViewGroup);
    if (group) {
        Int32 count = 0;
        group->GetChildCount(&count);

        AutoPtr<IView> child;
        for (Int32 i = 0; i < count; i++) {
            if (child != NULL) child->Release();
            group->GetChildAt(i, (IView**)&child);
            ForceLayout(child);
        }
    }
}

void ViewRoot::StartInputEvent(
    /* [in] */ IRunnable* finishedCallback)
{
    Mutex::Autolock lock(mFinishedCallbackLock);
    if (mFinishedCallback != NULL) {
        Logger::W(TAG, "Received a new input event from the input queue but there is "
                "already an unfinished input event in progress.");
    }

    mFinishedCallback = finishedCallback;
}

void ViewRoot::FinishInputEvent()
{
    Mutex::Autolock lock(mFinishedCallbackLock);
    if (LOCAL_LOGV) {
        Logger::D(TAG, "Telling window manager input event is finished");
    }

    if (mFinishedCallback != NULL) {
        mFinishedCallback->Run();
        mFinishedCallback = NULL;
    }
    else {
        Logger::W(TAG,
            StringBuffer("Attempted to tell the input queue that the current input ")
            + "event is finished but there is no input event actually in progress.");
    }
}

/**
* Something in the current window tells us we need to change the touch mode.  For
* example, we are not in touch mode, and the user touches the screen.
*
* If the touch mode has changed, tell the window manager, and handle it locally.
*
* @param inTouchMode Whether we want to be in touch mode.
* @return True if the touch mode changed and focus changed was changed as a result
*/
Boolean ViewRoot::EnsureTouchMode(
    /* [in] */ Boolean inTouchMode)
{
    if (DBG) {
        Logger::D("touchmode", StringBuffer("ensureTouchMode(") + inTouchMode +
            "), current touch mode is" + mAttachInfo->mInTouchMode);
    }

    if (mAttachInfo->mInTouchMode == inTouchMode) {
        return FALSE;
    }

    // tell the window manager
    //
    sWindowSession->SetInTouchMode(inTouchMode);
    //try {
    //    sWindowSession.setInTouchMode(inTouchMode);
    //} catch (RemoteException e) {
    //    throw new RuntimeException(e);
    //}

    // handle the change
    return EnsureTouchModeLocally(inTouchMode);
}

/**
* Ensure that the touch mode for this window is set, and if it is changing,
* take the appropriate action.
* @param inTouchMode Whether we want to be in touch mode.
* @return True if the touch mode changed and focus changed was changed as a result
*/
Boolean ViewRoot::EnsureTouchModeLocally(
    /* [in] */ Boolean inTouchMode)
{
    if (DBG) {
        Logger::D("touchmode", StringBuffer("ensureTouchModeLocally(")
            + inTouchMode + "), current "
            + "touch mode is " + mAttachInfo->mInTouchMode);
    }

    if (mAttachInfo->mInTouchMode == inTouchMode) {
        return FALSE;
    }

    mAttachInfo->mInTouchMode = inTouchMode;
    //mAttachInfo->mTreeObserver.dispatchOnTouchModeChanged(inTouchMode);

    return (inTouchMode) ? EnterTouchMode() : LeaveTouchMode();
}

Boolean ViewRoot::EnterTouchMode()
{
    if (mView != NULL) {
        Boolean hasFocus = FALSE;
        mView->HasFocus(&hasFocus);
        if (hasFocus) {
            // note: not relying on mFocusedView here because this could
            // be when the window is first being added, and mFocused isn't
            // set yet.
            //
            AutoPtr<IView> focused;
            mView->FindFocus((IView**)&focused);
            if (focused != NULL) {
                Boolean isFocusable;
                focused->IsFocusableInTouchMode(&isFocusable);
                if (!isFocusable) {
                    IViewGroup* ancestorToTakeFocus =
                        FindAncestorToTakeFocusInTouchMode(focused.Get());

                    if (ancestorToTakeFocus != NULL) {
                        // there is an ancestor that wants focus after its descendants that
                        // is focusable in touch mode.. give it focus
                        //
                        Boolean result;
                        ancestorToTakeFocus->RequestFocus(&result);

                        return result;
                    }
                    else {
                        // nothing appropriate to have focus in touch mode, clear it out
                        //
                        ((View*)mView->Probe(EIID_View))->UnFocus();
                        //mAttachInfo->mTreeObserver.dispatchOnGlobalFocusChange(focused, NULL);
                        mFocusedView = NULL;

                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}

/**
* Find an ancestor of focused that wants focus after its descendants and is
* focusable in touch mode.
* @param focused The currently focused view.
* @return An appropriate view, or NULL if no such view exists.
*/
IViewGroup* ViewRoot::FindAncestorToTakeFocusInTouchMode(
    /* [in] */ IView* focused)
{
    AutoPtr<IViewParent> parent;
    focused->GetParent((IViewParent**)&parent);

    IViewGroup* vgParent = (IViewGroup*)parent->Probe(EIID_IViewGroup);
    while (vgParent) {
        Int32 focusability;
        vgParent->GetDescendantFocusability(&focusability);
        Boolean isFocusable;
        vgParent->IsFocusableInTouchMode(&isFocusable);
        if (focusability == ViewGroup::FOCUS_AFTER_DESCENDANTS
            && isFocusable) {
            return vgParent;
        }

        if (((View*)vgParent->Probe(EIID_View))->IsRootNamespace()) {
            return NULL;
        }
        else {
            vgParent->GetParent((IViewParent**)&parent);
            vgParent = (IViewGroup*)parent->Probe(EIID_IViewGroup);
        }
    }

    return NULL;
}

Boolean ViewRoot::LeaveTouchMode()
{
    if (mView != NULL) {
        Boolean hasFocus = FALSE;
        mView->HasFocus(&hasFocus);
        if (hasFocus) {
            // i learned the hard way to not trust mFocusedView :)
            //
            AutoPtr<IView> temp;
            mView->FindFocus((IView**)&temp);
            mFocusedView = temp;

            if (mFocusedView == NULL || IViewGroup::Probe(mFocusedView) == NULL) {
                // some view has focus, let it keep it
                return FALSE;
            }
            else {
                Int32 focusability;
                IViewGroup::Probe(mFocusedView)->GetDescendantFocusability(&focusability);
                if (focusability != ViewGroup::FOCUS_AFTER_DESCENDANTS) {
                    // some view group has focus, and doesn't prefer its children
                    // over itself for focus, so let them keep it.
                    return FALSE;
                }
            }
        }

        // find the best view to give focus to in this brave new non-touch-mode
        // world
        //
        AutoPtr<IView> focused;
        FocusSearch(NULL, View::FOCUS_DOWN, (IView**)&focused);
        if (focused != NULL) {
            Boolean result;
            focused->RequestFocusEx(View::FOCUS_DOWN, &result);
            return result;
        }
    }

    return FALSE;
}

void ViewRoot::DeliverPointerEvent(
    /* [in] */ IMotionEvent* event)
{
    assert(event);

    {
        printf("====================A MotionEvent===================\n");
        Int64 downTime, eventTime;
        Float x, y, pressure, size, orientation;
        event->GetDownTime(&downTime);
        event->GetEventTime(&eventTime);
        event->GetX(&x);
        event->GetY(&y);
        event->GetPressure(&pressure);
        event->GetSize(&size);
        event->GetOrientation(&orientation);
        printf("event = 0x%08x, downtime = %d, eventtime = %d,\n",
           event, downTime, eventTime);

        printf("x = %f, y = %f, pressure = %f, size = %f, orientation = %f\n",
           x, y, pressure, size, orientation);

        printf("\n");
    }

    if (mTranslator != NULL) {
        mTranslator->TranslateEventInScreenToAppWindow(event);
    }

    Boolean handled = FALSE;
    if (mView != NULL && mAdded) {
        // enter touch mode on the down
        Int32 action;
        event->GetAction(&action);

        Boolean isDown = (action == MotionEvent_ACTION_DOWN);
        if (isDown) {
            EnsureTouchMode(TRUE);
        }

        if(LOCAL_LOGV) {
            CaptureMotionLog("captureDispatchPointer", event);
        }

        if (mCurScrollY != 0) {
            event->OffsetLocation(0, mCurScrollY);
        }

        if (MEASURE_LATENCY) {
            //lt.sample("A Dispatching TouchEvents", System.nanoTime() - event->getEventTimeNano());
        }

        mView->DispatchTouchEvent(event, &handled);
        if (MEASURE_LATENCY) {
            //lt.sample("B Dispatched TouchEvents ", System.nanoTime() - event->getEventTimeNano());
        }

        if (!handled && isDown) {
            Int32 edgeSlop = 12;//mViewConfiguration->GetScaledEdgeSlop();

            Int32 edgeFlags;
            event->GetEdgeFlags(&edgeFlags);
            Int32 direction = View::FOCUS_UP;
            Float x;
            event->GetX(&x);
            Float y;
            event->GetY(&y);
            Int32 deltas[2];

            if ((edgeFlags & MotionEvent_EDGE_TOP) != 0) {
                direction = View::FOCUS_DOWN;
                if ((edgeFlags & MotionEvent_EDGE_LEFT) != 0) {
                    deltas[0] = edgeSlop;
                    x += edgeSlop;
                }
                else if ((edgeFlags & MotionEvent_EDGE_RIGHT) != 0) {
                    deltas[0] = -edgeSlop;
                    x -= edgeSlop;
                }
            }
            else if ((edgeFlags & MotionEvent_EDGE_BOTTOM) != 0) {
                direction = View::FOCUS_UP;
                if ((edgeFlags & MotionEvent_EDGE_LEFT) != 0) {
                    deltas[0] = edgeSlop;
                    x += edgeSlop;
                }
                else if ((edgeFlags & MotionEvent_EDGE_RIGHT) != 0) {
                    deltas[0] = -edgeSlop;
                    x -= edgeSlop;
                }
            }
            else if ((edgeFlags & MotionEvent_EDGE_LEFT) != 0) {
                direction = View::FOCUS_RIGHT;
            }
            else if ((edgeFlags & MotionEvent_EDGE_RIGHT) != 0) {
                direction = View::FOCUS_LEFT;
            }

            IViewGroup* viewGroup = (IViewGroup*)mView->Probe(EIID_IViewGroup);
            if (edgeFlags != 0 && viewGroup) {
                IView* nearest = NULL; //FocusFinder.getInstance().findNearestTouchable(
                    //((ViewGroup) mView), x, y, direction, deltas);
                if (nearest != NULL) {
                    event->OffsetLocation(deltas[0], deltas[1]);
                    event->SetEdgeFlags(0);
                    Boolean result;
                    mView->DispatchTouchEvent(event, &result);
                }
            }
        }
    }
}

void ViewRoot::DeliverTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    assert(event);
    if (DEBUG_TRACKBALL)
        Logger::D(TAG, "Motion event:" /*+ event*/);

    Boolean handled = FALSE;
    if (mView != NULL && mAdded) {
        mView->DispatchTrackballEvent(event, &handled);
        if (handled) {
            // If we reach this, we delivered a trackball event to mView and
            // mView consumed it. Because we will not translate the trackball
            // event into a key event, touch mode will not exit, so we exit
            // touch mode here.
            EnsureTouchMode(FALSE);
            return;
        }

        // Otherwise we could do something here, like changing the focus
        // or something?
    }

    Int64 curTime = SystemClock::GetUptimeMillis();
    if ((mLastTrackballTime+MAX_TRACKBALL_DELAY) < curTime) {
        // It has been too Int64 since the last movement,
        // so restart at the beginning.
        mTrackballAxisX->Reset(0);
        mTrackballAxisY->Reset(0);
        mLastTrackballTime = curTime;
    }

    Int32 action;
    event->GetAction(&action);

    Int32 metastate;
    event->GetMetaState(&metastate);

    switch (action) {
    case MotionEvent_ACTION_DOWN:
        {
            mTrackballAxisX->Reset(2);
            mTrackballAxisY->Reset(2);
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                curTime, curTime,
                KeyEvent_ACTION_DOWN, KeyEvent_KEYCODE_DPAD_CENTER,
                0, metastate, (IKeyEvent**)&keyEvent);
            DeliverKeyEvent(keyEvent, FALSE);
        }
        break;
    case MotionEvent_ACTION_UP:
        {
            mTrackballAxisX->Reset(2);
            mTrackballAxisY->Reset(2);
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                curTime, curTime,
                1, 23,
                0, metastate, (IKeyEvent**)&keyEvent);
            DeliverKeyEvent(keyEvent, FALSE);
        }
        break;
    }

    Float x, y;
    event->GetX(&x);
    event->GetY(&y);

    if (DEBUG_TRACKBALL) {
        //Logger::D(
        //    TAG, StringBuffer("TB X=") + mTrackballAxisX->mPosition + " step="
        //    + mTrackballAxisX->mStep + " dir=" + mTrackballAxisX->mDir
        //    + " acc=" + mTrackballAxisX->mAcceleration
        //    + " move=" + x + " / Y=" + mTrackballAxisY->mPosition + " step="
        //    + mTrackballAxisY->mStep + " dir=" + mTrackballAxisY->mDir
        //    + " acc=" + mTrackballAxisY->mAcceleration
        //    + " move=" + y);
    }

    Int64 eventTime;
    event->GetEventTime(&eventTime);

    Float xOff = mTrackballAxisX->Collect(x, eventTime, "X");
    Float yOff = mTrackballAxisY->Collect(y, eventTime, "Y");

    // Generate DPAD events based on the trackball movement.
    // We pick the axis that has moved the most as the direction of
    // the DPAD.  When we generate DPAD events for one axis, then the
    // other axis is reset -- we don't want to perform DPAD jumps due
    // to slight movements in the trackball when making major movements
    // along the other axis.
    Int32 keycode = 0;
    Int32 movement = 0;
    Float accel = 1;
    if (xOff > yOff) {
        Float xPrecision;
        event->GetXPrecision(&xPrecision);
        movement = mTrackballAxisX->Generate(2/xPrecision);
        if (movement != 0) {
            keycode = movement > 0 ? KeyEvent_KEYCODE_DPAD_RIGHT
                : KeyEvent_KEYCODE_DPAD_LEFT;
            accel = mTrackballAxisX->mAcceleration;
            mTrackballAxisY->Reset(2);
        }
    }
    else if (yOff > 0) {
        Float yPrecision;
        event->GetYPrecision(&yPrecision);
        movement = mTrackballAxisY->Generate(2/yPrecision);
        if (movement != 0) {
            keycode = movement > 0 ? KeyEvent_KEYCODE_DPAD_DOWN
                : KeyEvent_KEYCODE_DPAD_UP;
            accel = mTrackballAxisY->mAcceleration;
            mTrackballAxisX->Reset(2);
        }
    }

    if (keycode != 0) {
        if (movement < 0)
            movement = -movement;
        Int32 accelMovement = (Int32)(movement * accel);

        if (DEBUG_TRACKBALL) {
            //Logger::D(
                //TAG, StringBuffer("Move: movement=") + movement
                //+ " accelMovement=" + accelMovement
                //+ " accel=" + accel);
        }

        if (accelMovement > movement) {
            if (DEBUG_TRACKBALL)
                Logger::D("foo", StringBuffer("Delivering fake DPAD: ") + keycode);
            movement--;

            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                curTime, curTime,
                KeyEvent_ACTION_MULTIPLE, keycode,
                accelMovement-movement, metastate,
                (IKeyEvent**)&keyEvent);

            DeliverKeyEvent(keyEvent, FALSE);
        }

        while (movement > 0) {
            if (DEBUG_TRACKBALL)
                Logger::D("foo", StringBuffer("Delivering fake DPAD: ") + keycode);

            movement--;
            curTime = SystemClock::GetUptimeMillis();

            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                curTime, curTime,
                KeyEvent_ACTION_DOWN, keycode,
                accelMovement-movement, metastate,
                (IKeyEvent**)&keyEvent);
            DeliverKeyEvent(keyEvent, FALSE);

            CKeyEvent::New(
                curTime, curTime,
                KeyEvent_ACTION_UP, keycode,
                accelMovement-movement, metastate,
                (IKeyEvent**)&keyEvent);
            DeliverKeyEvent(keyEvent, FALSE);
        }

        mLastTrackballTime = curTime;
    }
}

/**
 * @param keyCode The key code
 * @return True if the key is directional.
 */
Boolean ViewRoot::IsDirectional(
    /* [in] */ Int32 keyCode)
{
    switch (keyCode) {
        case KeyEvent_KEYCODE_DPAD_LEFT:
        case KeyEvent_KEYCODE_DPAD_RIGHT:
        case KeyEvent_KEYCODE_DPAD_UP:
        case KeyEvent_KEYCODE_DPAD_DOWN:
            return TRUE;
    }
    return FALSE;
}

/**
 * Returns true if this key is a keyboard key.
 * @param keyEvent The key event->
 * @return whether this key is a keyboard key.
 */
Boolean ViewRoot::IsKeyboardKey(
    /* [in] */ IKeyEvent* keyEvent)
{
    Int32 convertedKey = 0;
    keyEvent->GetUnicodeChar(&convertedKey);

    return convertedKey > 0;
}

/**
* See if the key event means we should leave touch mode (and leave touch
* mode if so).
* @param event The key event->
* @return Whether this key event should be consumed (meaning the act of
*   leaving touch mode alone is considered the event).
*/
Boolean ViewRoot::CheckForLeavingTouchModeAndConsume(
    /* [in] */ IKeyEvent* event)
{
    Int32 action;
    event->GetAction(&action);
    if (action != KeyEvent_ACTION_DOWN
        && action != KeyEvent_ACTION_MULTIPLE) {
        return FALSE;
    }

    Int32 flags;
    event->GetFlags(&flags);
    if ((flags&KeyEvent_FLAG_KEEP_TOUCH_MODE) != 0) {
        return FALSE;
    }

    // only relevant if we are in touch mode
    if (!mAttachInfo->mInTouchMode) {
        return FALSE;
    }

    // if something like an edit text has focus and the user is typing,
    // leave touch mode
    //
    // note: the condition of not being a keyboard key is kind of a hacky
    // approximation of whether we think the focused view will want the
    // key; if we knew for sure whether the focused view would consume
    // the event, that would be better.
    //
    Boolean hasFocus = FALSE;
    if (mView != NULL) {
        mView->HasFocus(&hasFocus);
    }

    if (IsKeyboardKey(event) && hasFocus) {
        AutoPtr<IView> temp;
        mView->FindFocus((IView**)&temp);
        mFocusedView = temp;
        assert(mFocusedView.Get());
        IViewGroup* viewGroup = (IViewGroup*)mFocusedView->Probe(EIID_IViewGroup);
        if (viewGroup) {
            Int32 focusability;
            viewGroup->GetDescendantFocusability(&focusability);
            if (focusability == ViewGroup::FOCUS_AFTER_DESCENDANTS) {
                // something has focus, but is holding it weakly as a container
                return FALSE;
            }
        }

        if (EnsureTouchMode(FALSE)) {
            Logger::E(TAG,
                "should not have changed focus when leaving touch mode while a view has focus.");
            //throw new IllegalStateException("should not have changed focus "
            //    + "when leaving touch mode while a view has focus.");
        }
        return FALSE;
    }

    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    if (IsDirectional(keyCode)) {
        // no view has focus, so we leave touch mode (and find something
        // to give focus to).  the event is consumed if we were able to
        // find something to give focus to.
        return EnsureTouchMode(FALSE);
    }

    return FALSE;
}

///**
//* log motion events
//*/
void ViewRoot::CaptureMotionLog(
    /* [in] */ const char* subTag,
    /* [in] */ IMotionEvent* ev)
{
//    //check dynamic switch
//    if (ev == NULL ||
//        SystemProperties.getInt(ViewDebug.SYSTEM_PROPERTY_CAPTURE_EVENT, 0) == 0) {
//            return;
//    }
//
//    StringBuilder sb = new StringBuilder(subTag + ": ");
//    sb.append(ev.getDownTime()).append(',');
//    sb.append(ev.getEventTime()).append(',');
//    sb.append(ev.getAction()).append(',');
//    sb.append(ev.getX()).append(',');
//    sb.append(ev.getY()).append(',');
//    sb.append(ev.getPressure()).append(',');
//    sb.append(ev.getSize()).append(',');
//    sb.append(ev.getMetaState()).append(',');
//    sb.append(ev.getXPrecision()).append(',');
//    sb.append(ev.getYPrecision()).append(',');
//    sb.append(ev.getDeviceId()).append(',');
//    sb.append(ev.getEdgeFlags());
//    Log.d(TAG, sb.toString());
}

///**
//* log motion events
//*/
void ViewRoot::CaptureKeyLog(
    /* [in] */ const char* subTag,
    /* [in] */ IKeyEvent* ev)
{
//    //check dynamic switch
//    if (ev == NULL ||
//        SystemProperties.getInt(ViewDebug.SYSTEM_PROPERTY_CAPTURE_EVENT, 0) == 0) {
//            return;
//    }
//    StringBuilder sb = new StringBuilder(subTag + ": ");
//    sb.append(ev.getDownTime()).append(',');
//    sb.append(ev.getEventTime()).append(',');
//    sb.append(ev.getAction()).append(',');
//    sb.append(ev.getKeyCode()).append(',');
//    sb.append(ev.getRepeatCount()).append(',');
//    sb.append(ev.getMetaState()).append(',');
//    sb.append(ev.getDeviceId()).append(',');
//    sb.append(ev.getScanCode());
//    Log.d(TAG, sb.toString());
}

Int32 ViewRoot::EnqueuePendingEvent(
    /* [in] */ IInterface* event,
    /* [in] */ Boolean sendDone)
{
    Int32 seq = mPendingEventSeq + 1;
    if (seq < 0)
        seq = 0;
    mPendingEventSeq = seq;
    mPendingEvents[seq] = event;

    return sendDone ? seq : -seq;
}

ECode ViewRoot::RetrievePendingEvent(
    /* [in] */Int32 seq,
    /* [out] */ IInterface** event)
{
    if (event == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (seq < 0)
        seq = -seq;

    Map<Int32, AutoPtr<IInterface> >::Iterator find = mPendingEvents.Find(seq);
    if (find != mPendingEvents.End()) {
        *event = find->mSecond;
        (*event)->AddRef();
        mPendingEvents.Erase(find);
    }
    else {
        *event = NULL;
    }

    return NOERROR;
}

void ViewRoot::DeliverKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean sendDone)
{
    assert(event);
    {
        printf("====================A KeyEvent===================\n");
        Boolean isDown, isSystem, alt, shift, sym, longPress;
        event->IsDown(&isDown);
        event->IsSystem(&isSystem);
        event->IsAltPressed(&alt);
        event->IsShiftPressed(&shift);
        event->IsSymPressed(&sym);
        event->IsLongPress(&longPress);

        //printf("isDown = %d, isSystem = %d, alt = %d, shift = %d, sym = %d, longPress = %d\n",
        //    isDown, isSystem, alt, shift, sym, longPress);

        Int32 keyCode, scanCode, deviceID, uChar;
        String chars;
        Int64 downTime, eventTime;
        Char16 label, number;

        event->GetKeyCode(&keyCode);
        event->GetScanCode(&scanCode);
        event->GetKeyboardDevice(&deviceID);
        event->GetCharacters(&chars);
        event->GetDownTime(&downTime);
        event->GetEventTime(&eventTime);
        event->GetDisplayLabel(&label);
        event->GetUnicodeChar(&uChar);
        event->GetNumber(&number);

        // printf("keyCode = %d, scanCode = %d, deviceID = %d, unicodeChar = %d\n",
        //     keyCode, scanCode, deviceID, uChar);

        // printf("characters = %s\n", (const char*)chars);

        // printf("downTime = %d, eventTime = %d\n", downTime, eventTime);

        // printf("label = %d, '%c', number = %d\n", label, (char)label, number);
        printf("\n");
    }
    // If mView is NULL, we just consume the key event because it doesn't
    // make sense to do anything else with it.
    //
    Boolean handled = TRUE;
    if (mView != NULL) {
        mView->DispatchKeyEventPreIme(event, &handled);
    }

    if (handled) {
        if (sendDone) {
            FinishInputEvent();
        }
        return;
    }

    // If it is possible for this window to interact with the input
    // method window, then we want to first dispatch our key events
    // to the input method.
    if (mLastWasImTarget) {
        AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
        if (imm != NULL && mView != NULL) {
            Int32 seq = EnqueuePendingEvent(event, sendDone);
            // if (DEBUG_IMF) Log.v(TAG, "Sending key event to IME: seq="
            //     + seq + " event=" + event);
            AutoPtr<IContext> ctx;
            mView->GetContext((IContext**)&ctx);
            imm->DispatchKeyEvent(ctx, seq, event,
                mInputMethodCallback);
            return;
        }
    }

    DeliverKeyEventToViewHierarchy(event, sendDone);
}

void ViewRoot::HandleFinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    AutoPtr<IKeyEvent> event;
    RetrievePendingEvent(seq, (IInterface**)&event);

    if (DEBUG_IMF) {
        //Logger::D(TAG, "IME finished event: seq=" + seq
        //    + " handled=" + handled + " event=" + event);
    }

    if (event != NULL) {
        Boolean sendDone = seq >= 0;
        if (!handled) {
            DeliverKeyEventToViewHierarchy(event, sendDone);
            return;
        }
        else if (sendDone) {
            FinishInputEvent();
        }
        else {
            //Logger::W(TAG, "handleFinishedEvent(seq=" + seq
            //    + " handled=" + handled + " ev=" + event
            //    + ") neither delivering nor finishing key");
        }
    }
}

void ViewRoot::DeliverKeyEventToViewHierarchy(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean sendDone)
{
    if (mView != NULL && mAdded) {
        Int32 action;
        event->GetAction(&action);
        Boolean isDown = (action == KeyEvent_ACTION_DOWN);

        if (CheckForLeavingTouchModeAndConsume(event)) {
            return;
        }

        if (LOCAL_LOGV) { //Config.LOGV
            CaptureKeyLog("captureDispatchKeyEvent", event);
        }

        Boolean keyHandled = TRUE;
        mView->DispatchKeyEvent(event, &keyHandled);

        if (!keyHandled && isDown) {
            Int32 direction = 0;
            Int32 keyCode;
            event->GetKeyCode(&keyCode);

            switch (keyCode) {
            case KeyEvent_KEYCODE_DPAD_LEFT:
                direction = View::FOCUS_LEFT;
                break;
            case KeyEvent_KEYCODE_DPAD_RIGHT:
                direction = View::FOCUS_RIGHT;
                break;
            case KeyEvent_KEYCODE_DPAD_UP:
                direction = View::FOCUS_UP;
                break;
            case KeyEvent_KEYCODE_DPAD_DOWN:
                direction = View::FOCUS_DOWN;
                break;
            }

            if (direction != 0) {
                AutoPtr<IView> focused;
                if (mView != NULL) {
                    mView->FindFocus((IView**)&focused);
                }

                if (focused != NULL) {
                    AutoPtr<IView> v;
                    focused->FocusSearch(direction, (IView**)&v);
                    Boolean focusPassed = FALSE;
                    if (v != NULL && v != focused) {
                        // do the math the get the interesting rect
                        // of previous focused into the coord system of
                        // newly focused view
                        //
                        focused->GetFocusedRect((IRect*)mTempRect);
                        IViewGroup* viewGroup = (IViewGroup*)mView->Probe(EIID_IViewGroup);
                        if (viewGroup) {
                            viewGroup->OffsetDescendantRectToMyCoords(
                                focused, mTempRect);
                            viewGroup->OffsetRectIntoDescendantCoords(
                                v, mTempRect);
                        }
                        v->RequestFocusEx2(direction, mTempRect, &focusPassed);
                    }

                    if (!focusPassed) {
                        Boolean result;
                        mView->DispatchUnhandledMove(focused, direction, &result);
                    }
                    else {
                        //PlaySoundEffect(SoundEffectConstants.getContantForFocusDirection(direction));
                    }
                }
            }
        }
    }

    if (sendDone) {
        FinishInputEvent();
    }
    // Let the exception fall through -- the looper will catch
    // it and take care of the bad app for us.
}

//AudioManager ViewRoot::GetAudioManager()
//{
//    if (mView == NULL) {
//        throw new IllegalStateException("getAudioManager called when there is no mView");
//    }
//    if (mAudioManager == NULL) {
//        mAudioManager = (AudioManager) mView.getContext().getSystemService(Context.AUDIO_SERVICE);
//    }
//    return mAudioManager;
//}

Int32 ViewRoot::RelayoutWindow(
    /* [in] */ IWindowManagerLayoutParams* params,
    /* [in] */ Int32 viewVisibility,
    /* [in] */ Boolean insetsPending)
{
    Float appScale = mAttachInfo->mApplicationScale;
    Boolean restore = FALSE;

    if (params != NULL && mTranslator != NULL) {
        restore = TRUE;
        params->Backup();
        mTranslator->TranslateWindowLayout(params);
    }

    if (params != NULL) {
        if (DBG) {
            Logger::D(TAG, "WindowLayout in layoutWindow:" /*+ params*/);
        }
    }

    mPendingConfiguration->mSeq = 0;

    Int32 measuredWidth, measuredHeight;
    AutoPtr<IRect> winFrame, pendingContentInsets, pendingVisibleInsets;
    AutoPtr<IConfiguration> pendingConfiguration;
    AutoPtr<ISurface> surface;
    Int32 relayoutResult = 0;

    mView->GetMeasuredWidth(&measuredWidth);
    mView->GetMeasuredHeight(&measuredHeight);

    sWindowSession->Relayout(
        (IInnerWindow*)mWindow, params,
        (Int32)(measuredWidth * appScale + 0.5f),
        (Int32)(measuredHeight * appScale + 0.5f),
        viewVisibility, insetsPending, (IRect*)mWinFrame,
        (IRect*)mPendingContentInsets, (IRect*)mPendingVisibleInsets,
        (IConfiguration*)mPendingConfiguration, (ISurface*)mSurface,
        (IRect**)&winFrame, (IRect**)&pendingContentInsets,
        (IRect**)&pendingVisibleInsets, (IConfiguration**)&pendingConfiguration,
        &relayoutResult, (ISurface**)&surface);
    mWinFrame->SetEx(winFrame);
    mPendingContentInsets->SetEx(pendingContentInsets);
    mPendingVisibleInsets->SetEx(pendingVisibleInsets);
    mPendingConfiguration->SetTo(pendingConfiguration);
    Handle32 nativeSurface;
    surface->GetSurface(&nativeSurface);
    mSurface->SetSurface(nativeSurface);

    if (restore) {
        params->Restore();
    }

    if (mTranslator != NULL) {
        mTranslator->TranslateRectInScreenToAppWinFrame(mWinFrame);
        mTranslator->TranslateRectInScreenToAppWindow(mPendingContentInsets);
        mTranslator->TranslateRectInScreenToAppWindow(mPendingVisibleInsets);
    }

    return relayoutResult;
}

void ViewRoot::PlaySoundEffect(
    /* [in] */ Int32 effectId)
{
    //ECode ec = CheckThread();
    //if (FAILED(ec)) {
    //    return ec;
    //}

    //try {
    //    final AudioManager audioManager = getAudioManager();

    //    switch (effectId) {
    //            case SoundEffectConstants.CLICK:
    //                audioManager.playSoundEffect(AudioManager.FX_KEY_CLICK);
    //                return;
    //            case SoundEffectConstants.NAVIGATION_DOWN:
    //                audioManager.playSoundEffect(AudioManager.FX_FOCUS_NAVIGATION_DOWN);
    //                return;
    //            case SoundEffectConstants.NAVIGATION_LEFT:
    //                audioManager.playSoundEffect(AudioManager.FX_FOCUS_NAVIGATION_LEFT);
    //                return;
    //            case SoundEffectConstants.NAVIGATION_RIGHT:
    //                audioManager.playSoundEffect(AudioManager.FX_FOCUS_NAVIGATION_RIGHT);
    //                return;
    //            case SoundEffectConstants.NAVIGATION_UP:
    //                audioManager.playSoundEffect(AudioManager.FX_FOCUS_NAVIGATION_UP);
    //                return;
    //            default:
    //                throw new IllegalArgumentException("unknown effect id " + effectId +
    //                    " not defined in " + SoundEffectConstants.class.getCanonicalName());
    //    }
    //} catch (IllegalStateException e) {
    //    // Exception thrown by getAudioManager() when mView is NULL
    //    Log.e(TAG, "FATAL EXCEPTION when attempting to play sound effect: " + e);
    //    e.printStackTrace();
    //}
}

ECode ViewRoot::PerformHapticFeedback(
    /* [in] */ Int32 effectId,
    /* [in] */ Boolean always,
    /* [out] */ Boolean* result)
{
    return sWindowSession->PerformHapticFeedback(mWindow, effectId, always, result);
}

ECode ViewRoot::FocusSearch(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out */ IView** result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckThread());

    IViewGroup* viewGroup = (IViewGroup*)mView->Probe(EIID_IViewGroup);
    if (viewGroup == NULL) {
        *result  = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *result = FocusFinder::GetInstance()->FindNextFocus(
        (IViewGroup*)mView->Probe(EIID_IViewGroup), focused, direction);

    return NOERROR;
}

ECode ViewRoot::FocusSearchEx(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out */ IView** result)
{
    return FocusSearch(focused, direction, result);
}

void ViewRoot::Debug()
{
    //mView->Debug();
}

ECode ViewRoot::Die(
    /* [in] */ Boolean immediate)
{
    if (immediate) {
        return DoDie();
    }
    else {
        ECode (STDCALL ViewRoot::*pHandlerFunc)();
        pHandlerFunc = &ViewRoot::DoDie;

        SendMessage(*(Handle32*)&pHandlerFunc, NULL);
    }

    return NOERROR;
}

ECode ViewRoot::DoDie()
{
    ECode ec = CheckThread();
    if (FAILED(ec)) {
        return ec;
    }

    if (LOCAL_LOGV) /*(Config.LOGV)*/ {
        Logger::D(TAG,
            StringBuffer("DIE in ")/* + this + " of " + mSurface*/);
    }


    Mutex::Autolock lock(mSyncLock);

    if (mAdded && !mFirst) {
        Int32 viewVisibility;
        mView->GetVisibility(&viewVisibility);

        Boolean viewVisibilityChanged = mViewVisibility != viewVisibility;
        if (mWindowAttributesChanged || viewVisibilityChanged) {
            // If layout params have been changed, first give them
            // to the window manager to make sure it has the correct
            // animation info.
            //
            if ((RelayoutWindow(mWindowAttributes, viewVisibility, FALSE)
                & WindowManagerImpl_RELAYOUT_FIRST_TIME) != 0) {
                ec = sWindowSession->FinishDrawing(mWindow);
                if (FAILED(ec)) {
                    return ec;
                }
            }
        }

        mSurface->ReleaseSurface();
    }

    if (mAdded) {
        mAdded = FALSE;
        DispatchDetachedFromWindow();
    }

    return NOERROR;
}

void ViewRoot::DispatchFinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    void (STDCALL ViewRoot::*pHandlerFunc)(
        Int32 seq, Boolean handled);

    pHandlerFunc = &ViewRoot::HandleFinishedEvent;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(seq);
    params->WriteBoolean(handled);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

ECode ViewRoot::DispatchResized(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* coveredInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ Boolean reportDraw,
    /* [in] */ IConfiguration* newConfig)
{
    //if (DEBUG_LAYOUT) Log.v(TAG, "Resizing " + this + ": w=" + w
    //    + " h=" + h + " coveredInsets=" + coveredInsets.toShortString()
    //    + " visibleInsets=" + visibleInsets.toShortString()
    //    + " reportDraw=" + reportDraw);

    if (mTranslator != NULL) {
        mTranslator->TranslateRectInScreenToAppWindow(coveredInsets);
        mTranslator->TranslateRectInScreenToAppWindow(visibleInsets);
        w = (Int32)(w * ((CompatibilityInfo::Translator*)mTranslator.Get())
            ->mApplicationInvertedScale);
        h = (Int32)(h * ((CompatibilityInfo::Translator*)mTranslator.Get())
            ->mApplicationInvertedScale);
    }

    ECode (STDCALL ViewRoot::*pHandlerFunc)(
        Int32 w, Int32 h, Boolean reportDraw, ResizedInfo* info);

    pHandlerFunc = &ViewRoot::HandleResized;

    ResizedInfo* ri = new ResizedInfo();
    CRect::New(coveredInsets, (IRect**)&(ri->mCoveredInsets));
    CRect::New(visibleInsets, (IRect**)&(ri->mVisibleInsets));
    ri->mNewConfig = newConfig;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(w);
    params->WriteInt32(h);
    params->WriteBoolean(reportDraw);
    params->WriteInt32((Handle32)ri);

    return SendMessage(*(Handle32*)&pHandlerFunc, params);
}

void ViewRoot::DispatchKey(
    /* [in] */ IKeyEvent* event)
{
    DispatchKey(event, FALSE);
}

ECode ViewRoot::DispatchKey(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean sendDone)
{
    //noinspection ConstantConditions
    Int32 action;
    event->GetAction(&action);
    if (FALSE && action == KeyEvent_ACTION_DOWN) {
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        if (keyCode == KeyEvent_KEYCODE_CAMERA) {
            if (LOCAL_LOGV) { //Config.LOGD
                Logger::D("keydisp",
                "===================================================");
            }

            if (LOCAL_LOGV) //Config.LOGD
                Logger::D("keydisp", "Focused view Hierarchy is:");

            Debug();

            if (LOCAL_LOGV) { //Config.LOGD
                Logger::D("keydisp",
                "===================================================");
            }
        }
    }

    ECode (STDCALL ViewRoot::*pHandlerFunc)(IKeyEvent* event, Boolean visible);

    pHandlerFunc = &ViewRoot::HandleKey;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(event);
    params->WriteBoolean(sendDone);

    Int64 eventTime;
    event->GetEventTime(&eventTime);

    return SendMessageAtTime(*(Handle32*)&pHandlerFunc, params, eventTime);
}

void ViewRoot::DispatchMotion(
    /* [in] */ IMotionEvent* event)
{
    DispatchMotion(event, FALSE);
}

ECode ViewRoot::DispatchMotion(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Boolean sendDone)
{
    Int32 source;
    event->GetSource(&source);
    if ((source & InputDevice_SOURCE_CLASS_POINTER) != 0) {
        DispatchPointer(event, sendDone);
    }
    else if ((source & InputDevice_SOURCE_CLASS_TRACKBALL) != 0) {
        DispatchTrackball(event, sendDone);
    }
    else {
        // TODO
        Logger::W(TAG, "Dropping unsupported motion event (unimplemented): " /*+ event*/);
        if (sendDone) {
            FinishInputEvent();
        }
    }

    return NOERROR;
}

void ViewRoot::DispatchPointer(
    /* [in] */ IMotionEvent* event)
{
    DispatchPointer(event, FALSE);
}

ECode ViewRoot::DispatchPointer(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Boolean sendDone)
{
    ECode (STDCALL ViewRoot::*pHandlerFunc)(IMotionEvent* event, Boolean visible);

    pHandlerFunc = &ViewRoot::HandlePointer;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(event);
    params->WriteBoolean(sendDone);

    Int64 eventTime;
    event->GetEventTime(&eventTime);

    return SendMessageAtTime(*(Handle32*)&pHandlerFunc, params, eventTime);
}

void ViewRoot::DispatchTrackball(
    /* [in] */ IMotionEvent* event)
{
    DispatchTrackball(event, FALSE);
}

ECode ViewRoot::DispatchTrackball(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Boolean sendDone)
{
    ECode (STDCALL ViewRoot::*pHandlerFunc)(IMotionEvent* event, Boolean visible);

    pHandlerFunc = &ViewRoot::HandleTrackball;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(event);
    params->WriteBoolean(sendDone);

    Int64 eventTime;
    event->GetEventTime(&eventTime);

    return SendMessageAtTime(*(Handle32*)&pHandlerFunc, params, eventTime);
}

ECode ViewRoot::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    void (STDCALL ViewRoot::*pHandlerFunc)(Boolean visible);

    pHandlerFunc = &ViewRoot::HandleAppVisibility;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(visible);

    return SendMessage(*(Handle32*)&pHandlerFunc, params);
}

void ViewRoot::DispatchGetNewSurface()
{
    void (STDCALL ViewRoot::*pHandlerFunc)();

    pHandlerFunc = &ViewRoot::HandleGetNewSurface;

    SendMessage(*(Handle32*)&pHandlerFunc, NULL);
}

void ViewRoot::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean inTouchMode)
{
    ECode (STDCALL ViewRoot::*pHandlerFunc)(
        Boolean hasFocus, Boolean inTouchMode);

    pHandlerFunc = &ViewRoot::HandleWindowFocusChanged;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(hasFocus);
    params->WriteBoolean(inTouchMode);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

void ViewRoot::DispatchKeyFromIme(
    /* [in] */ IKeyEvent* event)
{
    Int32 flags = 0;
    event->GetFlags(&flags);
    if ((flags & KeyEvent_FLAG_FROM_SYSTEM) != 0) {
        // The IME is trying to say this event is from the
        // system!  Bad bad bad!
        AutoPtr<IKeyEvent> badEvent;

        CKeyEvent::ChangeFlags(event, flags &~ KeyEvent_FLAG_FROM_SYSTEM,
                    (IKeyEvent**) &badEvent);
    }

    DeliverKeyEventToViewHierarchy(event, FALSE);
}

void ViewRoot::DispatchCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    ECode (STDCALL ViewRoot::*pHandlerFunc)(const String& reason);

    pHandlerFunc = &ViewRoot::HandleCloseSystemDialogs;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(reason);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

/**
* The window is getting focus so if there is anything focused/selected
* send an {@link AccessibilityEvent} to announce that.
*/
void ViewRoot::SendAccessibilityEvents()
{
    //if (!AccessibilityManager.getInstance(mView.getContext()).isEnabled()) {
    //    return;
    //}
    //mView.sendAccessibilityEvent(AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED);
    //View focusedView = mView.findFocus();
    //if (focusedView != NULL && focusedView != mView) {
    //    focusedView.sendAccessibilityEvent(AccessibilityEvent.TYPE_VIEW_FOCUSED);
    //}
}

ECode ViewRoot::ShowContextMenuForChild(
    /* [in] */ IView* originalView,
    /* [out] */ Boolean* result)
{
    assert(result);
    *result = FALSE;

    return NOERROR;
}

ECode ViewRoot::CreateContextMenuEx(
    /* [in] */ IContextMenu* menu)
{
    return NOERROR;
}

void ViewRoot::CreateContextMenu(
    /* [in] */ IContextMenu* menu)
{
}

ECode ViewRoot::ChildDrawableStateChanged(
    /* [in] */ IView* child)
{
    return NOERROR;
}

IRect* ViewRoot::GetWindowFrame()
{
    return mWinFrame.Get();
}

ECode ViewRoot::CheckThread()
{
    if (mThreadID != pthread_self()) {
//         throw new CalledFromWrongThreadException(
//                    "Only the original thread that created a view hierarchy can touch its views.");
        Logger::E(TAG,
            "Only the original thread that created a view hierarchy can touch its views.");

        return E_CALLED_FROM_WRONG_THREAD_EXCEPTION;
    }

    return NOERROR;
}

ECode ViewRoot::RequestDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    // ViewRoot never intercepts touch event, so this can be a no-op

    return NOERROR;
}

ECode ViewRoot::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate,
    /* [out] */ Boolean* result)
{
    assert(result);
    *result = ScrollToRectOrFocus(rectangle, immediate);

    return NOERROR;
}

ECode ViewRoot::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode ViewRoot::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode ViewRoot::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode ViewRoot::HandleResized(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Boolean reportDraw,
    /* [in] */ ResizedInfo* info)
{
    assert(info);

    if (!reportDraw) {
        if (mWinFrame->GetWidth() == w && mWinFrame->GetHeight() == h
            && mPendingContentInsets->Equals(info->mCoveredInsets)
            && mPendingVisibleInsets->Equals(info->mVisibleInsets)
            && info->mNewConfig == NULL) {
                return NOERROR;
        }
    }

    if (mAdded) {
        IConfiguration* config = info->mNewConfig;
        if (config != NULL) {
            UpdateConfiguration(config, FALSE);
        }
        mWinFrame->mLeft = 0;
        mWinFrame->mRight = w;
        mWinFrame->mTop = 0;
        mWinFrame->mBottom = h;
        mPendingContentInsets->SetEx(info->mCoveredInsets);
        mPendingVisibleInsets->SetEx(info->mVisibleInsets);

        if (reportDraw) {
            mReportNextDraw = TRUE;
        }

        if (mView != NULL) {
            ForceLayout(mView);
        }

        RequestLayout();
    }

    return NOERROR;
}

ECode ViewRoot::HandleKey(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean sendDone)
{
    if (LOCAL_LOGV) {
        Logger::D(TAG, StringBuffer("Dispatching key ")
            /*+ event + " to " + mView.Get()*/);
    }

    DeliverKeyEvent(event, sendDone);

    return NOERROR;
}

ECode ViewRoot::HandlePointer(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Boolean sendDone)
{
    DeliverPointerEvent(event);

    event->Recycle();
    if (sendDone) {
        FinishInputEvent();
    }

    if (LOCAL_LOGV || WATCH_POINTER)
        Logger::D(TAG, "Done dispatching!");

    return NOERROR;
}

ECode ViewRoot::HandleTrackball(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Boolean sendDone)
{
    DeliverTrackballEvent(event);

    event->Recycle();
    if (sendDone) {
        FinishInputEvent();
    }

    if (LOCAL_LOGV || WATCH_POINTER)
        Logger::D(TAG, "Done dispatching!");

    return NOERROR;
}

ECode ViewRoot::HandleWindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean inTouchMode)
{
    if (mAdded) {
        mAttachInfo->mHasWindowFocus = hasFocus;
        if (hasFocus) {
            EnsureTouchModeLocally(inTouchMode);

            //if (mGlWanted) {
            //    CheckEglErrors();
            //    // we lost the gl context, so recreate it.
            //    if (mGlWanted && !mUseGL) {
            //        InitializeGL();
            //        if (mGlCanvas != NULL) {
            //            Float appScale = mAttachInfo->mApplicationScale;
            //            mGlCanvas->SetViewport(
            //                (Int32)(mWidth * appScale + 0.5f),
            //                (Int32)(mHeight * appScale + 0.5f));
            //        }
            //    }
            //}
        }

        mLastWasImTarget = CWindowManagerLayoutParams::MayUseInputMethod(
                mWindowAttributes->mFlags);

        AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
        if (mView != NULL) {
            if (hasFocus && imm != NULL && mLastWasImTarget) {
                imm->StartGettingWindowFocus(mView);
            }

            mAttachInfo->mKeyDispatchState->Reset();
            mView->DispatchWindowFocusChanged(hasFocus);
        }

        // Note: must be done after the focus change callbacks,
        // so all of the view state is set up correctly.
        if (hasFocus) {
            //if (imm != NULL && mLastWasImTarget) {
            //    imm.onWindowFocus(mView, mView.findFocus(),
            //        mWindowAttributes.softInputMode,
            //        !mHasHadWindowFocus, mWindowAttributes.flags);
            //}

            // Clear the forward bit.  We can just do this directly, since
            // the window manager doesn't care about it.
            //
            mWindowAttributes->mSoftInputMode &=
                ~WindowManagerLayoutParams_SOFT_INPUT_IS_FORWARD_NAVIGATION;

            AutoPtr<IWindowManagerLayoutParams> params;
            mView->GetLayoutParams((IViewGroupLayoutParams**)&params);

            ((CWindowManagerLayoutParams*)params.Get())->mSoftInputMode &=
                ~WindowManagerLayoutParams_SOFT_INPUT_IS_FORWARD_NAVIGATION;
            mHasHadWindowFocus = TRUE;
        }

        if (hasFocus && mView != NULL) {
            SendAccessibilityEvents();
        }
    }
    return NOERROR;
}

ECode ViewRoot::HandleCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    if (mView != NULL) {
        mView->OnCloseSystemDialogs(reason);
    }

    return NOERROR;
}

ECode ViewRoot::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode ViewRoot::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode ViewRoot::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode ViewRoot::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode ViewRoot::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode ViewRoot::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode ViewRoot::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode ViewRoot::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode ViewRoot::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode ViewRoot::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode ViewRoot::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    if (message == DISPATCH_KEY_FROM_IME) {
        void (STDCALL ViewRoot::*pHandlerFunc)(IKeyEvent*);
        pHandlerFunc = &ViewRoot::DispatchKeyFromIme;
        return SendMessage(*(Handle32*)&pHandlerFunc, params);
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}
