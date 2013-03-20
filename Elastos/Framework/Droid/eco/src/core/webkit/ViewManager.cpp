#include "webkit/ViewManager.h"
#include "webkit/CWebView.h"
#include "view/View.h"
#include "utils/CDisplayMetrics.h"
#include "widget/CAbsoluteLayoutLayoutParams.h"
#include "view/CSurfaceView.h"
#include <elastos/Math.h>

PInterface ViewManager::ChildView::Runnable::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    return NULL;
}

UInt32 ViewManager::ChildView::Runnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewManager::ChildView::Runnable::Release()
{
    return ElRefBase::Release();
}

ECode ViewManager::ChildView::Runnable::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IRunnable*)this) {
        *iID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ViewManager::ChildView::Runnable::Run()
{
    return NOERROR;
}

ViewManager::ChildView::Runnable::Runnable()
{
}

ECode ViewManager::ChildView::RunnableAttachView::Run()
{
    mViewManager -> RequestLayout(mChildView);
    AutoPtr<IViewParent> pViewParent;
    (mChildView -> mView) -> GetParent((IViewParent**)&pViewParent);
    if(pViewParent == NULL) {
        mChildView -> AttachViewOnUIThread();
    }
    return NOERROR;
}

ViewManager::ChildView::RunnableAttachView::RunnableAttachView(
    /* [in] */ ChildView* childView,
    /* [in] */ ViewManager* viewManager)
{
    mChildView = childView;
    mViewManager = viewManager;
    return ;
}

ECode ViewManager::ChildView::RunnableRemoveView::Run()
{
    mChildView ->  RemoveViewOnUIThread();
    return NOERROR;
}

ViewManager::ChildView::RunnableRemoveView::RunnableRemoveView(
    /* [in] */ ChildView* childView)
{
    mChildView = childView;
    return ;
}

ECode ViewManager::ChildView::RunnableSetVisibility::Run()
{
    mSurfaceView -> SetVisibility(View_VISIBLE);
    return NOERROR;
}

ViewManager::ChildView::RunnableSetVisibility::RunnableSetVisibility(
    /* [in] */ ISurfaceView* surfaceView)
{
    mSurfaceView = surfaceView;
    return ;
}

ECode ViewManager::ChildView::RunnablePostResetState::Run()
{
    mViewManager -> mReadyToDraw = FALSE;
    return NOERROR;
}

ViewManager::ChildView::RunnablePostResetState::RunnablePostResetState(
    /* [in] */ ViewManager* viewManager)
{
    mViewManager = viewManager;
    return ;
}

ECode ViewManager::ChildView::RunnablePostReadyToDraw::Run()
{
    mViewManager -> mReadyToDraw = TRUE;

    Int32 nLen = (mViewManager -> mChildren).GetSize();
    List<AutoPtr<ChildView> >::Iterator iterT;
    List<AutoPtr<ChildView> >::Iterator iterE;
    iterT = (mViewManager -> mChildren).Begin();
    iterE = (mViewManager -> mChildren).End();
    for(int n = 0; n < nLen ; n ++ ) {
        (*iterT) -> mView -> SetVisibility(View_VISIBLE);
        iterT ++;
    }

    return NOERROR;
}

ViewManager::ChildView::RunnablePostReadyToDraw::RunnablePostReadyToDraw(
    /* [in] */ ViewManager* viewManager)
{
    mViewManager = viewManager;
    return ;
}

ViewManager::ChildView::ChildView()
{
    return ;
}

ViewManager::ChildView::ChildView(
    /* [in] */  ViewManager* viewManager)
{
    mViewManager = viewManager;
    return ;
}

void ViewManager::ChildView::SetBounds(
    /* [in] */ Int32 x, 
    /* [in] */ Int32 y, 
    /* [in] */ Int32 width, 
    /* [in] */ Int32 height)
{
    this -> mX = x;
    this -> mY = y;
    this -> mWidth = width;
    this -> mHeight = height;
}

void ViewManager::ChildView::AttachView(
    /* [in] */ Int32 x, 
    /* [in] */ Int32 y, 
    /* [in] */ Int32 width, 
    /* [in] */ Int32 height)
{
    if( mView == NULL)
    {
        return;
    }
    SetBounds(x, y, width, height);

    Boolean bPost = FALSE;
    AutoPtr<RunnableAttachView> pRunnable = new RunnableAttachView(this,mViewManager);
//    ( ((CWebView*)((mViewManager ->mWebView).Get())) -> mPrivateHandler ) -> Post((IRunnable*)pRunnable.Get(),&bPost);
}

void ViewManager::ChildView::AttachViewOnUIThread()
{
    ((CWebView*)((mViewManager -> mWebView).Get())) -> AddView(mView.Get());            //WebView e- AbsoluteLayout e- ViewGroup  ->  void ViewGroup::addView(View child);
    (mViewManager -> mChildren).PushBack(this);
    if( !(mViewManager -> mReadyToDraw) ) {
        mView -> SetVisibility(View::GONE);
    }
}

void ViewManager::ChildView::RemoveView()
{
    if( mView.Get() == NULL ) {
        return;
    }
    Boolean bPost = FALSE;
    AutoPtr<RunnableRemoveView> pRunnable = new RunnableRemoveView(this);
//    ( ((CWebView*)((mViewManager -> mWebView).Get())) -> mPrivateHandler ) -> Post((IRunnable*)pRunnable.Get(),&bPost);
}

void ViewManager::ChildView::RemoveViewOnUIThread()
{
    ((CWebView*)((mViewManager -> mWebView).Get())) -> RemoveViewInLayout(mView.Get());
    (mViewManager -> mChildren).Remove(this);
}

ViewManager::ViewManager(
    /* [in] */ IWebView* w)
{
    mWebView = w;
    AutoPtr<IResources> pResources;
    w -> GetResources((IResources**)&pResources);
    AutoPtr<IDisplayMetrics> pDisplayMetrics;
    pResources -> GetDisplayMetrics((IDisplayMetrics**)&pDisplayMetrics);
    CDisplayMetrics* pCDisplayMetrics = (CDisplayMetrics*)(pDisplayMetrics.Get());
    Int32 nWidthPixels = pCDisplayMetrics -> mWidthPixels;
    Int32 nHeightPixels = pCDisplayMetrics -> mHeightPixels;
    Int32 pixelArea = nWidthPixels * nHeightPixels;
    /* set the threshold to be 275% larger than the screen size. The
       percentage is simply an estimation and is not based on anything but
       basic trial-and-error tests run on multiple devices.
    */
    MAX_SURFACE_AREA = (Int32)(pixelArea * 2.75);
}

ViewManager::ChildView* ViewManager::CreateView()
{
    AutoPtr<ChildView> pChildView = new ChildView(this);
    return pChildView.Get();
}

void ViewManager::RequestLayout(
    /* [in] */ ChildView* v)
{
    Int32 width = ((CWebView*)(mWebView.Get())) -> ContentToViewDimension(v -> mWidth);
    Int32 height = ((CWebView*)(mWebView.Get())) -> ContentToViewDimension(v -> mHeight);
    Int32 x = ((CWebView*)(mWebView.Get())) -> ContentToViewX(v -> mX);
    Int32 y = ((CWebView*)(mWebView.Get())) -> ContentToViewY(v -> mY);

    IAbsoluteLayoutLayoutParams* lp;
    AutoPtr<IViewGroupLayoutParams> layoutParams;
    (v -> mView) -> GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);

    //lp = CAbsoluteLayoutLayoutParams::Probe(layoutParams.Get());
    lp = (IAbsoluteLayoutLayoutParams*)(layoutParams -> Probe(EIID_IAbsoluteLayoutLayoutParams));
    if( lp != NULL ) {
        ((CAbsoluteLayoutLayoutParams*)lp) -> mWidth = width;
        ((CAbsoluteLayoutLayoutParams*)lp) -> mHeight = height;
        ((CAbsoluteLayoutLayoutParams*)lp) -> mX = x;
        ((CAbsoluteLayoutLayoutParams*)lp) -> mY = y;
    }
    else {
        CAbsoluteLayoutLayoutParams::New(width, height, x, y,&lp);
    }

    // apply the layout to the view
    (v -> mView) -> SetLayoutParams(lp);

    //AutoPtr<ISurfaceView> sView = CSurfaceView::Probe( (v -> mView).Get());
    AutoPtr<ISurfaceView> sView = (ISurfaceView*)((v -> mView) -> Probe(EIID_ISurfaceView));
    if( sView.Get() != NULL ) {
        Boolean bIsFixedSize;
        sView -> IsFixedSize(&bIsFixedSize);
        if( bIsFixedSize && mZoomInProgress) {
            /* If we're already fixed, and we're in a zoom, then do nothing
              about the size. Just wait until we get called at the end of
              the zoom session (with mZoomInProgress false) and we'll
              fixup our size then.
            */
            return;
        }

        /* Compute proportional fixed width/height if necessary.
         *
         * NOTE: plugins (e.g. Flash) must not explicitly fix the size of
         * their surface. The logic below will result in unexpected behavior
         * for the plugin if they attempt to fix the size of the surface.
        */
        Int32 fixedW = width;
        Int32 fixedH = height;
        if (fixedW > MAX_SURFACE_DIMENSION || fixedH > MAX_SURFACE_DIMENSION)  {
            if ( (v -> mWidth) > (v -> mHeight) ) {
                fixedW = MAX_SURFACE_DIMENSION;
                fixedH = (v -> mHeight) * MAX_SURFACE_DIMENSION / (v -> mWidth);
            } 
            else {
                fixedH = MAX_SURFACE_DIMENSION;
                fixedW = (v -> mWidth) * MAX_SURFACE_DIMENSION / (v -> mHeight);
            }
        }
        if ( (fixedW * fixedH) > MAX_SURFACE_AREA)  {
            float area = MAX_SURFACE_AREA;
            if ( (v -> mWidth) > (v -> mHeight) ) {
                fixedW = (Int32) Math::Sqrt(area * (v -> mWidth) / (v -> mHeight));
                fixedH = (v -> mHeight) * fixedW / (v -> mWidth);
            }
            else  {
                fixedH = (Int32) Math::Sqrt(area * (v -> mHeight) / (v -> mWidth));
                fixedW = (v -> mWidth) * fixedH / (v -> mHeight);
            }
        }

        if (fixedW != width || fixedH != height) {
            // if we get here, either our dimensions or area (or both)
            // exeeded our max, so we had to compute fixedW and fixedH
            AutoPtr<ISurfaceHolder> sHolder;
            sView -> GetHolder((ISurfaceHolder**)&sHolder);
            sHolder -> SetFixedSize(fixedW, fixedH);
        } 
        else if (!bIsFixedSize && mZoomInProgress)  {
            // just freeze where we were (view size) until we're done with
            // the zoom progress
            AutoPtr<ISurfaceHolder> sHolder;
            sView -> GetHolder((ISurfaceHolder**)&sHolder);            
            Int32 sViewWidth = 0;
            Int32 sViewHeight = 0;
            sView -> GetWidth(&sViewWidth); //ISurfaceHolder : public View
            sView -> GetHeight(&sViewHeight);
            sHolder -> SetFixedSize( sViewWidth ,sViewHeight);
        }
        else if (bIsFixedSize && !mZoomInProgress)  {
            /* The changing of visibility is a hack to get around a bug in
             * the framework that causes the surface to revert to the size
             * it was prior to being fixed before it redraws using the
             * values currently in its layout.
             *
             * The surface is destroyed when it is set to invisible and then
             * recreated at the new dimensions when it is made visible. The
             * same destroy/create step occurs without the change in
             * visibility, but then exhibits the behavior described in the
             * previous paragraph.
            */
            Int32 visibility;
            sView -> GetVisibility(&visibility);
            if ( visibility == (View_INVISIBLE) )  {
                sView -> SetVisibility(View_INVISIBLE);
                AutoPtr<ISurfaceHolder> sHolder;
                sView -> GetHolder((ISurfaceHolder**)&sHolder);
                sHolder -> SetSizeFromLayout();
                // setLayoutParams() only requests the layout. If we set it
                // to VISIBLE now, it will use the old dimension to set the
                // size. Post a message to ensure that it shows the new size.

                Boolean bPost = FALSE;
                ChildView::RunnableSetVisibility * pRunnable = new ChildView::RunnableSetVisibility(sView.Get());
//                ( ((CWebView*)(mWebView.Get())) -> mPrivateHandler ) -> Post((IRunnable*)pRunnable,&bPost);
            }
            else  {
                AutoPtr<ISurfaceHolder> sHolder;
                sView -> GetHolder((ISurfaceHolder**)&sHolder);
                sHolder -> SetSizeFromLayout();
            }
        }
    }
}

