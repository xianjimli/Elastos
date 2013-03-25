
#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#define __USE_MALLOC
#include "Elastos.Framework.Core.h"
#endif
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>

using namespace Elastos;

class Activity
    : public ElRefBase
    , public IObject
    , public IActivity
    , public IWindowCallback
    , public IKeyEventCallback
    , public IViewOnCreateContextMenuListener
{
public:

    IKeyEventCallback_METHODS_DECL();

    Activity();

    virtual ~Activity();

    virtual CARAPI Initialize();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI GetApplication(
        /* [out] */ IApplication** application);

    /** Is this activity embedded inside of another activity? */
    CARAPI IsChild(
        /* [out] */ Boolean* isChild);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI SetIntent(
        /* [in] */ IIntent* newIntent);

    CARAPI GetID(
        /* [out] */ String* id);

    CARAPI Create(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI PostCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI Start();

    CARAPI Restart();

    CARAPI Resume();

    CARAPI Pause();

    CARAPI Stop();

    CARAPI Destroy();

    CARAPI DispatchActivityResult(
        /* [in] */ const String& who,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

    CARAPI Attach(
        /* [in] */ IContext* ctx,
        /* [in] */ IApplicationApartment* apartment,
        /* [in] */ IInstrumentation* instr,
        /* [in] */ IBinder* token,
        /* [in] */ IApplication* application,
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info,
        /* [in] */ ICharSequence* title,
        /* [in] */ IActivity* parent,
        /* [in] */ const String& id,
        /* [in] */ IInterface* lastNonConfigurationInstance,
        /* [in] */ IConfiguration* config);

    CARAPI AttachEx(
        /* [in] */ IContext* context,
        /* [in] */ IApplicationApartment* apartment,
        /* [in] */ IInstrumentation* instr,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 ident,
        /* [in] */ IApplication* application,
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info,
        /* [in] */ ICharSequence* title,
        /* [in] */ IActivity* parent,
        /* [in] */ const String& id,
        /* [in] */ IInterface* lastNonConfigurationInstance,
        /* [in] */ IObjectStringMap* lastNonConfigurationChildInstances,
        /* [in] */ IConfiguration* config);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI OnLowMemory();

    CARAPI DispatchNewIntent(
        /* [in] */ IIntent *intent);

    CARAPI SetCalled(
        /* [in] */ Boolean called);

    CARAPI IsCalled(
        /* [out] */ Boolean* called);

    CARAPI SetFinishing(
        /* [in] */ Boolean finished);

    CARAPI IsFinishing(
        /* [out] */ Boolean* finished);

    CARAPI MoveTaskToBack(
        /* [in] */ Boolean nonRoot,
        /* [out] */ Boolean* succeed);

    CARAPI SetRequestedOrientation(
        /* [int] */ Int32 requestedOrientation);

    CARAPI GetRequestedOrientation(
        /* [out] */ Int32* orientation);

    CARAPI MakeVisible();

    CARAPI GetActivityToken(
        /* [out] */ IBinder** binder);

    CARAPI PerformStart();

    CARAPI PerformRestart();

    CARAPI PerformResume();

    CARAPI PerformPause();

    CARAPI PerformUserLeaving();

    CARAPI PerformStop();

    CARAPI PerformSaveInstanceState(
        /* [in] */ IBundle* outState);

    CARAPI StartActivityFromChild(
        /* [in] */ IActivity* child,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode);

    CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI StartService(
        /* [in] */ IIntent* service,
        /* [out] */ IComponentName** name);

    CARAPI StopService(
        /* [in] */ IIntent* service,
        /* [out] */ Boolean* succeeded);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI UnbindService(
        /* [in] */ IServiceConnection* conn);

    CARAPI GetSystemService(
        /* [in] */ CString name,
        /* [out] */ IInterface** object);

    CARAPI CreateCapsuleContext(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ IContext** ctx);

    CARAPI CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* value);

    CARAPI EnforceCallingOrSelfPermission(
        /* [in] */ CString permission,
        /* [in] */ CString message);

    CARAPI CheckPermissionEx(
        /* [in] */ const String& permName,
        /* [in] */ const String& pkgName,
        /* [out] */ Int32 * result);

    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Int32 * result);

    CARAPI CheckUriPermission(
        /* [in] */ IUri * uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32 * result);

    CARAPI CheckUriPermissionEx(
        /* [in] */ IUri * uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32 * result);

    CARAPI RevokeUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI CheckCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* perm);

    CARAPI GrantUriPermission(
        /* [in] */ const String& toCapsule,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI GetAssets(
        /* [out] */ IAssetManager** assetManager);

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI GetCapsuleManager(
        /* [out] */ ILocalCapsuleManager** capsuleManager);

    CARAPI GetContentResolver(
        /* [out] */ IContentResolver** resolver);

    CARAPI GetApplicationContext(
        /* [out] */ IContext** ctx);

    CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [out] */ String* str);

    CARAPI SetTheme(
        /* [in] */ Int32 resid);

    CARAPI GetTheme(
        /* [out] */ ITheme** theme);

    CARAPI ObtainStyledAttributes(
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx(
        /* [in] */ Int32 resid,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx2(
        /* [in] */ IAttributeSet* set,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx3(
        /* [in] */ IAttributeSet* set,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [out] */ ITypedArray** styles);

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetCapsuleName(
        /* [out] */ String* capsuleName);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI GetCapsuleResourcePath(
        /* [out] */ String* path);

    CARAPI GetFilesDir(
        /* [out] */ IFile** filesDir);

    CARAPI GetExternalFilesDir(
        /* [in] */ const String& type,
        /* [out] */ IFile** filesDir);

    CARAPI GetCacheDir(
        /* [out] */ IFile** cacheDir);

    CARAPI GetDir(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFile** dir);

    /**
     * @return the base context as set by the constructor or setBaseContext
     */
    CARAPI GetBaseContext(
        /* [out] */ IContext** ctx);

    // internal use.
    CARAPI IsStartedActivity(
        /* [out] */ Boolean* isStartedActivity);

    CARAPI GetWindowEx(
        /* [out] */ IWindow** window);

    CARAPI SetDecorView(
        /* [in] */ IView* decor);

    CARAPI GetDecorView(
        /* [out] */ IView** decor);

    CARAPI IsVisibleFromClient(
        /* [out] */ Boolean* visible);

    CARAPI SetWindowAdded(
        /* [in] */ Boolean added);

    CARAPI GetWindowManagerEx(
        /* [out] */ ILocalWindowManager** mgr);

    /**
     * Returns complete component name of this activity.
     *
     * @return Returns the complete component name for this activity
     */
    virtual CARAPI GetComponentName(
        /* [out] */ IComponentName** name);

    /**
     * Change the title associated with this activity.  If this is a
     * top-level activity, the title for its window will change.  If it
     * is an embedded activity, the parent can do whatever it wants
     * with it.
     */
    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    /**
     * Change the title associated with this activity.  If this is a
     * top-level activity, the title for its window will change.  If it
     * is an embedded activity, the parent can do whatever it wants
     * with it.
     */
    CARAPI SetTitleEx(
        /* [in] */ Int32 titleId);

    CARAPI SetTitleColor(
        /* [in] */ Int32 textColor);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI GetTitleColor(
        /* [out] */ Int32* textColor);

    CARAPI RunOnUiThread(
        /* [in] */ IRunnable* action);

    CARAPI OnCreateOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* allowToShow);

    CARAPI OnPrepareOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* res);

    CARAPI OnContextItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* res);

    CARAPI OnOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* res);

    CARAPI OnOptionsMenuClosed(
        /* [in] */ IMenu* menu);

    CARAPI OnContextMenuClosed(
        /* [in] */ IMenu* menu);

    /**
     * @see Activity#openOptionsMenu()
     */
    CARAPI OpenOptionsMenu();

    /**
     * @see Activity#closeOptionsMenu()
     */
    CARAPI CloseOptionsMenu();

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    /**
     * @see Activity#registerForContextMenu(View)
     */
    CARAPI RegisterForContextMenu(
        /* [in] */ IView* view);

    /**
     * @see Activity#unregisterForContextMenu(View)
     */
    CARAPI UnregisterForContextMenu(
        /* [in] */ IView* view);

    /**
     * @see Activity#openContextMenu(View)
     */
    CARAPI OpenContextMenu(
        /* [in] */ IView* view);


    //For IWindowCallback interface.
    CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* isConsumed);

    CARAPI DispatchTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* isConsumed);

    CARAPI DispatchTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* isConsumed);

    CARAPI DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event,
        /* [out] */ Boolean* isCompleted);

    CARAPI OnCreatePanelView(
        /* [in] */ Int32 featureId,
        /* [out] */ IView** view);

    CARAPI OnCreatePanelMenu(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* allowToShow);

    CARAPI OnPreparePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IView* view,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* allowToShow);

    CARAPI OnMenuOpened(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* allowToOpen);

    CARAPI OnMenuItemSelected(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* toFinish);

    CARAPI OnWindowAttributesChanged(
        /* [in] */ IWindowManagerLayoutParams* attrs);

    CARAPI OnContentChanged();

    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI OnPanelClosed(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    CARAPI OnSearchRequested(
        /* [out] */ Boolean* isLaunched);

    CARAPI GetMenuInflater (
        /* [out] */ IMenuInflater** menuInflater);

    //For IKeyEventCallback interface.
    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyLongPress(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnBackPressed();

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

    CARAPI OnTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

public:
    /**
     * Finds a view that was identified by the id attribute from the XML that
     * was processed in {@link #onCreate}.
     *
     * @return The view if found or null otherwise.
     */
    CARAPI_(AutoPtr<IView>) FindViewById(
        /* [in] */ Int32 id);

    /**
     * Set the activity content from a layout resource.  The resource will be
     * inflated, adding all top-level views to the activity.
     *
     * @param layoutResID Resource ID to be inflated.
     */
    CARAPI SetContentView(
        /* [in] */ Int32 layoutResID);

    /**
     * Set the activity content to an explicit view.  This view is placed
     * directly into the activity's view hierarchy.  It can itself be a complex
     * view hierarhcy.
     *
     * @param view The desired content to display.
     */
    CARAPI SetContentView(
        /* [in] */ IView* view);

    /**
     * Set the activity content to an explicit view.  This view is placed
     * directly into the activity's view hierarchy.  It can itself be a complex
     * view hierarhcy.
     *
     * @param view The desired content to display.
     * @param params Layout parameters for the view.
     */
    CARAPI SetContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    /** Retrieve the window manager for showing custom windows. */
    CARAPI_(AutoPtr<ILocalWindowManager>) GetWindowManager();

    /**
     * Retrieve the current {@link android.view.Window} for the activity.
     * This can be used to directly access parts of the Window API that
     * are not available through Activity/Screen.
     *
     * @return Window The current window, or null if the activity is not
     *         visual.
     */
    CARAPI_(AutoPtr<IWindow>) GetWindow();

    CARAPI ShowDialog(
        /* [in] */ Int32 id);

    CARAPI ShowDialogEx(
        /* [in] */ Int32 id,
        /* [in] */ IBundle* args,
        /* [out] */ Boolean* res);

    CARAPI DismissDialog(
        /* [in] */ Int32 id);

    CARAPI RemoveDialog(
        /* [in] */ Int32 id);

protected:
    CARAPI Finish();

    virtual CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI_(AutoPtr<IDialog>) CreateDialog(
        /* [in] */ Int32 dialogId,
        /* [in] */ IBundle* state,
        /* [in] */ IBundle* args);

    virtual CARAPI OnPostCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnStart();

    virtual CARAPI OnRestart();

    virtual CARAPI OnResume();

    virtual CARAPI OnPause();

    virtual CARAPI OnPostResume();

    virtual CARAPI OnStop();

    virtual CARAPI OnDestroy();

    virtual CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

    virtual CARAPI OnNewIntent(
        /* [in] */ IIntent *intent);

    virtual CARAPI_(void) OnUserInteraction();

    virtual CARAPI_(void) OnUserLeaveHint();

    CARAPI StartActivityForResult(
        /* [in] */ IIntent *pIntent,
        /* [in] */ Int32 requestCode);

    CARAPI SetResult(
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *pResultData);

    /*protected*/
    virtual CARAPI OnTitleChanged(
        /* [in] */ ICharSequence* title,
        /* [in] */ Int32 color);

    virtual CARAPI AttachBaseContext(
        /* [in] */ IContext* base);

    virtual CARAPI OnApplyThemeResource(
        /* [in] */ ITheme* theme,
        /* [in] */ Int32 resid,
        /* [in] */ Boolean first);

    virtual CARAPI_(AutoPtr<IDialog>) OnCreateDialog(
        /* [in] */ Int32 id);

    virtual CARAPI_(AutoPtr<IDialog>) OnCreateDialog(
        /* [in] */ Int32 id,
        /* [in] */ IBundle* args);

    virtual CARAPI_(void) OnPrepareDialog(
        /* [in] */ Int32 id,
        /* [in] */ IDialog* dialog);

    virtual CARAPI_(void) OnPrepareDialog(
        /* [in] */ Int32 id,
        /* [in] */ IDialog* dialog,
        /* [in] */ IBundle* args);

private:
    CARAPI InitializeTheme();

private:
    class ManagedDialog
    {
    public:
        AutoPtr<IDialog> mDialog;
        AutoPtr<IBundle> mArgs;
    };

protected:
    AutoPtr<IContext> mBase;
    AutoPtr<IIntent> mResultData;
    Int32 mResultCode;
    AutoPtr<IBinder> mToken;

private:
    HashMap<Int32, ManagedDialog*>* mManagedDialogs;

    // set by the thread after the constructor and before onCreate(Bundle savedInstanceState) is called.
    AutoPtr<IInstrumentation> mInstrumentation;
    String mEmbeddedID;
    AutoPtr<IApplication> mApplication;
    AutoPtr<IIntent> mIntent;
    AutoPtr<IComponentName> mComponent;
    AutoPtr<IApplicationApartment> mApartment;
    AutoPtr<IActivity> mParent;
    Boolean mCalled;
    Boolean mResumed;
    Boolean mStopped;
    Boolean mFinished;
    Boolean mStartedActivity;
    AutoPtr<IWindow> mWindow;
    AutoPtr<ILocalWindowManager> mWindowManager;
    AutoPtr<IView> mDecor;
    // frome ContextThemeWrapper.java
    AutoPtr<ILayoutInflater> mInflater;
    Boolean mWindowAdded;
    Boolean mVisibleFromClient;

    AutoPtr<ICharSequence> mTitle;
    Int32 mTitleColor;

    Boolean mTitleReady;

    Int32 mDefaultKeyMode;

    Int32 mThemeResource;
    AutoPtr<ITheme> mTheme;
    AutoPtr<IThread> mUiThread;
};

#endif //__ACTIVITY_H__
