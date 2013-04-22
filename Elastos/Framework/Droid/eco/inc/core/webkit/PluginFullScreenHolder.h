#ifndef __PLUGINFULLSCREENHOLDER_H__
#define __PLUGINFULLSCREENHOLDER_H__

#include "app/Dialog.h"

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

class PluginFullScreenHolder : public Dialog {

public:
    /*package*/
    PluginFullScreenHolder(
        /* [in] */ IWebView* webView, 
        /* [in] */ Int32 npp);

public:
    //@Override
    //public
    virtual CARAPI SetContentView(
        /* [in] */ IView* contentView);

    //@Override
    //public
    virtual CARAPI OnBackPressed();

    //@Override
    //public
    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode, 
        /* [in] */ IKeyEvent* event);

    //@Override
    //public
    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode, 
        /* [in] */ IKeyEvent* event);

    //@Override
    //public
    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    //public
    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

protected:
    //@Override
    virtual CARAPI_(void) OnStop();

private:
    /*const*/ AutoPtr<IWebView> mWebView;
    Int32 mNpp;
    AutoPtr<IView> mContentView;

};

#endif //__PLUGINFULLSCREENHOLDER_H__