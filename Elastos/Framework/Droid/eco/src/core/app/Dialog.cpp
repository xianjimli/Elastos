
#include "app/Dialog.h"
#include "impl/CPolicyManager.h"
#include "os/CApartment.h"
#include "os/CBundle.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/ViewConfiguration.h"
#include "content/CContextThemeWrapper.h"

const Int32 Dialog::DISMISS;
const Int32 Dialog::CANCEL;
const Int32 Dialog::SHOW;

const String Dialog::DIALOG_SHOWING_TAG("android:dialogShowing");
const String Dialog::DIALOG_HIERARCHY_TAG("android:dialogHierarchy");

Dialog::DismissAction::DismissAction(
    /* [in] */ Dialog* host)
    : mHost(host)
{}

ECode Dialog::DismissAction::Run()
{
    mHost->DismissDialog();

    return NOERROR;
}

Dialog::Dialog()
    : mCancelable(TRUE)
    , mCanceledOnTouchOutside(FALSE)
    , mCreated(FALSE)
    , mShowing(FALSE)
    , mUiThread(0)
{}

/**
 * Create a Dialog window that uses a custom dialog style.
 *
 * @param context The Context in which the Dialog should run. In particular, it
 *                uses the window manager and theme from this context to
 *                present its UI.
 * @param theme A style resource describing the theme to use for the
 * window. See <a href="{@docRoot}guide/topics/resources/available-resources.html#stylesandthemes">Style
 * and Theme Resources</a> for more information about defining and using
 * styles.  This theme is applied on top of the current theme in
 * <var>context</var>.  If 0, the default dialog theme will be used.
 */
Dialog::Dialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
    : mCancelable(TRUE)
    , mCanceledOnTouchOutside(FALSE)
    , mCreated(FALSE)
    , mShowing(FALSE)
    , mUiThread(0)
{
    Init(context, theme);
}

Dialog::Dialog(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
    : mCancelable(TRUE)
    , mCanceledOnTouchOutside(FALSE)
    , mCreated(FALSE)
    , mShowing(FALSE)
    , mUiThread(0)
{
    Init(context, cancelable, cancelListener);
}

/**
 * Retrieve the Context this Dialog is running in.
 *
 * @return Context The Context that was supplied to the constructor.
 */
AutoPtr<IContext> Dialog::GetContext()
{
    return mContext;
}

/**
 * Sets the Activity that owns this dialog. An example use: This Dialog will
 * use the suggested volume control stream of the Activity.
 *
 * @param activity The Activity that owns this dialog.
 */
ECode Dialog::SetOwnerActivity(
    /* [in] */ IActivity* activity)
{
    assert(activity);
    mOwnerActivity = activity;

    Int32 streamType;
//    mOwnerActivity->GetVolumeControlStream(&streamType);
    return GetWindow()->SetVolumeControlStream(streamType);
}

/**
 * Returns the Activity that owns this Dialog. For example, if
 * {@link Activity#showDialog(Int32)} is used to show this Dialog, that
 * Activity will be the owner (by default). Depending on how this dialog was
 * created, this may return NULL.
 *
 * @return The Activity that owns this Dialog.
 */
AutoPtr<IActivity> Dialog::GetOwnerActivity()
{
    return mOwnerActivity;
}

/**
 * @return Whether the dialog is currently showing.
 */
Boolean Dialog::IsShowing()
{
    return mShowing;
}

/**
 * Start the dialog and display it on screen.  The window is placed in the
 * application layer and opaque.  Note that you should not override this
 * method to do initialization when the dialog is shown, instead implement
 * that in {@link #OnStart}.
 */
ECode Dialog::Show()
{
    if (mShowing) {
        if (mDecor != NULL) {
            mDecor->SetVisibility(View_VISIBLE);
        }
        return NOERROR;
    }

    if (!mCreated) {
        DispatchOnCreate(NULL);
    }

    OnStart();
    mDecor = NULL;
    mWindow->GetDecorView((IView**)&mDecor);
    AutoPtr<IWindowManagerLayoutParams> l;
    mWindow->GetAttributes((IWindowManagerLayoutParams**)&l);
    if ((((CWindowManagerLayoutParams*)l.Get())->mSoftInputMode
        & WindowManagerLayoutParams_SOFT_INPUT_IS_FORWARD_NAVIGATION) == 0) {
        AutoPtr<IWindowManagerLayoutParams> nl;
        CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&nl);
        Int32 changes;
        nl->CopyFrom(l, &changes);
        ((CWindowManagerLayoutParams*)nl.Get())->mSoftInputMode |=
            WindowManagerLayoutParams_SOFT_INPUT_IS_FORWARD_NAVIGATION;
        l = nl;
    }

    mWindowManager->AddViewEx5(mDecor, l);
    mShowing = TRUE;

    SendShowMessage();

    return NOERROR;
}

