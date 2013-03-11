
#ifndef __BASEERRORDIALOG_H__
#define __BASEERRORDIALOG_H__

#include "app/AlertDialog.h"
#include "app/AlertDialogMacro.h"
#include "app/DialogMacro.h"
#include "view/ViewMacro.h"
#include "view/WindowCallbackMacro.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

class BaseErrorDialog
    : public AlertDialog
    , public ElRefBase
    , public IAlertDialog
    , public IWindowCallback
    , public IKeyEventCallback
    , public IViewOnCreateContextMenuListener
{
public:
    IALERTDIALOG_METHODS_DECL();

    IDIALOG_METHODS_DECL();

    IWINDOWCALLBACK_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    BaseErrorDialog(
        /* [in] */ IContext* context);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(void) OnStart();

    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

private:
    CARAPI_(void) HandleOnStartMessage();

    CARAPI_(void) SetEnabled(
        /* [in] */ Boolean enabled);

private:
    Boolean mConsuming;

public:
    AutoPtr<IApartment> mApartment;
};
#endif //__BASEERRORDIALOG_H__
