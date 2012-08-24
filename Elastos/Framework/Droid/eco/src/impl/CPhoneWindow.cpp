
#include "impl/CPhoneWindow.h"
#include "view/LayoutInflater.h"
#include "view/CViewGroupLayoutParams.h"
#include <Logger.h>
#include <stdio.h>

#include "graphics/CPaint.h"

using namespace Elastos::Utility::Logging;

// {45CF35EA-34D4-43a3-A1C7-2649411BAA61}
extern "C" const InterfaceID EIID_RootViewSurfaceTaker =
    {0x45cf35ea, 0x34d4, 0x43a3, {0xa1, 0xc7, 0x26, 0x49, 0x41, 0x1b, 0xaa, 0x61}};

const char* CPhoneWindow::TAG = "PhoneWindow";

IVIEW_METHODS_IMPL(
    CPhoneWindow::DecorView, FrameLayout, CPhoneWindow::DecorView);

IVIEWGROUP_METHODS_IMPL(
    CPhoneWindow::DecorView, FrameLayout, CPhoneWindow::DecorView);

IDrawableCallback_METHODS_IMPL(
    CPhoneWindow::DecorView, FrameLayout, CPhoneWindow::DecorView);

IKeyEventCallback_METHODS_IMPL(
    CPhoneWindow::DecorView, FrameLayout, CPhoneWindow::DecorView);

IAccessibilityEventSource_METHODS_IMPL(
    CPhoneWindow::DecorView, FrameLayout, CPhoneWindow::DecorView);

CPhoneWindow::DecorView::DecorView(
    /* [in] */ CPhoneWindow* host,
    /* [in] */ IContext* context,
    /* [in] */ Int32 featureId) :
    FrameLayout(context),
    mHost(host),
    mFeatureId(featureId),
    mWatchingForMenu(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mDrawingBounds));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mBackgroundPadding));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mFramePadding));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mFrameOffsets));
}

PInterface CPhoneWindow::DecorView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IFrameLayout*)this;
    }
    else if (riid == EIID_IView) {
        return (IView*)this;
    }
    else if (riid == EIID_IFrameLayout) {
        return (IFrameLayout*)this;
    }
    else if (riid == EIID_IViewGroup) {
        return (IViewGroup*)(IFrameLayout*)this;
    }
    else if (riid == EIID_IViewParent) {
        return (IViewParent*)this;
    }
    else if (riid == EIID_IViewManager) {
        return (IViewManager*)this;
    }
    else if (riid == EIID_IDrawableCallback) {
        return (IDrawableCallback*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    }
    else if (riid == EIID_IAccessibilityEvent) {
        return (IAccessibilityEvent*)this;
    }
    else if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)(FrameLayout*)this);
    }
    else if (riid == EIID_RootViewSurfaceTaker) {
        return reinterpret_cast<PInterface>((RootViewSurfaceTaker*)this);
    }

    return NULL;
}

UInt32 CPhoneWindow::DecorView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPhoneWindow::DecorView::Release()
{
    return ElRefBase::Release();
}

ECode CPhoneWindow::DecorView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::DecorView::RequestLayoutEx()
{
    return RequestLayout();
}

ECode CPhoneWindow::DecorView::IsLayoutRequestedEx(
    /* [out] */ Boolean* result)
{
    return IsLayoutRequested(result);
}

ECode CPhoneWindow::DecorView::RequestTransparentRegion(
    /* [in] */ IView* child)
{
    return FrameLayout::RequestTransparentRegion(child);
}

ECode CPhoneWindow::DecorView::InvalidateChild(
    /* [in] */ IView* child,
    /* [in] */ IRect* r)
{
    return FrameLayout::InvalidateChild(child, r);
}

ECode CPhoneWindow::DecorView::InvalidateChildInParent(
    /* [in] */ ArrayOf<Int32>* location,
    /* [in] */ IRect* r,
    /* [out] */ IViewParent** parent)
{
    VALIDATE_NOT_NULL(parent);
    AutoPtr<IViewParent> temp = FrameLayout::InvalidateChildInParent(location, r);
    *parent = temp;
    if (*parent) {
        (*parent)->AddRef();
    }

    return NOERROR;
}

ECode CPhoneWindow::DecorView::GetParentEx(
    /* [out] */ IViewParent** parent)
{
    return GetParent(parent);
}

ECode CPhoneWindow::DecorView::RequestChildFocus(
    /* [in] */ IView* child,
    /* [in] */ IView* focused)
{
    return FrameLayout::RequestChildFocus(child, focused);
}

ECode CPhoneWindow::DecorView::RecomputeViewAttributes(
    /* [in] */ IView* child)
{
    return FrameLayout::RecomputeViewAttributes(child);
}

ECode CPhoneWindow::DecorView::ClearChildFocus(
    /* [in] */ IView* child)
{
    return FrameLayout::ClearChildFocus(child);
}

ECode CPhoneWindow::DecorView::GetChildVisibleRect(
    /* [in] */ IView* child,
    /* [in] */ IRect* r,
    /* [in] */ IPoint* offset,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return FrameLayout::GetChildVisibleRect(child, r, offset, result);
}

ECode CPhoneWindow::DecorView::FocusSearchEx(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out] */ IView** focus)
{
    VALIDATE_NOT_NULL(focus);
    AutoPtr<IView> v = FrameLayout::FocusSearch(focused, direction);
    *focus = v.Get();
    if (*focus) {
        (*focus)->AddRef();
    }

    return NOERROR;
}

ECode CPhoneWindow::DecorView::BringChildToFront(
    /* [in] */ IView* child)
{
    return FrameLayout::BringChildToFront(child);
}

ECode CPhoneWindow::DecorView::FocusableViewAvailable(
    /* [in] */ IView* v)
{
    return FrameLayout::FocusableViewAvailable(v);
}

ECode CPhoneWindow::DecorView::ShowContextMenuForChild(
    /* [in] */ IView* originalView,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FrameLayout::ShowContextMenuForChild(originalView);

    return NOERROR;
}

ECode CPhoneWindow::DecorView::ChildDrawableStateChanged(
    /* [in] */ IView* child)
{
    return FrameLayout::ChildDrawableStateChanged(child);
}

ECode CPhoneWindow::DecorView::RequestDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    return FrameLayout::RequestDisallowInterceptTouchEvent(disallowIntercept);
}

ECode CPhoneWindow::DecorView::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FrameLayout::RequestChildRectangleOnScreen(child, rectangle, immediate);

    return NOERROR;
}

