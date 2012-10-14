
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
#include "view/menu/CMenuDialogHelper.h"

class ViewRoot;
class CMenuDialogHelper;

static Int32 R_styleable_Theme[] = {
    0x01010030, 0x01010031, 0x01010032, 0x01010033,
    0x01010034, 0x01010035, 0x01010036, 0x01010037,
    0x01010038, 0x01010039, 0x0101003a, 0x0101003b,
    0x0101003c, 0x0101003d, 0x0101003e, 0x0101003f,
    0x01010040, 0x01010041, 0x01010042, 0x01010043,
    0x01010044, 0x01010045, 0x01010046, 0x01010047,
    0x01010048, 0x01010049, 0x0101004a, 0x0101004b,
    0x0101004c, 0x0101004d, 0x0101004e, 0x0101004f,
    0x01010050, 0x01010051, 0x01010052, 0x01010053,
    0x01010054, 0x01010055, 0x01010056, 0x01010057,
    0x01010058, 0x01010059, 0x0101005a, 0x0101005b,
    0x0101005c, 0x0101005d, 0x0101005e, 0x0101005f,
    0x01010060, 0x01010061, 0x01010062, 0x0101006a,
    0x0101006b, 0x0101006c, 0x0101006d, 0x0101006e,
    0x0101006f, 0x01010070, 0x01010071, 0x01010072,
    0x01010073, 0x01010074, 0x01010075, 0x01010076,
    0x01010077, 0x01010078, 0x01010079, 0x0101007a,
    0x0101007b, 0x0101007c, 0x0101007d, 0x0101007e,
    0x01010080, 0x01010081, 0x01010082, 0x01010083,
    0x01010084, 0x01010085, 0x01010086, 0x01010087,
    0x01010088, 0x01010089, 0x0101008a, 0x0101008b,
    0x0101008c, 0x0101008d, 0x0101008e, 0x0101008f,
    0x01010090, 0x01010091, 0x01010092, 0x01010093,
    0x01010094, 0x010100ae, 0x01010206, 0x01010207,
    0x01010208, 0x0101020d, 0x0101020f, 0x01010210,
    0x01010212, 0x01010213, 0x01010214, 0x01010219,
    0x0101021a, 0x0101021e, 0x0101021f, 0x01010222,
    0x0101022b, 0x01010230, 0x01010267, 0x01010287,
    0x01010288, 0x01010289, 0x0101028b, 0x01010292,
    0x010102a0, 0x010102a1, 0x010102ab, 0x010102ae,
    0x010102af, 0x010102b0, 0x010102b1, 0x010102b2,
    0x010102b3, 0x010102b6, 0x010102b9, 0x010102c5,
    0x010102c6, 0x010102c7, 0x010102c8, 0x010102cc,
    0x010102cd, 0x010102ce, 0x010102cf, 0x010102d0
};

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

    public:
        Int32 mDefaultOpacity;
    };

    class PanelFeatureState {
        public:
            /** Feature ID for this panel. */
            Int32 featureId;

            // Information pulled from the style for this panel.

            Int32 background;

            /** The background when the panel spans the entire available width. */
            Int32 fullBackground;

            Int32 gravity;

            Int32 x;

            Int32 y;

            Int32 windowAnimations;

            /** Dynamic state of the panel. */
            AutoPtr<DecorView> decorView;

            /** The panel that was returned by onCreatePanelView(). */
            AutoPtr<IView> createdPanelView;

            /** The panel that we are actually showing. */
            AutoPtr<IView> shownPanelView;

            /** Use {@link #setMenu} to set this. */
            AutoPtr<IMenu> menu;

            /**
             * Whether the panel has been prepared (see
             * {@link PhoneWindow#preparePanel}).
             */
            Boolean isPrepared;

            /**
             * Whether an item's action has been performed. This happens in obvious
             * scenarios (user clicks on menu item), but can also happen with
             * chording menu+(shortcut key).
             */
            Boolean isHandled;

            Boolean isOpen;

            /**
             * True if the menu is in expanded mode, false if the menu is in icon
             * mode
             */
            Boolean isInExpandedMode;

            Boolean qwertyMode;

            Boolean refreshDecorView;

            Boolean wasLastOpen;

            Boolean wasLastExpanded;

            /**
             * Contains the state of the menu when told to freeze.
             */
            AutoPtr<IBundle> frozenMenuState;

        public:
            PanelFeatureState(
                /* [in] */ Int32 featureId) {
                this->featureId = featureId;

                refreshDecorView = FALSE;
            }

            ECode SetStyle(
                /* [in] */ IContext* context) {
                if (context == NULL) {
                    return E_INVALID_ARGUMENT;
                }

                AutoPtr<ITypedArray> a;
                context->ObtainStyledAttributes(ArrayOf<Int32>(R_styleable_Theme, sizeof(R_styleable_Theme) / sizeof(Int32)),
                            (ITypedArray**) &a);

                a->GetResourceId(R_styleable_Theme[46], 0, &background);
                a->GetResourceId(R_styleable_Theme[47], 0, &fullBackground);
                a->GetResourceId(R_styleable_Theme[93], 0, &windowAnimations);
                a->Recycle();

                return NOERROR;
            }

            ECode SetMenu(
                /* [in] */ IMenu* menu) {
                printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
                if (menu == NULL) {
                    return E_INVALID_ARGUMENT;
                }

                this->menu = menu;

                if (frozenMenuState != NULL) {
                    ((MenuBuilder*)menu)->RestoreHierarchyState(frozenMenuState);
                    frozenMenuState = NULL;
                }

                return NOERROR;
            }

            ECode OnSaveInstanceState(
                /* [out] */ IParcelable** parcelable) {
                SavedState* savedState = new SavedState();
                savedState->featureId = featureId;
                savedState->isOpen = isOpen;
                savedState->isInExpandedMode = isInExpandedMode;

                if (menu != NULL) {
                    CBundle::New((IBundle**) &savedState->menuState);
                    ((MenuBuilder*)menu.Get())->SaveHierarchyState(savedState->menuState);
                }

                //return savedState;
                return NOERROR;
            }

            void OnRestoreInstanceState(
                /* [in] */ IParcelable* state) {
                SavedState* savedState = (SavedState*) state;
                featureId = savedState->featureId;
                wasLastOpen = savedState->isOpen;
                wasLastExpanded = savedState->isInExpandedMode;
                frozenMenuState = savedState->menuState;

                /*
                 * A LocalActivityManager keeps the same instance of this class around.
                 * The first time the menu is being shown after restoring, the
                 * Activity.onCreateOptionsMenu should be called. But, if it is the
                 * same instance then menu != null and we won't call that method.
                 * So, clear this.  Also clear any cached views.
                 */
                menu = NULL;
                createdPanelView = NULL;
                shownPanelView = NULL;
                decorView = NULL;
            }

        //class SavedState implements Parcelable {
        class SavedState {
            public:
                Int32 featureId;
                Boolean isOpen;
                Boolean isInExpandedMode;
                AutoPtr<IBundle> menuState;

            public:
                Int32 DescribeContents() {
                    return 0;
                }

                void WriteToParcel(
                    /* [in] */ IParcel* dest,
                    /* [in] */ Int32 flags) {
                    dest->WriteInt32(featureId);
                    dest->WriteInt32(isOpen ? 1 : 0);
                    dest->WriteInt32(isInExpandedMode ? 1 : 0);

                    if (isOpen) {
                        dest->WriteInterfacePtr(menuState);
                    }
                }

                static ECode ReadFromParcel(
                    /* [in] */ IParcel*  source,
                    SavedState** savedState) {
                    Int32 temp;
                    source->ReadInt32(&temp);
                    (*savedState)->featureId = temp;
                    source->ReadInt32(&temp);
                    (*savedState)->isOpen = temp == 1;

                    source->ReadInt32(&temp);
                    (*savedState)->isInExpandedMode = temp == 1;

                    if ((*savedState)->isOpen) {
                        source->ReadInterfacePtr((Handle32*) &((*savedState)->menuState));
                    }

                    return NOERROR;
                }
//TODO
/*
                static Parcelable.Creator<SavedState> CREATOR
                        = new Parcelable.Creator<SavedState>() {
                    public SavedState createFromParcel(Parcel in) {
                        return readFromParcel(in);
                    }

                    public SavedState[] newArray(int size) {
                        return new SavedState[size];
                    }
                };*/
        };

    };

    /**
     * Simple implementation of MenuBuilder.Callback that:
     * <li> Opens a submenu when selected.
     * <li> Calls back to the callback's onMenuItemSelected when an item is
     * selected.
     */
    class ContextMenuCallback:
        public ElRefBase,
        public IMenuBuilderCallback {
        private:
            AutoPtr<CPhoneWindow> mHost;
            Int32 mFeatureId;
            CMenuDialogHelper* mSubMenuHelper;

        public:
            ContextMenuCallback(
                /* [in] */ CPhoneWindow* host,
                /* [in] */ Int32 featureId) {
                mFeatureId = featureId;
                mHost = host;
            }

            UInt32 AddRef() {
                return ElRefBase::AddRef();
            }

            UInt32 Release() {
                return ElRefBase::Release();
            }

            ECode GetInterfaceID(
                /* [in] */ IInterface* iinterface,
                /* [in] */ InterfaceID* id) {
                return NOERROR;
            }

            PInterface Probe(
                /* [in]  */ REIID riid) {
                if (riid == EIID_IMenuBuilderCallback) {
                    return (IMenuBuilderCallback*)this;
                }

                return NULL;
            }

            ECode OnSubMenuSelected(
                /* [in] */ ISubMenu* submenu,
                /* [out] */ Boolean* state) {
                *state = FALSE;
                return NOERROR;
            }

            ECode OnCloseMenu(
                /* [in] */ IMenuBuilder* menu,
                /* [in] */ Boolean allMenusAreClosing) {
                if (allMenusAreClosing) {
                    AutoPtr<IWindowCallback> callback;
                    mHost->GetCallback((IWindowCallback**) &callback);

                    if (callback != NULL) callback->OnPanelClosed(mFeatureId, (IMenu*)menu);

                    if (mHost->mContextMenu != NULL && menu == mHost->mContextMenu->Probe(EIID_IMenu)) {
                        mHost->DismissContextMenu();
                    }

                    // Dismiss the submenu, if it is showing
                    if (mSubMenuHelper != NULL) {
                        mSubMenuHelper->Dismiss();
                        mSubMenuHelper = NULL;
                    }
                }

                return NOERROR;
            }

            ECode OnCloseSubMenu(
                /* [in] */ ISubMenu* menu) {
                AutoPtr<IWindowCallback> callback;
                mHost->GetCallback((IWindowCallback**) &callback);

                if (callback != NULL) {
                    AutoPtr<IMenuBuilder> rootMenu;
                    ((IMenuBuilder*)(MenuBuilder*)menu)->GetRootMenu((IMenuBuilder**) &rootMenu);
                    callback->OnPanelClosed(mFeatureId, (IMenu*)rootMenu.Get());
                }

                return NOERROR;
            }

            ECode OnMenuItemSelected(
                /* [in] */ IMenuBuilder* menu,
                /* [in] */ IMenuItem* item,
                /* [out] */ Boolean* toFinish) {
                AutoPtr<IWindowCallback> callback;
                mHost->GetCallback((IWindowCallback**) &callback);

                *toFinish = FALSE;
                if (callback != NULL) {
                    callback->OnMenuItemSelected(mFeatureId, item, toFinish);
                }

                return NOERROR;
            }

            ECode OnMenuModeChange(
                /* [in] */ IMenuBuilder* menu) {
                return NOERROR;
            }

            Boolean OnSubMenuSelected(
                /* [in] */ ISubMenu* subMenu) {
                // Set a simple callback for the submenu
                if (subMenu != NULL) {
                    ((MenuBuilder*)subMenu)->SetCallback(this);
                }

                // The window manager will give us a valid window token
                mSubMenuHelper = new CMenuDialogHelper((IMenuBuilder*)subMenu);
                mSubMenuHelper->Show(NULL);

                return TRUE;
            }
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

    CARAPI ClosePanelEx(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ Boolean doCallback);

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

    CARAPI OnKeyDownPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    CARAPI PreparePanel(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* prepared);

    /**
     * Gets a panel's state based on its feature ID.
     *
     * @param featureId The feature ID of the panel.
     * @param required Whether the panel is required (if it is required and it
     *            isn't in our features, this throws an exception).
     * @return The panel state.
     */
    ECode GetPanelState(
        /* [in] */ Int32 featureId,
        /* [in] */ Boolean required,
        /* [out] */ PanelFeatureState** out);

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
    ECode GetPanelStateEx(
        /* [in] */ Int32 featureId,
        /* [in] */ Boolean required,
        /* [in] */ PanelFeatureState* convertPanelState,
        /* [out] */ PanelFeatureState** out);

    ECode FindMenuPanel(
        /* [in] */ IMenu* menu,
        /* [out] */ PanelFeatureState** out);

    ECode OnMenuModeChange(
        /* [in] */ IMenuBuilder* menu);

    ECode OnMenuItemSelected(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* state);

    ECode OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    ECode OnSubMenuSelected(
        /* [in] */ ISubMenu* subMenu,
        /* [out] */ Boolean* state);

    ECode OnCloseSubMenu(
        /* [in] */ ISubMenu* menu);

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

    CARAPI InitializePanelMenu(
        /* [in] */ PanelFeatureState* st,
        /* [out] */ Boolean* inited);

    CARAPI ReopenMenu(
        /* [in] */ Boolean toggleMenuMode);

    ECode InitializePanelDecor(
        /* [in] */ PanelFeatureState* st,
        /* [out] */ Boolean* inited);

    ECode InitializePanelContent(
        /* [in] */ PanelFeatureState* st,
        /* [out] */ Boolean* inited);

private:
    CARAPI_(AutoPtr<DecorView>) GenerateDecor();

    CARAPI GenerateLayout(
        /* [in] */ DecorView* decor,
        /* [out] */ IViewGroup** viewGroup);

    CARAPI_(void) InstallDecor();

    CARAPI CloseContextMenu();

    CARAPI DismissContextMenu();

    void CallOnPanelClosed(
        /* [in] */ Int32 featureId,
        /* [in] */ PanelFeatureState* panel,
        /* [in] */ IMenu* menu);

    ECode OpenPanel(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ IKeyEvent* event);

private:
    static const char* TAG;

    static const Boolean SWEEP_OPEN_MENU = FALSE;
    List<PanelFeatureState*>* mPanels;
    PanelFeatureState* mPreparedPanel;

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

    Int32 mPanelChordingKey;

    Boolean mPanelMayLongPress;

    AutoPtr<CMenuDialogHelper> mContextMenuHelper;

    AutoPtr<ContextMenuBuilder> mContextMenu;

    AutoPtr<ContextMenuCallback> mContextMenuCallback;

//friend class ContextMenuCallback;
};

#endif // __CPHONEWINDOW_H__