void ViewManager::StartZoom()
{
    mZoomInProgress = TRUE;
    Int32 nLen = mChildren.GetSize();
    List<AutoPtr<ChildView> >::Iterator iterT;
    List<AutoPtr<ChildView> >::Iterator iterE;
    iterT = mChildren.Begin();
    iterE = mChildren.End();
    for(int n = 0; n < nLen ; n ++ ) {
        RequestLayout((*iterT).Get());
        iterT ++;
    }
}

void ViewManager::EndZoom()
{
    mZoomInProgress = FALSE;
    Int32 nLen = mChildren.GetSize();
    List<AutoPtr<ChildView> >::Iterator iterT;
    List<AutoPtr<ChildView> >::Iterator iterE;
    iterT = mChildren.Begin();
    iterE = mChildren.End();
    for(int n = 0; n < nLen ; n ++ ) {
        RequestLayout((*iterT).Get());
        iterT ++;
    }
}

void ViewManager::ScaleAll()
{
    Int32 nLen = mChildren.GetSize();
    List<AutoPtr<ChildView> >::Iterator iterT;
    List<AutoPtr<ChildView> >::Iterator iterE;
    iterT = mChildren.Begin();
    iterE = mChildren.End();
    for(int n = 0; n < nLen ; n ++ ) {
        RequestLayout((*iterT).Get());
        iterT ++;
    }
}

