
#include "view/menu/MenuDialogHelper.h"
#include "view/menu/MenuBuilder.h"
#include "app/CAlertDialogBuilder.h"
#include "view/CWindowManagerLayoutParams.h"
#include <stdio.h>


MenuDialogHelper::MenuDialogHelper(
    /* [in] */ IMenuBuilder* menu)
    : mMenu(menu)
{}

PInterface MenuDialogHelper::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (IDialogInterfaceOnClickListener*)this;
    }
    else if (riid == EIID_IDialogInterfaceOnKeyListener) {
        return (IDialogInterfaceOnKeyListener*)this;
    }

    return NULL;
}

UInt32 MenuDialogHelper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MenuDialogHelper::Release()
{
    return ElRefBase::Release();
}

ECode MenuDialogHelper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IDialogInterfaceOnClickListener*)this) {
        *pIID = EIID_IDialogInterfaceOnClickListener;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IDialogInterfaceOnKeyListener*)this) {
        *pIID = EIID_IDialogInterfaceOnKeyListener;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

void MenuDialogHelper::Show(
    /* [in] */ IBinder* windowToken)
{
    // Many references to mMenu, create local reference
    AutoPtr<IMenuBuilder> menu = mMenu;

    // Get an adapter for the menu item views
    menu->GetMenuAdapter(MenuBuilder::TYPE_DIALOG, (IMenuAdapter**)&mAdapter);

    //TODO
    Int32 tmpCount = 0;
    mAdapter->GetCount(&tmpCount);

    // Get the builder for the dialog
    AutoPtr<IContext> context;
    menu->GetContext((IContext**)&context);
    AutoPtr<IAlertDialogBuilder> builder;
    CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&builder);
    builder->SetAdapter(mAdapter, this);

    // Set the title
    AutoPtr<IView> headerView;
    menu->GetHeaderView((IView**)&headerView);
    if (headerView != NULL) {
        // Menu's client has given a custom header view, use it
        builder->SetCustomTitle(headerView);
    }
    else {
        // Otherwise use the (text) title and icon
        AutoPtr<IDrawable> icon;
        menu->GetHeaderIcon((IDrawable**)&icon);
        builder->SetIconEx(icon);

        AutoPtr<ICharSequence> csq;
        menu->GetHeaderTitle((ICharSequence**)&csq);
        builder->SetTitleEx(csq);
    }

    // Set the key listener
    builder->SetOnKeyListener(this);

    // Since this is for a menu, disable the recycling of views
    // This is done by the menu framework anyway
    builder->SetRecycleOnMeasureEnabled(FALSE);

    // Show the menu
    builder->Create((IAlertDialog**)&mDialog);

    AutoPtr<IWindow> window;
    mDialog->GetWindow((IWindow**)&window);
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* wp = (CWindowManagerLayoutParams*)(lp.Get());
    wp->mType = WindowManagerLayoutParams_TYPE_APPLICATION_ATTACHED_DIALOG;
    if (windowToken != NULL) {
        wp->mToken = windowToken;
    }
    wp->mFlags |= WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM;

    mDialog->Show();
}

ECode MenuDialogHelper::OnKey(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    if (keyCode == KeyEvent_KEYCODE_MENU || keyCode == KeyEvent_KEYCODE_BACK) {
        Int32 action, repeatCount;
        event->GetAction(&action);
        event->GetRepeatCount(&repeatCount);
        Boolean isCanceled;
        if (action == KeyEvent_ACTION_DOWN
                && repeatCount == 0) {
            AutoPtr<IWindow> win;
            mDialog->GetWindow((IWindow**)&win);
            if (win != NULL) {
                AutoPtr<IView> decor;
                win->GetDecorView((IView**)&decor);
                if (decor != NULL) {
                    AutoPtr<IDispatcherState> dispatcher;
                    decor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);
                    if (dispatcher != NULL) {
                        dispatcher->StartTracking(event, (IDialogInterfaceOnClickListener*)this);
                        *flag = TRUE;
                        return NOERROR;
                    }
                }
            }
        }
        else if (action == KeyEvent_ACTION_UP && (event->IsCanceled(&isCanceled), !isCanceled)) {
            AutoPtr<IWindow> win;
            mDialog->GetWindow((IWindow**)&win);
            if (win != NULL) {
                AutoPtr<IView> decor;
                win->GetDecorView((IView**) &decor);
                if (decor != NULL) {
                    AutoPtr<IDispatcherState> dispatcher;
                    decor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);
                    Boolean isTracking;
                    if (dispatcher != NULL && (dispatcher->IsTracking(event, &isTracking), isTracking)) {
                        mMenu->CloseEx(TRUE);
                        dialog->Dismiss();
                        *flag = TRUE;
                        return NOERROR;
                    }
                }
            }
        }
    }

    // Menu shortcut matching
    return mMenu->PerformShortcut(keyCode, event, 0, flag);
}

void MenuDialogHelper::Dismiss()
{
    if (mDialog != NULL) {
        mDialog->Dismiss();
    }
}

ECode MenuDialogHelper::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    AutoPtr<IMenuItem> item;
    mAdapter->GetItem(which, (IInterface**)&item);

    Boolean state;
    return mMenu->PerformItemAction(item , 0, &state);
}
