
#include "server/CWindowManagerService.h"
#include <Slogger.h>
#include "view/WindowManagerPolicy.h"
#include "view/ViewTreeObserver.h"
#include "graphics/ElPixelFormat.h"
#include "os/Process.h"
#include "os/SystemClock.h"
#include "os/Binder.h"
#include <elastos/Math.h>
#include <Logger.h>
#include "app/ActivityManagerNative.h"

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

#define UNUSED(x) (void)x

const char* CWindowManagerService::TAG = "CWindowManagerService";
const Int32 CWindowManagerService::TYPE_LAYER_MULTIPLIER;
const Int32 CWindowManagerService::TYPE_LAYER_OFFSET;
const Int32 CWindowManagerService::WINDOW_LAYER_MULTIPLIER;
const Int32 CWindowManagerService::MAX_ANIMATION_DURATION;
const Int32 CWindowManagerService::DEFAULT_DIM_DURATION;
const Int32 CWindowManagerService::DEFAULT_FADE_IN_OUT_DURATION;
const Int32 CWindowManagerService::DIM_DURATION_MULTIPLIER;
const Int32 CWindowManagerService::INJECTION_TIMEOUT_MILLIS;
const Int32 CWindowManagerService::UPDATE_FOCUS_NORMAL;
const Int32 CWindowManagerService::UPDATE_FOCUS_WILL_ASSIGN_LAYERS;
const Int32 CWindowManagerService::UPDATE_FOCUS_PLACING_SURFACES;
const Int32 CWindowManagerService::UPDATE_FOCUS_WILL_PLACE_SURFACES;
const Int32 CWindowManagerService::ADJUST_WALLPAPER_LAYERS_CHANGED;
const Int32 CWindowManagerService::ADJUST_WALLPAPER_VISIBILITY_CHANGED;
const Millisecond64 CWindowManagerService::WALLPAPER_TIMEOUT;
const Millisecond64 CWindowManagerService::WALLPAPER_TIMEOUT_RECOVERY;
const Int32 CWindowManagerService::ALLOW_DISABLE_YES;
const Int32 CWindowManagerService::ALLOW_DISABLE_NO;
const Int32 CWindowManagerService::ALLOW_DISABLE_UNKNOWN;
const Int64 CWindowManagerService::DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;

AutoPtr<IAnimation> CWindowManagerService::sDummyAnimation
    = new CWindowManagerService::DummyAnimation;

CWindowManagerService::WindowToken::WindowToken(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 type,
    /* [in] */ Boolean ex) :
    mToken(token),
    mWindowType(type),
    mExplicit(ex),
    mAppWindowToken(NULL),
    mPaused(FALSE),
    mHidden(FALSE),
    mHasVisible(FALSE),
    mWaitingToShow(FALSE),
    mWaitingToHide(FALSE),
    mSendingToBottom(FALSE),
    mSendingToTop(FALSE)
{
}

void CWindowManagerService::WindowToken::GetDescription(
    /* [out] */ String* description)
{
    assert(description != NULL);
    if (mStringName.IsNull()) {
        StringBuffer sb("WindowToken{");
//        sb.append(Integer.toHexString(System.identityHashCode(this)));
        sb += " token=";
        String tmp;
        if (mToken != NULL)
            mToken->GetDescription((String*)&tmp);
        sb += tmp;
        sb += "}";
        mStringName = (const char*)sb;
    }
    *description = mStringName;
}

Boolean CWindowManagerService::DummyAnimationBase::GetTransformation(
    /* [in] */ Int64 currentTime,
    /* [in] */ ITransformation* outTransformation)
{
    return FALSE;
}

IANIMATION_METHODS_IMPL(
    CWindowManagerService::DummyAnimation,
    CWindowManagerService::DummyAnimationBase,
    CWindowManagerService::DummyAnimationBase);

PInterface CWindowManagerService::DummyAnimation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IAnimation*)this;
    }
    else if (riid == EIID_IAnimation) {
        return (IAnimation*)this;
    }
    else if (riid == EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }

    return NULL;
}

UInt32 CWindowManagerService::DummyAnimation::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CWindowManagerService::DummyAnimation::Release()
{
    return ElRefBase::Release();
}

ECode CWindowManagerService::DummyAnimation::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID)
        return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IAnimation*)this) {
        *pIID = EIID_IAnimation;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CWindowManagerService::FadeInOutAnimationBase::FadeInOutAnimationBase(
    /* [in] */ Boolean fadeIn)
{
    AutoPtr<IInterpolator> interpolator;
    CAccelerateInterpolator::New((IInterpolator**)&interpolator);
    SetInterpolator(interpolator);
    SetDuration(DEFAULT_FADE_IN_OUT_DURATION);
    mFadeIn = fadeIn;
}

ECode CWindowManagerService::FadeInOutAnimationBase::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    // width is the screen width {@see AppWindowToken#stepAnimatinoLocked}
    mWidth = width;

    return NOERROR;
}

Int32 CWindowManagerService::FadeInOutAnimationBase::GetZAdjustment()
{
    return Animation_ZORDER_TOP;
}

void CWindowManagerService::FadeInOutAnimationBase::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    Float x = interpolatedTime;
    if (!mFadeIn) {
        x = 1.0f - x; // reverse the interpolation for fade out
    }

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    if (x < 0.5) {
        // move the window out of the screen.
        matrix->SetTranslate(mWidth, 0);
    }
    else {
        matrix->SetTranslate(0, 0);// show
        t->SetAlpha((x - 0.5f) * 2);
    }
}

IANIMATION_METHODS_IMPL(
    CWindowManagerService::FadeInOutAnimation,
    CWindowManagerService::FadeInOutAnimationBase,
    CWindowManagerService::FadeInOutAnimationBase);

CWindowManagerService::FadeInOutAnimation::FadeInOutAnimation(
    /* [in] */ Boolean fadeIn)
    : FadeInOutAnimationBase(fadeIn)
{
}

PInterface CWindowManagerService::FadeInOutAnimation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IAnimation*)this;
    }
    else if (riid == EIID_IAnimation) {
        return (IAnimation*)this;
    }
    else if (riid == EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }

    return NULL;
}

UInt32 CWindowManagerService::FadeInOutAnimation::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CWindowManagerService::FadeInOutAnimation::Release()
{
    return ElRefBase::Release();
}

ECode CWindowManagerService::FadeInOutAnimation::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID)
        return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IAnimation*)this) {
        *pIID = EIID_IAnimation;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CWindowManagerService::StartingData::StartingData(
    /* [in] */ const String& cap,
    /* [in] */ Int32 theme,
    /* [in] */ ICharSequence* nonLocalizedLabel,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 icon)
    : mCap(cap)
    , mTheme(theme)
    , mLabelRes(labelRes)
    , mIcon(icon)
    , mNonLocalizedLabel(nonLocalizedLabel)
{}

CWindowManagerService::StartingData::~StartingData()
{}

CWindowManagerService::AppWindowToken::AppWindowToken(
    /* [in] */ CWindowManagerService* wmService,
    /* [in] */ IApplicationToken* token) :
    WindowToken((IBinder*)token, WindowManagerLayoutParams_TYPE_APPLICATION, TRUE),
    mWMService(wmService),
    mAppToken(token),
    mGroupId(-1),
    mAppFullscreen(FALSE),
    mRequestedOrientation(ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED),
    mInPendingTransaction(FALSE),
    mAllDrawn(FALSE),
    mWillBeHidden(FALSE),
    mHiddenRequested(FALSE),
    mClientHidden(FALSE),
    mReportedVisible(FALSE),
    mRemoved(FALSE),
    mFreezingScreen(FALSE),
    mAnimating(FALSE),
    mStartingData(NULL),
    mStartingWindow(NULL),
    mStartingDisplayed(FALSE),
    mStartingMoved(FALSE),
    mFirstWindowDrawn(FALSE)
{
    mLastTransactionSequence = mWMService->mTransactionSequence - 1;
    mAppWindowToken = this;
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTransformation));
}

CWindowManagerService::CWindowManagerService() :
    mKeyguardDisabled(FALSE),
    mAllowDisableKeyguard(ALLOW_DISABLE_UNKNOWN),
    mLimitedAlphaCompositing(FALSE),
    mBlurShown(FALSE),
    mTransactionSequence(0),
    mSafeMode(FALSE),
    mDisplayEnabled(FALSE),
    mSystemBooted(FALSE),
    mInitialDisplayWidth(0),
    mInitialDisplayHeight(0),
    mRotation(0),
    mRequestedRotation(0),
    mForcedAppOrientation(ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED),
    mLayoutNeeded(TRUE),
    mAnimationPending(FALSE),
    mDisplayFrozen(FALSE),
    mWaitingForConfig(FALSE),
    mWindowsFreezingScreen(FALSE),
    mFreezeGcPending(0),
    mAppsFreezingScreen(0),
    mLayoutSeq(0),
    mFocusMayChange(FALSE),
    mNextAppTransition(WindowManagerPolicy::TRANSIT_UNSET),
    mAppTransitionReady(FALSE),
    mAppTransitionRunning(FALSE),
    mAppTransitionTimeout(FALSE),
    mStartingIconInTransition(FALSE),
    mSkipAppTransitionAnimation(FALSE),
    mInputMethodTargetWaitingAnim(FALSE),
    mLastWallpaperX(-1),
    mLastWallpaperY(-1),
    mLastWallpaperXStep(-1),
    mLastWallpaperYStep(-1),
    mWindowAnimationScale(1.0),
    mTransitionAnimationScale(1.0),
    mTurnOnScreen(FALSE),
    mInTouchMode(FALSE),
    mWindowsChanged(FALSE),
    mScreenLayout(Configuration_SCREENLAYOUT_SIZE_UNDEFINED),
    mBackgroundFillerShown(FALSE),
    mInLayout(FALSE)
{
    AutoPtr<IApartmentHelper> helper;
    assert(SUCCEEDED(CApartmentHelper::AcquireSingleton((IApartmentHelper**)&helper)));
    assert(SUCCEEDED(helper->GetMainApartment((IApartment**)&mApartment))
            && (mApartment != NULL));

//    WindowManagerPolicy mPolicy = PolicyManager.makeNewWindowManager();
    AutoPtr<IPolicyManager> pm;
    assert(SUCCEEDED(CPolicyManager::AcquireSingleton((IPolicyManager**)&pm)));
    assert(SUCCEEDED(pm->MakeNewWindowManager((IWindowManagerPolicy**)&mPolicy)));
    assert(SUCCEEDED(CConfiguration::New(
            (IConfiguration**)&mCurConfiguration)));
    assert(SUCCEEDED(CConfiguration::New(
            (IConfiguration**)&mTempConfiguration)));

    assert(SUCCEEDED(CRect::New((IRect**)&mCompatibleScreenFrame)));

    ActivityManagerNative::GetDefault((IActivityManager**)&mActivityManager);
    mInputMonitor = new InputMonitor(this);

    mInputManager = new InputManager(NULL/*context*/, this);

    assert(SUCCEEDED(mInputManager->Start()));

    mTmpFloats = ArrayOf<Float>::Alloc(9);
}

CWindowManagerService::~CWindowManagerService()
{
    ArrayOf<Float>::Free(mTmpFloats);
}

CWindowManagerService::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ Boolean haveInputMethod)
{
    mContext = ctx;
    mHaveInputMethods = haveInputMethod;
}

//CWindowManagerService::CWindowManagerService(
//    /* [in] */ IContext* context,
//    /* [in] */ /*PowerManagerService pm,*/
//    /* [in] */ Boolean haveInputMethods) :
//    mContext(context),
//    mHaveInputMethods(haveInputMethods),
//    mKeyguardDisabled(FALSE)
//{
////    mBroadcastReceiver = new BroadcastReceiver() {
////    @Override
////    public void onReceive(Context context, Intent intent) {
////        mPolicy.enableKeyguard(true);
////        synchronized(mKeyguardTokenWatcher) {
////            // lazily evaluate this next time we're asked to disable keyguard
////            mAllowDisableKeyguard = ALLOW_DISABLE_UNKNOWN;
////            mKeyguardDisabled = false;
////        }
////    }
////    };
//
////    mPolicy = PolicyManager.makeNewWindowManager();
//
//
//}

Boolean CWindowManagerService::CanBeImeTarget(
    /* [in] */ WindowState* w)
{
    Int32 flags;
    w->mAttrs->GetFlags(&flags);
    Int32 fl = flags
            & (WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE | WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM);
    if (fl == 0 || fl ==
            (WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE|WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM)) {
        return w->IsVisibleOrAdding();
    }
    return FALSE;
}

List<AutoPtr<CWindowManagerService::WindowState> >::Iterator
CWindowManagerService::FindDesiredInputMethodWindowIndexLocked(
    /* [in] */ Boolean willMove)
{
    List<AutoPtr<WindowState> >& localmWindows = mWindows;
    AutoPtr<WindowState> w;
    List<AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = localmWindows.RBegin(); rit != localmWindows.REnd(); ++rit) {
        w = *rit;

        //Slog.i(TAG, "Checking window @" + i + " " + w + " fl=0x"
        //        + Integer.toHexString(w.mAttrs.flags));
        if (CanBeImeTarget(w)) {
            //Slog.i(TAG, "Putting input method here!");

            // Yet more tricksyness!  If this window is a "starting"
            // window, we do actually want to be on top of it, but
            // it is not -really- where input will go.  So if the caller
            // is not actually looking to move the IME, look down below
            // for a real window to target...
            List<AutoPtr<WindowState> >::ReverseIterator temp = rit;
            Int32 type;
            w->mAttrs->GetType(&type);
            if (!willMove
                    && type == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING
                    && ++temp != localmWindows.REnd()) {
                AutoPtr<WindowState> wb = *temp;
                if (wb->mAppToken == w->mAppToken && CanBeImeTarget(wb)) {
                    ++rit;
                    w = wb;
                }
            }
            break;
        }
    }

    mUpcomingInputMethodTarget = w;

//    if (DEBUG_INPUT_METHOD) Slog.v(TAG, "Desired input method target="
//            + w + " willMove=" + willMove);

    if (willMove && w != NULL) {
        AutoPtr<WindowState> curTarget = mInputMethodTarget;
        if (curTarget != NULL && curTarget->mAppToken != NULL) {

            // Now some fun for dealing with window animations that
            // modify the Z order.  We need to look at all windows below
            // the current target that are in this app, finding the highest
            // visible one in layering.
            AppWindowToken* token = curTarget->mAppToken;
            AutoPtr<WindowState> highestTarget;
            List<AutoPtr<WindowState> >::Iterator highestPosIt;
            if (token->mAnimating || token->mAnimation != NULL) {
                List<AutoPtr<WindowState> >::ReverseIterator posRIt;
                for (posRIt = localmWindows.RBegin(); posRIt != localmWindows.REnd(); ++posRIt) {
                    if (*posRIt == curTarget) {
                        break;
                    }
                }
                for (; posRIt != localmWindows.REnd(); ++posRIt) {
                    AutoPtr<WindowState> win = *posRIt;
                    if (win->mAppToken != token) {
                        break;
                    }
                    if (!win->mRemoved) {
                        if (highestTarget == NULL || win->mAnimLayer >
                                highestTarget->mAnimLayer) {
                            highestTarget = win;
                            highestPosIt = (++posRIt).GetBase();
                        }
                    }
                }
            }

            if (highestTarget != NULL) {
//                if (DEBUG_INPUT_METHOD) Slog.v(TAG, "mNextAppTransition="
//                        + mNextAppTransition + " " + highestTarget
//                        + " animating=" + highestTarget.isAnimating()
//                        + " layer=" + highestTarget.mAnimLayer
//                        + " new layer=" + w.mAnimLayer);

                if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
                    // If we are currently setting up for an animation,
                    // hold everything until we can find out what will happen.
                    mInputMethodTargetWaitingAnim = TRUE;
                    mInputMethodTarget = highestTarget;
                    return ++highestPosIt;
                }
                else if (highestTarget->IsAnimating() &&
                        highestTarget->mAnimLayer > w->mAnimLayer) {
                    // If the window we are currently targeting is involved
                    // with an animation, and it is on top of the next target
                    // we will be over, then hold off on moving until
                    // that is done.
                    mInputMethodTarget = highestTarget;
                    return ++highestPosIt;
                }
            }
        }
    }

    //Slog.i(TAG, "Placing input method @" + (i+1));
    if (w != NULL) {
        if (willMove) {
//            if (DEBUG_INPUT_METHOD) {
//                RuntimeException e = NULL;
//                if (!HIDE_STACK_CRAWLS) {
//                    e = new RuntimeException();
//                    e.fillInStackTrace();
//                }
//                Slog.w(TAG, "Moving IM target from "
//                        + mInputMethodTarget + " to " + w, e);
//            }
            mInputMethodTarget = w;
            if (w->mAppToken != NULL) {
                SetInputMethodAnimLayerAdjustment(w->mAppToken->mAnimLayerAdjustment);
            }
            else {
                SetInputMethodAnimLayerAdjustment(0);
            }
        }
        return rit.GetBase();
    }
    if (willMove) {
//        if (DEBUG_INPUT_METHOD) {
//            RuntimeException e = NULL;
//            if (!HIDE_STACK_CRAWLS) {
//                e = new RuntimeException();
//                e.fillInStackTrace();
//            }
//            Slog.w(TAG, "Moving IM target from "
//                    + mInputMethodTarget + " to NULL", e);
//        }
        mInputMethodTarget = NULL;
        SetInputMethodAnimLayerAdjustment(0);
    }

    return List<AutoPtr<WindowState> >::Iterator(NULL);  // -1
}

void CWindowManagerService::AddInputMethodWindowToListLocked(
    /* [in] */ WindowState* win)
{
    List<AutoPtr<WindowState> >::Iterator posIt =
            FindDesiredInputMethodWindowIndexLocked(TRUE);
    if (posIt != List<AutoPtr<WindowState> >::Iterator(NULL)) {
        win->mTargetAppToken = mInputMethodTarget->mAppToken;
//        if (DEBUG_WINDOW_MOVEMENT) Slog.v(
//                TAG, "Adding input method window " + win + " at " + pos);
        mWindows.Insert(posIt, win);
        mWindowsChanged = TRUE;
        MoveInputMethodDialogsLocked(++posIt);
        return;
    }
    win->mTargetAppToken = NULL;
    AddWindowToListInOrderLocked(win, TRUE);
    MoveInputMethodDialogsLocked(posIt);
}

void CWindowManagerService::SetInputMethodAnimLayerAdjustment(
    /* [in] */ Int32 adj)
{
//    if (DEBUG_LAYERS) Slog.v(TAG, "Setting im layer adj to " + adj);
    mInputMethodAnimLayerAdjustment = adj;
    AutoPtr<WindowState> imw = mInputMethodWindow;
    List<AutoPtr<WindowState> >::ReverseIterator rit;
    if (imw != NULL) {
        imw->mAnimLayer = imw->mLayer + adj;
//        if (DEBUG_LAYERS) Slog.v(TAG, "IM win " + imw
//                + " anim layer: " + imw.mAnimLayer);
        for (rit = imw->mChildWindows.RBegin(); rit != imw->mChildWindows.REnd(); ++rit) {
            AutoPtr<WindowState> cw = *rit;
            cw->mAnimLayer = cw->mLayer + adj;
//            if (DEBUG_LAYERS) Slog.v(TAG, "IM win " + cw
//                    + " anim layer: " + cw.mAnimLayer);
        }
    }
    for (rit = mInputMethodDialogs.RBegin(); rit != mInputMethodDialogs.REnd(); ++rit) {
        imw = *rit;
        imw->mAnimLayer = imw->mLayer + adj;
//        if (DEBUG_LAYERS) Slog.v(TAG, "IM win " + imw
//                + " anim layer: " + imw.mAnimLayer);
    }
}

List<AutoPtr<CWindowManagerService::WindowState> >::Iterator
CWindowManagerService::TmpRemoveWindowLocked(
    /* [in] */ List<AutoPtr<CWindowManagerService::WindowState> >::Iterator interestingPosIt,
    /* [in] */ WindowState* win)
{
    Boolean isBefore = TRUE;
    AutoPtr<WindowState> posWin;
    List<AutoPtr<WindowState> >::Iterator wposIt;
    for (wposIt = mWindows.Begin(); wposIt != mWindows.End(); ++wposIt) {
        if (wposIt == interestingPosIt && isBefore == TRUE) isBefore= FALSE;
        AutoPtr<WindowState> w = *wposIt;
        if (w.Get() == win) {
            if (isBefore) interestingPosIt--;
            posWin = w;
            break;
        }
    }
    if (posWin) {
//        if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Temp removing at " + wpos + ": " + win);
        mWindows.Erase(wposIt);
        mWindowsChanged = TRUE;
        List<AutoPtr<WindowState> >::ReverseIterator rit = win->mChildWindows.RBegin();
        for (; rit != win->mChildWindows.REnd(); ++rit) {
            AutoPtr<WindowState> cw = *rit;
            for (wposIt = mWindows.Begin(); wposIt != mWindows.End(); ++wposIt) {
                if (wposIt == interestingPosIt && isBefore == TRUE) isBefore= FALSE;
                AutoPtr<WindowState> w = *wposIt;
                if (w == cw) {
                    if (isBefore) interestingPosIt--;
//                    if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Temp removing child at "
//                            + cpos + ": " + cw);
                    break;
                }
            }
            mWindows.Erase(wposIt);
        }
    }
    return interestingPosIt;
}


void CWindowManagerService::ReAddWindowToListInOrderLocked(
    /* [in] */ WindowState* win)
{
    AddWindowToListInOrderLocked(win, FALSE);
    // This is a hack to get all of the child windows added as well
    // at the right position.  Child windows should be rare and
    // this case should be rare, so it shouldn't be that big a deal.
    List<AutoPtr<WindowState> >::Iterator it = Find(
        mWindows.Begin(), mWindows.End(), AutoPtr<WindowState>(win));
    if (it != mWindows.End()) {
//        if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "ReAdd removing from " + wpos
//                + ": " + win);
        mWindows.Erase(it);
        mWindowsChanged = TRUE;
        ReAddWindowLocked(it, win);
    }
}

void CWindowManagerService::LogWindowList(
    /* [in] */ const String& prefix)
{
    List<AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = mWindows.RBegin(); rit != mWindows.REnd(); ++rit) {
//        Slog.v(TAG, prefix + "#" + N + ": " + mWindows.get(N));
    }
}

void CWindowManagerService::MoveInputMethodDialogsLocked(
    /* [in] */ List<AutoPtr<CWindowManagerService::WindowState> >::Iterator posIt)
{
    List<AutoPtr<WindowState> > dialogs = mInputMethodDialogs;
    List<AutoPtr<WindowState> >::Iterator it;
//    if (DEBUG_INPUT_METHOD) Slog.v(TAG, "Removing " + N + " dialogs w/pos=" + pos);
    for (it = dialogs.Begin(); it != dialogs.End(); ++it) {
        posIt = TmpRemoveWindowLocked(posIt, *it);
    }
//    if (DEBUG_INPUT_METHOD) {
//        Slog.v(TAG, "Window list w/pos=" + pos);
//        logWindowList("  ");
//    }

    if (posIt != List<AutoPtr<WindowState> >::Iterator(NULL)) {
        AppWindowToken* targetAppToken = mInputMethodTarget->mAppToken;
        if (posIt != mWindows.End()) {
            AutoPtr<WindowState> wp = *posIt;
            if (wp == mInputMethodWindow) {
                ++posIt;
            }
        }
//        if (DEBUG_INPUT_METHOD) Slog.v(TAG, "Adding " + N + " dialogs at pos=" + pos);
        for (it = dialogs.Begin(); it != dialogs.End(); ++it) {
            AutoPtr<WindowState> win = *it;
            win->mTargetAppToken = targetAppToken;
            posIt = ReAddWindowLocked(posIt, win);
        }
//        if (DEBUG_INPUT_METHOD) {
//            Slog.v(TAG, "Final window list:");
//            logWindowList("  ");
//        }
        return;
    }
    for (it = dialogs.Begin(); it != dialogs.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        win->mTargetAppToken = NULL;
        ReAddWindowToListInOrderLocked(win);
//        if (DEBUG_INPUT_METHOD) {
//            Slog.v(TAG, "No IM target, final list:");
//            logWindowList("  ");
//        }
    }
}

void CWindowManagerService::AdjustInputMethodDialogsLocked()
{
    MoveInputMethodDialogsLocked(FindDesiredInputMethodWindowIndexLocked(TRUE));
}

Boolean CWindowManagerService::IsWallpaperVisible(
    /* [in] */ WindowState* wallpaperTarget)
{
//    if (DEBUG_WALLPAPER) Slog.v(TAG, "Wallpaper vis: target obscured="
//            + (wallpaperTarget != NULL ? Boolean.toString(wallpaperTarget.mObscured) : "??")
//            + " anim=" + ((wallpaperTarget != NULL && wallpaperTarget.mAppToken != NULL)
//                    ? wallpaperTarget.mAppToken.animation : NULL)
//            + " upper=" + mUpperWallpaperTarget
//            + " lower=" + mLowerWallpaperTarget);
    return (wallpaperTarget != NULL
            && (!wallpaperTarget->mObscured || (wallpaperTarget->mAppToken != NULL
            && wallpaperTarget->mAppToken->mAnimation != NULL)))
            || mUpperWallpaperTarget != NULL
            || mLowerWallpaperTarget != NULL;
}

void CWindowManagerService::SetWallpaperAnimLayerAdjustmentLocked(
    /* [in] */ Int32 adj)
{
//    if (DEBUG_LAYERS || DEBUG_WALLPAPER) Slog.v(TAG,
//            "Setting wallpaper layer adj to " + adj);
    mWallpaperAnimLayerAdjustment = adj;
    List<WindowToken*>::ReverseIterator rit;
    for (rit = mWallpaperTokens.RBegin(); rit != mWallpaperTokens.REnd(); ++rit) {
        WindowToken* token = *rit;
        List<AutoPtr<WindowState> >::ReverseIterator writ;
        for (writ = token->mWindows.RBegin(); writ != token->mWindows.REnd(); ++writ) {
            AutoPtr<WindowState> wallpaper = *writ;
            wallpaper->mAnimLayer = wallpaper->mLayer + adj;
//            if (DEBUG_LAYERS || DEBUG_WALLPAPER) Slog.v(TAG, "Wallpaper win "
//                    + wallpaper + " anim layer: " + wallpaper.mAnimLayer);
        }
    }
}

void CWindowManagerService::WallpaperOffsetsComplete(
    /* [in] */ IBinder* window)
{
    Mutex::Autolock lock(mWindowMapLock);

    if (mWaitingOnWallpaper != NULL &&
            (IBinder*)(mWaitingOnWallpaper->mClient) == window) {
        mWaitingOnWallpaper = NULL;
//        mWindowMap.notifyAll();//????????????????????
    }
}

void CWindowManagerService::RemoveWindow(
    /* [in] */ CWindowSession* session,
    /* [in] */ IInnerWindow* client)
{
    Mutex::Autolock lock(mWindowMapLock);

    AutoPtr<WindowState> win = WindowForClientLocked(session, client, FALSE);
    if (win == NULL) {
        return;
    }
    RemoveWindowLocked(session, win);
}

void CWindowManagerService::SetTransparentRegionWindow(
    /* [in] */ CWindowSession* session,
    /* [in] */ IInnerWindow* client,
    /* [in] */ IRegion* region)
{
//    long origId = Binder.clearCallingIdentity();
//    try {
    Mutex::Autolock lock(mWindowMapLock);

    AutoPtr<WindowState> w = WindowForClientLocked(session, client, FALSE);
    if ((w != NULL) && (w->mSurface != NULL)) {
//        if (SHOW_TRANSACTIONS) Slog.i(TAG, ">>> OPEN TRANSACTION");
        AutoPtr<ISurfaceHelper> helper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
        helper->OpenTransaction();
//        try {
//            if (SHOW_TRANSACTIONS) logSurface(w,
//                    "transparentRegionHint=" + region, NULL);
        w->mSurface->SetTransparentRegionHint(region);
//        if (SHOW_TRANSACTIONS) Slog.i(TAG, "<<< CLOSE TRANSACTION");
        helper->CloseTransaction();
//        } finally {
//            if (SHOW_TRANSACTIONS) Slog.i(TAG, "<<< CLOSE TRANSACTION");
//            Surface.closeTransaction();
//        }
    }
//    } finally {
//        Binder.restoreCallingIdentity(origId);
//    }
}

void CWindowManagerService::SetInsetsWindow(
    /* [in] */ CWindowSession* session,
    /* [in] */ IInnerWindow* client,
    /* [in] */ Int32 touchableInsets,
    /* [in] */ IRect* contentInsets,
    /* [in] */ IRect* visibleInsets)
{
//    long origId = Binder.clearCallingIdentity();
//    try {
    Mutex::Autolock lock(mWindowMapLock);

    AutoPtr<WindowState> w = WindowForClientLocked(session, client, FALSE);
    if (w != NULL) {
        w->mGivenInsetsPending = FALSE;
        w->mGivenContentInsets->SetEx(contentInsets);
        w->mGivenVisibleInsets->SetEx(visibleInsets);
        w->mTouchableInsets = touchableInsets;
        mLayoutNeeded = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
    }
//    } finally {
//        Binder.restoreCallingIdentity(origId);
//    }
}

void CWindowManagerService::GetWindowDisplayFrame(
    /* [in] */ CWindowSession* session,
    /* [in] */ IInnerWindow* client,
    /* [in] */ IRect* outDisplayFrame)
{
    Mutex::Autolock lock(mWindowMapLock);

    AutoPtr<WindowState> win = WindowForClientLocked(session, client, FALSE);
    if (win == NULL) {
        outDisplayFrame->SetEmpty();
        return;
    }
    outDisplayFrame->SetEx(win->mDisplayFrame);
}

void CWindowManagerService::SetWindowWallpaperPositionLocked(
    /* [in] */ WindowState* window,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep)
{
    if (window->mWallpaperX != x || window->mWallpaperY != y)  {
        window->mWallpaperX = x;
        window->mWallpaperY = y;
        window->mWallpaperXStep = xStep;
        window->mWallpaperYStep = yStep;
        if (UpdateWallpaperOffsetLocked(window, TRUE)) {
            PerformLayoutAndPlaceSurfacesLocked();
        }
    }
}

void CWindowManagerService::WallpaperCommandComplete(
    /* [in] */ IBinder* window,
    /* [in] */ IBundle* result)
{
    Mutex::Autolock lock(mWindowMapLock);

    if (mWaitingOnWallpaper != NULL &&
            (IBinder*)(mWaitingOnWallpaper->mClient) == window) {
        mWaitingOnWallpaper = NULL;
//        mWindowMap.notifyAll();
    }
}

AutoPtr<IBundle> CWindowManagerService::SendWindowWallpaperCommandLocked(
    /* [in] */ WindowState* window,
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    if (window == mWallpaperTarget || window == mLowerWallpaperTarget
            || window == mUpperWallpaperTarget) {
        Boolean doWait = sync;
        List<WindowToken*>::ReverseIterator rit;
        for (rit = mWallpaperTokens.RBegin(); rit != mWallpaperTokens.REnd(); ++rit) {
            WindowToken* token = *rit;
            List<AutoPtr<WindowState> >::ReverseIterator wRIt;
            for (wRIt = token->mWindows.RBegin(); wRIt != token->mWindows.REnd(); ++wRIt) {
                AutoPtr<WindowState> wallpaper = *wRIt;
//                try {
                wallpaper->mClient->DispatchWallpaperCommand(action,
                        x, y, z, extras, sync);
                // We only want to be synchronous with one wallpaper.
                sync = FALSE;
//                } catch (RemoteException e) {
//                }
            }
        }

        if (doWait) {
            // XXX Need to wait for result.
        }
    }

    return NULL;
}

void CWindowManagerService::FinishDrawingWindow(
    /* [in] */ CWindowSession* session,
    /* [in] */ IInnerWindow* client)
{
//    final long origId = Binder.clearCallingIdentity();
    Mutex::Autolock lock(mWindowMapLock);

    AutoPtr<WindowState> win = WindowForClientLocked(session, client, FALSE);
    if (win != NULL && win->FinishDrawingLocked()) {
        Int32 flags;
        win->mAttrs->GetFlags(&flags);
        if ((flags & WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER) != 0) {
            AdjustWallpaperWindowsLocked();
        }
        mLayoutNeeded = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
    }

//    Binder.restoreCallingIdentity(origId);
}

Boolean CWindowManagerService::CheckCallingPermission(
    /* [in] */ CString permission,
    /* [in] */ CString func)
{
    // Quick check: if the calling permission is me, it's all okay.
    if (Binder::GetCallingPid() == Process::MyPid()) {
      return TRUE;
    }

    Int32 perm;
    mContext->CheckCallingPermission(permission, &perm);
    if (perm == CapsuleManager_PERMISSION_GRANTED) {
      return TRUE;
    }

    // String msg = "Permission Denial: " + func + " from pid="
    //        + Binder.getCallingPid()
    //        + ", uid=" + Binder.getCallingUid()
    //        + " requires " + permission;
    // Slog.w(TAG, msg);
    return FALSE;
}

ECode CWindowManagerService::AddWindowToken(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 type)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
           String("AddWindowToken()"))) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);

    WindowToken* wtoken = NULL;
    HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator tokenIt = mTokenMap.Find(token);
    if (tokenIt != mTokenMap.End()) {
        wtoken = tokenIt->mSecond;
    }

    if (wtoken != NULL) {
//        Slog.w(TAG, "Attempted to add existing input method token: " + token);
        return NOERROR;
    }
    wtoken = new WindowToken(token, type, TRUE);
    mTokenMap[token] = wtoken;
    mTokenList.PushBack(wtoken);
    if (type == WindowManagerLayoutParams_TYPE_WALLPAPER) {
        mWallpaperTokens.PushBack(wtoken);
    }

    return NOERROR;
}

ECode CWindowManagerService::RemoveWindowToken(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
           "RemoveWindowToken()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

//    long origId = Binder.clearCallingIdentity();
    Mutex::Autolock lock(mWindowMapLock);

    WindowToken* wtoken = NULL;
    HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator tokenIt = mTokenMap.Find(token);
    if (tokenIt != mTokenMap.End()) {
        wtoken = tokenIt->mSecond;
    }
    mTokenMap.Erase(tokenIt);
    mTokenList.Remove(wtoken);
    if (wtoken == NULL) {
//        Slog.w(TAG, "Attempted to remove non-existing token: " + token);
    }
    else {
        Boolean delayed = FALSE;
        if (!wtoken->mHidden) {
            wtoken->mHidden = TRUE;

            Boolean changed = FALSE;

            List<AutoPtr<WindowState> >::Iterator it = wtoken->mWindows.Begin();
            for (; it != wtoken->mWindows.End(); ++it) {
                AutoPtr<WindowState> win = *it;

                if (win->IsAnimating()) {
                    delayed = TRUE;
                }

                if (win->IsVisibleNow()) {
                    ApplyAnimationLocked(win,
                            WindowManagerPolicy::TRANSIT_EXIT, FALSE);
                    changed = TRUE;
                }
            }

            if (changed) {
                mLayoutNeeded = TRUE;
                PerformLayoutAndPlaceSurfacesLocked();
                UpdateFocusedWindowLocked(UPDATE_FOCUS_NORMAL);
            }

            if (delayed) {
                mExitingTokens.PushBack(wtoken);
            }
            else if (wtoken->mWindowType == WindowManagerLayoutParams_TYPE_WALLPAPER) {
                mWallpaperTokens.Remove(wtoken);
            }
        }

        mInputMonitor->UpdateInputWindowsLw();
    }

    return NOERROR;
//    Binder.restoreCallingIdentity(origId);
}

ECode CWindowManagerService::AddAppToken(
    /* [in] */ Int32 addPos,
    /* [in] */ IApplicationToken* token,
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 requestedOrientation,
    /* [in] */ Boolean fullscreen)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
           "AddAppToken()")) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    // Get the dispatching timeout here while we are not holding any locks so that it
    // can be cached by the AppWindowToken.  The timeout value is used later by the
    // input dispatcher in code that does hold locks.  If we did not cache the value
    // here we would run the chance of introducing a deadlock between the window manager
    // (which holds locks while updating the input dispatcher state) and the activity manager
    // (which holds locks while querying the application token).
//    long inputDispatchingTimeoutNanos;
//    try {
//        inputDispatchingTimeoutNanos = token.getKeyDispatchingTimeout() * 1000000L;
//    } catch (RemoteException ex) {
//        Slogger::W(TAG, "Could not get dispatching timeout.", ex);
//        inputDispatchingTimeoutNanos = DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;
//    }

    Mutex::Autolock lock(mWindowMapLock);

    AppWindowToken* wtoken = FindAppWindowToken(token);
    if (wtoken != NULL) {
        String appDes;
        token->GetDescription(&appDes);
        Slogger::W(TAG, StringBuffer("Attempted to add existing app token: ")
                + appDes);
        return NOERROR;
    }

    wtoken = new AppWindowToken(this, token);
//    wtoken.inputDispatchingTimeoutNanos = inputDispatchingTimeoutNanos;
    wtoken->mGroupId = groupId;
    wtoken->mAppFullscreen = fullscreen;
//    wtoken.mRequestedOrientation = requestedOrientation;
    mAppTokens.Insert(addPos, wtoken);
//    if (localLOGV) Slogger::V(TAG, "Adding new app token: " + wtoken);
    mTokenMap[token] = wtoken;
    mTokenList.PushBack(wtoken);
//
    // Application tokens start out hidden.
    wtoken->mHidden = TRUE;
    wtoken->mHiddenRequested = TRUE;

    return NOERROR;
}

ECode CWindowManagerService::SetAppGroupId(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 groupId)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
        "SetAppStartingIcon()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);

    AppWindowToken* wtoken = FindAppWindowToken(token);
    if (wtoken == NULL) {
//        Slog.w(TAG, "Attempted to set group id of non-existing app token: " + token);
        return NOERROR;
    }
    wtoken->mGroupId = groupId;

    return NOERROR;
}

Int32 CWindowManagerService::GetOrientationFromWindowsLocked()
{
    List<AutoPtr<WindowState> >::ReverseIterator rit = mWindows.RBegin();
    while (rit != mWindows.REnd()) {
        AutoPtr<WindowState> wtoken = *rit;
        ++rit;
        if (wtoken->mAppToken != NULL) {
            // We hit an application window. so the orientation will be determined by the
            // app window. No point in continuing further.
            return ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED;
        }
        if (!wtoken->IsVisibleLw() || !wtoken->mPolicyVisibilityAfterAnim) {
            continue;
        }

        Int32 req;
        wtoken->mAttrs->GetScreenOrientation(&req);
        if((req == ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED) ||
                (req == ActivityInfo_SCREEN_ORIENTATION_BEHIND)){
            continue;
        }
        else {
            return req;
        }
    }
    return ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED;
}

Int32 CWindowManagerService::GetOrientationFromAppTokensLocked()
{
    Int32 curGroup = 0;
    Int32 lastOrientation = ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED;
    Boolean findingBehind = FALSE;
    Boolean haveGroup = FALSE;
    Boolean lastFullscreen = FALSE;
    List<AppWindowToken*>::ReverseIterator rit;
    for (rit = mAppTokens.RBegin(); rit != mAppTokens.REnd(); ++rit) {
        AppWindowToken* wtoken = *rit;
        // if we're about to tear down this window and not seek for
        // the behind activity, don't use it for orientation
        if (!findingBehind
                && (!wtoken->mHidden && wtoken->mHiddenRequested)) {
            continue;
        }

        if (!haveGroup) {
            // We ignore any hidden applications on the top.
            if (wtoken->mHiddenRequested || wtoken->mWillBeHidden) {
                continue;
            }
            haveGroup = TRUE;
            curGroup = wtoken->mGroupId;
            lastOrientation = wtoken->mRequestedOrientation;
        }
        else if (curGroup != wtoken->mGroupId) {
            // If we have hit a new application group, and the bottom
            // of the previous group didn't explicitly say to use
            // the orientation behind it, and the last app was
            // full screen, then we'll stick with the
            // user's orientation.
            if (lastOrientation != ActivityInfo_SCREEN_ORIENTATION_BEHIND
                    && lastFullscreen) {
                return lastOrientation;
            }
        }
        Int32 orientation = wtoken->mRequestedOrientation;
        // If this application is fullscreen, and didn't explicitly say
        // to use the orientation behind it, then just take whatever
        // orientation it has and ignores whatever is under it.
        lastFullscreen = wtoken->mAppFullscreen;
        if (lastFullscreen
                && orientation != ActivityInfo_SCREEN_ORIENTATION_BEHIND) {
            return orientation;
        }
        // If this application has requested an explicit orientation,
        // then use it.
        if (orientation != ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED
                && orientation != ActivityInfo_SCREEN_ORIENTATION_BEHIND) {
            return orientation;
        }
        findingBehind |= (orientation == ActivityInfo_SCREEN_ORIENTATION_BEHIND);
    }
    return ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED;
}

// This is used for debugging
ECode CWindowManagerService::StartViewServer(
    /* [in] */ Int32 port,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

//    if (IsSystemSecure()) {
//        *result = FALSE;
//        return NOERROR;
//    }
//

    if (!CheckCallingPermission("elastos.permission.DUMP", /*Manifest.permission.DUMP,*/
            "StartViewServer")) {
        *result = FALSE;
        return NOERROR;
    }
//
//    if (port < 1024) {
//        *result = FALSE;
//        return NOERROR;
//    }
//
//    ECode ec = NOERROR;
//    if (mViewServer != NULL) {
//        Boolean isRun;
//        if (!mViewServer->IsRunning()) {
////            try {
//            ec = mViewServer->Start(result);
//            if (SUCCEEDED(ec)) return ec;
////            Slog.w(TAG, "View server did not start");
////            } catch (IOException e) {
////                Slog.w(TAG, "View server did not start");
////            }
//        }
//        *result = FALSE;
//        return NOERROR;
//    }
//
////    try {
//    mViewServer = new ViewServer(this, port);
//    return mViewServer->Start(result);
////    } catch (IOException e) {
////        Slog.w(TAG, "View server did not start");
////    }
    return E_NOT_IMPLEMENTED;
}

Boolean CWindowManagerService::IsSystemSecure()
{
//    return "1".equals(SystemProperties.get(SYSTEM_SECURE, "1")) &&
//            "0".equals(SystemProperties.get(SYSTEM_DEBUGGABLE, "0"));

    return FALSE;
}

ECode CWindowManagerService::StopViewServer(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (IsSystemSecure()) {
        *result = FALSE;
        return NOERROR;
    }

    if (!CheckCallingPermission("elastos.permission.DUMP", /*Manifest.permission.DUMP,*/
            "StopViewServer")) {
        *result = FALSE;
        return NOERROR;
    }
//
//    if (mViewServer != NULL) {
//        *result = mViewServer->Stop();
//        return NOERROR;
//    }
    *result = FALSE;
    return NOERROR;
}

ECode CWindowManagerService::IsViewServerRunning(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (IsSystemSecure()) {
        *result = FALSE;
        return NOERROR;
    }

    if (!CheckCallingPermission("elastos.permission.Dump", /*Manifest.permission.DUMP,*/
            "IsViewServerRunning")) {
        *result = FALSE;
        return NOERROR;
    }

//    return mViewServer != NULL && mViewServer->IsRunning();
    return FALSE;
}

/**
 * Lists all availble windows in the system. The listing is written in the
 * specified Socket's output stream with the following syntax:
 * windowHashCodeInHexadecimal windowName
 * Each line of the ouput represents a different window.
 *
 * @param client The remote client to send the listing to.
 * @return False if an error occured, true otherwise.
 */
//Boolean CWindowManagerService::ViewServerListWindows(
//    /* [in] */ Socket client)
//{
//    return FALSE;
//}

/**
 * Returns the focused window in the following format:
 * windowHashCodeInHexadecimal windowName
 *
 * @param client The remote client to send the listing to.
 * @return False if an error occurred, true otherwise.
 */
//CARAPI_(Boolean) ViewServerGetFocusedWindow(
//    /* [in] */ Socket client)
//{
//    return FALSE;
//}

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
//CARAPI_(Boolean) ViewServerWindowCommand(
//    /* [in] */ Socket client,
//    /* [in] */ const String& command,
//    /* [in] */ const String& parameters)
//{
//    return FALSE;
//}

void CWindowManagerService::AddWindowChangeListener(
    /* [in] */ WindowChangeListener* listener)
{
    Mutex::Autolock lock(mWindowMapLock);
    mWindowChangeListeners.PushBack(listener);
}

void CWindowManagerService::RemoveWindowChangeListener(
    /* [in] */ WindowChangeListener* listener)
{
    Mutex::Autolock lock(mWindowMapLock);
    mWindowChangeListeners.Remove(listener);
}

void CWindowManagerService::NotifyWindowsChanged()
{
    WindowChangeListener** windowChangeListeners;
    Int32 N = 0;
    Mutex::Autolock lock(mWindowMapLock);
    {
        if(mWindowChangeListeners.Begin() == mWindowChangeListeners.End()) {
            return;
        }
        N = mWindowChangeListeners.GetSize();
        windowChangeListeners = new WindowChangeListener*[N];
        List<WindowChangeListener*>::Iterator it = mWindowChangeListeners.Begin();
        for (Int32 i = 0; i < N && it != mWindowChangeListeners.End(); ++it, ++i) {
            windowChangeListeners[i] = *it;
        }
    }
    for (Int32 i = 0; i < N; ++i) {
        windowChangeListeners[i]->WindowsChanged();
    }
    delete [] windowChangeListeners;
}

void CWindowManagerService::NotifyFocusChanged()
{
    WindowChangeListener** windowChangeListeners;
    Int32 N = 0;
    Mutex::Autolock lock(mWindowMapLock);
    {
        if(mWindowChangeListeners.Begin() == mWindowChangeListeners.End()) {
            return;
        }
        N = mWindowChangeListeners.GetSize();
        windowChangeListeners = new WindowChangeListener*[N];
        List<WindowChangeListener*>::Iterator it = mWindowChangeListeners.Begin();
        for (Int32 i = 0; i < N && it != mWindowChangeListeners.End(); ++it, ++i) {
            windowChangeListeners[i] = *it;
        }
    }
    for (Int32 i = 0; i < N; ++i) {
        windowChangeListeners[i]->FocusChanged();
    }
    delete [] windowChangeListeners;
}

AutoPtr<CWindowManagerService::WindowState>
CWindowManagerService::FindWindow(
    /* [in] */ Int32 hashCode)
{
    if (hashCode == -1) {
        return GetFocusedWindow();
    }

//    Mutex::Autolock lock(mWindowMapLock);
//    {
//        List<AutoPtr<WindowState> >& windows = mWindows;
//        List<AutoPtr<WindowState> >::Iterator it;
//
//        for (it = windows.Begin(); it != windows.End(); ++it) {
//            AutoPtr<WindowState> w = *it;
//            if (System.identityHashCode(w) == hashCode) {
//                return w;
//            }
//        }
//    }

    return NULL;
}

ECode CWindowManagerService::OpenSession(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IInputContext* inputContext,
    /* [out] */ IWindowSession** session)
{
    assert(session != NULL);
//    if (client == NULL) throw new IllegalArgumentException("NULL client");
//    if (inputContext == NULL) throw new IllegalArgumentException("NULL inputContext");
    return CWindowSession::NewByFriend(
        (IWindowManager*)this, client, inputContext, (CWindowSession**)session);
}

ECode CWindowManagerService::InputMethodClientHasFocus(
    /* [in] */ IInputMethodClient* client,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mWindowMapLock);
    // The focus for the client is the window immediately below
    // where we would place the input method window.
    List<AutoPtr<WindowState> >::Iterator it =
        FindDesiredInputMethodWindowIndexLocked(FALSE);
    AutoPtr<WindowState> imFocus = NULL;
    if (it != List<AutoPtr<WindowState> >::Iterator(NULL) && it != mWindows.Begin()) {
        imFocus = *(--it);
        if (imFocus != NULL) {
            if (imFocus->mSession->mClient != NULL &&
                    imFocus->mSession->mClient.Get() == client) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

void CWindowManagerService::RebuildAppWindowListLocked()
{
    List<AutoPtr<WindowState> >::Iterator lastWallpaper;
//    Int32 numRemoved = 0;

    // First remove all existing app windows.
    List<AutoPtr<WindowState> >::Iterator it = mWindows.Begin();
    while (it != mWindows.End()) {
        AutoPtr<WindowState> w = *it;
        if (w->mAppToken != NULL) {
//            if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG,
//                    "Rebuild removing window: " + win);
            it = mWindows.Erase(it);
            mWindowsChanged = TRUE;
//            numRemoved++;
            continue;
        }
        else {
            Int32 type;
            w->mAttrs->GetType(&type);
            if (type == WindowManagerLayoutParams_TYPE_WALLPAPER) {
                if (it == mWindows.Begin()) lastWallpaper = it;
                else {
                    List<AutoPtr<WindowState> >::Iterator tmpIt = lastWallpaper;
                    if (++tmpIt == it) lastWallpaper = it;
                }
            }
        }
        ++it;
    }

    // The wallpaper window(s) typically live at the bottom of the stack,
    // so skip them before adding app tokens.
    lastWallpaper++;
    it = lastWallpaper;

    // First add all of the exiting app tokens...  these are no longer
    // in the main app list, but still have windows shown.  We put them
    // in the back because now that the animation is over we no longer
    // will care about them.
    List<AppWindowToken*>::Iterator awtIt = mExitingAppTokens.Begin();
    for (; awtIt != mExitingAppTokens.End(); ++awtIt) {
        it = ReAddAppWindowsLocked(it, *awtIt);
    }

    // And add in the still active app tokens in Z order.
    for (awtIt = mAppTokens.Begin(); awtIt != mAppTokens.End(); ++awtIt) {
        it = ReAddAppWindowsLocked(it, *awtIt);
    }

//    i -= lastWallpaper;
//    if (i != numRemoved) {
//        Slog.w(TAG, "Rebuild removed " + numRemoved
//                + " windows but added " + i);
//    }
}

// These can only be called when injecting events to your own window,
// or by holding the INJECT_EVENTS permission.  These methods may block
// until pending input events are finished being dispatched even when 'sync' is false.
// Avoid calling these methods on your UI thread or use the 'NoWait' version instead.
ECode CWindowManagerService::InjectKeyEvent(
    /* [in] */ IKeyEvent* ev,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int64 downTime, eventTime;
    ev->GetDownTime(&downTime);
    ev->GetEventTime(&eventTime);

    Int32 action, code, repeatCount, metaState, deviceId, scancode, source;
    ev->GetAction(&action);
    ev->GetKeyCode(&code);
    ev->GetRepeatCount(&repeatCount);
    ev->GetMetaState(&metaState);
    ev->GetDeviceId(&deviceId);
    ev->GetScanCode(&scancode);
    ev->GetSource(&source);

    if (source == InputDevice_SOURCE_UNKNOWN) {
        source = InputDevice_SOURCE_KEYBOARD;
    }

    if (eventTime == 0) eventTime = SystemClock::GetUptimeMillis();
    if (downTime == 0) downTime = eventTime;

    AutoPtr<IKeyEvent> newEvent;
    assert(SUCCEEDED(CKeyEvent::New(downTime, eventTime, action, code,
            repeatCount, metaState, deviceId, scancode,
            KeyEvent_FLAG_FROM_SYSTEM, source, (IKeyEvent**)&newEvent)));

    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();

    Int32 res = mInputManager->InjectInputEvent(newEvent, pid, uid,
        sync ? InputManager::INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_FINISH
        : InputManager::INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_RESULT,
        INJECTION_TIMEOUT_MILLIS);

    Binder::RestoreCallingIdentity(ident);
    return ReportInjectionResult(res, result);
}

ECode CWindowManagerService::InjectPointerEvent(
    /* [in] */ IMotionEvent* ev,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();

    AutoPtr<IMotionEventHelper> helper;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
    AutoPtr<IMotionEvent> newEvent;
    helper->Obtain(ev, (IMotionEvent**)&newEvent);
    Int32 source;
    newEvent->GetSource(&source);
    if ((source & InputDevice_SOURCE_CLASS_POINTER) == 0) {
        newEvent->SetSource(InputDevice_SOURCE_TOUCHSCREEN);
    }

    Int32 res = mInputManager->InjectInputEvent(newEvent, pid, uid,
        sync ? InputManager::INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_FINISH
        : InputManager::INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_RESULT,
        INJECTION_TIMEOUT_MILLIS);

    Binder::RestoreCallingIdentity(ident);
    return ReportInjectionResult(res, result);
}

ECode CWindowManagerService::InjectTrackballEvent(
    /* [in] */ IMotionEvent* ev,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();

    AutoPtr<IMotionEventHelper> helper;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
    AutoPtr<IMotionEvent> newEvent;
    helper->Obtain(ev, (IMotionEvent**)&newEvent);
    Int32 source;
    newEvent->GetSource(&source);
    if ((source & InputDevice_SOURCE_CLASS_TRACKBALL) == 0) {
        newEvent->SetSource(InputDevice_SOURCE_CLASS_TRACKBALL);
    }

    Int32 res = mInputManager->InjectInputEvent(newEvent, pid, uid,
        sync ? InputManager::INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_FINISH
        : InputManager::INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_RESULT,
        INJECTION_TIMEOUT_MILLIS);

    Binder::RestoreCallingIdentity(ident);
    return ReportInjectionResult(res, result);
}

ECode CWindowManagerService::InjectInputEventNoWait(
    /* [in] */ IInputEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();

    Int32 res = mInputManager->InjectInputEvent(ev, pid, uid,
        InputManager::INPUT_EVENT_INJECTION_SYNC_NONE,
        INJECTION_TIMEOUT_MILLIS);

    Binder::RestoreCallingIdentity(ident);
    return ReportInjectionResult(res, result);
}

ECode CWindowManagerService::ReportInjectionResult(
    /* [in] */ Int32 result,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    switch (result) {
        case InputManager::INPUT_EVENT_INJECTION_PERMISSION_DENIED:
//            Slog.w(TAG, "Input event injection permission denied.");
//            throw new SecurityException(
//                    "Injecting to another application requires INJECT_EVENTS permission");
            return E_SECURITY_EXCEPTION;
        case InputManager::INPUT_EVENT_INJECTION_SUCCEEDED:
            //Slog.v(TAG, "Input event injection succeeded.");
            *succeeded = TRUE;
            return NOERROR;
        case InputManager::INPUT_EVENT_INJECTION_TIMED_OUT:
//            Slog.w(TAG, "Input event injection timed out.");
            *succeeded = FALSE;
            return NOERROR;
        case InputManager::INPUT_EVENT_INJECTION_FAILED:
        default:
//            Slog.w(TAG, "Input event injection failed.");
            *succeeded = FALSE;
            return NOERROR;
    }
}

// These can only be called when holding the MANAGE_APP_TOKENS permission.
ECode CWindowManagerService::PauseKeyDispatching(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "PauseKeyDispatching()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);
    WindowToken* wtoken = NULL;
    HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator it
            = mTokenMap.Find(token);
    if (it != mTokenMap.End()) {
        wtoken = it->mSecond;
    }

    if (wtoken != NULL) {
        mInputMonitor->PauseDispatchingLw(wtoken);
    }

    return NOERROR;
}

ECode CWindowManagerService::ResumeKeyDispatching(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "ResumeKeyDispatching()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);
    WindowToken* wtoken = NULL;
    HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator it
            = mTokenMap.Find(token);
    if (it != mTokenMap.End()) {
        wtoken = it->mSecond;
    }

    if (wtoken != NULL) {
        mInputMonitor->ResumeDispatchingLw(wtoken);
    }

    return NOERROR;
}

ECode CWindowManagerService::SetEventDispatching(
    /* [in] */ Boolean enabled)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "ResumeKeyDispatching()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);
    mInputMonitor->SetEventDispatchingLw(enabled);

    return NOERROR;
}

ECode CWindowManagerService::SetAppOrientation(
    /* [in] */ IApplicationToken* token,
    /* [in] */ Int32 requestedOrientation)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "SetAppOrientation()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);
    AppWindowToken* wtoken = FindAppWindowToken((IBinder*)token);
    if (wtoken == NULL) {
//        Slog.w(TAG, "Attempted to set orientation of non-existing app token: " + token);
        return NOERROR;
    }

    wtoken->mRequestedOrientation = requestedOrientation;

    return NOERROR;
}

ECode CWindowManagerService::GetAppOrientation(
    /* [in] */ IApplicationToken* token,
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    Mutex::Autolock lock(mWindowMapLock);
    AppWindowToken* wtoken = FindAppWindowToken((IBinder*)token);
    if (wtoken == NULL) {
        *orientation = ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED;
        return NOERROR;
    }

    *orientation = wtoken->mRequestedOrientation;
    return NOERROR;
}

ECode CWindowManagerService::SetFocusedApp(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean moveFocusNow)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "SetFocusedApp()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);

    Boolean changed = FALSE;
    if (token == NULL) {
//        if (DEBUG_FOCUS) Slog.v(TAG, "Clearing focused app, was " + mFocusedApp);
        changed = mFocusedApp != NULL;
        mFocusedApp = NULL;
        if (changed) {
            mInputMonitor->SetFocusedAppLw(NULL);
        }
    }
    else {
        AppWindowToken* newFocus = FindAppWindowToken(token);
        if (newFocus == NULL) {
//            Slog.w(TAG, "Attempted to set focus to non-existing app token: " + token);
            return NOERROR;
        }
        changed = mFocusedApp != newFocus;
        mFocusedApp = newFocus;
//        if (DEBUG_FOCUS) Slog.v(TAG, "Set focused app to: " + mFocusedApp);
        if (changed) {
            mInputMonitor->SetFocusedAppLw(newFocus);
        }
    }

    if (moveFocusNow && changed) {
//        final long origId = Binder.clearCallingIdentity();
        UpdateFocusedWindowLocked(UPDATE_FOCUS_NORMAL);
//        Binder.restoreCallingIdentity(origId);
    }

    return NOERROR;
}

ECode CWindowManagerService::PrepareAppTransition(
    /* [in] */ Int32 transit)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "PrepareAppTransition()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);
//    if (DEBUG_APP_TRANSITIONS) Slog.v(
//            TAG, "Prepare app transition: transit=" + transit
//            + " mNextAppTransition=" + mNextAppTransition);
    Boolean isOn;
    mPolicy->IsScreenOn(&isOn);
    if (!mDisplayFrozen && isOn) {
        if (mNextAppTransition == WindowManagerPolicy::TRANSIT_UNSET
                || mNextAppTransition == WindowManagerPolicy::TRANSIT_NONE) {
            mNextAppTransition = transit;
        }
        else if (transit == WindowManagerPolicy::TRANSIT_TASK_OPEN
                && mNextAppTransition == WindowManagerPolicy::TRANSIT_TASK_CLOSE) {
            // Opening a new task always supersedes a close for the anim.
            mNextAppTransition = transit;
        }
        else if (transit == WindowManagerPolicy::TRANSIT_ACTIVITY_OPEN
                && mNextAppTransition == WindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE) {
            // Opening a new activity always supersedes a close for the anim.
            mNextAppTransition = transit;
        }
        mAppTransitionReady = FALSE;
        mAppTransitionTimeout = FALSE;
        mStartingIconInTransition = FALSE;
        mSkipAppTransitionAnimation = FALSE;

        ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
        pHandlerFunc = &CWindowManagerService::HandleAppTransitionTimeout;

        RemoveMessage(*(Handle32*)&pHandlerFunc);
        SendMessageDelayed(*(Handle32*)&pHandlerFunc, NULL, 5000);
    }

    return NOERROR;
}

ECode CWindowManagerService::GetPendingAppTransition(
    /* [out] */ Int32* transit)
{
    VALIDATE_NOT_NULL(transit);

    *transit = mNextAppTransition;
    return NOERROR;
}

ECode CWindowManagerService::OverridePendingAppTransition(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 enterAnim,
    /* [in] */ Int32 exitAnim)
{
    if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
        mNextAppTransitionPackage = capsuleName;
        mNextAppTransitionEnter = enterAnim;
        mNextAppTransitionExit = exitAnim;
    }

    return NOERROR;
}

ECode CWindowManagerService::ExecuteAppTransition()
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "ExecuteAppTransition()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);
//    if (DEBUG_APP_TRANSITIONS) {
//        RuntimeException e = new RuntimeException("here");
//        e.fillInStackTrace();
//        Slog.w(TAG, "Execute app transition: mNextAppTransition="
//                + mNextAppTransition, e);
//    }
    if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
        mAppTransitionReady = TRUE;
//        final long origId = Binder.clearCallingIdentity();
        PerformLayoutAndPlaceSurfacesLocked();
//        Binder.restoreCallingIdentity(origId);
    }

    return NOERROR;
}

ECode CWindowManagerService::SetAppStartingWindow(
    /* [in] */ IBinder* token,
    /* [in] */ const String& cap,
    /* [in] */ Int32 theme,
    /* [in] */ ICharSequence* nonLocalizedLabel,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 icon,
    /* [in] */ IBinder* transferFrom,
    /* [in] */ Boolean createIfNeeded)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "SetAppStartingIcon()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);

//    if (DEBUG_STARTING_WINDOW) Slog.v(
//            TAG, "setAppStartingIcon: token=" + token + " cap=" + pkg
//            + " transferFrom=" + transferFrom);

    AppWindowToken* wtoken = FindAppWindowToken(token);
    if (wtoken == NULL) {
//        Slog.w(TAG, "Attempted to set icon of non-existing app token: " + token);
        return NOERROR;
    }

    // If the display is frozen, we won't do anything until the
    // actual window is displayed so there is no reason to put in
    // the starting window.
    Boolean isOn;
    mPolicy->IsScreenOn(&isOn);
    if (mDisplayFrozen || !isOn) {
        return NOERROR;
    }

    if (wtoken->mStartingData != NULL) {
        return NOERROR;
    }

    if (transferFrom != NULL) {
        AppWindowToken* ttoken = FindAppWindowToken(transferFrom);
        if (ttoken != NULL) {
            AutoPtr<WindowState> startingWindow = ttoken->mStartingWindow;
            if (startingWindow != NULL) {
                if (mStartingIconInTransition) {
                    // In this case, the starting icon has already
                    // been displayed, so start letting windows get
                    // shown immediately without any more transitions.
                    mSkipAppTransitionAnimation = TRUE;
                }
//                if (DEBUG_STARTING_WINDOW) Slog.v(TAG,
//                        "Moving existing starting from " + ttoken
//                        + " to " + wtoken);
//                final long origId = Binder.clearCallingIdentity();

                // Transfer the starting window over to the new
                // token.
                wtoken->mStartingData = ttoken->mStartingData;
                wtoken->mStartingView = ttoken->mStartingView;
                wtoken->mStartingWindow = startingWindow;
                ttoken->mStartingData = NULL;
                ttoken->mStartingView = NULL;
                ttoken->mStartingWindow = NULL;
                ttoken->mStartingMoved = TRUE;
                startingWindow->mToken = wtoken;
                startingWindow->mRootToken = wtoken;
                startingWindow->mAppToken = wtoken;
//                if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG,
//                        "Removing starting window: " + startingWindow);
                mWindows.Remove(startingWindow);
                mWindowsChanged = TRUE;
                ttoken->mWindows.Remove(startingWindow);
                ttoken->mAllAppWindows.Remove(startingWindow);
                AddWindowToListInOrderLocked(startingWindow, TRUE);

                // Propagate other interesting state between the
                // tokens.  If the old token is displayed, we should
                // immediately force the new one to be displayed.  If
                // it is animating, we need to move that animation to
                // the new one.
                if (ttoken->mAllDrawn) {
                    wtoken->mAllDrawn = TRUE;
                }
                if (ttoken->mFirstWindowDrawn) {
                    wtoken->mFirstWindowDrawn = TRUE;
                }
                if (!ttoken->mHidden) {
                    wtoken->mHidden = FALSE;
                    wtoken->mHiddenRequested = FALSE;
                    wtoken->mWillBeHidden = FALSE;
                }
                if (wtoken->mClientHidden != ttoken->mClientHidden) {
                    wtoken->mClientHidden = ttoken->mClientHidden;
                    wtoken->SendAppVisibilityToClients();
                }
                if (ttoken->mAnimation != NULL) {
                    wtoken->mAnimation = ttoken->mAnimation;
                    wtoken->mAnimating = ttoken->mAnimating;
                    wtoken->mAnimLayerAdjustment = ttoken->mAnimLayerAdjustment;
                    ttoken->mAnimation = NULL;
                    ttoken->mAnimLayerAdjustment = 0;
                    wtoken->UpdateLayers();
                    ttoken->UpdateLayers();
                }

                UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES);
                mLayoutNeeded = TRUE;
                PerformLayoutAndPlaceSurfacesLocked();
//                Binder.restoreCallingIdentity(origId);
                return NOERROR;
            }
            else if (ttoken->mStartingData != NULL) {
                // The previous app was getting ready to show a
                // starting window, but hasn't yet done so.  Steal it!
//                if (DEBUG_STARTING_WINDOW) Slog.v(TAG,
//                        "Moving pending starting from " + ttoken
//                        + " to " + wtoken);
                wtoken->mStartingData = ttoken->mStartingData;
                ttoken->mStartingData = NULL;
                ttoken->mStartingMoved = TRUE;

                ECode (STDCALL CWindowManagerService::*pHandlerFunc)(AppWindowToken*);
                pHandlerFunc = &CWindowManagerService::HandleAddStarting;

                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);
                params->WriteInt32((Handle32)wtoken);

                // Note: we really want to do sendMessageAtFrontOfQueue() because we
                // want to process the message ASAP, before any other queued
                // messages.
                mApartment->PostCppCallbackAtFrontOfQueue(
                    (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
                return NOERROR;
            }
        }
    }

    // There is no existing starting window, and the caller doesn't
    // want us to create one, so that's it!
    if (!createIfNeeded) {
        return NOERROR;
    }

    // If this is a translucent or wallpaper window, then don't
    // show a starting window -- the current effect (a full-screen
    // opaque starting window that fades away to the real contents
    // when it is ready) does not work for this.
//    if (theme != 0) {
//        AttributeCache.Entry ent = AttributeCache.instance().get(cap, theme,
//                com.android.internal.R.styleable.Window);
//        if (ent.array.getBoolean(
//                com.android.internal.R.styleable.Window_windowIsTranslucent, false)) {
//            return;
//        }
//        if (ent.array.getBoolean(
//                com.android.internal.R.styleable.Window_windowIsFloating, false)) {
//            return;
//        }
//        if (ent.array.getBoolean(
//                com.android.internal.R.styleable.Window_windowShowWallpaper, false)) {
//            return;
//        }
//    }

    mStartingIconInTransition = TRUE;
    wtoken->mStartingData = new StartingData(
            cap, theme, nonLocalizedLabel, labelRes, icon);

    ECode (STDCALL CWindowManagerService::*pHandlerFunc)(AppWindowToken*);
    pHandlerFunc = &CWindowManagerService::HandleAddStarting;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)wtoken);

    // Note: we really want to do sendMessageAtFrontOfQueue() because we
    // want to process the message ASAP, before any other queued
    // messages.
    mApartment->PostCppCallbackAtFrontOfQueue(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);

    return NOERROR;
}

ECode CWindowManagerService::SetAppWillBeHidden(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "SetAppWillBeHidden()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AppWindowToken* wtoken;

    Mutex::Autolock lock(mWindowMapLock);

    wtoken = FindAppWindowToken(token);
    if (wtoken) {
        wtoken->mWillBeHidden = TRUE;
    }
    else {
//        Slog.w(TAG, "Attempted to set will be hidden of non-existing app token: " + token);
    }

    return NOERROR;
    return NOERROR;
}

Boolean CWindowManagerService::SetTokenVisibilityLocked(
    /* [in] */ AppWindowToken* wtoken,
    /* [in] */ IWindowManagerLayoutParams* lp,
    /* [in] */ Boolean visible,
    /* [in] */ Int32 transit,
    /* [in] */ Boolean performLayout)
{
    Boolean delayed = FALSE;

    if (wtoken->mClientHidden == visible) {
        wtoken->mClientHidden = !visible;
        wtoken->SendAppVisibilityToClients();
    }

    wtoken->mWillBeHidden = FALSE;
    if (wtoken->mHidden == visible) {
        Boolean changed = FALSE;
//        if (DEBUG_APP_TRANSITIONS) Slog.v(
//            TAG, "Changing app " + wtoken + " hidden=" + wtoken.hidden
//            + " performLayout=" + performLayout);

        Boolean runningAppAnimation = FALSE;

        if (transit != WindowManagerPolicy::TRANSIT_UNSET) {
            if (wtoken->mAnimation == sDummyAnimation) {
                wtoken->mAnimation = NULL;
            }
            ApplyAnimationLocked(wtoken, lp, transit, visible);
            changed = TRUE;
            if (wtoken->mAnimation != NULL) {
                delayed = runningAppAnimation = TRUE;
            }
        }

        List<AutoPtr<WindowState> >::Iterator it = wtoken->mAllAppWindows.Begin();;
        for (; it != wtoken->mAllAppWindows.End(); ++it) {
            AutoPtr<WindowState> win = *it;
            if (win == wtoken->mStartingWindow) {
                continue;
            }

            if (win->IsAnimating()) {
                delayed = TRUE;
            }

            //Slog.i(TAG, "Window " + win + ": vis=" + win.isVisible());
            //win.dump("  ");
            if (visible) {
                if (!win->IsVisibleNow()) {
                    if (!runningAppAnimation) {
                        ApplyAnimationLocked(win,
                                WindowManagerPolicy::TRANSIT_ENTER, TRUE);
                    }
                    changed = TRUE;
                }
            }
            else if (win->IsVisibleNow()) {
                if (!runningAppAnimation) {
                    ApplyAnimationLocked(win,
                            WindowManagerPolicy::TRANSIT_EXIT, FALSE);
                }
                changed = TRUE;
            }
        }

        wtoken->mHidden = wtoken->mHiddenRequested = !visible;
        if (!visible) {
            UnsetAppFreezingScreenLocked(wtoken, TRUE, TRUE);
        }
        else {
            // If we are being set visible, and the starting window is
            // not yet displayed, then make sure it doesn't get displayed.
            AutoPtr<WindowState> swin = wtoken->mStartingWindow;
            if (swin != NULL && (swin->mDrawPending
                    || swin->mCommitDrawPending)) {
                swin->mPolicyVisibility = FALSE;
                swin->mPolicyVisibilityAfterAnim = FALSE;
             }
        }

//        if (DEBUG_APP_TRANSITIONS) Slog.v(TAG, "setTokenVisibilityLocked: " + wtoken
//                  + ": hidden=" + wtoken.hidden + " hiddenRequested="
//                  + wtoken.hiddenRequested);

        if (changed) {
            mLayoutNeeded = TRUE;
            if (performLayout) {
                UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES);
                PerformLayoutAndPlaceSurfacesLocked();
            }
            else {
                mInputMonitor->UpdateInputWindowsLw();
            }
        }
    }

    if (wtoken->mAnimation != NULL) {
        delayed = TRUE;
    }

    return delayed;
}

ECode CWindowManagerService::SetAppVisibility(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean visible)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "SetAppVisibility()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AppWindowToken* wtoken;

    Mutex::Autolock lock(mWindowMapLock);

    wtoken = FindAppWindowToken(token);
    if (wtoken == NULL) {
//        Slog.w(TAG, "Attempted to set visibility of non-existing app token: " + token);
        return NOERROR;
    }

//    if (DEBUG_APP_TRANSITIONS || DEBUG_ORIENTATION) {
//        RuntimeException e = NULL;
//        if (!HIDE_STACK_CRAWLS) {
//            e = new RuntimeException();
//            e.fillInStackTrace();
//        }
//        Slog.v(TAG, "setAppVisibility(" + token + ", " + visible
//                + "): mNextAppTransition=" + mNextAppTransition
//                + " hidden=" + wtoken.hidden
//                + " hiddenRequested=" + wtoken.hiddenRequested, e);
//    }

    // If we are preparing an app transition, then delay changing
    // the visibility of this token until we execute that transition.
    Boolean isOn;
    mPolicy->IsScreenOn(&isOn);
    if (!mDisplayFrozen && isOn
            && mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
        // Already in requested state, don't do anything more.
        if (wtoken->mHiddenRequested != visible) {
            return NOERROR;
        }
        wtoken->mHiddenRequested = !visible;

//        if (DEBUG_APP_TRANSITIONS) Slog.v(
//                TAG, "Setting dummy animation on: " + wtoken);
        wtoken->SetDummyAnimation();
        mOpeningApps.Remove(wtoken);
        mClosingApps.Remove(wtoken);
        wtoken->mWaitingToShow = wtoken->mWaitingToHide = FALSE;
        wtoken->mInPendingTransaction = TRUE;
        if (visible) {
            mOpeningApps.PushBack(wtoken);
            wtoken->mStartingDisplayed = FALSE;
            wtoken->mStartingMoved = FALSE;

            // If the token is currently hidden (should be the
            // common case), then we need to set up to wait for
            // its windows to be ready.
            if (wtoken->mHidden) {
                wtoken->mAllDrawn = FALSE;
                wtoken->mWaitingToShow = TRUE;

                if (wtoken->mClientHidden) {
                    // In the case where we are making an app visible
                    // but holding off for a transition, we still need
                    // to tell the client to make its windows visible so
                    // they get drawn.  Otherwise, we will wait on
                    // performing the transition until all windows have
                    // been drawn, they never will be, and we are sad.
                    wtoken->mClientHidden = FALSE;
                    wtoken->SendAppVisibilityToClients();
                }
            }
        }
        else {
            mClosingApps.PushBack(wtoken);

            // If the token is currently visible (should be the
            // common case), then set up to wait for it to be hidden.
            if (!wtoken->mHidden) {
                wtoken->mWaitingToHide = TRUE;
            }
        }
        return NOERROR;
    }

//    final long origId = Binder.clearCallingIdentity();
    SetTokenVisibilityLocked(wtoken, NULL, visible, WindowManagerPolicy::TRANSIT_UNSET, TRUE);
    wtoken->UpdateReportedVisibilityLocked();
//    Binder.restoreCallingIdentity(origId);

    return NOERROR;
}

void CWindowManagerService::UnsetAppFreezingScreenLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ Boolean unfreezeSurfaceNow,
        /* [in] */ Boolean force)
{
    if (wtoken->mFreezingScreen) {
//        if (DEBUG_ORIENTATION) Slog.v(TAG, "Clear freezing of " + wtoken
//                + " force=" + force);
        Boolean unfrozeWindows = FALSE;
        List<AutoPtr<WindowState> >::Iterator it = wtoken->mAllAppWindows.Begin();
        for (; it != wtoken->mAllAppWindows.End(); ++it) {
            AutoPtr<WindowState> w = *it;
            if (w->mAppFreezing) {
                w->mAppFreezing = FALSE;
                if (w->mSurface != NULL && !w->mOrientationChanging) {
                    w->mOrientationChanging = TRUE;
                }
                unfrozeWindows = TRUE;
            }
        }
        if (force || unfrozeWindows) {
//            if (DEBUG_ORIENTATION) Slog.v(TAG, "No longer freezing: " + wtoken);
            wtoken->mFreezingScreen = FALSE;
            mAppsFreezingScreen--;
        }
        if (unfreezeSurfaceNow) {
            if (unfrozeWindows) {
                mLayoutNeeded = TRUE;
                PerformLayoutAndPlaceSurfacesLocked();
            }
            StopFreezingDisplayLocked();
        }
    }
}

void CWindowManagerService::StartAppFreezingScreenLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ Int32 configChanges)
{
//    if (DEBUG_ORIENTATION) {
//        RuntimeException e = NULL;
//        if (!HIDE_STACK_CRAWLS) {
//            e = new RuntimeException();
//            e.fillInStackTrace();
//        }
//        Slog.i(TAG, "Set freezing of " + wtoken.appToken
//                + ": hidden=" + wtoken.hidden + " freezing="
//                + wtoken.freezingScreen, e);
//    }
    if (!wtoken->mHiddenRequested) {
        if (!wtoken->mFreezingScreen) {
            wtoken->mFreezingScreen = TRUE;
            mAppsFreezingScreen++;
            if (mAppsFreezingScreen == 1) {
                StartFreezingDisplayLocked();
                ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
                pHandlerFunc = &CWindowManagerService::HandleAppFreezeTimeout;

                RemoveMessage(*(Handle32*)&pHandlerFunc);
                SendMessageDelayed(*(Handle32*)&pHandlerFunc, NULL, 5000);
            }
        }

        List<AutoPtr<WindowState> >::Iterator it = wtoken->mAllAppWindows.Begin();
        for (; it != wtoken->mAllAppWindows.End(); ++it) {
            AutoPtr<WindowState> w = *it;
            w->mAppFreezing = TRUE;
        }
    }
}

ECode CWindowManagerService::StartAppFreezingScreen(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 configChanges)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "StartAppFreezingScreen()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);

    Boolean isOn;
    mPolicy->IsScreenOn(&isOn);
    if (configChanges == 0 && !mDisplayFrozen && isOn) {
//        if (DEBUG_ORIENTATION) Slog.v(TAG, "Skipping set freeze of " + token);
        return NOERROR;
    }

    AppWindowToken* wtoken = FindAppWindowToken(token);
    if (wtoken == NULL || wtoken->mAppToken == NULL) {
//        Slog.w(TAG, "Attempted to freeze screen with non-existing app token: " + wtoken);
        return NOERROR;
    }
//    final long origId = Binder.clearCallingIdentity();
    StartAppFreezingScreenLocked(wtoken, configChanges);
//    Binder.restoreCallingIdentity(origId);

    return NOERROR;
}

ECode CWindowManagerService::StopAppFreezingScreen(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean force)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "StopAppFreezingScreen()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);

    AppWindowToken* wtoken = FindAppWindowToken(token);
    if (wtoken == NULL || wtoken->mAppToken == NULL) {
        return NOERROR;
    }
//    final long origId = Binder.clearCallingIdentity();
//    if (DEBUG_ORIENTATION) Slog.v(TAG, "Clear freezing of " + token
//            + ": hidden=" + wtoken.hidden + " freezing=" + wtoken.freezingScreen);
    UnsetAppFreezingScreenLocked(wtoken, TRUE, force);
//    Binder.restoreCallingIdentity(origId);

    return NOERROR;
}

ECode CWindowManagerService::RemoveAppToken(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "RemoveAppToken()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AppWindowToken* wtoken = NULL;
    AppWindowToken* startingToken = NULL;
    Boolean delayed = FALSE;

//    final long origId = Binder.clearCallingIdentity();
    Mutex::Autolock lock(mWindowMapLock);

    WindowToken* basewtoken = NULL;
    HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator tokenIt
            = mTokenMap.Find(token);
    if (tokenIt != mTokenMap.End()) {
        basewtoken = tokenIt->mSecond;
    }
    mTokenMap.Erase(tokenIt);
    mTokenList.Remove(basewtoken);
    if (basewtoken != NULL && (wtoken = basewtoken->mAppWindowToken) != NULL) {
//        if (DEBUG_APP_TRANSITIONS) Slog.v(TAG, "Removing app token: " + wtoken);
        delayed = SetTokenVisibilityLocked(wtoken, NULL, FALSE, WindowManagerPolicy::TRANSIT_UNSET, TRUE);
        wtoken->mInPendingTransaction = FALSE;
        mOpeningApps.Remove(wtoken);
        wtoken->mWaitingToShow = FALSE;

        List<AppWindowToken*>::Iterator it =
                Find(mClosingApps.Begin(), mClosingApps.End(), wtoken);
        if (it != mClosingApps.End()) {
            delayed = TRUE;
        }
        else if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
            mClosingApps.PushBack(wtoken);
            wtoken->mWaitingToHide = TRUE;
            delayed = TRUE;
        }
//        if (DEBUG_APP_TRANSITIONS) Slog.v(
//                TAG, "Removing app " + wtoken + " delayed=" + delayed
//                + " animation=" + wtoken.animation
//                + " animating=" + wtoken.animating);
        if (delayed) {
            // set the token aside because it has an active animation to be finished
            mExitingAppTokens.PushBack(wtoken);
        }
        else {
            // Make sure there is no animation running on this token,
            // so any windows associated with it will be removed as
            // soon as their animations are complete
            wtoken->mAnimation = NULL;
            wtoken->mAnimating = FALSE;
        }
        mAppTokens.Remove(wtoken);
        if (mLastEnterAnimToken == wtoken) {
            mLastEnterAnimToken = NULL;
            mLastEnterAnimParams = NULL;
        }
        wtoken->mRemoved = TRUE;
        if (wtoken->mStartingData != NULL) {
            startingToken = wtoken;
        }
        UnsetAppFreezingScreenLocked(wtoken, TRUE, TRUE);
        if (mFocusedApp == wtoken) {
//            if (DEBUG_FOCUS) Slog.v(TAG, "Removing focused app token:" + wtoken);
            mFocusedApp = NULL;
            UpdateFocusedWindowLocked(UPDATE_FOCUS_NORMAL);
            mInputMonitor->SetFocusedAppLw(NULL);
        }
    }
    else {
//        Slog.w(TAG, "Attempted to remove non-existing app token: " + token);
    }

    if (!delayed && wtoken != NULL) {
        wtoken->UpdateReportedVisibilityLocked();
    }
//    Binder.restoreCallingIdentity(origId);

    if (startingToken != NULL) {
//        if (DEBUG_STARTING_WINDOW) Slog.v(TAG, "Schedule remove starting "
//                + startingToken + ": app token removed");

        ECode (STDCALL CWindowManagerService::*pHandlerFunc)(
            AppWindowToken* appWinToken);
        pHandlerFunc = &CWindowManagerService::HandleRemoveStarting;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32((Handle32)startingToken);
        SendMessage(*(Handle32*)&pHandlerFunc, params);
    }

    return NOERROR;
}

ECode CWindowManagerService::MoveAppToken(
    /* [in] */ Int32 index,
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "MoveAppToken()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);

//    if (DEBUG_REORDER) Slog.v(TAG, "Initial app tokens:");
//    if (DEBUG_REORDER) dumpAppTokensLocked();
    AppWindowToken* wtoken = FindAppWindowToken(token);
    if (wtoken == NULL /*|| !mAppTokens.Remove(wtoken)*/) {
//        Slog.w(TAG, "Attempting to reorder token that doesn't exist: "
//              + token + " (" + wtoken + ")");
        return NOERROR;
    }
    List<AppWindowToken*>::Iterator it = mAppTokens.Insert(index, wtoken);
//    if (DEBUG_REORDER) Slog.v(TAG, "Moved " + token + " to " + index + ":");
//    if (DEBUG_REORDER) dumpAppTokensLocked();

//    final long origId = Binder.clearCallingIdentity();
//    if (DEBUG_REORDER) Slog.v(TAG, "Removing windows in " + token + ":");
//    if (DEBUG_REORDER) dumpWindowsLocked();
    if (TmpRemoveAppWindowsLocked(wtoken)) {
//        if (DEBUG_REORDER) Slog.v(TAG, "Adding windows back in:");
//        if (DEBUG_REORDER) dumpWindowsLocked();
        ReAddAppWindowsLocked(FindWindowOffsetLocked(it), wtoken);
//        if (DEBUG_REORDER) Slog.v(TAG, "Final window list:");
//        if (DEBUG_REORDER) dumpWindowsLocked();
        UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES);
        mLayoutNeeded = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
    }
//    Binder.restoreCallingIdentity(origId);

    return NOERROR;
}

ECode CWindowManagerService::MoveAppTokensToTop(
    /* [in] */ IObjectContainer* tokens)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "MoveAppTokensToTop()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }
//
//    final long origId = Binder.clearCallingIdentity();

    Mutex::Autolock lock(mWindowMapLock);

    RemoveAppTokensLocked(tokens);
    AutoPtr<IObjectEnumerator> objEmu;
    ASSERT_SUCCEEDED(tokens->GetObjectEnumerator((IObjectEnumerator**)&objEmu));

    Boolean isSucceeded;
    objEmu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IBinder> token;
        objEmu->Current((IInterface**)&token);
        AppWindowToken* wt = FindAppWindowToken(token.Get());
        if (wt != NULL) {
            mAppTokens.PushBack(wt);
            if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
                mToTopApps.Remove(wt);
                mToBottomApps.Remove(wt);
                mToTopApps.PushBack(wt);
                wt->mSendingToBottom = FALSE;
                wt->mSendingToTop = TRUE;
            }
        }
        objEmu->MoveNext(&isSucceeded);
    }

    if (mNextAppTransition == WindowManagerPolicy::TRANSIT_UNSET) {
        MoveAppWindowsLocked(tokens, mAppTokens.End());
    }
////    Binder.restoreCallingIdentity(origId);

    return NOERROR;
}

ECode CWindowManagerService::MoveAppTokensToBottom(
    /* [in] */ IObjectContainer* tokens)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "MoveAppTokensToBottom()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

//    final long origId = Binder.clearCallingIdentity();
    Mutex::Autolock lock(mWindowMapLock);

    RemoveAppTokensLocked(tokens);
    AutoPtr<IObjectEnumerator> objEmu;
    ASSERT_SUCCEEDED(tokens->GetObjectEnumerator((IObjectEnumerator**)&objEmu));

    Boolean isSucceeded;
    objEmu->MoveNext(&isSucceeded);

    List<AppWindowToken*>::Iterator posIt = mAppTokens.Begin();
    List<AppWindowToken*>::Iterator tbaIt = mToBottomApps.Begin();
    while (isSucceeded) {
        AutoPtr<IBinder> token;
        objEmu->Current((IInterface**)&token);
        AppWindowToken* wt = FindAppWindowToken(token);
        if (wt != NULL) {
            posIt = mAppTokens.Insert(posIt, wt);
            if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
                mToTopApps.Remove(wt);
                mToBottomApps.Remove(wt);
                tbaIt = mToBottomApps.Insert(tbaIt, wt);
                wt->mSendingToTop = FALSE;
                wt->mSendingToBottom = TRUE;
            }
            posIt++;
        }
        ++tbaIt;
        objEmu->MoveNext(&isSucceeded);
    }

    if (mNextAppTransition == WindowManagerPolicy::TRANSIT_UNSET) {
        MoveAppWindowsLocked(tokens, mAppTokens.Begin());
    }
//    Binder.restoreCallingIdentity(origId);

    return NOERROR;
}

// Re-evaluate the current orientation from the caller's state.
// If there is a change, the new Configuration is returned and the
// caller must call setNewConfiguration() sometime later.
ECode CWindowManagerService::UpdateOrientationFromAppTokens(
    /* [in] */ IConfiguration* currentConfig,
    /* [in] */ IBinder* freezeThisOneIfNeeded,
    /* [out] */ IConfiguration** _config)
{
    VALIDATE_NOT_NULL(_config);

    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "UpdateOrientationFromAppTokens()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

//    long ident = Binder.clearCallingIdentity();

    AutoPtr<IConfiguration> config;

    Mutex::Autolock lock(mWindowMapLock);

    if (UpdateOrientationFromAppTokensLocked()) {
        if (freezeThisOneIfNeeded != NULL) {
            AppWindowToken* wtoken = FindAppWindowToken(
                    freezeThisOneIfNeeded);
            if (wtoken != NULL) {
                StartAppFreezingScreenLocked(wtoken,
                        ActivityInfo_CONFIG_ORIENTATION);
            }
        }
        config = ComputeNewConfigurationLocked();

    }
    else if (currentConfig != NULL) {
        // No obvious action we need to take, but if our current
        // state mismatches the activity manager's, update it,
        // disregarding font scale, which should remain set to
        // the value of the previous configuration.
        Float fscale;
        currentConfig->GetFontScale(&fscale);
        mTempConfiguration->SetToDefaults();
        mTempConfiguration->SetFontScale(fscale);
        if (ComputeNewConfigurationLocked(mTempConfiguration)) {
            Int32 result;
            currentConfig->Diff(mTempConfiguration, &result);
            if (result != 0) {
                mWaitingForConfig = TRUE;
                mLayoutNeeded = TRUE;
                StartFreezingDisplayLocked();
                FAIL_RETURN(CConfiguration::New(
                        (IConfiguration*)mTempConfiguration, (IConfiguration**)&config));
            }
        }
    }

//    Binder.restoreCallingIdentity(ident);
    *_config = config;
    if (*_config) (*_config)->AddRef();
    return NOERROR;
}

Int32 CWindowManagerService::ComputeForcedAppOrientationLocked()
{
    Int32 req = GetOrientationFromWindowsLocked();
    if (req == ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED) {
        req = GetOrientationFromAppTokensLocked();
    }
    return req;
}

ECode CWindowManagerService::SetNewConfiguration(
    /* [in] */ IConfiguration* config)
{
    if (!CheckCallingPermission("elastos.permission.MANAGE_APP_TOKENS", /*android.Manifest.permission.MANAGE_APP_TOKENS,*/
            "SetNewConfiguration()")) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mWindowMapLock);
    FAIL_RETURN(CConfiguration::New(config,
            (IConfiguration**)&mCurConfiguration));
    mWaitingForConfig = FALSE;
    return PerformLayoutAndPlaceSurfacesLocked();
}

// these require DISABLE_KEYGUARD permission
ECode CWindowManagerService::DisableKeyguard(
    /* [in] */ IBinder* token,
    /* [in] */ const String& tag)
{
    Int32 perm;
    mContext->CheckCallingOrSelfPermission("elastos.permission.DISABLE_KEYGUARD", /*android.Manifest.permission.DISABLE_KEYGUARD)*/
                &perm);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
        //throw new SecurityException("Requires DISABLE_KEYGUARD permission");
        return E_SECURITY_EXCEPTION;
    }

//    Mutex::Autolock lock(mKeyguardTokenWatcherLock);

//    mKeyguardTokenWatcher->Acquire(token, tag);

    return E_NOT_IMPLEMENTED;
}

ECode CWindowManagerService::ReenableKeyguard(
    /* [in] */ IBinder* token)
{
    Int32 perm;
    mContext->CheckCallingOrSelfPermission("elastos.permission.DISABLE_KEYGUARD", /*android.Manifest.permission.DISABLE_KEYGUARD)*/
                &perm);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
        //throw new SecurityException("Requires DISABLE_KEYGUARD permission");
        return E_SECURITY_EXCEPTION;
    }

//    Mutex::Autolock lock(mKeyguardTokenWatcherLock);

//    mKeyguardTokenWatcher->release(token);
//
//    if (!mKeyguardTokenWatcher->IsAcquired()) {
//        // If we are the last one to reenable the keyguard wait until
//        // we have actually finished reenabling until returning.
//        // It is possible that reenableKeyguard() can be called before
//        // the previous disableKeyguard() is handled, in which case
//        // neither mKeyguardTokenWatcher.acquired() or released() would
//        // be called. In that case mKeyguardDisabled will be false here
//        // and we have nothing to wait for.
//        while (mKeyguardDisabled) {
////            try {
//            ECode ec = mKeyguardTokenWatcher->Wait();
////            } catch (InterruptedException e) {
////                Thread.currentThread().interrupt();
////            }
//        }
//    }

    return E_NOT_IMPLEMENTED;
}

ECode CWindowManagerService::ExitKeyguardSecurely(
    /* [in] */ IOnKeyguardExitResult* keyguardCallback)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowManagerService::InKeyguardRestrictedInputMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return mPolicy->InKeyguardRestrictedKeyInputMode(result);
}

ECode CWindowManagerService::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    Mutex::Autolock lock(mWindowMapLock);
    List<AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = mWindows.RBegin(); rit != mWindows.REnd(); ++rit) {
        AutoPtr<WindowState> w = *rit;
        if (w->mSurface != NULL) {
//            try {
            w->mClient->CloseSystemDialogs(reason);
//            } catch (RemoteException e) {
//            }
        }
    }

    return NOERROR;
}

// These can only be called with the SET_ANIMATON_SCALE permission.
ECode CWindowManagerService::GetAnimationScale(
    /* [in] */ Int32 which,
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);

    switch (which) {
        case 0:
            *scale = mWindowAnimationScale;
            return NOERROR;
        case 1:
            *scale = mTransitionAnimationScale;
            return NOERROR;
    }
    *scale = 0;
    return NOERROR;
}

//    GetAnimationScales(
//        [out] float[] scales);

ECode CWindowManagerService::SetAnimationScale(
    /* [in] */ Int32 which,
    /* [in] */ Float scale)
{
    if (!CheckCallingPermission("elastos.permission.SET_ANIMATION_SCALE", /*android.Manifest.permission.SET_ANIMATION_SCALE,*/
            "SetAnimationScale()")) {
        //throw new SecurityException("Requires SET_ANIMATION_SCALE permission");
        return E_SECURITY_EXCEPTION;
    }

    if (scale < 0) scale = 0;
    else if (scale > 20) scale = 20;
    scale = Math::Abs(scale);
    switch (which) {
        case 0: mWindowAnimationScale = FixScale(scale); break;
        case 1: mTransitionAnimationScale = FixScale(scale); break;
    }

    // Persist setting
    ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
    pHandlerFunc = &CWindowManagerService::HandlePersistAnimationScale;

    SendMessage(*(Handle32*)&pHandlerFunc, NULL);

    return NOERROR;
}

//    SetAnimationScales(
//        [in] float[] scales);

// These require the READ_INPUT_STATE permission.
ECode CWindowManagerService::GetSwitchState(
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetSwitchState()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetSwitchState(-1, InputDevice_SOURCE_ANY, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetSwitchStateForDevice(
    /* [in] */ Int32 devid,
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetSwitchStateForDevice()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetSwitchState(devid, InputDevice_SOURCE_ANY, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetScancodeState(
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetScancodeState()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetScanCodeState(-1, InputDevice_SOURCE_ANY, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetScancodeStateForDevice(
    /* [in] */ Int32 devid,
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetScancodeStateForDevice()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetScanCodeState(devid, InputDevice_SOURCE_ANY, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetTrackballScancodeState(
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetTrackballScancodeState()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetScanCodeState(-1, InputDevice_SOURCE_TRACKBALL, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetDPadScancodeState(
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetDPadScancodeState()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetScanCodeState(-1, InputDevice_SOURCE_DPAD, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetKeycodeState(
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetKeycodeState()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetKeyCodeState(-1, InputDevice_SOURCE_ANY, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetKeycodeStateForDevice(
    /* [in] */ Int32 devid,
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetKeycodeStateForDevice()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetKeyCodeState(devid, InputDevice_SOURCE_ANY, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetTrackballKeycodeState(
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetTrackballKeycodeState()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetKeyCodeState(-1, InputDevice_SOURCE_TRACKBALL, sw);

    return NOERROR;
}

ECode CWindowManagerService::GetDPadKeycodeState(
    /* [in] */ Int32 sw,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "GetDPadKeycodeState()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    *state = mInputManager->GetKeyCodeState(-1, InputDevice_SOURCE_DPAD, sw);

    return NOERROR;
}

// Report whether the hardware supports the given keys; returns true if successful
//    HasKeys(
//        [in] int[] keycodes,
//        [inout] boolean[] keyExists,
//        [out] Boolean* result);

ECode CWindowManagerService::MonitorInput(
    /* [in] */ const String& inputChannelName,
    /* [out] */ IInputChannel** channel)
{
    VALIDATE_NOT_NULL(channel);

    if (!CheckCallingPermission("elastos.permission.READ_INPUT_STATE", /*android.Manifest.permission.READ_INPUT_STATE,*/
            "MonitorInput()")) {
        //throw new SecurityException("Requires READ_INPUT_STATE permission");
        return E_SECURITY_EXCEPTION;
    }
    return mInputManager->MonitorInput(inputChannelName, channel);
}

// Get input device information.
ECode CWindowManagerService::GetInputDevice(
    /* [in] */ Int32 deviceId,
    /* [out] */ IInputDevice** device)
{
//    return mInputManager->GetInputDevice(deviceId, device);
    return E_NOT_IMPLEMENTED;
}

//    GetInputDeviceIds(
//        [out] int[] ids);

void CWindowManagerService::EnableScreenAfterBoot()
{
    Mutex::Autolock lock(mWindowMapLock);

    if (mSystemBooted) {
        return;
    }
    mSystemBooted = TRUE;

    PerformEnableScreen();
}

void CWindowManagerService::EnableScreenIfNeededLocked()
{
    if (mDisplayEnabled) {
        return;
    }
    if (!mSystemBooted) {
        return;
    }
    ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
    pHandlerFunc = &CWindowManagerService::HandleEnableScreen;

    SendMessage(*(Handle32*)&pHandlerFunc, NULL);
}

void CWindowManagerService::PerformEnableScreen()
{
    Mutex::Autolock lock(mWindowMapLock);

    if (mDisplayEnabled) {
        return;
    }
    if (!mSystemBooted) {
        return;
    }

    // Don't enable the screen until all existing windows
    // have been drawn.
    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mWindows.Begin(); it != mWindows.End(); ++it) {
        AutoPtr<WindowState> w = *it;
        if (w->IsVisibleLw() && !w->mObscured
                && (w->mOrientationChanging || !w->IsDrawnLw())) {
            return;
        }
    }

    mDisplayEnabled = TRUE;
//    if (FALSE) {
//        Slog.i(TAG, "ENABLING SCREEN!");
//        StringWriter sw = new StringWriter();
//        PrintWriter pw = new PrintWriter(sw);
//        this.dump(NULL, pw, NULL);
//        Slog.i(TAG, sw.toString());
//    }
//    try {
//    AutoPtr<IBinder> surfaceFlinger = ServiceManager.getService("SurfaceFlinger");
//    if (surfaceFlinger != NULL) {
//        //Slog.i(TAG, "******* TELLING SURFACE FLINGER WE ARE BOOTED!");
//        Parcel data = Parcel.obtain();
//        data.writeInterfaceToken("android.ui.ISurfaceComposer");
//        surfaceFlinger.transact(IBinder.FIRST_CALL_TRANSACTION,
//                                data, NULL, 0);
//        data.recycle();
//    }
//    } catch (RemoteException ex) {
//        Slog.e(TAG, "Boot completed: SurfaceFlinger is dead!");
//    }

    mPolicy->EnableScreenAfterBoot();

    // Make sure the last requested orientation has been applied.
    SetRotationUnchecked(WindowManagerPolicy::USE_LAST_ROTATION, FALSE,
            mLastRotationFlags | Surface_FLAGS_ORIENTATION_ANIMATION_DISABLE);
}

// For testing
ECode CWindowManagerService::SetInTouchMode(
    /* [in] */ Boolean showFocus)
{
    Mutex::Autolock lock(mWindowMapLock);

    mInTouchMode = showFocus;

    return NOERROR;
}

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
ECode CWindowManagerService::SetRotation(
    /* [in] */ Int32 rotation,
    /* [in] */ Boolean alwaysSendConfiguration,
    /* [in] */ Int32 animFlags)
{
    if (!CheckCallingPermission("elastos.permission.SET_ORIENTATION", /*android.Manifest.permission.SET_ORIENTATION,*/
            "SetRotation()")) {
        //throw new SecurityException("Requires SET_ORIENTATION permission");
        return E_SECURITY_EXCEPTION;
    }

    SetRotationUnchecked(rotation, alwaysSendConfiguration, animFlags);

    return NOERROR;
}

void CWindowManagerService::SetRotationUnchecked(
    /* [in] */ Int32 rotation,
    /* [in] */ Boolean alwaysSendConfiguration,
    /* [in] */ Int32 animFlags)
{
//    if(DEBUG_ORIENTATION) Slog.v(TAG,
//            "alwaysSendConfiguration set to "+alwaysSendConfiguration);
//
//    long origId = Binder.clearCallingIdentity();
    Boolean changed;
    {
        Mutex::Autolock lock(mWindowMapLock);

        changed = SetRotationUncheckedLocked(rotation, animFlags);
    }

    if (changed || alwaysSendConfiguration) {
        SendNewConfiguration();
    }

//    Binder.restoreCallingIdentity(origId);
}

/**
 * Apply a new rotation to the screen, respecting the requests of
 * applications.  Use WindowManagerPolicy.USE_LAST_ROTATION to simply
 * re-evaluate the desired rotation.
 *
 * Returns NULL if the rotation has been changed.  In this case YOU
 * MUST CALL setNewConfiguration() TO UNFREEZE THE SCREEN.
 */
Boolean CWindowManagerService::SetRotationUncheckedLocked(
    /* [in] */ Int32 rotation,
    /* [in] */ Int32 animFlags)
{
    Boolean changed;
    if (rotation == WindowManagerPolicy::USE_LAST_ROTATION) {
        rotation = mRequestedRotation;
    }
    else {
        mRequestedRotation = rotation;
        mLastRotationFlags = animFlags;
    }
//    if (DEBUG_ORIENTATION) Slog.v(TAG, "Overwriting rotation value from " + rotation);
    mPolicy->RotationForOrientationLw(mForcedAppOrientation,
            mRotation, mDisplayEnabled, &rotation);
//    if (DEBUG_ORIENTATION) Slog.v(TAG, "new rotation is set to " + rotation);
    changed = mDisplayEnabled && mRotation != rotation;

    if (changed) {
//        if (DEBUG_ORIENTATION) Slog.v(TAG,
//                "Rotation changed to " + rotation
//                + " from " + mRotation
//                + " (forceApp=" + mForcedAppOrientation
//                + ", req=" + mRequestedRotation + ")");
        mRotation = rotation;
        mWindowsFreezingScreen = TRUE;

        ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
        pHandlerFunc = &CWindowManagerService::HandleWindowFreezeTimeout;

        RemoveMessage(*(Handle32*)&pHandlerFunc);
        SendMessageDelayed(*(Handle32*)&pHandlerFunc, NULL, 2000);

        mWaitingForConfig = TRUE;
        mLayoutNeeded = TRUE;
        StartFreezingDisplayLocked();
//        Slog.i(TAG, "Setting rotation to " + rotation + ", animFlags=" + animFlags);
        mInputManager->SetDisplayOrientation(0, rotation);
        if (mDisplayEnabled) {
//            CSurface::SetOrientationEx(0, rotation, animFlags);
        }
        List<AutoPtr<WindowState> >::ReverseIterator rit;
        for (rit = mWindows.RBegin(); rit != mWindows.REnd(); ++rit) {
            AutoPtr<WindowState> w = *rit;
            if (w->mSurface != NULL) {
                w->mOrientationChanging = TRUE;
            }
        }
//        for (rit = mRotationWatchers.RBegin(); rit != mRotationWatchers.REnd(); ++rit) {
//            try {
//            (*rit)->OnRotationChanged(rotation);
//            } catch (RemoteException e) {
//            }
//        }
    }

    return changed;
}

/**
 * Retrieve the current screen orientation, constants as per
 * {@link android.view.Surface}.
 */
ECode CWindowManagerService::GetRotation(
    /* [out] */ Int32* rotation)
{
    assert(rotation != NULL);

    *rotation = mRotation;

    return NOERROR;
}

Boolean CWindowManagerService::UpdateWallpaperOffsetLocked(
    /* [in] */ WindowState* changingTarget,
    /* [in] */ Boolean sync)
{
    Int32 dw, dh;
    mDisplay->GetWidth(&dw);
    mDisplay->GetHeight(&dh);

    Boolean changed = FALSE;

    AutoPtr<WindowState> target = mWallpaperTarget;
    if (target != NULL) {
        if (target->mWallpaperX >= 0) {
            mLastWallpaperX = target->mWallpaperX;
        }
        else if (changingTarget->mWallpaperX >= 0) {
            mLastWallpaperX = changingTarget->mWallpaperX;
        }
        if (target->mWallpaperY >= 0) {
            mLastWallpaperY = target->mWallpaperY;
        }
        else if (changingTarget->mWallpaperY >= 0) {
            mLastWallpaperY = changingTarget->mWallpaperY;
        }
    }

    List<WindowToken*>::ReverseIterator rit;
    for (rit = mWallpaperTokens.RBegin(); rit != mWallpaperTokens.REnd(); ++rit) {
        WindowToken* token = *rit;
        List<AutoPtr<WindowState> >::ReverseIterator wRIt;
        for (wRIt = token->mWindows.RBegin(); wRIt != token->mWindows.REnd(); ++wRIt) {
            AutoPtr<WindowState> wallpaper = *wRIt;
            if (UpdateWallpaperOffsetLocked(wallpaper, dw, dh, sync)) {
                wallpaper->ComputeShownFrameLocked();
                changed = TRUE;
                // We only want to be synchronous with one wallpaper.
                sync = FALSE;
            }
        }
    }

    return changed;
}

void CWindowManagerService::UpdateWallpaperVisibilityLocked()
{
    Boolean visible = IsWallpaperVisible(mWallpaperTarget);
    Int32 dw, dh;
    mDisplay->GetWidth(&dw);
    mDisplay->GetHeight(&dh);

    List<WindowToken*>::ReverseIterator rit;
    for (rit = mWallpaperTokens.RBegin(); rit != mWallpaperTokens.REnd(); ++rit) {
        WindowToken* token = *rit;
        if (token->mHidden == visible) {
            token->mHidden = !visible;
            // Need to do a layout to ensure the wallpaper now has the
            // correct size.
            mLayoutNeeded = TRUE;
        }

        List<AutoPtr<WindowState> >::ReverseIterator wRIt;
        for (wRIt = token->mWindows.RBegin(); wRIt != token->mWindows.REnd(); ++wRIt) {
            AutoPtr<WindowState> wallpaper = *wRIt;
            if (visible) {
                UpdateWallpaperOffsetLocked(wallpaper, dw, dh, FALSE);
            }

            if (wallpaper->mWallpaperVisible != visible) {
                wallpaper->mWallpaperVisible = visible;
//                try {
//                if (DEBUG_VISIBILITY || DEBUG_WALLPAPER) Slog.v(TAG,
//                        "Updating visibility of wallpaper " + wallpaper
//                        + ": " + visible);
                wallpaper->mClient->DispatchAppVisibility(visible);
//                } catch (RemoteException e) {
//                }
            }
        }
    }
}

Int32 CWindowManagerService::AddWindow(
    /* [in] */ CWindowSession* session,
    /* [in] */ IInnerWindow* client,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Int32 viewVisibility,
    /* [in] */ IRect* inContentInsets,
    /* [in] */ IInputChannel* inInputChannel,
    /* [out] */ IRect** outContentInsets,
    /* [out] */ IInputChannel** outInputChannel)
{
    Int32 res;
//    int res = mPolicy.checkAddPermission(attrs);
//    if (res != WindowManagerImpl.ADD_OKAY) {
//        return res;
//    }

    Boolean reportNewConfig = FALSE;
    AutoPtr<WindowState> attachedWindow;
    AutoPtr<WindowState> win;

    {
        Mutex::Autolock lock(mWindowMapLock);

        // Instantiating a Display requires talking with the simulator,
        // so don't do it until we know the system is mostly up and
        // running.
        if (mDisplay == NULL) {
            AutoPtr<ILocalWindowManager> wm;
//            assert(SUCCEEDED(mContext->GetSystemService(
//                    Context_WINDOW_SERVICE, (IInterface**)&wm)));
            //todo:
            CWindowManagerImpl::AcquireSingleton((ILocalWindowManager**)&wm);

            AutoPtr<IDisplay> display;
            assert(SUCCEEDED(wm->GetDefaultDisplay((IDisplay**)&display)));
            mDisplay = display;
            mDisplay->GetWidth(&mInitialDisplayWidth);
            mDisplay->GetHeight(&mInitialDisplayHeight);
            mInputManager->SetDisplaySize(0, mInitialDisplayWidth, mInitialDisplayHeight);
            reportNewConfig = FALSE;
            //printf("=====%s, %d, Width: %d, Height: %d=====\n",
            //        __FILE__, __LINE__, mInitialDisplayWidth, mInitialDisplayHeight);
        }

        HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> >::Iterator it
            = mWindowMap.Find(AutoPtr<IBinder>(client));
        if (it != mWindowMap.End() && it->mSecond != NULL) {
//            Slogger::W(TAG, "Window " + client + " is already added");
            return WindowManagerImpl_ADD_DUPLICATE_ADD;
        }

        AutoPtr<IBinder> attrsToken;
        attrs->GetToken((IBinder**)&attrsToken);
        Int32 attrsType;
        attrs->GetType(&attrsType);
        if (attrsType >= WindowManagerLayoutParams_FIRST_SUB_WINDOW &&
                attrsType <= WindowManagerLayoutParams_LAST_SUB_WINDOW) {
            attachedWindow = WindowForClientLocked(NULL, attrsToken, FALSE);
            if (attachedWindow == NULL) {
//                Slogger::W(TAG, "Attempted to add window with token that is not a window: "
//                      + attrsCls->mToken + ".  Aborting.");
                return WindowManagerImpl_ADD_BAD_SUBWINDOW_TOKEN;
            }
            Int32 attachedWinType;
            attachedWindow->mAttrs->GetType(&attachedWinType);
            if (attachedWinType >= WindowManagerLayoutParams_FIRST_SUB_WINDOW
                    && attachedWinType <= WindowManagerLayoutParams_LAST_SUB_WINDOW) {
//                Slogger::W(TAG, "Attempted to add window with token that is a sub-window: "
//                        + attrsCls->mToken + ".  Aborting.");
                return WindowManagerImpl_ADD_BAD_SUBWINDOW_TOKEN;
            }
        }

        Boolean addToken = FALSE;
        WindowToken* token = NULL;
        if (attrsToken != NULL) {
            HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator tokenIt
                = mTokenMap.Find(attrsToken);
            if (tokenIt != mTokenMap.End()) {
                token = tokenIt->mSecond;
            }
        }

        if (token == NULL) {
            if (attrsType >= WindowManagerLayoutParams_FIRST_APPLICATION_WINDOW
                    && attrsType <= WindowManagerLayoutParams_LAST_APPLICATION_WINDOW) {
//                Slogger::W(TAG, "Attempted to add application window with unknown token "
//                      + attrsCls->mToken + ".  Aborting.");
                return WindowManagerImpl_ADD_BAD_APP_TOKEN;
            }
            if (attrsType == WindowManagerLayoutParams_TYPE_INPUT_METHOD) {
//                Slogger::W(TAG, "Attempted to add input method window with unknown token "
//                      + attrsCls->mToken + ".  Aborting.");
                return WindowManagerImpl_ADD_BAD_APP_TOKEN;
            }
            if (attrsType == WindowManagerLayoutParams_TYPE_WALLPAPER) {
//                Slogger::W(TAG, "Attempted to add wallpaper window with unknown token "
//                      + attrsCls->mToken + ".  Aborting.");
                return WindowManagerImpl_ADD_BAD_APP_TOKEN;
            }
            token = new WindowToken(attrsToken, -1, FALSE);
            addToken = TRUE;
        }
        else if (attrsType >= WindowManagerLayoutParams_FIRST_APPLICATION_WINDOW
                && attrsType <= WindowManagerLayoutParams_LAST_APPLICATION_WINDOW) {
            AppWindowToken* atoken = token->mAppWindowToken;
            if (atoken == NULL) {
//                Slogger::W(TAG, "Attempted to add window with non-application token "
//                      + token + ".  Aborting.");
                return WindowManagerImpl_ADD_NOT_APP_TOKEN;
            }
            else if (atoken->mRemoved) {
//                Slogger::W(TAG, "Attempted to add window with exiting application token "
//                      + token + ".  Aborting.");
                return WindowManagerImpl_ADD_APP_EXITING;
            }
            if (attrsType == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING
                    && atoken->mFirstWindowDrawn) {
                // No need for this guy!
                AutoPtr<ICharSequence> title;
                attrs->GetTitle((ICharSequence**)&title);
//                if (localLOGV) Slogger::V(
//                        TAG, "**** NO NEED TO START: " + title.GetPayload());
                return WindowManagerImpl_ADD_STARTING_NOT_NEEDED;
            }
        }
        else if (attrsType == WindowManagerLayoutParams_TYPE_INPUT_METHOD) {
            if (token->mWindowType != WindowManagerLayoutParams_TYPE_INPUT_METHOD) {
//                Slogger::W(TAG, "Attempted to add input method window with bad token "
//                        + attrsCls->mToken + ".  Aborting.");
                  return WindowManagerImpl_ADD_BAD_APP_TOKEN;
            }
        }
        else if (attrsType == WindowManagerLayoutParams_TYPE_WALLPAPER) {
            if (token->mWindowType != WindowManagerLayoutParams_TYPE_WALLPAPER) {
//                Slogger::W(TAG, "Attempted to add wallpaper window with bad token "
//                        + attrsCls->mToken + ".  Aborting.");
                  return WindowManagerImpl_ADD_BAD_APP_TOKEN;
            }
        }

        win = new WindowState(this, session, client, token,
                attachedWindow, attrs, viewVisibility);
//        if (win->mDeathRecipient == NULL) {
//            // Client has apparently died, so there is no reason to
//            // continue.
//            Slogger::W(TAG, "Adding window client " + client
//                    + " that is dead, aborting.");
//            return WindowManagerImpl_ADD_APP_EXITING;
//        }

        mPolicy->AdjustWindowParamsLw(win->mAttrs);

        mPolicy->PrepareAddWindowLw(win, attrs, &res);
        if (res != WindowManagerImpl_ADD_OKAY) {
            return res;
        }

        if (outInputChannel != NULL) {
            String name;
            win->MakeInputChannelName(&name);
            AutoPtr<IInputChannel> serverChannel;
            AutoPtr<IInputChannel> clientChannel;
            AutoPtr<IInputChannelHelper> helper;
            assert(SUCCEEDED(CInputChannelHelper::AcquireSingleton((IInputChannelHelper**)&helper)));
            assert(SUCCEEDED(helper->OpenInputChannelPair(name,
                    (IInputChannel**)&serverChannel, (IInputChannel**)&clientChannel)));
            win->mInputChannel = serverChannel;
            assert(SUCCEEDED(CInputChannel::New(outInputChannel)));
            clientChannel->TransferToBinderOutParameter(*outInputChannel);

            assert(SUCCEEDED(mInputManager->RegisterInputChannel(win->mInputChannel)));
        }

        // From now on, no exceptions or errors allowed!

        res = WindowManagerImpl_ADD_OKAY;

//        final long origId = Binder.clearCallingIdentity();

        if (addToken) {
            if (attrsToken != NULL)
                mTokenMap[attrsToken] = token;
            mTokenList.PushBack(token);
        }
        win->Attach();
        mWindowMap[client] = win;

        if (attrsType == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING &&
                token->mAppWindowToken != NULL) {
            token->mAppWindowToken->mStartingWindow = win;
        }

        Boolean imMayMove = TRUE;
        UNUSED(imMayMove);

        if (attrsType == WindowManagerLayoutParams_TYPE_INPUT_METHOD) {
           mInputMethodWindow = win;
           AddInputMethodWindowToListLocked(win);
           imMayMove = FALSE;
        }
        else if (attrsType == WindowManagerLayoutParams_TYPE_INPUT_METHOD_DIALOG) {
           mInputMethodDialogs.PushBack(win);
           AddWindowToListInOrderLocked(win, TRUE);
           AdjustInputMethodDialogsLocked();
           imMayMove = FALSE;
        }
        else {
            AddWindowToListInOrderLocked(win, TRUE);
            if (attrsType == WindowManagerLayoutParams_TYPE_WALLPAPER) {
                mLastWallpaperTimeoutTime = 0;
                AdjustWallpaperWindowsLocked();
            }
            else {
                Int32 attrsFlags;
                attrs->GetFlags(&attrsFlags);
                if ((attrsFlags & WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER) != 0) {
                    AdjustWallpaperWindowsLocked();
                }
            }
        }

        win->mEnterAnimationPending = TRUE;

        mPolicy->GetContentInsetHintLw(attrs, inContentInsets);

        if (mInTouchMode) {
            res |= WindowManagerImpl_ADD_FLAG_IN_TOUCH_MODE;
        }
        if (win == NULL || win->mAppToken == NULL || !win->mAppToken->mClientHidden) {
            res |= WindowManagerImpl_ADD_FLAG_APP_VISIBLE;
        }

        Boolean focusChanged = FALSE;
        if (win->CanReceiveKeys()) {
            focusChanged = UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_ASSIGN_LAYERS);
            if (focusChanged) {
                imMayMove = FALSE;
            }
        }

        if (imMayMove) {
            //MoveInputMethodWindowsIfNeededLocked(FALSE);
        }

        AssignLayersLocked();
        // Don't do layout here, the window must call
        // relayout to be displayed, so we'll do it there.

        //dump();

        if (focusChanged) {
            FinishUpdateFocusedWindowAfterAssignLayersLocked();
        }

//        if (localLOGV) Slogger::V(
//            TAG, "New client " + client
//            + ": window=" + win);

        if (win->IsVisibleOrAdding() && UpdateOrientationFromAppTokensLocked()) {
            reportNewConfig = TRUE;
        }
    }

    // sendNewConfiguration() checks caller permissions so we must call it with
    // privilege.  updateOrientationFromAppTokens() clears and resets the caller
    // identity anyway, so it's safe to just clear & restore around this whole
    // block.
//    final long origId = Binder.clearCallingIdentity();
//    if (reportNewConfig) {
//        SendNewConfiguration();
//    }
//    Binder.restoreCallingIdentity(origId);

    *outContentInsets = inContentInsets;
    (*outContentInsets)->AddRef();

    return res;
}

void CWindowManagerService::RemoveWindowLocked(
    /* [in] */ CWindowSession* session,
    /* [in] */ WindowState* win)
{
//    if (localLOGV || DEBUG_FOCUS) Slog.v(
//        TAG, "Remove " + win + " client="
//        + Integer.toHexString(System.identityHashCode(
//            win.mClient.asBinder()))
//        + ", surface=" + win.mSurface);
//
//    final long origId = Binder.clearCallingIdentity();

    win->DisposeInputChannel();

//    if (DEBUG_APP_TRANSITIONS) Slog.v(
//            TAG, "Remove " + win + ": mSurface=" + win.mSurface
//            + " mExiting=" + win.mExiting
//            + " isAnimating=" + win.isAnimating()
//            + " app-animation="
//            + (win.mAppToken != NULL ? win.mAppToken.animation : NULL)
//            + " inPendingTransaction="
//            + (win.mAppToken != NULL ? win.mAppToken.inPendingTransaction : false)
//            + " mDisplayFrozen=" + mDisplayFrozen);
    // Visibility of the removed window. Will be used later to update orientation later on.
    Boolean wasVisible = FALSE;
    // First, see if we need to run an animation.  If we do, we have
    // to hold off on removing the window until the animation is done.
    // If the display is frozen, just remove immediately, since the
    // animation wouldn't be seen.
    Boolean isOn;
    mPolicy->IsScreenOn(&isOn);
    if (win->mSurface != NULL && !mDisplayFrozen && isOn) {
        // If we are not currently running the exit animation, we
        // need to see about starting one.
        if ((wasVisible = win->IsWinVisibleLw())) {
            Int32 transit = WindowManagerPolicy::TRANSIT_EXIT;
            Int32 type;
            win->GetAttrs()->GetType(&type);
            if (type == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING) {
                transit = WindowManagerPolicy::TRANSIT_PREVIEW_DONE;
            }
            // Try starting an animation.
            if (ApplyAnimationLocked(win, transit, FALSE)) {
                win->mExiting = TRUE;
            }
        }
        if (win->mExiting || win->IsAnimating()) {
            // The exit animation is running... wait for it!
            //Slog.i(TAG, "*** Running exit animation...");
            win->mExiting = TRUE;
            win->mRemoveOnExit = TRUE;
            mLayoutNeeded = TRUE;
            UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES);
            PerformLayoutAndPlaceSurfacesLocked();
            if (win->mAppToken != NULL) {
                win->mAppToken->UpdateReportedVisibilityLocked();
            }
            //dump();
//            Binder.restoreCallingIdentity(origId);
            return;
        }
    }

    RemoveWindowInnerLocked(session, win);
    // Removing a visible window will effect the computed orientation
    // So just update orientation if needed.
    if (wasVisible && ComputeForcedAppOrientationLocked()
        != mForcedAppOrientation && UpdateOrientationFromAppTokensLocked()) {
        ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
        pHandlerFunc = &CWindowManagerService::HandleSendNewConfiguration;

        SendMessage(*(Handle32*)&pHandlerFunc, NULL);
    }
    UpdateFocusedWindowLocked(UPDATE_FOCUS_NORMAL);
//    Binder.restoreCallingIdentity(origId);
}

Int32 CWindowManagerService::RelayoutWindow(
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
    /* [out] */ ISurface** outSurface)
{
    Boolean displayed = FALSE;
    Boolean inTouchMode;
    Boolean configChanged;
//    long origId = Binder.clearCallingIdentity();

    {
        Mutex::Autolock lock(mWindowMapLock);

        AutoPtr<WindowState> win = WindowForClientLocked(session, client, FALSE);
        if (win == NULL) {
            return 0;
        }

        win->mRequestedWidth = requestedWidth;
        win->mRequestedHeight = requestedHeight;

        if (attrs != NULL) {
            mPolicy->AdjustWindowParamsLw(attrs);
        }

        Int32 winFlags = 0;
        win->mAttrs->GetFlags(&winFlags);
        Int32 attrChanges = 0;
        Int32 flagChanges = 0;
        if (attrs != NULL) {
            Int32 attrsFlags;
            attrs->GetFlags(&attrsFlags);
            flagChanges = winFlags ^= attrsFlags;
            win->mAttrs->SetFlags(winFlags);
            win->mAttrs->CopyFrom(attrs, &attrChanges);
        }

//        if (DEBUG_LAYOUT) Slogger::V(TAG, "Relayout " + win + ": " + win->mAttrs);

        if ((attrChanges & WindowManagerLayoutParams_ALPHA_CHANGED) != 0) {
            attrs->GetAlpha(&win->mAlpha);
        }

        win->mAttrs->GetFlags(&winFlags);
        Boolean scaledWindow =
            ((winFlags & WindowManagerLayoutParams_FLAG_SCALED) != 0);

        if (scaledWindow) {
            // requested{Width|Height} Surface's physical size
            // attrs.{width|height} Size on screen
            Int32 width, height;
            attrs->GetWidth(&width);
            attrs->GetHeight(&height);
            win->mHScale = (width  != requestedWidth)  ?
                    (width  / (Float)requestedWidth) : 1.0f;
            win->mVScale = (height != requestedHeight) ?
                    (height / (Float)requestedHeight) : 1.0f;
        }
        else {
            win->mHScale = win->mVScale = 1;
        }

        Boolean imMayMove = (flagChanges & (
                WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM |
                WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE)) != 0;

        Boolean focusMayChange = win->mViewVisibility != viewVisibility
                || ((flagChanges & WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE) != 0)
                || (!win->mRelayoutCalled);

        Boolean wallpaperMayMove = win->mViewVisibility != viewVisibility
                && (winFlags & WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER) != 0;

        win->mRelayoutCalled = TRUE;
        Int32 oldVisibility = win->mViewVisibility;
        win->mViewVisibility = viewVisibility;
        if (viewVisibility == View_VISIBLE &&
                (win->mAppToken == NULL || !win->mAppToken->mClientHidden)) {
            displayed = !win->IsVisibleLw();
            if (win->mExiting) {
                win->mExiting = FALSE;
                win->mAnimation = NULL;
            }
            if (win->mDestroying) {
                win->mDestroying = FALSE;
                mDestroySurface.Remove(win);
            }
            if (oldVisibility == View_GONE) {
                win->mEnterAnimationPending = TRUE;
            }
            if (displayed) {
                Boolean isScreenOn;
                mPolicy->IsScreenOn(&isScreenOn);
                if (win->mSurface != NULL && !win->mDrawPending
                        && !win->mCommitDrawPending && !mDisplayFrozen
                        && isScreenOn) {
                    ApplyEnterAnimationLocked(win);
                }
                if ((winFlags & WindowManagerLayoutParams_FLAG_TURN_SCREEN_ON) != 0) {
//                    if (DEBUG_VISIBILITY) Slogger::V(TAG,
//                            "Relayout window turning screen on: " + win);
                    win->mTurnOnScreen = TRUE;
                }
                Int32 diff = 0;
                if (win->mConfiguration != mCurConfiguration
                        && (win->mConfiguration == NULL
                                || (mCurConfiguration->Diff(win->mConfiguration, &diff), diff != 0))) {
                    win->mConfiguration = mCurConfiguration;
//                    if (DEBUG_CONFIGURATION) {
//                        Slogger::I(TAG, "Window " + win + " visible with new config: "
//                                + win->mConfiguration + " / 0x"
//                                + Integer.toHexString(diff));
//                    }
                    inConfig->SetTo(mCurConfiguration);
                }
            }
            if ((attrChanges & WindowManagerLayoutParams_FORMAT_CHANGED) != 0) {
                // To change the format, we need to re-build the surface.
                win->DestroySurfaceLocked();
                displayed = TRUE;
            }
//            try {
            AutoPtr<ISurface> surface;
            win->CreateSurfaceLocked((ISurface**)&surface);
            if (surface != NULL) {
                ECode ec = inSurface->CopyFrom(surface);
                if (FAILED(ec)) {
                    mInputMonitor->UpdateInputWindowsLw();
                    return 0;
                }
                win->mReportDestroySurface = FALSE;
                win->mSurfacePendingDestroy = FALSE;
//                if (SHOW_TRANSACTIONS) Slogger::I(TAG,
//                        "  OUT SURFACE " + outSurface + ": copied");
            }
            else {
                // For some reason there isn't a surface.  Clear the
                // caller's object so they see the same state.
                inSurface->ReleaseSurface();
            }
//            } catch (Exception e) {
//                mInputMonitor.updateInputWindowsLw();
//
//                Slog.w(TAG, "Exception thrown when creating surface for client "
//                         + client + " (" + win.mAttrs.getTitle() + ")",
//                         e);
//                Binder.restoreCallingIdentity(origId);
//                return 0;
//            }
            if (displayed) {
                focusMayChange = TRUE;
            }
            Int32 winType;
            win->mAttrs->GetType(&winType);
            if (winType == WindowManagerLayoutParams_TYPE_INPUT_METHOD
                    && mInputMethodWindow == NULL) {
                mInputMethodWindow = win;
                imMayMove = TRUE;
            }
            if (winType == WindowManagerLayoutParams_TYPE_INPUT_METHOD
                    && win->mAppToken != NULL
                    && win->mAppToken->mStartingWindow != NULL) {
                // Special handling of starting window over the base
                // window of the app: propagate lock screen flags to it,
                // to provide the correct semantics while starting.
                Int32 mask =
                    WindowManagerLayoutParams_FLAG_SHOW_WHEN_LOCKED
                    | WindowManagerLayoutParams_FLAG_DISMISS_KEYGUARD
                    | WindowManagerLayoutParams_FLAG_ALLOW_LOCK_WHILE_SCREEN_ON;
                Int32 swFlags;
                win->mAppToken->mStartingWindow->mAttrs->GetFlags(&swFlags);
                swFlags = (swFlags &~ mask) | (winFlags & mask);
                win->mAppToken->mStartingWindow->mAttrs->SetFlags(swFlags);
            }
        }
        else {
            win->mEnterAnimationPending = FALSE;
            if (win->mSurface != NULL) {
//                if (DEBUG_VISIBILITY) Slogger::I(TAG, "Relayout invis " + win
//                        + ": mExiting=" + win->mExiting
//                        + " mSurfacePendingDestroy=" + win->mSurfacePendingDestroy);
                // If we are not currently running the exit animation, we
                // need to see about starting one.
                if (!win->mExiting || win->mSurfacePendingDestroy) {
                    // Try starting an animation; if there isn't one, we
                    // can destroy the surface right away.
                    Int32 transit = WindowManagerPolicy::TRANSIT_EXIT;
                    Int32 type1;
                    win->GetAttrs()->GetType(&type1);
                    if (type1 == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING) {
                        transit = WindowManagerPolicy::TRANSIT_PREVIEW_DONE;
                    }
                    if (!win->mSurfacePendingDestroy && win->IsWinVisibleLw() &&
                          ApplyAnimationLocked(win, transit, FALSE)) {
                        focusMayChange = TRUE;
                        win->mExiting = TRUE;
                    }
                    else if (win->IsAnimating()) {
                        // Currently in a hide animation... turn this into
                        // an exit.
                        win->mExiting = TRUE;
                    }
                    else if (win == mWallpaperTarget) {
                        // If the wallpaper is currently behind this
                        // window, we need to change both of them inside
                        // of a transaction to avoid artifacts.
                        win->mExiting = TRUE;
                        win->mAnimating = TRUE;
                    }
                    else {
                        if (mInputMethodWindow == win) {
                            mInputMethodWindow = NULL;
                        }
                        win->DestroySurfaceLocked();
                    }
                }
            }

            Int32 flags1;
            if (win->mSurface == NULL || (win->GetAttrs()->GetFlags(&flags1), flags1
                    & WindowManagerLayoutParams_FLAG_KEEP_SURFACE_WHILE_ANIMATING) == 0
                    || win->mSurfacePendingDestroy) {
                // We are being called from a local process, which
                // means outSurface holds its current surface.  Ensure the
                // surface object is cleared, but we don't want it actually
                // destroyed at this point.
                win->mSurfacePendingDestroy = FALSE;
                inSurface->ReleaseSurface();
//                if (DEBUG_VISIBILITY) Slogger::I(TAG, "Releasing surface in: " + win);
            }
            else if (win->mSurface != NULL) {
//                if (DEBUG_VISIBILITY) Slogger::I(TAG,
//                        "Keeping surface, will report destroy: " + win);
                win->mReportDestroySurface = TRUE;
                inSurface->CopyFrom(win->mSurface);//interface or class???
            }
        }

        if (focusMayChange) {
            //System.out.println("Focus may change: " + win.mAttrs.getTitle());
            if (UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES)) {
                imMayMove = FALSE;
            }
            //System.out.println("Relayout " + win + ": focus=" + mCurrentFocus);
        }

        // updateFocusedWindowLocked() already assigned layers so we only need to
        // reassign them at this point if the IM window state gets shuffled
        Boolean assignLayers = FALSE;

        if (imMayMove) {
            if (MoveInputMethodWindowsIfNeededLocked(FALSE) || displayed) {
                // Little hack here -- we -should- be able to rely on the
                // function to return TRUE if the IME has moved and needs
                // its layer recomputed.  However, if the IME was hidden
                // and isn't actually moved in the list, its layer may be
                // out of data so we make sure to recompute it.
                assignLayers = TRUE;
            }
        }
        if (wallpaperMayMove) {
            if ((AdjustWallpaperWindowsLocked() & ADJUST_WALLPAPER_LAYERS_CHANGED) != 0) {
                assignLayers = TRUE;
            }
        }

        mLayoutNeeded = TRUE;
        win->mGivenInsetsPending = insetsPending;
        if (assignLayers) {
            AssignLayersLocked();
        }
        configChanged = UpdateOrientationFromAppTokensLocked();

        PerformLayoutAndPlaceSurfacesLocked();
        if (displayed && win->mIsWallpaper) {
            Int32 width, height;
            mDisplay->GetWidth(&width);
            mDisplay->GetHeight(&height);
            UpdateWallpaperOffsetLocked(win, width, height, FALSE);
        }
        if (win->mAppToken != NULL) {
            win->mAppToken->UpdateReportedVisibilityLocked();
        }
        inFrame->SetEx(win->mFrame);
        inContentInsets->SetEx(win->mContentInsets);
        inVisibleInsets->SetEx(win->mVisibleInsets);
//        if (localLOGV) Slogger::V(
//            TAG, "Relayout given client " + client
//            + ", requestedWidth=" + requestedWidth
//            + ", requestedHeight=" + requestedHeight
//            + ", viewVisibility=" + viewVisibility
//            + "\nRelayout returning frame=" + outFrame
//            + ", surface=" + outSurface);
//
//        if (localLOGV || DEBUG_FOCUS) Slogger::V(
//            TAG, "Relayout of " + win + ": focusMayChange=" + focusMayChange);

        inTouchMode = mInTouchMode;

        mInputMonitor->UpdateInputWindowsLw();
    }

    if (configChanged) {
        SendNewConfiguration();
    }

//    Binder.restoreCallingIdentity(origId);
//    CRect* _r = (CRect*)inFrame;
//printf("@@@@@@@@@ Left: %d, Right: %d, Top: %d, Bottom: %d @@@@@@@@@\n",
//    _r->mLeft, _r->mRight, _r->mTop, _r->mBottom);
    *outFrame = inFrame;
    (*outFrame)->AddRef();
    *outContentInsets = inContentInsets;
    (*outContentInsets)->AddRef();
    *outVisibleInsets = inVisibleInsets;
    (*outVisibleInsets)->AddRef();
    *outConfig = inConfig;
    (*outConfig)->AddRef();
    *outSurface = inSurface;
    (*outSurface)->AddRef();

    return (inTouchMode ? WindowManagerImpl_RELAYOUT_IN_TOUCH_MODE : 0)
            | (displayed ? WindowManagerImpl_RELAYOUT_FIRST_TIME : 0);
}

/**
 * Watch the rotation of the screen.  Returns the current rotation,
 * calls back when it changes.
 */
ECode CWindowManagerService::WatchRotation(
    /* [in] */ IRotationWatcher* watcher,
    /* [out] */ Int32* rotation)
{
    VALIDATE_NOT_NULL(rotation);

//    final IBinder watcherBinder = watcher.asBinder();
//    IBinder.DeathRecipient dr = new IBinder.DeathRecipient() {
//        public void binderDied() {
//            synchronized (mWindowMap) {
//                for (int i=0; i<mRotationWatchers.size(); i++) {
//                    if (watcherBinder == mRotationWatchers.get(i).asBinder()) {
//                        IRotationWatcher removed = mRotationWatchers.remove(i);
//                        if (removed != NULL) {
//                            removed.asBinder().unlinkToDeath(this, 0);
//                        }
//                        i--;
//                    }
//                }
//            }
//        }
//    };

    Mutex::Autolock lock(mWindowMapLock);
//        try {
//        watcher.asBinder().linkToDeath(dr, 0);
//        mRotationWatchers.PushBack(watcher);
//        } catch (RemoteException e) {
//            // Client died, no cleanup needed.
//        }

        *rotation = mRotation;
        return NOERROR;
}

// -------------------------------------------------------------
// Application Window Tokens
// -------------------------------------------------------------

void CWindowManagerService::ValidateAppTokens(
    /* [in] */ const List<AutoPtr<CActivityRecord> > & tokens)
{
    List<AutoPtr<CActivityRecord> >::ConstReverseIterator rit = tokens.RBegin();
    List<AppWindowToken*>::ReverseIterator appRIt = mAppTokens.RBegin();
    while (rit != tokens.REnd() && appRIt != mAppTokens.REnd()) {
        AppWindowToken* wtoken = *appRIt;
        if (wtoken->mRemoved) {
            ++appRIt;
            continue;
        }
        if ((IBinder*)rit->Get() != wtoken->mToken) {
//            Slog.w(TAG, "Tokens out of sync: external is " + tokens.get(v)
//                  + " @ " + v + ", internal is " + wtoken.token + " @ " + m);
        }
        ++rit;
        ++appRIt;
    }
    while (rit != tokens.REnd()) {
//        Slog.w(TAG, "External token not found: " + tokens.get(v) + " @ " + v);
        ++rit;
    }
    while (appRIt != mAppTokens.REnd()) {
        AppWindowToken* wtoken = *appRIt;
        if (!wtoken->mRemoved) {
//            Slog.w(TAG, "Invalid internal token: " + wtoken.token + " @ " + m);
        }
        ++appRIt;
    }
}

void CWindowManagerService::ApplyEnterAnimationLocked(
    /* [in] */ WindowState* win)
{
    Int32 transit = WindowManagerPolicy::TRANSIT_SHOW;
    if (win->mEnterAnimationPending) {
        win->mEnterAnimationPending = FALSE;
        transit = WindowManagerPolicy::TRANSIT_ENTER;
    }

    ApplyAnimationLocked(win, transit, TRUE);
}

Boolean CWindowManagerService::TmpRemoveAppWindowsLocked(
    /* [in] */ WindowToken* token)
{
    if (token->mWindows.Begin() == token->mWindows.End()) {
        return FALSE;
    }

    List<AutoPtr<WindowState> >::Iterator it = token->mWindows.Begin();
    while (it != token->mWindows.End()) {
        AutoPtr<WindowState> win = *it;
//            if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Tmp removing app window " + win);

        mWindowsChanged = TRUE;
        List<AutoPtr<WindowState> >::ReverseIterator crit = win->mChildWindows.RBegin();
        while (crit != win->mChildWindows.REnd()) {
            AutoPtr<WindowState> cwin = *crit;
//                if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG,
//                        "Tmp removing child window " + cwin);
            mWindows.Remove(cwin);
            ++crit;
        }
        it = mWindows.Erase(it);
    }
    return TRUE;
}

List<AutoPtr<CWindowManagerService::WindowState> >::Iterator
CWindowManagerService::FindWindowOffsetLocked(
    /* [in] */ List<AppWindowToken*>::Iterator tokenPosIt)
{

    if (tokenPosIt == mAppTokens.End()) {
        List<AutoPtr<WindowState> >::ReverseIterator wsrit = mWindows.RBegin();
        while (wsrit != mWindows.REnd()) {
            AutoPtr<WindowState> win = *wsrit;
            if (win->GetAppToken() != NULL) {
                return wsrit.GetBase();
            }
        }
    }

    List<AppWindowToken*>::ReverseIterator tokenPosRit(tokenPosIt);

    while (tokenPosRit != mAppTokens.REnd()) {
        // Find the first app token below the new position that has
        // a window displayed.
        AppWindowToken* wtoken = *tokenPosRit;
//        if (DEBUG_REORDER) Slog.v(TAG, "Looking for lower windows @ "
//                + tokenPos + " -- " + wtoken.token);
        if (wtoken->mSendingToBottom) {
//            if (DEBUG_REORDER) Slog.v(TAG,
//                    "Skipping token -- currently sending to bottom");
            tokenPosRit++;
            continue;
        }
        List<AutoPtr<WindowState> >::ReverseIterator wsRit = wtoken->mWindows.RBegin();
        while (wsRit != wtoken->mWindows.REnd()) {
            AutoPtr<WindowState> win = *wsRit;
            List<AutoPtr<WindowState> >::ReverseIterator cwsRit = win->mChildWindows.RBegin();
            while (cwsRit != win->mChildWindows.REnd()) {
                AutoPtr<WindowState> cwin = *cwsRit;
                if (cwin->mSubLayer >= 0) {
                    List<AutoPtr<WindowState> >::ReverseIterator wsRit2 = mWindows.RBegin();
                    for (;wsRit2 != mWindows.REnd(); wsRit2++) {
                        if (*wsRit2 == cwin) {
//                            if (DEBUG_REORDER) Slog.v(TAG,
//                                    "Found child win @" + (pos+1));
                            return wsRit2.GetBase();
                        }
                    }
                }
            }
            List<AutoPtr<WindowState> >::ReverseIterator wsRit2 = mWindows.RBegin();
            for (;wsRit2 != mWindows.REnd(); wsRit2++) {
                if (*wsRit2 == win) {
//                      if (DEBUG_REORDER) Slog.v(TAG,
//                              "Found child win @" + (pos+1));
                    return wsRit2.GetBase();
                }
            }
        }
        tokenPosRit++;
    }

    return mWindows.Begin();
}

List<AutoPtr<CWindowManagerService::WindowState> >::Iterator
CWindowManagerService::ReAddWindowLocked(
        /* [in] */ List<AutoPtr<WindowState> >::Iterator indexIt,
        /* [in] */ WindowState* win)
{
    Boolean added = FALSE;
    List<AutoPtr<WindowState> >::Iterator cwIt = win->mChildWindows.Begin();
    for (; cwIt != win->mChildWindows.End(); cwIt++) {
        AutoPtr<WindowState> cwin = *cwIt;
        if (!added && cwin->mSubLayer >= 0) {
//            if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Re-adding child window at "
//                    + index + ": " + cwin);
            indexIt = mWindows.Insert(indexIt, win);
            indexIt++;
            added = TRUE;
        }
//        if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Re-adding window at "
//                + index + ": " + cwin);
        indexIt = mWindows.Insert(indexIt, cwin);
        indexIt++;
    }
    if (!added) {
//        if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Re-adding window at "
//                + index + ": " + win);
        indexIt = mWindows.Insert(indexIt, win);
        indexIt++;
    }
    mWindowsChanged = TRUE;
    return indexIt;
}

List<AutoPtr<CWindowManagerService::WindowState> >::Iterator
CWindowManagerService::ReAddAppWindowsLocked(
    /* [in] */ List<AutoPtr<WindowState> >::Iterator indexIt,
    /* [in] */ WindowToken* token)
{
    List<AutoPtr<WindowState> >::Iterator it = token->mWindows.Begin();
    for (; it != token->mWindows.End(); ++it) {
        indexIt = ReAddWindowLocked(indexIt, *it);
    }
    return indexIt;
}

void CWindowManagerService::RemoveAppTokensLocked(
    /* [in] */ IObjectContainer* tokens)
{
    // XXX This should be done more efficiently!
    // (take advantage of the fact that both lists should be
    // ordered in the same way.)
    AutoPtr<IObjectEnumerator> objEmu;
    tokens->GetObjectEnumerator((IObjectEnumerator**)&objEmu);
    if (objEmu.Get() == NULL) {
        return;
    }

    Boolean isSucceeded;
    objEmu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IBinder> token;
        objEmu->Current((IInterface**)&token);
        if (token) {
            AppWindowToken* wtoken = FindAppWindowToken(token);
            Boolean isExit = FALSE;
            List<AppWindowToken*>::Iterator it;
            for (it = mAppTokens.Begin(); it != mAppTokens.End(); ++it) {
                if (*it == wtoken) {
                    isExit = TRUE;
                    mAppTokens.Erase(it);
                    break;
                }
            }
            if (!isExit) {
//                Slog.w(TAG, "Attempting to reorder token that doesn't exist: "
//                      + token + " (" + wtoken + ")");
            }
        }
        objEmu->MoveNext(&isSucceeded);
    }

//?????????????????????????have question
    // XXX This should be done more efficiently!
        // (take advantage of the fact that both lists should be
        // ordered in the same way.)
//        int N = tokens.size();
//        for (int i=0; i<N; i++) {
//            IBinder token = tokens.get(i);
//            final AppWindowToken wtoken = findAppWindowToken(token);
//            if (!mAppTokens.remove(wtoken)) {
//                Slog.w(TAG, "Attempting to reorder token that doesn't exist: "
//                      + token + " (" + wtoken + ")");
//                i--;
//                N--;
//            }
//        }
}

void CWindowManagerService::MoveAppWindowsLocked(
    /* [in] */ AppWindowToken* wtoken,
    /* [in] */ List<AppWindowToken*>::Iterator tokenPosIt,
    /* [in] */ Boolean updateFocusAndLayout)
{
    // First remove all of the windows from the list.
    TmpRemoveAppWindowsLocked(wtoken);

    // Where to start adding?
    List<AutoPtr<WindowState> >::Iterator posIt = \
        FindWindowOffsetLocked(tokenPosIt);

    // And now add them back at the correct place.
    posIt = ReAddAppWindowsLocked(posIt, wtoken);

    if (updateFocusAndLayout) {
        if (!UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES)) {
            AssignLayersLocked();
        }
        mLayoutNeeded = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
    }
}

void CWindowManagerService::MoveAppWindowsLocked(
    /* [in] */ IObjectContainer* tokens,
    /* [in] */ List<AppWindowToken*>::Iterator tokenPosIt)
{
    // First remove all of the windows from the list.
    AutoPtr<IObjectEnumerator> objEmu;
    ASSERT_SUCCEEDED(tokens->GetObjectEnumerator((IObjectEnumerator**)&objEmu));

    Boolean isSucceeded;
    objEmu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IBinder> tokenBinder;
        objEmu->Current((IInterface**)&tokenBinder);
        WindowToken* token = NULL;
        HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator it
                = mTokenMap.Find(tokenBinder);
        if (it != mTokenMap.End()) {
            token = it->mSecond;
        }
        if (token != NULL) {
            TmpRemoveAppWindowsLocked(token);
        }
        objEmu->MoveNext(&isSucceeded);
    }

    // Where to start adding?
    List<AutoPtr<WindowState> >::Iterator posIt = FindWindowOffsetLocked(tokenPosIt);

    // And now add them back at the correct place.
    objEmu->Reset();
    objEmu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IBinder> tokenBinder;
        objEmu->Current((IInterface**)&tokenBinder);
        WindowToken* token = NULL;
        HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator it
                = mTokenMap.Find(tokenBinder);
        if (it != mTokenMap.End()) {
            token = it->mSecond;
        }
        if (token != NULL) {
            posIt = ReAddAppWindowsLocked(posIt, token);
        }
        objEmu->MoveNext(&isSucceeded);
    }

    if (!UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES)) {
        AssignLayersLocked();
    }
    mLayoutNeeded = TRUE;
    PerformLayoutAndPlaceSurfacesLocked();

    //dump();
}

Boolean CWindowManagerService::UpdateFocusedWindowLocked(
    /* [in] */ Int32 mode)
{
    AutoPtr<WindowState> newFocus = ComputeFocusedWindowLocked();
    if (mCurrentFocus != newFocus) {
        // This check makes sure that we don't already have the focus
        // change message pending.
        ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
        pHandlerFunc = &CWindowManagerService::HandleReportFocusChange;

        RemoveMessage(*(Handle32*)&pHandlerFunc);
        SendMessage(*(Handle32*)&pHandlerFunc, NULL);

//        if (localLOGV) Slog.v(
//            TAG, "Changing focus from " + mCurrentFocus + " to " + newFocus);
        AutoPtr<WindowState> oldFocus = mCurrentFocus;
        mCurrentFocus = newFocus;
        mLosingFocus.Remove(newFocus);

        AutoPtr<WindowState> imWindow = mInputMethodWindow;
        if (newFocus != imWindow && oldFocus != imWindow) {
            if (MoveInputMethodWindowsIfNeededLocked(
                    mode != UPDATE_FOCUS_WILL_ASSIGN_LAYERS &&
                    mode != UPDATE_FOCUS_WILL_PLACE_SURFACES)) {
                mLayoutNeeded = TRUE;
            }
            if (mode == UPDATE_FOCUS_PLACING_SURFACES) {
                PerformLayoutLockedInner();
            }
            else if (mode == UPDATE_FOCUS_WILL_PLACE_SURFACES) {
                // Client will do the layout, but we need to assign layers
                // for handleNewWindowLocked() below.
                AssignLayersLocked();
            }
        }

        if (mode != UPDATE_FOCUS_WILL_ASSIGN_LAYERS) {
            // If we defer assigning layers, then the caller is responsible for
            // doing this part.
            FinishUpdateFocusedWindowAfterAssignLayersLocked();
        }
        return TRUE;
    }
    return FALSE;
}

void CWindowManagerService::FinishUpdateFocusedWindowAfterAssignLayersLocked()
{
    mInputMonitor->SetInputFocusLw(mCurrentFocus);
}

AutoPtr<CWindowManagerService::WindowState>
CWindowManagerService::ComputeFocusedWindowLocked()
{
    AutoPtr<WindowState> result;

    List<AppWindowToken*>::ReverseIterator nextAppRit = mAppTokens.RBegin();
    WindowToken* nextApp =
            (mAppTokens.Begin() != mAppTokens.End()) ? *nextAppRit : NULL;

    List<AutoPtr<WindowState> >::ReverseIterator wsRit;
    for (wsRit = mWindows.RBegin(); wsRit != mWindows.REnd(); ++wsRit) {
        AutoPtr<WindowState> win = *wsRit;

//        if (localLOGV || DEBUG_FOCUS) Slog.v(
//            TAG, "Looking for focus: " + i
//            + " = " + win
//            + ", flags=" + win.mAttrs.flags
//            + ", canReceive=" + win.canReceiveKeys());

        AppWindowToken* thisApp = win->mAppToken;

        // If this window's application has been removed, just skip it.
        if (thisApp != NULL && thisApp->mRemoved) {
            continue;
        }

        // If there is a focused app, don't allow focus to go to any
        // windows below it.  If this is an application window, step
        // through the app tokens until we find its app.
        Int32 winType;
        win->mAttrs->GetType(&winType);
        if (thisApp != NULL && nextApp != NULL && thisApp != nextApp
                && winType != WindowManagerLayoutParams_TYPE_APPLICATION_STARTING) {
            List<AppWindowToken*>::ReverseIterator origAppRit = nextAppRit;
            while (nextAppRit != mAppTokens.REnd()) {
                if (nextApp == mFocusedApp) {
                    // Whoops, we are below the focused app...  no focus
                    // for you!
//                    if (localLOGV || DEBUG_FOCUS) Slog.v(
//                        TAG, "Reached focused app: " + mFocusedApp);
                    return NULL;
                }
                ++nextAppRit;
                nextApp = *nextAppRit;
                if (nextApp == thisApp) {
                    break;
                }
            }
            if (thisApp != nextApp) {
                // Uh oh, the app token doesn't exist!  This shouldn't
                // happen, but if it does we can get totally hosed...
                // so restart at the original app.
                nextAppRit = origAppRit;
                nextApp = *nextAppRit;
            }
        }

        // Dispatch to this window if it is wants key events.
        if (win->CanReceiveKeys()) {
//            if (DEBUG_FOCUS) Slog.v(
//                    TAG, "Found focus @ " + i + " = " + win);
            result = win;
            break;
        }
    }

    return result;
}

Boolean CWindowManagerService::MoveInputMethodWindowsIfNeededLocked(
    /* [in] */ Boolean needAssignLayers)
{
    AutoPtr<WindowState> imWin = mInputMethodWindow;
    if (imWin == NULL && mInputMethodDialogs.Begin() == mInputMethodDialogs.End()) {
        return FALSE;
    }

    List<AutoPtr<WindowState> >::Iterator imPosIt =
            FindDesiredInputMethodWindowIndexLocked(TRUE);
    if (imPosIt != List<AutoPtr<WindowState> >::Iterator(NULL)) {
        // In this case, the input method windows are to be placed
        // immediately above the window they are targeting.

        // First check to see if the input method windows are already
        // located here, and contiguous.
        AutoPtr<WindowState> firstImWin = (imPosIt != mWindows.End()) ? *imPosIt : NULL;

        // Figure out the actual input method window that should be
        // at the bottom of their stack.
        AutoPtr<WindowState> baseImWin = imWin != NULL
                ? imWin : *(mInputMethodDialogs.Begin());
        if (baseImWin->mChildWindows.Begin() != baseImWin->mChildWindows.End()) {
            AutoPtr<WindowState> cw = *(baseImWin->mChildWindows.Begin());
            if (cw->mSubLayer < 0) baseImWin = cw;
        }

        if (firstImWin == baseImWin) {
            // The windows haven't moved...  but are they still contiguous?
            // First find the top IM window.
            List<AutoPtr<WindowState> >::Iterator posIt = imPosIt;
            if (posIt != mWindows.End()) {
                for (++posIt; posIt != mWindows.End(); ++posIt) {
                    if (!(*posIt)->mIsImWindow) {
                        break;
                    }
                }

                if (posIt != mWindows.End())
                    ++posIt;

                // Now there should be no more input method windows above.
                for (; posIt != mWindows.End(); ++posIt) {
                    if ((*posIt)->mIsImWindow) {
                        break;
                    }
                }
            }

            if (posIt == mWindows.End()) {
                // All is good!
                return FALSE;
            }
        }

        if (imWin != NULL) {
//            if (DEBUG_INPUT_METHOD) {
//                Slog.v(TAG, "Moving IM from " + imPos);
//                logWindowList("  ");
//            }
            imPosIt = TmpRemoveWindowLocked(imPosIt, imWin);
//            if (DEBUG_INPUT_METHOD) {
//                Slog.v(TAG, "List after moving with new pos " + imPos + ":");
//                logWindowList("  ");
//            }
            imWin->mTargetAppToken = mInputMethodTarget->mAppToken;
            ReAddWindowLocked(imPosIt, imWin);
//            if (DEBUG_INPUT_METHOD) {
//                Slog.v(TAG, "List after moving IM to " + imPos + ":");
//                logWindowList("  ");
//            }
            if (mInputMethodDialogs.Begin() != mInputMethodDialogs.End()) {
                List<AutoPtr<WindowState> >::Iterator temp = imPosIt;
                MoveInputMethodDialogsLocked(++temp);
            }
        }
        else {
            MoveInputMethodDialogsLocked(imPosIt);
        }
    }
    else {
        // In this case, the input method windows go in a fixed layer,
        // because they aren't currently associated with a focus window.

        if (imWin != NULL) {
//            if (DEBUG_INPUT_METHOD) Slog.v(TAG, "Moving IM from " + imPos);
            TmpRemoveWindowLocked(mWindows.Begin(), imWin);
            imWin->mTargetAppToken = NULL;
            ReAddWindowToListInOrderLocked(imWin);
//            if (DEBUG_INPUT_METHOD) {
//                Slog.v(TAG, "List with no IM target:");
//                logWindowList("  ");
//            }
            if (mInputMethodDialogs.Begin() != mInputMethodDialogs.End()) {
                MoveInputMethodDialogsLocked(List<AutoPtr<WindowState> >::Iterator(NULL));
            }
        }
        else {
            MoveInputMethodDialogsLocked(List<AutoPtr<WindowState> >::Iterator(NULL));
        }

    }

    if (needAssignLayers) {
        AssignLayersLocked();
    }

    return TRUE;
}

ECode CWindowManagerService::PerformLayoutAndPlaceSurfacesLocked()
{
    if (mInLayout) {
//        if (DEBUG) {
//            throw new RuntimeException("Recursive call!");
//            Slogger::W(TAG, "Recursive call!");
//            return E_RUNTIME_EXCEPTION;
//        }
//        Slog.w(TAG, "performLayoutAndPlaceSurfacesLocked called while in layout");
          return NOERROR;
    }

    if (mWaitingForConfig) {
        // Our configuration has changed (most likely rotation), but we
        // don't yet have the complete configuration to report to
        // applications.  Don't do any window layout until we have it.
        return NOERROR;
    }

    if (mDisplay == NULL) {
        // Not yet initialized, nothing to do.
        return NOERROR;
    }

    Boolean recoveringMemory = FALSE;
    if (mForceRemoves != NULL) {
        recoveringMemory = TRUE;
        // Wait a little it for things to settle down, and off we go.
        List<AutoPtr<WindowState> >::Iterator it;
        for (it = mForceRemoves->Begin(); it != mForceRemoves->End(); ++it) {
            AutoPtr<WindowState> ws = *it;
//            Slog.i(TAG, "Force removing: " + ws);
            RemoveWindowInnerLocked(ws->mSession, ws);
        }
        mForceRemoves = NULL;
//        Slog.w(TAG, "Due to memory failure, waiting a bit for next layout");
//        Object tmp = new Object();
//        synchronized (tmp) {
//            try {
//                tmp.wait(250);
//            } catch (InterruptedException e) {
//            }
//        }
    }

    List<AutoPtr<WindowState> >::ReverseIterator rit;
    mInLayout = TRUE;
//    try {
    PerformLayoutAndPlaceSurfacesLockedInner(recoveringMemory);

    ECode ec;
    rit = mPendingRemove.RBegin();
    if (rit != mPendingRemove.REnd()) {
        for (; rit != mPendingRemove.REnd(); ++rit) {
            AutoPtr<WindowState> w = *rit;
            RemoveWindowInnerLocked(w->mSession, w);
        }
        mPendingRemove.Clear();

        mInLayout = FALSE;
        AssignLayersLocked();
        mLayoutNeeded = TRUE;
        ec = PerformLayoutAndPlaceSurfacesLocked();
        if (FAILED(ec)) {
            mInLayout = FALSE;
//          Slog.e(TAG, "Unhandled exception while layout out windows", e);
            return ec;
        }
    }
    else {
        mInLayout = FALSE;
        if (mLayoutNeeded) {
            ec = RequestAnimationLocked(0);
            if (FAILED(ec)) {
                mInLayout = FALSE;
//              Slog.e(TAG, "Unhandled exception while layout out windows", e);
                return ec;
            }
        }
    }

    if (mWindowsChanged && !mWindowChangeListeners.IsEmpty()) {
        ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
        pHandlerFunc = &CWindowManagerService::HandleReportWindowsChange;

        RemoveMessage(*(Handle32*)&pHandlerFunc);
        SendMessage(*(Handle32*)&pHandlerFunc, NULL);
    }

    return ec;
//    } catch (RuntimeException e) {
//        mInLayout = false;
//        Slog.e(TAG, "Unhandled exception while layout out windows", e);
//    }
}

Int32 CWindowManagerService::PerformLayoutLockedInner()
{
    if (!mLayoutNeeded) {
        return 0;
    }

    mLayoutNeeded = FALSE;

    Int32 dw, dh;
    mDisplay->GetWidth(&dw);
    mDisplay->GetHeight(&dh);

//    if (DEBUG_LAYOUT) Slog.v(TAG, "performLayout: needed="
//            + mLayoutNeeded + " dw=" + dw + " dh=" + dh);

    mPolicy->BeginLayoutLw(dw, dh);

    Int32 seq = mLayoutSeq + 1;
    if (seq < 0) seq = 0;
    mLayoutSeq = seq;

    // First perform layout of any root windows (not attached
    // to another window).
//    Int32 topAttached = -1;
    List<AutoPtr<WindowState> >::ReverseIterator topAttachedRIt = mWindows.REnd();
    List<AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = mWindows.RBegin(); rit != mWindows.REnd(); ++rit) {
        AutoPtr<WindowState> win = *rit;

        // Don't do layout of a window if it is not visible, or
        // soon won't be visible, to avoid wasting time and funky
        // changes while a window is animating away.
        AppWindowToken* atoken = win->mAppToken;
        Boolean gone = win->mViewVisibility == View_GONE
                || !win->mRelayoutCalled
                || win->mRootToken->mHidden
                || (atoken != NULL && atoken->mHiddenRequested)
                || win->mAttachedHidden
                || win->mExiting || win->mDestroying;

        if (!win->mLayoutAttached) {
//            if (DEBUG_LAYOUT) Slog.v(TAG, "First pass " + win
//                    + ": gone=" + gone + " mHaveFrame=" + win.mHaveFrame
//                    + " mLayoutAttached=" + win.mLayoutAttached);
//            if (DEBUG_LAYOUT && gone) Slog.v(TAG, "  (mViewVisibility="
//                    + win.mViewVisibility + " mRelayoutCalled="
//                    + win.mRelayoutCalled + " hidden="
//                    + win.mRootToken.hidden + " hiddenRequested="
//                    + (atoken != NULL && atoken.hiddenRequested)
//                    + " mAttachedHidden=" + win.mAttachedHidden);
        }

        // If this view is GONE, then skip it -- keep the current
        // frame, and let the caller know so they can ignore it
        // if they want.  (We do the normal layout for INVISIBLE
        // windows, since that means "perform layout as normal,
        // just don't display").
        if (!gone || !win->mHaveFrame) {
            if (!win->mLayoutAttached) {
                ASSERT_SUCCEEDED(mPolicy->LayoutWindowLw(
                        win, win->mAttrs, NULL));
                win->mLayoutSeq = seq;
//                if (DEBUG_LAYOUT) Slog.v(TAG, "-> mFrame="
//                        + win.mFrame + " mContainingFrame="
//                        + win.mContainingFrame + " mDisplayFrame="
//                        + win.mDisplayFrame);
            }
            else {
                if (topAttachedRIt == mWindows.REnd()) topAttachedRIt = rit;
            }
        }
    }

    // Now perform layout of attached windows, which usually
    // depend on the position of the window they are attached to.
    // XXX does not deal with windows that are attached to windows
    // that are themselves attached.
    for (rit = topAttachedRIt; rit != mWindows.REnd(); ++rit) {
        AutoPtr<WindowState> win = *rit;

        // If this view is GONE, then skip it -- keep the current
        // frame, and let the caller know so they can ignore it
        // if they want.  (We do the normal layout for INVISIBLE
        // windows, since that means "perform layout as normal,
        // just don't display").
        if (win->mLayoutAttached) {
//            if (DEBUG_LAYOUT) Slog.v(TAG, "Second pass " + win
//                    + " mHaveFrame=" + win.mHaveFrame
//                    + " mViewVisibility=" + win.mViewVisibility
//                    + " mRelayoutCalled=" + win.mRelayoutCalled);
            if ((win->mViewVisibility != View_GONE && win->mRelayoutCalled)
                    || !win->mHaveFrame) {
                ASSERT_SUCCEEDED(mPolicy->LayoutWindowLw(win,
                        win->mAttrs, win->mAttachedWindow));
                win->mLayoutSeq = seq;
//                if (DEBUG_LAYOUT) Slog.v(TAG, "-> mFrame="
//                        + win.mFrame + " mContainingFrame="
//                        + win.mContainingFrame + " mDisplayFrame="
//                        + win.mDisplayFrame);
            }
        }
    }

    // Window frames may have changed.  Tell the input dispatcher about it.
    mInputMonitor->UpdateInputWindowsLw();

    Int32 bitSet;
    mPolicy->FinishLayoutLw(&bitSet);

    return bitSet;
}

void CWindowManagerService::PerformLayoutAndPlaceSurfacesLockedInner(
    /* [in] */ Boolean recoveringMemory)
{
    Millisecond64 currentTime = SystemClock::GetUptimeMillis();
    Int32 dw, dh;
    mDisplay->GetWidth(&dw);
    mDisplay->GetHeight(&dh);

    if (mFocusMayChange) {
        mFocusMayChange = FALSE;
        UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES);
    }

    // Initialize state of exiting tokens.
    List<WindowToken*>::ReverseIterator winRIt = mExitingTokens.RBegin();
    for (; winRIt != mExitingTokens.REnd(); ++winRIt) {
        (*winRIt)->mHasVisible = FALSE;
    }

    // Initialize state of exiting applications.
    List<AppWindowToken*>::ReverseIterator appWinRIt = mExitingAppTokens.RBegin();
    for (; appWinRIt != mExitingAppTokens.REnd(); ++appWinRIt) {
        (*appWinRIt)->mHasVisible = FALSE;
    }

    Boolean orientationChangeComplete = TRUE;
    AutoPtr<IWindowSession> holdScreen;
    Float screenBrightness = -1;
    Float buttonBrightness = -1;
    Boolean focusDisplayed = FALSE;
    Boolean animating = FALSE;
    Boolean createWatermark = FALSE;

    if (mFxSession == NULL) {
        CSurfaceSession::New((ISurfaceSession**)&mFxSession);
        createWatermark = TRUE;
    }

//    if (SHOW_TRANSACTIONS) Slog.i(TAG, ">>> OPEN TRANSACTION");
    AutoPtr<ISurfaceHelper> helper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
    helper->OpenTransaction();

//    if (createWatermark) {
//        CreateWatermark();
//    }
//    if (mWatermark != NULL) {
//        mWatermark->PositionSurface(dw, dh);
//    }

//    try {
    Boolean wallpaperForceHidingChanged = FALSE;
    Int32 repeats = 0;
    Int32 changes = 0;

    do {
        repeats++;
        if (repeats > 6) {
//            Slog.w(TAG, "Animation repeat aborted after too many iterations");
            mLayoutNeeded = FALSE;
            break;
        }

        if ((changes & (WindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER
                | WindowManagerPolicy::FINISH_LAYOUT_REDO_CONFIG
                | WindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT)) != 0) {
            if ((changes & WindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER) != 0) {
                if ((AdjustWallpaperWindowsLocked() & ADJUST_WALLPAPER_LAYERS_CHANGED) != 0) {
                    AssignLayersLocked();
                    mLayoutNeeded = TRUE;
                }
            }
            if ((changes & WindowManagerPolicy::FINISH_LAYOUT_REDO_CONFIG) != 0) {
//                if (DEBUG_LAYOUT) Slog.v(TAG, "Computing new config from layout");
                if (UpdateOrientationFromAppTokensLocked()) {
                    mLayoutNeeded = TRUE;
                    ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
                    pHandlerFunc = &CWindowManagerService::HandleSendNewConfiguration;

                    SendMessage(*(Handle32*)&pHandlerFunc, NULL);
                }
            }
            if ((changes & WindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT) != 0) {
                mLayoutNeeded = TRUE;
            }
        }

        // FIRST LOOP: Perform a layout, if needed.
        if (repeats < 4) {
            changes = PerformLayoutLockedInner();
            if (changes != 0) {
                continue;
            }
        }
        else {
//            Slog.w(TAG, "Layout repeat skipped after too many iterations");
            changes = 0;
        }

        Int32 transactionSequence = ++mTransactionSequence;

        // Update animations of all applications, including those
        // associated with exiting/removed apps
        Boolean tokensAnimating = FALSE;
        List<AppWindowToken*>::Iterator awtIt;
        for (awtIt = mAppTokens.Begin(); awtIt != mAppTokens.End(); ++awtIt) {
            if ((*awtIt)->StepAnimationLocked(currentTime, dw, dh)) {
                tokensAnimating = TRUE;
            }
        }
        for (awtIt = mExitingAppTokens.Begin(); awtIt != mExitingAppTokens.End(); ++awtIt) {
            if ((*awtIt)->StepAnimationLocked(currentTime, dw, dh)) {
                tokensAnimating = TRUE;
            }
        }

        // SECOND LOOP: Execute animations and update visibility of windows.

//        if (DEBUG_APP_TRANSITIONS) Slog.v(TAG, "*** ANIM STEP: seq="
//                + transactionSequence + " tokensAnimating="
//                + tokensAnimating);

        animating = tokensAnimating;

        Boolean tokenMayBeDrawn = FALSE;
        Boolean wallpaperMayChange = FALSE;
        Boolean forceHiding = FALSE;

        mPolicy->BeginAnimationLw(dw, dh);

        List<AutoPtr<WindowState> >::ReverseIterator wsRit;
        for (wsRit = mWindows.RBegin(); wsRit != mWindows.REnd(); ++wsRit) {
            AutoPtr<WindowState> w = *wsRit;

            IWindowManagerLayoutParams* attrs = w->mAttrs;

            if (w->mSurface != NULL) {
                // Execute animation.
                if (w->CommitFinishDrawingLocked(currentTime)) {
                    Int32 wFlags;
                    w->mAttrs->GetFlags(&wFlags);
                    if ((wFlags
                            & WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER) != 0) {
//                      if (DEBUG_WALLPAPER) Slog.v(TAG,
//                                "First draw done in potential wallpaper target " + w);
                        wallpaperMayChange = TRUE;
                    }
                }

                Boolean wasAnimating = w->mAnimating;
                if (w->StepAnimationLocked(currentTime, dw, dh)) {
                    animating = TRUE;
                    //w.dump("  ");
                }
                if (wasAnimating && !w->mAnimating && mWallpaperTarget == w) {
                    wallpaperMayChange = TRUE;
                }

                Boolean forceHide, beForceHidden;
                if (mPolicy->DoesForceHide(w, attrs, &forceHide), forceHide) {
                    if (!wasAnimating && animating) {
//                        if (DEBUG_VISIBILITY) Slog.v(TAG,
//                                "Animation done that could impact force hide: "
//                                + w);
                        wallpaperForceHidingChanged = TRUE;
                        mFocusMayChange = TRUE;
                    }
                    else if (w->IsReadyForDisplay() && w->mAnimation == NULL) {
                        forceHiding = TRUE;
                    }
                }
                else if (mPolicy->CanBeForceHidden(w, attrs, &beForceHidden), beForceHidden) {
                    Boolean changed;
                    if (forceHiding) {
                        changed = w->HideLw(FALSE, FALSE);
//                        if (DEBUG_VISIBILITY && changed) Slog.v(TAG,
//                                "Now policy hidden: " + w);
                    }
                    else {
                        changed = w->ShowLw(FALSE, FALSE);
//                        if (DEBUG_VISIBILITY && changed) Slog.v(TAG,
//                                "Now policy shown: " + w);
                        if (changed) {
                            if (wallpaperForceHidingChanged
                                    && w->IsVisibleNow() /*w.isReadyForDisplay()*/) {
                                // Assume we will need to animate.  If
                                // we don't (because the wallpaper will
                                // stay with the lock screen), then we will
                                // clean up later.
//                                Animation a = mPolicy.createForceHideEnterAnimation();
//                                if (a != NULL) {
//                                    w.setAnimation(a);
//                                }
                            }
                            if (mCurrentFocus == NULL || mCurrentFocus->mLayer < w->mLayer) {
                                  // We are showing on to of the current
                                  // focus, so re-evaluate focus to make
                                  // sure it is correct.
                                  mFocusMayChange = TRUE;
                            }
                        }
                    }
                    Int32 attrsFlags;
                    attrs->GetFlags(&attrsFlags);
                    if (changed && (attrsFlags
                            & WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER) != 0) {
                        wallpaperMayChange = TRUE;
                    }
                }

                mPolicy->AnimatingWindowLw(w, attrs);
            }

            AppWindowToken* atoken = w->mAppToken;
            if (atoken != NULL && (!atoken->mAllDrawn || atoken->mFreezingScreen)) {
                if (atoken->mLastTransactionSequence != transactionSequence) {
                    atoken->mLastTransactionSequence = transactionSequence;
                    atoken->mNumInterestingWindows = atoken->mNumDrawnWindows = 0;
                    atoken->mStartingDisplayed = FALSE;
                }
                Int32 wType;
                if ((w->IsOnScreen() || (w->mAttrs->GetType(&wType), wType
                        == WindowManagerLayoutParams_TYPE_BASE_APPLICATION))
                        && !w->mExiting && !w->mDestroying) {
//                        if (DEBUG_VISIBILITY || DEBUG_ORIENTATION) {
//                            Slog.v(TAG, "Eval win " + w + ": isDrawn="
//                                    + w.isDrawnLw()
//                                    + ", isAnimating=" + w.isAnimating());
//                            if (!w->IsDrawnLw()) {
//                           Slog.v(TAG, "Not displayed: s=" + w->mSurface
//                                    + " pv=" + w->mPolicyVisibility
//                                    + " dp=" + w->mDrawPending
//                                    + " cdp=" + w->mCommitDrawPending
//                                    + " ah=" + w->mAttachedHidden
//                                    + " th=" + atoken.hiddenRequested
//                                    + " a=" + w->mAnimating);
//                            }
//                        }
                    if (w != atoken->mStartingWindow) {
                        if (!atoken->mFreezingScreen || !w->mAppFreezing) {
                            atoken->mNumInterestingWindows++;
                            if (w->IsDrawnLw()) {
                                atoken->mNumDrawnWindows++;
//                                if (DEBUG_VISIBILITY || DEBUG_ORIENTATION) Slog.v(TAG,
//                                        "tokenMayBeDrawn: " + atoken
//                                        + " freezingScreen=" + atoken.freezingScreen
//                                        + " mAppFreezing=" + w->mAppFreezing);
                                tokenMayBeDrawn = TRUE;
                            }
                        }
                    }
                    else if (w->IsDrawnLw()) {
                        atoken->mStartingDisplayed = TRUE;
                    }
                }
            }
            else if (w->mReadyToShow) {
                w->PerformShowLocked();
            }
        }

        Int32 bitSet;
        mPolicy->FinishAnimationLw(&bitSet);
        changes |= bitSet;

        if (tokenMayBeDrawn) {
            // See if any windows have been drawn, so they (and others
            // associated with them) can now be shown.
            List<WindowToken*>::Iterator wtIt;
            for (wtIt = mTokenList.Begin(); wtIt != mTokenList.End(); ++wtIt) {
                AppWindowToken* wtoken = (*wtIt)->mAppWindowToken;
                if (wtoken == NULL) {
                    continue;
                }
                if (wtoken->mFreezingScreen) {
                    Int32 numInteresting = wtoken->mNumInterestingWindows;
                    if (numInteresting > 0 && wtoken->mNumDrawnWindows >= numInteresting) {
//                        if (DEBUG_VISIBILITY) Slog.v(TAG,
//                                "allDrawn: " + wtoken
//                                + " interesting=" + numInteresting
//                                + " drawn=" + wtoken.numDrawnWindows);
                        wtoken->ShowAllWindowsLocked();
                        UnsetAppFreezingScreenLocked(wtoken, FALSE, TRUE);
                        orientationChangeComplete = TRUE;
                    }
                }
                else if (!wtoken->mAllDrawn) {
                    Int32 numInteresting = wtoken->mNumInterestingWindows;
                    if (numInteresting > 0 && wtoken->mNumDrawnWindows >= numInteresting) {
//                        if (DEBUG_VISIBILITY) Slog.v(TAG,
//                                "allDrawn: " + wtoken
//                                + " interesting=" + numInteresting
//                                + " drawn=" + wtoken.numDrawnWindows);
                        wtoken->mAllDrawn = TRUE;
                        changes |= WindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM;

                        // We can now show all of the drawn windows!
                        if (Find(mOpeningApps.Begin(), mOpeningApps.End(), wtoken)
                                == mOpeningApps.End()) {
                            wtoken->ShowAllWindowsLocked();
                        }
                    }
                }
            }
        }

        // If we are ready to perform an app transition, check through
        // all of the app tokens to be shown and see if they are ready
        // to go.
        if (mAppTransitionReady) {
            Boolean goodToGo = TRUE;
//            if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                    "Checking " + NN + " opening apps (frozen="
//                    + mDisplayFrozen + " timeout="
//                    + mAppTransitionTimeout + ")...");
            if (!mDisplayFrozen && !mAppTransitionTimeout) {
                // If the display isn't frozen, wait to do anything until
                // all of the apps are ready.  Otherwise just go because
                // we'll unfreeze the display when everyone is ready.
                List<AppWindowToken*>::Iterator it;
                for (it = mOpeningApps.Begin(); it != mOpeningApps.End() && goodToGo; ++it) {
                    AppWindowToken* wtoken = *it;
//                    if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                            "Check opening app" + wtoken + ": allDrawn="
//                            + wtoken.allDrawn + " startingDisplayed="
//                            + wtoken.startingDisplayed);
                    if (!wtoken->mAllDrawn && !wtoken->mStartingDisplayed
                            && !wtoken->mStartingMoved) {
                        goodToGo = FALSE;
                    }
                }
            }
            if (goodToGo) {
//                if (DEBUG_APP_TRANSITIONS) Slog.v(TAG, "**** GOOD TO GO");
                Int32 transit = mNextAppTransition;
                if (mSkipAppTransitionAnimation) {
                    transit = WindowManagerPolicy::TRANSIT_UNSET;
                }
                mNextAppTransition = WindowManagerPolicy::TRANSIT_UNSET;
                mAppTransitionReady = FALSE;
                mAppTransitionRunning = TRUE;
                mAppTransitionTimeout = FALSE;
                mStartingIconInTransition = FALSE;
                mSkipAppTransitionAnimation = FALSE;

                ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
                pHandlerFunc = &CWindowManagerService::HandleAppTransitionTimeout;

                RemoveMessage(*(Handle32*)&pHandlerFunc);

                // If there are applications waiting to come to the
                // top of the stack, now is the time to move their windows.
                // (Note that we don't do apps going to the bottom
                // here -- we want to keep their windows in the old
                // Z-order until the animation completes.)
                if (mToTopApps.Begin() != mToTopApps.End()) {
                    List<AppWindowToken*>::Iterator it;
                    for (it = mAppTokens.Begin(); it != mAppTokens.End(); ++it) {
                        AppWindowToken* wtoken = *it;
                        if (wtoken->mSendingToTop) {
                            wtoken->mSendingToTop = FALSE;
                            MoveAppWindowsLocked(wtoken, mAppTokens.End(), FALSE);
                        }
                    }
                    mToTopApps.Clear();
                }

                AutoPtr<WindowState> oldWallpaper = mWallpaperTarget;

                AdjustWallpaperWindowsLocked();
                wallpaperMayChange = FALSE;

                // The top-most window will supply the layout params,
                // and we will determine it below.
                AutoPtr<IWindowManagerLayoutParams> animLp;
                AppWindowToken* animToken = NULL;
                Int32 bestAnimLayer = -1;

//                if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                        "New wallpaper target=" + mWallpaperTarget
//                        + ", lower target=" + mLowerWallpaperTarget
//                        + ", upper target=" + mUpperWallpaperTarget);
                Int32 foundWallpapers = 0;
                // Do a first pass through the tokens for two
                // things:
                // (1) Determine if both the closing and opening
                // app token sets are wallpaper targets, in which
                // case special animations are needed
                // (since the wallpaper needs to stay static
                // behind them).
                // (2) Find the layout params of the top-most
                // application window in the tokens, which is
                // what will control the animation theme.
                List<AppWindowToken*>::Iterator it;
                for (it = mClosingApps.Begin(); it != mClosingApps.End(); ++it) {
                    AppWindowToken* wtoken = *it;
                    Int32 mode = 1;
                    if (mLowerWallpaperTarget != NULL) {
                        if (mLowerWallpaperTarget->mAppToken == wtoken
                                || mUpperWallpaperTarget->mAppToken == wtoken) {
                            foundWallpapers |= mode;
                        }
                    }
                    if (wtoken->mAppFullscreen) {
                        AutoPtr<WindowState> ws = wtoken->FindMainWindow();
                        if (ws != NULL) {
                            // If this is a compatibility mode
                            // window, we will always use its anim.
                            Int32 wsFlags;
                            ws->mAttrs->GetFlags(&wsFlags);
                            if ((wsFlags & WindowManagerLayoutParams_FLAG_COMPATIBLE_WINDOW) != 0) {
                                animLp = ws->mAttrs;
                                animToken = ws->mAppToken;
                                bestAnimLayer = Math::INT32_MAX_VALUE;
                            }
                            else if (ws->mLayer > bestAnimLayer) {
                                animLp = ws->mAttrs;
                                animToken = ws->mAppToken;
                                bestAnimLayer = ws->mLayer;
                            }
                        }
                    }
                }
                for (it = mOpeningApps.Begin(); it != mOpeningApps.End(); ++it) {
                    AppWindowToken* wtoken = *it;
                    Int32 mode = 2;
                    if (mLowerWallpaperTarget != NULL) {
                        if (mLowerWallpaperTarget->mAppToken == wtoken
                                || mUpperWallpaperTarget->mAppToken == wtoken) {
                            foundWallpapers |= mode;
                        }
                    }
                    if (wtoken->mAppFullscreen) {
                        AutoPtr<WindowState> ws = wtoken->FindMainWindow();
                        if (ws != NULL) {
                            // If this is a compatibility mode
                            // window, we will always use its anim.
                            Int32 wsFlags;
                            ws->mAttrs->GetFlags(&wsFlags);
                            if ((wsFlags & WindowManagerLayoutParams_FLAG_COMPATIBLE_WINDOW) != 0) {
                                animLp = ws->mAttrs;
                                animToken = ws->mAppToken;
                                bestAnimLayer = Math::INT32_MAX_VALUE;
                            }
                            else if (ws->mLayer > bestAnimLayer) {
                                animLp = ws->mAttrs;
                                animToken = ws->mAppToken;
                                bestAnimLayer = ws->mLayer;
                            }
                        }
                    }
                }

                if (foundWallpapers == 3) {
//                    if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                            "Wallpaper animation!");
                    switch (transit) {
                        case WindowManagerPolicy_TRANSIT_ACTIVITY_OPEN:
                        case WindowManagerPolicy_TRANSIT_TASK_OPEN:
                        case WindowManagerPolicy_TRANSIT_TASK_TO_FRONT:
                            transit = WindowManagerPolicy_TRANSIT_WALLPAPER_INTRA_OPEN;
                            break;
                        case WindowManagerPolicy_TRANSIT_ACTIVITY_CLOSE:
                        case WindowManagerPolicy_TRANSIT_TASK_CLOSE:
                        case WindowManagerPolicy_TRANSIT_TASK_TO_BACK:
                            transit = WindowManagerPolicy_TRANSIT_WALLPAPER_INTRA_CLOSE;
                            break;
                    }
//                    if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                            "New transit: " + transit);
                }
                else if (oldWallpaper != NULL) {
                    // We are transitioning from an activity with
                    // a wallpaper to one without.
                    transit = WindowManagerPolicy_TRANSIT_WALLPAPER_CLOSE;
//                    if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                            "New transit away from wallpaper: " + transit);
                }
                else if (mWallpaperTarget != NULL) {
                    // We are transitioning from an activity without
                    // a wallpaper to now showing the wallpaper
                    transit = WindowManagerPolicy_TRANSIT_WALLPAPER_OPEN;
//                    if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                            "New transit into wallpaper: " + transit);
                }

                if ((transit & WindowManagerPolicy_TRANSIT_ENTER_MASK) != 0) {
                    mLastEnterAnimToken = animToken;
                    mLastEnterAnimParams = animLp;
                }
                else if (mLastEnterAnimParams != NULL) {
                    animLp = mLastEnterAnimParams;
                    mLastEnterAnimToken = NULL;
                    mLastEnterAnimParams = NULL;
                }

                // If all closing windows are obscured, then there is
                // no need to do an animation.  This is the case, for
                // example, when this transition is being done behind
                // the lock screen.
                Boolean allowAnimation;
                mPolicy->AllowAppAnimationsLw(&allowAnimation);
                if (!allowAnimation) {
                    animLp = NULL;
                }

                for (it = mOpeningApps.Begin(); it != mOpeningApps.End(); ++it) {
                    AppWindowToken* wtoken = *it;
//                    if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                            "Now opening app" + wtoken);
                    wtoken->mReportedVisible = FALSE;
                    wtoken->mInPendingTransaction = FALSE;
                    wtoken->mAnimation = NULL;
                    SetTokenVisibilityLocked(wtoken, animLp, TRUE, transit, FALSE);
                    wtoken->UpdateReportedVisibilityLocked();
                    wtoken->mWaitingToShow = FALSE;
                    wtoken->ShowAllWindowsLocked();
                }
                for (it = mClosingApps.Begin(); it != mClosingApps.End(); ++it) {
                    AppWindowToken* wtoken = *it;
//                    if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
//                            "Now closing app" + wtoken);
                    wtoken->mInPendingTransaction = FALSE;
                    wtoken->mAnimation = NULL;
                    SetTokenVisibilityLocked(wtoken, animLp, FALSE, transit, FALSE);
                    wtoken->UpdateReportedVisibilityLocked();
                    wtoken->mWaitingToHide = FALSE;
                    // Force the allDrawn flag, because we want to start
                    // this guy's animations regardless of whether it's
                    // gotten drawn.
                    wtoken->mAllDrawn = TRUE;
                }

                mNextAppTransitionPackage = NULL;

                mOpeningApps.Clear();
                mClosingApps.Clear();

                // This has changed the visibility of windows, so perform
                // a new layout to get them all up-to-date.
                changes |= WindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
                mLayoutNeeded = TRUE;
                if (!MoveInputMethodWindowsIfNeededLocked(TRUE)) {
                    AssignLayersLocked();
                }
                UpdateFocusedWindowLocked(UPDATE_FOCUS_PLACING_SURFACES);
                mFocusMayChange = FALSE;
            }
        }

        Int32 adjResult = 0;

        if (!animating && mAppTransitionRunning) {
            // We have finished the animation of an app transition.  To do
            // this, we have delayed a lot of operations like showing and
            // hiding apps, moving apps in Z-order, etc.  The app token list
            // reflects the correct Z-order, but the window list may now
            // be out of sync with it.  So here we will just rebuild the
            // entire app window list.  Fun!
            mAppTransitionRunning = FALSE;
            // Clear information about apps that were moving.
            mToBottomApps.Clear();

            RebuildAppWindowListLocked();
            changes |= WindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
            adjResult |= ADJUST_WALLPAPER_LAYERS_CHANGED;
            MoveInputMethodWindowsIfNeededLocked(FALSE);
            wallpaperMayChange = TRUE;
            // Since the window list has been rebuilt, focus might
            // have to be recomputed since the actual order of windows
            // might have changed again.
            mFocusMayChange = TRUE;
        }

        if (wallpaperForceHidingChanged && changes == 0 && !mAppTransitionReady) {
            // At this point, there was a window with a wallpaper that
            // was force hiding other windows behind it, but now it
            // is going away.  This may be simple -- just animate
            // away the wallpaper and its window -- or it may be
            // hard -- the wallpaper now needs to be shown behind
            // something that was hidden.
//            AutoPtr<WindowState> oldWallpaper = mWallpaperTarget;
            if (mLowerWallpaperTarget != NULL
                    && mLowerWallpaperTarget->mAppToken != NULL) {
//                if (DEBUG_WALLPAPER) Slog.v(TAG,
//                        "wallpaperForceHiding changed with lower="
//                        + mLowerWallpaperTarget);
//                if (DEBUG_WALLPAPER) Slog.v(TAG,
//                        "hidden=" + mLowerWallpaperTarget.mAppToken.hidden +
//                        " hiddenRequested=" + mLowerWallpaperTarget.mAppToken.hiddenRequested);
                if (mLowerWallpaperTarget->mAppToken->mHidden) {
                    // The lower target has become hidden before we
                    // actually started the animation...  let's completely
                    // re-evaluate everything.
                    mLowerWallpaperTarget = mUpperWallpaperTarget = NULL;
                    changes |= WindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM;
                }
            }
            adjResult |= AdjustWallpaperWindowsLocked();
            wallpaperMayChange = FALSE;
            wallpaperForceHidingChanged = FALSE;
//            if (DEBUG_WALLPAPER) Slog.v(TAG, "****** OLD: " + oldWallpaper
//                    + " NEW: " + mWallpaperTarget
//                    + " LOWER: " + mLowerWallpaperTarget);
            if (mLowerWallpaperTarget == NULL) {
                // Whoops, we don't need a special wallpaper animation.
                // Clear them out.
                forceHiding = FALSE;
                List<AutoPtr<WindowState> >::ReverseIterator wsRit;
                for (wsRit = mWindows.RBegin(); wsRit != mWindows.REnd(); ++wsRit) {
                    AutoPtr<WindowState> w = *wsRit;
                    if (w->mSurface != NULL) {
                        IWindowManagerLayoutParams* attrs = w->mAttrs;
                        Boolean forceHide, beForceHidden;
                        mPolicy->DoesForceHide(w, attrs, &forceHide);
                        if (forceHide && w->IsVisibleLw()) {
//                            if (DEBUG_FOCUS) Slog.i(TAG, "win=" + w + " force hides other windows");
                            forceHiding = TRUE;
                        }
                        else if (mPolicy->CanBeForceHidden(w, attrs, &beForceHidden), beForceHidden) {
                            if (!w->mAnimating) {
                                // We set the animation above so it
                                // is not yet running.
                                w->ClearAnimation();
                            }
                        }
                    }
                }
            }
        }

        if (wallpaperMayChange) {
//            if (DEBUG_WALLPAPER) Slog.v(TAG,
//                    "Wallpaper may change!  Adjusting");
            adjResult |= AdjustWallpaperWindowsLocked();
        }

        if ((adjResult & ADJUST_WALLPAPER_LAYERS_CHANGED) != 0) {
//            if (DEBUG_WALLPAPER) Slog.v(TAG,
//                    "Wallpaper layer changed: assigning layers + relayout");
            changes |= WindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
            AssignLayersLocked();
        }
        else if ((adjResult & ADJUST_WALLPAPER_VISIBILITY_CHANGED) != 0) {
//            if (DEBUG_WALLPAPER) Slog.v(TAG,
//                    "Wallpaper visibility changed: relayout");
            changes |= WindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
        }

        if (mFocusMayChange) {
            mFocusMayChange = FALSE;
            if (UpdateFocusedWindowLocked(UPDATE_FOCUS_PLACING_SURFACES)) {
                changes |= WindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM;
                adjResult = 0;
            }
        }

        if (mLayoutNeeded) {
            changes |= WindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
        }

//        if (DEBUG_APP_TRANSITIONS) Slog.v(TAG, "*** ANIM STEP: changes=0x"
//                + Integer.toHexString(changes));

        mInputMonitor->UpdateInputWindowsLw();
    } while (changes != 0);

    // THIRD LOOP: Update the surfaces of all windows.
    ECode ec;

    Boolean someoneLosingFocus = (mLosingFocus.Begin() != mLosingFocus.End());

    Boolean obscured = FALSE;
    Boolean blurring = FALSE;
    Boolean dimming = FALSE;
    Boolean covered = FALSE;
    Boolean syswin = FALSE;
    Boolean backgroundFillerShown = FALSE;

    List<AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = mWindows.RBegin(); rit != mWindows.REnd(); ++rit) {
        AutoPtr<WindowState> w = *rit;

        Boolean displayed = FALSE;
        IWindowManagerLayoutParams* attrs = w->mAttrs;
        Int32 attrFlags;
        attrs->GetFlags(&attrFlags);

        if (w->mSurface != NULL) {
            // XXX NOTE: The logic here could be improved.  We have
            // the decision about whether to resize a window separated
            // from whether to hide the surface.  This can cause us to
            // resize a surface even if we are going to hide it.  You
            // can see this by (1) holding device in landscape mode on
            // home screen; (2) tapping browser icon (device will rotate
            // to landscape; (3) tap home.  The wallpaper will be resized
            // in step 2 but then immediately hidden, causing us to
            // have to resize and then redraw it again in step 3.  It
            // would be nice to figure out how to avoid this, but it is
            // difficult because we do need to resize surfaces in some
            // cases while they are hidden such as when first showing a
            // window.

            w->ComputeShownFrameLocked();
//            if (localLOGV) Slog.v(
//                    TAG, "Placing surface #" + i + " " + w->mSurface
//                    + ": new=" + w->mShownFrame + ", old="
//                    + w->mLastShownFrame);

            Boolean resize;
            Int32 wFlags, width, height;
            w->mAttrs->GetFlags(&wFlags);
            if ((wFlags & WindowManagerLayoutParams_FLAG_SCALED) != 0) {
                resize = (w->mLastRequestedWidth != w->mRequestedWidth) ||
                        (w->mLastRequestedHeight != w->mRequestedHeight);
                // for a scaled surface, we just want to use
                // the requested size.
                width  = w->mRequestedWidth;
                height = w->mRequestedHeight;
                w->mLastRequestedWidth = width;
                w->mLastRequestedHeight = height;
                w->mLastShownFrame->SetEx(w->mShownFrame);
//                try {
//                if (SHOW_TRANSACTIONS) logSurface(w,
//                        "POS " + w->mShownFrame.left
//                        + ", " + w->mShownFrame.top, NULL);
                w->mShownFrame->GetLeft(&w->mSurfaceX);
                w->mShownFrame->GetTop(&w->mSurfaceY);
                ec = w->mSurface->SetPosition(w->mSurfaceX, w->mSurfaceY);
                if (FAILED(ec)) {
//                    Slog.w(TAG, "Error positioning surface in " + w, e);
                    if (!recoveringMemory) {
                        ReclaimSomeSurfaceMemoryLocked(w, "position");
                    }
                }
//                } catch (RuntimeException e) {
//                    Slog.w(TAG, "Error positioning surface in " + w, e);
//                    if (!recoveringMemory) {
//                        reclaimSomeSurfaceMemoryLocked(w, "position");
//                    }
//                }
            }
            else {
                w->mLastShownFrame->Equals(w->mShownFrame, &resize);
                resize = !resize;
                w->mShownFrame->GetWidth(&width);
                w->mShownFrame->GetHeight(&height);
                w->mLastShownFrame->SetEx(w->mShownFrame);
            }

            if (resize) {
                if (width < 1) width = 1;
                if (height < 1) height = 1;
                if (w->mSurface != NULL) {
//                    try {
//                        if (SHOW_TRANSACTIONS) logSurface(w,
//                                "POS " + w->mShownFrame.left + ","
//                                + w->mShownFrame.top + " SIZE "
//                                + w->mShownFrame.width() + "x"
//                                + w->mShownFrame.height(), NULL);
                    w->mSurfaceResized = TRUE;
                    w->mSurfaceW = width;
                    w->mSurfaceH = height;
                    ec = w->mSurface->SetSize(width, height);
                    if (SUCCEEDED(ec)) {
                        w->mShownFrame->GetLeft(&w->mSurfaceX);
                        w->mShownFrame->GetTop(&w->mSurfaceY);
                        ec = w->mSurface->SetPosition(w->mSurfaceX, w->mSurfaceY);
                    }
                    if (FAILED(ec)) {
//                        Slog.e(TAG, "Failure updating surface of " + w
//                            + "size=(" + width + "x" + height
//                            + ")", e);
                        if (!recoveringMemory) {
                            ReclaimSomeSurfaceMemoryLocked(w, "size");
                        }
                    }
//                } catch (RuntimeException e) {
                    // If something goes wrong with the surface (such
                    // as running out of memory), don't take down the
                    // entire system.
//                    Slog.e(TAG, "Failure updating surface of " + w
//                            + "size=(" + width + "x" + height
//                            + "), pos=(" + w->mShownFrame.left
//                            + "," + w->mShownFrame.top + ")", e);
//                    if (!recoveringMemory) {
//                        ReclaimSomeSurfaceMemoryLocked(w, "size");
//                    }
//                }
                }
            }
            if (!w->mAppFreezing && w->mLayoutSeq == mLayoutSeq) {
                Boolean equals;
                w->mLastContentInsets->Equals(w->mContentInsets, &equals);
                w->mContentInsetsChanged = !equals;
                w->mLastVisibleInsets->Equals(w->mVisibleInsets, &equals);
                w->mVisibleInsetsChanged = !equals;
                Int32 diff;
                Boolean configChanged =
                    w->mConfiguration != mCurConfiguration
                    && (w->mConfiguration == NULL
                            || (mCurConfiguration->Diff(w->mConfiguration, &diff), diff != 0));
//                if (DEBUG_CONFIGURATION && configChanged) {
//                    Slog.v(TAG, "Win " + w + " config changed: "
//                            + mCurConfiguration);
//                }
//                if (localLOGV) Slog.v(TAG, "Resizing " + w
//                        + ": configChanged=" + configChanged
//                        + " last=" + w->mLastFrame + " frame=" + w->mFrame);
                w->mLastFrame->Equals(w->mFrame, &equals);
                if (!equals || w->mContentInsetsChanged
                        || w->mVisibleInsetsChanged
                        || w->mSurfaceResized
                        || configChanged) {
                    w->mLastFrame->SetEx(w->mFrame);
                    w->mLastContentInsets->SetEx(w->mContentInsets);
                    w->mLastVisibleInsets->SetEx(w->mVisibleInsets);
                    // If the screen is currently frozen, then keep
                    // it frozen until this window draws at its new
                    // orientation.
                    if (mDisplayFrozen) {
//                        if (DEBUG_ORIENTATION) Slog.v(TAG,
//                                "Resizing while display frozen: " + w);
                        w->mOrientationChanging = TRUE;
                        if (!mWindowsFreezingScreen) {
                            mWindowsFreezingScreen = TRUE;
                            // XXX should probably keep timeout from
                            // when we first froze the display.
                            ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
                            pHandlerFunc = &CWindowManagerService::HandleWindowFreezeTimeout;

                            RemoveMessage(*(Handle32*)&pHandlerFunc);
                            SendMessageDelayed(*(Handle32*)&pHandlerFunc, NULL, 2000);
                        }
                    }
                    // If the orientation is changing, then we need to
                    // hold off on unfreezing the display until this
                    // window has been redrawn; to do that, we need
                    // to go through the process of getting informed
                    // by the application when it has finished drawing.
                    if (w->mOrientationChanging) {
//                        if (DEBUG_ORIENTATION) Slog.v(TAG,
//                                "Orientation start waiting for draw in "
//                                + w + ", surface " + w->mSurface);
                        w->mDrawPending = TRUE;
                        w->mCommitDrawPending = FALSE;
                        w->mReadyToShow = FALSE;
                        if (w->mAppToken != NULL) {
                            w->mAppToken->mAllDrawn = FALSE;
                        }
                    }
//                    if (DEBUG_RESIZE || DEBUG_ORIENTATION) Slog.v(TAG,
//                            "Resizing window " + w + " to " + w->mFrame);
                    mResizingWindows.PushBack(w);
                }
                else if (w->mOrientationChanging) {
                    if (!w->mDrawPending && !w->mCommitDrawPending) {
//                        if (DEBUG_ORIENTATION) Slog.v(TAG,
//                                "Orientation not waiting for draw in "
//                                + w + ", surface " + w->mSurface);
                        w->mOrientationChanging = FALSE;
                    }
                }
            }

            if (w->mAttachedHidden || !w->IsReadyForDisplay()) {
                if (!w->mLastHidden) {
                    //dump();
//                    if (DEBUG_CONFIGURATION) Slog.v(TAG, "Window hiding: waitingToShow="
//                            + w->mRootToken.waitingToShow + " polvis="
//                            + w->mPolicyVisibility + " atthid="
//                            + w->mAttachedHidden + " tokhid="
//                            + w->mRootToken.hidden + " vis="
//                            + w->mViewVisibility);
                    w->mLastHidden = TRUE;
//                    if (SHOW_TRANSACTIONS) logSurface(w,
//                            "HIDE (performLayout)", NULL);
                    if (w->mSurface != NULL) {
                        w->mSurfaceShown = FALSE;
//                        try {
                        ec = w->mSurface->Hide();
                        if (FAILED(ec)) {
//                            Slog.w(TAG, "Exception hiding surface in " + w);
                        }
//                    } catch (RuntimeException e) {
//                        Slog.w(TAG, "Exception hiding surface in " + w);
//                    }
                    }
                }
                // If we are waiting for this window to handle an
                // orientation change, well, it is hidden, so
                // doesn't really matter.  Note that this does
                // introduce a potential glitch if the window
                // becomes unhidden before it has drawn for the
                // new orientation.
                if (w->mOrientationChanging) {
                    w->mOrientationChanging = FALSE;
//                    if (DEBUG_ORIENTATION) Slog.v(TAG,
//                            "Orientation change skips hidden " + w);
                }
            }
            else if (w->mLastLayer != w->mAnimLayer
                    || w->mLastAlpha != w->mShownAlpha
                    || w->mLastDsDx != w->mDsDx
                    || w->mLastDtDx != w->mDtDx
                    || w->mLastDsDy != w->mDsDy
                    || w->mLastDtDy != w->mDtDy
                    || w->mLastHScale != w->mHScale
                    || w->mLastVScale != w->mVScale
                    || w->mLastHidden) {
                displayed = TRUE;
                w->mLastAlpha = w->mShownAlpha;
                w->mLastLayer = w->mAnimLayer;
                w->mLastDsDx = w->mDsDx;
                w->mLastDtDx = w->mDtDx;
                w->mLastDsDy = w->mDsDy;
                w->mLastDtDy = w->mDtDy;
                w->mLastHScale = w->mHScale;
                w->mLastVScale = w->mVScale;
//                if (SHOW_TRANSACTIONS) logSurface(w,
//                        "alpha=" + w->mShownAlpha + " layer=" + w->mAnimLayer
//                        + " matrix=[" + (w->mDsDx*w->mHScale)
//                        + "," + (w->mDtDx*w->mVScale)
//                        + "][" + (w->mDsDy*w->mHScale)
//                        + "," + (w->mDtDy*w->mVScale) + "]", NULL);
                if (w->mSurface != NULL) {
//                    try {
                    w->mSurfaceAlpha = w->mShownAlpha;
                    ec = w->mSurface->SetAlpha(w->mShownAlpha);
                    if (SUCCEEDED(ec)) {
                        w->mSurfaceLayer = w->mAnimLayer;
                        ec = w->mSurface->SetLayer(w->mAnimLayer);
                        if (SUCCEEDED(ec)) {
                            ec = w->mSurface->SetMatrix(
                                    w->mDsDx * w->mHScale, w->mDtDx * w->mVScale,
                                    w->mDsDy * w->mHScale, w->mDtDy * w->mVScale);
                        }
                    }
                    if (FAILED(ec)) {
//                        Slog.w(TAG, "Error updating surface in " + w, e);
                        if (!recoveringMemory) {
                            ReclaimSomeSurfaceMemoryLocked(w, "update");
                        }
                    }
//                } catch (RuntimeException e) {
//                    Slog.w(TAG, "Error updating surface in " + w, e);
//                    if (!recoveringMemory) {
//                        reclaimSomeSurfaceMemoryLocked(w, "update");
//                    }
//                }
                }

                if (w->mLastHidden && !w->mDrawPending
                        && !w->mCommitDrawPending
                        && !w->mReadyToShow) {
//                    if (SHOW_TRANSACTIONS) logSurface(w,
//                            "SHOW (performLayout)", NULL);
//                    if (DEBUG_VISIBILITY) Slog.v(TAG, "Showing " + w
//                            + " during relayout");
                    if (ShowSurfaceRobustlyLocked(w)) {
                        w->mHasDrawn = TRUE;
                        w->mLastHidden = FALSE;
                    } else {
                        w->mOrientationChanging = FALSE;
                    }
                }
                if (w->mSurface != NULL) {
                    w->mToken->mHasVisible = TRUE;
                }
            }
            else {
                displayed = TRUE;
            }

            if (displayed) {
                if (!covered) {
                    Int32 width, height;
                    attrs->GetWidth(&width);
                    attrs->GetHeight(&height);
                    if (width == ViewGroupLayoutParams_MATCH_PARENT
                            && height == ViewGroupLayoutParams_MATCH_PARENT) {
                        covered = TRUE;
                    }
                }
                if (w->mOrientationChanging) {
                    if (w->mDrawPending || w->mCommitDrawPending) {
                        orientationChangeComplete = FALSE;
//                        if (DEBUG_ORIENTATION) Slog.v(TAG,
//                                "Orientation continue waiting for draw in " + w);
                    }
                    else {
                        w->mOrientationChanging = FALSE;
//                            if (DEBUG_ORIENTATION) Slog.v(TAG,
//                                    "Orientation change complete in " + w);
                    }
                }
                w->mToken->mHasVisible = TRUE;
            }
        }
        else if (w->mOrientationChanging) {
//            if (DEBUG_ORIENTATION) Slog.v(TAG,
//                    "Orientation change skips hidden " + w);
            w->mOrientationChanging = FALSE;
        }

        Boolean canBeSeen = w->IsDisplayedLw();

        if (someoneLosingFocus && w == mCurrentFocus && canBeSeen) {
            focusDisplayed = TRUE;
        }

        Boolean obscuredChanged = w->mObscured != obscured;

        // Update effect.
        if (!(w->mObscured = obscured)) {
            if (w->mSurface != NULL) {
                if ((attrFlags & WindowManagerLayoutParams_FLAG_KEEP_SCREEN_ON) != 0) {
                    holdScreen = w->mSession;
                }
                Float wScreenBrightness, wButtonBrightness;
                w->mAttrs->GetScreenBrightness(&wScreenBrightness);
                w->mAttrs->GetButtonBrightness(&wButtonBrightness);
                if (!syswin && wScreenBrightness >= 0
                        && screenBrightness < 0) {
                    screenBrightness = wScreenBrightness;
                }
                if (!syswin && wButtonBrightness >= 0
                        && buttonBrightness < 0) {
                    buttonBrightness = wButtonBrightness;
                }
                Int32 attrsType;
                attrs->GetType(&attrsType);
                if (canBeSeen
                        && (attrsType == WindowManagerLayoutParams_TYPE_SYSTEM_DIALOG
                         || attrsType == WindowManagerLayoutParams_TYPE_KEYGUARD
                         || attrsType == WindowManagerLayoutParams_TYPE_SYSTEM_ERROR)) {
                    syswin = TRUE;
                }
            }

            Boolean opaqueDrawn = canBeSeen && w->IsOpaqueDrawn();
            if (opaqueDrawn && w->IsFullscreen(dw, dh)) {
                // This window completely covers everything behind it,
                // so we want to leave all of them as unblurred (for
                // performance reasons).
                obscured = TRUE;
            }
            else if (opaqueDrawn && w->NeedsBackgroundFiller(dw, dh)) {
//                if (SHOW_TRANSACTIONS) Slog.d(TAG, "showing background filler");
                // This window is in compatibility mode, and needs background filler.
                obscured = TRUE;
                if (mBackgroundFillerSurface == NULL) {
//                    try {
                    ec = CSurface::New(mFxSession, 0,
                        String("BackGroundFiller"), 0, dw, dh,
                        ElPixelFormat::OPAQUE, Surface_FX_SURFACE_NORMAL,
                       (ISurface**)&mBackgroundFillerSurface);
                    if (FAILED(ec)) {
//                        Slog.e(TAG, "Exception creating filler surface", e);
                    }
//                    } catch (Exception e) {
//                        Slog.e(TAG, "Exception creating filler surface", e);
//                    }
                }
//                try {
                ec = mBackgroundFillerSurface->SetPosition(0, 0);
                if (SUCCEEDED(ec)) {
                    ec = mBackgroundFillerSurface->SetSize(dw, dh);
                    if (SUCCEEDED(ec)) {
                        // Using the same layer as Dim because they will never be shown at the
                        // same time.
                        ec = mBackgroundFillerSurface->SetLayer(w->mAnimLayer - 1);
                        if (SUCCEEDED(ec)) {
                            ec = mBackgroundFillerSurface->Show();
                        }
                    }
                }
                if (FAILED(ec)) {
//                    Slog.e(TAG, "Exception showing filler surface");
                }
//                } catch (RuntimeException e) {
//                    Slog.e(TAG, "Exception showing filler surface");
//                }
                backgroundFillerShown = TRUE;
                mBackgroundFillerShown = TRUE;
            }
            else if (canBeSeen && !obscured &&
                    ((attrFlags & WindowManagerLayoutParams_FLAG_BLUR_BEHIND) |
                        WindowManagerLayoutParams_FLAG_DIM_BEHIND) != 0) {
//                if (localLOGV) Slog.v(TAG, "Win " + w
//                        + ": blurring=" + blurring
//                        + " obscured=" + obscured
//                        + " displayed=" + displayed);
                if ((attrFlags & WindowManagerLayoutParams_FLAG_DIM_BEHIND) != 0) {
                    if (!dimming) {
                        //Slog.i(TAG, "DIM BEHIND: " + w);
                        //TODO:
                        //
                        // dimming = TRUE;
                        // if (mDimAnimator == NULL) {
                        //     mDimAnimator = new DimAnimator(mFxSession);
                        // }
                        // mDimAnimator->Show(dw, dh);
                        // mDimAnimator->UpdateParameters(w, currentTime);
                    }
                }
                if ((attrFlags & WindowManagerLayoutParams_FLAG_BLUR_BEHIND) != 0) {
                    if (!blurring) {
                        //Slog.i(TAG, "BLUR BEHIND: " + w);
                        blurring = TRUE;
                        if (mBlurSurface == NULL) {
//                            if (SHOW_TRANSACTIONS) Slog.i(TAG, "  BLUR "
//                                    + mBlurSurface + ": CREATE");
//                            try {
                            ec = CSurface::New(mFxSession, 0,
                                String("BlurSurface"), -1, 16, 16,
                                ElPixelFormat::OPAQUE,
                                Surface_FX_SURFACE_BLUR,
                                (ISurface**)&mBlurSurface);
//                            } catch (Exception e) {
//                                Slog.e(TAG, "Exception creating Blur surface", e);
//                            }
                        }
                        if (mBlurSurface != NULL) {
//                            if (SHOW_TRANSACTIONS) Slog.i(TAG, "  BLUR "
//                                    + mBlurSurface + ": pos=(0,0) (" +
//                                    dw + "x" + dh + "), layer=" + (w->mAnimLayer-1));
                            mBlurSurface->SetPosition(0, 0);
                            mBlurSurface->SetSize(dw, dh);
                            mBlurSurface->SetLayer(w->mAnimLayer - 2);
                            if (!mBlurShown) {
//                                try {
//                                if (SHOW_TRANSACTIONS) Slog.i(TAG, "  BLUR "
//                                        + mBlurSurface + ": SHOW");
                                mBlurSurface->Show();
//                                } catch (RuntimeException e) {
//                                    Slog.w(TAG, "Failure showing blur surface", e);
//                                }
                                mBlurShown = TRUE;
                            }
                        }
                    }
                }
            }
        }

        if (obscuredChanged && mWallpaperTarget == w) {
            // This is the wallpaper target and its obscured state
            // changed... make sure the current wallaper's visibility
            // has been updated accordingly.
            UpdateWallpaperVisibilityLocked();
        }
    }

    if (backgroundFillerShown == FALSE && mBackgroundFillerShown) {
        mBackgroundFillerShown = FALSE;
//        if (SHOW_TRANSACTIONS) Slog.d(TAG, "hiding background filler");
//        try {
        ec = mBackgroundFillerSurface->Hide();
//        } catch (RuntimeException e) {
//            Slog.e(TAG, "Exception hiding filler surface", e);
//        }
    }

    if (mDimAnimator != NULL && mDimAnimator->mDimShown) {
        Boolean isScreenOn;
        mPolicy->IsScreenOn(&isScreenOn);
        animating |= mDimAnimator->UpdateSurface(dimming, currentTime,
                mDisplayFrozen || !isScreenOn);
    }

    if (!blurring && mBlurShown) {
//        if (SHOW_TRANSACTIONS) Slog.i(TAG, "  BLUR " + mBlurSurface
//                + ": HIDE");
//        try {
        ec = mBlurSurface->Hide();
//        } catch (IllegalArgumentException e) {
//            Slog.w(TAG, "Illegal argument exception hiding blur surface");
//        }
        mBlurShown = FALSE;
    }

//    if (SHOW_TRANSACTIONS) Slog.i(TAG, "<<< CLOSE TRANSACTION");
//    } catch (RuntimeException e) {
//        Slog.e(TAG, "Unhandled exception in Window Manager", e);
//    }

    mInputMonitor->UpdateInputWindowsLw();

    helper->CloseTransaction();

    if (mWatermark != NULL) {
        mWatermark->DrawIfNeeded();
    }

//    if (DEBUG_ORIENTATION && mDisplayFrozen) Slog.v(TAG,
//            "With display frozen, orientationChangeComplete="
//            + orientationChangeComplete);
    if (orientationChangeComplete) {
        if (mWindowsFreezingScreen) {
            mWindowsFreezingScreen = FALSE;
            ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
            pHandlerFunc = &CWindowManagerService::HandleWindowFreezeTimeout;

            RemoveMessage(*(Handle32*)&pHandlerFunc);
        }
        StopFreezingDisplayLocked();
    }

    if (mResizingWindows.Begin() != mResizingWindows.End()) {
        List<AutoPtr<WindowState> >::ReverseIterator rwRit = mResizingWindows.RBegin();
        for (; rwRit != mResizingWindows.REnd(); ++rwRit) {
            AutoPtr<WindowState> win = *rwRit;
//            try {
//            if (DEBUG_RESIZE || DEBUG_ORIENTATION) Slog.v(TAG,
//                    "Reporting new frame to " + win + ": " + win.mFrame);
            Int32 diff = 0;
            Boolean configChanged =
                win->mConfiguration != mCurConfiguration
                && (win->mConfiguration == NULL
                        || (mCurConfiguration->Diff(win->mConfiguration, &diff), diff != 0));
//            if ((DEBUG_RESIZE || DEBUG_ORIENTATION || DEBUG_CONFIGURATION)
//                    && configChanged) {
//                Slog.i(TAG, "Sending new config to window " + win + ": "
//                        + win.mFrame.width() + "x" + win.mFrame.height()
//                        + " / " + mCurConfiguration + " / 0x"
//                        + Integer.toHexString(diff));
//            }
            win->mConfiguration = mCurConfiguration;
            Int32 w, h;
            win->mFrame->GetWidth(&w);
            win->mFrame->GetHeight(&h);
            ec = win->mClient->Resized(w, h, win->mLastContentInsets,
                    win->mLastVisibleInsets, win->mDrawPending,
                    configChanged ? win->mConfiguration : NULL);
            if (FAILED(ec)) {
                win->mOrientationChanging = FALSE;
            } else {
                win->mContentInsetsChanged = FALSE;
                win->mVisibleInsetsChanged = FALSE;
                win->mSurfaceResized = FALSE;
            }
//        } catch (RemoteException e) {
//            win.mOrientationChanging = false;
//        }
        }
        mResizingWindows.Clear();
    }

    // Destroy the surface of any windows that are no longer visible.
    Boolean wallpaperDestroyed = FALSE;
    if (mDestroySurface.Begin() != mDestroySurface.End()) {
        List<AutoPtr<WindowState> >::ReverseIterator dsRit = mDestroySurface.RBegin();
        for (; dsRit != mDestroySurface.REnd(); ++dsRit) {
            AutoPtr<WindowState> win = *dsRit;
            win->mDestroying = FALSE;
            if (mInputMethodWindow == win) {
                mInputMethodWindow = NULL;
            }
            if (win == mWallpaperTarget) {
                wallpaperDestroyed = TRUE;
            }
            win->DestroySurfaceLocked();
        }
        mDestroySurface.Clear();
    }

    // Time to remove any exiting tokens?
    List<WindowToken*>::Iterator etIt = mExitingTokens.Begin();
    while (etIt != mExitingTokens.End()) {
        WindowToken* token = *etIt;
        if (!token->mHasVisible) {
            etIt = mExitingTokens.Erase(etIt);
            if (token->mWindowType == WindowManagerLayoutParams_TYPE_WALLPAPER) {
                mWallpaperTokens.Remove(token);
            }
        }
        else ++etIt;
    }

    // Time to remove any exiting applications?
    List<AppWindowToken*>::Iterator eatIt = mExitingAppTokens.Begin();
    while (eatIt != mExitingAppTokens.End()) {
        AppWindowToken* token = *eatIt;
        Boolean contains = \
            Find(mClosingApps.Begin(), mClosingApps.End(), token) != mClosingApps.End();
        if (!token->mHasVisible && !contains) {
            // Make sure there is no animation running on this token,
            // so any windows associated with it will be removed as
            // soon as their animations are complete
            token->mAnimation = NULL;
            token->mAnimating = FALSE;
            mAppTokens.Remove(token);
            eatIt = mExitingAppTokens.Erase(eatIt);
            if (mLastEnterAnimToken == token) {
                mLastEnterAnimToken = NULL;
                mLastEnterAnimParams = NULL;
            }
        }
        else ++eatIt;
    }

    Boolean needRelayout = FALSE;

    if (!animating && mAppTransitionRunning) {
        // We have finished the animation of an app transition.  To do
        // this, we have delayed a lot of operations like showing and
        // hiding apps, moving apps in Z-order, etc.  The app token list
        // reflects the correct Z-order, but the window list may now
        // be out of sync with it.  So here we will just rebuild the
        // entire app window list.  Fun!
        mAppTransitionRunning = FALSE;
        needRelayout = TRUE;
        RebuildAppWindowListLocked();
        AssignLayersLocked();
        // Clear information about apps that were moving.
        mToBottomApps.Clear();
    }

    if (focusDisplayed) {
        ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
        pHandlerFunc = &CWindowManagerService::HandleReportLosingFocus;

        SendMessage(*(Handle32*)&pHandlerFunc, NULL);
    }

    if (wallpaperDestroyed) {
        needRelayout = AdjustWallpaperWindowsLocked() != 0;
    }
    if (needRelayout) {
        RequestAnimationLocked(0);
    }
    else if (animating) {
        RequestAnimationLocked(currentTime + (1000/60) - SystemClock::GetUptimeMillis());
    }

    mInputMonitor->UpdateInputWindowsLw();

//    if (DEBUG_FREEZE) Slog.v(TAG, "Layout: mDisplayFrozen=" + mDisplayFrozen
//            + " holdScreen=" + holdScreen);
    if (!mDisplayFrozen) {
        SetHoldScreenLocked(holdScreen != NULL);
        if (screenBrightness < 0 || screenBrightness > 1.0f) {
//            mPowerManager->SetScreenBrightnessOverride(-1);
        }
        else {
//            mPowerManager->SetScreenBrightnessOverride((Int32)
//                    (screenBrightness * Power.BRIGHTNESS_ON));
        }
        if (buttonBrightness < 0 || buttonBrightness > 1.0f) {
//            mPowerManager->SetButtonBrightnessOverride(-1);
        }
        else {
//            mPowerManager->SetButtonBrightnessOverride((Int32)
//                    (buttonBrightness * Power.BRIGHTNESS_ON));
        }
        if (holdScreen != mHoldingScreenOn) {
            mHoldingScreenOn = holdScreen;
            ECode (STDCALL CWindowManagerService::*pHandlerFunc)(IWindowSession*);
            pHandlerFunc = &CWindowManagerService::HandleHoldScreenChanged;

            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(holdScreen.Get());
            SendMessage(*(Handle32*)&pHandlerFunc, params);
        }
    }

    if (mTurnOnScreen) {
//        if (DEBUG_VISIBILITY) Slog.v(TAG, "Turning screen on after layout!");
//        mPowerManager->UserActivity(SystemClock.uptimeMillis(), FALSE,
//                LocalPowerManager.BUTTON_EVENT, TRUE);
        mTurnOnScreen = FALSE;
    }

    // Check to see if we are now in a state where the screen should
    // be enabled, because the window obscured flags have changed.
    EnableScreenIfNeededLocked();
}

void CWindowManagerService::StartFreezingDisplayLocked()
{
    if (mDisplayFrozen) {
        return;
    }

//    mScreenFrozenLock.acquire();

    Int64 now = SystemClock::GetUptimeMillis();
    //Slog.i(TAG, "Freezing, gc pending: " + mFreezeGcPending + ", now " + now);
    if (mFreezeGcPending != 0) {
        if (now > (mFreezeGcPending + 1000)) {
            //Slog.i(TAG, "Gc!  " + now + " > " + (mFreezeGcPending+1000));
            ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
            pHandlerFunc = &CWindowManagerService::HandleForceGc;

            RemoveMessage(*(Handle32*)&pHandlerFunc);
//            Runtime.getRuntime().gc();
            mFreezeGcPending = now;
        }
    }
    else {
        mFreezeGcPending = now;
    }

//    if (DEBUG_FREEZE) Slog.v(TAG, "*** FREEZING DISPLAY", new RuntimeException());

    mDisplayFrozen = TRUE;

    mInputMonitor->FreezeInputDispatchingLw();

    if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
        mNextAppTransition = WindowManagerPolicy::TRANSIT_UNSET;
        mNextAppTransitionPackage = NULL;
        mAppTransitionReady = TRUE;
    }

//    if (PROFILE_ORIENTATION) {
//        File file = new File("/data/system/frozen");
//        Debug.startMethodTracing(file.toString(), 8 * 1024 * 1024);
//    }
//??????????????i need static function    CSurface::FreezeDisplay(0);
}

void CWindowManagerService::StopFreezingDisplayLocked()
{
    if (!mDisplayFrozen) {
        return;
    }

    if (mWaitingForConfig || mAppsFreezingScreen > 0 || mWindowsFreezingScreen) {
        return;
    }

//    if (DEBUG_FREEZE) Slog.v(TAG, "*** UNFREEZING DISPLAY", new RuntimeException());

    mDisplayFrozen = FALSE;

    ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
    pHandlerFunc = &CWindowManagerService::HandleAppFreezeTimeout;

    RemoveMessage(*(Handle32*)&pHandlerFunc);

//    if (PROFILE_ORIENTATION) {
//        Debug.stopMethodTracing();
//    }
//??????????????i need static function    CSurface::UnfreezeDisplay(0);

    mInputMonitor->ThawInputDispatchingLw();

    // While the display is frozen we don't re-compute the orientation
    // to avoid inconsistent states.  However, something interesting
    // could have actually changed during that time so re-evaluate it
    // now to catch that.
    if (UpdateOrientationFromAppTokensLocked()) {
        pHandlerFunc = &CWindowManagerService::HandleSendNewConfiguration;

        SendMessage(*(Handle32*)&pHandlerFunc, NULL);
    }

    // A little kludge: a lot could have happened while the
    // display was frozen, so now that we are coming back we
    // do a gc so that any remote references the system
    // processes holds on others can be released if they are
    // no longer needed.
    pHandlerFunc = &CWindowManagerService::HandleForceGc;

    RemoveMessage(*(Handle32*)&pHandlerFunc);
    SendMessageDelayed(*(Handle32*)&pHandlerFunc, NULL, 2000);

//    mScreenFrozenLock.release();
}

Int32 CWindowManagerService::GetPropertyInt(
    /* [in] */ ArrayOf<String>* tokens,
    /* [in] */ Int32 index,
    /* [in] */ Int32 defUnits,
    /* [in] */ Int32 defDps,
    /* [in] */ IDisplayMetrics* dm)
{
    if (index < tokens->GetLength()) {
        String str = (*tokens)[index];
        if (!str.IsNullOrEmpty()) {
//            try {
//??????            Int32 val = Integer.parseInt(str);
//            return val;
//            } catch (Exception e) {
//            }
        }
    }
    if (defUnits == TypedValue_COMPLEX_UNIT_PX) {
        return defDps;
    }
    AutoPtr<ITypedValueHelper> helper;
    CTypedValueHelper::AcquireSingleton((ITypedValueHelper**)&helper);
    Float val;
    helper->ApplyDimension(defUnits, defDps, dm, &val);
    return (Int32)val;
}

CWindowManagerService::Watermark::Watermark(
    /* [in] */ CWindowManagerService* service,
    /* [in] */ CSurfaceSession* session,
    /* [in] */ ArrayOf<String>* tokens) :
        mWMService(service)
{
}

CWindowManagerService::Watermark::~Watermark()
{
}

void CWindowManagerService::CreateWatermark()
{
}

Boolean CWindowManagerService::UpdateWallpaperOffsetLocked(
    /* [in] */ WindowState* wallpaperWin,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh,
    /* [in] */ Boolean sync)
{
    Boolean changed = FALSE;
    Boolean rawChanged = FALSE;
    Float wpx = mLastWallpaperX >= 0 ? mLastWallpaperX : 0.5;
    Float wpxs = mLastWallpaperXStep >= 0 ? mLastWallpaperXStep : -1.0;
    Int32 wallpaperWinRight, wallpaperWinLeft;
    wallpaperWin->mFrame->GetRight(&wallpaperWinRight);
    wallpaperWin->mFrame->GetLeft(&wallpaperWinLeft);
    Int32 availw = wallpaperWinRight - wallpaperWinLeft - dw;
    Int32 offset = availw > 0 ? - (Int32)(availw * wpx + 0.5) : 0;
    changed = wallpaperWin->mXOffset != offset;
    if (changed) {
//        if (DEBUG_WALLPAPER) Slog.v(TAG, "Update wallpaper "
//                + wallpaperWin + " x: " + offset);
        wallpaperWin->mXOffset = offset;
    }
    if (wallpaperWin->mWallpaperX != wpx || wallpaperWin->mWallpaperXStep != wpxs) {
        wallpaperWin->mWallpaperX = wpx;
        wallpaperWin->mWallpaperXStep = wpxs;
        rawChanged = TRUE;
    }

    Float wpy = mLastWallpaperY >= 0 ? mLastWallpaperY : 0.5;
    Float wpys = mLastWallpaperYStep >= 0 ? mLastWallpaperYStep : -1.0;
    Int32 wallpaperWinBottom, wallpaperWinTop;
    wallpaperWin->mFrame->GetBottom(&wallpaperWinBottom);
    wallpaperWin->mFrame->GetTop(&wallpaperWinTop);
    Int32 availh = wallpaperWinBottom - wallpaperWinTop - dh;
    offset = availh > 0 ? - (Int32)(availh * wpy + 0.5) : 0;
    if (wallpaperWin->mYOffset != offset) {
//        if (DEBUG_WALLPAPER) Slog.v(TAG, "Update wallpaper "
//                + wallpaperWin + " y: " + offset);
        changed = TRUE;
        wallpaperWin->mYOffset = offset;
    }
    if (wallpaperWin->mWallpaperY != wpy || wallpaperWin->mWallpaperYStep != wpys) {
        wallpaperWin->mWallpaperY = wpy;
        wallpaperWin->mWallpaperYStep = wpys;
        rawChanged = TRUE;
    }

    if (rawChanged) {
//        try {
//        if (DEBUG_WALLPAPER) Slog.v(TAG, "Report new wp offset "
//                + wallpaperWin + " x=" + wallpaperWin.mWallpaperX
//                + " y=" + wallpaperWin.mWallpaperY);
        if (sync) {
            mWaitingOnWallpaper = wallpaperWin;
        }
        wallpaperWin->mClient->DispatchWallpaperOffsets(
                wallpaperWin->mWallpaperX, wallpaperWin->mWallpaperY,
                wallpaperWin->mWallpaperXStep, wallpaperWin->mWallpaperYStep, sync);
        if (sync) {
            if (mWaitingOnWallpaper != NULL) {
                Millisecond64 start = SystemClock::GetUptimeMillis();
                if ((mLastWallpaperTimeoutTime + WALLPAPER_TIMEOUT_RECOVERY)
                        < start) {
//                    try {
//                    if (DEBUG_WALLPAPER) Slog.v(TAG,
//                            "Waiting for offset complete...");
//                    mWindowMap.wait(WALLPAPER_TIMEOUT);//????????????????
//                    } catch (InterruptedException e) {
//                    }
//                    if (DEBUG_WALLPAPER) Slog.v(TAG, "Offset complete!");
                    if ((start + WALLPAPER_TIMEOUT)
                            < SystemClock::GetUptimeMillis()) {
//                        Slog.i(TAG, "Timeout waiting for wallpaper to offset: "
//                                + wallpaperWin);
                        mLastWallpaperTimeoutTime = start;
                    }
                }
                mWaitingOnWallpaper = NULL;
            }
        }
//        } catch (RemoteException e) {
//        }
    }

    return changed;
}

AutoPtr<CWindowManagerService::WindowState>
CWindowManagerService::WindowForClientLocked(
    /* [in] */ CWindowSession* session,
    /* [in] */ IInnerWindow* client,
    /* [in] */ Boolean throwOnError)
{
    return WindowForClientLocked(session, (IBinder*)client, throwOnError);
}

/**
 * Watch the rotation of the screen.  Returns the current rotation,
 * calls back when it changes.
 */
AutoPtr<CWindowManagerService::WindowState>
CWindowManagerService::WindowForClientLocked(
    /* [in] */ CWindowSession* session,
    /* [in] */ IBinder* client,
    /* [in] */ Boolean throwOnError)
{
    AutoPtr<WindowState> win;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> >::Iterator it
            = mWindowMap.Find(client);
    if (it != mWindowMap.End()) {
        win = it->mSecond;
    }

//    if (localLOGV) Slog.v(
//        TAG, "Looking up client " + client + ": " + win);
    if (win == NULL) {
//        RuntimeException ex = new IllegalArgumentException(
//                "Requested window " + client + " does not exist");
//        if (throwOnError) {
//            throw ex;
//        }
//        Slog.w(TAG, "Failed looking up window", ex);
        return NULL;
    }
    if (session != NULL && (CWindowSession*)(win->mSession) != session) {
//        RuntimeException ex = new IllegalArgumentException(
//                "Requested window " + client + " is in session " +
//                win.mSession + ", not " + session);
//        if (throwOnError) {
//            throw ex;
//        }
//        Slog.w(TAG, "Failed looking up window", ex);
        return NULL;
    }

    return win;
}

void CWindowManagerService::PlaceWindowBefore(
    /* [in] */ WindowState* pos,
    /* [in] */ WindowState* window)
{
//    if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT) Slog.v(
//        TAG, "Adding window " + window + " at "
//        + i + " of " + mWindows.size() + " (before " + pos + ")");

    List<AutoPtr<WindowState> >::Iterator it;
    it = Find(mWindows.Begin(), mWindows.End(), AutoPtr<WindowState>(pos));
    mWindows.Insert(it, window);
    mWindowsChanged = TRUE;
}

void CWindowManagerService::PlaceWindowAfter(
    /* [in] */ WindowState* pos,
    /* [in] */ WindowState* window)
{
//    if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT) Slog.v(
//        TAG, "Adding window " + window + " at "
//        + (i+1) + " of " + mWindows.size() + " (after " + pos + ")");

    List<AutoPtr<WindowState> >::Iterator it;
    it = Find(mWindows.Begin(), mWindows.End(), AutoPtr<WindowState>(pos));
    if (it == mWindows.End()) {
        mWindows.PushBack(window);
    }
    else {
        mWindows.Insert(++it, window);
    }
    mWindowsChanged = TRUE;
}

//This method finds out the index of a window that has the same app token as
//win. used for z ordering the windows in mWindows
List<AutoPtr<CWindowManagerService::WindowState> >::Iterator
CWindowManagerService::FindIteratorBasedOnAppTokens(
   /* [in] */ WindowState* win)
{
    //use a local variable to cache mWindows
    List<AutoPtr<WindowState> >& localmWindows = mWindows;

    List<AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = localmWindows.RBegin(); rit != localmWindows.REnd(); ++rit) {
        AutoPtr<WindowState> wentry = *rit;
        if(wentry->mAppToken == win->mAppToken) {
            return (++rit).GetBase();
        }
    }
    return localmWindows.End();
}

void CWindowManagerService::AddWindowToListInOrderLocked(
    /* [in] */ WindowState* win,
    /* [in] */ Boolean addToToken)
{
//      IInnerWindow* client = win->mClient;
    WindowToken* token = win->mToken;
    List<AutoPtr<WindowState> >& localmWindows = mWindows;

    AutoPtr<WindowState> attached = win->mAttachedWindow;
    List<AutoPtr<WindowState> >::Iterator winsIt;
    List<AutoPtr<WindowState> >::ReverseIterator winsRIt;
    if (attached == NULL) {
        List<AutoPtr<WindowState> >::Iterator tokenWindowsPosIt = token->mWindows.End();
        if (token->mAppWindowToken != NULL) {
            if (token->mWindows.Begin() != token->mWindows.End()) {
                // If this application has existing windows, we
                // simply place the new window on top of them... but
                // keep the starting window on top.
                Int32 winType;
                win->mAttrs->GetType(&winType);
                if (winType == WindowManagerLayoutParams_TYPE_BASE_APPLICATION) {
                    // Base windows go behind everything else.
                    PlaceWindowBefore(*(token->mWindows.Begin()), win);
                    tokenWindowsPosIt = token->mWindows.Begin();
                }
                else {
                    AppWindowToken* atoken = win->mAppToken;
                    if (atoken != NULL &&
                            *(token->mWindows.RBegin()) == atoken->mStartingWindow) {
                        PlaceWindowBefore(*(token->mWindows.RBegin()), win);
                        tokenWindowsPosIt = --token->mWindows.End();
                    }
                    else {
                        List<AutoPtr<WindowState> >::Iterator it = FindIteratorBasedOnAppTokens(win);
                        if(it != localmWindows.End()) {
                            //there is a window above this one associated with the same
                            //apptoken note that the window could be a floating window
                            //that was created later or a window at the top of the list of
                            //windows associated with this token.
//                            if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT) Slog.v(
//                                    TAG, "Adding window " + win + " at "
//                                    + (newIdx+1) + " of " + N);
                            localmWindows.Insert(++it, win);
                            mWindowsChanged = TRUE;
                        }
                    }
                }
            }
            else {
//                if (localLOGV) Slog.v(
//                    TAG, "Figuring out where to add app window "
//                    + client.asBinder() + " (token=" + token + ")");
                // Figure out where the window should go, based on the
                // order of applications.

                AutoPtr<WindowState> pos;
                List<AppWindowToken*>::ReverseIterator awtRIt;
                for (awtRIt = mAppTokens.RBegin();
                        awtRIt != mAppTokens.REnd(); ++awtRIt) {
                    AppWindowToken* t = *awtRIt;
                    if (t == token) {
                        ++awtRIt;
                        break;
                    }

                    // We haven't reached the token yet; if this token
                    // is not going to the bottom and has windows, we can
                    // use it as an anchor for when we do reach the token.
                    if (!t->mSendingToBottom &&
                            t->mWindows.Begin() != t->mWindows.End()) {
                        pos = *(t->mWindows.Begin());
                    }
                }
                // We now know the index into the apps.  If we found
                // an app window above, that gives us the position; else
                // we need to look some more.
                if (pos != NULL) {
                    // Move behind any windows attached to this one.
                    WindowToken* atoken = NULL;
                    HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator tmIt
                            = mTokenMap.Find((IBinder*)(pos->mClient));
                    if (tmIt != mTokenMap.End()) {
                        atoken = tmIt->mSecond;
                    }

                    if (atoken != NULL) {
                        if (atoken->mWindows.Begin() != atoken->mWindows.End()) {
                            AutoPtr<WindowState> bottom = *(atoken->mWindows.Begin());
                            if (bottom->mSubLayer < 0) {
                                pos = bottom;
                            }
                        }
                    }
                    PlaceWindowBefore(pos, win);
                }
                else {
                    // Continue looking down until we find the first
                    // token that has windows.
                    for (; awtRIt != mAppTokens.REnd(); ++awtRIt) {
                        AppWindowToken* t = *awtRIt;
                        if (t->mWindows.Begin() != t->mWindows.End()) {
                            pos = *(t->mWindows.RBegin());
                            break;
                        }
                    }

                    if (pos != NULL) {
                        // Move in front of any windows attached to this
                        // one.
                        WindowToken* atoken = NULL;
                        HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator tmIt
                                = mTokenMap.Find((IBinder*)(pos->mClient));
                        if (tmIt != mTokenMap.End()) {
                            atoken = tmIt->mSecond;
                        }

                        if (atoken != NULL) {
                            if (atoken->mWindows.Begin() != atoken->mWindows.End()) {
                                AutoPtr<WindowState> top = *(atoken->mWindows.RBegin());
                                if (top->mSubLayer >= 0) {
                                    pos = top;
                                }
                            }
                        }
                        PlaceWindowAfter(pos, win);
                    }
                    else {
                        // Just search for the start of this layer.
                        Int32 myLayer = win->mBaseLayer;
                        for (winsIt = localmWindows.Begin();
                                winsIt != localmWindows.End(); ++winsIt) {
                            AutoPtr<WindowState> w = *winsIt;
                            if (w->mBaseLayer > myLayer) {
                                break;
                            }
                        }
//                        if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT) Slog.v(
//                                TAG, "Adding window " + win + " at "
//                                + i + " of " + N);
                        localmWindows.Insert(winsIt, win);
                        mWindowsChanged = TRUE;
                    }
                }
            }
        } else {
            // Figure out where window should go, based on layer.
            Int32 myLayer = win->mBaseLayer;
            for (winsRIt = localmWindows.RBegin();
                    winsRIt != localmWindows.REnd(); ++winsRIt) {
                if ((*winsRIt)->mBaseLayer <= myLayer) {
                    break;
                }
            }
//            if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT) Slog.v(
//                    TAG, "Adding window " + win + " at "
//                    + i + " of " + N);
            localmWindows.Insert(winsRIt.GetBase(), win);
            mWindowsChanged = TRUE;
        }
        if (addToToken) {
            token->mWindows.Insert(tokenWindowsPosIt, win);
        }
    }
    else {
        // Figure out this window's ordering relative to the window
        // it is attached to.
        Int32 sublayer = win->mSubLayer;
        Int32 largestSublayer = 0x80000000;//Integer.MIN_VALUE;
        AutoPtr<WindowState> windowWithLargestSublayer;
        for (winsIt = token->mWindows.Begin();
                winsIt != token->mWindows.End(); ++winsIt) {
            AutoPtr<WindowState> w = *winsIt;
            Int32 wSublayer = w->mSubLayer;
            if (wSublayer >= largestSublayer) {
                largestSublayer = wSublayer;
                windowWithLargestSublayer = w;
            }
            if (sublayer < 0) {
                // For negative sublayers, we go below all windows
                // in the same sublayer.
                if (wSublayer >= sublayer) {
                    if (addToToken) {
                        token->mWindows.Insert(winsIt, win);
                    }
                    PlaceWindowBefore(
                        wSublayer >= 0 ? attached : w, win);
                    break;
                }
            }
            else {
                // For positive sublayers, we go above all windows
                // in the same sublayer.
                if (wSublayer > sublayer) {
                    if (addToToken) {
                        token->mWindows.Insert(winsIt, win);
                    }
                    PlaceWindowBefore(w, win);
                    break;
                }
            }
        }
        if (winsIt == token->mWindows.End()) {
            if (addToToken) {
                token->mWindows.PushBack(win);
            }
            if (sublayer < 0) {
                PlaceWindowBefore(attached, win);
            }
            else {
                PlaceWindowAfter(largestSublayer >= 0
                                 ? windowWithLargestSublayer
                                 : attached,
                                 win);
            }
        }
    }

    if (win->mAppToken != NULL && addToToken) {
        win->mAppToken->mAllAppWindows.PushBack(win);
    }
}

void CWindowManagerService::RemoveWindowInnerLocked(
        /* [in] */ CWindowSession* session,
        /* [in] */ WindowState* win)
{
    win->mRemoved = TRUE;

//    if (mInputMethodTarget == win) {
//        MoveInputMethodWindowsIfNeededLocked(FALSE);
//    }

//    if (false) {
//        RuntimeException e = new RuntimeException("here");
//        e.fillInStackTrace();
//        Slog.w(TAG, "Removing window " + win, e);
//    }

//    mPolicy->RemoveWindowLw(win);
    win->RemoveLocked();

    mWindowMap.Erase(IBinder::Probe(win->mClient.Get()));
    mWindows.Remove(win);
    mWindowsChanged = TRUE;
//    if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Final remove of window: " + win);

    Int32 winType, winFlags;
    if (mInputMethodWindow.Get() == win) {
        mInputMethodWindow = NULL;
    }
    else {
        win->mAttrs->GetType(&winType);
        if (winType == WindowManagerLayoutParams_TYPE_INPUT_METHOD_DIALOG) {
            mInputMethodDialogs.Remove(win);
        }
    }

    WindowToken* token = win->mToken;
    AppWindowToken* atoken = win->mAppToken;
    token->mWindows.Remove(win);
    if (atoken != NULL) {
        atoken->mAllAppWindows.Remove(win);
    }
//    if (localLOGV) Slog.v(
//            TAG, "**** Removing window " + win + ": count="
//            + token.windows.size());
    if (token->mWindows.Begin() == token->mWindows.End()) {
        if (!token->mExplicit && token->mToken != NULL) {
            mTokenMap.Erase(token->mToken);
            mTokenList.Remove(token);
        }
        else if (atoken != NULL) {
            atoken->mFirstWindowDrawn = FALSE;
        }
    }

    if (atoken != NULL) {
        if (atoken->mStartingWindow.Get() == win) {
            atoken->mStartingWindow = NULL;
        }
        else if (atoken->mAllAppWindows.Begin() == atoken->mAllAppWindows.End() && atoken->mStartingData != NULL) {
            // If this is the last window and we had requested a starting
            // transition window, well there is no point now.
            atoken->mStartingData = NULL;
        }
        else if (++atoken->mAllAppWindows.Begin() == atoken->mAllAppWindows.End() && atoken->mStartingView != NULL) {
            // If this is the last window except for a starting transition
            // window, we need to get rid of the starting transition.
//            if (DEBUG_STARTING_WINDOW) {
//                Slog.v(TAG, "Schedule remove starting " + token
//                        + ": no more real windows");
//            }

            ECode (STDCALL CWindowManagerService::*pHandlerFunc)(
                    AppWindowToken* appWinToken);
            pHandlerFunc = &CWindowManagerService::HandleRemoveStarting;

            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32((Handle32)atoken);
            SendMessage(*(Handle32*)&pHandlerFunc, params);
        }
    }

    win->mAttrs->GetType(&winType);
    if (winType == WindowManagerLayoutParams_TYPE_WALLPAPER) {
        mLastWallpaperTimeoutTime = 0;
        AdjustWallpaperWindowsLocked();
    }
    else {
        win->mAttrs->GetFlags(&winFlags);
        if ((winFlags & WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER) != 0) {
            AdjustWallpaperWindowsLocked();
        }
    }

    if (!mInLayout) {
        AssignLayersLocked();
        mLayoutNeeded = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
        if (win->mAppToken != NULL) {
            win->mAppToken->UpdateReportedVisibilityLocked();
        }
    }

    mInputMonitor->UpdateInputWindowsLw();
}

Int32 CWindowManagerService::AdjustWallpaperWindowsLocked()
{
    Int32 changed = 0;

    Int32 dw, dh;
    mDisplay->GetWidth(&dw);
    mDisplay->GetHeight(&dh);

    // First find top-most window that has asked to be on top of the
    // wallpaper; all wallpapers go behind it.
    List<AutoPtr<WindowState> >& localmWindows = mWindows;
    AutoPtr<WindowState> w;
    AutoPtr<WindowState> foundW;
    List<AutoPtr<WindowState> >::Iterator foundIt = localmWindows.Begin();
    AutoPtr<WindowState> topCurW;
    List<AutoPtr<WindowState> >::Iterator topCurIt = localmWindows.Begin();
    List<AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = mWindows.RBegin(); rit != mWindows.REnd(); ++rit) {
        w = *rit;
        Int32 wType;
        w->mAttrs->GetType(&wType);
        if ((wType == WindowManagerLayoutParams_TYPE_WALLPAPER)) {
            if (topCurW == NULL) {
                topCurW = w;
                topCurIt = --rit.GetBase();
            }
            continue;
        }
        topCurW = NULL;
        if (w->mAppToken != NULL) {
            // If this window's app token is hidden and not animating,
            // it is of no interest to us.
            if (w->mAppToken->mHidden && w->mAppToken->mAnimation == NULL) {
//                if (DEBUG_WALLPAPER) Slog.v(TAG,
//                        "Skipping hidden or animating token: " + w);
                topCurW = NULL;
                continue;
            }
        }
//        if (DEBUG_WALLPAPER) Slog.v(TAG, "Win " + w + ": readyfordisplay="
//                + w.isReadyForDisplay() + " drawpending=" + w.mDrawPending
//                + " commitdrawpending=" + w.mCommitDrawPending);
        Int32 wFlags;
        w->mAttrs->GetFlags(&wFlags);
        if ((wFlags & WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER) != 0
                && w->IsReadyForDisplay()
                && (mWallpaperTarget == w
                        || (!w->mDrawPending && !w->mCommitDrawPending))) {
//            if (DEBUG_WALLPAPER) Slog.v(TAG,
//                    "Found wallpaper activity: #" + i + "=" + w);
            foundW = w;
            foundIt = --rit.GetBase();
            if (w == mWallpaperTarget && ((w->mAppToken != NULL
                    && w->mAppToken->mAnimation != NULL)
                    || w->mAnimation != NULL)) {
                // The current wallpaper target is animating, so we'll
                // look behind it for another possible target and figure
                // out what is going on below.
//                if (DEBUG_WALLPAPER) Slog.v(TAG, "Win " + w
//                        + ": token animating, looking behind.");
                continue;
            }
            break;
        }
    }

    if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
        // If we are currently waiting for an app transition, and either
        // the current target or the next target are involved with it,
        // then hold off on doing anything with the wallpaper.
        // Note that we are checking here for just whether the target
        // is part of an app token...  which is potentially overly aggressive
        // (the app token may not be involved in the transition), but good
        // enough (we'll just wait until whatever transition is pending
        // executes).
        if (mWallpaperTarget != NULL && mWallpaperTarget->mAppToken != NULL) {
//            if (DEBUG_WALLPAPER) Slog.v(TAG,
//                    "Wallpaper not changing: waiting for app anim in current target");
            return 0;
        }
        if (foundW != NULL && foundW->mAppToken != NULL) {
//            if (DEBUG_WALLPAPER) Slog.v(TAG,
//                    "Wallpaper not changing: waiting for app anim in found target");
            return 0;
        }
    }

    if (mWallpaperTarget != foundW) {
//        if (DEBUG_WALLPAPER) {
//            Slog.v(TAG, "New wallpaper target: " + foundW
//                    + " oldTarget: " + mWallpaperTarget);
//        }

        mLowerWallpaperTarget = NULL;
        mUpperWallpaperTarget = NULL;

        AutoPtr<WindowState> oldW = mWallpaperTarget;
        mWallpaperTarget = foundW;

        // Now what is happening...  if the current and new targets are
        // animating, then we are in our super special mode!
        if (foundW != NULL && oldW != NULL) {
            Boolean oldAnim = oldW->mAnimation != NULL
                || (oldW->mAppToken != NULL && oldW->mAppToken->mAnimation != NULL);
            Boolean foundAnim = foundW->mAnimation != NULL
                || (foundW->mAppToken != NULL && foundW->mAppToken->mAnimation != NULL);
//            if (DEBUG_WALLPAPER) {
//                Slog.v(TAG, "New animation: " + foundAnim
//                        + " old animation: " + oldAnim);
//            }
            if (foundAnim && oldAnim) {
                Boolean isUpper = TRUE;
                List<AutoPtr<WindowState> >::Iterator oldIt =
                        Find(localmWindows.Begin(), localmWindows.End(), oldW);
//                if (DEBUG_WALLPAPER) {
//                    Slog.v(TAG, "New i: " + foundI + " old i: " + oldI);
//                }
                if (oldIt != localmWindows.End()) {
//                    if (DEBUG_WALLPAPER) {
//                        Slog.v(TAG, "Animating wallpapers: old#" + oldI
//                                + "=" + oldW + "; new#" + foundI
//                                + "=" + foundW);
//                    }

                    // Set the new target correctly.
                    if (foundW->mAppToken != NULL && foundW->mAppToken->mHiddenRequested) {
//                        if (DEBUG_WALLPAPER) {
//                            Slog.v(TAG, "Old wallpaper still the target.");
//                        }
                        mWallpaperTarget = oldW;
                    }

                    // Now set the upper and lower wallpaper targets
                    // correctly, and make sure that we are positioning
                    // the wallpaper below the lower.
                    if (isUpper) {
                        // The new target is on top of the old one.
//                        if (DEBUG_WALLPAPER) {
//                            Slog.v(TAG, "Found target above old target.");
//                        }
                        mUpperWallpaperTarget = foundW;
                        mLowerWallpaperTarget = oldW;
                        foundW = oldW;
                        foundIt = oldIt;
                    }
                    else {
                        // The new target is below the old one.
//                        if (DEBUG_WALLPAPER) {
//                            Slog.v(TAG, "Found target below old target.");
//                        }
                        mUpperWallpaperTarget = oldW;
                        mLowerWallpaperTarget = foundW;
                    }
                }
            }
        }
    }
    else if (mLowerWallpaperTarget != NULL) {
        // Is it time to stop animating?
        Boolean lowerAnimating = mLowerWallpaperTarget->mAnimation != NULL
                || (mLowerWallpaperTarget->mAppToken != NULL
                && mLowerWallpaperTarget->mAppToken->mAnimation != NULL);
        Boolean upperAnimating = mUpperWallpaperTarget->mAnimation != NULL
                || (mUpperWallpaperTarget->mAppToken != NULL
                && mUpperWallpaperTarget->mAppToken->mAnimation != NULL);
        if (!lowerAnimating || !upperAnimating) {
//            if (DEBUG_WALLPAPER) {
//                Slog.v(TAG, "No longer animating wallpaper targets!");
//            }
            mLowerWallpaperTarget = NULL;
            mUpperWallpaperTarget = NULL;
        }
    }

    Boolean visible = foundW != NULL;
    if (visible) {
        // The window is visible to the compositor...  but is it visible
        // to the user?  That is what the wallpaper cares about.
        visible = IsWallpaperVisible(foundW);
//        if (DEBUG_WALLPAPER) Slog.v(TAG, "Wallpaper visibility: " + visible);

        // If the wallpaper target is animating, we may need to copy
        // its layer adjustment.  Only do this if we are not transfering
        // between two wallpaper targets.
        mWallpaperAnimLayerAdjustment =
                (mLowerWallpaperTarget == NULL && foundW->mAppToken != NULL)
                ? foundW->mAppToken->mAnimLayerAdjustment : 0;

        Int32 layer;
        mPolicy->GetMaxWallpaperLayer(&layer);
        Int32 maxLayer = layer * TYPE_LAYER_MULTIPLIER + TYPE_LAYER_OFFSET;

        // Now w is the window we are supposed to be behind...  but we
        // need to be sure to also be behind any of its attached windows,
        // AND any starting window associated with it, AND below the
        // maximum layer the policy allows for wallpapers.
        while (foundIt != localmWindows.Begin()) {
            AutoPtr<WindowState> wb = *(--List<AutoPtr<WindowState> >::Iterator(foundIt));
            Int32 wbType;
            wb->mAttrs->GetType(&wbType);
            if (wb->mBaseLayer < maxLayer &&
                    wb->mAttachedWindow != foundW &&
                    (wbType != WindowManagerLayoutParams_TYPE_APPLICATION_STARTING ||
                            wb->mToken != foundW->mToken)) {
                // This window is not related to the previous one in any
                // interesting way, so stop here.
                break;
            }
            foundW = wb;
            foundIt--;
        }
    }
    else {
//        if (DEBUG_WALLPAPER) Slog.v(TAG, "No wallpaper target");
    }

    if (foundW == NULL && topCurW != NULL) {
        // There is no wallpaper target, so it goes at the bottom.
        // We will assume it is the same place as last time, if known.
        foundW = topCurW;
        foundIt = ++List<AutoPtr<WindowState> >::Iterator(topCurIt);
    }
    else {
        // Okay i is the position immediately above the wallpaper.  Look at
        // what is below it for later.
        foundW = foundIt != localmWindows.Begin()
                ? *(--List<AutoPtr<WindowState> >::Iterator(foundIt)) : NULL;
    }

    if (visible) {
        if (mWallpaperTarget->mWallpaperX >= 0) {
            mLastWallpaperX = mWallpaperTarget->mWallpaperX;
            mLastWallpaperXStep = mWallpaperTarget->mWallpaperXStep;
        }
        if (mWallpaperTarget->mWallpaperY >= 0) {
            mLastWallpaperY = mWallpaperTarget->mWallpaperY;
            mLastWallpaperYStep = mWallpaperTarget->mWallpaperYStep;
        }
    }

    // Start stepping backwards from here, ensuring that our wallpaper windows
    // are correctly placed.
    List<WindowToken*>::ReverseIterator wtRit;
    for (wtRit = mWallpaperTokens.RBegin(); wtRit != mWallpaperTokens.REnd(); ++wtRit) {
        WindowToken* token = *wtRit;
        if (token->mHidden == visible) {
            changed |= ADJUST_WALLPAPER_VISIBILITY_CHANGED;
            token->mHidden = !visible;
            // Need to do a layout to ensure the wallpaper now has the
            // correct size.
            mLayoutNeeded = TRUE;
        }

        for (rit = token->mWindows.RBegin(); rit != token->mWindows.REnd(); ++rit) {
            AutoPtr<WindowState> wallpaper = *rit;

            if (visible) {
                UpdateWallpaperOffsetLocked(wallpaper, dw, dh, FALSE);
            }

            // First, make sure the client has the current visibility
            // state.
            if (wallpaper->mWallpaperVisible != visible) {
                wallpaper->mWallpaperVisible = visible;
//                try {
//                if (DEBUG_VISIBILITY || DEBUG_WALLPAPER) Slog.v(TAG,
//                        "Setting visibility of wallpaper " + wallpaper
//                        + ": " + visible);
                wallpaper->mClient->DispatchAppVisibility(visible);
//                } catch (RemoteException e) {
//                }
            }

            wallpaper->mAnimLayer = wallpaper->mLayer + mWallpaperAnimLayerAdjustment;
//            if (DEBUG_LAYERS || DEBUG_WALLPAPER) Slog.v(TAG, "Wallpaper win "
//                    + wallpaper + " anim layer: " + wallpaper.mAnimLayer);

            // First, if this window is at the current index, then all
            // is well.
            if (wallpaper == foundW) {
                foundIt--;
                foundW = foundIt != localmWindows.Begin()
                        ? *(--List<AutoPtr<WindowState> >::Iterator(foundIt)) : NULL;
                continue;
            }

            // The window didn't match...  the current wallpaper window,
            // wherever it is, is in the wrong place, so make sure it is
            // not in the list.
            Boolean isUpper = TRUE;
            List<AutoPtr<WindowState> >::Iterator oldIt;
            for (oldIt = localmWindows.Begin(); oldIt != localmWindows.End(); ++oldIt) {
                if (oldIt == foundIt) isUpper = FALSE;
                if (*oldIt == wallpaper) break;
            }
            if (oldIt != localmWindows.End()) {
//                if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Wallpaper removing at "
//                        + oldIndex + ": " + wallpaper);
                localmWindows.Erase(oldIt);
                mWindowsChanged = TRUE;
                if (!isUpper) {
                    foundIt--;
                }
            }

            // Now stick it in.
//            if (DEBUG_WALLPAPER || DEBUG_WINDOW_MOVEMENT) Slog.v(TAG,
//                    "Moving wallpaper " + wallpaper
//                    + " from " + oldIndex + " to " + foundI);

            localmWindows.Insert(foundIt, wallpaper);
            mWindowsChanged = TRUE;
            changed |= ADJUST_WALLPAPER_LAYERS_CHANGED;
        }
    }

    return changed;
}

void CWindowManagerService::AssignLayersLocked()
{
    Int32 curBaseLayer = 0;
    Int32 curLayer = 0;
    List<AutoPtr<WindowState> >::Iterator it;

    for (it = mWindows.Begin(); it != mWindows.End(); it++) {
        AutoPtr<WindowState> w = *it;
        if (w->mBaseLayer == curBaseLayer || w->mIsImWindow
                || (it != mWindows.Begin() && w->mIsWallpaper)) {
            curLayer += WINDOW_LAYER_MULTIPLIER;
            w->mLayer = curLayer;
        }
        else {
            curBaseLayer = curLayer = w->mBaseLayer;
            w->mLayer = curLayer;
        }
        if (w->mTargetAppToken != NULL) {
            w->mAnimLayer = w->mLayer + w->mTargetAppToken->mAnimLayerAdjustment;
        }
        else if (w->mAppToken != NULL) {
            w->mAnimLayer = w->mLayer + w->mAppToken->mAnimLayerAdjustment;
        }
        else {
            w->mAnimLayer = w->mLayer;
        }
        if (w->mIsImWindow) {
            w->mAnimLayer += mInputMethodAnimLayerAdjustment;
        }
        else if (w->mIsWallpaper) {
            w->mAnimLayer += mWallpaperAnimLayerAdjustment;
        }
//        if (DEBUG_LAYERS) Slog.v(TAG, "Assign layer " + w + ": "
//                + w->mAnimLayer);
        //System.out.println(
        //    "Assigned layer " + curLayer + " to " + w->mClient.asBinder());
    }
}

/*
 * Determine the new desired orientation of the display, returning
 * a non-NULL new Configuration if it has changed from the current
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
Boolean CWindowManagerService::UpdateOrientationFromAppTokensLocked()
{
    if (mDisplayFrozen) {
        // If the display is frozen, some activities may be in the middle
        // of restarting, and thus have removed their old window.  If the
        // window has the flag to hide the lock screen, then the lock screen
        // can re-appear and inflict its own orientation on us.  Keep the
        // orientation stable until this all settles down.
        return FALSE;
    }

    Boolean changed = FALSE;
//    long ident = Binder.clearCallingIdentity();
//    try {
    Int32 req = ComputeForcedAppOrientationLocked();

    if (req != mForcedAppOrientation) {
        mForcedAppOrientation = req;
        //send a message to Policy indicating orientation change to take
        //action like disabling/enabling sensors etc.,
        mPolicy->SetCurrentOrientationLw(req);
        if (SetRotationUncheckedLocked(WindowManagerPolicy::USE_LAST_ROTATION,
                mLastRotationFlags | Surface_FLAGS_ORIENTATION_ANIMATION_DISABLE)) {
            changed = TRUE;
        }
    }

    return changed;
//    } finally {
//        Binder.restoreCallingIdentity(ident);
//    }
}

Boolean CWindowManagerService::ApplyAnimationLocked(
    /* [in] */ WindowState* win,
    /* [in] */ Int32 transit,
    /* [in] */ Boolean isEntrance)
{
    if (win->mLocalAnimating && win->mAnimationIsEntrance == isEntrance) {
        // If we are trying to apply an animation, but already running
        // an animation of the same type, then just leave that one alone.
        return TRUE;
    }

    // Only apply an animation if the display isn't frozen.  If it is
    // frozen, there is no reason to animate and it can cause strange
    // artifacts when we unfreeze the display if some different animation
    // is running.
    Boolean isOn;
    mPolicy->IsScreenOn(&isOn);
    if (!mDisplayFrozen && isOn) {
        Int32 anim;
        mPolicy->SelectAnimationLw(win, transit, &anim);
        Int32 attr = -1;
        AutoPtr<IAnimation> a;
        if (anim != 0) {
            AutoPtr<IAnimationUtils> animationUtils;
            CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
            animationUtils->LoadAnimation(mContext, anim, (IAnimation**)&a);
        }
        else {
            switch (transit) {
                case WindowManagerPolicy_TRANSIT_ENTER:
                    attr = 0;//com.android.internal.R.styleable.WindowAnimation_windowEnterAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_EXIT:
                    attr = 1;//com.android.internal.R.styleable.WindowAnimation_windowExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_SHOW:
                    attr = 2;//com.android.internal.R.styleable.WindowAnimation_windowShowAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_HIDE:
                    attr = 3;//com.android.internal.R.styleable.WindowAnimation_windowHideAnimation;
                    break;
            }
            if (attr >= 0) {
                a = LoadAnimation(win->mAttrs, attr);
            }
        }
        //if (DEBUG_ANIM) Slog.v(TAG, "applyAnimation: win=" + win
        //        + " anim=" + anim + " attr=0x" + Integer.toHexString(attr)
        //        + " mAnimation=" + win.mAnimation
        //        + " isEntrance=" + isEntrance);
        if (a != NULL) {
 /*           if (DEBUG_ANIM) {
                RuntimeException e = NULL;
                if (!HIDE_STACK_CRAWLS) {
                    e = new RuntimeException();
                    e.fillInStackTrace();
                }
                Slog.v(TAG, "Loaded animation " + a + " for " + win, e);
            }*/
            win->SetAnimation(a);
            win->mAnimationIsEntrance = isEntrance;
        }
    }
    else {
        win->ClearAnimation();
    }

    return win->mAnimation != NULL;
}

AutoPtr<IAnimation> CWindowManagerService::LoadAnimation(
    /* [in] */ IWindowManagerLayoutParams* lp,
    /* [in] */ Int32 animAttr)
{
    AutoPtr<IAnimation> animation;
    Int32 anim = 0;
    AutoPtr<IContext> context = mContext;
    //if (animAttr >= 0) {
    //    AttributeCache.Entry ent = GetCachedAnimations(lp);
    //    if (ent != null) {
    //        context = ent.context;
    //        anim = ent.array.getResourceId(animAttr, 0);
    //    }
    //}
    if (anim != 0) {
        AutoPtr<IAnimationUtils> animationUtils;
        CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
        animationUtils->LoadAnimation(context, anim, (IAnimation**)&animation);
    }

    return animation;
}

AutoPtr<IAnimation> CWindowManagerService::LoadAnimation(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 resId)
{
    AutoPtr<IAnimation> animation;
    Int32 anim = 0;
    AutoPtr<IContext> context = mContext;
    //if (resId >= 0) {
    //    AttributeCache.Entry ent = getCachedAnimations(packageName, resId);
    //    if (ent != null) {
    //        context = ent.context;
    //        anim = resId;
    //    }
    //}
    if (anim != 0) {
        AutoPtr<IAnimationUtils> animationUtils;
        CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
        animationUtils->LoadAnimation(context, anim, (IAnimation**)&animation);
    }

    return animation;
}

Boolean CWindowManagerService::ApplyAnimationLocked(
    /* [in] */ AppWindowToken* wtoken,
    /* [in] */ IWindowManagerLayoutParams* lp,
    /* [in] */ Int32 transit,
    /* [in] */ Boolean enter)
{
    // Only apply an animation if the display isn't frozen.  If it is
    // frozen, there is no reason to animate and it can cause strange
    // artifacts when we unfreeze the display if some different animation
    // is running.
    Boolean isOn;
    mPolicy->IsScreenOn(&isOn);
    if (!mDisplayFrozen && isOn) {
        AutoPtr<IAnimation> a;
        Int32 flags;
        if (lp != NULL && (lp->GetFlags(&flags), flags & WindowManagerLayoutParams_FLAG_COMPATIBLE_WINDOW) != 0) {
            a = new FadeInOutAnimation(enter);
//            if (DEBUG_ANIM) Slog.v(TAG,
//                    "applying FadeInOutAnimation for a window in compatibility mode");
        }
        else if (!mNextAppTransitionPackage.IsNull()) {
            a = LoadAnimation(mNextAppTransitionPackage, enter ?
                    mNextAppTransitionEnter : mNextAppTransitionExit);
        }
        else {
            Int32 animAttr = 0;
            switch (transit) {
                case WindowManagerPolicy_TRANSIT_ACTIVITY_OPEN:
                    animAttr = enter
                            ? 4//com.android.internal.R.styleable.WindowAnimation_activityOpenEnterAnimation
                            : 5;//com.android.internal.R.styleable.WindowAnimation_activityOpenExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_ACTIVITY_CLOSE:
                    animAttr = enter
                            ? 6//com.android.internal.R.styleable.WindowAnimation_activityCloseEnterAnimation
                            : 7;//com.android.internal.R.styleable.WindowAnimation_activityCloseExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_TASK_OPEN:
                    animAttr = enter
                            ? 8//com.android.internal.R.styleable.WindowAnimation_taskOpenEnterAnimation
                            : 9;//com.android.internal.R.styleable.WindowAnimation_taskOpenExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_TASK_CLOSE:
                    animAttr = enter
                            ? 10//com.android.internal.R.styleable.WindowAnimation_taskCloseEnterAnimation
                            : 11;//com.android.internal.R.styleable.WindowAnimation_taskCloseExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_TASK_TO_FRONT:
                    animAttr = enter
                            ? 12//com.android.internal.R.styleable.WindowAnimation_taskToFrontEnterAnimation
                            : 13;//com.android.internal.R.styleable.WindowAnimation_taskToFrontExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_TASK_TO_BACK:
                    animAttr = enter
                            ? 14//com.android.internal.R.styleable.WindowAnimation_taskToBackEnterAnimation
                            : 15;//com.android.internal.R.styleable.WindowAnimation_taskToBackExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_WALLPAPER_OPEN:
                    animAttr = enter
                            ? 16//com.android.internal.R.styleable.WindowAnimation_wallpaperOpenEnterAnimation
                            : 17;//com.android.internal.R.styleable.WindowAnimation_wallpaperOpenExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_WALLPAPER_CLOSE:
                    animAttr = enter
                            ? 18//com.android.internal.R.styleable.WindowAnimation_wallpaperCloseEnterAnimation
                            : 19;//com.android.internal.R.styleable.WindowAnimation_wallpaperCloseExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_WALLPAPER_INTRA_OPEN:
                    animAttr = enter
                            ? 20//com.android.internal.R.styleable.WindowAnimation_wallpaperIntraOpenEnterAnimation
                            : 21;//com.android.internal.R.styleable.WindowAnimation_wallpaperIntraOpenExitAnimation;
                    break;
                case WindowManagerPolicy_TRANSIT_WALLPAPER_INTRA_CLOSE:
                    animAttr = enter
                            ? 22//com.android.internal.R.styleable.WindowAnimation_wallpaperIntraCloseEnterAnimation
                            : 23;//com.android.internal.R.styleable.WindowAnimation_wallpaperIntraCloseExitAnimation;
                    break;
            }
            a = animAttr != 0 ? LoadAnimation(lp, animAttr) : NULL;
//            if (DEBUG_ANIM) Slog.v(TAG, "applyAnimation: wtoken=" + wtoken
//                    + " anim=" + a
//                    + " animAttr=0x" + Integer.toHexString(animAttr)
//                    + " transit=" + transit);
        }
        if (a != NULL) {
//            if (DEBUG_ANIM) {
//                RuntimeException e = NULL;
//                if (!HIDE_STACK_CRAWLS) {
//                    e = new RuntimeException();
//                    e.fillInStackTrace();
//                }
//                Slog.v(TAG, "Loaded animation " + a + " for " + wtoken, e);
//            }
            wtoken->SetAnimation(a);
        }
    }
    else {
        wtoken->ClearAnimation();
    }

    return wtoken->mAnimation != NULL;
}

CWindowManagerService::AppWindowToken*
CWindowManagerService::FindAppWindowToken(
    /* [in] */ IBinder* token)
{
    HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator
        wtit = mTokenMap.Find(AutoPtr<IBinder>(token));
    if (wtit != mTokenMap.End() && wtit->mSecond != NULL) {
        return wtit->mSecond->mAppWindowToken;
    }
    else {
        return NULL;
    }
}

/*
 * Instruct the Activity Manager to fetch the current configuration and broadcast
 * that to config-changed listeners if appropriate.
 */
ECode CWindowManagerService::SendNewConfiguration()
{
//    try {
    return ((CActivityManagerService*)(mActivityManager.Get()))->UpdateConfiguration(NULL);
//    } catch (RemoteException e) {
//    }
}

AutoPtr<IConfiguration> CWindowManagerService::ComputeNewConfiguration()
{
    Mutex::Autolock lock(mWindowMapLock);
    return ComputeNewConfigurationLocked();
}

AutoPtr<IConfiguration> CWindowManagerService::ComputeNewConfigurationLocked()
{
    AutoPtr<IConfiguration> config;
    ASSERT_SUCCEEDED(CConfiguration::New((IConfiguration**)&config));

    if (!ComputeNewConfigurationLocked(config)) {
        return NULL;
    }
    return config;
}

Boolean CWindowManagerService::ComputeNewConfigurationLocked(
    /* [in] */ IConfiguration* config)
{
    if (mDisplay == NULL) {
        return FALSE;
    }

    mInputManager->GetInputConfiguration((IConfiguration*)config);

    // Use the effective "visual" dimensions based on current rotation
    Boolean rotated = (mRotation == Surface_ROTATION_90
            || mRotation == Surface_ROTATION_270);
    Int32 dw = rotated ? mInitialDisplayHeight : mInitialDisplayWidth;
    Int32 dh = rotated ? mInitialDisplayWidth : mInitialDisplayHeight;

    Int32 orientation = Configuration_ORIENTATION_SQUARE;
    if (dw < dh) {
        orientation = Configuration_ORIENTATION_PORTRAIT;
    }
    else if (dw > dh) {
        orientation = Configuration_ORIENTATION_LANDSCAPE;
    }
    config->SetOrientation(orientation);

    AutoPtr<IDisplayMetrics> dm;
    ASSERT_SUCCEEDED(CDisplayMetrics::New((IDisplayMetrics**)&dm));
    mDisplay->GetMetrics(dm);
    AutoPtr<ICompatibilityInfoHelper> helper;
    CCompatibilityInfoHelper::AcquireSingleton((ICompatibilityInfoHelper**)&helper);
    helper->UpdateCompatibleScreenFrame(dm, orientation, mCompatibleScreenFrame);

    if (mScreenLayout == Configuration_SCREENLAYOUT_SIZE_UNDEFINED) {
        // Note we only do this once because at this point we don't
        // expect the screen to change in this way at runtime, and want
        // to avoid all of this computation for every config change.
        Int32 longSize = dw;
        Int32 shortSize = dh;
        if (longSize < shortSize) {
            Int32 tmp = longSize;
            longSize = shortSize;
            shortSize = tmp;
        }
        Float dmDensity;
        dm->GetDensity(&dmDensity);
        longSize = (Int32)(longSize / dmDensity);
        shortSize = (Int32)(shortSize / dmDensity);

        // These semi-magic numbers define our compatibility modes for
        // applications with different screens.  Don't change unless you
        // make sure to test lots and lots of apps!
        if (longSize < 470) {
            // This is shorter than an HVGA normal density screen (which
            // is 480 pixels on its long side).
            mScreenLayout = Configuration_SCREENLAYOUT_SIZE_SMALL
                    | Configuration_SCREENLAYOUT_LONG_NO;
        }
        else {
            // What size is this screen screen?
            if (longSize >= 800 && shortSize >= 600) {
                // SVGA or larger screens at medium density are the point
                // at which we consider it to be an extra large screen.
                mScreenLayout = Configuration_SCREENLAYOUT_SIZE_XLARGE;
            }
            else if (longSize >= 530 && shortSize >= 400) {
                // SVGA or larger screens at high density are the point
                // at which we consider it to be a large screen.
                mScreenLayout = Configuration_SCREENLAYOUT_SIZE_LARGE;
            }
            else {
                mScreenLayout = Configuration_SCREENLAYOUT_SIZE_NORMAL;
            }

            // If this screen is wider than normal HVGA, or taller
            // than FWVGA, then for old apps we want to run in size
            // compatibility mode.
            if (shortSize > 321 || longSize > 570) {
                mScreenLayout |= Configuration_SCREENLAYOUT_COMPAT_NEEDED;
            }

            // Is this a long screen?
            if (((longSize * 3) / 5) >= (shortSize-1)) {
                // Anything wider than WVGA (5:3) is considering to be long.
                mScreenLayout |= Configuration_SCREENLAYOUT_LONG_YES;
            }
            else {
                mScreenLayout |= Configuration_SCREENLAYOUT_LONG_NO;
            }
        }
    }
    config->SetScreenLayout(mScreenLayout);

    config->SetKeyboardHidden(Configuration_KEYBOARDHIDDEN_NO);
    config->SetHardKeyboardHidden(Configuration_HARDKEYBOARDHIDDEN_NO);
    mPolicy->AdjustConfigurationLw(config);
    return TRUE;
}

/**
 * Must be called with the main window manager lock held.
 */
void CWindowManagerService::SetHoldScreenLocked(
    /* [in] */ Boolean holding)
{
//    Boolean state = mHoldingScreenWakeLock->IsHeld();
//    if (holding != state) {
//        if (holding) {
//            mHoldingScreenWakeLock->Acquire();
//        } else {
//            mPolicy->ScreenOnStoppedLw();
//            mHoldingScreenWakeLock->Release();
//        }
//    }
}

ECode CWindowManagerService::RequestAnimationLocked(
        /* [in] */ Millisecond64 delay)
{
    if (!mAnimationPending) {
        mAnimationPending = TRUE;
        ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
        pHandlerFunc = &CWindowManagerService::HandleAnimate;

        SendMessageDelayed(*(Handle32*)&pHandlerFunc, NULL, delay);
    }

    return NOERROR;
}

/**
 * Have the surface flinger show a surface, robustly dealing with
 * error conditions.  In particular, if there is not enough memory
 * to show the surface, then we will try to get rid of other surfaces
 * in order to succeed.
 *
 * @return Returns true if the surface was successfully shown.
 */
Boolean CWindowManagerService::ShowSurfaceRobustlyLocked(
    /* [in] */ WindowState* win)
{
    if (win->mSurface != NULL) {
        win->mSurfaceShown = TRUE;
        ECode ec = win->mSurface->Show();
        if (FAILED(ec)) {
//            Slog.w(TAG, "Failure showing surface " + win.mSurface + " in " + win);
            ReclaimSomeSurfaceMemoryLocked(win, "show");
            return FALSE;
        }
        if (win->mTurnOnScreen) {
//            if (DEBUG_VISIBILITY) Slog.v(TAG,
//                    "Show surface turning screen on: " + win);
            win->mTurnOnScreen = FALSE;
            mTurnOnScreen = TRUE;
        }
    }
    return TRUE;
}

void CWindowManagerService::ReclaimSomeSurfaceMemoryLocked(
    /* [in] */ WindowState* win,
    /* [in] */ const char* operation)
{
    AutoPtr<ISurface> surface = win->mSurface;

//    EventLog.writeEvent(EventLogTags.WM_NO_SURFACE_MEMORY, win.toString(),
//            win.mSession.mPid, operation);
//??????????how to deal with???????
//    if (mForceRemoves == NULL) {
//        mForceRemoves = new ArrayList<WindowState>();
//    }

//    long callingIdentity = Binder.clearCallingIdentity();
//    try {
    // There was some problem...   first, do a sanity check of the
    // window list to make sure we haven't left any dangling surfaces
    // around.
    List<AutoPtr<WindowState> >::Iterator it;
    Boolean leakedSurface = FALSE;
//    Slog.i(TAG, "Out of memory for surface!  Looking for leaks...");
    //?????????????have question, i--&N--????????????
    for (it = mWindows.Begin(); it != mWindows.End(); ++it) {
        AutoPtr<WindowState> ws = *it;
        if (ws->mSurface != NULL) {
            HashSet<AutoPtr<IWindowSession> >::Iterator wsIt =
                    mSessions.Find(ws->mSession);
            if (wsIt == mSessions.End()) {
//                Slog.w(TAG, "LEAKED SURFACE (session doesn't exist): "
//                        + ws + " surface=" + ws.mSurface
//                        + " token=" + win.mToken
//                        + " pid=" + ws.mSession.mPid
//                        + " uid=" + ws.mSession.mUid);
                ws->mSurface->Destroy();
                ws->mSurfaceShown = FALSE;
                ws->mSurface = NULL;
                mForceRemoves->PushBack(ws);
//                i--;
//                N--;
                leakedSurface = TRUE;
            }
            else if (win->mAppToken != NULL && win->mAppToken->mClientHidden) {
//                Slog.w(TAG, "LEAKED SURFACE (app token hidden): "
//                        + ws + " surface=" + ws.mSurface
//                        + " token=" + win.mAppToken);
                ws->mSurface->Destroy();
                ws->mSurfaceShown = FALSE;
                ws->mSurface = NULL;
                leakedSurface = TRUE;
            }
        }
    }

    Boolean killedApps = FALSE;
    if (!leakedSurface) {
//        Slog.w(TAG, "No leaked surfaces; killing applicatons!");
//        SparseIntArray pidCandidates = new SparseIntArray();
        for (it = mWindows.Begin(); it != mWindows.End(); ++it) {
            AutoPtr<WindowState> ws = *it;
            if (ws->mSurface != NULL) {
//                pidCandidates.append(ws.mSession.mPid, ws.mSession.mPid);
            }
        }
//        if (pidCandidates.size() > 0) {
//            int[] pids = new int[pidCandidates.size()];
//            for (int i=0; i<pids.length; i++) {
//                pids[i] = pidCandidates.keyAt(i);
//            }
//            try {
//                if (mActivityManager.killPids(pids, "Free memory")) {
//                    killedApps = true;
//                }
//            } catch (RemoteException e) {
//            }
//        }
    }

    if (leakedSurface || killedApps) {
        // We managed to reclaim some memory, so get rid of the trouble
        // surface and ask the app to request another one.
//        Slog.w(TAG, "Looks like we have reclaimed some memory, clearing surface for retry.");
        if (surface != NULL) {
            surface->Destroy();
            win->mSurfaceShown = FALSE;
            win->mSurface = NULL;
        }

//        try {
            win->mClient->DispatchGetNewSurface();
//        } catch (RemoteException e) {
//        }
    }
//    } finally {
//        Binder.restoreCallingIdentity(callingIdentity);
//    }
}

void CWindowManagerService::DumpAppTokensLocked()
{
    List<AppWindowToken*>::ReverseIterator rit;
    for (rit = mAppTokens.RBegin(); rit != mAppTokens.REnd(); ++rit) {
//        Slog.v(TAG, "  #" + i + ": " + mAppTokens.get(i).token);
    }
}

void CWindowManagerService::DumpWindowsLocked()
{
    List<AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = mWindows.RBegin(); rit != mWindows.REnd(); ++rit) {
//        Slog.v(TAG, "  #" + i + ": " + mAppTokens.get(i).token);
    }
}

Boolean CWindowManagerService::ShouldAllowDisableKeyguard()
{
    // We fail safe and prevent disabling keyguard in the unlikely event this gets
    // called before DevicePolicyManagerService has started.
    if (mAllowDisableKeyguard == ALLOW_DISABLE_UNKNOWN) {
//        DevicePolicyManager dpm = (DevicePolicyManager) mContext.getSystemService(
//                Context.DEVICE_POLICY_SERVICE);
//        if (dpm != NULL) {
//            mAllowDisableKeyguard = dpm.getPasswordQuality(NULL)
//                    == DevicePolicyManager.PASSWORD_QUALITY_UNSPECIFIED ?
//                            ALLOW_DISABLE_YES : ALLOW_DISABLE_NO;
//        }
    }
    return mAllowDisableKeyguard == ALLOW_DISABLE_YES;
}

Float CWindowManagerService::FixScale(
    /* [in] */ Float scale)
{
    if (scale < 0) scale = 0;
    else if (scale > 20) scale = 20;
    return Math::Abs(scale);
}

AutoPtr<CWindowManagerService::WindowState>
CWindowManagerService::GetFocusedWindow()
{
    Mutex::Autolock lock(mWindowMapLock);
    return GetFocusedWindowLocked();
}

AutoPtr<CWindowManagerService::WindowState>
CWindowManagerService::GetFocusedWindowLocked()
{
    return mCurrentFocus;
}

Boolean CWindowManagerService::DetectSafeMode()
{
    mPolicy->DetectSafeMode(&mSafeMode);
    return mSafeMode;
}

void CWindowManagerService::SystemReady()
{
    mPolicy->SystemReady();
}

CWindowManagerService::InputMonitor::InputMonitor(
    /* [in] */ CWindowManagerService* service) :
    mService(service),
    mInputDispatchFrozen(FALSE),
    mInputDispatchEnabled(TRUE)
{
#ifdef _linux
    mTempInputWindows = new InputWindowList();
    mTempInputApplication = new InputApplication();
#endif
}

CWindowManagerService::InputMonitor::~InputMonitor()
{
    //todo:
    //release mTempInputWindows and mTempInputApplication
    delete mTempInputWindows;
    delete mTempInputApplication;
}

/* Notifies the window manager about a broken input channel.
 *
 * Called by the InputManager.
 */
ECode CWindowManagerService::InputMonitor::NotifyInputChannelBroken(
    /* [in] */ IInputChannel* inputChannel)
{
    Mutex::Autolock lock(mService->mWindowMapLock);

    AutoPtr<CWindowManagerService::WindowState> windowState = GetWindowStateForInputChannelLocked(inputChannel);
    if (windowState == NULL) {
        return NOERROR; // irrelevant
    }

//    Slogger::I(TAG, StringBuffer("WINDOW DIED ") + windowState);
    mService->RemoveWindowLocked(windowState->mSession, windowState);

    return NOERROR;
}

ECode CWindowManagerService::InputMonitor::NotifyANR(
    /* [in] */ void* token,
    /* [in] */ IInputChannel* inputChannel,
    /* [out] */ Int64* timeout)
{
    AppWindowToken* appWindowToken = NULL;
    if (inputChannel != NULL) {
        Mutex::Autolock lock(mService->mWindowMapLock);

        AutoPtr<CWindowManagerService::WindowState> windowState =
                GetWindowStateForInputChannelLocked(inputChannel);
        if (windowState != NULL) {
//            Slog.i(TAG, "Input event dispatching timed out sending to "
//                    + windowState.mAttrs.getTitle());
            appWindowToken = windowState->mAppToken;
        }
    }

    if (appWindowToken == NULL && token != NULL) {
        appWindowToken = (AppWindowToken*)token;
        Slogger::I(TAG, StringBuffer("Input event dispatching timed out sending to application ")
                + appWindowToken->mStringName);
    }

    if (appWindowToken != NULL && appWindowToken->mAppToken != NULL) {
        // Notify the activity manager about the timeout and let it decide whether
        // to abort dispatching or keep waiting.
        Boolean abort;
        appWindowToken->mAppToken->KeyDispatchingTimedOut(&abort);
        if (!abort) {
            // The activity manager declined to abort dispatching.
            // Wait a bit longer and timeout again later.
            *timeout = appWindowToken->mInputDispatchingTimeoutNanos;
            return NOERROR;
        }
    }
    *timeout = 0; // abort dispatching
    return NOERROR;
}

AutoPtr<CWindowManagerService::WindowState>
CWindowManagerService::InputMonitor::GetWindowStateForInputChannel(
    /* [in] */ IInputChannel* inputChannel)
{
    Mutex::Autolock lock(mService->mWindowMapLock);
    return GetWindowStateForInputChannelLocked(inputChannel);
}

AutoPtr<CWindowManagerService::WindowState>
CWindowManagerService::InputMonitor::GetWindowStateForInputChannelLocked(
    /* [in] */ IInputChannel* inputChannel)
{
    List<AutoPtr<CWindowManagerService::WindowState> >::Iterator it =
            (mService->mWindows).Begin();
    for (; it != (mService->mWindows).End(); ++it) {
        AutoPtr<CWindowManagerService::WindowState> windowState = *it;
        if (windowState->mInputChannel.Get() == inputChannel) {
            return windowState;
        }
    }

    return NULL;
}

/* Updates the cached window information provided to the input dispatcher. */
void CWindowManagerService::InputMonitor::UpdateInputWindowsLw()
{
    // Populate the input window list with information about all of the windows that
    // could potentially receive input.
    // As an optimization, we could try to prune the list of windows but this turns
    // out to be difficult because only the native code knows for sure which window
    // currently has touch focus.
    List<AutoPtr<CWindowManagerService::WindowState> >::ReverseIterator rit =
            (mService->mWindows).RBegin();
    for (; rit != (mService->mWindows).REnd(); ++rit) {
        AutoPtr<CWindowManagerService::WindowState> child = *rit;
        if (child->mInputChannel == NULL || child->mRemoved) {
            // Skip this window because it cannot possibly receive input.
            continue;
        }

        Int32 flags, type;
        child->mAttrs->GetFlags(&flags);
        child->mAttrs->GetType(&type);

        Boolean hasFocus = (child == mInputFocus);
        Boolean isVisible = child->IsVisibleLw();
        Boolean hasWallpaper = (child == mService->mWallpaperTarget)
                && (type != WindowManagerLayoutParams_TYPE_KEYGUARD);
        //printf("====== FILE: %s, LINE: %d, hasFocus: %d, isVisible: %d, hasWallpaper: %d ======\n",
        //        __FILE__, __LINE__, hasFocus, isVisible, hasWallpaper);

        // Add a window to our list of input windows.
        InputWindow* inputWindow = mTempInputWindows->Add();
        inputWindow->mInputChannel = child->mInputChannel;
        child->GetDescription(&(inputWindow->mName));
        inputWindow->mLayoutParamsFlags = flags;
        inputWindow->mLayoutParamsType = type;
        inputWindow->mDispatchingTimeoutNanos = child->GetInputDispatchingTimeoutNanos();
        inputWindow->mVisible = isVisible;
        inputWindow->mCanReceiveKeys = child->CanReceiveKeys();
        inputWindow->mHasFocus = hasFocus;
        inputWindow->mHasWallpaper = hasWallpaper;
        inputWindow->mPaused = child->mAppToken != NULL ? child->mAppToken->mPaused : FALSE;
        inputWindow->mLayer = child->mLayer;
        inputWindow->mOwnerPid = child->mSession->mPid;
        inputWindow->mOwnerUid = child->mSession->mUid;

        IRect* frame = child->mFrame;
        Int32 frameLeft, frameTop, frameRight, frameBottom;
        frame->GetLeft(&frameLeft);
        frame->GetTop(&frameTop);
        frame->GetRight(&frameRight);
        frame->GetBottom(&frameBottom);
        inputWindow->mFrameLeft = frameLeft;
        inputWindow->mFrameTop = frameTop;
        inputWindow->mFrameRight = frameRight;
        inputWindow->mFrameBottom = frameBottom;


        IRect* visibleFrame = child->mVisibleFrame;
        visibleFrame->GetLeft(&inputWindow->mVisibleFrameLeft);
        visibleFrame->GetTop(&inputWindow->mVisibleFrameTop);
        visibleFrame->GetRight(&inputWindow->mVisibleFrameRight);
        visibleFrame->GetBottom(&inputWindow->mVisibleFrameBottom);

        switch (child->mTouchableInsets) {
            default:
            case ViewTreeObserver_InternalInsetsInfo_TOUCHABLE_INSETS_FRAME:
            {
                inputWindow->mTouchableAreaLeft = frameLeft;
                inputWindow->mTouchableAreaTop = frameTop;
                inputWindow->mTouchableAreaRight = frameRight;
                inputWindow->mTouchableAreaBottom = frameBottom;
                break;
            }

            case ViewTreeObserver_InternalInsetsInfo_TOUCHABLE_INSETS_CONTENT:
            {
                IRect* inset = child->mGivenContentInsets;
                Int32 insetLeft, insetTop, insetRight, insetBottom;
                inset->GetLeft(&insetLeft);
                inset->GetTop(&insetTop);
                inset->GetRight(&insetRight);
                inset->GetBottom(&insetBottom);
                inputWindow->mTouchableAreaLeft = frameLeft + insetLeft;
                inputWindow->mTouchableAreaTop = frameTop + insetTop;
                inputWindow->mTouchableAreaRight = frameRight - insetRight;
                inputWindow->mTouchableAreaBottom = frameBottom - insetBottom;
                break;
            }

            case ViewTreeObserver_InternalInsetsInfo_TOUCHABLE_INSETS_VISIBLE:
            {
                IRect* inset = child->mGivenVisibleInsets;
                Int32 insetLeft, insetTop, insetRight, insetBottom;
                inset->GetLeft(&insetLeft);
                inset->GetTop(&insetTop);
                inset->GetRight(&insetRight);
                inset->GetBottom(&insetBottom);
                inputWindow->mTouchableAreaLeft = frameLeft + insetLeft;
                inputWindow->mTouchableAreaTop = frameTop + insetTop;
                inputWindow->mTouchableAreaRight = frameRight - insetRight;
                inputWindow->mTouchableAreaBottom = frameBottom - insetBottom;
                break;
            }
        }
    }

    // Send windows to native code.
    assert(SUCCEEDED(mService->mInputManager->SetInputWindows(
            mTempInputWindows->GetNullTerminatedArray())));

    // Clear the list in preparation for the next round.
    // Also avoids keeping InputChannel objects referenced unnecessarily.
    mTempInputWindows->Clear();
}

/* Notifies that the lid switch changed state. */
ECode CWindowManagerService::InputMonitor::NotifyLidSwitchChanged(
    /* [in] */ Millisecond64 whenNanos,
    /* [in] */ Boolean lidOpen)
{
    return mService->mPolicy->NotifyLidSwitchChanged(whenNanos, lidOpen);
}

/* Provides an opportunity for the window manager policy to intercept early key
 * processing as soon as the key has been read from the device. */
ECode CWindowManagerService::InputMonitor::InterceptKeyBeforeQueueing(
    /* [in] */ Millisecond64 whenNanos,
    /* [in] */ Int32 action,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 scanCode,
    /* [in] */ Int32 policyFlags,
    /* [in] */ Boolean isScreenOn,
    /* [out] */ Int32* result)
{
    return mService->mPolicy->InterceptKeyBeforeQueueing(whenNanos, action, flags,
            keyCode, scanCode, policyFlags, isScreenOn, result);
}

/* Provides an opportunity for the window manager policy to process a key before
 * ordinary dispatch. */
ECode CWindowManagerService::InputMonitor::InterceptKeyBeforeDispatching(
    /* [in] */ IInputChannel* focus,
    /* [in] */ Int32 action,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 scanCode,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 policyFlags,
    /* [out] */ Boolean* result)
{
    AutoPtr<CWindowManagerService::WindowState> windowState =
            GetWindowStateForInputChannel(focus);
    return mService->mPolicy->InterceptKeyBeforeDispatching(windowState, action, flags,
            keyCode, scanCode, metaState, repeatCount, policyFlags, result);
}

/* Called when the current input focus changes.
 * Layer assignment is assumed to be complete by the time this is called.
 */
void CWindowManagerService::InputMonitor::SetInputFocusLw(
    /* [in] */ WindowState* newWindow)
{
//    if (DEBUG_INPUT) {
//        Slog.d(TAG, "Input focus has changed to " + newWindow);
//    }

    if (newWindow != mInputFocus) {
        if (newWindow != NULL && newWindow->CanReceiveKeys()) {
            // Displaying a window implicitly causes dispatching to be unpaused.
            // This is to protect against bugs if someone pauses dispatching but
            // forgets to resume.
            newWindow->mToken->mPaused = FALSE;
        }

        mInputFocus = newWindow;
        UpdateInputWindowsLw();
    }
}

void CWindowManagerService::InputMonitor::SetFocusedAppLw(
    /* [in] */ AppWindowToken* newApp)
{
    // Focused app has changed.
    if (newApp == NULL) {
        mService->mInputManager->SetFocusedApplication(NULL);
    }
    else {
        newApp->GetDescription(&(mTempInputApplication->mName));
        mTempInputApplication->mDispatchingTimeoutNanos =
                newApp->mInputDispatchingTimeoutNanos;
        mTempInputApplication->mToken = newApp;

        mService->mInputManager->SetFocusedApplication(mTempInputApplication);
    }
}

void CWindowManagerService::InputMonitor::PauseDispatchingLw(
    /* [in] */ WindowToken* window)
{
    if (!window->mPaused) {
//        if (DEBUG_INPUT) {
//            Slog.v(TAG, "Pausing WindowToken " + window);
//        }

        window->mPaused = TRUE;
        UpdateInputWindowsLw();
    }
}

void CWindowManagerService::InputMonitor::ResumeDispatchingLw(
    /* [in] */ WindowToken* window)
{
    if (window->mPaused) {
//        if (DEBUG_INPUT) {
//            Slog.v(TAG, "Resuming WindowToken " + window);
//        }

        window->mPaused = FALSE;
        UpdateInputWindowsLw();
    }
}

void CWindowManagerService::InputMonitor::FreezeInputDispatchingLw()
{
    if (!mInputDispatchFrozen) {
//        if (DEBUG_INPUT) {
//            Slog.v(TAG, "Freezing input dispatching");
//        }

        mInputDispatchFrozen = TRUE;
        UpdateInputDispatchModeLw();
    }
}

void CWindowManagerService::InputMonitor::ThawInputDispatchingLw()
{
    if (mInputDispatchFrozen) {
//        if (DEBUG_INPUT) {
//            Slog.v(TAG, "Thawing input dispatching");
//        }

        mInputDispatchFrozen = FALSE;
        UpdateInputDispatchModeLw();
    }
}

void CWindowManagerService::InputMonitor::SetEventDispatchingLw(
    /* [in] */ Boolean enabled)
{
    if (mInputDispatchEnabled != enabled) {
//        if (DEBUG_INPUT) {
//            Slog.v(TAG, "Setting event dispatching to " + enabled);
//        }

        mInputDispatchEnabled = enabled;
        UpdateInputDispatchModeLw();
    }
}

void CWindowManagerService::InputMonitor::UpdateInputDispatchModeLw()
{
    mService->mInputManager->SetInputDispatchMode(
            mInputDispatchEnabled, mInputDispatchFrozen);
}

void CWindowManagerService::AppWindowToken::SetAnimation(
    /* [in] */ IAnimation* anim)
{
    //if (localLOGV) Slog.v(
    //    TAG, "Setting animation in " + this + ": " + anim);

    mAnimation = anim;
    mAnimating = FALSE;
    anim->RestrictDuration(MAX_ANIMATION_DURATION);
    anim->ScaleCurrentDuration(mWMService->mTransitionAnimationScale);
    Int32 zorder;
    anim->GetZAdjustment(&zorder);
    Int32 adj = 0;
    if (zorder == Animation_ZORDER_TOP) {
        adj = TYPE_LAYER_OFFSET;
    }
    else if (zorder == Animation_ZORDER_BOTTOM) {
        adj = -TYPE_LAYER_OFFSET;
    }

    if (mAnimLayerAdjustment != adj) {
        mAnimLayerAdjustment = adj;
        UpdateLayers();
    }
}

void CWindowManagerService::AppWindowToken::SetDummyAnimation()
{
    if (mAnimation == NULL) {
//        if (localLOGV) Slog.v(
//            TAG, "Setting dummy animation in " + this);
        mAnimation = sDummyAnimation;
    }
}

void CWindowManagerService::AppWindowToken::ClearAnimation()
{
    if (mAnimation != NULL) {
        mAnimation = NULL;
        mAnimating = TRUE;
    }
}

void CWindowManagerService::AppWindowToken::UpdateLayers()
{
    Int32 adj = mAnimLayerAdjustment;
    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mAllAppWindows.Begin(); it != mAllAppWindows.End(); ++it) {
        AutoPtr<WindowState> w = *it;
        w->mAnimLayer = w->mLayer + adj;
//        if (DEBUG_LAYERS) Slog.v(TAG, "Updating layer " + w + ": "
//                + w.mAnimLayer);
        if (w == mWMService->mInputMethodTarget) {
            mWMService->SetInputMethodAnimLayerAdjustment(adj);
        }
        if (w == mWMService->mWallpaperTarget &&
            mWMService->mLowerWallpaperTarget == NULL) {
            mWMService->SetWallpaperAnimLayerAdjustmentLocked(adj);
        }
    }
}

void CWindowManagerService::AppWindowToken::SendAppVisibilityToClients()
{
    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mAllAppWindows.Begin(); it != mAllAppWindows.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        if (win == mStartingWindow && mClientHidden) {
            // Don't hide the starting window.
            continue;
        }
//        try {
//        if (DEBUG_VISIBILITY) Slog.v(TAG,
//                "Setting visibility of " + win + ": " + (!clientHidden));
        win->mClient->DispatchAppVisibility(!mClientHidden);
//        } catch (RemoteException e) {
//        }
    }
}

void CWindowManagerService::AppWindowToken::ShowAllWindowsLocked()
{
    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mAllAppWindows.Begin(); it != mAllAppWindows.End(); ++it) {
        AutoPtr<WindowState> w = *it;
//        if (DEBUG_VISIBILITY) Slog.v(TAG,
//                "performing show on: " + w);
        w->PerformShowLocked();
    }
}

// This must be called while inside a transaction.
Boolean CWindowManagerService::AppWindowToken::StepAnimationLocked(
    /* [in] */ Millisecond64 currentTime,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    Boolean isOn;
    mWMService->mPolicy->IsScreenOn(&isOn);
    if (!mWMService->mDisplayFrozen && isOn) {
        // We will run animations as long as the display isn't frozen.

        if (mAnimation == sDummyAnimation) {
            // This guy is going to animate, but not yet.  For now count
            // it as not animating for purposes of scheduling transactions;
            // when it is really time to animate, this will be set to
            // a real animation and the next call will execute normally.
            return FALSE;
        }

        if ((mAllDrawn || mAnimating || mStartingDisplayed) && mAnimation != NULL) {
            if (!mAnimating) {
//                if (DEBUG_ANIM) Slog.v(
//                    TAG, "Starting animation in " + this +
//                    " @ " + currentTime + ": dw=" + dw + " dh=" + dh
//                    + " scale=" + mTransitionAnimationScale
//                    + " allDrawn=" + allDrawn + " animating=" + animating);
                mAnimation->Initialize(dw, dh, dw, dh);
                mAnimation->SetStartTime(currentTime);
                mAnimating = TRUE;
            }
            mTransformation->Clear();
            Boolean more;
            mAnimation->GetTransformation(currentTime, mTransformation, &more);
//            if (DEBUG_ANIM) Slog.v(
//                TAG, "Stepped animation in " + this +
//                ": more=" + more + ", xform=" + transformation);
            if (more) {
                // we're done!
                mHasTransformation = TRUE;
                return TRUE;
            }
//            if (DEBUG_ANIM) Slog.v(
//                TAG, "Finished animation in " + this +
//                " @ " + currentTime);
            mAnimation = NULL;
        }
    }
    else if (mAnimation != NULL) {
        // If the display is frozen, and there is a pending animation,
        // clear it and make sure we run the cleanup code.
        mAnimating = TRUE;
        mAnimation = NULL;
    }

    mHasTransformation = FALSE;

    if (!mAnimating) {
        return FALSE;
    }

    ClearAnimation();
    mAnimating = FALSE;
    if (mWMService->mInputMethodTarget != NULL &&
        mWMService->mInputMethodTarget->mAppToken == this) {
        mWMService->MoveInputMethodWindowsIfNeededLocked(TRUE);
    }

//    if (DEBUG_ANIM) Slog.v(
//            TAG, "Animation done in " + this
//            + ": reportedVisible=" + reportedVisible);

    mTransformation->Clear();
    if (mAnimLayerAdjustment != 0) {
        mAnimLayerAdjustment = 0;
        UpdateLayers();
    }

    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mWindows.Begin(); it != mWindows.End(); ++it) {
        (*it)->FinishExit();
    }
    UpdateReportedVisibilityLocked();

    return FALSE;
}

void CWindowManagerService::AppWindowToken::UpdateReportedVisibilityLocked()
{
    if (mAppToken == NULL) {
        return;
    }

    Int32 numInteresting = 0;
    Int32 numVisible = 0;
    Boolean nowGone = TRUE;

//    if (DEBUG_VISIBILITY) Slog.v(TAG, "Update reported visibility: " + this);
    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mAllAppWindows.Begin(); it != mAllAppWindows.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        Int32 winType;
        win->mAttrs->GetType(&winType);
        if (win == mStartingWindow || win->mAppFreezing
                || win->mViewVisibility != View_VISIBLE
                || winType == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING) {
            continue;
        }
//        if (DEBUG_VISIBILITY) {
//            Slog.v(TAG, "Win " + win + ": isDrawn="
//                    + win.isDrawnLw()
//                    + ", isAnimating=" + win.isAnimating());
//            if (!win.isDrawnLw()) {
//                Slog.v(TAG, "Not displayed: s=" + win.mSurface
//                        + " pv=" + win.mPolicyVisibility
//                        + " dp=" + win.mDrawPending
//                        + " cdp=" + win.mCommitDrawPending
//                        + " ah=" + win.mAttachedHidden
//                        + " th="
//                        + (win.mAppToken != NULL
//                                ? win.mAppToken.hiddenRequested : false)
//                        + " a=" + win.mAnimating);
//            }
//        }
        numInteresting++;
        if (win->IsDrawnLw()) {
            if (!win->IsAnimating()) {
                numVisible++;
            }
            nowGone = FALSE;
        }
        else if (win->IsAnimating()) {
            nowGone = FALSE;
        }
    }

    Boolean nowVisible = numInteresting > 0 && numVisible >= numInteresting;
//    if (DEBUG_VISIBILITY) Slog.v(TAG, "VIS " + this + ": interesting="
//            + numInteresting + " visible=" + numVisible);
    if (nowVisible != mReportedVisible) {
//        if (DEBUG_VISIBILITY) Slog.v(
//                TAG, "Visibility changed in " + this
//                + ": vis=" + nowVisible);
        mReportedVisible = nowVisible;

        ECode (STDCALL CWindowManagerService::*pHandlerFunc)(
            Boolean, Boolean, AppWindowToken*);
        pHandlerFunc = &CWindowManagerService::HandleReportAppTokenWindows;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteBoolean(nowVisible);
        params->WriteBoolean(nowGone);
        params->WriteInt32((Handle32)this);
        mWMService->SendMessage(*(Handle32*)&pHandlerFunc, params);
    }
}

AutoPtr<CWindowManagerService::WindowState> CWindowManagerService::AppWindowToken::
FindMainWindow()
{
    List<AutoPtr<WindowState> >::ReverseIterator rit = mWindows.RBegin();
    while (rit != mWindows.REnd()) {
        AutoPtr<WindowState> win = *rit;
        Int32 winType;
        win->mAttrs->GetType(&winType);
        if (winType == WindowManagerLayoutParams_TYPE_BASE_APPLICATION
                || winType == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING) {
            return win;
        }
        ++rit;
    }
    return NULL;
}

void CWindowManagerService::AppWindowToken::GetDescription(
    /* [out] */ String* des)
{
    assert(des != NULL);
    if (mStringName.IsNull()) {
        StringBuffer sb("AppWindowToken{");
//        sb.append(Integer.toHexString(System.identityHashCode(this)));
        sb += " token=";
        String tmp;
        if (mToken)
            mToken->GetDescription((String*)&tmp);
        sb += tmp;
        sb += "}";
        mStringName = (const char*)sb;
    }
    *des =  mStringName;
}

CWindowManagerService::WindowState::WindowState(
    /* [in] */ CWindowManagerService* wmService,
    /* [in] */ CWindowSession* session,
    /* [in] */ IInnerWindow* client,
    /* [in] */ WindowToken*  token,
    /* [in] */ WindowState* attachedWindow,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Int32 viewVisibility) :
    mWMService(wmService),
    mSession(session),
    mClient(client),
    mToken(token),
    mRootToken(NULL),
    mAppToken(NULL),
    mTargetAppToken(NULL),
    mLayoutAttached(FALSE),
    mIsImWindow(FALSE),
    mIsWallpaper(FALSE),
    mIsFloatingLayer(FALSE),
    mViewVisibility(viewVisibility),
    mPolicyVisibility(TRUE),
    mPolicyVisibilityAfterAnim(TRUE),
    mAppFreezing(FALSE),
    mReportDestroySurface(FALSE),
    mSurfacePendingDestroy(FALSE),
    mAttachedHidden(FALSE),
    mLastHidden(FALSE),
    mWallpaperVisible(FALSE),
    mHaveFrame(FALSE),
    mObscured(FALSE),
    mTurnOnScreen(FALSE),
    mLayoutSeq(-1),
    mSurfaceResized(FALSE),
    mVisibleInsetsChanged(FALSE),
    mContentInsetsChanged(FALSE),
    mGivenInsetsPending(FALSE),
    mDsDx(1),
    mDtDx(0),
    mDsDy(0),
    mDtDy(1),
    mLastDsDx(1),
    mLastDtDx(0),
    mLastDsDy(0),
    mLastDtDy(1),
    mHScale(1),
    mVScale(1),
    mLastHScale(1),
    mLastVScale(1),
    mShownAlpha(1),
    mAlpha(1),
    mLastAlpha(1),
    mEnterAnimationPending(FALSE),
    mAnimating(FALSE),
    mLocalAnimating(FALSE),
    mAnimationIsEntrance(FALSE),
    mHasTransformation(FALSE),
    mHasLocalTransformation(FALSE),
    mWallpaperX(-1),
    mWallpaperY(-1),
    mWallpaperXStep(-1),
    mWallpaperYStep(-1),
    mRelayoutCalled(FALSE),
    mDrawPending(FALSE),
    mCommitDrawPending(FALSE),
    mReadyToShow(FALSE),
    mHasDrawn(FALSE),
    mExiting(FALSE),
    mDestroying(FALSE),
    mRemoveOnExit(FALSE),
    mOrientationChanging(FALSE),
    mRemoved(FALSE),
    mSurfaceShown(FALSE),
    mWasPaused(FALSE)
{
    assert(SUCCEEDED(CRect::New((IRect**)&mShownFrame)));
    assert(SUCCEEDED(CRect::New((IRect**)&mLastShownFrame)));
    assert(SUCCEEDED(CRect::New((IRect**)&mVisibleInsets)));
    assert(SUCCEEDED(CRect::New((IRect**)&mLastVisibleInsets)));
    assert(SUCCEEDED(CRect::New((IRect**)&mContentInsets)));
    assert(SUCCEEDED(CRect::New((IRect**)&mLastContentInsets)));
    assert(SUCCEEDED(CRect::New((IRect**)&mGivenContentInsets)));
    assert(SUCCEEDED(CRect::New((IRect**)&mGivenVisibleInsets)));
    assert(SUCCEEDED(CRect::New((IRect**)&mFrame)));
    assert(SUCCEEDED(CRect::New((IRect**)&mLastFrame)));
    assert(SUCCEEDED(CRect::New((IRect**)&mContainingFrame)));
    assert(SUCCEEDED(CRect::New((IRect**)&mDisplayFrame)));
    assert(SUCCEEDED(CRect::New((IRect**)&mContentFrame)));
    assert(SUCCEEDED(CRect::New((IRect**)&mVisibleFrame)));

    assert(SUCCEEDED(CWindowManagerLayoutParams::New(
            (IWindowManagerLayoutParams**)&mAttrs)));
    Int32 changes;
    mAttrs->CopyFrom(attrs, &changes);

    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTransformation));

    ASSERT_SUCCEEDED(CMatrix::New((IMatrix**)&mTmpMatrix));
//    DeathRecipient deathRecipient = new DeathRecipient();
    mAttrs->GetAlpha(&mAlpha);
//    if (localLOGV) Slog.v(
//        TAG, "Window " + this + " client=" + c.asBinder()
//        + " token=" + token + " (" + mAttrs.token + ")");
//    try {
//        c.asBinder().linkToDeath(deathRecipient, 0);
//    } catch (RemoteException e) {
//        mDeathRecipient = NULL;
//        mAttachedWindow = NULL;
//        mLayoutAttached = false;
//        mIsImWindow = false;
//        mIsWallpaper = false;
//        mIsFloatingLayer = false;
//        mBaseLayer = 0;
//        mSubLayer = 0;
//        return;
//    }
//    mDeathRecipient = deathRecipient;

    Int32 type;
    mAttrs->GetType(&type);
    if ((type >= WindowManagerLayoutParams_FIRST_SUB_WINDOW
        && type <= WindowManagerLayoutParams_LAST_SUB_WINDOW)) {
        // The multiplier here is to reserve space for multiple
        // windows in the same type layer.
        Int32 layer, attachedWinType;
        attachedWindow->mAttrs->GetType(&attachedWinType);
        mWMService->mPolicy->WindowTypeToLayerLw(attachedWinType, &layer);
        mBaseLayer = layer * TYPE_LAYER_MULTIPLIER + TYPE_LAYER_OFFSET;
        mWMService->mPolicy->SubWindowTypeToLayerLw(type, &mSubLayer);
        mAttachedWindow = attachedWindow;
        mAttachedWindow->mChildWindows.PushBack(this);
        mLayoutAttached = type !=
            WindowManagerLayoutParams_TYPE_APPLICATION_ATTACHED_DIALOG;
        mIsImWindow = attachedWinType == WindowManagerLayoutParams_TYPE_INPUT_METHOD
            || attachedWinType == WindowManagerLayoutParams_TYPE_INPUT_METHOD_DIALOG;
        mIsWallpaper = attachedWinType == WindowManagerLayoutParams_TYPE_WALLPAPER;
        mIsFloatingLayer = mIsImWindow || mIsWallpaper;
    }
    else {
        // The multiplier here is to reserve space for multiple
        // windows in the same type layer.
        Int32 layer;
        mWMService->mPolicy->WindowTypeToLayerLw(type, &layer);
        mBaseLayer = layer * TYPE_LAYER_MULTIPLIER + TYPE_LAYER_OFFSET;
        mSubLayer = 0;
        mAttachedWindow = NULL;
        mLayoutAttached = FALSE;
        mIsImWindow = type == WindowManagerLayoutParams_TYPE_INPUT_METHOD
            || type == WindowManagerLayoutParams_TYPE_INPUT_METHOD_DIALOG;
        mIsWallpaper = type == WindowManagerLayoutParams_TYPE_WALLPAPER;
        mIsFloatingLayer = mIsImWindow || mIsWallpaper;
    }

    WindowState* appWin = this;
    while (appWin->mAttachedWindow != NULL) {
        appWin = mAttachedWindow;
    }
    WindowToken* appToken = appWin->mToken;
    while (appToken->mAppWindowToken == NULL) {
        WindowToken* parent = NULL;
        HashMap<AutoPtr<IBinder>, WindowToken*>::Iterator it = mWMService->mTokenMap.End();
        if (appToken->mToken != NULL)
            it = mWMService->mTokenMap.Find(appToken->mToken);
        if (it != mWMService->mTokenMap.End()) {
            parent = it->mSecond;
        }
        if ((parent == NULL) || (appToken == parent)) {
            break;
        }
        appToken = parent;
    }
    mRootToken = appToken;
    mAppToken = appToken->mAppWindowToken;

    mSurface = NULL;
    mRequestedWidth = 0;
    mRequestedHeight = 0;
    mLastRequestedWidth = 0;
    mLastRequestedHeight = 0;
    mXOffset = 0;
    mYOffset = 0;
    mLayer = 0;
    mAnimLayer = 0;
    mLastLayer = 0;
}

PInterface CWindowManagerService::WindowState::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IWindowState*)this;
    }
    else if (riid == EIID_IWindowState) {
        return (IWindowState*)this;
    }
    return NULL;
}

UInt32 CWindowManagerService::WindowState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CWindowManagerService::WindowState::Release()
{
    return ElRefBase::Release();
}

ECode CWindowManagerService::WindowState::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowManagerService::WindowState::GetFrameLw(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IRect> temp = GetFrameLw();
    *result = temp;
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetShownFrameLw(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IRect> temp = GetShownFrameLw();
    *result = temp;
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetDisplayFrameLw(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IRect> temp = GetDisplayFrameLw();
    *result = temp;
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetContentFrameLw(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IRect> temp = GetContentFrameLw();
    *result = temp;
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetVisibleFrameLw(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IRect> temp = GetVisibleFrameLw();
    *result = temp;
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetGivenInsetsPendingLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetGivenInsetsPendingLw();

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetGivenContentInsetsLw(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IRect> temp = GetGivenContentInsetsLw();
    *result = temp;
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetGivenVisibleInsetsLw(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IRect> temp = GetGivenVisibleInsetsLw();
    *result = temp;
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetAttrs(
    /* [out] */ IWindowManagerLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IWindowManagerLayoutParams> temp = GetAttrs();
    *params = temp;
    if (*params) {
        (*params)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetSurfaceLayer(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetSurfaceLayer();

    return NOERROR;
}

ECode CWindowManagerService::WindowState::GetAppToken(
    /* [out] */ IApplicationToken** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IApplicationToken> temp = GetAppToken();
    *result = temp;
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CWindowManagerService::WindowState::HasAppShownWindows(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = HasAppShownWindows();

    return NOERROR;
}

ECode CWindowManagerService::WindowState::IsVisibleLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = IsVisibleLw();

    return NOERROR;
}

ECode CWindowManagerService::WindowState::IsVisibleOrBehindKeyguardLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = IsVisibleOrBehindKeyguardLw();

    return NOERROR;
}

ECode CWindowManagerService::WindowState::IsDisplayedLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = IsDisplayedLw();

    return NOERROR;
}

ECode CWindowManagerService::WindowState::HasDrawnLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = HasDrawnLw();

    return NOERROR;
}

ECode CWindowManagerService::WindowState::HideLw(
    /* [in] */ Boolean doAnimation,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = HideLw(doAnimation);

    return NOERROR;
}

ECode CWindowManagerService::WindowState::ShowLw(
    /* [in] */ Boolean doAnimation,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ShowLw(doAnimation);

    return NOERROR;
}

void CWindowManagerService::WindowState::Attach()
{
//    if (localLOGV) Slog.v(
//        TAG, "Attaching " + this + " token=" + mToken
//        + ", list=" + mToken.windows);
    mSession->WindowAddedLocked();
}

ECode CWindowManagerService::WindowState::ComputeFrameLw(
    /* [in] */ IRect* pf,
    /* [in] */ IRect* df,
    /* [in] */ IRect* cf,
    /* [in] */ IRect* vf)
{
    mHaveFrame = TRUE;

    IRect* container = mContainingFrame;
    container->SetEx(pf);

    IRect* display = mDisplayFrame;
    display->SetEx(df);

    Int32 flags;
    mAttrs->GetFlags(&flags);
    if ((flags & WindowManagerLayoutParams_FLAG_COMPATIBLE_WINDOW) != 0) {
        Boolean result;
        container->IntersectEx(mWMService->mCompatibleScreenFrame, &result);
        if ((flags & WindowManagerLayoutParams_FLAG_LAYOUT_NO_LIMITS) == 0) {
            display->IntersectEx(mWMService->mCompatibleScreenFrame, &result);
        }
    }

    Int32 pw, ph;
    container->GetWidth(&pw);
    container->GetHeight(&ph);

    Int32 width, height, w,h;
    mAttrs->GetWidth(&width);
    mAttrs->GetHeight(&height);
    if ((flags & WindowManagerLayoutParams_FLAG_SCALED) != 0) {
        w = width < 0 ? pw : width;
        h = height < 0 ? ph : height;
    }
    else {
        w = width == ViewGroupLayoutParams_MATCH_PARENT ? pw : mRequestedWidth;
        h = height== ViewGroupLayoutParams_MATCH_PARENT ? ph : mRequestedHeight;
    }

    IRect* content = mContentFrame;
    content->SetEx(cf);

    IRect* visible = mVisibleFrame;
    visible->SetEx(vf);

    IRect* frame = mFrame;
    Int32 fw, fh;
    frame->GetWidth(&fw);
    frame->GetHeight(&fh);

    //System.out.println("In: w=" + w + " h=" + h + " container=" +
    //                   container + " x=" + mAttrs.x + " y=" + mAttrs.y);

    Int32 attrsGravity, attrsX, attrsY;
    Float attrsHMargin, attrsVMargin;
    mAttrs->GetGravity(&attrsGravity);
    mAttrs->GetX(&attrsX);
    mAttrs->GetY(&attrsY);
    mAttrs->GetHorizontalMargin(&attrsHMargin);
    mAttrs->GetVerticalMargin(&attrsVMargin);
    AutoPtr<IGravity> gravity;
    CGravity::AcquireSingleton((IGravity**)&gravity);
    gravity->ApplyEx(attrsGravity, w, h, container,
            (Int32)(attrsX + attrsHMargin * pw),
            (Int32)(attrsY + attrsVMargin * ph), frame);

    //System.out.println("Out: " + mFrame);

    // Now make sure the window fits in the overall display.
    gravity->ApplyDisplay(attrsGravity, df, frame);
//    CRect* _r = frame;
//printf("@@@@@@@@@ Left: %d, Right: %d, Top: %d, Bottom: %d @@@@@@@@@\n",
//    _r->mLeft, _r->mRight, _r->mTop, _r->mBottom);
    // Make sure the content and visible frames are inside of the
    // final window frame.
    Int32 contentLeft, contentTop, contentRight, contentBottom;
    content->GetLeft(&contentLeft);
    content->GetTop(&contentTop);
    content->GetRight(&contentRight);
    content->GetBottom(&contentBottom);

    Int32 visibleLeft, visibleTop, visibleRight, visibleBottom;
    visible->GetLeft(&visibleLeft);
    visible->GetTop(&visibleTop);
    visible->GetRight(&visibleRight);
    visible->GetBottom(&visibleBottom);

    Int32 frameLeft, frameTop, frameRight, frameBottom;
    frame->GetLeft(&frameLeft);
    frame->GetTop(&frameTop);
    frame->GetRight(&frameRight);
    frame->GetBottom(&frameBottom);

    if (contentLeft < frameLeft) {
        contentLeft = frameLeft;
        content->SetLeft(contentLeft);
    }
    if (contentTop < frameTop) {
        contentTop = frameTop;
        content->SetTop(contentTop);
    }
    if (contentRight > frameRight) {
        contentRight = frameRight;
        content->SetRight(contentRight);
    }
    if (contentBottom > frameBottom) {
        contentBottom = frameBottom;
        content->SetBottom(contentBottom);
    }
    if (visibleLeft < frameLeft) {
        visibleLeft = frameLeft;
        visible->SetLeft(visibleLeft);
    }
    if (visibleTop < frameTop) {
        visibleTop = frameTop;
        visible->SetTop(visibleTop);
    }
    if (visibleRight > frameRight) {
        visibleRight = frameRight;
        visible->SetRight(visibleRight);
    }
    if (visibleBottom > frameBottom) {
        visibleBottom = frameBottom;
        visible->SetBottom(visibleBottom);
    }

    mContentInsets->SetLeft(contentLeft - frameLeft);
    mContentInsets->SetTop(contentTop - frameTop);
    mContentInsets->SetRight(frameRight - contentRight);
    mContentInsets->SetBottom(frameBottom - contentBottom);

    mVisibleInsets->SetLeft(visibleLeft - frameLeft);
    mVisibleInsets->SetTop(visibleTop - frameTop);
    mVisibleInsets->SetRight(frameRight - visibleRight);
    mVisibleInsets->SetBottom(frameBottom - visibleBottom);

    Int32 fwTemp, fhTemp;
    frame->GetWidth(&fwTemp);
    frame->GetHeight(&fhTemp);
    if (mIsWallpaper && (fw != fwTemp || fh != fhTemp)) {
        Int32 w, h;
        mWMService->mDisplay->GetWidth(&w);
        mWMService->mDisplay->GetHeight(&h);
        mWMService->UpdateWallpaperOffsetLocked(this, w, h, FALSE);
    }

//    if (localLOGV) {
//        //if ("com.google.android.youtube".equals(mAttrs.packageName)
//        //        && mAttrs.type == WindowManager.LayoutParams.TYPE_APPLICATION_PANEL) {
//            Slog.v(TAG, "Resolving (mRequestedWidth="
//                    + mRequestedWidth + ", mRequestedheight="
//                    + mRequestedHeight + ") to" + " (pw=" + pw + ", ph=" + ph
//                    + "): frame=" + mFrame.toShortString()
//                    + " ci=" + contentInsets.toShortString()
//                    + " vi=" + visibleInsets.toShortString());
//        //}
//    }
    return NOERROR;
}

AutoPtr<IRect> CWindowManagerService::WindowState::GetFrameLw()
{
    return mFrame;
}

AutoPtr<IRect> CWindowManagerService::WindowState::GetShownFrameLw()
{
    return mShownFrame;
}

AutoPtr<IRect> CWindowManagerService::WindowState::GetDisplayFrameLw()
{
    return mDisplayFrame;
}

AutoPtr<IRect> CWindowManagerService::WindowState::GetContentFrameLw()
{
    return mContentFrame;
}

AutoPtr<IRect> CWindowManagerService::WindowState::GetVisibleFrameLw()
{
    return mVisibleFrame;
}

Boolean CWindowManagerService::WindowState::GetGivenInsetsPendingLw()
{
    return mGivenInsetsPending;
}

AutoPtr<IRect> CWindowManagerService::WindowState::GetGivenContentInsetsLw()
{
    return mGivenContentInsets;
}

AutoPtr<IRect> CWindowManagerService::WindowState::GetGivenVisibleInsetsLw()
{
    return mGivenVisibleInsets;
}

AutoPtr<IWindowManagerLayoutParams>
CWindowManagerService::WindowState::GetAttrs()
{
    return mAttrs;
}

Int32 CWindowManagerService::WindowState::GetSurfaceLayer()
{
    return mLayer;
}

Int64 CWindowManagerService::WindowState::GetInputDispatchingTimeoutNanos()
{
    return mAppToken != NULL
            ? mAppToken->mInputDispatchingTimeoutNanos
            : DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;
}

Boolean CWindowManagerService::WindowState::HasAppShownWindows()
{
    return mAppToken != NULL ? mAppToken->mFirstWindowDrawn : FALSE;
}

void CWindowManagerService::WindowState::SetAnimation(
    /* [out] */ IAnimation* anim)
{
    //if (localLOGV) Slog.v(
    //    TAG, "Setting animation in " + this + ": " + anim);
    mAnimating = FALSE;
    mLocalAnimating = FALSE;
    mAnimation = anim;
    mAnimation->RestrictDuration(MAX_ANIMATION_DURATION);
    mAnimation->ScaleCurrentDuration(mWMService->mWindowAnimationScale);
}

// This must be called while inside a transaction.
Boolean CWindowManagerService::WindowState::CommitFinishDrawingLocked(
    /* [in] */ Millisecond64 currentTime)
{
    //Slog.i(TAG, "commitFinishDrawingLocked: " + mSurface);
    if (!mCommitDrawPending) {
        return FALSE;
    }
    mCommitDrawPending = FALSE;
    mReadyToShow = TRUE;
    Int32 type;
    mAttrs->GetType(&type);
    Boolean starting = type == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING;
    AppWindowToken* atoken = mAppToken;
    if (atoken == NULL || atoken->mAllDrawn || starting) {
        PerformShowLocked();
    }
    return TRUE;
}

// This must be called while inside a transaction.
Boolean CWindowManagerService::WindowState::PerformShowLocked()
{
//    if (DEBUG_VISIBILITY) {
//        RuntimeException e = NULL;
//        if (!HIDE_STACK_CRAWLS) {
//            e = new RuntimeException();
//            e.fillInStackTrace();
//        }
//        Slog.v(TAG, "performShow on " + this
//                + ": readyToShow=" + mReadyToShow + " readyForDisplay=" + isReadyForDisplay()
//                + " starting=" + (mAttrs.type == TYPE_APPLICATION_STARTING), e);
//    }
    if (mReadyToShow && IsReadyForDisplay()) {
//        if (SHOW_TRANSACTIONS || DEBUG_ORIENTATION) logSurface(this,
//                "SHOW (performShowLocked)", NULL);
//        if (DEBUG_VISIBILITY) Slog.v(TAG, "Showing " + this
//                + " during animation: policyVis=" + mPolicyVisibility
//                + " attHidden=" + mAttachedHidden
//                + " tok.hiddenRequested="
//                + (mAppToken != NULL ? mAppToken.hiddenRequested : false)
//                + " tok.hidden="
//                + (mAppToken != NULL ? mAppToken.hidden : false)
//                + " animating=" + mAnimating
//                + " tok animating="
//                + (mAppToken != NULL ? mAppToken.animating : false));
        if (!(mWMService->ShowSurfaceRobustlyLocked(this))) {
            return FALSE;
        }
        mLastAlpha = -1;
        mHasDrawn = TRUE;
        mLastHidden = FALSE;
        mReadyToShow = FALSE;
        mWMService->EnableScreenIfNeededLocked();

        mWMService->ApplyEnterAnimationLocked(this);

        List<AutoPtr<WindowState> >::ReverseIterator rit = mChildWindows.RBegin();
        while (rit != mChildWindows.REnd()) {
            AutoPtr<WindowState> c = *rit;
            if (c->mAttachedHidden) {
                c->mAttachedHidden = FALSE;
                if (c->mSurface != NULL) {
                    c->PerformShowLocked();
                    // It hadn't been shown, which means layout not
                    // performed on it, so now we want to make sure to
                    // do a layout.  If called from within the transaction
                    // loop, this will cause it to restart with a new
                    // layout.
                    mWMService->mLayoutNeeded = TRUE;
                }
            }
            ++rit;
        }

        Int32 type;
        mAttrs->GetType(&type);
        if (type != WindowManagerLayoutParams_TYPE_APPLICATION_STARTING
                && mAppToken != NULL) {
            mAppToken->mFirstWindowDrawn = TRUE;

            if (mAppToken->mStartingData != NULL) {
//                if (DEBUG_STARTING_WINDOW || DEBUG_ANIM) Slog.v(TAG,
//                        "Finish starting " + mToken
//                        + ": first real window is shown, no animation");
                // If this initial window is animating, stop it -- we
                // will do an animation to reveal it from behind the
                // starting window, so there is no need for it to also
                // be doing its own stuff.
                if (mAnimation != NULL) {
                    mAnimation = NULL;
                    // Make sure we clean up the animation.
                    mAnimating = TRUE;
                }
                mWMService->mFinishedStarting.PushBack(mAppToken);

                ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
                pHandlerFunc = &CWindowManagerService::HandleFinishedStarting;

                mWMService->SendMessage(*(Handle32*)&pHandlerFunc, NULL);
            }
            mAppToken->UpdateReportedVisibilityLocked();
        }
    }
    return TRUE;
}

// This must be called while inside a transaction.  Returns true if
// there is more animation to run.
Boolean CWindowManagerService::WindowState::StepAnimationLocked(
    /* [in] */ Millisecond64 currentTime,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    Boolean isOn;
    mWMService->mPolicy->IsScreenOn(&isOn);
    if (!mWMService->mDisplayFrozen && isOn) {
        // We will run animations as long as the display isn't frozen.

        if (!mDrawPending && !mCommitDrawPending && mAnimation != NULL) {
            mHasTransformation = TRUE;
            mHasLocalTransformation = TRUE;
            if (!mLocalAnimating) {
//                if (DEBUG_ANIM) Slog.v(
//                    TAG, "Starting animation in " + this +
//                    " @ " + currentTime + ": ww=" + mFrame.width() + " wh=" + mFrame.height() +
//                    " dw=" + dw + " dh=" + dh + " scale=" + mWindowAnimationScale);
                Int32 w, h;
                mFrame->GetWidth(&w);
                mFrame->GetHeight(&h);
                mAnimation->Initialize(w, h, dw, dh);
                mAnimation->SetStartTime(currentTime);
                mLocalAnimating = TRUE;
                mAnimating = TRUE;
            }
            mTransformation->Clear();
            Boolean more;
            mAnimation->GetTransformation(currentTime, mTransformation, &more);
            //if (DEBUG_ANIM) Slog.v(
            //    TAG, "Stepped animation in " + this +
            //    ": more=" + more + ", xform=" + mTransformation);
            if (more) {
                // we're not done!
                return TRUE;
            }
            //if (DEBUG_ANIM) Slog.v(
            //    TAG, "Finished animation in " + this +
            //    " @ " + currentTime);
            mAnimation = NULL;
            //WindowManagerService.this.dump();
        }
        mHasLocalTransformation = FALSE;
        if ((!mLocalAnimating || mAnimationIsEntrance) && mAppToken != NULL
                && mAppToken->mAnimation != NULL) {
            // When our app token is animating, we kind-of pretend like
            // we are as well.  Note the mLocalAnimating mAnimationIsEntrance
            // part of this check means that we will only do this if
            // our window is not currently exiting, or it is not
            // locally animating itself.  The idea being that one that
            // is exiting and doing a local animation should be removed
            // once that animation is done.
            mAnimating = TRUE;
            mHasTransformation = TRUE;
            mTransformation->Clear();
            return FALSE;
        }
        else if (mHasTransformation) {
            // Little trick to get through the path below to act like
            // we have finished an animation.
            mAnimating = TRUE;
        }
        else if (IsAnimating()) {
            mAnimating = TRUE;
        }
    }
    else if (mAnimation != NULL) {
        // If the display is frozen, and there is a pending animation,
        // clear it and make sure we run the cleanup code.
        mAnimating = TRUE;
        mLocalAnimating = TRUE;
        mAnimation = NULL;
    }

    if (!mAnimating && !mLocalAnimating) {
        return FALSE;
    }

//    if (DEBUG_ANIM) Slog.v(
//        TAG, "Animation done in " + this + ": exiting=" + mExiting
//        + ", reportedVisible="
//        + (mAppToken != NULL ? mAppToken.reportedVisible : false));

    mAnimating = FALSE;
    mLocalAnimating = FALSE;
    mAnimation = NULL;
    mAnimLayer = mLayer;
    if (mIsImWindow) {
        mAnimLayer += mWMService->mInputMethodAnimLayerAdjustment;
    }
    else if (mIsWallpaper) {
        mAnimLayer += mWMService->mWallpaperAnimLayerAdjustment;
    }
//    if (DEBUG_LAYERS) Slog.v(TAG, "Stepping win " + this
//            + " anim layer: " + mAnimLayer);
    mHasTransformation = FALSE;
    mHasLocalTransformation = FALSE;
    if (mPolicyVisibility != mPolicyVisibilityAfterAnim) {
//        if (DEBUG_VISIBILITY) {
//            Slog.v(TAG, "Policy visibility changing after anim in " + this + ": "
//                    + mPolicyVisibilityAfterAnim);
//        }
        mPolicyVisibility = mPolicyVisibilityAfterAnim;
        if (!mPolicyVisibility) {
            if (mWMService->mCurrentFocus.Get() == this) {
                mWMService->mFocusMayChange = TRUE;
            }
            // Window is no longer visible -- make sure if we were waiting
            // for it to be displayed before enabling the display, that
            // we allow the display to be enabled now.
            mWMService->EnableScreenIfNeededLocked();
        }
    }
//    mTransformation->Clear();
    Int32 type;
    mAttrs->GetType(&type);
    if (mHasDrawn
            && type == WindowManagerLayoutParams_TYPE_APPLICATION_STARTING
            && mAppToken != NULL
            && mAppToken->mFirstWindowDrawn
            && mAppToken->mStartingData != NULL) {
//        if (DEBUG_STARTING_WINDOW) Slog.v(TAG, "Finish starting "
//                + mToken + ": first real window done animating");
        mWMService->mFinishedStarting.PushBack(mAppToken);

        ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
        pHandlerFunc = &CWindowManagerService::HandleFinishedStarting;

        mWMService->SendMessage(*(Handle32*)&pHandlerFunc, NULL);
    }

    FinishExit();

    if (mAppToken != NULL) {
        mAppToken->UpdateReportedVisibilityLocked();
    }

    return FALSE;
}

void CWindowManagerService::WindowState::FinishExit()
{
//    if (DEBUG_ANIM) Slog.v(
//            TAG, "finishExit in " + this
//            + ": exiting=" + mExiting
//            + " remove=" + mRemoveOnExit
//            + " windowAnimating=" + isWindowAnimating());

    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mChildWindows.Begin(); it != mChildWindows.End(); ++it) {
        (*it)->FinishExit();
    }

    if (!mExiting) {
        return;
    }

    if (IsWindowAnimating()) {
        return;
    }

//    if (localLOGV) Slog.v(
//            TAG, "Exit animation finished in " + this
//            + ": remove=" + mRemoveOnExit);
    if (mSurface != NULL) {
        mWMService->mDestroySurface.PushBack(this);
        mDestroying = TRUE;
//        if (SHOW_TRANSACTIONS) logSurface(this, "HIDE (finishExit)", NULL);
        mSurfaceShown = FALSE;
//        try {
        ECode ec = mSurface->Hide();
        if (FAILED(ec)) {
//            Slog.w(TAG, "Error hiding surface in " + this, e);
        }
//        } catch (RuntimeException e) {
//            Slog.w(TAG, "Error hiding surface in " + this, e);
//        }
        mLastHidden = TRUE;
    }
    mExiting = FALSE;
    if (mRemoveOnExit) {
        mWMService->mPendingRemove.PushBack(this);
        mRemoveOnExit = FALSE;
    }
}

Boolean CWindowManagerService::WindowState::IsIdentityMatrix(
    /* [in] */ Float dsdx,
    /* [in] */ Float dtdx,
    /* [in] */ Float dsdy,
    /* [in] */ Float dtdy)
{
    if (dsdx < 0.99999f || dsdx > 1.00001f) return FALSE;
    if (dtdy < 0.99999f || dtdy > 1.00001f) return FALSE;
    if (dtdx < -0.000001f || dtdx > 0.000001f) return FALSE;
    if (dsdy < -0.000001f || dsdy > 0.000001f) return FALSE;
    return TRUE;
}

void CWindowManagerService::WindowState::ComputeShownFrameLocked()
{
    Boolean selfTransformation = mHasLocalTransformation;
    AutoPtr<ITransformation> attachedTransformation =
            (mAttachedWindow != NULL && mAttachedWindow->mHasLocalTransformation)
            ? mAttachedWindow->mTransformation : NULL;
    AutoPtr<ITransformation> appTransformation =
            (mAppToken != NULL && mAppToken->mHasTransformation)
            ? mAppToken->mTransformation : NULL;

    // Wallpapers are animated based on the "real" window they
    // are currently targeting.
    Int32 type;
    mAttrs->GetType(&type);
    if (type == WindowManagerLayoutParams_TYPE_WALLPAPER
            && mWMService->mLowerWallpaperTarget == NULL
            && mWMService->mWallpaperTarget != NULL) {
        Boolean detach;
        if (mWMService->mWallpaperTarget->mHasLocalTransformation &&
            mWMService->mWallpaperTarget->mAnimation != NULL &&
            !(mWMService->mWallpaperTarget->mAnimation->GetDetachWallpaper(&detach), detach)) {
            attachedTransformation = mWMService->mWallpaperTarget->mTransformation;
            //if (DEBUG_WALLPAPER && attachedTransformation != NULL) {
            //    Slog.v(TAG, "WP target attached xform: " + attachedTransformation);
            //}
        }
        if (mWMService->mWallpaperTarget->mAppToken != NULL &&
            mWMService->mWallpaperTarget->mAppToken->mHasTransformation &&
            mWMService->mWallpaperTarget->mAppToken->mAnimation != NULL &&
            !(mWMService->mWallpaperTarget->mAppToken->mAnimation->GetDetachWallpaper(&detach), detach)) {
            appTransformation = mWMService->mWallpaperTarget->mAppToken->mTransformation;
            //if (DEBUG_WALLPAPER && appTransformation != NULL) {
            //    Slog.v(TAG, "WP target app xform: " + appTransformation);
            //}
        }
    }

    if (selfTransformation || attachedTransformation != NULL
            || appTransformation != NULL) {
        // cache often used attributes locally
        Int32 frameLeft, frameTop;
        mFrame->GetLeft(&frameLeft);
        mFrame->GetTop(&frameTop);
        ArrayOf<Float>* tmpFloats = mWMService->mTmpFloats;
        AutoPtr<IMatrix> tmpMatrix = mTmpMatrix;

        // Compute the desired transformation.
        tmpMatrix->SetTranslate(0, 0);
        Boolean res;
        if (selfTransformation) {
            AutoPtr<IMatrix> matrix;
            mTransformation->GetMatrix((IMatrix**)&matrix);
            tmpMatrix->PostConcat(matrix, &res);
        }
        tmpMatrix->PostTranslate(frameLeft, frameTop, &res);
        if (attachedTransformation != NULL) {
            AutoPtr<IMatrix> matrix;
            attachedTransformation->GetMatrix((IMatrix**)&matrix);
            tmpMatrix->PostConcat(matrix, &res);
        }
        if (appTransformation != NULL) {
            AutoPtr<IMatrix> matrix;
            appTransformation->GetMatrix((IMatrix**)&matrix);
            tmpMatrix->PostConcat(matrix, &res);
        }

        // "convert" it into SurfaceFlinger's format
        // (a 2x2 matrix + an offset)
        // Here we must not transform the position of the surface
        // since it is already included in the transformation.
        //Slog.i(TAG, "Transform: " + matrix);

        tmpMatrix->GetValues(*tmpFloats);
        mDsDx = (*tmpFloats)[Matrix_MSCALE_X];
        mDtDx = (*tmpFloats)[Matrix_MSKEW_X];
        mDsDy = (*tmpFloats)[Matrix_MSKEW_Y];
        mDtDy = (*tmpFloats)[Matrix_MSCALE_Y];
        Int32 x = (Int32)(*tmpFloats)[Matrix_MTRANS_X] + mXOffset;
        Int32 y = (Int32)(*tmpFloats)[Matrix_MTRANS_Y] + mYOffset;
        Int32 w, h;
        mFrame->GetWidth(&w);
        mFrame->GetHeight(&h);
        mShownFrame->Set(x, y, x + w, y + h);

        // Now set the alpha...  but because our current hardware
        // can't do alpha transformation on a non-opaque surface,
        // turn it off if we are running an animation that is also
        // transforming since it is more important to have that
        // animation be smooth.
        mShownAlpha = mAlpha;
        Int32 format;
        mAttrs->GetFormat(&format);
        if (!mWMService->mLimitedAlphaCompositing
            || (!ElPixelFormat::FormatHasAlpha(format)
            || (IsIdentityMatrix(mDsDx, mDtDx, mDsDy, mDtDy)
            && x == frameLeft && y == frameTop))) {
            //Slog.i(TAG, "Applying alpha transform");
            Float alpha;
            if (selfTransformation) {
                mTransformation->GetAlpha(&alpha);
                mShownAlpha *=  alpha;
            }
            if (attachedTransformation != NULL) {
                attachedTransformation->GetAlpha(&alpha);
                mShownAlpha *=  alpha;
            }
            if (appTransformation != NULL) {
                appTransformation->GetAlpha(&alpha);
                mShownAlpha *=  alpha;
            }
        }
        else {
            //Slog.i(TAG, "Not applying alpha transform");
        }

        //if (localLOGV) Slog.v(
        //    TAG, "Continuing animation in " + this +
        //    ": " + mShownFrame +
        //    ", alpha=" + mTransformation.getAlpha());
        return;
    }

    mShownFrame->SetEx(mFrame);
    if (mXOffset != 0 || mYOffset != 0) {
        mShownFrame->Offset(mXOffset, mYOffset);
    }
    mShownAlpha = mAlpha;
    mDsDx = 1;
    mDtDx = 0;
    mDsDy = 0;
    mDtDy = 1;
}

void CWindowManagerService::WindowState::ClearAnimation()
{
    if (mAnimation != NULL) {
        mAnimating = TRUE;
        mLocalAnimating = FALSE;
        mAnimation = NULL;
    }
}

ECode CWindowManagerService::WindowState::CreateSurfaceLocked(
    /* [out] */ ISurface** surface)
{
    assert(surface);

    ECode ec = NOERROR;

    if (mSurface == NULL) {
        mReportDestroySurface = FALSE;
        mSurfacePendingDestroy = FALSE;
        mDrawPending = TRUE;
        mCommitDrawPending = FALSE;
        mReadyToShow = FALSE;
        if (mAppToken != NULL) {
            mAppToken->mAllDrawn = FALSE;
        }

        Int32 flags = 0;
        Int32 attrsMemoryType;
        mAttrs->GetMemoryType(&attrsMemoryType);
        if (attrsMemoryType == WindowManagerLayoutParams_MEMORY_TYPE_PUSH_BUFFERS) {
            flags |= Surface_PUSH_BUFFERS;
        }

        Int32 attrsFlags;
        mAttrs->GetFlags(&attrsFlags);
        if ((attrsFlags & WindowManagerLayoutParams_FLAG_SECURE) != 0) {
            flags |= Surface_SECURE;
        }
//        if (DEBUG_VISIBILITY) Slog.v(
//            TAG, "Creating surface in session "
//            + mSession->mSurfaceSession + " window " + this
//            + " w=" + mFrame->Width()
//            + " h=" + mFrame->Height() + " format="
//            + mAttrs->mFormat + " flags=" + flags);

        Int32 w;
        Int32 h;
        mFrame->GetWidth(&w);
        mFrame->GetHeight(&h);
        if ((attrsFlags & WindowManagerLayoutParams_FLAG_SCALED) != 0) {
            // for a scaled surface, we always want the requested
            // size.
            w = mRequestedWidth;
            h = mRequestedHeight;
        }

        // Something is wrong and SurfaceFlinger will not like this,
        // try to revert to sane values
        if (w <= 0) w = 1;
        if (h <= 0) h = 1;

        mSurfaceShown = FALSE;
        mSurfaceLayer = 0;
        mSurfaceAlpha = 1;
        mSurfaceX = 0;
        mSurfaceY = 0;
        mSurfaceW = w;
        mSurfaceH = h;

//        try {
        AutoPtr<ICharSequence> title;
        mAttrs->GetTitle((ICharSequence**)&title);
        String strTitle;
        if (title != NULL) {
            title->ToString(&strTitle);
        }
        Int32 format;
        mAttrs->GetFormat(&format);
/* TODO:
        ec = CSurface::New(
            mSession->mSurfaceSession,
            mSession->mPid, title->GetPayload(),
            0, w, h, mAttrs->mFormat, flags,
            (ISurface**)&mSurface);
*/
//        AutoPtr<ISurfaceSession> surfaceSession;

//        assert(SUCCEEDED(CSurfaceSession::New(
//            (ISurfaceSession**)&surfaceSession)));
        assert(SUCCEEDED(CSurface::New(
            mSession->mSurfaceSession,
            mSession->mPid, strTitle,

            //TODO :Temporarily modify format = -2;
            //0, w, h, format, flags,
            0, w, h, -2, flags,
            (ISurface**)&mSurface)));

        if (FAILED(ec)) {
//            Slog.e(TAG, "Exception creating surface, 0x%8", ec);
            return ec;
        }
//            if (SHOW_TRANSACTIONS) Slog.i(TAG, "  CREATE SURFACE "
//                    + mSurface + " IN SESSION "
//                    + mSession.mSurfaceSession
//                    + ": pid=" + mSession.mPid + " format="
//                    + mAttrs.format + " flags=0x"
//                    + Integer.toHexString(flags)
//                    + " / " + this);
//        } catch (Surface.OutOfResourcesException e) {
//            Slog.w(TAG, "OutOfResourcesException creating surface");
//            reclaimSomeSurfaceMemoryLocked(this, "create");
//            return NULL;
//        } catch (Exception e) {
//            Slog.e(TAG, "Exception creating surface", e);
//            return NULL;
//        }

//        if (localLOGV) Slog.v(
//            TAG, "Got surface: " + mSurface
//            + ", set left=" + mFrame.left + " top=" + mFrame.top
//            + ", animLayer=" + mAnimLayer);
//        if (SHOW_TRANSACTIONS) {
//            Slog.i(TAG, ">>> OPEN TRANSACTION");
//            if (SHOW_TRANSACTIONS) logSurface(this,
//                    "CREATE pos=(" + mFrame.left + "," + mFrame.top + ") (" +
//                    mFrame.width() + "x" + mFrame.height() + "), layer=" +
//                    mAnimLayer + " HIDE", NULL);
//        }
        AutoPtr<ISurfaceHelper> helper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
        helper->OpenTransaction();

//            try {
//                try {
        Int32 l, t;
        mFrame->GetLeft(&l);
        mFrame->GetTop(&t);
        mSurfaceX = l + mXOffset;
        mSurfaceY = t + mYOffset;
        mSurface->SetPosition(mSurfaceX, mSurfaceY);
//
//        mSurfaceLayer = mAnimLayer;
//        ec = mSurface->SetLayer(mAnimLayer);
//        if (FAILED(ec)) {
//            printf("=====%s, %d=====\n", __FILE__, __LINE__);
//            goto RuntimeException;
//        }
//
//        mSurfaceShown = FALSE;
//        ec = mSurface->Hide();
//        if (FAILED(ec)) {
//            printf("=====%s, %d=====\n", __FILE__, __LINE__);
//            goto RuntimeException;
//        }
//
//        if ((mAttrs->mFlags & WindowManagerLayoutParams_FLAG_DITHER) != 0) {
////            if (SHOW_TRANSACTIONS) logSurface(this, "DITHER", NULL);
//            ec = mSurface->SetFlags(Surface_SURFACE_DITHER,
//                    Surface_SURFACE_DITHER);
//            if (FAILED(ec)) {
//                printf("=====%s, %d=====\n", __FILE__, __LINE__);
//                goto RuntimeException;
//            }
//        }
////            } catch (RuntimeException e) {
////                Slog.w(TAG, "Error creating surface in " + w, e);
////                reclaimSomeSurfaceMemoryLocked(this, "create-init");
////            }
//        mLastHidden = TRUE;
//        if (SHOW_TRANSACTIONS) Slog.i(TAG, "<<< CLOSE TRANSACTION");
        helper->CloseTransaction();
    }

    if (mSurface) {
        *surface = mSurface;
        (*surface)->AddRef();
    }

    return ec;

//        } finally {
//            if (SHOW_TRANSACTIONS) Slog.i(TAG, "<<< CLOSE TRANSACTION");
//            Surface.closeTransaction();
//        }
//        if (localLOGV) Slog.v(
//                TAG, "Created surface " + this);
//    }
//  RuntimeException:
//    Slog.w(TAG, "Error creating surface in " + w, e);
//    assert(mWMService);
//    mWMService->ReclaimSomeSurfaceMemoryLocked(this, "create-init");
//    if (SHOW_TRANSACTIONS) Slog.i(TAG, "<<< CLOSE TRANSACTION");
//    CSurface::CloseTransaction();

//    return ec;
}

void CWindowManagerService::WindowState::DestroySurfaceLocked()
{
    if (mAppToken != NULL && this == mAppToken->mStartingWindow) {
        mAppToken->mStartingDisplayed = FALSE;
    }

    if (mSurface != NULL) {
        mDrawPending = FALSE;
        mCommitDrawPending = FALSE;
        mReadyToShow = FALSE;

        List<AutoPtr<WindowState> >::ReverseIterator rit;
        for (rit = mChildWindows.RBegin(); rit != mChildWindows.REnd(); ++rit) {
            AutoPtr<WindowState> c = *rit;
            c->mAttachedHidden = TRUE;
        }

        if (mReportDestroySurface) {
            mReportDestroySurface = FALSE;
            mSurfacePendingDestroy = TRUE;
//            try {
            mClient->DispatchGetNewSurface();
            // We'll really destroy on the next time around.
            return;
//            } catch (RemoteException e) {
//            }
        }

//        try {
//        if (DEBUG_VISIBILITY) {
//            RuntimeException e = NULL;
//            if (!HIDE_STACK_CRAWLS) {
//                e = new RuntimeException();
//                e.fillInStackTrace();
//            }
//            Slog.w(TAG, "Window " + this + " destroying surface "
//                    + mSurface + ", session " + mSession, e);
//        }
//        if (SHOW_TRANSACTIONS) {
//            RuntimeException e = NULL;
//            if (!HIDE_STACK_CRAWLS) {
//                e = new RuntimeException();
//                e.fillInStackTrace();
//            }
//            if (SHOW_TRANSACTIONS) logSurface(this, "DESTROY", e);
//        }
        mSurface->Destroy();
//        } catch (RuntimeException e) {
//            Slog.w(TAG, "Exception thrown when destroying Window " + this
//                + " surface " + mSurface + " session " + mSession
//                + ": " + e.toString());
//        }

        mSurfaceShown = FALSE;
        mSurface = NULL;
    }
}

Boolean CWindowManagerService::WindowState::FinishDrawingLocked()
{
    if (mDrawPending) {
//        if (SHOW_TRANSACTIONS || DEBUG_ORIENTATION) Slog.v(
//            TAG, "finishDrawingLocked: " + mSurface);
        mCommitDrawPending = TRUE;
        mDrawPending = FALSE;
        return TRUE;
    }
    return FALSE;
}

IApplicationToken* CWindowManagerService::WindowState::GetAppToken()
{
    return mAppToken != NULL ? mAppToken->mAppToken : NULL;
}

/**
 * Is this window visible?  It is not visible if there is no
 * surface, or we are in the process of running an exit animation
 * that will remove the surface, or its app token has been hidden.
 */
Boolean CWindowManagerService::WindowState::IsVisibleLw()
{
    AppWindowToken* atoken = mAppToken;
    return mSurface != NULL && mPolicyVisibility && !mAttachedHidden
            && (atoken == NULL || !atoken->mHiddenRequested)
            && !mExiting && !mDestroying;
}

Boolean CWindowManagerService::WindowState::IsVisibleOrBehindKeyguardLw()
{
    AppWindowToken* atoken = mAppToken;
    return mSurface != NULL && !mAttachedHidden
            && (atoken == NULL ? mPolicyVisibility : !atoken->mHiddenRequested)
            && (mOrientationChanging || (!mDrawPending && !mCommitDrawPending))
            && !mExiting && !mDestroying;
}

/**
 * Is this window visible, ignoring its app token?  It is not visible
 * if there is no surface, or we are in the process of running an exit animation
 * that will remove the surface.
 */
Boolean CWindowManagerService::WindowState::IsWinVisibleLw()
{
    AppWindowToken* atoken = mAppToken;
    return mSurface != NULL && mPolicyVisibility && !mAttachedHidden
            && (atoken == NULL || !atoken->mHiddenRequested || atoken->mAnimating)
            && !mExiting && !mDestroying;
}

/**
 * The same as isVisible(), but follows the current hidden state of
 * the associated app token, not the pending requested hidden state.
 */
Boolean CWindowManagerService::WindowState::IsVisibleNow()
{
    return mSurface != NULL && mPolicyVisibility && !mAttachedHidden
            && !mRootToken->mHidden && !mExiting && !mDestroying;
}

/**
 * Same as isVisible(), but we also count it as visible between the
 * call to IWindowSession.add() and the first relayout().
 */
Boolean CWindowManagerService::WindowState::IsVisibleOrAdding()
{
    AppWindowToken* atoken = mAppToken;
    return ((mSurface != NULL && !mReportDestroySurface)
                    || (!mRelayoutCalled && mViewVisibility == View_VISIBLE))
            && mPolicyVisibility && !mAttachedHidden
            && (atoken == NULL || !atoken->mHiddenRequested)
            && !mExiting && !mDestroying;
}

/**
 * Is this window currently on-screen?  It is on-screen either if it
 * is visible or it is currently running an animation before no longer
 * being visible.
 */
Boolean CWindowManagerService::WindowState::IsOnScreen()
{
    AppWindowToken* atoken = mAppToken;
    if (atoken != NULL) {
        return mSurface != NULL && mPolicyVisibility && !mDestroying
            && ((!mAttachedHidden && !atoken->mHiddenRequested)
            || mAnimation != NULL || atoken->mAnimation != NULL);
    }
    else {
        return mSurface != NULL && mPolicyVisibility && !mDestroying
            && (!mAttachedHidden || mAnimation != NULL);
    }
}

/**
 * Like isOnScreen(), but we don't return true if the window is part
 * of a transition that has not yet been started.
 */
Boolean CWindowManagerService::WindowState::IsReadyForDisplay()
{
    if (mRootToken->mWaitingToShow &&
            mWMService->mNextAppTransition != WindowManagerPolicy_TRANSIT_UNSET) {
        return FALSE;
    }
    AppWindowToken* atoken = mAppToken;
    Boolean animating = atoken != NULL
            ? (atoken->mAnimation != NULL) : FALSE;
    return mSurface != NULL && mPolicyVisibility && !mDestroying
            && ((!mAttachedHidden && mViewVisibility == View_VISIBLE
            && !mRootToken->mHidden) || mAnimation != NULL || animating);
}

/** Is the window or its container currently animating? */
Boolean CWindowManagerService::WindowState::IsAnimating()
{
    AutoPtr<WindowState> attached = mAttachedWindow;
    AppWindowToken* atoken = mAppToken;
    return mAnimation != NULL
        || (attached != NULL && attached->mAnimation != NULL)
        || (atoken != NULL && (atoken->mAnimation != NULL
        || atoken->mInPendingTransaction));

    return FALSE;
}

/** Is this window currently animating? */
Boolean CWindowManagerService::WindowState::IsWindowAnimating()
{
    return mAnimation != NULL;
}

/**
 * Like isOnScreen, but returns false if the surface hasn't yet
 * been drawn.
 */
Boolean CWindowManagerService::WindowState::IsDisplayedLw()
{
    AppWindowToken* atoken = mAppToken;
    return mSurface != NULL && mPolicyVisibility && !mDestroying
        && !mDrawPending && !mCommitDrawPending
        && ((!mAttachedHidden &&
                (atoken == NULL || !atoken->mHiddenRequested))
                || mAnimating);
}

/**
 * Returns true if the window has a surface that it has drawn a
 * complete UI in to.  Note that this returns true if the orientation
 * is changing even if the window hasn't redrawn because we don't want
 * to stop things from executing during that time.
 */
Boolean CWindowManagerService::WindowState::IsDrawnLw()
{
//    AppWindowToken* atoken = mAppToken;
    return mSurface != NULL && !mDestroying
        && (mOrientationChanging || (!mDrawPending && !mCommitDrawPending));
}

/**
 * Return true if the window is opaque and fully drawn.  This indicates
 * it may obscure windows behind it.
 */
Boolean CWindowManagerService::WindowState::IsOpaqueDrawn()
{
    Int32 format, type;
    mAttrs->GetFormat(&format);
    mAttrs->GetType(&type);
    return (format == ElPixelFormat::OPAQUE
        || type == WindowManagerLayoutParams_TYPE_WALLPAPER)
        && mSurface != NULL && mAnimation == NULL
        && (mAppToken == NULL || mAppToken->mAnimation == NULL)
        && !mDrawPending && !mCommitDrawPending;
}

Boolean CWindowManagerService::WindowState::NeedsBackgroundFiller(
    /* [in] */ Int32 screenWidth,
    /* [in] */ Int32 screenHeight)
{
    Int32 flags, type;
    mAttrs->GetFlags(&flags);
    mAttrs->GetType(&type);

    Int32 frameLeft, frameTop, frameRight,frameBottom;
    mFrame->GetLeft(&frameLeft);
    mFrame->GetTop(&frameTop);
    mFrame->GetRight(&frameRight);
    mFrame->GetBottom(&frameBottom);

    Int32 scFrameLeft, scFrameTop, scFrameRight, scFrameBottom;
    mWMService->mCompatibleScreenFrame->GetLeft(&scFrameLeft);
    mWMService->mCompatibleScreenFrame->GetTop(&scFrameTop);
    mWMService->mCompatibleScreenFrame->GetRight(&scFrameRight);
    mWMService->mCompatibleScreenFrame->GetBottom(&scFrameBottom);
    return
         // only if the application is requesting compatible window
         (flags & WindowManagerLayoutParams_FLAG_COMPATIBLE_WINDOW) != 0 &&
         // only if it's visible
         mHasDrawn && mViewVisibility == View_VISIBLE &&
         // and only if the application fills the compatible screen
         frameLeft <= scFrameLeft && frameTop <= scFrameTop &&
         frameRight >= scFrameRight && frameBottom >= scFrameBottom &&
         // and starting window do not need background filler
         type != WindowManagerLayoutParams_TYPE_APPLICATION_STARTING;
}

Boolean CWindowManagerService::WindowState::IsFullscreen(
    /* [in] */ Int32 screenWidth,
    /* [in] */ Int32 screenHeight)
{
    Int32 left, top, right, bottom;
    mFrame->GetLeft(&left);
    mFrame->GetTop(&top);
    mFrame->GetRight(&right);
    mFrame->GetBottom(&bottom);
    return left <= 0 && top <= 0 && right >= screenWidth && bottom >= screenHeight;
}

void CWindowManagerService::WindowState::RemoveLocked()
{
//    DisposeInputChannel();

    if (mAttachedWindow != NULL) {
        mAttachedWindow->mChildWindows.Remove(this);
    }
    DestroySurfaceLocked();
//    mSession->WindowRemovedLocked();
////    try {
////        mClient.asBinder().unlinkToDeath(mDeathRecipient, 0);
////    } catch (RuntimeException e) {
////        // Ignore if it has already been removed (usually because
////        // we are doing this as part of processing a death note.)
////    }
}

void CWindowManagerService::WindowState::DisposeInputChannel()
{
    if (mInputChannel != NULL) {
        mWMService->mInputManager->UnregisterInputChannel(mInputChannel);

        mInputChannel->Dispose();
        mInputChannel = NULL;
    }
}

Boolean CWindowManagerService::WindowState::ShowLw(
    /* [in] */ Boolean doAnimation)
{
    return ShowLw(doAnimation, TRUE);
}

Boolean CWindowManagerService::WindowState::ShowLw(
    /* [in] */ Boolean doAnimation,
    /* [in] */ Boolean requestAnim)
{
    if (mPolicyVisibility && mPolicyVisibilityAfterAnim) {
        return FALSE;
    }
//    if (DEBUG_VISIBILITY) Slog.v(TAG, "Policy visibility true: " + this);
    if (doAnimation) {
//        if (DEBUG_VISIBILITY) Slog.v(TAG, "doAnimation: mPolicyVisibility="
//                + mPolicyVisibility + " mAnimation=" + mAnimation);
        Boolean isOn;
        mWMService->mPolicy->IsScreenOn(&isOn);
        if (mWMService->mDisplayFrozen || !isOn) {
            doAnimation = FALSE;
        }
        else if (mPolicyVisibility && mAnimation == NULL) {
            // Check for the case where we are currently visible and
            // not animating; we do not want to do animation at such a
            // point to become visible when we already are.
            doAnimation = FALSE;
        }
    }
    mPolicyVisibility = TRUE;
    mPolicyVisibilityAfterAnim = TRUE;
    if (doAnimation) {
        mWMService->ApplyAnimationLocked(this, WindowManagerPolicy::TRANSIT_ENTER, TRUE);
    }
    if (requestAnim) {
        mWMService->RequestAnimationLocked(0);
    }
    return TRUE;
}

Boolean CWindowManagerService::WindowState::HideLw(
    /* [in] */ Boolean doAnimation)
{
    return HideLw(doAnimation, TRUE);
}

Boolean CWindowManagerService::WindowState::HideLw(
    /* [in] */ Boolean doAnimation,
    /* [in] */ Boolean requestAnim)
{
    if (doAnimation) {
        Boolean isOn;
        mWMService->mPolicy->IsScreenOn(&isOn);
        if (mWMService->mDisplayFrozen || !isOn) {
            doAnimation = FALSE;
        }
    }
    Boolean current = doAnimation ? mPolicyVisibilityAfterAnim
            : mPolicyVisibility;
    if (!current) {
        return FALSE;
    }
    if (doAnimation) {
        mWMService->ApplyAnimationLocked(this, WindowManagerPolicy::TRANSIT_EXIT, FALSE);
        doAnimation = FALSE;
        if (mAnimation == NULL) {
            doAnimation = FALSE;
        }
    }
    if (doAnimation) {
        mPolicyVisibilityAfterAnim = FALSE;
    }
    else {
//        if (DEBUG_VISIBILITY) Slog.v(TAG, "Policy visibility false: " + this);
        mPolicyVisibilityAfterAnim = FALSE;
        mPolicyVisibility = FALSE;
        // Window is no longer visible -- make sure if we were waiting
        // for it to be displayed before enabling the display, that
        // we allow the display to be enabled now.
        mWMService->EnableScreenIfNeededLocked();
        if (mWMService->mCurrentFocus.Get() == this) {
            mWMService->mFocusMayChange = TRUE;
        }
    }
    if (requestAnim) {
        mWMService->RequestAnimationLocked(0);
    }
    return TRUE;
}

 /**
 * DimAnimator class that controls the dim animation. This holds the surface and
 * all state used for dim animation.
 */
CWindowManagerService::DimAnimator::DimAnimator(
    /* [in] */ ISurfaceSession* session) :
    mDimShown(FALSE)
{
    if (mDimSurface == NULL) {
//    if (SHOW_TRANSACTIONS) Slog.i(TAG, "  DIM "
//            + mDimSurface + ": CREATE");
//    try {
        assert(SUCCEEDED(CSurface::New(session, 0, String("DimSurface"),
            -1, 16, 16, ElPixelFormat::OPAQUE, Surface_FX_SURFACE_DIM,
            (ISurface**)&mDimSurface)));
        mDimSurface->SetAlpha(0.0);
//    } catch (Exception e) {
//        Slog.e(TAG, "Exception creating Dim surface", e);
//    }
    }
}

void CWindowManagerService::Watermark::DrawIfNeeded()
{
}

/**
 * Show the dim surface.
 */
void CWindowManagerService::DimAnimator::Show(
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    if (!mDimShown) {
//        if (SHOW_TRANSACTIONS) Slog.i(TAG, "  DIM " + mDimSurface + ": SHOW pos=(0,0) (" +
//                dw + "x" + dh + ")");
        mDimShown = TRUE;
//        try {
        mLastDimWidth = dw;
        mLastDimHeight = dh;
        mDimSurface->SetPosition(0, 0);
        mDimSurface->SetSize(dw, dh);
        mDimSurface->Show();
//        } catch (RuntimeException e) {
//            Slog.w(TAG, "Failure showing dim surface", e);
//        }
    }
    else if (mLastDimWidth != dw || mLastDimHeight != dh) {
        mLastDimWidth = dw;
        mLastDimHeight = dh;
        mDimSurface->SetSize(dw, dh);
    }
}

/**
 * Set's the dim surface's layer and update dim parameters that will be used in
 * {@link updateSurface} after all windows are examined.
 */
void CWindowManagerService::DimAnimator::UpdateParameters(
    /* [in] */ WindowState* w,
    /* [in] */ Millisecond64 currentTime)
{
    mDimSurface->SetLayer(w->mAnimLayer - 1);

    Float dimAmount;
    w->mAttrs->GetDimAmount(&dimAmount);
    Float target = w->mExiting ? 0 : dimAmount;
//    if (SHOW_TRANSACTIONS) Slog.i(TAG, "  DIM " + mDimSurface
//            + ": layer=" + (w.mAnimLayer-1) + " target=" + target);
    if (mDimTargetAlpha != target) {
        // If the desired dim level has changed, then
        // start an animation to it.
        mLastDimAnimTime = currentTime;
        Int64 duration = DEFAULT_DIM_DURATION;
        if (w->mAnimating && w->mAnimation != NULL) {
            w->mAnimation->ComputeDurationHint(&duration);
        }

        if (target > mDimTargetAlpha) {
            // This is happening behind the activity UI,
            // so we can make it run a little longer to
            // give a stronger impression without disrupting
            // the user.
            duration *= DIM_DURATION_MULTIPLIER;
        }
        if (duration < 1) {
            // Don't divide by zero
            duration = 1;
        }
        mDimTargetAlpha = target;
        mDimDeltaPerMs = (mDimTargetAlpha - mDimCurrentAlpha) / duration;
    }
}

/**
 * Updating the surface's alpha. Returns true if the animation continues, or returns
 * false when the animation is finished and the dim surface is hidden.
 */
Boolean CWindowManagerService::DimAnimator::UpdateSurface(
    /* [in] */ Boolean dimming,
    /* [in] */ Millisecond64 currentTime,
    /* [in] */ Boolean displayFrozen)
{
    if (!dimming) {
        if (mDimTargetAlpha != 0) {
            mLastDimAnimTime = currentTime;
            mDimTargetAlpha = 0;
            mDimDeltaPerMs = (-mDimCurrentAlpha) / DEFAULT_DIM_DURATION;
        }
    }

    Boolean animating = FALSE;
    if (mLastDimAnimTime != 0) {
        mDimCurrentAlpha += mDimDeltaPerMs
                * (currentTime - mLastDimAnimTime);
        Boolean more = TRUE;
        if (displayFrozen) {
            // If the display is frozen, there is no reason to animate.
            more = FALSE;
        }
        else if (mDimDeltaPerMs > 0) {
            if (mDimCurrentAlpha > mDimTargetAlpha) {
                more = FALSE;
            }
        }
        else if (mDimDeltaPerMs < 0) {
            if (mDimCurrentAlpha < mDimTargetAlpha) {
                more = FALSE;
            }
        }
        else {
            more = FALSE;
        }

        // Do we need to continue animating?
        if (more) {
//            if (SHOW_TRANSACTIONS) Slog.i(TAG, "  DIM "
//                    + mDimSurface + ": alpha=" + mDimCurrentAlpha);
            mLastDimAnimTime = currentTime;
            mDimSurface->SetAlpha(mDimCurrentAlpha);
            animating = TRUE;
        }
        else {
            mDimCurrentAlpha = mDimTargetAlpha;
            mLastDimAnimTime = 0;
//            if (SHOW_TRANSACTIONS) Slog.i(TAG, "  DIM "
//                    + mDimSurface + ": final alpha=" + mDimCurrentAlpha);
            mDimSurface->SetAlpha(mDimCurrentAlpha);
            if (!dimming) {
//                if (SHOW_TRANSACTIONS) Slog.i(TAG, "  DIM " + mDimSurface
//                        + ": HIDE");
//                try {
                mDimSurface->Hide();
//                } catch (RuntimeException e) {
//                    Slog.w(TAG, "Illegal argument exception hiding dim surface");
//                }
                mDimShown = FALSE;
            }
        }
    }
    return animating;
}

Boolean CWindowManagerService::WindowState::CanReceiveKeys()
{
    Int32 flags;
    mAttrs->GetFlags(&flags);
    return IsVisibleOrAdding()
            && (mViewVisibility == View_VISIBLE)
            && ((flags & WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE) == 0);
}

Boolean CWindowManagerService::WindowState::HasDrawnLw()
{
    return mHasDrawn;
}

void CWindowManagerService::WindowState::MakeInputChannelName(
    /* [out] */ String* name)
{
//    return Integer.toHexString(System.identityHashCode(this))
//                + " " + mAttrs.getTitle();
    assert(name != NULL);
    *name = "Elastos_2012";
}

void CWindowManagerService::WindowState::GetDescription(
    /* [out] */ String* description)
{
//    if (mStringNameCache == NULL || mLastTitle != mAttrs.getTitle()
//            || mWasPaused != mToken.paused) {
//        mLastTitle = mAttrs.getTitle();
//        mWasPaused = mToken.paused;
//        mStringNameCache = "Window{" + Integer.toHexString(System.identityHashCode(this))
//                + " " + mLastTitle + " paused=" + mWasPaused + "}";
//    }
//    return mStringNameCache;

    assert(description != NULL);
    *description = "Window{}";
}

ECode CWindowManagerService::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode CWindowManagerService::SendMessageDelayed(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 delayMillis)
{
    return mApartment->PostCppCallbackDelayed(
        (Handle32)this, pvFunc, params, 0, delayMillis);
}

ECode CWindowManagerService::RemoveMessage(
        /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode CWindowManagerService::HandleReportFocusChange()
{
    AutoPtr<WindowState> lastFocus;
    AutoPtr<WindowState> newFocus;

    {
        Mutex::Autolock lock(mWindowMapLock);

        lastFocus = mLastFocus;
        newFocus = mCurrentFocus;
        if (lastFocus == newFocus) {
            // Focus is not changing, so nothing to do.
            return NOERROR;
        }

        mLastFocus = newFocus;

        if (newFocus != NULL && lastFocus != NULL
            && !newFocus->IsDisplayedLw()) {
            mLosingFocus.PushBack(lastFocus);
            lastFocus = NULL;
        }
    }

    if (lastFocus != newFocus) {
        if (newFocus != NULL) {
            FAIL_RETURN(newFocus->mClient->WindowFocusChanged(TRUE, mInTouchMode));
            NotifyFocusChanged();
        }

        if (lastFocus != NULL) {
            FAIL_RETURN(lastFocus->mClient->WindowFocusChanged(FALSE, mInTouchMode));
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleReportLosingFocus()
{
    List<AutoPtr<WindowState> > losers;

    {
        Mutex::Autolock lock(mWindowMapLock);

        losers.Assign(mLosingFocus.Begin(), mLosingFocus.End());
        mLosingFocus.Clear();
    }

    List<AutoPtr<WindowState> >::Iterator iter = losers.Begin();
    for (; iter != losers.End(); ++iter) {
        FAIL_RETURN((*iter)->mClient->WindowFocusChanged(FALSE, mInTouchMode));
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleAnimate()
{
    Mutex::Autolock lock(mWindowMapLock);
    mAnimationPending = FALSE;

    return PerformLayoutAndPlaceSurfacesLocked();
}

ECode CWindowManagerService::HandleAddStarting(
    /* [in] */ AppWindowToken* wtoken)
{
    StartingData* sd = wtoken->mStartingData;

    if (sd == NULL) {
        // Animation has been canceled... do nothing.
        return NOERROR;
    }

    //if (DEBUG_STARTING_WINDOW) Slog.v(TAG, "Add starting "
    //        + wtoken + ": pkg=" + sd.pkg);

    AutoPtr<IView> view;
    ECode ec = mPolicy->AddStartingWindow(
        wtoken->mToken, sd->mCap,
        sd->mTheme, sd->mNonLocalizedLabel, sd->mLabelRes,
        sd->mIcon, (IView**)&view);

    if (FAILED(ec)) {
        Logger::W(TAG, "Exception when adding starting window");
    }

    if (view != NULL) {
        Boolean abort = FALSE;

        {
            Mutex::Autolock lock(mWindowMapLock);

            if (wtoken->mRemoved || wtoken->mStartingData == NULL) {
                // If the window was successfully added, then
                // we need to remove it.
                if (wtoken->mStartingWindow != NULL) {
                    //if (DEBUG_STARTING_WINDOW) {
                    //    Slog.v(TAG,
                    //        "Aborted starting " + wtoken
                    //        + ": removed=" + wtoken.removed
                    //        + " startingData=" + wtoken.startingData);
                    //}
                    wtoken->mStartingWindow = NULL;
                    wtoken->mStartingData = NULL;
                    abort = TRUE;
                }
            }
            else {
                wtoken->mStartingView = view;
            }

            //if (DEBUG_STARTING_WINDOW && !abort) Slog.v(TAG,
            //        "Added starting " + wtoken
            //        + ": startingWindow="
            //        + wtoken.startingWindow + " startingView="
            //        + wtoken.startingView);
        }

        if (abort) {
            ec = mPolicy->RemoveStartingWindow(wtoken->mToken, view);
            if (FAILED(ec)) {
                Logger::W(TAG, "Exception when removing starting window");
            }
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleRemoveStarting(
    /* [in] */ AppWindowToken* wtoken)
{
    AutoPtr<IBinder> token;
    AutoPtr<IView> view;

    {
        Mutex::Autolock lock(mWindowMapLock);

        //if (DEBUG_STARTING_WINDOW) Slog.v(TAG, "Remove starting "
        //    + wtoken + ": startingWindow="
        //    + wtoken.startingWindow + " startingView="
        //    + wtoken.startingView);

        if (wtoken->mStartingWindow != NULL) {
            view = wtoken->mStartingView;
            token = wtoken->mToken;
            wtoken->mStartingData = NULL;
            wtoken->mStartingView = NULL;
            wtoken->mStartingWindow = NULL;
        }
    }

    if (view != NULL) {
        if (FAILED(mPolicy->RemoveStartingWindow(token, view))) {
            Logger::W(TAG, "Exception when removing starting window");
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleFinishedStarting()
{
    AutoPtr<IBinder> token;
    AutoPtr<IView> view;

    while (TRUE) {
        {
            Mutex::Autolock lock(mWindowMapLock);

            Int32 N = mFinishedStarting.GetSize();
            if (N <= 0) {
                break;
            }

            AppWindowToken* wtoken = mFinishedStarting.GetBack();
            mFinishedStarting.PopBack();

            //if (DEBUG_STARTING_WINDOW) Slog.v(TAG,
            //        "Finished starting " + wtoken
            //        + ": startingWindow=" + wtoken.startingWindow
            //        + " startingView=" + wtoken.startingView);

            if (wtoken->mStartingWindow == NULL) {
                continue;
            }

            view = wtoken->mStartingView;
            token = wtoken->mToken;
            wtoken->mStartingData = NULL;
            wtoken->mStartingView = NULL;
            wtoken->mStartingWindow = NULL;
        }

        if (FAILED(mPolicy->RemoveStartingWindow(token, view))) {
            Logger::W(TAG, "Exception when removing starting window");
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleReportAppTokenWindows(
    /* [in] */ Boolean nowVisible,
    /* [in] */ Boolean nowGone,
    /* [in] */ AppWindowToken* wtoken)
{
    //if (DEBUG_VISIBILITY) Slog.v(
    //    TAG, "Reporting visible in " + wtoken
    //    + " visible=" + nowVisible
    //    + " gone=" + nowGone);

    if (nowVisible) {
        return wtoken->mAppToken->WindowsVisible();
    }
    else {  //nowGone???
        return wtoken->mAppToken->WindowsGone();
    }
}

ECode CWindowManagerService::HandleWindowFreezeTimeout()
{
    Mutex::Autolock lock(mWindowMapLock);

    Logger::W(TAG, "Window freeze timeout expired.");

    List<AutoPtr<WindowState> >::ReverseIterator iter = mWindows.RBegin();
    for (; iter != mWindows.REnd(); ++iter) {
        if ((*iter)->mOrientationChanging) {
            (*iter)->mOrientationChanging = FALSE;
            Logger::W(TAG, "Force clearing orientation change: "/* + w*/);
        }
    }

    return PerformLayoutAndPlaceSurfacesLocked();
}

ECode CWindowManagerService::HandleHoldScreenChanged(
    /* [in] */ IWindowSession* newHold)
{
    AutoPtr<IWindowSession> oldHold;

    {
        Mutex::Autolock lock(mWindowMapLock);

        oldHold = mLastReportedHold;
        mLastReportedHold = newHold;
    }

    if (oldHold.Get() != newHold) {
        //try {
        //    if (oldHold != NULL) {
        //        mBatteryStats->NoteStopWakelock(oldHold.mUid, -1,
        //            "window",
        //            BatteryStats.WAKE_TYPE_WINDOW);
        //    }

        //    if (newHold != NULL) {
        //        mBatteryStats.noteStartWakelock(newHold.mUid, -1,
        //            "window",
        //            BatteryStats.WAKE_TYPE_WINDOW);
        //    }
        //} catch (RemoteException e) {
        //}
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleAppTransitionTimeout()
{
    Mutex::Autolock lock(mWindowMapLock);

    if (mNextAppTransition != WindowManagerPolicy::TRANSIT_UNSET) {
        //if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
        //    "*** APP TRANSITION TIMEOUT");
        mAppTransitionReady = TRUE;
        mAppTransitionTimeout = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
    }

    return NOERROR;
}

ECode CWindowManagerService::HandlePersistAnimationScale()
{
    //Settings.System.putFloat(mContext.getContentResolver(),
    //    Settings.System.WINDOW_ANIMATION_SCALE, mWindowAnimationScale);
    //Settings.System.putFloat(mContext.getContentResolver(),
    //    Settings.System.TRANSITION_ANIMATION_SCALE, mTransitionAnimationScale);

    return NOERROR;
}

ECode CWindowManagerService::HandleForceGc()
{
    {
        Mutex::Autolock lock(mWindowMapLock);

        if (mAnimationPending) {
            // If we are animating, don't do the gc now but
            // delay a bit so we don't interrupt the animation.
            ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
            pHandlerFunc = &CWindowManagerService::HandleForceGc;

            SendMessageDelayed(*(Handle32*)&pHandlerFunc, NULL, 2000);

            return NOERROR;
        }
        // If we are currently rotating the display, it will
        // schedule a new message when done.
        if (mDisplayFrozen) {
            return NOERROR;
        }
        mFreezeGcPending = 0;
    }

    //Runtime.getRuntime().gc();

    return NOERROR;
}

ECode CWindowManagerService::HandleEnableScreen()
{
    PerformEnableScreen();

    return NOERROR;
}

ECode CWindowManagerService::HandleAppFreezeTimeout()
{
    Mutex::Autolock lock(mWindowMapLock);

    Slogger::W(TAG, "App freeze timeout expired.");
    List<AppWindowToken*>::ReverseIterator iter = mAppTokens.RBegin();
    for (; iter != mAppTokens.REnd(); ++iter) {
        if ((*iter)->mFreezingScreen) {
            Slogger::W(TAG, "Force clearing freeze: " /*+ *iter*/);
            UnsetAppFreezingScreenLocked(*iter, TRUE, TRUE);
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleSendNewConfiguration()
{
    ECode (STDCALL CWindowManagerService::*pHandlerFunc)();
    pHandlerFunc = &CWindowManagerService::HandleSendNewConfiguration;

    RemoveMessage(*(Handle32*)&pHandlerFunc);

    return SendNewConfiguration();
}

ECode CWindowManagerService::HandleReportWindowsChange()
{
    if (mWindowsChanged) {
        {
            Mutex::Autolock lock(mWindowMapLock);
            mWindowsChanged = FALSE;
        }
        NotifyWindowsChanged();
    }

    return NOERROR;
}