ECode CPhoneWindow::DecorView::AddViewEx5(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return FrameLayout::AddView(view, params);
}

ECode CPhoneWindow::DecorView::UpdateViewLayout(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return FrameLayout::UpdateViewLayout(view, params);
}

ECode CPhoneWindow::DecorView::RemoveView(
    /* [in] */ IView* view)
{
    return FrameLayout::RemoveView(view);
}

ECode CPhoneWindow::DecorView::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return FrameLayout::SetForegroundGravity(foregroundGravity);
}

ECode CPhoneWindow::DecorView::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return FrameLayout::SetForeground(drawable);
}

ECode CPhoneWindow::DecorView::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = FrameLayout::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode CPhoneWindow::DecorView::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return FrameLayout::SetMeasureAllChildren(measureAll);
}

ECode CPhoneWindow::DecorView::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll)
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();

    return NOERROR;
}

ECode CPhoneWindow::DecorView::WillYouTakeTheSurface(
    /* [out] */ ISurfaceHolderCallback2** cback)
{
    VALIDATE_NOT_NULL(cback);
    if (mFeatureId < 0) {
        *cback = mHost->mTakeSurfaceCallback;
        if (*cback) {
            (*cback)->AddRef();
        }
    }
    else {
        *cback = NULL;
    }

    return NOERROR;
}

ECode CPhoneWindow::DecorView::SetSurfaceType(
    /* [in] */ Int32 type)
{
    return mHost->SetType(type);
}

ECode CPhoneWindow::DecorView::SetSurfaceFormat(
    /* [in] */ Int32 format)
{
    return mHost->SetFormat(format);
}

ECode CPhoneWindow::DecorView::SetSurfaceKeepScreenOn(
    /* [in] */ Boolean keepOn)
{
    if (keepOn) {
        mHost->AddFlags(WindowManagerLayoutParams_FLAG_KEEP_SCREEN_ON);
    }
    else {
        mHost->ClearFlags(WindowManagerLayoutParams_FLAG_KEEP_SCREEN_ON);
    }

    return NOERROR;
}

ECode CPhoneWindow::DecorView::WillYouTakeTheInputQueue(
    /* [out] */ InputQueue::Callback** inputQueueCallback)
{
    VALIDATE_NOT_NULL(inputQueueCallback);
    if (mFeatureId < 0) {
        *inputQueueCallback = mHost->mTakeInputQueueCallback;
    }
    else {
        *inputQueueCallback = NULL;
    }

    return NOERROR;
}

void CPhoneWindow::DecorView::StartChanging()
{
    mChanging = TRUE;
}

void CPhoneWindow::DecorView::FinishChanging()
{
    mChanging = FALSE;
    DrawableChanged();
}

void CPhoneWindow::DecorView::SetWindowBackground(
    /* [in] */ IDrawable* drawable)
{
    AutoPtr<IDrawable> bg;
    GetBackground((IDrawable**)&bg);
    if (bg.Get() != drawable) {
        SetBackgroundDrawable(drawable);
        if (drawable != NULL) {
            Boolean result;
            drawable->GetPadding((IRect*)mBackgroundPadding.Get(), &result);
        } else {
            mBackgroundPadding->SetEmpty();
        }
        DrawableChanged();
    }
}

void CPhoneWindow::DecorView::SetWindowFrame(
    /* [in] */ IDrawable* drawable)
{
    AutoPtr<IDrawable> bg;
    GetBackground((IDrawable**)&bg);
    if (bg.Get() != drawable) {
        SetForeground(drawable);
        if (drawable != NULL) {
            Boolean result;
            drawable->GetPadding((IRect*)mFramePadding.Get(), &result);
        } else {
            mFramePadding->SetEmpty();
        }
        DrawableChanged();
    }
}

Boolean CPhoneWindow::DecorView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    Int32 action;
    event->GetAction(&action);
    Boolean isDown = action == KeyEvent_ACTION_DOWN;

    /*
     * If the user hits another key within the play sound delay, then
     * cancel the sound
     */
    //if (keyCode != KeyEvent_KEYCODE_VOLUME_DOWN
    //    && keyCode != KeyEvent_KEYCODE_VOLUME_UP
    //    && mVolumeKeyUpTime + VolumePanel.PLAY_SOUND_DELAY
    //    > SystemClock.uptimeMillis()) {
    //    /*
    //     * The user has hit another key during the delay (e.g., 300ms)
    //     * since the last volume key up, so cancel any sounds.
    //     */
    //    AudioManager audioManager = (AudioManager) getContext().getSystemService(
    //            Context.AUDIO_SERVICE);
    //    if (audioManager != NULL) {
    //        audioManager.adjustSuggestedStreamVolume(AudioManager.ADJUST_SAME,
    //                mVolumeControlStreamType, AudioManager.FLAG_REMOVE_SOUND_AND_VIBRATE);
    //    }
    //}

    Int32 repeatCount;
    event->GetRepeatCount(&repeatCount);
    //if (isDown && (repeatCount == 0)) {
    //    // First handle chording of panel key: if a panel key is held
    //    // but not released, try to execute a shortcut in it.
    //    if ((mPanelChordingKey > 0) && (mPanelChordingKey != keyCode)) {
    //        // Perform the shortcut (mPreparedPanel can be NULL since
    //        // global shortcuts (such as search) don't rely on a
    //        // prepared panel or menu).
    //        Boolean handled = PerformPanelShortcut(mPreparedPanel, keyCode, event,
    //                Menu.FLAG_PERFORM_NO_CLOSE);

    //        if (!handled) {
    //            /*
    //             * If not handled, then pass it to the view hierarchy
    //             * and anyone else that may be interested.
    //             */
    //            DispatchKeyShortcutEvent(event, &handled);

    //            if (handled && mPreparedPanel != NULL) {
    //                mPreparedPanel.isHandled = TRUE;
    //            }
    //        }

    //        if (handled) {
    //            return TRUE;
    //        }
    //    }

    //    // If a panel is open, perform a shortcut on it without the
    //    // chorded panel key
    //    if ((mPreparedPanel != NULL) && mPreparedPanel.isOpen) {
    //        if (performPanelShortcut(mPreparedPanel, keyCode, event, 0)) {
    //            return TRUE;
    //        }
    //    }
    //}

    AutoPtr<IWindowCallback> cb;
    mHost->GetCallback((IWindowCallback**)&cb);
    Boolean handled = FALSE;
    if (cb != NULL && mFeatureId < 0) {
        cb->DispatchKeyEvent(event, &handled);
    }
    else {
        handled = FrameLayout::DispatchKeyEvent(event);
    }

    if (handled) {
        return TRUE;
    }

    return isDown ? mHost->OnKeyDown(mFeatureId, keyCode, event)
            : mHost->OnKeyUp(mFeatureId, keyCode, event);
}