/**
 * Hide the dialog, but do not dismiss it.
 */
ECode Dialog::Hide()
{
    if (mDecor != NULL) {
        mDecor->SetVisibility(View_GONE);
    }

    return NOERROR;
}

/**
 * Dismiss this dialog, removing it from the screen. This method can be
 * invoked safely from any thread.  Note that you should not override this
 * method to do cleanup when the dialog is dismissed, instead implement
 * that in {@link #OnStop}.
 */
ECode Dialog::Dismiss()
{
    if (pthread_self() != mUiThread) {
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

        mApartment->PostCppCallback(
            (Handle32)mDismissAction.Get(), *(Handle32*)&pHandlerFunc, NULL, 0);
    }
    else {
        mDismissAction->Run();
    }

    return NOERROR;
}

void Dialog::DismissDialog()
{
    if (mDecor == NULL || !mShowing) {
        return;
    }

    mWindowManager->RemoveView(mDecor);
    mDecor = NULL;
    mWindow->CloseAllPanels();
    OnStop();
    mShowing = FALSE;

    SendDismissMessage();
}

void Dialog::SendDismissMessage()
{
    //if (mDismissMessage != NULL) {
    //    // Obtain a new message so this dialog can be re-used
    //    Message.obtain(mDismissMessage).sendToTarget();
    //}

    if (mDismissListener != NULL) {
        ECode (STDCALL IDialogInterfaceOnDismissListener::*pHandlerFunc)(
            IDialogInterface*);
        pHandlerFunc = &IDialogInterfaceOnDismissListener::OnDismiss;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(this->Probe(EIID_IDialogInterface));

        mApartment->PostCppCallback(
            (Handle32)mDismissListener.Get(), *(Handle32*)&pHandlerFunc, params, 0);
    }
}

void Dialog::SendShowMessage()
{
    //if (mShowMessage != NULL) {
    //    // Obtain a new message so this dialog can be re-used
    //    Message.obtain(mShowMessage).sendToTarget();
    //}

    if (mShowListener != NULL) {
        ECode (STDCALL IDialogInterfaceOnShowListener::*pHandlerFunc)(
            IDialogInterface*);
        pHandlerFunc = &IDialogInterfaceOnShowListener::OnShow;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(this->Probe(EIID_IDialogInterface));

        mApartment->PostCppCallback(
            (Handle32)mShowListener.Get(), *(Handle32*)&pHandlerFunc, params, 0);
    }
}

// internal method to make sure mcreated is set properly without requiring
// users to call through to super in onCreate
ECode Dialog::DispatchOnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    if (!mCreated) {
        OnCreate(savedInstanceState);
        mCreated = TRUE;
    }

    return NOERROR;
}

/**
 * Similar to {@link Activity#onCreate}, you should initialized your dialog
 * in this method, including calling {@link #setContentView}.
 * @param savedInstanceState If this dialog is being reinitalized after a
 *     the hosting activity was previously shut down, holds the result from
 *     the most recent call to {@link #onSaveInstanceState}, or NULL if this
 *     is the first time.
 */
void Dialog::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
}

/**
 * Called when the dialog is starting.
 */
void Dialog::OnStart()
{
}

/**
 * Called to tell you that you're stopping.
 */
void Dialog::OnStop()
{
}

/**
 * Saves the state of the dialog into a bundle.
 *
 * The default implementation saves the state of its view hierarchy, so you'll
 * likely want to call through to super if you override this to save additional
 * state.
 * @return A bundle with the state of the dialog.
 */
