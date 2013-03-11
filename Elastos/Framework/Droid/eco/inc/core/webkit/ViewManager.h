#ifndef __VIEWMANAGER_H__
#define __VIEWMANAGER_H__

#include "ext/frameworkext.h"

class ViewManager {

public:
    class ChildView 
    {
    public:
        Int32 x;
        Int32 y;
        Int32 width;
        Int32 height;
        IView* mView; // generic view to show

	public:
        ChildView();

        virtual CARAPI_(void) SetBounds(
        	/* [in] */ Int32 x, 
        	/* [in] */ Int32 y, 
        	/* [in] */ Int32 width, 
        	/* [in] */ Int32 height);

        virtual CARAPI_(void) AttachView(
        	/* [in] */ Int32 x, 
        	/* [in] */ Int32 y, 
        	/* [in] */ Int32 width, 
        	/* [in] */ Int32 height);

        virtual CARAPI_(void) removeView();

    private:
		CARAPI_(void) AttachViewOnUIThread();        

		CARAPI_(void) RemoveViewOnUIThread();
    };

public:

    ViewManager(
    	/* [in] */ IWebView* w);

    virtual CARAPI_(ChildView*) CreateView();



    virtual CARAPI_(void) StartZoom();

    virtual CARAPI_(void) EndZoom();

    virtual CARAPI_(void) ScaleAll();

    virtual CARAPI_(void) HideAll();

    virtual CARAPI_(void) ShowAll();

    virtual CARAPI_(void) PostResetStateAll();

    virtual CARAPI_(void) PostReadyToDrawAll();

    virtual CARAPI_(ChildView*) HitTest(
    	/* [in] */ Int32 contentX, 
    	/* [in] */ Int32 contentY);

private:
    /**
     * This should only be called from the UI thread.
     */
	CARAPI_(void) RequestLayout(
		/* [in] */ ChildView* v);

private:
	const IWebView* mWebView;
//	const ArrayList<ChildView> mChildren;
    Boolean mHidden;
    Boolean mReadyToDraw;
    Boolean mZoomInProgress;

    // Threshold at which a surface is prevented from further increasing in size
	const Int32 MAX_SURFACE_AREA;
    // GPU Limit (hard coded for now)
	static const Int32 MAX_SURFACE_DIMENSION = 2048;
};

#endif //__VIEWMANAGER_H__