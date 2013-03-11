
#ifndef __LAUNCHWARNINTWINDOW_H__
#define __LAUNCHWARNINTWINDOW_H__

#include "app/Dialog.h"
#include "app/DialogMacro.h"
#include "view/ViewMacro.h"
#include "view/WindowCallbackMacro.h"
#include <elastos/ElRefBase.h>

class LaunchWarningWindow
    : public Dialog
    , public ElRefBase
    , public IDialog
    , public IWindowCallback
    , public IKeyEventCallback
    , public IViewOnCreateContextMenuListener
{
public:
    IDIALOG_METHODS_DECL();

    IWINDOWCALLBACK_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    LaunchWarningWindow(
        /* [in] */ IContext* context,
        /* [in] */ CActivityRecord* cur,
        /* [in] */ CActivityRecord* next);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

private:
    // TODO: Add your private member variables here.
};

#endif // __LAUNCHWARNINTWINDOW_H__
