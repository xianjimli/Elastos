
#include "impl/CPhoneWindow.h"
#include "view/LayoutInflater.h"
#include "view/CViewGroupLayoutParams.h"
#include <Logger.h>
#include <stdio.h>

#include "graphics/CPaint.h"
#include "graphics/ElPixelFormat.h"
#include "view/ElKeyCharacterMap.h"
#include "content/CConfiguration.h"

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
    mWatchingForMenu(FALSE),
    mDefaultOpacity(ElPixelFormat::OPAQUE)
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
//TODO
//    VALIDATE_NOT_NULL(result);
//    *result = FrameLayout::ShowContextMenuForChild(originalView);

    // Reuse the context menu builder
    if (mHost->mContextMenu == NULL) {
        AutoPtr<IContext> context;
        GetContext((IContext**) &context);

        mHost->mContextMenu = new ContextMenuBuilder(context);

        if (mHost->mContextMenuCallback == NULL) {
            mHost->mContextMenuCallback = new CPhoneWindow::ContextMenuCallback(mHost, Window_FEATURE_CONTEXT_MENU);
        }

        mHost->mContextMenu->SetCallback(mHost->mContextMenuCallback);
    } else {
        mHost->mContextMenu->ClearAll();
    }

    AutoPtr<IBinder> binder;
    originalView->GetWindowToken((IBinder**)&binder);

    mHost->mContextMenu->Show(originalView, binder, (CMenuDialogHelper**) &(mHost->mContextMenuHelper));
    *result = mHost->mContextMenuHelper != NULL;

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
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
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
    if (cb == NULL) {
        printf("==== File: %s, Line: %d ====, FUNC : %s, ---------------cb ========null.\n", __FILE__, __LINE__, __FUNCTION__);
    }
    else {
        printf("==== File: %s, Line: %d ====, FUNC : %s, ---------------cb !!!!!!!=null.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    Boolean handled = FALSE;
    //printf("==== File: %s, Line: %d ====, FUNC : %s, cb===[%0x], mFeatureId=[%d].\n", __FILE__, __LINE__, __FUNCTION__, cb, mFeatureId);
    printf("==== File: %s, Line: %d ====, FUNC : %s, mFeatureId=[%d].\n", __FILE__, __LINE__, __FUNCTION__, mFeatureId);
    if (cb != NULL && mFeatureId < 0) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        cb->DispatchKeyEvent(event, &handled);
    }
    else {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        handled = FrameLayout::DispatchKeyEvent(event);
    }
    printf("==== File: %s, Line: %d ====, FUNC : %s, handled==%d.\n", __FILE__, __LINE__, __FUNCTION__, handled);

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
                if (mHost != NULL) {
                    mHost->ClosePanel(mFeatureId);
                }

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

                if (mHost != NULL) {
                    mHost->ClosePanel(mFeatureId);
                }

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
            AutoPtr<IKeyEvent> event;
            CKeyEvent::New(KeyEvent_ACTION_DOWN, KeyEvent_KEYCODE_MENU, (IKeyEvent**) &event);
            mHost->OpenPanel(Window_FEATURE_OPTIONS_PANEL, event);

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
    , mContextMenuHelper(NULL)
    , mContextMenu(NULL)
    , mContextMenuCallback(NULL)
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

ECode CPhoneWindow::OnKeyDownPanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IKeyEvent* event)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (event == NULL) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<IContext> context;
    GetContext((IContext**) &context);

    if (context == NULL) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<IResources> resource;
    context->GetResources((IResources**) &resource);
    if (resource == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 keycode, repeatCount;
    event->GetRepeatCount(&repeatCount);
    event->GetKeyCode(&keycode);

    if (repeatCount == 0) {
        mPanelChordingKey = keycode;
        mPanelMayLongPress = FALSE;

        PanelFeatureState* st = NULL;
        GetPanelState(featureId, TRUE, &st);
        printf("==== File: %s, Line: %d ====, FUNC : %s, st==[0x%08x].\n", __FILE__, __LINE__, __FUNCTION__, st);

        if (!st->isOpen) {
            AutoPtr<IConfiguration> config;
            resource->GetConfiguration((IConfiguration**) &config);
            CConfiguration* src = (CConfiguration*)config.Get();
            if (src == NULL) {
                return E_INVALID_ARGUMENT;
            }

            if (src->mKeyboard == Configuration_KEYBOARD_NOKEYS) {
                mPanelMayLongPress = TRUE;
            }

            Boolean prepared;
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
            return PreparePanel(st, event, &prepared);
        }

    } else if (mPanelMayLongPress && mPanelChordingKey == keycode) {

        Int32 flags;
        event->GetFlags(&flags);
        if ((flags & KeyEvent_FLAG_LONG_PRESS) != 0) {
            // We have had a long press while in a state where this
            // should be executed...  do it!
            mPanelChordingKey = 0;
            mPanelMayLongPress = FALSE;

            //TODO
/*            InputMethodManager imm = (InputMethodManager)
                    getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
            if (imm != null) {
                mDecor.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
                imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
            }*/
        }
    }

    return NOERROR;
}