AutoPtr<IBundle> Dialog::OnSaveInstanceState()
{
    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    bundle->PutBoolean(DIALOG_SHOWING_TAG, mShowing);
    if (mCreated) {
        AutoPtr<IBundle> wBundle;
        mWindow->SaveHierarchyState((IBundle**)&bundle);
        bundle->PutBundle(DIALOG_HIERARCHY_TAG, wBundle);
    }
    return bundle;
}

/**
 * Restore the state of the dialog from a previously saved bundle.
 *
 * The default implementation restores the state of the dialog's view
 * hierarchy that was saved in the default implementation of {@link #onSaveInstanceState()},
 * so be sure to call through to super when overriding unless you want to
 * do all restoring of state yourself.
 * @param savedInstanceState The state of the dialog previously saved by
 *     {@link #onSaveInstanceState()}.
 */
ECode Dialog::OnRestoreInstanceState(
    /* [in] */ IBundle* savedInstanceState)
{
    AutoPtr<IBundle> dialogHierarchyState;
    savedInstanceState->GetBundle(DIALOG_HIERARCHY_TAG, (IBundle**)&dialogHierarchyState);
    if (dialogHierarchyState == NULL) {
        // dialog has never been shown, or onCreated, nothing to restore.
        return NOERROR;
    }

    DispatchOnCreate(savedInstanceState);
    mWindow->RestoreHierarchyState(dialogHierarchyState);
    Boolean show;
    savedInstanceState->GetBoolean(DIALOG_SHOWING_TAG, &show);
    if (show) {
        Show();
    }

    return NOERROR;
}

/**
 * Retrieve the current Window for the activity.  This can be used to
 * directly access parts of the Window API that are not available
 * through Activity/Screen.
 *
 * @return Window The current window, or NULL if the activity is not
 *         visual.
 */
AutoPtr<IWindow> Dialog::GetWindow()
{
    return mWindow;
}

/**
 * Call {@link android.view.Window#getCurrentFocus} on the
 * Window if this Activity to return the currently focused view.
 *
 * @return View The current View with focus or NULL.
 *
 * @see #GetWindow
 * @see android.view.Window#getCurrentFocus
 */
AutoPtr<IView> Dialog::GetCurrentFocus()
{
    AutoPtr<IView> focus;
    if (mWindow != NULL) {
        mWindow->GetCurrentFocus((IView**)&focus);
    }

    return focus;
}

/**
 * Finds a view that was identified by the id attribute from the XML that
 * was processed in {@link #OnStart}.
 *
 * @param id the identifier of the view to find
 * @return The view if found or NULL otherwise.
 */
AutoPtr<IView> Dialog::FindViewById(
    /* [in] */ Int32 id)
{
    AutoPtr<IView> view;
    mWindow->FindViewById(id, (IView**)&view);

    return view;
}

/**
 * Set the screen content from a layout resource.  The resource will be
 * inflated, adding all top-level views to the screen.
 *
 * @param layoutResID Resource ID to be inflated.
 */
ECode Dialog::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    return mWindow->SetContentView(layoutResID);
}

/**
 * Set the screen content to an explicit view.  This view is placed
 * directly into the screen's view hierarchy.  It can itself be a complex
 * view hierarhcy.
 *
 * @param view The desired content to display.
 */
ECode Dialog::SetContentView(
    /* [in] */ IView* view)
{
    return mWindow->SetContentViewEx(view);
}

/**
 * Set the screen content to an explicit view.  This view is placed
 * directly into the screen's view hierarchy.  It can itself be a complex
 * view hierarhcy.
 *
 * @param view The desired content to display.
 * @param params Layout parameters for the view.
 */
ECode Dialog::SetContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return mWindow->SetContentViewEx2(view, params);
}

/**
 * Add an additional content view to the screen.  Added after any existing
 * ones in the screen -- existing views are NOT removed.
 *
 * @param view The desired content to display.
 * @param params Layout parameters for the view.
 */
ECode Dialog::AddContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return mWindow->AddContentView(view, params);
}

/**
 * Set the title text for this dialog's window.
 *
 * @param title The new text to display in the title.
 */
ECode Dialog::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mWindow->SetTitle(title);
    AutoPtr<IWindowManagerLayoutParams> l;
    mWindow->GetAttributes((IWindowManagerLayoutParams**)&l);
    l->SetTitle(title);

    return NOERROR;
}

