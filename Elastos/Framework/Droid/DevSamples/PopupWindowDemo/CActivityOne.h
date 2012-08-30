
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "Activity.h"
#include "_CActivityOne.h"

class CActivityOne : public Activity
{
public:
    class MyListener : public IViewOnClickListener
    {
    public:
        MyListener(
            /* [in] */ CActivityOne* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        AutoPtr<CActivityOne> mHost;
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

private:
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

    //创建一个包含自定义view的PopupWindow
    AutoPtr<IPopupWindow> CreatePopupWindow(
        /* [in] */ IContext* cx);

private:
};

#endif // __CACTIVITYONE_H__
