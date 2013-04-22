#ifndef __VIEWMANAGER_H__
#define __VIEWMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

class ViewManager: public ElRefBase {

public:
    class ChildView : public ElRefBase
    {

    public:
        class Runnable: public ElRefBase, public IRunnable
        {

        public:
            CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface* Object,
                /* [out] */ InterfaceID* iID);

        public:
            virtual CARAPI Run();

        public:
            Runnable();
        };

        class RunnableAttachView: public Runnable
        {
        public:
            CARAPI Run();
        public:
            RunnableAttachView(
                /* [in] */ ChildView* childView,
                /* [in] */ ViewManager* viewManager);        
        public:
            ChildView* mChildView;
            ViewManager* mViewManager;
        };

        class RunnableRemoveView: public Runnable
        {
        public:
            CARAPI Run();
        public:
            RunnableRemoveView(
                /* [in] */ ChildView* childView);        
        public:
            ChildView* mChildView;
        };

        class RunnableSetVisibility: public Runnable
        {
        public:
            CARAPI Run();
        public:
            RunnableSetVisibility(
                /* [in] */ ISurfaceView* surfaceView);
        public:
            ISurfaceView* mSurfaceView;
        };

        class RunnablePostResetState: public Runnable
        {
        public:
            CARAPI Run();
        public:
            RunnablePostResetState(
                /* [in] */ ViewManager* viewManager);
        public:
            ViewManager* mViewManager;
        };

        class RunnablePostReadyToDraw: public Runnable
        {
        public:
            CARAPI Run();
        public:
            RunnablePostReadyToDraw(
                /* [in] */ ViewManager* viewManager);
        public:
            ViewManager* mViewManager;
        };

    public:
        Int32 mX;
        Int32 mY;
        Int32 mWidth;
        Int32 mHeight;
        AutoPtr<IView> mView; // generic view to show

    public:
        ViewManager * mViewManager;

    public:

        friend class Runnable;

        //package
        ChildView(
            /* [in] */  ViewManager * viewManager);

        //package
        virtual CARAPI_(void) SetBounds(
            /* [in] */ Int32 x, 
            /* [in] */ Int32 y, 
            /* [in] */ Int32 width, 
            /* [in] */ Int32 height);

        //package
        virtual CARAPI_(void) AttachView(
            /* [in] */ Int32 x, 
            /* [in] */ Int32 y, 
            /* [in] */ Int32 width, 
            /* [in] */ Int32 height);

        //package
        virtual CARAPI_(void) RemoveView();

    private:
        ChildView();

        CARAPI_(void) AttachViewOnUIThread();        

        CARAPI_(void) RemoveViewOnUIThread();
    };

public:

    friend class ChildView::Runnable;

    //package
    ViewManager(
        /* [in] */ IWebView* w);

    //package
    virtual CARAPI_(ChildView*) CreateView();

    //package
    virtual CARAPI_(void) StartZoom();

    //package
    virtual CARAPI_(void) EndZoom();

    //package
    virtual CARAPI_(void) ScaleAll();

    //package
    virtual CARAPI_(void) HideAll();

    //package
    virtual CARAPI_(void) ShowAll();

    //package
    virtual CARAPI_(void) PostResetStateAll();

    //package
    virtual CARAPI_(void) PostReadyToDrawAll();

    //package
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
    /*const*/ AutoPtr<IWebView> mWebView;
    /*const*/ List < AutoPtr<ChildView> >  mChildren;
    Boolean mHidden;
    Boolean mReadyToDraw;
    Boolean mZoomInProgress;

    // Threshold at which a surface is prevented from further increasing in size
    /*const*/ Int32 MAX_SURFACE_AREA;
    // GPU Limit (hard coded for now)
    static const Int32 MAX_SURFACE_DIMENSION = 2048;
};

#endif //__VIEWMANAGER_H__