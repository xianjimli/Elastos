
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "Activity.h"
#include "_CActivityOne.h"

class CActivityOne : public Activity
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
            /* [in] */ CActivityOne* host);

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
        AutoPtr<CActivityOne> mHost;
        Int32 mRef;
    };

public:
    virtual CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

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

    CARAPI CreateNavigationBar();

private:
    AutoPtr<IDrawable> mDrawable1;
    AutoPtr<IDrawable> mDrawable2;

    AutoPtr<IView> mButton1;
    AutoPtr<IView> mButton2;
    AutoPtr<IView> mButton3;

    AutoPtr<IAnimation> mAlphaAnimation;
    AutoPtr<IAnimation> mScaleAnimation;
    AutoPtr<IAnimation> mRotateAnimation;
    AutoPtr<IAnimation> mTranslateAnimation;

    AutoPtr<IView> mAnimationButton;
    AutoPtr<IView> mDialogButton;
    AutoPtr<IView> mContent;


    AutoPtr<IView> mBack;
    AutoPtr<IView> mHome;
    AutoPtr<IView> mMenu;
};

#endif // __CACTIVITYONE_H__