/**
 * Set the title text for this dialog's window. The text is retrieved
 * from the resources with the supplied identifier.
 *
 * @param titleId the title's text resource identifier
 */
ECode Dialog::SetTitle(
    /* [in] */ Int32 titleId)
{
    AutoPtr<ICharSequence> title;
    mContext->GetText(titleId, (ICharSequence**)&title);

    return SetTitle(title);
}

/**
 * A key was pressed down.
 *
 * <p>If the focused view didn't want this event, this method is called.
 *
 * <p>The default implementation consumed the KEYCODE_BACK to later
 * handle it in {@link #onKeyUp}.
 *
 * @see #onKeyUp
 * @see android.view.KeyEvent
 */
Boolean Dialog::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == KeyEvent_KEYCODE_BACK) {
        event->StartTracking();
        return TRUE;
    }

    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyLongPress(Int32, KeyEvent)
 * KeyEvent.Callback.onKeyLongPress()}: always returns FALSE (doesn't handle
 * the event).
 */
Boolean Dialog::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * A key was released.
 *
 * <p>The default implementation handles KEYCODE_BACK to close the
 * dialog.
 *
 * @see #onKeyDown
 * @see KeyEvent
 */
Boolean Dialog::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean isTracking;
    Boolean isCanceled;
    if (keyCode == KeyEvent_KEYCODE_BACK
        && (event->IsTracking(&isTracking), isTracking)
        && !(event->IsCanceled(&isCanceled), isCanceled)) {
        OnBackPressed();
        return TRUE;
    }
    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyMultiple(Int32, Int32, KeyEvent)
 * KeyEvent.Callback.onKeyMultiple()}: always returns FALSE (doesn't handle
 * the event).
 */
Boolean Dialog::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Called when the dialog has detected the user's press of the back
 * key.  The default implementation simply cancels the dialog (only if
 * it is cancelable), but you can override this to do whatever you want.
 */
ECode Dialog::OnBackPressed()
{
    if (mCancelable) {
        Cancel();
    }

    return NOERROR;
}

/**
 * Called when a touch screen event was not handled by any of the views
 * under it. This is most useful to process touch events that happen outside
 * of your window bounds, where there is no view to receive it.
 *
 * @param event The touch screen event being processed.
 * @return Return TRUE if you have consumed the event, FALSE if you haven't.
 *         The default implementation will cancel the dialog when a touch
 *         happens outside of the window bounds.
 */
Boolean Dialog::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    if (mCancelable && mCanceledOnTouchOutside
        && (event->GetAction(&action), action) == MotionEvent_ACTION_DOWN
        && IsOutOfBounds(event)) {
        Cancel();
        return TRUE;
    }

    return FALSE;
}

Boolean Dialog::IsOutOfBounds(
    /* [in] */ IMotionEvent* event)
{
    Float fx, fy;
    event->GetX(&fx);
    event->GetY(&fy);
    Int32 x = (Int32)fx;
    Int32 y = (Int32)fy;
    Int32 slop = ViewConfiguration::Get(mContext)->GetScaledWindowTouchSlop();
    AutoPtr<IView> decorView;
    GetWindow()->GetDecorView((IView**)&decorView);
    Int32 width, height;
    decorView->GetWidth(&width);
    decorView->GetHeight(&height);
    return (x < -slop) || (y < -slop)
            || (x > (width + slop))
            || (y > (height + slop));
}

/**
 * Called when the trackball was moved and not handled by any of the
 * views inside of the activity.  So, for example, if the trackball moves
 * while focus is on a button, you will receive a call here because
 * buttons do not normally do anything with trackball events.  The call
 * here happens <em>before</em> trackball movements are converted to
 * DPAD key events, which then get sent back to the view hierarchy, and
 * will be processed at the point for things like focus navigation.
 *
 * @param event The trackball event being processed.
 *
 * @return Return TRUE if you have consumed the event, FALSE if you haven't.
 * The default implementation always returns FALSE.
 */
Boolean Dialog::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

ECode Dialog::OnWindowAttributesChanged(
    /* [in] */ IWindowManagerLayoutParams* params)
{
    if (mDecor != NULL) {
        mWindowManager->UpdateViewLayout(mDecor, params);
    }

    return NOERROR;
}

