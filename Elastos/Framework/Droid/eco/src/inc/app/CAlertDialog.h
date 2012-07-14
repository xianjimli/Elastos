
#ifndef __CALERTDIALOG_H__
#define __CALERTDIALOG_H__

#include "_CAlertDialog.h"
#include "app/AlertDialog.h"
#include "app/DialogMacro.h"
#include "view/ViewMacro.h"
#include "view/WindowCallbackMacro.h"

CarClass(CAlertDialog), public AlertDialog
{
public:
    IDIALOG_METHODS_DECL();

    IWINDOWCALLBACK_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    CARAPI GetButton(
        /* [in] */ Int32 whichButton,
        /* [out] */ IButton** button);

    CARAPI GetListView(
        /* [out] */ IListView** listView);

    CARAPI SetCustomTitle(
        /* [in] */ IView* customTitleView);

    CARAPI SetMessage(
        /* [in] */ ICharSequence* message);

    CARAPI SetView(
        /* [in] */ IView* view);

    CARAPI SetViewEx(
        /* [in] */ IView* view,
        /* [in] */ Int32 viewSpacingLeft,
        /* [in] */ Int32 viewSpacingTop,
        /* [in] */ Int32 viewSpacingRight,
        /* [in] */ Int32 viewSpacingBottom);

    CARAPI SetButton(
        /* [in] */ Int32 whichButton,
        /* [in] */ ICharSequence* text);

    CARAPI SetButtonEx(
        /* [in] */ Int32 whichButton,
        /* [in] */ ICharSequence* text,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetIcon(
        /* [in] */ Int32 resId);

    CARAPI SetIconEx(
        /* [in] */ IDrawable* icon);

    CARAPI SetInverseBackgroundForced(
        /* [in] */ Boolean forceInverseBackground);

private:
    // TODO: Add your private member variables here.
};

#endif // __CALERTDIALOG_H__
