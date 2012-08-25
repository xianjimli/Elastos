
#ifndef __CPHONEWINDOW_H__
#define __CPHONEWINDOW_H__

#include "_CPhoneWindow.h"
#include "view/Window.h"
#include "view/RootViewSurfaceTaker.h"
#include "view/BaseSurfaceHolder.h"
#include "widget/FrameLayout.h"
#include <elastos/AutoPtr.h>
#include "view/ViewMacro.h"

class ViewRoot;

CarClass(CPhoneWindow), public Window
{
    friend class ViewRoot;

private:
    class DecorView :
        public ElRefBase,
        public FrameLayout,
        public IFrameLayout,
        public IViewParent,
        public IViewManager,
        public RootViewSurfaceTaker,
        public IDrawableCallback,
        public IKeyEventCallback,
        public IAccessibilityEventSource
    {
    public:
        DecorView(
            /* [in] */ CPhoneWindow* host,
            /* [in] */ IContext* context,
            /* [in] */ Int32 featureId);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        // =========================================================
        // IView
        // =========================================================
        IVIEW_METHODS_DECL();

        // =========================================================
        // IViewGroup
        // =========================================================
        IVIEWGROUP_METHODS_DECL();

        // =========================================================
        // IViewParent
        // =========================================================
        IVIEWPARENT_METHODS_DECL();

        // =========================================================
        // IViewManager
        // =========================================================
        IVIEWMANAGER_METHODS_DECL();

        IDrawableCallback_METHODS_DECL();

        IKeyEventCallback_METHODS_DECL();

        IAccessibilityEventSource_METHODS_DECL();

        CARAPI SetForegroundGravity(
            /* [in] */ Int32 foregroundGravity);

        CARAPI SetForeground(
            /* [in] */ IDrawable* drawable);

        CARAPI GetForeground(
            /* [out] */ IDrawable** foreground);

        CARAPI SetMeasureAllChildren(
            /* [in] */ Boolean measureAll);

        CARAPI GetConsiderGoneChildrenWhenMeasuring(
            /* [out] */ Boolean* measureAll);

        CARAPI WillYouTakeTheSurface(
            /* [out] */ ISurfaceHolderCallback2** cback);

        CARAPI SetSurfaceType(
            /* [in] */ Int32 type);

        CARAPI SetSurfaceFormat(
            /* [in] */ Int32 format);

        CARAPI SetSurfaceKeepScreenOn(
            /* [in] */ Boolean keepOn);

        CARAPI WillYouTakeTheInputQueue(
            /* [out] */ InputQueue::Callback** inputQueueCallback);

    public:
        CARAPI_(void) StartChanging();

        CARAPI_(void) FinishChanging();

        CARAPI_(void) SetWindowBackground(
            /* [in] */ IDrawable* drawable);

        CARAPI_(Boolean) DispatchKeyEvent(
            /* [in] */ IKeyEvent* event);

        CARAPI_(Boolean) DispatchTouchEvent(
            /* [in] */ IMotionEvent* event);

        CARAPI_(Boolean) DispatchTrackballEvent(
            /* [in] */ IMotionEvent* event);

        CARAPI_(Boolean) SuperDispatchKeyEvent(
            /* [in] */ IKeyEvent* event);

        CARAPI_(Boolean) SuperDispatchTouchEvent(
            /* [in] */ IMotionEvent* event);

        CARAPI_(Boolean) SuperDispatchTrackballEvent(
            /* [in] */ IMotionEvent* event);

        CARAPI_(Boolean) OnTouchEvent(
            /* [in] */ IMotionEvent* event);

        CARAPI_(Boolean) OnInterceptTouchEvent(
            /* [in] */ IMotionEvent* event);

        CARAPI_(void) SetWindowFrame(
            /* [in] */ IDrawable* drawable);

    private:
        CARAPI_(void) DrawableChanged();

        CARAPI_(Boolean) IsOutOfBounds(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

    private:
        AutoPtr<CPhoneWindow> mHost;

        /** The feature ID of the panel, or -1 if this is the application's DecorView */
        Int32 mFeatureId;

        AutoPtr<CRect> mDrawingBounds;

        AutoPtr<CRect> mBackgroundPadding;

        AutoPtr<CRect> mFramePadding;

        AutoPtr<CRect> mFrameOffsets;

        Boolean mChanging;

        AutoPtr<IDrawable> mMenuBackground;

        Boolean mWatchingForMenu;
        Int32 mDownY;
    };

public:
    CPhoneWindow();

    ~CPhoneWindow();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetContext(
        /* [out] */ IContext** context);

    CARAPI GetWindowStyle(
        /* [out] */ ITypedArray** attrs);

    CARAPI SetContainer(
        /* [in] */ IWindow* container);

    CARAPI GetContainer(
        /* [out] */ IWindow** container);

    CARAPI HasChildren(
        /* [out] */  Boolean* hasChildren);

    CARAPI SetWindowManager(
        /* [in] */ IWindowManager* wm,
        /* [in] */ IBinder* appToken,
        /* [in] */ const String& appName);

    CARAPI GetWindowManager(
        /* [out] */ IWindowManager** wm);

    CARAPI SetCallback(
        /* [in] */ IWindowCallback* cb);

    CARAPI GetCallback(
        /* [out] */ IWindowCallback** cb);

    CARAPI TakeSurface(
        /* [in] */ ISurfaceHolderCallback2* cb);

    CARAPI TakeInputQueue(
        /* [in] */ InputQueue::Callback* callback);

    CARAPI IsFloating(
        /* [out] */ Boolean* isFloating);

    CARAPI SetLayout(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetType(
        /* [in] */ Int32 type);

    CARAPI SetFormat(
        /* [in] */ Int32 format);

    CARAPI SetWindowAnimations(
        /* [in] */ Int32 resId);

    CARAPI SetSoftInputMode(
        /* [in] */ Int32 mode);

    CARAPI AddFlags(
        /* [in] */ Int32 flags);

    CARAPI ClearFlags(
        /* [in] */ Int32 flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    CARAPI SetAttributes(
        /* [in] */ IWindowManagerLayoutParams* a);

    CARAPI GetAttributes(
        /* [out] */ IWindowManagerLayoutParams** params);

    CARAPI RequestFeature(
        /* [in] */ Int32 featureId,
        /* [out] */ Boolean* result);

    CARAPI MakeActive();

    CARAPI IsActive(
        /* [out] */ Boolean* isActive);

    CARAPI FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView** view);

    CARAPI SetContentView(
        /* [in] */ Int32 layoutResID);

    CARAPI SetContentViewEx(
        /* [in] */ IView* view);

    CARAPI SetContentViewEx2(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI AddContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI GetCurrentFocus(
        /* [out] */ IView** view);

    /**
     * Return a LayoutInflater instance that can be used to inflate XML view layout
     * resources for use in this Window.
     *
     * @return LayoutInflater The shared LayoutInflater.
     */
    CARAPI GetLayoutInflater(
        /* [out] */ ILayoutInflater** inflater);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetTitleColor(
        /* [in] */ Int32 textColor);

    CARAPI OpenPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    CARAPI ClosePanel(
        /* [in] */ Int32 featureId);

    CARAPI TogglePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    CARAPI PerformPanelShortcut(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI PerformPanelIdentifierAction(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI CloseAllPanels();

    CARAPI PerformContextMenuIdentifierAction(
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI SetBackgroundDrawableResource(
        /* [in] */ Int32 resid);

    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI SetFeatureDrawableResource(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 resId);

    CARAPI SetFeatureDrawableUri(
        /* [in] */ Int32 featureId,
        /* [in] */ IUri* uri);

    CARAPI SetFeatureDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable);

    CARAPI SetFeatureDrawableAlpha(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 alpha);

    CARAPI SetFeatureInt(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 value);

    /**
     * Request that key events come to this activity. Use this if your activity
     * has no views with focus, but the activity still wants a chance to process
     * key events.
     */
    CARAPI TakeKeyEvents(
        /* [in] */ Boolean get);

    CARAPI SuperDispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* succeeded);

    CARAPI SuperDispatchTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* succeeded);

    CARAPI SuperDispatchTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* succeeded);

    CARAPI GetDecorView(
        /* [out] */ IView** view);

    CARAPI PeekDecorView(
        /* [out] */ IView** view);

    CARAPI SaveHierarchyState(
        /* [out] */ IBundle** instanceState);

    CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI SetChildDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable);

    CARAPI SetChildInt(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 value);

    CARAPI IsShortcutKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* isShortcutKey);

    CARAPI SetVolumeControlStream(
        /* [in] */ Int32 streamType);

    CARAPI GetVolumeControlStream(
        /* [out] */ Int32* streamType);

    CARAPI constructor(
        /* [in] */ IContext* context);

protected:
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) OnActive();

    CARAPI_(Mutex&) GetSelfSyncLock();

