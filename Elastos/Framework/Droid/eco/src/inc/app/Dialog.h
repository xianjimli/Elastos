
#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "os/Runnable.h"
#include <elastos/AutoPtr.h>

class Dialog
{
private:
    static const Int32 DISMISS = 0x43;
    static const Int32 CANCEL = 0x44;
    static const Int32 SHOW = 0x45;

    static const String DIALOG_SHOWING_TAG;
    static const String DIALOG_HIERARCHY_TAG;

private:
    class DismissAction : public Runnable
    {
    public:
        DismissAction(
            /* [in] */ Dialog* host);

        CARAPI Run();

    private:
        Dialog* mHost;
    };

public:
    Dialog();

    Dialog(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme = 0);

    Dialog(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI_(AutoPtr<IContext>) GetContext();

    CARAPI SetOwnerActivity(
        /* [in] */ IActivity* activity);

    CARAPI_(AutoPtr<IActivity>) GetOwnerActivity();

    virtual CARAPI_(Boolean) IsShowing();

    virtual CARAPI Show();

    virtual CARAPI Hide();

    virtual CARAPI Dismiss();

    virtual CARAPI DispatchOnCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI_(AutoPtr<IBundle>) OnSaveInstanceState();

    virtual CARAPI OnRestoreInstanceState(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI_(AutoPtr<IWindow>) GetWindow();

    virtual CARAPI_(AutoPtr<IView>) GetCurrentFocus();

    virtual CARAPI_(AutoPtr<IView>) FindViewById(
        /* [in] */ Int32 id);

    virtual CARAPI SetContentView(
        /* [in] */ Int32 layoutResID);

    virtual CARAPI SetContentView(
        /* [in] */ IView* view);

    virtual CARAPI SetContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI AddContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI SetTitle(
        /* [in] */ Int32 titleId);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyLongPress(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI OnBackPressed();


    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI OnWindowAttributesChanged(
        /* [in] */ IWindowManagerLayoutParams* params);

    virtual CARAPI OnContentChanged();

    virtual CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    virtual CARAPI OnAttachedToWindow();

    virtual CARAPI OnDetachedFromWindow();

    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) DispatchTouchEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Boolean) DispatchTrackballEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI_(AutoPtr<IView>) OnCreatePanelView(
        /* [in] */ Int32 featureId);

    virtual CARAPI_(Boolean) OnCreatePanelMenu(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnPreparePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IView* view,
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnMenuOpened(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnMenuItemSelected(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenuItem* item);

    virtual CARAPI OnPanelClosed(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnCreateOptionsMenu(
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnPrepareOptionsMenu(
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnOptionsItemSelected(
        /* [in] */ IMenuItem* item);

    virtual CARAPI OnOptionsMenuClosed(
        /* [in] */ IMenu* menu);

    virtual CARAPI OpenOptionsMenu();

    virtual CARAPI CloseOptionsMenu();

    virtual CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    virtual CARAPI RegisterForContextMenu(
        /* [in] */ IView* view);

    virtual CARAPI UnregisterForContextMenu(
        /* [in] */ IView* view);

    virtual CARAPI OpenContextMenu(
        /* [in] */ IView* view);

    virtual CARAPI_(Boolean) OnContextItemSelected(
        /* [in] */ IMenuItem* item);

    virtual CARAPI OnContextMenuClosed(
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnSearchRequested();

    virtual CARAPI TakeKeyEvents(
        /* [in] */ Boolean get);

    CARAPI_(Boolean) RequestWindowFeature(
        /* [in] */ Int32 featureId);

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

    virtual CARAPI_(AutoPtr<ILayoutInflater>) GetLayoutInflater();

    virtual CARAPI SetCancelable(
        /* [in] */ Boolean flag);

    virtual CARAPI SetCanceledOnTouchOutside(
        /* [in] */ Boolean cancel);

    virtual CARAPI Cancel();

    virtual CARAPI SetOnCancelListener(
        /* [in] */ IDialogInterfaceOnCancelListener* listener);

    virtual CARAPI SetOnDismissListener(
        /* [in] */ IDialogInterfaceOnDismissListener* listener);

    virtual CARAPI SetOnShowListener(
        /* [in] */ IDialogInterfaceOnShowListener* listener);

    CARAPI SetVolumeControlStream(
        /* [in] */ Int32 streamType);

    CARAPI_(Int32) GetVolumeControlStream();

    virtual CARAPI SetOnKeyListener(
        /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener);

protected:
    virtual CARAPI_(void) OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI_(void) OnStart();

    virtual CARAPI_(void) OnStop();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme = 0);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

private:
    CARAPI_(void) DismissDialog();

    CARAPI_(void) SendDismissMessage();

    CARAPI_(void) SendShowMessage();

    CARAPI_(Boolean) IsOutOfBounds(
        /* [in] */ IMotionEvent* event);

    CARAPI_(AutoPtr<IComponentName>) GetAssociatedActivity();

protected:
    AutoPtr<IContext> mContext;
    AutoPtr<IWindowManager> mWindowManager;
    AutoPtr<IWindow> mWindow;
    AutoPtr<IView> mDecor;
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Boolean mCancelable;

private:
    AutoPtr<IActivity> mOwnerActivity;

    //Message mCancelMessage;
    //Message mDismissMessage;
    //Message mShowMessage;

    AutoPtr<IDialogInterfaceOnCancelListener> mCancelListener;
    AutoPtr<IDialogInterfaceOnDismissListener> mDismissListener;
    AutoPtr<IDialogInterfaceOnShowListener> mShowListener;

    /**
     * Whether to cancel the dialog when a touch is received outside of the
     * window's bounds.
     */
    Boolean mCanceledOnTouchOutside;

    AutoPtr<IDialogInterfaceOnKeyListener> mOnKeyListener;

    Boolean mCreated;
    Boolean mShowing;

    Int32 mUiThread;
    AutoPtr<IApartment> mApartment;

    AutoPtr<IRunnable> mDismissAction;
};
#endif //__DIALOG_H__