ECode Dialog::OnContentChanged()
{
    return NOERROR;
}

ECode Dialog::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    return NOERROR;
}

ECode Dialog::OnAttachedToWindow()
{
    return NOERROR;
}

ECode Dialog::OnDetachedFromWindow()
{
    return NOERROR;
}

/**
 * Called to process key events.  You can override this to intercept all
 * key events before they are dispatched to the window.  Be sure to call
 * this implementation for key events that should be handled normally.
 *
 * @param event The key event.
 *
 * @return Boolean Return TRUE if this event was consumed.
 */
Boolean Dialog::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    Boolean consumed;
    if ((mOnKeyListener != NULL) &&
        (mOnKeyListener->OnKey(
            (IDialogInterface*)this->Probe(EIID_IDialogInterface),
            keyCode, event, &consumed), consumed)) {
        return TRUE;
    }

    Boolean res;
    mWindow->SuperDispatchKeyEvent(event, &res);
    if (res) {
        return TRUE;
    }

    AutoPtr<IDispatcherState> state;
    if (mDecor != NULL) {
        mDecor->GetKeyDispatcherState((IDispatcherState**)&state);
    }

    event->DispatchEx(
        (IKeyEventCallback*)this->Probe(EIID_IKeyEventCallback),
        state, this->Probe(EIID_IInterface), &res);

    return res;
}

/**
 * Called to process touch screen events.  You can override this to
 * intercept all touch screen events before they are dispatched to the
 * window.  Be sure to call this implementation for touch screen events
 * that should be handled normally.
 *
 * @param ev The touch screen event.
 *
 * @return Boolean Return TRUE if this event was consumed.
 */
Boolean Dialog::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Boolean res;
    mWindow->SuperDispatchTouchEvent(ev, &res);
    if (res) {
        return TRUE;
    }

    return OnTouchEvent(ev);
}

/**
 * Called to process trackball events.  You can override this to
 * intercept all trackball events before they are dispatched to the
 * window.  Be sure to call this implementation for trackball events
 * that should be handled normally.
 *
 * @param ev The trackball event.
 *
 * @return Boolean Return TRUE if this event was consumed.
 */
Boolean Dialog::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* ev)
{
    Boolean res;
    mWindow->SuperDispatchTrackballEvent(ev, &res);
    if (res) {
        return TRUE;
    }

    return OnTrackballEvent(ev);
}

Boolean Dialog::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
//    event.setClassName(getClass().getName());
//    event.setPackageName(mContext.getPackageName());
//
//    LayoutParams params = GetWindow().getAttributes();
//    Boolean isFullScreen = (params.width == LayoutParams.MATCH_PARENT) &&
//        (params.height == LayoutParams.MATCH_PARENT);
//    event.setFullScreen(isFullScreen);

    return FALSE;
}

/**
 * @see Activity#onCreatePanelView(Int32)
 */
AutoPtr<IView> Dialog::OnCreatePanelView(
    /* [in] */ Int32 featureId)
{
    return NULL;
}

/**
 * @see Activity#onCreatePanelMenu(Int32, Menu)
 */
Boolean Dialog::OnCreatePanelMenu(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    if (featureId == Window_FEATURE_OPTIONS_PANEL) {
        return OnCreateOptionsMenu(menu);
    }

    return FALSE;
}

/**
 * @see Activity#onPreparePanel(Int32, View, Menu)
 */
Boolean Dialog::OnPreparePanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IView* view,
    /* [in] */ IMenu* menu)
{
    if (featureId == Window_FEATURE_OPTIONS_PANEL && menu != NULL) {
        Boolean goforit = OnPrepareOptionsMenu(menu);
        Boolean hasVisibleItems;
        return goforit && (menu->HasVisibleItems(&hasVisibleItems),
            hasVisibleItems);
    }
    return TRUE;
}

/**
 * @see Activity#onMenuOpened(Int32, Menu)
 */
Boolean Dialog::OnMenuOpened(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    return TRUE;
}

/**
 * @see Activity#onMenuItemSelected(Int32, MenuItem)
 */
Boolean Dialog::OnMenuItemSelected(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenuItem* item)
{
    return FALSE;
}

/**
 * @see Activity#onPanelClosed(Int32, Menu)
 */