void ViewManager::HideAll()
{
    if (mHidden) {
        return;
    }
    Int32 nLen = mChildren.GetSize();
    List<AutoPtr<ChildView> >::Iterator iterT;
    List<AutoPtr<ChildView> >::Iterator iterE;
    iterT = mChildren.Begin();
    iterE = mChildren.End();
    for(int n = 0; n < nLen ; n ++ ) {
        (*iterT) -> mView -> SetVisibility(View_GONE);
        iterT ++;
    }
    mHidden = TRUE;
}

void ViewManager::ShowAll()
{
    if (!mHidden)  {
        return;
    }
    Int32 nLen = mChildren.GetSize();
    List<AutoPtr<ChildView> >::Iterator iterT;
    List<AutoPtr<ChildView> >::Iterator iterE;
    iterT = mChildren.Begin();
    iterE = mChildren.End();
    for(int n = 0; n < nLen ; n ++ ) {
        (*iterT) -> mView -> SetVisibility(View_VISIBLE);
        iterT ++;
    }
    mHidden = FALSE;
}

void ViewManager::PostResetStateAll()
{
    Boolean bPost = FALSE;
    AutoPtr<ChildView::RunnablePostResetState> pRunnable = new ChildView::RunnablePostResetState(this);
//    ( ((CWebView*)(mWebView.Get())) -> mPrivateHandler ) -> Post((IRunnable*)pRunnable.Get(),&bPost);
}

