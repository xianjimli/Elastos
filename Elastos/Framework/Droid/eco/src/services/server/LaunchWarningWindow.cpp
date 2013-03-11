
#include "LaunchWarningWindow.h"

IDIALOG_METHODS_IMPL(LaunchWarningWindow, Dialog, Dialog);

IWINDOWCALLBACK_METHODS_IMPL(LaunchWarningWindow, Dialog, Dialog);

IKeyEventCallback_METHODS_IMPL(LaunchWarningWindow, Dialog, Dialog);

LaunchWarningWindow::LaunchWarningWindow(
    /* [in] */ IContext* context,
    /* [in] */ CActivityRecord* cur,
    /* [in] */ CActivityRecord* next)
        : Dialog(context, 0x010300aa/*R.style.Theme_Toast*/)
{
    RequestWindowFeature(Window_FEATURE_LEFT_ICON);
    GetWindow()->SetType(WindowManagerLayoutParams_TYPE_SYSTEM_ALERT);
    GetWindow()->AddFlags(WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE
            | WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE);

    SetContentView(0x0109003a/*R.layout.launch_warning*/);

    AutoPtr<ICharSequence> text;
    res->GetText(0x01040305/*R.string.launch_warning_title*/,
            (ICharSequence**)&text);
    SetTitle(text);

    GetWindow()->setFeatureDrawableResource(Window_FEATURE_LEFT_ICON,
            0x01080027/*R.drawable.ic_dialog_alert*/);

    AutoPtr<IImageView> icon = (IImageView*)FindViewById(0x010201eb/*R.id.replace_app_icon*/);
    //icon->SetImageDrawable(next.info.applicationInfo.loadIcon(context.getPackageManager()));

    AutoPtr<ITextView> text = (ITextView*)FindViewById(0x010201ec/*R.id.replace_message*/);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    String s;
    res->GetString(0x01040306/*R.string.launch_warning_replace*/, &s);
    text->SetText(s);
    // text->SetText(context.getResources().getString(,
    //         next.info.applicationInfo.loadLabel(context.getPackageManager()).toString()));

    icon = (IImageView*)FindViewById(0x010201ed/*R.id.original_app_icon*/);
    //icon.setImageDrawable(cur.info.applicationInfo.loadIcon(context.getPackageManager()));
    text = (ITextView*)FindViewById(0x010201ee/*R.id.original_message*/);
    res->GetString(0x01040307/*R.string.launch_warning_original*/, &s);
    text->SetText(s);
    // text.setText(context.getResources().getString(R.string.launch_warning_original,
    //         cur.info.applicationInfo.loadLabel(context.getPackageManager()).toString()));
}

PInterface LaunchWarningWindow::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IDialog) {
        return (PInterface)(IDialog*)this;
    }
    else if (riid == EIID_IDialogInterface) {
        return (PInterface)(IDialogInterface*)this;
    }
    else if (riid == EIID_IWindowCallback) {
        return (PInterface)(IWindowCallback*)this;
    }
    if (riid == EIID_IKeyEventCallback) {
        return (PInterface)(IKeyEventCallback*)this;
    }
    else if (riid == EIID_IViewOnCreateContextMenuListener) {
       return (PInterface)(IViewOnCreateContextMenuListener*)this;
    }

    return NULL;
}

UInt32 LaunchWarningWindow::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LaunchWarningWindow::Release()
{
    return ElRefBase::Release();
}

ECode LaunchWarningWindow::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode LaunchWarningWindow::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return Dialog::OnCreateContextMenu(menu, v, menuInfo);
}