ECode Dialog::OnPanelClosed(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    return NOERROR;
}

/**
 * It is usually safe to proxy this call to the owner activity's
 * {@link Activity#onCreateOptionsMenu(Menu)} if the client desires the same
 * menu for this Dialog.
 *
 * @see Activity#onCreateOptionsMenu(Menu)
 * @see #getOwnerActivity()
 */
Boolean Dialog::OnCreateOptionsMenu(
    /* [in] */ IMenu* menu)
{
    return TRUE;
}

/**
 * It is usually safe to proxy this call to the owner activity's
 * {@link Activity#onPrepareOptionsMenu(Menu)} if the client desires the
 * same menu for this Dialog.
 *
 * @see Activity#onPrepareOptionsMenu(Menu)
 * @see #getOwnerActivity()
 */
Boolean Dialog::OnPrepareOptionsMenu(
    /* [in] */ IMenu* menu)
{
    return TRUE;
}

/**
 * @see Activity#onOptionsItemSelected(MenuItem)
 */
Boolean Dialog::OnOptionsItemSelected(
    /* [in] */ IMenuItem* item)
{
    return FALSE;
}

/**
 * @see Activity#onOptionsMenuClosed(Menu)
 */
ECode Dialog::OnOptionsMenuClosed(
    /* [in] */ IMenu* menu)
{
    return NOERROR;
}

/**
 * @see Activity#openOptionsMenu()
 */
ECode Dialog::OpenOptionsMenu()
{
    return mWindow->OpenPanel(Window_FEATURE_OPTIONS_PANEL, NULL);
}

/**
 * @see Activity#closeOptionsMenu()
 */
ECode Dialog::CloseOptionsMenu()
{
    return mWindow->ClosePanel(Window_FEATURE_OPTIONS_PANEL);
}

/**
 * @see Activity#onCreateContextMenu(ContextMenu, View, ContextMenuInfo)
 */
ECode Dialog::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return NOERROR;
}

/**
 * @see Activity#registerForContextMenu(View)
 */
ECode Dialog::RegisterForContextMenu(
    /* [in] */ IView* view)
{
    return view->SetOnCreateContextMenuListener(
        (IViewOnCreateContextMenuListener*)this->Probe(
        EIID_IViewOnCreateContextMenuListener));
}

/**
 * @see Activity#unregisterForContextMenu(View)
 */
ECode Dialog::UnregisterForContextMenu(
    /* [in] */ IView* view)
{
    return view->SetOnCreateContextMenuListener(NULL);
}

/**
 * @see Activity#openContextMenu(View)
 */
ECode Dialog::OpenContextMenu(
    /* [in] */ IView* view)
{
    Boolean res;
    return view->ShowContextMenu(&res);
}

/**
 * @see Activity#onContextItemSelected(MenuItem)
 */
Boolean Dialog::OnContextItemSelected(
    /* [in] */ IMenuItem* item)
{
    return FALSE;
}

/**
 * @see Activity#onContextMenuClosed(Menu)
 */
ECode Dialog::OnContextMenuClosed(
    /* [in] */ IMenu* menu)
{
    return NOERROR;
}

/**
 * This hook is called when the user signals the desire to start a search.
 */
Boolean Dialog::OnSearchRequested()
{
//    final SearchManager searchManager = (SearchManager) mContext
//            .getSystemService(Context_SEARCH_SERVICE);
//
//    // associate search with owner activity
//    final ComponentName appName = getAssociatedActivity();
//    if (appName != NULL) {
//        searchManager.startSearch(NULL, FALSE, appName, NULL, FALSE);
//        dismiss();
//        return TRUE;
//    } else {
//        return FALSE;
//    }
    return FALSE;
}

/**
 * @return The activity associated with this dialog, or NULL if there is no assocaited activity.
 */
AutoPtr<IComponentName> Dialog::GetAssociatedActivity()
{
    AutoPtr<IActivity> activity = mOwnerActivity;
    AutoPtr<IContext> context = GetContext();
    while (activity == NULL && context != NULL) {
        if (IActivity::Probe(context)) {
            activity = IActivity::Probe(context);  // found it!
        }
        else {
            if (IContextWrapper::Probe(context)) {// unwrap one level
                IContextWrapper::Probe(context)->GetBaseContext(
                    (IContext**)&context);
            }
            else {// done
                context = NULL;
            }
        }
    }
    AutoPtr<IComponentName> name;
    if (activity != NULL) {
        activity->GetComponentName((IComponentName**)&name);
    }
    return name;
}

