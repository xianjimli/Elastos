
#include "CMenuDialogHelper.h"
#include "CMenuBuilder.h"
#include "CAlertDialogBuilder.h"
#include "view/CWindowManagerLayoutParams.h"

PInterface CMenuDialogHelper::Probe(
    /* [in]  */ REIID riid) {
    if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (IDialogInterfaceOnClickListener*)this;
    }
    else if (riid == EIID_IDialogInterfaceOnKeyListener) {
        return (IDialogInterfaceOnKeyListener*)this;
    }

    return NULL;
}

UInt32 CMenuDialogHelper::AddRef() {
    return ElRefBase::AddRef();
}

UInt32 CMenuDialogHelper::Release() {
    return ElRefBase::Release();
}

ECode CMenuDialogHelper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID) {
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IDialogInterfaceOnClickListener*)this) {
        *pIID = EIID_IDialogInterfaceOnClickListener;
    }
    else if (pObject == (IInterface*)(IDialogInterfaceOnKeyListener*)this) {
        *pIID = EIID_IDialogInterfaceOnKeyListener;
    }

    return E_INVALID_ARGUMENT;
}

ECode CMenuDialogHelper::Show(
        /* [in] */ IBinder* windowToken) {
    // Many references to mMenu, create local reference
    AutoPtr<IMenuBuilder> menu = mMenu;

    // Get an adapter for the menu item views
    if (menu != NULL) {
        menu->GetMenuAdapter(CMenuBuilder::TYPE_DIALOG, (IMenuAdapter**) &mAdapter);
    }

    AutoPtr<IContext> context;
    menu->GetContext((IContext**) &context);

    if (context != NULL) {
        // Get the builder for the dialog
        AutoPtr<IAlertDialogBuilder> builder;
        CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&builder);

        if (builder != NULL) {
            builder->SetAdapter(mAdapter, this);
        }

        // Set the title
        AutoPtr<IView> headerView;
        menu->GetHeaderView((IView**) &headerView);

        if (headerView != NULL) {
            // Menu's client has given a custom header view, use it
            builder->SetCustomTitle(headerView);
        } else {
            // Otherwise use the (text) title and icon
            AutoPtr<IDrawable> icon;
            menu->GetHeaderIcon((IDrawable**) &icon);
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
        mDialog->GetWindow((IWindow**) &window);

        if (window != NULL) {
            AutoPtr<IWindowManagerLayoutParams> lp;
            window->GetAttributes((IWindowManagerLayoutParams**) &lp);
            AutoPtr<CWindowManagerLayoutParams> wp = (CWindowManagerLayoutParams*)(lp.Get());

            wp->mType = WindowManagerLayoutParams_TYPE_APPLICATION_ATTACHED_DIALOG;
            if (windowToken != NULL) {
                wp->mToken = windowToken;
            }
            wp->mFlags |= WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM;

            mDialog->Show();
        }
    }

    return NOERROR;
}

ECode CMenuDialogHelper::OnKey(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag) {
    *flag = TRUE;

    if (keyCode == KeyEvent_KEYCODE_MENU || keyCode == KeyEvent_KEYCODE_BACK) {
        Int32 action, repeatCount;
        event->GetAction(&action);
        event->GetRepeatCount(&repeatCount);

        AutoPtr<IWindow> win;
        mDialog->GetWindow((IWindow**) &win);

        Boolean isCanceled;
        event->IsCanceled(&isCanceled);

        if (action == KeyEvent_ACTION_DOWN
                && repeatCount == 0) {
            if (win != NULL) {
                AutoPtr<IView> decor;
                win->GetDecorView((IView**) &decor);
                if (decor != NULL) {
                    AutoPtr<IDispatcherState> dispatcher;
                    decor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);
                    if (dispatcher != NULL) {
                        dispatcher->StartTracking(event, Probe(EIID_IDialogInterfaceOnKeyListener));
                    }
                }
            }
        } else if (action == KeyEvent_ACTION_UP && !isCanceled) {
            if (win != NULL) {
                AutoPtr<IView> decor;
                win->GetDecorView((IView**) &decor);

                if (decor != NULL) {
                    AutoPtr<IDispatcherState> dispatcher;
                    decor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);

                    Boolean isTracking;
                    dispatcher->IsTracking(event, &isTracking);
                    if (dispatcher != NULL && isTracking) {
                        mMenu->CloseEx(TRUE);
                        dialog->Dismiss();
                    }
                }
            }
        }
    }

    // Menu shortcut matching
    return ((MenuBuilder*)mMenu.Get())->PerformShortcut(keyCode, event, 0, flag);
}

ECode CMenuDialogHelper::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which) {
    AutoPtr<IMenuItem> item;
    mAdapter->GetItem(which, (IInterface**) &item);

    Boolean state;
    return mMenu->PerformItemAction(item , 0, &state);
}

ECode CMenuDialogHelper::OnKey(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event) {
    return NOERROR;
}