Boolean CPhoneWindow::DecorView::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IWindowCallback> cb;
    mHost->GetCallback((IWindowCallback**)&cb);

    Boolean handled = FALSE;
    if (cb != NULL && mFeatureId < 0) {
        cb->DispatchTouchEvent(event, &handled);
    }
    else {
        handled = FrameLayout::DispatchTouchEvent(event);
    }

    return handled;
}

Boolean CPhoneWindow::DecorView::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IWindowCallback> cb;
    mHost->GetCallback((IWindowCallback**)&cb);

    Boolean handled = FALSE;
    if (cb != NULL && mFeatureId < 0) {
        cb->DispatchTrackballEvent(event, &handled);
    }
    else {
        handled = FrameLayout::DispatchTrackballEvent(event);
    }

    return handled;
}

Boolean CPhoneWindow::DecorView::SuperDispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return FrameLayout::DispatchKeyEvent(event);
}

Boolean CPhoneWindow::DecorView::SuperDispatchTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    return FrameLayout::DispatchTouchEvent(event);
}

Boolean CPhoneWindow::DecorView::SuperDispatchTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    return FrameLayout::DispatchTrackballEvent(event);
}

Boolean CPhoneWindow::DecorView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    return OnInterceptTouchEvent(event);
}

Boolean CPhoneWindow::DecorView::IsOutOfBounds(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return x < -5 || y < -5 || x > (FrameLayout::GetWidth() + 5)
        || y > (FrameLayout::GetHeight() + 5);
}

Boolean CPhoneWindow::DecorView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    event->GetAction(&action);
    if (mFeatureId >= 0) {
        if (action == MotionEvent_ACTION_DOWN) {
            Float fx, fy;
            event->GetX(&fx);
            event->GetY(&fy);
            Int32 x = (Int32)fx;
            Int32 y = (Int32)fy;
            if (IsOutOfBounds(x, y)) {
                //ClosePanel(mFeatureId);
                return TRUE;
            }
        }
    }

    if (!CPhoneWindow::SWEEP_OPEN_MENU) {
        return FALSE;
    }

    Float fy;
    event->GetY(&fy);
    if (mFeatureId >= 0) {
        if (action == MotionEvent_ACTION_DOWN) {
            Logger::D(TAG, "Watchiing!");
            mWatchingForMenu = TRUE;
            mDownY = (Int32)fy;
            return FALSE;
        }

        if (!mWatchingForMenu) {
            return FALSE;
        }

        Int32 y = (Int32)fy;
        if (action == MotionEvent_ACTION_MOVE) {
            if (y > (mDownY + 30)) {
                Logger::D(TAG, "Closing!");
                //closePanel(mFeatureId);
                mWatchingForMenu = FALSE;
                return TRUE;
            }
        }
        else if (action == MotionEvent_ACTION_UP) {
            mWatchingForMenu = FALSE;
        }

        return FALSE;
    }

    if (action == MotionEvent_ACTION_DOWN) {
        Int32 y = fy;
        Boolean hasChildren;
        mHost->HasChildren(&hasChildren);
        if (y >= (FrameLayout::GetHeight()-5) && !hasChildren) {
            Logger::D(TAG, "Watchiing!");
            mWatchingForMenu = TRUE;
        }
        return FALSE;
    }

    if (!mWatchingForMenu) {
        return FALSE;
    }

    Int32 y = fy;
    if (action == MotionEvent_ACTION_MOVE) {
        if (y < (FrameLayout::GetHeight()-30)) {
            Logger::D(TAG, "Opening!");
            //openPanel(FEATURE_OPTIONS_PANEL, new KeyEvent(
            //        KeyEvent_ACTION_DOWN, KeyEvent_KEYCODE_MENU));
            mWatchingForMenu = FALSE;
            return TRUE;
        }
    }
    else if (action == MotionEvent_ACTION_UP) {
        mWatchingForMenu = FALSE;
    }

    return FALSE;
}

void CPhoneWindow::DecorView::DrawableChanged() {
//    if (mChanging) {
//        return;
//    }
//
//    setPadding(mFramePadding.left + mBackgroundPadding.left, mFramePadding.top
//            + mBackgroundPadding.top, mFramePadding.right + mBackgroundPadding.right,
//            mFramePadding.bottom + mBackgroundPadding.bottom);
//    requestLayout();
//    invalidate();
//
//    Int32 opacity = PixelFormat.OPAQUE;
//
//    // Note: if there is no background, we will assume opaque. The
//    // common case seems to be that an application sets there to be
//    // no background so it can draw everything itself. For that,
//    // we would like to assume OPAQUE and let the app force it to
//    // the slower TRANSLUCENT mode if that is really what it wants.
//    Drawable bg = getBackground();
//    Drawable fg = getForeground();
//    if (bg != NULL) {
//        if (fg == NULL) {
//            opacity = bg.getOpacity();
//        } else if (mFramePadding.left <= 0 && mFramePadding.top <= 0
//                && mFramePadding.right <= 0 && mFramePadding.bottom <= 0) {
//            // If the frame padding is zero, then we can be opaque
//            // if either the frame -or- the background is opaque.
//            Int32 fop = fg.getOpacity();
//            Int32 bop = bg.getOpacity();
//            if (Config.LOGV)
//                Log.v(TAG, "Background opacity: " + bop + ", Frame opacity: " + fop);
//            if (fop == PixelFormat.OPAQUE || bop == PixelFormat.OPAQUE) {
//                opacity = PixelFormat.OPAQUE;
//            } else if (fop == PixelFormat.UNKNOWN) {
//                opacity = bop;
//            } else if (bop == PixelFormat.UNKNOWN) {
//                opacity = fop;
//            } else {
//                opacity = Drawable.resolveOpacity(fop, bop);
//            }
//        } else {
//            // For now we have to assume translucent if there is a
//            // frame with padding... there is no way to tell if the
//            // frame and background together will draw all pixels.
//            if (Config.LOGV)
//                Log.v(TAG, "Padding: " + mFramePadding);
//            opacity = PixelFormat.TRANSLUCENT;
//        }
//    }
//
//    if (Config.LOGV)
//        Log.v(TAG, "Background: " + bg + ", Frame: " + fg);
//    if (Config.LOGV)
//        Log.v(TAG, "Selected default opacity: " + opacity);
//
//    mDefaultOpacity = opacity;
//    if (mFeatureId < 0) {
//        setDefaultWindowFormat(opacity);
//    }
}