/**
 * Request that key events come to this dialog. Use this if your
 * dialog has no views with focus, but the dialog still wants
 * a chance to process key events.
 *
 * @param get TRUE if the dialog should receive key events, FALSE otherwise
 * @see android.view.Window#takeKeyEvents
 */
ECode Dialog::TakeKeyEvents(
    /* [in] */ Boolean get)
{
    return mWindow->TakeKeyEvents(get);
}

/**
 * Enable extended window features.  This is a convenience for calling
 * {@link android.view.Window#requestFeature GetWindow().requestFeature()}.
 *
 * @param featureId The desired feature as defined in
 *                  {@link android.view.Window}.
 * @return Returns TRUE if the requested feature is supported and now
 *         enabled.
 *
 * @see android.view.Window#requestFeature
 */
Boolean Dialog::RequestWindowFeature(
    /* [in] */ Int32 featureId)
{
    Boolean res;
    GetWindow()->RequestFeature(featureId, &res);

    return res;
}

/**
 * Convenience for calling
 * {@link android.view.Window#setFeatureDrawableResource}.
 */
ECode Dialog::SetFeatureDrawableResource(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 resId)
{
    return GetWindow()->SetFeatureDrawableResource(featureId, resId);
}

/**
 * Convenience for calling
 * {@link android.view.Window#setFeatureDrawableUri}.
 */
ECode Dialog::SetFeatureDrawableUri(
    /* [in] */ Int32 featureId,
    /* [in] */ IUri* uri)
{
    return GetWindow()->SetFeatureDrawableUri(featureId, uri);
}

/**
 * Convenience for calling
 * {@link android.view.Window#setFeatureDrawable(Int32, Drawable)}.
 */
ECode Dialog::SetFeatureDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable)
{
    return GetWindow()->SetFeatureDrawable(featureId, drawable);
}

/**
 * Convenience for calling
 * {@link android.view.Window#setFeatureDrawableAlpha}.
 */
ECode Dialog::SetFeatureDrawableAlpha(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 alpha)
{
    return GetWindow()->SetFeatureDrawableAlpha(featureId, alpha);
}

AutoPtr<ILayoutInflater> Dialog::GetLayoutInflater()
{
    AutoPtr<ILayoutInflater> layout;
    GetWindow()->GetLayoutInflater((ILayoutInflater**)&layout);

    return layout;
}

/**
 * Sets whether this dialog is cancelable with the
 * {@link KeyEvent#KEYCODE_BACK BACK} key.
 */
ECode Dialog::SetCancelable(
    /* [in] */ Boolean flag)
{
    mCancelable = flag;

    return NOERROR;
}

/**
 * Sets whether this dialog is canceled when touched outside the window's
 * bounds. If setting to TRUE, the dialog is set to be cancelable if not
 * already set.
 *
 * @param cancel Whether the dialog should be canceled when touched outside
 *            the window.
 */
ECode Dialog::SetCanceledOnTouchOutside(
    /* [in] */ Boolean cancel)
{
    if (cancel && !mCancelable) {
        mCancelable = TRUE;
    }

    mCanceledOnTouchOutside = cancel;

    return NOERROR;
}

/**
 * Cancel the dialog.  This is essentially the same as calling {@link #dismiss()}, but it will
 * also call your {@link DialogInterface.OnCancelListener} (if registered).
 */
ECode Dialog::Cancel()
{
    //if (mCancelMessage != NULL) {

    //    // Obtain a new message so this dialog can be re-used
    //    Message.obtain(mCancelMessage).sendToTarget();
    //}
    if (mCancelListener != NULL) {
        ECode (STDCALL IDialogInterfaceOnCancelListener::*pHandlerFunc)(
            IDialogInterface*);
        pHandlerFunc = &IDialogInterfaceOnCancelListener::OnCancel;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(this->Probe(EIID_IDialogInterface));

        mApartment->PostCppCallback(
            (Handle32)mCancelListener.Get(), *(Handle32*)&pHandlerFunc, params, 0);
    }
    Dismiss();

    return NOERROR;
}

