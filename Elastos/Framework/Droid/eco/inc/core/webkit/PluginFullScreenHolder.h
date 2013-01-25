#ifndef __PLUGINFULLSCREENHOLDER_H__
#define __PLUGINFULLSCREENHOLDER_H__

class PluginFullScreenHolder : public Dialog {

public:
	PluginFullScreenHolder(
		/* [in] */ WebView webView, 
		/* [in] */ Int32 npp);

public:
    //@Override
	virtual CARAPI_(void) SetContentView(
		/* [in] */ View contentView);

    //@Override
	virtual CARAPI_(void) OnBackPressed();

    //@Override
	virtual CARAPI_(Boolean) OnKeyDown(
		/* [in] */ Int32 keyCode, 
		/* [in] */ KeyEvent event);

    //@Override
	virtual CARAPI_(Boolean) OnKeyUp(
		/* [in] */ Int32 keyCode, 
		/* [in] */ KeyEvent event);

    //@Override
	virtual CARAPI_(Boolean) OnTouchEvent(
		/* [in] */ MotionEvent event);

    //@Override
	virtual CARAPI_(Boolean) OnTrackballEvent(
		/* [in] */ MotionEvent event);

protected:
    //@Override
	virtual CARAPI_(void) OnStop();

private:
	const WebView mWebView;
	const Int32 mNpp;
	View mContentView;

};

#endif //__PLUGINFULLSCREENHOLDER_H__