CPhoneWindow::CPhoneWindow()
    : mTakeInputQueueCallback(NULL)
    , mBackgroundResource(0)
    , mFrameResource(0)
    , mTextColor(0)
    , mTitleColor(0)
{}

CPhoneWindow::~CPhoneWindow()
{
    delete mTakeInputQueueCallback;
}

ECode CPhoneWindow::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    LayoutInflater::From(context, (ILayoutInflater**)&mLayoutInflater);
    return NOERROR;
}

PInterface CPhoneWindow::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Window) {
        return reinterpret_cast<PInterface>((Window*)this);
    }
    return _CPhoneWindow::Probe(riid);
}

UInt32 CPhoneWindow::AddRef()
{
    return _CPhoneWindow::AddRef();
}

UInt32 CPhoneWindow::Release()
{
    return _CPhoneWindow::Release();
}

ECode CPhoneWindow::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return _CPhoneWindow::GetInterfaceID(pObject, pIID);
}

ECode CPhoneWindow::SetContainer(
    /* [in] */ IWindow* container)
{
    return Window::SetContainer(container);
}

ECode CPhoneWindow::RequestFeature(
    /* [in] */ Int32 featureId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mContentParent != NULL) {
//        throw new AndroidRuntimeException("requestFeature() must be called before adding content");
        return E_RUNTIME_EXCEPTION;
    }
    const Int32 features = GetFeatures();
    if ((features != DEFAULT_FEATURES) && (featureId == Window_FEATURE_CUSTOM_TITLE)) {
        /* Another feature is enabled and the user is trying to enable the custom title feature */
//        throw new AndroidRuntimeException("You cannot combine custom titles with other title features");
        return E_RUNTIME_EXCEPTION;
    }
    if (((features & (1 << Window_FEATURE_CUSTOM_TITLE)) != 0) &&
         (featureId != Window_FEATURE_CUSTOM_TITLE)) {
        /* Custom title feature is enabled and the user is trying to enable another feature */
//        throw new AndroidRuntimeException("You cannot combine custom titles with other title features");
        return E_RUNTIME_EXCEPTION;
    }
    if (featureId == Window_FEATURE_OPENGL) {
        mWindowAttributes->mMemoryType = WindowManagerLayoutParams_MEMORY_TYPE_GPU;
    }
    return Window::RequestFeature(featureId, result);
}

ECode CPhoneWindow::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    if (mContentParent == NULL) {
        InstallDecor();
    }
    else {
        mContentParent->RemoveAllViews();
    }

    AutoPtr<IView> root;
    mLayoutInflater->Inflate(layoutResID, mContentParent.Get(), (IView**)&root);
    AutoPtr<IWindowCallback> cb;
    GetCallback((IWindowCallback**)&cb);
    if (cb != NULL) {
        cb->OnContentChanged();
    }

    return NOERROR;
}

ECode CPhoneWindow::SetContentViewEx(
    /* [in] */ IView* view)
{
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        ViewGroupLayoutParams::MATCH_PARENT,
        ViewGroupLayoutParams::MATCH_PARENT,
        (IViewGroupLayoutParams**)&params);

    return SetContentViewEx2(view, params);
}

ECode CPhoneWindow::SetContentViewEx2(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (mContentParent == NULL) {
        InstallDecor();
    }
    else {
        mContentParent->RemoveAllViews();
    }

    mContentParent->AddViewEx3(view, params);

    AutoPtr<IWindowCallback> cb;
    GetCallback((IWindowCallback**)&cb);
    if (cb != NULL) {
        cb->OnContentChanged();
    }

    return NOERROR;
}

ECode CPhoneWindow::AddContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (mContentParent == NULL) {
        InstallDecor();
    }
    FAIL_RETURN(mContentParent->AddViewEx3(view, params));
    AutoPtr<IWindowCallback> cb;
    GetCallback((IWindowCallback**)&cb);
    if (cb != NULL) {
        cb->OnContentChanged();
    }
    return NOERROR;
}

ECode CPhoneWindow::GetCurrentFocus(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    if (mDecor != NULL) {
        return mDecor->FindFocus(view);
    }
    else {
        *view = NULL;
        return NOERROR;
    }
}

ECode CPhoneWindow::TakeSurface(
    /* [in] */ ISurfaceHolderCallback2* cb)
{
    mTakeSurfaceCallback = cb;
    return NOERROR;
}

ECode CPhoneWindow::TakeInputQueue(
    /* [in] */ InputQueue::Callback* callback)
{
    mTakeInputQueueCallback = callback;

    return NOERROR;
}

ECode CPhoneWindow::IsFloating(
    /* [out] */ Boolean* isFloating)
{
    VALIDATE_NOT_NULL(isFloating);
    *isFloating = mIsFloating;
    return NOERROR;
}

ECode CPhoneWindow::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater);
    *inflater = mLayoutInflater;
    if (*inflater) (*inflater)->AddRef();
    return NOERROR;
}

ECode CPhoneWindow::SetTitle(
    /* [in] */ ICharSequence* title)
{
    if (mTitleView != NULL) {
        mTitleView->SetText(title);
    }
    mTitle = title;
    return NOERROR;
}

ECode CPhoneWindow::SetTitleColor(
    /* [in] */ Int32 textColor)
{
    if (mTitleView != NULL) {
        mTitleView->SetTextColor(textColor);
    }
    mTitleColor = textColor;
    return NOERROR;
}

ECode CPhoneWindow::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::OpenPanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IKeyEvent* event)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::ClosePanel(
    /* [in] */ Int32 featureId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::TogglePanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IKeyEvent* event)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::CloseAllPanels()
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::PerformPanelShortcut(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::PerformPanelIdentifierAction(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::PerformContextMenuIdentifierAction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* drawable)
{
    if (drawable != mBackgroundDrawable || mBackgroundResource != 0) {
        mBackgroundResource = 0;
        mBackgroundDrawable = drawable;
        if (mDecor != NULL) {
            mDecor->SetWindowBackground(drawable);
        }
    }
    return NOERROR;
}

ECode CPhoneWindow::SetFeatureDrawableResource(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 resId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::SetFeatureDrawableUri(
    /* [in] */ Int32 featureId,
    /* [in] */ IUri* uri)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::SetFeatureDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::SetFeatureDrawableAlpha(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 alpha)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::SetFeatureInt(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::TakeKeyEvents(
    /* [in] */ Boolean get)
{
    return mDecor->SetFocusable(get);
}

ECode CPhoneWindow::SuperDispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    *succeeded = mDecor->SuperDispatchKeyEvent(event);
    return NOERROR;
}

ECode CPhoneWindow::SuperDispatchTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    *succeeded = mDecor->SuperDispatchTouchEvent(event);
    return NOERROR;
}

ECode CPhoneWindow::SuperDispatchTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    *succeeded = mDecor->SuperDispatchTrackballEvent(event);
    return NOERROR;
}

