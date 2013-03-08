#include "webkit/PluginFullScreenHolder.h"
#include "../../../res/gen/R.h"

PluginFullScreenHolder::PluginFullScreenHolder(
		/* [in] */ IWebView* webView, 
		/* [in] */ Int32 npp) : Dialog(NULL,0)
{
	IContext* tContext;
	webView -> GetContext(&tContext);
	tContext -> Release();
	Dialog::Init(tContext,(Int32)( R::style::Theme_NoTitleBar_Fullscreen ));
	mWebView = webView;
	mNpp = npp;
}


ECode PluginFullScreenHolder::SetContentView(
	/* [in] */ IView* contentView)
{

	return NOERROR;
}


ECode PluginFullScreenHolder::OnBackPressed()
{
	return NOERROR;
}


Boolean PluginFullScreenHolder::OnKeyDown(
	/* [in] */ Int32 keyCode, 
	/* [in] */ IKeyEvent* event)
{
	return FALSE;
}


Boolean PluginFullScreenHolder::OnKeyUp(
	/* [in] */ Int32 keyCode, 
	/* [in] */ IKeyEvent* event)
{
	return FALSE;
}


Boolean PluginFullScreenHolder::OnTouchEvent(
	/* [in] */ IMotionEvent* event)
{
	return FALSE;
}


Boolean PluginFullScreenHolder::OnTrackballEvent(
	/* [in] */ IMotionEvent* event)
{
	return FALSE;
}

PInterface PluginFullScreenHolder::Probe(
        /* [in] */ REIID riid)
{
	return NULL;
}

void PluginFullScreenHolder::OnStop()
{

}