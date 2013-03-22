
#ifndef __TOAST_H__
#define __TOAST_H__

#include "os/Runnable.h"
#include <elastos/AutoPtr.h>

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/ElRefBase.h>
/**
 * A toast is a view containing a quick little message for the user.  The toast class
 * helps you create and show those.
 * {@more}
 *
 * <p>
 * When the view is shown to the user, appears as a floating view over the
 * application.  It will never receive focus.  The user will probably be in the
 * middle of typing something else.  The idea is to be as unobtrusive as
 * possible, while still showing the user the information you want them to see.
 * Two examples are the volume control, and the brief message saying that your
 * settings have been saved.
 * <p>
 * The easiest way to use this class is to call one of the static methods that constructs
 * everything you need and returns a new Toast object.
 */ 
class Toast
{
    friend class CToastHelper;
private:
    class TN : public ElRefBase, ITransientNotification 
    {
    private:   
        class ShowAction : public Runnable
        {
        public:
            ShowAction(
                /* [in] */ TN* host);

            CARAPI Run();

        private:
            TN* mHost;
        };

        class HideAction : public Runnable
        {
        public:
            HideAction(
                /* [in] */ TN* host);

            CARAPI Run();

        private:
            TN* mHost;
        };    

    public:
        TN(
            /* [in] */ Toast* host);
        ~TN();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Show();

        CARAPI Hide();

        CARAPI HandleShow();

        CARAPI HandleHide();

    private:
        AutoPtr<IRunnable> mShow;
        AutoPtr<IRunnable> mHide;
        AutoPtr<IWindowManagerLayoutParams> mParams;
                //= new WindowManager.LayoutParams();
        AutoPtr<ILocalWindowManager> mWM;
        Toast* mHost;
    };

public:
    Toast(
        /* [in] */ IContext* context);

    CARAPI Show();

    CARAPI Cancel();

    CARAPI SetView(
        /* [in] */ IView* view);

    CARAPI GetView(
        /* [out] */ IView** view);

    CARAPI SetDuration(
        /* [in] */ Int32 duration);

    CARAPI GetDuration(
        /* [out] */ Int32* duration);

    CARAPI SetMargin(
        /* [in] */ Float horizontalMargin,
        /* [in] */ Float verticalMargin);
        
    CARAPI GetHorizontalMargin(
        /* [out] */ Float* horizontalMargin);

    CARAPI GetVerticalMargin(
        /* [out] */ Float* verticalMargin);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 xOffset, 
        /* [in] */ Int32 yOffset);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI GetXOffset(
        /* [out] */ Int32* xOffset);

    CARAPI GetYOffset(
        /* [out] */ Int32* yOffset);
    
    CARAPI SetText(
        /* [in] */ Int32 resId);

    CARAPI SetTextEx(
        /* [in] */ ICharSequence* s);

protected:
    Toast();

    CARAPI Init(
        /* [in] */ IContext* context);

protected:
    AutoPtr<IContext> mContext;
    Int32 mDuration;
    Int32 mGravity ;
    Int32 mX ;
    Int32 mY ;
    Float mHorizontalMargin;
    Float mVerticalMargin;
    AutoPtr<IView> mView;
    AutoPtr<IView> mNextView;
      
    AutoPtr<IApartment> mApartment;
    AutoPtr<TN> mTN;   
};

#endif