/**
 * A key was pressed down and not handled by anything else in the window.
 *
 * @see #onKeyUp
 * @see android.view.KeyEvent
 */
Boolean CPhoneWindow::OnKeyDown(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IDispatcherState> dispatcher;
    if (mDecor != NULL) {
        mDecor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);
    }

    switch (keyCode) {
    case KeyEvent_KEYCODE_VOLUME_UP:
    case KeyEvent_KEYCODE_VOLUME_DOWN:
        {
            //AudioManager audioManager = (AudioManager) getContext().getSystemService(
            //        Context.AUDIO_SERVICE);
            //if (audioManager != NULL) {
            //    /*
            //     * Adjust the volume in on key down since it is more
            //     * responsive to the user.
            //     */
            //    audioManager.adjustSuggestedStreamVolume(
            //            keyCode == KeyEvent_KEYCODE_VOLUME_UP
            //                    ? AudioManager.ADJUST_RAISE
            //                    : AudioManager.ADJUST_LOWER,
            //            mVolumeControlStreamType,
            //            AudioManager.FLAG_SHOW_UI | AudioManager.FLAG_VIBRATE);
            //}
            return TRUE;
        }
    case KeyEvent_KEYCODE_MEDIA_PLAY_PAUSE:
        {
            /* Suppress PLAYPAUSE toggle when phone is ringing or in-call
             * to avoid music playback */
            //if (mTelephonyManager == NULL) {
            //    mTelephonyManager = (TelephonyManager) getContext().getSystemService(
            //        Context.TELEPHONY_SERVICE);
            //}
            //if (mTelephonyManager != NULL &&
            //    mTelephonyManager.getCallState() != TelephonyManager.CALL_STATE_IDLE) {
            //        return TRUE;  // suppress key event
            //}
        }
    case KeyEvent_KEYCODE_MUTE:
    case KeyEvent_KEYCODE_HEADSETHOOK:
    case KeyEvent_KEYCODE_MEDIA_STOP:
    case KeyEvent_KEYCODE_MEDIA_NEXT:
    case KeyEvent_KEYCODE_MEDIA_PREVIOUS:
    case KeyEvent_KEYCODE_MEDIA_REWIND:
    case KeyEvent_KEYCODE_MEDIA_FAST_FORWARD:
        {
            //Intent intent = new Intent(Intent.ACTION_MEDIA_BUTTON, NULL);
            //intent.putExtra(Intent.EXTRA_KEY_EVENT, event);
            //getContext().sendOrderedBroadcast(intent, NULL);
            return TRUE;
        }
    case KeyEvent_KEYCODE_CAMERA:
        {
            //if (getKeyguardManager().inKeyguardRestrictedInputMode()
            //        || dispatcher == NULL) {
            //    break;
            //}
            //if (event.getRepeatCount() == 0) {
            //    dispatcher.startTracking(event, this);
            //} else if (event.isLongPress() && dispatcher.isTracking(event)) {
            //    dispatcher.performedLongPress(event);
            //    mDecor.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
            //    sendCloseSystemWindows();
            //    // Broadcast an intent that the Camera button was longpressed
            //    Intent intent = new Intent(Intent.ACTION_CAMERA_BUTTON, NULL);
            //    intent.putExtra(Intent.EXTRA_KEY_EVENT, event);
            //    getContext().sendOrderedBroadcast(intent, NULL);
            //}
            return TRUE;
        }
    case KeyEvent_KEYCODE_MENU:
        {
            //onKeyDownPanel((featureId < 0) ? FEATURE_OPTIONS_PANEL : featureId, event);
            return TRUE;
        }
    case KeyEvent_KEYCODE_BACK:
        {
            Int32 repeatCount;
            event->GetRepeatCount(&repeatCount);
            if (repeatCount > 0) {
                break;
            }
            if (featureId < 0) {
                break;
            }
            // Currently don't do anything with long press.
            dispatcher->StartTracking(event, this->Probe(EIID_IInterface));
            return TRUE;
        }
    case KeyEvent_KEYCODE_CALL:
        {
            //if (getKeyguardManager().inKeyguardRestrictedInputMode()
            //        || dispatcher == NULL) {
            //    break;
            //}
            //if (event.getRepeatCount() == 0) {
            //    dispatcher.startTracking(event, this);
            //} else if (event.isLongPress() && dispatcher.isTracking(event)) {
            //    dispatcher.performedLongPress(event);
            //    mDecor.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
            //    // launch the VoiceDialer
            //    Intent intent = new Intent(Intent.ACTION_VOICE_COMMAND);
            //    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            //    try {
            //        sendCloseSystemWindows();
            //        getContext().startActivity(intent);
            //    } catch (ActivityNotFoundException e) {
            //        startCallActivity();
            //    }
            //}
            return TRUE;
        }
    case KeyEvent_KEYCODE_SEARCH:
        {
            //if (getKeyguardManager().inKeyguardRestrictedInputMode()
            //        || dispatcher == NULL) {
            //    break;
            //}
            //if (event.getRepeatCount() == 0) {
            //    dispatcher.startTracking(event, this);
            //} else if (event.isLongPress() && dispatcher.isTracking(event)) {
            //    Configuration config = getContext().getResources().getConfiguration();
            //    if (config.keyboard == Configuration.KEYBOARD_NOKEYS
            //            || config.hardKeyboardHidden
            //                    == Configuration.HARDKEYBOARDHIDDEN_YES) {
            //        // launch the search activity
            //        Intent intent = new Intent(Intent.ACTION_SEARCH_LONG_PRESS);
            //        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            //        try {
            //            mDecor.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
            //            sendCloseSystemWindows();
            //            getSearchManager().stopSearch();
            //            getContext().startActivity(intent);
            //            // Only clear this if we successfully start the
            //            // activity; otherwise we will allow the normal short
            //            // press action to be performed.
            //            dispatcher.performedLongPress(event);
            //            return TRUE;
            //        } catch (ActivityNotFoundException e) {
            //            // Ignore
            //        }
            //    }
            //}
        }
        break;
    default:
        break;
    }

    return FALSE;
}