/**
 * Set a listener to be invoked when the dialog is canceled.
 * <p>
 * This will only be invoked when the dialog is canceled, if the creator
 * needs to know when it is dismissed in general, use
 * {@link #setOnDismissListener}.
 *
 * @param listener The {@link DialogInterface.OnCancelListener} to use.
 */
ECode Dialog::SetOnCancelListener(
    /* [in] */ IDialogInterfaceOnCancelListener* listener)
{
    mCancelListener = listener;
    return NOERROR;
    //if (listener != NULL) {
    //    mCancelMessage = mListenersHandler.obtainMessage(CANCEL, listener);
    //}
    //else {
    //    mCancelMessage = NULL;
    //}
}

/**
 * Set a message to be sent when the dialog is canceled.
 * @param msg The msg to send when the dialog is canceled.
 * @see #SetOnCancelListener(android.content.DialogInterface.OnCancelListener)
 */
//ECode Dialog::SetCancelMessage(final Message msg) {
//    mCancelMessage = msg;
//}

/**
 * Set a listener to be invoked when the dialog is dismissed.
 * @param listener The {@link DialogInterface.OnDismissListener} to use.
 */
ECode Dialog::SetOnDismissListener(
    /* [in] */ IDialogInterfaceOnDismissListener* listener)
{
    mDismissListener = listener;
    return NOERROR;
    //if (listener != NULL) {
    //    mDismissMessage = mListenersHandler.obtainMessage(DISMISS, listener);
    //} else {
    //    mDismissMessage = NULL;
    //}
}

/**
 * Sets a listener to be invoked when the dialog is shown.
 * @param listener The {@link DialogInterface.OnShowListener} to use.
 */
ECode Dialog::SetOnShowListener(
    /* [in] */ IDialogInterfaceOnShowListener* listener)
{
    mShowListener = listener;
    return NOERROR;
    //if (listener != NULL) {
    //    mShowMessage = mListenersHandler.obtainMessage(SHOW, listener);
    //} else {
    //    mShowMessage = NULL;
    //}
}

/**
 * Set a message to be sent when the dialog is dismissed.
 * @param msg The msg to send when the dialog is dismissed.
 */
//ECode Dialog::SetDismissMessage(final Message msg) {
//    mDismissMessage = msg;
//}

/**
 * By default, this will use the owner Activity's suggested stream type.
 *
 * @see Activity#setVolumeControlStream(Int32)
 * @see #setOwnerActivity(Activity)
 */
ECode Dialog::SetVolumeControlStream(
    /* [in] */ Int32 streamType)
{
    return GetWindow()->SetVolumeControlStream(streamType);
}

/**
 * @see Activity#getVolumeControlStream()
 */
Int32 Dialog::GetVolumeControlStream()
{
    Int32 volume;
    GetWindow()->GetVolumeControlStream(&volume);

    return volume;
}

/**
 * Sets the callback that will be called if a key is dispatched to the dialog.
 */
ECode Dialog::SetOnKeyListener(
    /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener)
{
    mOnKeyListener = onKeyListener;

    return NOERROR;
}

ECode Dialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    mDismissAction = (IRunnable*)new DismissAction(this);

    if (theme == 0) {
        theme = 0x0103000b/*com.android.internal.R.style.Theme_Dialog*/;
    }
    CContextThemeWrapper::New(
        context, theme, (IContextThemeWrapper**)&mContext);

    context->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&mWindowManager);

    AutoPtr<IPolicyManager> pm;
    CPolicyManager::AcquireSingleton((IPolicyManager**)&pm);
    pm->MakeNewWindow(mContext, (IWindow**)&mWindow);

    mWindow->SetCallback((IWindowCallback*)this->Probe(EIID_IWindowCallback));
    mWindow->SetWindowManager(mWindowManager, NULL, String(NULL));
    mWindow->SetGravity(Gravity_CENTER);
    mUiThread = pthread_self();

    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));

    return NOERROR;
}

ECode Dialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    FAIL_RETURN(Init(context));
    mCancelable = cancelable;
    SetOnCancelListener(cancelListener);

    return NOERROR;
}

