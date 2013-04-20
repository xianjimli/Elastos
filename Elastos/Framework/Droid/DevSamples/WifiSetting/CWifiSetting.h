
#ifndef __CWIFISETTING_H__
#define __CWIFISETTING_H__

#include "Activity.h"
#include "_CWifiSetting.h"

class CWifiSetting : public Activity
{
public:
    class MyListener
    : public IViewOnTouchListener
    , public IViewOnKeyListener
    , public IOnItemClickListener
    , public IViewOnClickListener
    , public IDialogInterfaceOnClickListener
    {
    public:
        MyListener(
            /* [in] */ CWifiSetting* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        CARAPI OnKey(
            /* [in] */ IView * v,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

       	CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI OnClick(
            /* [in] */ IView* v);

        CARAPI OnClick(
        		/* [in] */ IDialogInterface* dialog,
        		/* [in] */ Int32 which);

    private:
        AutoPtr<CWifiSetting> mHost;
        Int32 mRef;
    };

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

	CARAPI_(AutoPtr<IDialog>) OnCreateDialog(
    /* [in] */ Int32 id);

private:
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

    // CARAPI CreateNavigationBar();

    CARAPI_(void) SetDialogTitle(
        /* [in] */ ICharSequence* title);

private:
    AutoPtr<IView> mConnectButton;
    AutoPtr<IView> mCancelButton;
    AutoPtr<IView> mWifiStatusCheck;

    // AutoPtr<IView> mBack;
    // AutoPtr<IView> mHome;
    // AutoPtr<IView> mMenu;
    AutoPtr<ILayoutInflater> mInflater;
    AutoPtr<IAlertDialog> mWifiDialog;

friend class MyListener;
};

#endif // __CWIFISETTING_H__
