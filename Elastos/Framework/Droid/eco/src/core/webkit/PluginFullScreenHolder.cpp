#include "webkit/PluginFullScreenHolder.h"

#include "../../../res/gen/R.h"
#include "view/CViewGroupLayoutParams.h"
#include "webkit/CWebView.h"

PluginFullScreenHolder::PluginFullScreenHolder(
        /* [in] */ IWebView* webView, 
        /* [in] */ Int32 npp) : Dialog(NULL,0)
{
    AutoPtr<IContext> tContext;
    webView->GetContext((IContext**)&tContext);
    Dialog::Init(tContext.Get(),(Int32)( R::style::Theme_NoTitleBar_Fullscreen ));
    mWebView = webView;
    mNpp = npp;
}

ECode PluginFullScreenHolder::SetContentView(
    /* [in] */ IView* contentView)
{
    // as we are sharing the View between full screen and
    // embedded mode, we have to remove the
    // AbsoluteLayout.LayoutParams set by embedded mode to
    // ViewGroup.LayoutParams before adding it to the dialog
    
    AutoPtr<IViewGroupLayoutParams> pViewGroupLayoutParams;
    CViewGroupLayoutParams::New(ViewGroupLayoutParams::MATCH_PARENT,ViewGroupLayoutParams::MATCH_PARENT,(IViewGroupLayoutParams**)&pViewGroupLayoutParams);
    contentView->SetLayoutParams( pViewGroupLayoutParams.Get() );

    // fixed size is only used either during pinch zoom or surface is too
    // big. Make sure it is not fixed size before setting it to the full
    // screen content view. The SurfaceView will be set to the correct mode
    // by the ViewManager when it is re-attached to the WebView.
    ISurfaceView* pISurfaceView = ISurfaceView::Probe(contentView);    
    if( pISurfaceView != NULL ) {
        Boolean bFixedSize;
        pISurfaceView->IsFixedSize(&bFixedSize);
        if(bFixedSize) {
            AutoPtr<ISurfaceHolder> pHolder;
            pISurfaceView->GetHolder((ISurfaceHolder**)&pHolder);
            pHolder->SetSizeFromLayout();
        }
    }
    Dialog::SetContentView(contentView);
    mContentView = contentView;    
    return NOERROR;
}

ECode PluginFullScreenHolder::OnBackPressed()
{
    //JAVA:mWebView.mPrivateHandler.obtainMessage(WebView.HIDE_FULLSCREEN).sendToTarget();
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    //return ((CWebView::PrivateHandler*)((((CWebView*)(mWebView.Get()))->mPrivateHandler).Get()))->SendMessage(CWebView::HIDE_FULLSCREEN,params);
    return E_NOT_IMPLEMENTED;
}

Boolean PluginFullScreenHolder::OnKeyDown(
    /* [in] */ Int32 keyCode, 
    /* [in] */ IKeyEvent* event)
{
    Boolean bIsSystem;
    event -> IsSystem(&bIsSystem);
    if(bIsSystem) {
        return Dialog::OnKeyDown(keyCode, event);
    }
    ((CWebView*)(mWebView.Get())) -> OnKeyDown(keyCode, event);
    // always return true as we are the handler
    return TRUE;
}

Boolean PluginFullScreenHolder::OnKeyUp(
    /* [in] */ Int32 keyCode, 
    /* [in] */ IKeyEvent* event)
{
    Boolean bIsSystem;
    event -> IsSystem(&bIsSystem);
    if(bIsSystem) {
        return Dialog::OnKeyUp(keyCode, event);
    }
    // always return true as we are the handler
    return TRUE;
}

Boolean PluginFullScreenHolder::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    // always return true as we don't want the event to propagate any further
    return TRUE;
}

Boolean PluginFullScreenHolder::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    Boolean bResult;
    mWebView -> OnTrackballEvent(event,&bResult);
    // always return true as we are the handler
    return TRUE;
}

PInterface PluginFullScreenHolder::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IDialog) {
        return (IDialog*)this;
    }
    return NULL;
}

void PluginFullScreenHolder::OnStop()
{
    Dialog::OnStop();
    // manually remove the contentView's parent since the dialog does not
    AutoPtr<IViewParent> pViewParent;
    mContentView -> GetParent( (IViewParent**)&pViewParent );
    if( mContentView != NULL && pViewParent.Get() != NULL ) {
        IViewGroup* vg = (IViewGroup *)(pViewParent.Get());
        //vg -> RemoveView(mContentView.Get());
    }
    AutoPtr<WebViewCore> pWebViewCore;
    pWebViewCore = ((CWebView*)(mWebView.Get()))-> GetWebViewCore();
    pWebViewCore -> SendMessage(WebViewCore::EventHub::HIDE_FULLSCREEN, mNpp, 0);
}