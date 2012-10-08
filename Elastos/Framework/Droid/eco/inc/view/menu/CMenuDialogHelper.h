
#ifndef __CMENUDIALOGHELPER_H_
#define __CMENUDIALOGHELPER_H_

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

using namespace Elastos;

/**
 * Helper for menus that appear as Dialogs (context and submenus).
 *
 * @hide
 */
//public class MenuDialogHelper implements DialogInterface.OnKeyListener, DialogInterface.OnClickListener {
class CMenuDialogHelper
    : public ElRefBase
    , public IDialogInterfaceOnClickListener
    , public IDialogInterfaceOnKeyListener
{

private:
    AutoPtr<IMenuBuilder> mMenu;
    AutoPtr<IListAdapter> mAdapter;
    AutoPtr<IAlertDialog> mDialog;

public:
    CMenuDialogHelper(
        /* [in] */ IMenuBuilder* menu) {
        mMenu = menu;
    }

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
    CARAPI Show(
        /* [in] */ IBinder* windowToken);

    CARAPI OnKey(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag);

    CARAPI OnKey(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    /**
     * Dismisses the menu's dialog.
     *
     * @see Dialog#dismiss()
     */
    ECode Dismiss() {
        if (mDialog != NULL) {
            mDialog->Dismiss();
        }

        return NOERROR;
    }

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ int which);

};

#endif //__CMENUDIALOGHELPER_H_
