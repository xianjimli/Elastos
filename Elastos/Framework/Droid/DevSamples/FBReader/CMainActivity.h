
#ifndef __CMAINACTIVITY_H__
#define __CMAINACTIVITY_H__

#include "CActivity.h"
#include "_CMainActivity.h"
#include "CBook.h"

class CMainActivity : public CActivity
{
public:
    class MyListener : public IViewOnTouchListener, public IViewOnKeyListener
    {
    public:
        MyListener(
            /* [in] */ CMainActivity* host);

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

    private:
        AutoPtr<CMainActivity> mHost;
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

private:
    AutoPtr<IBook> mBook;
};

#endif // __CMAINACTIVITY_H__