ECode CPhoneWindow::OpenPanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IKeyEvent* event)
{
    PanelFeatureState* out;
    GetPanelState(featureId, TRUE, &out);

    return OpenPanel(out, event);
}

ECode CPhoneWindow::InitializePanelDecor(
    /* [in] */ PanelFeatureState* st,
    /* [out] */ Boolean* inited) {
    *inited = FALSE;
    if (st == NULL) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<IContext> context;
    GetContext((IContext**) &context);

    st->decorView = new DecorView(this, context, st->featureId);
    st->gravity = Gravity_CENTER | Gravity_BOTTOM;

    st->SetStyle(context);

    *inited = TRUE;
    return NOERROR;
}

ECode CPhoneWindow::InitializePanelContent(
    /* [in] */ PanelFeatureState* st,
    /* [out] */ Boolean* inited) {

    *inited = FALSE;
    if (st->createdPanelView != NULL) {
        st->shownPanelView = st->createdPanelView;
        *inited = TRUE;
        return NOERROR;
    }

    AutoPtr<IMenuBuilder> menu = (IMenuBuilder*)st->menu.Get();
    if (menu == NULL) {
        return NOERROR;
    }

    menu->GetMenuView((st->isInExpandedMode) ? MenuBuilder::TYPE_EXPANDED
            : MenuBuilder::TYPE_ICON, st->decorView, (IMenuView**) &st->shownPanelView);

    if (st->shownPanelView != NULL) {
        // Use the menu View's default animations if it has any
        Int32 defaultAnimations;
        ((IMenuView*)(st->shownPanelView.Get()))->GetWindowAnimations(&defaultAnimations);
        if (defaultAnimations != 0) {
            st->windowAnimations = defaultAnimations;
        }

        *inited = TRUE;

        return NOERROR;
    }

    return NOERROR;
}

