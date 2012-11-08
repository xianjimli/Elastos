
#ifndef __CPHONEWINDOW_H__
#define __CPHONEWINDOW_H__

#include "_CPhoneWindow.h"
#include "view/Window.h"
#include "view/RootViewSurfaceTaker.h"
#include "view/BaseSurfaceHolder.h"
#include "widget/FrameLayout.h"
#include <elastos/AutoPtr.h>
#include "view/ViewMacro.h"
#include "os/CBundle.h"
#include "view/menu/ContextMenuBuilder.h"

class ViewRoot;

CarClass(CPhoneWindow)
    , public Window
    , IMenuBuilderCallback
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
        CPhoneWindow* mHost;

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

    public:
        Int32 mDefaultOpacity;
    };

    class PanelFeatureState : public ElRefBase
    {
    private:
        class SavedState
            : public ElRefBase
            , public IParcelable
        {
        public:
            SavedState();

            CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

            Int32 DescribeContents();

            CARAPI WriteToParcel(
                /* [in] */ IParcel* dest);

            CARAPI ReadFromParcel(
                /* [in] */ IParcel* source);

        public:
            Int32 mFeatureId;
            Boolean mIsOpen;
            Boolean mIsInExpandedMode;
            AutoPtr<IBundle> mMenuState;
        };

    public:
        PanelFeatureState(
            /* [in] */ Int32 featureId);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) SetStyle(
            /* [in] */ IContext* context);

        CARAPI_(void) SetMenu(
            /* [in] */ IMenu* menu);

        CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

        CARAPI_(void) OnRestoreInstanceState(
            /* [in] */ IParcelable* state);

    public:
        /** Feature ID for this panel. */
        Int32 mFeatureId;

        // Information pulled from the style for this panel.

        Int32 mBackground;

        /** The background when the panel spans the entire available width. */
        Int32 mFullBackground;

        Int32 mGravity;

        Int32 mX;

        Int32 mY;

        Int32 mWindowAnimations;

        /** Dynamic state of the panel. */
        AutoPtr<DecorView> mDecorView;

        /** The panel that was returned by onCreatePanelView(). */
        AutoPtr<IView> mCreatedPanelView;

        /** The panel that we are actually showing. */
        AutoPtr<IView> mShownPanelView;

        /** Use {@link #setMenu} to set this. */
        AutoPtr<IMenu> mMenu;

        /**
         * Whether the panel has been prepared (see
         * {@link PhoneWindow#preparePanel}).
         */
        Boolean mIsPrepared;

        /**
         * Whether an item's action has been performed. This happens in obvious
         * scenarios (user clicks on menu item), but can also happen with
         * chording menu+(shortcut key).
         */
        Boolean mIsHandled;

        Boolean mIsOpen;

        /**
         * True if the menu is in expanded mode, false if the menu is in icon
         * mode
         */
        Boolean mIsInExpandedMode;

        Boolean mQwertyMode;

        Boolean mRefreshDecorView;

        Boolean mWasLastOpen;

        Boolean mWasLastExpanded;

        /**
         * Contains the state of the menu when told to freeze.
         */
        AutoPtr<IBundle> mFrozenMenuState;
    };

    /**
     * Simple implementation of MenuBuilder.Callback that:
     * <li> Opens a submenu when selected.
     * <li> Calls back to the callback's onMenuItemSelected when an item is
     * selected.
     */
    class ContextMenuCallback:
        public ElRefBase,
        public IMenuBuilderCallback
    {
    public:
        ContextMenuCallback(
            /* [in] */ Int32 featureId,
            /* [in] */ CPhoneWindow* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* iinterface,
            /* [in] */ InterfaceID* id);

        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        CARAPI OnCloseSubMenu(
            /* [in] */ ISubMenuBuilder* menu);

        CARAPI OnMenuItemSelected(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* state);

        CARAPI OnMenuModeChange(
            /* [in] */ IMenuBuilder* menu);

        CARAPI OnSubMenuSelected(
            /* [in] */ ISubMenuBuilder* subMenu,
            /* [out] */ Boolean* state);

    private:
        Int32 mFeatureId;
        AutoPtr<MenuDialogHelper> mSubMenuHelper;
        CPhoneWindow* mHost;
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

    /**
     * Prepares the panel to either be opened or chorded. This creates the Menu
     * instance for the panel and populates it via the Activity callbacks.
     *
     * @param st The panel state to prepare.
     * @param event The event that triggered the preparing of the panel.
     * @return Whether the panel was prepared. If the panel should not be shown,
     *         returns false.
     */
    CARAPI PreparePanel(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* prepared);

    CARAPI OpenPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    CARAPI ClosePanel(
        /* [in] */ Int32 featureId);

    /**
     * Closes the given panel.
     *
     * @param st The panel to be closed.
     * @param doCallback Whether to notify the callback that the panel was
     *            closed. If the panel is in the process of re-opening or
     *            opening another panel (e.g., menu opening a sub menu), the
     *            callback should not happen and this variable should be false.
     *            In addition, this method internally will only perform the
     *            callback if the panel is open.
     */
    CARAPI ClosePanel(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ Boolean doCallback);

    CARAPI TogglePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    /**
     * Called when the panel key is pushed down.
     * @param featureId The feature ID of the relevant panel (defaults to FEATURE_OPTIONS_PANEL}.
     * @param event The key event.
     * @return Whether the key was handled.
     */
    CARAPI OnKeyDownPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* handled);

    /**
     * Called when the panel key is released.
     * @param featureId The feature ID of the relevant panel (defaults to FEATURE_OPTIONS_PANEL}.
     * @param event The key event.
     */
    CARAPI OnKeyUpPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    CARAPI CloseAllPanels();

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

    CARAPI PerformContextMenuIdentifierAction(
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI_(AutoPtr<PanelFeatureState>) FindMenuPanel(
        /* [in] */ IMenu* menu);

    CARAPI OnMenuItemSelected(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* state);

    CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    CARAPI OnCloseSubMenu(
        /* [in] */ ISubMenuBuilder* menu);

    CARAPI OnSubMenuSelected(
        /* [in] */ ISubMenuBuilder* subMenu,
        /* [out] */ Boolean* state);

    CARAPI OnMenuModeChange(
        /* [in] */ IMenuBuilder* menu);

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
    /**
     * Initializes the menu associated with the given panel feature state. You
     * must at the very least set PanelFeatureState.menu to the Menu to be
     * associated with the given panel state. The default implementation creates
     * a new menu for the panel state.
     *
     * @param st The panel whose menu is being initialized.
     * @return Whether the initialization was successful.
     */
    CARAPI_(Boolean) InitializePanelMenu(
        /* [in] */ PanelFeatureState* st);

    /**
     * Perform initial setup of a panel. This should at the very least set the
     * style information in the PanelFeatureState and must set
     * PanelFeatureState.decor to the panel's window decor view.
     *
     * @param st The panel being initialized.
     */
    CARAPI_(Boolean) InitializePanelDecor(
        /* [in] */ PanelFeatureState* st);

    /**
     * Initializes the panel associated with the panel feature state. You must
     * at the very least set PanelFeatureState.panel to the View implementing
     * its contents. The default implementation gets the panel from the menu.
     *
     * @param st The panel state being initialized.
     * @return Whether the initialization was successful.
     */
    CARAPI_(Boolean) InitializePanelContent(
        /* [in] */ PanelFeatureState* st);

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
    CARAPI_(void) OpenPanel(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ IKeyEvent* event);

    /**
     * Closes the context menu. This notifies the menu logic of the close, along
     * with dismissing it from the UI.
     */
    /* synchronized */
    CARAPI_(void) CloseContextMenu();

    /**
     * Dismisses just the context menu UI. To close the context menu, use
     * {@link #closeContextMenu()}.
     */
    /* synchronized */
    CARAPI_(void) DismissContextMenu();

    CARAPI_(void) ReopenMenu(
        /* [in] */ Boolean toggleMenuMode);

    /**
     * Opens the panels that have had their state restored. This should be
     * called sometime after {@link #restorePanelState} when it is safe to add
     * to the window manager.
     */
    CARAPI_(void) OpenPanelsAfterRestore();

    CARAPI_(AutoPtr<DecorView>) GenerateDecor();

    CARAPI GenerateLayout(
        /* [in] */ DecorView* decor,
        /* [out] */ IViewGroup** viewGroup);

    CARAPI_(void) InstallDecor();

    /**
     * Gets a panel's state based on its feature ID.
     *
     * @param featureId The feature ID of the panel.
     * @param required Whether the panel is required (if it is required and it
     *            isn't in our features, this throws an exception).
     * @return The panel state.
     */
    CARAPI_(AutoPtr<PanelFeatureState>) GetPanelState(
        /* [in] */ Int32 featureId,
        /* [in] */ Boolean required);

    /**
     * Gets a panel's state based on its feature ID.
     *
     * @param featureId The feature ID of the panel.
     * @param required Whether the panel is required (if it is required and it
     *            isn't in our features, this throws an exception).
     * @param convertPanelState Optional: If the panel state does not exist, use
     *            this as the panel state.
     * @return The panel state.
     */
    CARAPI_(AutoPtr<PanelFeatureState>) GetPanelState(
        /* [in] */ Int32 featureId,
        /* [in] */ Boolean required,
        /* [in] */ PanelFeatureState* convertPanelState);

    /**
     * Helper method for calling the {@link Callback#onPanelClosed(int, Menu)}
     * callback. This method will grab whatever extra state is needed for the
     * callback that isn't given in the parameters. If the panel is not open,
     * this will not perform the callback.
     *
     * @param featureId Feature ID of the panel that was closed. Must be given.
     * @param panel Panel that was closed. Optional but useful if there is no
     *            menu given.
     * @param menu The menu that was closed. Optional, but give if you have.
     */
    CARAPI_(void) CallOnPanelClosed(
        /* [in] */ Int32 featureId,
        /* [in] */ PanelFeatureState* panel,
        /* [in] */ IMenu* menu);

private:
    static const CString TAG;

    static const Boolean SWEEP_OPEN_MENU = FALSE;

private:
    /**
     * Simple callback used by the context menu and its submenus. The options
     * menu submenus do not use this (their behavior is more complex).
     */
    AutoPtr<ContextMenuCallback> mContextMenuCallback;

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

    ArrayOf< AutoPtr<PanelFeatureState> >* mPanels;

    /**
     * The panel that is prepared or opened (the most recent one if there are
     * multiple panels). Shortcuts will go to this panel. It gets set in
     * {@link #preparePanel} and cleared in {@link #closePanel}.
     */
    AutoPtr<PanelFeatureState> mPreparedPanel;

    /**
     * The keycode that is currently held down (as a modifier) for chording. If
     * this is 0, there is no key held down.
     */
    Int32 mPanelChordingKey;
    Boolean mPanelMayLongPress;

    Int32 mBackgroundResource;

    AutoPtr<IDrawable> mBackgroundDrawable;

    Int32 mFrameResource;

    Int32 mTextColor;

    AutoPtr<ICharSequence> mTitle;

    Int32 mTitleColor;

    AutoPtr<ContextMenuBuilder> mContextMenu;
    AutoPtr<MenuDialogHelper> mContextMenuHelper;
};

#endif // __CPHONEWINDOW_H__