/**
 * A key was released and not handled by anything else in the window.
 *
 * @see #onKeyDown
 * @see android.view.KeyEvent
 */
Boolean CPhoneWindow::OnKeyUp(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IDispatcherState> dispatcher;
    if (mDecor != NULL) {
        mDecor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);
    }

    if (dispatcher != NULL) {
        dispatcher->HandleUpEvent(event);
    }
    //Log.i(TAG, "Key up: repeat=" + event.getRepeatCount()
    //        + " flags=0x" + Integer.toHexString(event.getFlags()));

    switch (keyCode) {
    case KeyEvent_KEYCODE_VOLUME_UP:
    case KeyEvent_KEYCODE_VOLUME_DOWN:
        {
            //if (!event.isCanceled()) {
            //    AudioManager audioManager = (AudioManager) getContext().getSystemService(
            //            Context.AUDIO_SERVICE);
            //    if (audioManager != NULL) {
            //        /*
            //         * Play a sound. This is done on key up since we don't want the
            //         * sound to play when a user holds down volume down to mute.
            //         */
            //        audioManager.adjustSuggestedStreamVolume(
            //                AudioManager.ADJUST_SAME,
            //                mVolumeControlStreamType,
            //                AudioManager.FLAG_PLAY_SOUND);
            //        mVolumeKeyUpTime = SystemClock.uptimeMillis();
            //    }
            //}
            return TRUE;
        }
    case KeyEvent_KEYCODE_MENU:
        {
            //onKeyUpPanel(featureId < 0 ? FEATURE_OPTIONS_PANEL : featureId,
            //        event);
            return TRUE;
        }
    case KeyEvent_KEYCODE_BACK:
        {
            if (featureId < 0) {
                break;
            }

            Boolean isTracking, isCanceled;
            event->IsTracking(&isTracking);
            event->IsCanceled(&isCanceled);
            if (isTracking && !isCanceled) {
                //if (featureId == FEATURE_OPTIONS_PANEL) {
                //    PanelFeatureState st = getPanelState(featureId, FALSE);
                //    if (st != NULL && st.isInExpandedMode) {
                //        // If the user is in an expanded menu and hits back, it
                //        // should go back to the icon menu
                //        reopenMenu(TRUE);
                //        return TRUE;
                //    }
                //}
                //closePanel(featureId);
                return TRUE;
            }
        }
        break;
    case KeyEvent_KEYCODE_HEADSETHOOK:
    case KeyEvent_KEYCODE_MEDIA_PLAY_PAUSE:
    case KeyEvent_KEYCODE_MEDIA_STOP:
    case KeyEvent_KEYCODE_MEDIA_NEXT:
    case KeyEvent_KEYCODE_MEDIA_PREVIOUS:
    case KeyEvent_KEYCODE_MEDIA_REWIND:
    case KeyEvent_KEYCODE_MEDIA_FAST_FORWARD:
        {
            //Intent intent = new Intent(Intent.ACTION_MEDIA_BUTTON, NULL);
            //intent.putExtra(Intent.EXTRA_KEY_EVENT, event);
            //getContext().sendOrderedBroadcast(intent, NULL);
            return TRUE;
        }
    case KeyEvent_KEYCODE_CAMERA:
        {
            //if (getKeyguardManager().inKeyguardRestrictedInputMode()) {
            //    break;
            //}
            //if (event.isTracking() && !event.isCanceled()) {
            //    // Add short press behavior here if desired
            //}
            return TRUE;
        }
    case KeyEvent_KEYCODE_CALL:
        {
            //if (getKeyguardManager().inKeyguardRestrictedInputMode()) {
            //    break;
            //}
            //if (event.isTracking() && !event.isCanceled()) {
            //    startCallActivity();
            //}
            return TRUE;
        }
    case KeyEvent_KEYCODE_SEARCH:
        {
            ///*
            // * Do this in onKeyUp since the Search key is also used for
            // * chording quick launch shortcuts.
            // */
            //if (getKeyguardManager().inKeyguardRestrictedInputMode()) {
            //    break;
            //}
            //if (event.isTracking() && !event.isCanceled()) {
            //    launchDefaultSearch();
            //}
            return TRUE;
        }
    }

    return FALSE;
}

void CPhoneWindow::OnActive()
{
}

ECode CPhoneWindow::GetDecorView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    if (mDecor == NULL) {
        InstallDecor();
    }
    *view = mDecor.Get();
    if (*view) {
        (*view)->AddRef();
    }
    return NOERROR;
}

ECode CPhoneWindow::PeekDecorView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    *view = (IView*)mDecor;
    (*view)->AddRef();
    return NOERROR;
}

ECode CPhoneWindow::SaveHierarchyState(
    /* [out] */ IBundle** instanceState)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::RestoreHierarchyState(
    /* [in] */ IBundle* savedInstanceState)
{
    return E_NOT_IMPLEMENTED;
}

AutoPtr<CPhoneWindow::DecorView> CPhoneWindow::GenerateDecor()
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<CPhoneWindow::DecorView> decor =
            new DecorView(this, context.Get(), -1);
    return decor;
}