ECode CPhoneWindow::OpenPanel(
    /* [in] */ PanelFeatureState* st,
    /* [in] */ IKeyEvent* event) {
    // Already open, return
    if (st->isOpen) {
        return NOERROR;
    }

    AutoPtr<IWindowCallback> cb;
    GetCallback((IWindowCallback**) &cb);

    Boolean isOpened;
    cb->OnMenuOpened(st->featureId, st->menu, &isOpened);
    if ((cb != NULL) && (!isOpened)) {
        // Callback doesn't want the menu to open, reset any state
        return ClosePanelEx(st, TRUE);
    }

    AutoPtr<IWindowManager> wm;
    GetWindowManager((IWindowManager**) &wm);
    if (wm == NULL) {
        return E_INVALID_ARGUMENT;
    }

    // Prepare panel (should have been done before, but just in case)
    Boolean prepared;
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    PreparePanel(st, event, &prepared);
    if (!prepared) {
        return E_INVALID_ARGUMENT;
    }

    if (st->decorView == NULL || st->refreshDecorView) {
        if (st->decorView == NULL) {
            // Initialize the panel decor, this will populate st.decorView
            Boolean inited;
            InitializePanelDecor(st, &inited);

            if (!inited || (st->decorView == NULL)) {
                return E_INVALID_ARGUMENT;
            }
        } else if (st->refreshDecorView) {
            Int32 count;
            st->decorView->GetChildCount(&count);

            if (count > 0) {
                // Decor needs refreshing, so remove its views
                st->decorView->RemoveAllViews();
            }
        }

        // This will populate st.shownPanelView
        Boolean inited;
        InitializePanelContent(st, &inited);
        if (!inited || (st->shownPanelView == NULL)) {
            return E_INVALID_ARGUMENT;
        }

        AutoPtr<IViewGroupLayoutParams> lp;
        st->shownPanelView->GetLayoutParams((IViewGroupLayoutParams**) &lp);
        //ViewGroup.LayoutParams lp = st.shownPanelView.getLayoutParams();
        if (lp == NULL) {
            CViewGroupLayoutParams::New(ViewGroupLayoutParams_WRAP_CONTENT
                    , ViewGroupLayoutParams_WRAP_CONTENT, (IViewGroupLayoutParams**) &lp);
        }

        Int32 backgroundResId, lpWidth;
        lp->GetWidth(&lpWidth);
        if (lpWidth == ViewGroupLayoutParams_MATCH_PARENT) {
            // If the contents is fill parent for the width, set the
            // corresponding background
            backgroundResId = st->fullBackground;
        } else {
            // Otherwise, set the normal panel background
            backgroundResId = st->background;
        }

        AutoPtr<IDrawable> drawable;
        AutoPtr<IContext> context;
        GetContext((IContext**) &context);
        if (context != NULL) {
            AutoPtr<IResources> resource;
            context->GetResources((IResources**) &resource);
            if (resource != NULL) {
                resource->GetDrawable(backgroundResId, (IDrawable**) &drawable);
            }
        }

        if (drawable == NULL) {
            return E_INVALID_ARGUMENT;
        }

        st->decorView->SetWindowBackground(drawable);

        ((IViewGroup*)(st->decorView->Probe(EIID_IViewGroup)))->AddViewEx3((IView*)st->shownPanelView.Get()
                    , (IViewGroupLayoutParams*)lp.Get());

        /*
         * Give focus to the view, if it or one of its children does not
         * already have it.
         */
        Boolean focus;
        st->shownPanelView->HasFocus(&focus);
        if (!focus) {
            Boolean requested;
            st->shownPanelView->RequestFocus(&requested);
        }
    }

    st->isOpen = TRUE;
    st->isHandled = FALSE;

    AutoPtr<IWindowManagerLayoutParams> lp;
    CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* src = (CWindowManagerLayoutParams*)lp.Get();
    src->mWidth = ViewGroupLayoutParams_WRAP_CONTENT;
    src->mHeight = ViewGroupLayoutParams_WRAP_CONTENT;
    src->mX = st->x;
    src->mY = st->y;
    src->mType = WindowManagerLayoutParams_TYPE_APPLICATION_ATTACHED_DIALOG;
    src->mFlags = WindowManagerLayoutParams_FLAG_DITHER | WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM;
    src->mAlpha = st->decorView->mDefaultOpacity;

/*    WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
            WRAP_CONTENT, WRAP_CONTENT,
            st.x, st.y, WindowManager.LayoutParams.TYPE_APPLICATION_ATTACHED_DIALOG,
            WindowManager.LayoutParams.FLAG_DITHER
            | WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM,
            st.decorView.mDefaultOpacity);*/

    src->mGravity = st->gravity;
    src->mWindowAnimations = st->windowAnimations;

    wm->AddViewEx5(st->decorView, lp);
    // Log.v(TAG, "Adding main menu to window manager.");

    return NOERROR;
}