void ViewManager::PostReadyToDrawAll()
{
    Boolean bPost = FALSE;
    AutoPtr<ChildView::RunnablePostReadyToDraw> pRunnable = new ChildView::RunnablePostReadyToDraw(this);
//    ( ((CWebView*)(mWebView.Get())) -> mPrivateHandler ) -> Post((IRunnable*)pRunnable.Get(),&bPost);
}

ViewManager::ChildView* ViewManager::HitTest(
    /* [in] */ Int32 contentX, 
    /* [in] */ Int32 contentY)
{
    if (mHidden)  {
        return NULL;
    }
    Int32 nLen = mChildren.GetSize();
    List<AutoPtr<ChildView> >::Iterator iterT;
    List<AutoPtr<ChildView> >::Iterator iterE;
    iterT = mChildren.Begin();
    iterE = mChildren.End();
    Int32 nVisibility = 0;
    for(int n = 0; n < nLen ; n ++ ) {
        (*iterT) -> mView -> GetVisibility(&nVisibility);
        if (nVisibility == View_VISIBLE)   {
            if(contentX >= ((*iterT) ->mX) && contentX < ( ((*iterT) -> mX) + ((*iterT) ->mWidth) ) 
                        && contentY >= ((*iterT) ->mY) && contentY < (((*iterT) ->mY) + ((*iterT) -> mHeight)) )  {
                return (*iterT).Get();
            }
        }
        iterT ++;
    }
    return NULL;
}