ECode CPhoneWindow::GenerateLayout(
    /* [in] */ DecorView* decor,
    /* [out] */ IViewGroup** viewGroup)
{
    // Apply data from current theme.

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(GetWindowStyle((ITypedArray**)&a));

//    if (FALSE) {
//        System.out.println("From style:");
//        String s = "Attrs:";
//        for (Int32 i = 0; i < com.android.internal.R.styleable.Window.length; i++) {
//            s = s + " " + Integer.toHexString(com.android.internal.R.styleable.Window[i]) + "="
//                    + a.getString(i);
//        }
//        System.out.println(s);
//    }

    a->GetBoolean(4/*com.android.internal.R.styleable.Window_windowIsFloating*/,
            FALSE, &mIsFloating);
    Int32 flagsToUpdate = (WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN
            | WindowManagerLayoutParams_FLAG_LAYOUT_INSET_DECOR)
            & (~GetForcedWindowFlags());
    if (mIsFloating) {
        SetLayout(ViewGroupLayoutParams_WRAP_CONTENT, ViewGroupLayoutParams_WRAP_CONTENT);
        SetFlags(0, flagsToUpdate);
    }
    else {
        SetFlags(WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN
                | WindowManagerLayoutParams_FLAG_LAYOUT_INSET_DECOR, flagsToUpdate);
    }

    Boolean value;
    a->GetBoolean(3/*com.android.internal.R.styleable.Window_windowNoTitle*/,
            FALSE, &value);
    if (value) {
        RequestFeature(Window_FEATURE_NO_TITLE, &value);
    }

    a->GetBoolean(9/*com.android.internal.R.styleable.Window_windowFullscreen*/,
            FALSE, &value);
    if (value) {
        SetFlags(WindowManagerLayoutParams_FLAG_FULLSCREEN,
                WindowManagerLayoutParams_FLAG_FULLSCREEN & (~GetForcedWindowFlags()));
    }

    a->GetBoolean(14/*com.android.internal.R.styleable.Window_windowShowWallpaper*/,
            FALSE, &value);
    if (value) {
        SetFlags(WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER,
                WindowManagerLayoutParams_FLAG_SHOW_WALLPAPER & (~GetForcedWindowFlags()));
    }

    AutoPtr<IWindowManagerLayoutParams> _params;
    GetAttributes((IWindowManagerLayoutParams**)&_params);
    CWindowManagerLayoutParams* params = (CWindowManagerLayoutParams*)_params.Get();

    if (!HasSoftInputMode()) {
        a->GetInt32(13/*com.android.internal.R.styleable.Window_windowSoftInputMode*/,
                params->mSoftInputMode, &(params->mSoftInputMode));
    }

    a->GetBoolean(11/*com.android.internal.R.styleable.Window_backgroundDimEnabled*/,
            mIsFloating, &value);
    if (value) {
        /* All dialogs should have the window dimmed */
        if ((GetForcedWindowFlags() & WindowManagerLayoutParams_FLAG_DIM_BEHIND) == 0) {
            params->mFlags |= WindowManagerLayoutParams_FLAG_DIM_BEHIND;
        }
        a->GetFloat(0/*android.R.styleable.Window_backgroundDimAmount*/,
                0.5f, &(params->mDimAmount));
    }

    if (params->mWindowAnimations == 0) {
        a->GetResourceId(8/*com.android.internal.R.styleable.Window_windowAnimationStyle*/,
                0, &(params->mWindowAnimations));
    }

    // The rest are only done if this window is not embedded; otherwise,
    // the values are inherited from our container.
    AutoPtr<IWindow> container;
    GetContainer((IWindow**)&container);
    if (container == NULL) {
        if (mBackgroundDrawable == NULL) {
            if (mBackgroundResource == 0) {
                a->GetResourceId(1/*com.android.internal.R.styleable.Window_windowBackground*/,
                        0, &mBackgroundResource);
            }
            if (mFrameResource == 0) {
                a->GetResourceId(2/*com.android.internal.R.styleable.Window_windowFrame*/,
                        0, &mFrameResource);
            }
//            if (FALSE) {
//                System.out.println("Background: "
//                        + Integer.toHexString(mBackgroundResource) + " Frame: "
//                        + Integer.toHexString(mFrameResource));
//            }
        }
        a->GetColor(7/*com.android.internal.R.styleable.Window_textColor*/, 0xFF000000, &mTextColor);
    }

    // Inflate the window decor.

    Int32 layoutResource = 0;
    Int32 features = GetLocalFeatures();
    // System.out.println("Features: 0x" + Integer.toHexString(features));
    if ((features & ((1 << Window_FEATURE_LEFT_ICON) | (1 << Window_FEATURE_RIGHT_ICON))) != 0) {
        if (mIsFloating) {
            layoutResource = 0x01090023/*com.android.internal.R.layout.dialog_title_icons*/;
        } else {
            layoutResource = 0x01090059/*com.android.internal.R.layout.screen_title_icons*/;
        }
        // System.out.println("Title Icons!");
    } else if ((features & ((1 << Window_FEATURE_PROGRESS) | (1 << Window_FEATURE_INDETERMINATE_PROGRESS))) != 0) {
        // Special case for a window with only a progress bar (and title).
        // XXX Need to have a no-title version of embedded windows.
        layoutResource = 0x01090056/*com.android.internal.R.layout.screen_progress*/;
        // System.out.println("Progress!");
    } else if ((features & (1 << Window_FEATURE_CUSTOM_TITLE)) != 0) {
        // Special case for a window with a custom title.
        // If the window is floating, we need a dialog layout
        if (mIsFloating) {
            layoutResource = 0x01090021/*com.android.internal.R.layout.dialog_custom_title*/;
        } else {
            layoutResource = 0x01090055/*com.android.internal.R.layout.screen_custom_title*/;
        }
    } else if ((features & (1 << Window_FEATURE_NO_TITLE)) == 0) {
        // If no other features and not embedded, only need a title.
        // If the window is floating, we need a dialog layout
        if (mIsFloating) {
            layoutResource = 0x01090022/*com.android.internal.R.layout.dialog_title*/;
        } else {
            layoutResource = 0x01090058/*com.android.internal.R.layout.screen_title*/;
        }
        // System.out.println("Title!");
    } else {
        // Embedded, so no decoration is needed.
        layoutResource = 0x01090057/*com.android.internal.R.layout.screen_simple*/;
        // System.out.println("Simple!");
    }

    mDecor->StartChanging();
printf("==== File: %s, Line: %d, layoutResource: %p ====\n", __FILE__, __LINE__, layoutResource);
    AutoPtr<IView> in;
    mLayoutInflater->Inflate(layoutResource, NULL, (IView**)&in);

    AutoPtr<IViewGroupLayoutParams> vparams;
    CViewGroupLayoutParams::New(
            ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_MATCH_PARENT,
            (IViewGroupLayoutParams**)&vparams);
    decor->AddViewEx3(in.Get(), vparams.Get());

    AutoPtr<IView> tmp;
    FindViewById(ID_ANDROID_CONTENT, (IView**)&tmp);
    if (tmp == NULL) {
        Logger::E("CPhoneWindow", "Window couldn't find content container view");
        return E_RUNTIME_EXCEPTION;
    }
    AutoPtr<IViewGroup> contentParent = (IViewGroup*)tmp->Probe(EIID_IViewGroup);
    if (contentParent == NULL) {
        Logger::E("CPhoneWindow", "Window couldn't find content container view");
        return E_RUNTIME_EXCEPTION;
    }

    if ((features & (1 << Window_FEATURE_INDETERMINATE_PROGRESS)) != 0) {
//        ProgressBar progress = getCircularProgressBar(FALSE);
//        if (progress != NULL) {
//            progress.setIndeterminate(TRUE);
//        }
    }

    // Remaining setup -- of background and title -- that only applies
    // to top-level windows.
    container = NULL;
    GetContainer((IWindow**)&container);
    if (container == NULL) {
        AutoPtr<IDrawable> drawable = mBackgroundDrawable;
        if (mBackgroundResource != 0) {
            AutoPtr<IContext> context;
            GetContext((IContext**)&context);
            AutoPtr<IResources> res;
            context->GetResources((IResources**)&res);
            drawable = NULL;
            res->GetDrawable(mBackgroundResource, (IDrawable**)&drawable);
        }
        mDecor->SetWindowBackground(drawable);
        drawable = NULL;
        if (mFrameResource != 0) {
            AutoPtr<IContext> context;
            GetContext((IContext**)&context);
            AutoPtr<IResources> res;
            context->GetResources((IResources**)&res);
            res->GetDrawable(mFrameResource, (IDrawable**)&drawable);
        }
        mDecor->SetWindowFrame(drawable);

        // System.out.println("Text=" + Integer.toHexString(mTextColor) +
        // " Sel=" + Integer.toHexString(mTextSelectedColor) +
        // " Title=" + Integer.toHexString(mTitleColor));

        if (mTitleColor == 0) {
            mTitleColor = mTextColor;
        }

        if (mTitle != NULL) {
            SetTitle(mTitle);
        }
        SetTitleColor(mTitleColor);
    }

    mDecor->FinishChanging();

    *viewGroup = contentParent.Get();
    (*viewGroup)->AddRef();

    return NOERROR;
}