ECode CPhoneWindow::ClosePanel(
    /* [in] */ Int32 featureId)
{
    if (featureId == Window_FEATURE_CONTEXT_MENU) {
        CloseContextMenu();
    } else {
        PanelFeatureState* st;
        GetPanelState(featureId, TRUE, &st);

        ClosePanelEx(st, TRUE);
    }

    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindow::ClosePanelEx(
    /* [in] */ PanelFeatureState* st,
    /* [in] */ Boolean doCallback) {
    AutoPtr<IWindowManager> wm;
    GetWindowManager((IWindowManager**) &wm);
    if ((wm != NULL) && st != NULL && st->isOpen) {
        if (st->decorView != NULL) {
            wm->RemoveView(st->decorView);
        }

        if (doCallback) {
            CallOnPanelClosed(st->featureId, st, NULL);
        }
    }
    st->isPrepared = FALSE;
    st->isHandled = FALSE;
    st->isOpen = FALSE;

    // This view is no longer shown, so null it out
    st->shownPanelView = NULL;

    if (st->isInExpandedMode) {
        // Next time the menu opens, it should not be in expanded mode, so
        // force a refresh of the decor
        st->refreshDecorView = TRUE;
        st->isInExpandedMode = FALSE;
    }

    if (mPreparedPanel == st) {
        mPreparedPanel = NULL;
        mPanelChordingKey = 0;
    }

    return NOERROR;
}

void CPhoneWindow::CallOnPanelClosed(
    /* [in] */ Int32 featureId,
    /* [in] */ PanelFeatureState* panel,
    /* [in] */ IMenu* menu) {
    AutoPtr<IWindowCallback> cb;
    GetCallback((IWindowCallback**) &cb);

    if (cb == NULL) {
        return;
    }

    // Try to get a menu
    if (menu == NULL) {
        // Need a panel to grab the menu, so try to get that
        if (panel == NULL) {
            if ((featureId >= 0) && (featureId < mPanels->GetSize())) {
                panel = (*mPanels)[featureId];
            }
        }

        if (panel != NULL) {
            // menu still may be null, which is okay--we tried our best
            menu = panel->menu;
        }
    }

    // If the panel is not open, do not callback
    if ((panel != NULL) && (!panel->isOpen))
        return;

    cb->OnPanelClosed(featureId, menu);
}

ECode CPhoneWindow::CloseContextMenu() {
    if (mContextMenu != NULL) {
        mContextMenu->Close();
        DismissContextMenu();
    }

    return NOERROR;
}

ECode CPhoneWindow::DismissContextMenu() {
    mContextMenu = NULL;

    if (mContextMenuHelper != NULL) {
        mContextMenuHelper->Dismiss();
        mContextMenuHelper = NULL;
    }

    return NOERROR;
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
    printf("==== File: %s, Line: %d ====, keyCode===[%d].\n", __FILE__, __LINE__, keyCode);
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
            printf("==== File: %s, Line: %d ====, FUNC : %s---------------.\n", __FILE__, __LINE__, __FUNCTION__);
            OnKeyDownPanel((featureId < 0) ? Window_FEATURE_OPTIONS_PANEL : featureId, event);
            printf("==== File: %s, Line: %d ====, FUNC : %s---------------.\n", __FILE__, __LINE__, __FUNCTION__);
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
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
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
                if (featureId == Window_FEATURE_OPTIONS_PANEL) {
                   PanelFeatureState* st;
                   GetPanelState(featureId, FALSE, &st);

                   if (st != NULL && st->isInExpandedMode) {
                       // If the user is in an expanded menu and hits back, it
                       // should go back to the icon menu
                       ReopenMenu(TRUE);
                       return TRUE;
                   }
                }

                ClosePanel(featureId);

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
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
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

ECode CPhoneWindow::PreparePanel(
    /* [in] */ PanelFeatureState* st,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* prepared)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (st == NULL) {
        return E_INVALID_ARGUMENT;
    }

    // Already prepared (isPrepared will be reset to false later)
    if (st->isPrepared) {
        *prepared = TRUE;
        return NOERROR;
    }

    if ((mPreparedPanel != NULL) && (mPreparedPanel != st)) {
        // Another Panel is prepared and possibly open, so close it
        ClosePanelEx(mPreparedPanel, false);
    }

    AutoPtr<IWindowCallback> cb;
    GetCallback((IWindowCallback**) &cb);

    if (cb != NULL) {
        cb->OnCreatePanelView(st->featureId, (IView**)&st->createdPanelView);
    }

printf("==== File: %s, Line: %d ====, FUNC : %s, pointer=[0x%08x].\n", __FILE__, __LINE__, __FUNCTION__, st->createdPanelView.Get());
    if (st->createdPanelView == NULL) {
        // Init the panel state's menu--return false if init failed
        if (st->menu == NULL) {
            Boolean inited;
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
            InitializePanelMenu(st, &inited);
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);

            if (!inited || (st->menu == NULL)) {
                *prepared = FALSE;
                return NOERROR;
            }

            // Call callback, and return if it doesn't want to display menu
            Boolean allowToShow = FALSE;
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
            cb->OnCreatePanelMenu(st->featureId, st->menu, &allowToShow);
            printf("==== File: %s, Line: %d ====, FUNC : %s, allowToShow=[%d].\n", __FILE__, __LINE__, __FUNCTION__, allowToShow);
            if ((cb == NULL) || !allowToShow) {
                printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
                // Ditch the menu created above
                st->menu = NULL;
                *prepared = FALSE;

                return NOERROR;
            }
        }
        else {
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        }

        // Callback and return if the callback does not want to show the menu
        Boolean allow = FALSE;
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        cb->OnPreparePanel(st->featureId, st->createdPanelView, st->menu, &allow);
        printf("==== File: %s, Line: %d ====, FUNC : %s, allow = [%d].\n", __FILE__, __LINE__, __FUNCTION__, allow);
        if (!allow) {
            *prepared = FALSE;

            return NOERROR;
        }

        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        // Set the proper keymap
        Int32 deviceid;
        event->GetDeviceId(&deviceid);
        ElKeyCharacterMap* kmap = ElKeyCharacterMap::Load(event != NULL ? deviceid : 0);


        st->qwertyMode = kmap->GetKeyboardType() != ElKeyCharacterMap::NUMERIC;
        st->menu->SetQwertyMode(st->qwertyMode);
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    }
    else {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    // Set other state
    st->isPrepared = TRUE;
    st->isHandled = FALSE;
    mPreparedPanel = st;
    *prepared = TRUE;
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    return NOERROR;
}

ECode CPhoneWindow::InitializePanelMenu(
    /* [in] */ PanelFeatureState* st,
    /* [out] */ Boolean* inited) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    AutoPtr<IContext> context;
    GetContext((IContext**) &context);
    AutoPtr<CMenuBuilder> menu;
    CMenuBuilder::NewByFriend(context, (CMenuBuilder**) &menu);

    menu->SetCallback((IMenuBuilderCallback*)this);
    st->SetMenu((IMenu*)(menu->Probe(EIID_IMenuBuilder)));

    /**For Test*************************************/
    AutoPtr<IMenuItem> item = NULL;
    AutoPtr<ICharSequence> csq;
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    CStringWrapper::New(String("item1"), (ICharSequence**)&csq);
    //menu->AddEx2(0, 1, 1, csq, (IMenuItem**) &item);
    //menu->Close();
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);

    if (item == NULL) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    }
    else {
        printf("==== File: %s, Line: %d ====, FUNC : %s, item=[0x%08x].\n", __FILE__, __LINE__, __FUNCTION__, item.Get());
    }
    /**For Test*************************************/

    *inited = TRUE;
    return NOERROR;
}

