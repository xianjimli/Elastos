
#ifndef __MENUDIALOGHELPER_H_
#define __MENUDIALOGHELPER_H_


#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * Helper for menus that appear as Dialogs (context and submenus).
 *
 * @hide
 */
class MenuDialogHelper
    : public ElRefBase
    , public IDialogInterfaceOnClickListener
    , public IDialogInterfaceOnKeyListener
{
public:
    MenuDialogHelper(
        /* [in] */ IMenuBuilder* menu);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Shows menu as a dialog.
     *
     * @param windowToken Optional token to assign to the window.
     */
    CARAPI_(void) Show(
        /* [in] */ IBinder* windowToken);

    CARAPI OnKey(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag);

    /**
     * Dismisses the menu's dialog.
     *
     * @see Dialog#dismiss()
     */
    CARAPI_(void) Dismiss();

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ int which);

private:
    AutoPtr<IMenuBuilder> mMenu;
    AutoPtr<IListAdapter> mAdapter;
    AutoPtr<IAlertDialog> mDialog;
};

#endif //__MENUDIALOGHELPER_H_