void CPhoneWindow::InstallDecor()
{
    if (mDecor == NULL) {
        mDecor = GenerateDecor();
        mDecor->SetDescendantFocusability(ViewGroup::FOCUS_AFTER_DESCENDANTS);
        mDecor->SetIsRootNamespace(TRUE);
    }

    if (mContentParent == NULL) {
        ASSERT_SUCCEEDED(GenerateLayout(mDecor, (IViewGroup**)&mContentParent));

        FindViewById(0x01020016/*com.android.internal.R.id.title*/, (IView**)&mTitleView);
        if (mTitleView != NULL) {
            if ((GetLocalFeatures() & (1 << Window_FEATURE_NO_TITLE)) != 0) {
                AutoPtr<IView> titleContainer;
                FindViewById(0x01020176/*com.android.internal.R.id.title_container*/,
                        (IView**)&titleContainer);
                if (titleContainer != NULL) {
                    titleContainer->SetVisibility(View_GONE);
                }
                else {
                    mTitleView->SetVisibility(View_GONE);
                }
                if (IFrameLayout::Probe(mContentParent) != NULL) {
                    IFrameLayout::Probe(mContentParent)->SetForeground(NULL);
                }
            }
            else {
                mTitleView->SetText(mTitle);
            }
        }
    }
}

ECode CPhoneWindow::SetChildDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::SetChildInt(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::IsShortcutKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isShortcutKey)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::SetVolumeControlStream(
    /* [in] */ Int32 streamType)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::GetVolumeControlStream(
    /* [out] */ Int32* streamType)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    return Window::GetContext(context);
}

ECode CPhoneWindow::GetWindowStyle(
    /* [out] */ ITypedArray** attrs)
{
    VALIDATE_NOT_NULL(attrs);
    return Window::GetWindowStyle(attrs);
}

ECode CPhoneWindow::GetContainer(
    /* [out] */ IWindow** container)
{
    VALIDATE_NOT_NULL(container);
    return Window::GetContainer(container);
}

ECode CPhoneWindow::HasChildren(
    /* [out] */  Boolean* hasChildren)
{
    VALIDATE_NOT_NULL(hasChildren);
    return Window::HasChildren(hasChildren);
}

ECode CPhoneWindow::SetWindowManager(
    /* [in] */ IWindowManager* wm,
    /* [in] */ IBinder* appToken,
    /* [in] */ const String& appName)
{
    return Window::SetWindowManager(wm, appToken, appName);
}

ECode CPhoneWindow::GetWindowManager(
    /* [out] */ IWindowManager** wm)
{
    return Window::GetWindowManager(wm);
}

ECode CPhoneWindow::SetCallback(
    /* [in] */ IWindowCallback* cb)
{
    return Window::SetCallback(cb);
}

ECode CPhoneWindow::GetCallback(
    /* [out] */ IWindowCallback** cb)
{
    VALIDATE_NOT_NULL(cb);
    return Window::GetCallback(cb);
}

ECode CPhoneWindow::SetLayout(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Window::SetLayout(width, height);
}

ECode CPhoneWindow::SetGravity(
    /* [in] */ Int32 gravity)
{
    return Window::SetGravity(gravity);
}

ECode CPhoneWindow::SetType(
    /* [in] */ Int32 type)
{
    return Window::SetType(type);
}

ECode CPhoneWindow::SetFormat(
    /* [in] */ Int32 format)
{
    return Window::SetFormat(format);
}

ECode CPhoneWindow::SetWindowAnimations(
    /* [in] */ Int32 resId)
{
    return Window::SetWindowAnimations(resId);
}

ECode CPhoneWindow::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    return Window::SetSoftInputMode(mode);
}

ECode CPhoneWindow::AddFlags(
    /* [in] */ Int32 flags)
{
    return Window::AddFlags(flags);
}

ECode CPhoneWindow::ClearFlags(
    /* [in] */ Int32 flags)
{
    return Window::ClearFlags(flags);
}

ECode CPhoneWindow::SetFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    return Window::SetFlags(flags, mask);
}

ECode CPhoneWindow::SetAttributes(
    /* [in] */ IWindowManagerLayoutParams* a)
{
    return Window::SetAttributes(a);
}

ECode CPhoneWindow::GetAttributes(
    /* [out] */ IWindowManagerLayoutParams** params)
{
    return Window::GetAttributes(params);
}

ECode CPhoneWindow::MakeActive()
{
    return Window::MakeActive();
}

ECode CPhoneWindow::IsActive(
    /* [out] */ Boolean* isActive)
{
    VALIDATE_NOT_NULL(isActive);
    return Window::IsActive(isActive);
}

ECode CPhoneWindow::FindViewById(
    /* [in] */ Int32 id,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    return Window::FindViewById(id, view);
}

ECode CPhoneWindow::SetBackgroundDrawableResource(
    /* [in] */ Int32 resid)
{
    return Window::SetBackgroundDrawableResource(resid);
}

Mutex& CPhoneWindow::GetSelfSyncLock()
{
    return _m_syncLock;
}