ECode CPhoneWindow::GetPanelState(
    /* [in] */ Int32 featureId,
    /* [in] */ Boolean required,
    /* [out] */ PanelFeatureState** out)
{
    return GetPanelStateEx(featureId, required, NULL, out);
}

ECode CPhoneWindow::GetPanelStateEx(
    /* [in] */ Int32 featureId,
    /* [in] */ Boolean required,
    /* [in] */ PanelFeatureState* convertPanelState,
    /* [out] */ PanelFeatureState** out) {
    printf("==== File: %s, Line: %d ====, FUNC : %s, featureId=[%d], required=[%d].\n", __FILE__, __LINE__, __FUNCTION__,featureId, required);
    if ((GetFeatures() & (1 << featureId)) == 0) {
        if (!required) {
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
            *out = NULL;
            return NOERROR;
        }

        return E_INVALID_ARGUMENT;
        //throw new RuntimeException("The feature has not been requested");
    }

printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);

    List<PanelFeatureState*>* ar;
    if ((ar = mPanels) == NULL || ar->GetSize() <= featureId) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        List<PanelFeatureState*>* nar = new List<PanelFeatureState*>(featureId + 1);
        if (ar != NULL) {
            //System.arraycopy(ar, 0, nar, 0, ar.length);
            List<PanelFeatureState*>::Iterator iter = ar->Begin();

            for (; iter != ar->End(); ++iter) {
                PanelFeatureState* item = (PanelFeatureState*)(*iter);
                nar->PushBack(item);
            }
        }

        mPanels = ar = nar;
    }

    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);

    PanelFeatureState* st = (*ar)[featureId];
    printf("==== File: %s, Line: %d ====, FUNC : %s, st = [0x%08x].\n", __FILE__, __LINE__, __FUNCTION__, st);
    if (st == NULL) {
        (*ar)[featureId] = st = (convertPanelState != NULL)
                ? convertPanelState
                : new PanelFeatureState(featureId);
    }

    *out = st;
    return NOERROR;
}

