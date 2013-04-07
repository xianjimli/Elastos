
#ifndef __CSETONCLICKPENDINGINTENT_H__
#define __CSETONCLICKPENDINGINTENT_H__

#include "_CSetOnClickPendingIntent.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

#define SetOnClickPendingIntent_TAG (0)

CarClass(CSetOnClickPendingIntent)
{
public:
    class COnClickLintener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        COnClickLintener(
            /* [in] */ IRemoteViewsAction* remoteViewsAction);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        AutoPtr<IRemoteViewsAction> mRemoteViewsAction;
    };

    friend class COnClickLintener;

public:
    CSetOnClickPendingIntent();

    CARAPI Apply(
        /* [in] */ IView* root);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ Int32 id,
        /* [in] */ IPendingIntent* pendingIntent);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

private:
    Int32 mViewId;
    AutoPtr<IPendingIntent> mPendingIntent;
};

#endif // __CSETONCLICKPENDINGINTENT_H__
