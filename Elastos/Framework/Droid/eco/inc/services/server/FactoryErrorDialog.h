
#ifndef __FACTORYERRORDIALOG_H__
#define __FACTORYERRORDIALOG_H__

#include "BaseErrorDialog.h"
#include <elastos/ElRefBase.h>

class FactoryErrorDialog : public BaseErrorDialog
{
private:
    class PositiveButtonOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        PositiveButtonOnClickListener(
            /* [in] */ FactoryErrorDialog* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        FactoryErrorDialog* mHost;
    };

public:
    friend class PositiveButtonOnClickListener;

    FactoryErrorDialog(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* msg);

    CARAPI_(void) OnStop();

private:
    CARAPI_(void) HandleOnClickMessage();

};
#endif //__FACTORYERRORDIALOG_H__