ECode CPhoneWindow::FindMenuPanel(
    /* [in] */ IMenu* menu,
    /* [out] */ PanelFeatureState** out) {

    for (Int32 i = 0; i < mPanels->GetSize(); i++) {
        PanelFeatureState* panel = (*mPanels)[i];
        if (panel != NULL && panel->menu == menu) {
            *out = panel;
            return NOERROR;
        }
    }

    *out = NULL;
    return NOERROR;
}

ECode CPhoneWindow::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu) {
    return ReopenMenu(TRUE);
}

ECode CPhoneWindow::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* state) {
    AutoPtr<IWindowCallback> cb;
    GetCallback((IWindowCallback**)&cb);

    if (cb != NULL) {
        PanelFeatureState* panel;
        AutoPtr<IMenuBuilder> rootMenu;
        ((IMenuBuilder*)(MenuBuilder*)menu)->GetRootMenu((IMenuBuilder**) &rootMenu);

        FindMenuPanel((IMenu*)rootMenu.Get(), &panel);

        if (panel != NULL) {
            return cb->OnMenuItemSelected(panel->featureId, item, state);
        }
    }

    *state = FALSE;

    return NOERROR;
}

ECode CPhoneWindow::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing) {
    PanelFeatureState* panel;
    FindMenuPanel((IMenu*)menu, &panel);

    if (panel != NULL) {
        // Close the panel and only do the callback if the menu is being
        // closed
        // completely, not if opening a sub menu
        return ClosePanelEx(panel, allMenusAreClosing);
    }

    return NOERROR;
}

ECode CPhoneWindow::OnSubMenuSelected(
    /* [in] */ ISubMenu* subMenu,
    /* [out] */ Boolean* state) {
    Boolean has;
    subMenu->HasVisibleItems(&has);

    *state = TRUE;
    if (!has) {
        return NOERROR;
    }

    // The window manager will give us a valid window token
    //new MenuDialogHelper(subMenu).show(null);
    CMenuDialogHelper* temp = new CMenuDialogHelper((IMenuBuilder*)subMenu);
    if (temp != NULL) {
        temp->Show(NULL);
    }

    //TODO
    delete temp;
    temp = NULL;

    return NOERROR;
}

ECode CPhoneWindow::OnCloseSubMenu(
    /* [in] */ ISubMenu* menu) {
    AutoPtr<IMenu> parentMenu;
    ((IMenuBuilder*)menu)->GetRootMenu((IMenuBuilder**) &parentMenu);

    PanelFeatureState* panel;
    FindMenuPanel((IMenu*)parentMenu.Get(), &panel);

    // Callback
    if (panel != NULL) {
        CallOnPanelClosed(panel->featureId, panel, parentMenu);
        ClosePanelEx(panel, TRUE);
    }

    return NOERROR;
}

ECode CPhoneWindow::ReopenMenu(
    /* [in] */ Boolean toggleMenuMode) {
    PanelFeatureState* st;
    GetPanelState(Window_FEATURE_OPTIONS_PANEL, TRUE, &st);

    // Save the future expanded mode state since closePanel will reset it
    Boolean newExpandedMode = toggleMenuMode ? !st->isInExpandedMode : st->isInExpandedMode;

    st->refreshDecorView = TRUE;
    ClosePanelEx(st, FALSE);

    // Set the expanded mode state
    st->isInExpandedMode = newExpandedMode;

    return OpenPanel(st, NULL);
}