private:
    CARAPI_(AutoPtr<DecorView>) GenerateDecor();

    CARAPI GenerateLayout(
        /* [in] */ DecorView* decor,
        /* [out] */ IViewGroup** viewGroup);

    CARAPI_(void) InstallDecor();

private:
    static const char* TAG;

    static const Boolean SWEEP_OPEN_MENU = FALSE;

private:
    // This is the top-level view of the window, containing the window decor.
    AutoPtr<DecorView> mDecor;

    // This is the view in which the window contents are placed. It is either
    // mDecor itself, or a child of mDecor where the contents go.
    AutoPtr<IViewGroup> mContentParent;

    AutoPtr<ISurfaceHolderCallback2> mTakeSurfaceCallback;

    AutoPtr<BaseSurfaceHolder> mSurfaceHolder;

    InputQueue::Callback* mTakeInputQueueCallback;

    Boolean mIsFloating;

    AutoPtr<ILayoutInflater> mLayoutInflater;

    AutoPtr<ITextView> mTitleView;

    Int32 mBackgroundResource;

    AutoPtr<IDrawable> mBackgroundDrawable;

    Int32 mFrameResource;

    Int32 mTextColor;

    AutoPtr<ICharSequence> mTitle;

    Int32 mTitleColor;
};

#endif // __CPHONEWINDOW_H__
