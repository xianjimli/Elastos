
#include "GeckoApp.h"
#include "GeckoAppShell.h"
#include "CGeckoSurfaceView.h"
#include "GeckoErrors.h"
#include "GeckoEvent.h"
//#include "ByteBuffer.h"
#include <stdio.h>

Boolean CGeckoSurfaceView::sShowingSplashScreen = FALSE;
String CGeckoSurfaceView::sSplashStatusMsg("");

CGeckoSurfaceView::MySurfaceHoderCallback::MySurfaceHoderCallback(
    /* [in]  */ CGeckoSurfaceView* pHost)
    : mHost(pHost)
    , mRef(0)
{
}

PInterface CGeckoSurfaceView::MySurfaceHoderCallback::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ISurfaceHolderCallback*)this;
    }
    else if (riid == EIID_ISurfaceHolderCallback) {
        return (ISurfaceHolderCallback*)this;
    }

    return NULL;
}

UInt32 CGeckoSurfaceView::MySurfaceHoderCallback::AddRef()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is increased
    Int32 ref = atomic_inc(&mRef);
    // so we should increase ref before return
    return ++ref;
}

UInt32 CGeckoSurfaceView::MySurfaceHoderCallback::Release()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is decreased
    Int32 ref = atomic_dec(&mRef);
    // so we should decrease ref
    if (--ref == 0) {
        delete this;
    }
    assert(ref >= 0);
    return ref;
}

ECode CGeckoSurfaceView::MySurfaceHoderCallback::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }
    if (pObject == (IInterface*)(ISurfaceHolderCallback*)this) {
        *pIID = EIID_ISurfaceHolderCallback;
    }
    else if (pObject == (IInterface*)(IObject*)this) {
        *pIID = EIID_IObject;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CGeckoSurfaceView::MySurfaceHoderCallback::SurfaceCreated(
    /* [in] */ ISurfaceHolder* pHolder)
{
    GeckoEvent* pGeckoEvent = new GeckoEvent(GeckoEvent::SURFACE_CREATED);
    if (!pGeckoEvent) return E_OUT_OF_MEMORY;
    GeckoAppShell::SendEventToGecko(pGeckoEvent);
    if (sShowingSplashScreen) {
        mHost->DrawSplashScreen();
    }

    return NOERROR;
}

ECode CGeckoSurfaceView::MySurfaceHoderCallback::SurfaceChanged(
    /* [in] */ ISurfaceHolder* pHolder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (sShowingSplashScreen) {
        mHost->DrawSplashScreen(pHolder, width, height);
    }
    //mSurfaceLock.Lock();

    //try {
        if (mHost->mInDrawing) {
            printf("GeckoAppJava surfaceChanged while mInDrawing is true!\n");
        }

        Boolean invalidSize;

        if (width == 0 || height == 0) {
            if (mHost->mSoftwareBuffer) {
                mHost->mSoftwareBuffer->Release();
                mHost->mSoftwareBuffer = NULL;
            }

            if (mHost->mSoftwareBufferCopy) {
                mHost->mSoftwareBufferCopy->Release();
                mHost->mSoftwareBufferCopy = NULL;
            }

            if (mHost->mSoftwareBitmap) {
                mHost->mSoftwareBitmap->Release();
                mHost->mSoftwareBitmap = NULL;
            }
            invalidSize = TRUE;
        }
        else {
            invalidSize = FALSE;
        }

        Boolean result;
        GeckoApp::CheckLaunchState(GeckoApp::LaunchState_GeckoRunning, &result);
        Boolean doSyncDraw = mHost->mDrawMode == DRAW_2D  && !invalidSize && result;
        mHost->mSyncDraw = doSyncDraw;

        mHost->mFormat = format;
        mHost->mWidth = width;
        mHost->mHeight = height;
        mHost->mSurfaceValid = TRUE;

        printf("GeckoAppJava sufaceChanged: fmt: %d, dim: %d %d\n",
             format, width, height);

        IDisplayMetrics* pIMetrics;
        ECode ec = CDisplayMetrics::New(&pIMetrics);
        if (FAILED(ec)) {
            //mSurfaceLock.Unlock();
            return ec;
        }

        IDisplay* pIDisplay;
        ec = GeckoApp::mAppContext->GetWindowManager()->GetDefaultDisplay(&pIDisplay);
        if (FAILED(ec)) {
            pIMetrics->Release();
            //mSurfaceLock.Unlock();
            return ec;
        }

        ec = pIDisplay->GetMetrics(pIMetrics);
        pIDisplay->Release();
        if (FAILED(ec)) {
            pIMetrics->Release();
            //mSurfaceLock.Unlock();
            return ec;
        }

        /*GeckoEvent* pGeckoEvent = new GeckoEvent(
                    GeckoEvent::SIZE_CHANGED, width, height,
                    pIMetrics->mWidthPixels, pIMetrics->mHeightPixels);
        if (!pGeckoEvent) return E_OUT_OF_MEMORY;
        GeckoAppShell::SendEventToGecko(pGeckoEvent);*/
        pIMetrics->Release();

        if (doSyncDraw) {
            if (mHost->mDrawMode == DRAW_GLES_2 || sShowingSplashScreen) {
                //mSurfaceLock.Unlock();
                return NOERROR;
            }
            ICanvas* pICanvas;
            ec = pHolder->LockCanvas(&pICanvas);
            if (FAILED(ec)) {
                //mSurfaceLock.Unlock();
                return ec;
            }
            pICanvas->DrawARGB(255, 255, 255, 255);
            pHolder->UnlockCanvasAndPost(pICanvas);
            pICanvas->Release();
        }
        else {
            GeckoAppShell::ScheduleRedraw();
        }
        //mSurfaceLock.Unlock();
    //} finally {
    //    mSurfaceLock.Unlock();
    //}

    IInterface* pSyncDrawObject = NULL;
    //ec = mSyncDraws->Take(&pSyncDrawObject);
    if (FAILED(ec)) return ec;

    IBitmap* pIBitmap = IBitmap::Probe(pSyncDrawObject);
    if (pIBitmap) {
        ec =  mHost->Draw(pHolder, pIBitmap);
    }
    else {
        ec = mHost->Draw(pHolder, IBuffer::Probe(pSyncDrawObject));
    }
    pSyncDrawObject->Release();
    return ec;
}

ECode CGeckoSurfaceView::MySurfaceHoderCallback::SurfaceDestroyed(
    /* [in] */ ISurfaceHolder* pHolder)
{
    mHost->mSurfaceValid = FALSE;
    if (mHost->mSoftwareBuffer) {
        mHost->mSoftwareBuffer->Release();
        mHost->mSoftwareBuffer = NULL;
    }

    if (mHost->mSoftwareBufferCopy) {
        mHost->mSoftwareBufferCopy->Release();
        mHost->mSoftwareBufferCopy = NULL;
    }

    if (mHost->mSoftwareBitmap) {
        mHost->mSoftwareBitmap->Release();
        mHost->mSoftwareBitmap = NULL;
    }

    GeckoEvent* pEvent = new GeckoEvent(GeckoEvent::SURFACE_DESTROYED);
    if (!pEvent) return E_OUT_OF_MEMORY;
    return GeckoAppShell::SendEventToGecko(pEvent);
}

ECode CGeckoSurfaceView::constructor(
    /* [in] */ IContext* pContext)
{
    ECode ec = CGeckoSurfaceView::New(pContext, &mSurfaceView);
    if (FAILED(ec)) return ec;

    ISurfaceHolder* pIHolder;
    ec = GetHolder(&pIHolder);
    if (FAILED(ec)) return ec;

    MySurfaceHoderCallback* pCallback = new MySurfaceHoderCallback(this);
    if (!pCallback) return E_OUT_OF_MEMORY;

    pIHolder->AddCallback(pCallback);
    pIHolder->Release();

    ec = CGeckoInputConnection::New(this, &mInputConnection);
    if (FAILED(ec)) return ec;
    SetFocusable(TRUE);
    SetFocusableInTouchMode(TRUE);

    IDisplayMetrics* pIMetrics;
    ec = CDisplayMetrics::New(&pIMetrics);
    if (FAILED(ec)) return ec;

    IDisplay* pIDisplay;
    ec = GeckoApp::mAppContext->GetWindowManager()->GetDefaultDisplay(&pIDisplay);
    if (FAILED(ec)) {
        pIMetrics->Release();
        return ec;
    }

    ec = pIDisplay->GetMetrics(pIMetrics);
    pIDisplay->Release();
    if (FAILED(ec)) {
        pIMetrics->Release();
        return ec;
    }

    //mWidth = pIMetrics->mWidthPixels;
    //mHeight = pIMetrics->mHeightPixels;
    pIMetrics->Release();

    ec = CEditableFactory::AcquireSingleton(&mEditableFactory);
    if (FAILED(ec)) return ec;

    ICharSequence* pICharSequence;
    ec = CStringWrapper::New(String(""), &pICharSequence);
    if (FAILED(ec)) return ec;
    InitEditable(pICharSequence);
    pICharSequence->Release();

    return NOERROR;
}

CGeckoSurfaceView::CGeckoSurfaceView()
    : mInputConnection(NULL)
    , mIMEState(IME_STATE_DISABLED)
    , mIMELandscapeFS(FALSE)
    , mSurfaceValid(FALSE)
    , mInDrawing(FALSE)
    , mSyncDraw(FALSE)
    , mDrawMode(0)
    , mFormat(0)
    , mWidth(0)
    , mHeight(0)
    , mBufferWidth(0)
    , mBufferHeight(0)
    , mKeyListener(NULL)
    , mEditable(NULL)
    , mEditableFactory(NULL)
    , mSoftwareBitmap(NULL)
    , mSoftwareBuffer(NULL)
    , mSoftwareBufferCopy(NULL)
    , mSurfaceView(NULL)
{
    mIMETypeHint = "";
    mIMEActionHint = "";
}

CGeckoSurfaceView::~CGeckoSurfaceView()
{
    if (mSoftwareBufferCopy) {
        mSoftwareBufferCopy->Release();
    }
    if (mSoftwareBuffer) {
        mSoftwareBuffer->Release();
    }
    if (mSoftwareBitmap) {
        mSoftwareBitmap->Release();
    }
    if (mEditableFactory) {
        mEditableFactory->Release();
    }
    if (mEditable) {
        mEditable->Release();
    }
    if (mKeyListener) {
        mKeyListener->Release();
    }
    if (mInputConnection) {
        mInputConnection->Release();
    }
    if (mSurfaceView) {
        mSurfaceView->Release();
    }

}

ECode CGeckoSurfaceView::Finalize()
{
    //return mSurfaceView->Finalize();
    return E_NOT_IMPLEMENTED;
}

ECode CGeckoSurfaceView::DrawSplashScreen()
{
    ISurfaceHolder* pISurfaceHodler;
    ECode ec = GetHolder(&pISurfaceHodler);
    if (FAILED(ec)) return ec;

    ec = DrawSplashScreen(pISurfaceHodler, mWidth, mHeight);
    pISurfaceHodler->Release();
    return ec;
}

ECode CGeckoSurfaceView::DrawSplashScreen(
    /* [in] */ ISurfaceHolder* pHolder,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ICanvas* pICanvas;
    ECode ec = pHolder->LockCanvas(&pICanvas);
    if (FAILED(ec)) {
        printf("GeckoSurfaceView LockCanvas failed %x\n", ec);
        return ec;
    }

    IResources* pIResources = NULL;
    ec = GetResources(&pIResources);
    if (FAILED(ec)) goto exit;

    Int32 color;
    ec = pIResources->GetColor(0x7f040000, &color); //R.color.splash_background
    if (FAILED(ec)) goto exit;
    ec = pICanvas->DrawColor(color);
    if (FAILED(ec)) goto exit;

    IDrawable* pIDrawable;
    ec = pIResources->GetDrawable(0x7f020005, &pIDrawable);//R.drawable.splash
    if (FAILED(ec)) goto exit;
    Int32 x, y, w, h;
    pIDrawable->GetIntrinsicWidth(&w);
    pIDrawable->GetIntrinsicHeight(&h);
    x = (width - w)/2;
    y = (height - h)/2 - 16;
    pIDrawable->SetBounds(x, y, x + w, y + h);
    pIDrawable->Draw(pICanvas);
    pIDrawable->Release();

    IPaint* pIPaint;
    ec = CPaint::New(&pIPaint);
    if (FAILED(ec)) goto exit;
    pIPaint->SetTextAlign(PaintAlign_CENTER);
    pIPaint->SetTextSize(32);
    pIPaint->SetAntiAlias(TRUE);
    ec = pIResources->GetColor(0x7f040001, &color);//R.color.splash_font
    if (FAILED(ec)) {
        pIPaint->Release();
        goto exit;
    }
    pIPaint->SetColor(color);

    ec = pICanvas->DrawTextInString(
                sSplashStatusMsg, width/2, y + h + 16, pIPaint);
    pIPaint->Release();
    if (FAILED(ec)) goto exit;
    ec = pHolder->UnlockCanvasAndPost(pICanvas);
exit:
    pICanvas->Release();
    if (pIResources != NULL) {
        pIResources->Release();
    }
    return ec;
}

ECode CGeckoSurfaceView::Draw(
    /* [in] */ ISurfaceHolder* pHolder,
    /* [in] */ IBuffer* pBuffer)
{
    if (!pHolder || !pBuffer) {
        return E_INVALID_ARGUMENT;
    }
    Int32 capacity;
    ECode ec = pBuffer->Capacity(&capacity);
    if (FAILED(ec)) return ec;
    if (capacity != (mWidth * mHeight * 2)) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mSoftwareBufferLock);
    if (pBuffer != mSoftwareBuffer || !mSoftwareBufferCopy)
        return E_INVALID_ARGUMENT;

    ICanvas* pICanvas;
    ec = pHolder->LockCanvas(&pICanvas);
    if (FAILED(ec)) return ec;
    ec = mSoftwareBufferCopy->CopyPixelsFromBuffer(pBuffer);
    if (FAILED(ec)) {
        pICanvas->Release();
        return ec;
    }
    ec = pICanvas->DrawBitmap(mSoftwareBufferCopy, 0, 0, NULL);
    if (FAILED(ec)) {
        pICanvas->Release();
        return ec;
    }
    ec = pHolder->UnlockCanvasAndPost(pICanvas);
    pICanvas->Release();
    return ec;
}

ECode CGeckoSurfaceView::Draw(
    /* [in] */ ISurfaceHolder* pHolder,
    /* [in] */ IBitmap* pBitmap)
{
    if (!pHolder || !pBitmap) {
        return E_INVALID_ARGUMENT;
    }
    Int32 width, height;
    ECode ec = pBitmap->GetWidth(&width);
    if (FAILED(ec)) return ec;
    ec = pBitmap->GetHeight(&height);
    if (FAILED(ec)) return ec;
    if (width != mWidth || height != mHeight) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mSoftwareBitmapLock);
    if (pBitmap != mSoftwareBitmap)
        return E_INVALID_ARGUMENT;

    ICanvas* pICanvas;
    ec = pHolder->LockCanvas(&pICanvas);
    if (FAILED(ec)) return ec;
    ec = pICanvas->DrawBitmap(pBitmap, 0, 0, NULL);
    if (FAILED(ec)) {
        pICanvas->Release();
        return ec;
    }
    ec = pHolder->UnlockCanvasAndPost(pICanvas);
    pICanvas->Release();
    return ec;
}

ECode CGeckoSurfaceView::GetSoftwareDrawBitmap(
    /* [out] */IBitmap** ppBitmap)
{
    if (!ppBitmap) {
        return E_INVALID_ARGUMENT;
    }
    Int32 width = mWidth;
    Int32 height = mHeight;
    ECode ec;
    if (mSoftwareBitmap) {
        mSoftwareBitmap->GetHeight(&height);
        mSoftwareBitmap->GetWidth(&width);
    }
    if (!mSoftwareBitmap || height != mHeight || width != mWidth) {
        if (mSoftwareBitmap) {
            mSoftwareBitmap->Release();
            mSoftwareBitmap = NULL;
        }
        IBitmapFactory* pIBitmapFactory;
        ec = CBitmapFactory::AcquireSingleton(&pIBitmapFactory);
        if (FAILED(ec)) return ec;

        ec = pIBitmapFactory->CreateBitmapEx3(mWidth, mHeight,
                         BitmapConfig_RGB_565, &mSoftwareBitmap);
        pIBitmapFactory->Release();
        if (FAILED(ec)) return ec;
    }

    mDrawMode = DRAW_2D;
    *ppBitmap = mSoftwareBitmap;
    mSoftwareBitmap->AddRef();
    return NOERROR;
}

ECode CGeckoSurfaceView::GetSoftwareDrawBuffer(
    /* [out] */ IBuffer** ppBuffer)
{
    if (!ppBuffer) {
        return E_INVALID_ARGUMENT;
    }
    // We store pixels in 565 format, so two bytes per pixel (explaining
    // the * 2 in the following check/allocation)
    Int32 capacity = mWidth * mHeight * 2;
    ECode ec;
    if (mSoftwareBuffer) {
        mSoftwareBuffer->Capacity(&capacity);
    }
    if (!mSoftwareBuffer || capacity != (mWidth * mHeight * 2)) {
        if (mSoftwareBuffer) {
            mSoftwareBuffer->Release();
            mSoftwareBuffer = NULL;
        }
        //ec = ByteBuffer::AllocateDirect(mWidth * mHeight * 2, &mSoftwareBuffer);
        //if (FAILED(ec)) return ec;
    }

    Int32 width = mWidth;
    Int32 height = mHeight;
    if (mSoftwareBufferCopy) {
        mSoftwareBufferCopy->GetHeight(&height);
        mSoftwareBufferCopy->GetWidth(&width);
    }
    if (!mSoftwareBufferCopy || height != mHeight || width != mWidth) {
        if (mSoftwareBufferCopy) {
            mSoftwareBufferCopy->Release();
            mSoftwareBufferCopy = NULL;
        }
        IBitmapFactory* pIBitmapFactory;
        ec = CBitmapFactory::AcquireSingleton(&pIBitmapFactory);
        if (FAILED(ec)) return ec;

        ec = pIBitmapFactory->CreateBitmapEx3(mWidth, mHeight,
                         BitmapConfig_RGB_565, &mSoftwareBufferCopy);
        pIBitmapFactory->Release();
        if (FAILED(ec)) return ec;
    }

    mDrawMode = DRAW_2D;
    *ppBuffer = mSoftwareBuffer;
    mSoftwareBuffer->AddRef();
    return NOERROR;
}

ECode CGeckoSurfaceView::BeginDrawing(
    /* [out] */ Int32* pResult)
{
    if (!pResult) {
        return E_INVALID_ARGUMENT;
    }
    if (mInDrawing) {
        printf("GeckoAppJava Recursive beginDrawing call!\n");
        *pResult = DRAW_ERROR;
        return NOERROR;
    }

    /* Grab the lock, which we'll hold while we're drawing.
     * It gets released in endDrawing(), and is also used in surfaceChanged
     * to make sure that we don't change our surface details while
     * we're in the middle of drawing (and especially in the middle of
     * executing beginDrawing/endDrawing).
     *
     * We might not need to hold this lock in between
     * beginDrawing/endDrawing, and might just be able to make
     * surfaceChanged, beginDrawing, and endDrawing synchronized,
     * but this way is safer for now.
     */
    //mSurfaceLock.Lock();

    if (!mSurfaceValid) {
        printf("GeckoAppJava Surface not valid\n");
        //mSurfaceLock.Unlock();
        *pResult = DRAW_ERROR;
        return NOERROR;
    }

    mInDrawing = TRUE;
    mDrawMode = DRAW_GLES_2;
    *pResult = DRAW_GLES_2;
    return NOERROR;
}

ECode CGeckoSurfaceView::EndDrawing()
{
    if (!mInDrawing) {
        printf("GeckoAppJava endDrawing without beginDrawing!\n");
        return NOERROR;
    }

    ECode ec = NOERROR;
    if (!mSurfaceValid) {
        printf("GeckoAppJava endDrawing with false mSurfaceValid\n");
        ec = E_GECKO_ERROR_INVALID_SURFACE;
    }
    mInDrawing = false;

    /*if (!mSurfaceLock.isHeldByCurrentThread()) {
        printf("GeckoAppJava endDrawing while mSurfaceLock not held by current thread!\n");
    }

    mSurfaceLock.Unlock();*/
    return ec;
}

/* How this works:
 * Whenever we want to draw, we want to be sure that we do not lock
 * the canvas unless we're sure we can draw. Locking the canvas clears
 * the canvas to black in most cases, causing a black flash.
 * At the same time, the surface can resize/disappear at any moment
 * unless the canvas is locked.
 * Draws originate from a different thread so the surface could change
 * at any moment while we try to draw until we lock the canvas.
 *
 * Also, never try to lock the canvas while holding the surface lock
 * unless you're in SurfaceChanged, in which case the canvas was already
 * locked. Surface lock -> Canvas lock will lead to AB-BA deadlocks.
 */
ECode CGeckoSurfaceView::Draw2D(
    /* [in] */ IBitmap* pBitmap,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (!pBitmap) {
        return E_INVALID_ARGUMENT;
    }
    // mSurfaceLock ensures that we get mSyncDraw/mSoftwareBitmap/etc.
    // set correctly before determining whether we should do a sync draw
    //mSurfaceLock.Lock();

    ECode ec = NOERROR;
    if (mSyncDraw) {
        if (pBitmap != mSoftwareBitmap || width != mWidth || height != mHeight) {
            //mSurfaceLock.Unlock();
            return E_INVALID_ARGUMENT;
        }
        mSyncDraw = FALSE;

        //ec = mSyncDraws.put(pBitmap);
        //mSurfaceLock.Unlock();
        if (FAILED(ec)) {
            printf("GeckoAppJava getting sync draws queue error:%x\n", ec);
        }
        return ec;
    }
    //mSurfaceLock.Unlock();

    ISurfaceHolder* pISurfaceHolder;
    ec = GetHolder(&pISurfaceHolder);
    if (FAILED(ec)) return ec;

    ec = Draw(pISurfaceHolder, pBitmap);
    pISurfaceHolder->Release();
    return ec;
}

ECode CGeckoSurfaceView::Draw2D(
    /* [in] */ IBuffer* pBuffer,
    /* [in] */ Int32 stride)
{
    if (!pBuffer) {
        return E_INVALID_ARGUMENT;
    }
    //mSurfaceLock.Lock();

    ECode ec = NOERROR;
    if (mSyncDraw) {
        if (pBuffer != mSoftwareBuffer || stride != (mWidth * 2)) {
            //mSurfaceLock.Unlock();
            return E_INVALID_ARGUMENT;
        }
        mSyncDraw = FALSE;

        //ec = mSyncDraws.put(pBuffer);

        //mSurfaceLock.Unlock();
        if (FAILED(ec)) {
            printf("GeckoAppJava getting sync bitmaps queue error:%x\n", ec);
        }
        return ec;
    }
    //mSurfaceLock.Unlock();

    ISurfaceHolder* pISurfaceHolder;
    ec = GetHolder(&pISurfaceHolder);
    if (FAILED(ec)) return ec;

    ec = Draw(pISurfaceHolder, pBuffer);
    pISurfaceHolder->Release();
    return ec;
}

ECode CGeckoSurfaceView::SetEditable(
    /* [in] */ ICharSequence* pContents)
{
    ECode ec = mEditable->RemoveSpan(mInputConnection);
    if (FAILED(ec)) return ec;

    Int32 length = 0;
    ec = mEditable->GetLength(&length);
    if (FAILED(ec)) return ec;

    IEditable* pIEditable;
    ec = mEditable->ReplaceEx(0, length, pContents, &pIEditable);
    if (FAILED(ec)) return ec;
    pIEditable->Release();

    ec = pContents->GetLength(&length);
    if (FAILED(ec)) return ec;
    ec = mEditable->SetSpan(mInputConnection,
                 0, length, Spanned_SPAN_INCLUSIVE_INCLUSIVE);
    if (FAILED(ec)) return ec;

    //ec = Selection::SetSelection(mEditable, length);
    return ec;
}

ECode CGeckoSurfaceView::InitEditable(
    /* [in] */ ICharSequence* pContents)
{
    ECode ec = mEditableFactory->NewEditable(pContents, &mEditable);
    if (FAILED(ec)) return ec;

    Int32 length = 0;
    ec = pContents->GetLength(&length);
    if (FAILED(ec)) return ec;
    ec = mEditable->SetSpan(mInputConnection,
                        0, length, Spanned_SPAN_INCLUSIVE_INCLUSIVE);
    if (FAILED(ec)) return ec;
    //ec = Selection::SetSelection(mEditable, length);
    return ec;
}

ECode CGeckoSurfaceView::GetVerticalFadingEdgeLength(
    /* [out] */ Int32 * pLength)
{
    return mSurfaceView->GetVerticalFadingEdgeLength(pLength);
}

ECode CGeckoSurfaceView::SetFadingEdgeLength(
    /* [in] */ Int32 length)
{
    return mSurfaceView->SetFadingEdgeLength(length);
}

ECode CGeckoSurfaceView::GetHorizontalFadingEdgeLength(
    /* [out] */ Int32 * pLength)
{
    return mSurfaceView->GetHorizontalFadingEdgeLength(pLength);
}

ECode CGeckoSurfaceView::GetVerticalScrollbarWidth(
    /* [out] */ Int32 * pWidth)
{
    return mSurfaceView->GetVerticalScrollbarWidth(pWidth);
}

ECode CGeckoSurfaceView::GetHorizontalScrollbarHeight(
    /* [out] */ Int32 * pHeight)
{
    return mSurfaceView->GetHorizontalScrollbarHeight(pHeight);
}

ECode CGeckoSurfaceView::SetOnFocusChangeListener(
    /* [in] */ IViewOnFocusChangeListener * pL)
{
    return mSurfaceView->SetOnFocusChangeListener(pL);
}

ECode CGeckoSurfaceView::GetOnFocusChangeListener(
    /* [out] */ IViewOnFocusChangeListener ** ppL)
{
    return mSurfaceView->GetOnFocusChangeListener(ppL);
}

ECode CGeckoSurfaceView::SetOnClickListener(
    /* [in] */ IViewOnClickListener * pL)
{
    return mSurfaceView->SetOnClickListener(pL);
}

ECode CGeckoSurfaceView::SetOnLongClickListener(
    /* [in] */ IViewOnLongClickListener * pL)
{
    return mSurfaceView->SetOnLongClickListener(pL);
}

ECode CGeckoSurfaceView::SetOnCreateContextMenuListener(
    /* [in] */ IViewOnCreateContextMenuListener * pL)
{
    return mSurfaceView->SetOnCreateContextMenuListener(pL);
}

ECode CGeckoSurfaceView::PerformClick(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->PerformClick(pResult);
}

ECode CGeckoSurfaceView::PerformLongClick(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->PerformLongClick(pResult);
}

ECode CGeckoSurfaceView::ShowContextMenu(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->ShowContextMenu(pResult);
}

ECode CGeckoSurfaceView::SetOnKeyListener(
    /* [in] */ IViewOnKeyListener * pL)
{
    return mSurfaceView->SetOnKeyListener(pL);
}

ECode CGeckoSurfaceView::SetOnTouchListener(
    /* [in] */ IViewOnTouchListener * pL)
{
    return mSurfaceView->SetOnTouchListener(pL);
}

ECode CGeckoSurfaceView::RequestRectangleOnScreen(
    /* [in] */ IRect * pRectangle,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->RequestRectangleOnScreen(pRectangle, pResult);
}

ECode CGeckoSurfaceView::RequestRectangleOnScreenEx(
    /* [in] */ IRect * pRectangle,
    /* [in] */ Boolean immediate,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->RequestRectangleOnScreenEx(
                            pRectangle, immediate, pResult);
}

ECode CGeckoSurfaceView::ClearFocus()
{
    return mSurfaceView->ClearFocus();
}

ECode CGeckoSurfaceView::HasFocus(
    /* [out] */ Boolean * pHasFocus)
{
    return mSurfaceView->HasFocus(pHasFocus);
}

ECode CGeckoSurfaceView::HasFocusable(
    /* [out] */ Boolean * pHasFocusable)
{
    return mSurfaceView->HasFocusable(pHasFocusable);
}

ECode CGeckoSurfaceView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->DispatchPopulateAccessibilityEvent(
                                pEvent, pResult);
}

ECode CGeckoSurfaceView::GetContentDescription(
    /* [out, callee] */ ArrayOf<Char8> ** ppDescription)
{
    return mSurfaceView->GetContentDescription(ppDescription);
}

ECode CGeckoSurfaceView::SetContentDescription(
    /* [in] */ const ArrayOf<Char8> & contentDescription)
{
    return mSurfaceView->SetContentDescription(contentDescription);
}

ECode CGeckoSurfaceView::IsFocused(
    /* [out] */ Boolean * pFocused)
{
    return mSurfaceView->IsFocused(pFocused);
}

ECode CGeckoSurfaceView::FindFocus(
    /* [out] */ IView ** ppView)
{
    return mSurfaceView->FindFocus(ppView);
}

ECode CGeckoSurfaceView::SetScrollContainer(
    /* [in] */ Boolean isScrollContainer)
{
    return mSurfaceView->SetScrollContainer(isScrollContainer);
}

ECode CGeckoSurfaceView::GetDrawingCacheQuality(
    /* [out] */ Int32 * pQuality)
{
    return mSurfaceView->GetDrawingCacheQuality(pQuality);
}

ECode CGeckoSurfaceView::SetDrawingCacheQuality(
    /* [in] */ Int32 quality)
{
    return mSurfaceView->SetDrawingCacheQuality(quality);
}

ECode CGeckoSurfaceView::GetKeepScreenOn(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->GetKeepScreenOn(pResult);
}

ECode CGeckoSurfaceView::SetKeepScreenOn(
    /* [in] */ Boolean keepScreenOn)
{
    return mSurfaceView->SetKeepScreenOn(keepScreenOn);
}

ECode CGeckoSurfaceView::GetNextFocusLeftId(
    /* [out] */ Int32 * pNextFocusLeftId)
{
    return mSurfaceView->GetNextFocusLeftId(pNextFocusLeftId);
}

ECode CGeckoSurfaceView::SetNextFocusLeftId(
    /* [in] */ Int32 nextFocusLeftId)
{
    return mSurfaceView->SetNextFocusLeftId(nextFocusLeftId);
}

ECode CGeckoSurfaceView::GetNextFocusRightId(
    /* [out] */ Int32 * pNextFocusRightId)
{
    return mSurfaceView->GetNextFocusRightId(pNextFocusRightId);
}

ECode CGeckoSurfaceView::SetNextFocusRightId(
    /* [in] */ Int32 nextFocusRightId)
{
    return mSurfaceView->SetNextFocusRightId(nextFocusRightId);
}

ECode CGeckoSurfaceView::GetNextFocusUpId(
    /* [out] */ Int32 * pNextFocusUpId)
{
    return mSurfaceView->GetNextFocusUpId(pNextFocusUpId);
}

ECode CGeckoSurfaceView::SetNextFocusUpId(
    /* [in] */ Int32 nextFocusUpId)
{
    return mSurfaceView->SetNextFocusUpId(nextFocusUpId);
}

ECode CGeckoSurfaceView::GetNextFocusDownId(
    /* [out] */ Int32 * pNextFocusDownId)
{
    return mSurfaceView->GetNextFocusDownId(pNextFocusDownId);
}

ECode CGeckoSurfaceView::SetNextFocusDownId(
    /* [in] */ Int32 nextFocusDownId)
{
    return mSurfaceView->SetNextFocusDownId(nextFocusDownId);
}

ECode CGeckoSurfaceView::IsShown(
    /* [out] */ Boolean * pShown)
{
    return mSurfaceView->IsShown(pShown);
}

ECode CGeckoSurfaceView::IsFitsSystemWindowsFlagSet(
    /* [out] */ Boolean * pFlag)
{
    return mSurfaceView->IsFitsSystemWindowsFlagSet(pFlag);
}

ECode CGeckoSurfaceView::GetVisibility(
    /* [out] */ Int32 * pVisibility)
{
    return mSurfaceView->GetVisibility(pVisibility);
}

ECode CGeckoSurfaceView::SetVisibility(
    /* [in] */ Int32 visibility)
{
    return mSurfaceView->SetVisibility(visibility);
}

ECode CGeckoSurfaceView::IsEnabled(
    /* [out] */ Boolean * pEnabled)
{
    return mSurfaceView->IsEnabled(pEnabled);
}

ECode CGeckoSurfaceView::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return mSurfaceView->SetEnabled(enabled);
}

ECode CGeckoSurfaceView::SetFocusable(
    /* [in] */ Boolean focusable)
{
    return mSurfaceView->SetFocusable(focusable);
}

ECode CGeckoSurfaceView::SetFocusableInTouchMode(
    /* [in] */ Boolean focusableInTouchMode)
{
    return mSurfaceView->SetFocusableInTouchMode(focusableInTouchMode);
}

ECode CGeckoSurfaceView::SetSoundEffectsEnabled(
    /* [in] */ Boolean soundEffectsEnabled)
{
    return mSurfaceView->SetSoundEffectsEnabled(soundEffectsEnabled);
}

ECode CGeckoSurfaceView::IsSoundEffectsEnabled(
    /* [out] */ Boolean * pSoundEffectsEnabled)
{
    return mSurfaceView->IsSoundEffectsEnabled(pSoundEffectsEnabled);
}

ECode CGeckoSurfaceView::SetHapticFeedbackEnabled(
    /* [in] */ Boolean hapticFeedbackEnabled)
{
    return mSurfaceView->SetHapticFeedbackEnabled(hapticFeedbackEnabled);
}

ECode CGeckoSurfaceView::IsHapticFeedbackEnabled(
    /* [out] */ Boolean * pHapticFeedbackEnabled)
{
    return mSurfaceView->IsHapticFeedbackEnabled(pHapticFeedbackEnabled);
}

ECode CGeckoSurfaceView::SetWillNotDraw(
    /* [in] */ Boolean willNotDraw)
{
    return mSurfaceView->SetWillNotDraw(willNotDraw);
}

ECode CGeckoSurfaceView::WillNotDraw(
    /* [out] */ Boolean * pWillNotDraw)
{
    return mSurfaceView->WillNotDraw(pWillNotDraw);
}

ECode CGeckoSurfaceView::SetWillNotCacheDrawing(
    /* [in] */ Boolean willNotCacheDrawing)
{
    return mSurfaceView->SetWillNotCacheDrawing(willNotCacheDrawing);
}

ECode CGeckoSurfaceView::WillNotCacheDrawing(
    /* [out] */ Boolean * pCache)
{
    return mSurfaceView->WillNotCacheDrawing(pCache);
}

ECode CGeckoSurfaceView::IsClickable(
    /* [out] */ Boolean * pClickable)
{
    return mSurfaceView->IsClickable(pClickable);
}

ECode CGeckoSurfaceView::SetClickable(
    /* [in] */ Boolean clickable)
{
    return mSurfaceView->SetClickable(clickable);
}

ECode CGeckoSurfaceView::IsLongClickable(
    /* [out] */ Boolean * pLongClickable)
{
    return mSurfaceView->IsLongClickable(pLongClickable);
}

ECode CGeckoSurfaceView::SetLongClickable(
    /* [in] */ Boolean longClickable)
{
    return mSurfaceView->SetLongClickable(longClickable);
}

ECode CGeckoSurfaceView::SetPressed(
    /* [in] */ Boolean pressed)
{
    return mSurfaceView->SetPressed(pressed);
}

ECode CGeckoSurfaceView::IsPressed(
    /* [out] */ Boolean * pPressed)
{
    return mSurfaceView->IsPressed(pPressed);
}

ECode CGeckoSurfaceView::IsSaveEnabled(
    /* [out] */ Boolean * pEnabled)
{
    return mSurfaceView->IsSaveEnabled(pEnabled);
}

ECode CGeckoSurfaceView::SetSaveEnabled(
    /* [in] */ Boolean enabled)
{
    return mSurfaceView->SetSaveEnabled(enabled);
}

ECode CGeckoSurfaceView::GetFilterTouchesWhenObscured(
    /* [out] */ Boolean * pEnabled)
{
    return mSurfaceView->GetFilterTouchesWhenObscured(pEnabled);
}

ECode CGeckoSurfaceView::SetFilterTouchesWhenObscured(
    /* [in] */ Boolean enabled)
{
    return mSurfaceView->SetFilterTouchesWhenObscured(enabled);
}

ECode CGeckoSurfaceView::IsFocusable(
    /* [out] */ Boolean * pFocusable)
{
    return mSurfaceView->IsFocusable(pFocusable);
}

ECode CGeckoSurfaceView::IsFocusableInTouchMode(
    /* [out] */ Boolean * pFocusable)
{
    return mSurfaceView->IsFocusableInTouchMode(pFocusable);
}

ECode CGeckoSurfaceView::FocusSearch(
    /* [in] */ Int32 direction,
    /* [out] */ IView ** ppView)
{
    return mSurfaceView->FocusSearch(direction, ppView);
}

ECode CGeckoSurfaceView::DispatchUnhandledMove(
    /* [in] */ IView * pFocused,
    /* [in] */ Int32 direction,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->DispatchUnhandledMove(pFocused, direction, pResult);
}

ECode CGeckoSurfaceView::GetFocusables(
    /* [in] */ Int32 direction,
    /* [out] */ IObjectContainer ** ppViews)
{
    return mSurfaceView->GetFocusables(direction, ppViews);
}

ECode CGeckoSurfaceView::AddFocusables(
    /* [in] */ IObjectContainer * pViews,
    /* [in] */ Int32 direction)
{
    return mSurfaceView->AddFocusables(pViews, direction);
}

ECode CGeckoSurfaceView::AddFocusablesEx(
    /* [in] */ IObjectContainer * pViews,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 focusableMode)
{
    return mSurfaceView->AddFocusablesEx(pViews, direction, focusableMode);
}

ECode CGeckoSurfaceView::GetTouchables(
    /* [out] */ IObjectContainer ** ppViews)
{
    return mSurfaceView->GetTouchables(ppViews);
}

ECode CGeckoSurfaceView::AddTouchables(
    /* [in] */ IObjectContainer * pViews)
{
    return mSurfaceView->AddTouchables(pViews);
}

ECode CGeckoSurfaceView::RequestFocus(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->RequestFocus(pResult);
}

ECode CGeckoSurfaceView::RequestFocusEx(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->RequestFocusEx(direction, pResult);
}

ECode CGeckoSurfaceView::RequestFocusEx2(
    /* [in] */ Int32 direction,
    /* [in] */ IRect * pPreviouslyFocusedRect,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->RequestFocusEx2(
                    direction, pPreviouslyFocusedRect, pResult);
}

ECode CGeckoSurfaceView::RequestFocusFromTouch(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->RequestFocusFromTouch(pResult);
}

ECode CGeckoSurfaceView::DispatchStartTemporaryDetach()
{
    return mSurfaceView->DispatchStartTemporaryDetach();
}

ECode CGeckoSurfaceView::OnStartTemporaryDetach()
{
    return mSurfaceView->OnStartTemporaryDetach();
}

ECode CGeckoSurfaceView::DispatchFinishTemporaryDetach()
{
    return mSurfaceView->DispatchFinishTemporaryDetach();
}

ECode CGeckoSurfaceView::OnFinishTemporaryDetach()
{
    return mSurfaceView->OnFinishTemporaryDetach();
}

ECode CGeckoSurfaceView::GetKeyDispatcherState(
    /* [out] */ IDispatcherState ** ppState)
{
    return mSurfaceView->GetKeyDispatcherState(ppState);
}

ECode CGeckoSurfaceView::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->DispatchKeyEventPreIme(pEvent, pResult);
}

ECode CGeckoSurfaceView::DispatchKeyEvent(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->DispatchKeyEvent(pEvent, pResult);
}

ECode CGeckoSurfaceView::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->DispatchKeyShortcutEvent(pEvent, pResult);
}

ECode CGeckoSurfaceView::DispatchTouchEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->DispatchTouchEvent(pEvent, pResult);
}

ECode CGeckoSurfaceView::OnFilterTouchEventForSecurity(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->OnFilterTouchEventForSecurity(pEvent, pResult);
}

ECode CGeckoSurfaceView::DispatchTrackballEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->DispatchTrackballEvent(pEvent, pResult);
}

ECode CGeckoSurfaceView::DispatchWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    return mSurfaceView->DispatchWindowFocusChanged(hasFocus);
}

ECode CGeckoSurfaceView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    return mSurfaceView->OnWindowFocusChanged(hasWindowFocus);
}

ECode CGeckoSurfaceView::HasWindowFocus(
    /* [out] */ Boolean * pFocus)
{
    return mSurfaceView->HasWindowFocus(pFocus);
}

ECode CGeckoSurfaceView::DispatchDisplayHint(
    /* [in] */ Int32 hint)
{
    return mSurfaceView->DispatchDisplayHint(hint);
}

ECode CGeckoSurfaceView::DispatchWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    return mSurfaceView->DispatchWindowVisibilityChanged(visibility);
}

ECode CGeckoSurfaceView::GetWindowVisibility(
    /* [out] */ Int32 * pVisibility)
{
    return mSurfaceView->GetWindowVisibility(pVisibility);
}

ECode CGeckoSurfaceView::GetWindowVisibleDisplayFrame(
    /* [in] */ IRect * pOutRect)
{
    return mSurfaceView->GetWindowVisibleDisplayFrame(pOutRect);
}

ECode CGeckoSurfaceView::DispatchConfigurationChanged(
    /* [in] */ IConfiguration * pNewConfig)
{
    return mSurfaceView->DispatchConfigurationChanged(pNewConfig);
}

ECode CGeckoSurfaceView::IsInTouchMode(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->IsInTouchMode(pResult);
}

ECode CGeckoSurfaceView::GetContext(
    /* [out] */ IContext ** ppCtx)
{
    return mSurfaceView->GetContext(ppCtx);
}

ECode CGeckoSurfaceView::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    Boolean isSystem;
    ECode ec = pEvent->IsSystem(&isSystem);
    if (FAILED(ec)) return ec;
    if (isSystem) {
        return mSurfaceView->OnKeyPreIme(keyCode, pEvent, pResult);
    }

    Int32 action;
    ec = pEvent->GetAction(&action);
    if (FAILED(ec)) return ec;
    switch (action) {
        case KeyEvent_ACTION_DOWN:
            return OnKeyDown(keyCode, pEvent, pResult);
        case KeyEvent_ACTION_UP:
            return OnKeyUp(keyCode, pEvent, pResult);
        case KeyEvent_ACTION_MULTIPLE:
            Int32 repeatCount;
            ec = pEvent->GetRepeatCount(&repeatCount);
            if (FAILED(ec)) return ec;
            return OnKeyMultiple(keyCode, repeatCount, pEvent, pResult);
    }
    return mSurfaceView->OnKeyPreIme(keyCode, pEvent, pResult);
}

ECode CGeckoSurfaceView::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->OnKeyShortcut(keyCode, pEvent, pResult);
}

ECode CGeckoSurfaceView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* pEvent,
    /* [out] */ Boolean* pResult)
{
    ECode ec = NOERROR;
    Int32 flags = 0;
    Int32 repeatCount = 0;
    Boolean result = FALSE;
    pEvent->AddRef();
    pEvent->GetFlags(&flags);
    switch (keyCode) {
        case KeyEvent_KEYCODE_BACK:
            pEvent->GetRepeatCount(&repeatCount);
            if (repeatCount == 0) {
                pEvent->StartTracking();
                *pResult = TRUE;
            }
            else {
                *pResult = FALSE;
            }
            pEvent->Release();
            return NOERROR;
        case KeyEvent_KEYCODE_MENU:
            pEvent->GetRepeatCount(&repeatCount);
            if (repeatCount == 0) {
                pEvent->StartTracking();
                break;
            }
            else if ((flags & KeyEvent_FLAG_LONG_PRESS) != 0) {
                break;
            }
            // Ignore repeats for KEYCODE_MENU; they confuse the widget code.
            pEvent->Release();
            *pResult = FALSE;
            return NOERROR;
        case KeyEvent_KEYCODE_VOLUME_UP:
        case KeyEvent_KEYCODE_VOLUME_DOWN:
        case KeyEvent_KEYCODE_SEARCH:
            pEvent->Release();
            *pResult = FALSE;
            return NOERROR;
        case KeyEvent_KEYCODE_DEL:
            // See comments in GeckoInputConnection.onKeyDel
            if (!mInputConnection) break;
            mInputConnection->OnKeyDel(&result);
            if (result) {
                pEvent->Release();
                *pResult = TRUE;
                return NOERROR;
            }
            break;
        case KeyEvent_KEYCODE_ENTER:
            if ((flags & KeyEvent_FLAG_EDITOR_ACTION) != 0 &&
                mIMEActionHint.EqualsIgnoreCase("next")) {
                Int32 action = 0;
                pEvent->GetAction(&action);
                pEvent->Release();
                ec = CKeyEvent::New(action, KeyEvent_KEYCODE_TAB, &pEvent);
                if (FAILED(ec)) return ec;
            }
            break;
        default:
            break;
    }
    // KeyListener returns true if it handled the event for us.
    ec = mKeyListener->OnKeyDown(this, mEditable, keyCode, pEvent, &result);
    if (FAILED(ec)) {
        pEvent->Release();
        return ec;
    }
    if (mIMEState == IME_STATE_DISABLED ||
        keyCode == KeyEvent_KEYCODE_ENTER ||
        keyCode == KeyEvent_KEYCODE_DEL ||
        (flags & KeyEvent_FLAG_SOFT_KEYBOARD) != 0 ||
        !result) {
        GeckoEvent* pGeckoEvent = new GeckoEvent(pEvent);
        pEvent->Release();
        if (!pGeckoEvent) return E_OUT_OF_MEMORY;
        GeckoAppShell::SendEventToGecko(pGeckoEvent);
    }
    else pEvent->Release();
    *pResult = TRUE;
    return NOERROR;
}

ECode CGeckoSurfaceView::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* pEvent,
    /* [out] */ Boolean* pResult)
{
    ECode ec;
    GeckoEvent* pGeckoEvent;
    switch (keyCode) {
        case KeyEvent_KEYCODE_BACK:
            pGeckoEvent = new GeckoEvent(pEvent);
            if (!pGeckoEvent) return E_OUT_OF_MEMORY;
            GeckoAppShell::SendEventToGecko(pGeckoEvent);
            *pResult = TRUE;
            return NOERROR;
        case KeyEvent_KEYCODE_MENU:
            IContext* pIContext;
            ec = GetContext(&pIContext);
            if (FAILED(ec)) return ec;

            ILocalInputMethodManager* pImm;
            ec = pIContext->GetSystemService(
                            Context_INPUT_METHOD_SERVICE, (IInterface**)&pImm);
            pIContext->Release();
            if (FAILED(ec)) return ec;
            IBinder* pIBinder;
            ec = GetWindowToken(&pIBinder);
            if (FAILED(ec)) {
                pImm->Release();
                return ec;
            }
            ec = pImm->ToggleSoftInputFromWindow(pIBinder,
                          InputMethodManager_SHOW_FORCED, 0);
            pImm->Release();
            if (FAILED(ec)) return ec;
            *pResult = TRUE;
            return NOERROR;
        default:
            break;
    }
    *pResult = FALSE;
    return NOERROR;
}

ECode CGeckoSurfaceView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* pEvent,
    /* [out] */ Boolean* pResult)
{
    switch (keyCode) {
        case KeyEvent_KEYCODE_BACK:
            Boolean isTracking;
            Boolean isCanceled;
            pEvent->IsTracking(&isTracking);
            pEvent->IsCanceled(&isCanceled);
            if (!isTracking || isCanceled) {
                *pResult = FALSE;
                return NOERROR;
            }
            break;
        default:
            break;
    }
    Int32 flags = 0;
    pEvent->GetFlags(&flags);
    Boolean result;
    ECode ec = mKeyListener->OnKeyUp(this,
                            mEditable, keyCode, pEvent, &result);
    if (FAILED(ec)) return ec;

    if (mIMEState == IME_STATE_DISABLED ||
        keyCode == KeyEvent_KEYCODE_ENTER ||
        keyCode == KeyEvent_KEYCODE_DEL ||
        (flags & KeyEvent_FLAG_SOFT_KEYBOARD) != 0 ||
        !result) {
        GeckoEvent* pGeckoEvent = new GeckoEvent(pEvent);
        if (!pGeckoEvent) return E_OUT_OF_MEMORY;
        GeckoAppShell::SendEventToGecko(pGeckoEvent);
    }
    *pResult = TRUE;
    return NOERROR;
}

ECode CGeckoSurfaceView::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* pEvent,
    /* [out] */ Boolean* pResult)
{
    GeckoEvent* pGeckoEvent = new GeckoEvent(pEvent);
    if (!pGeckoEvent) return E_OUT_OF_MEMORY;
    GeckoAppShell::SendEventToGecko(pGeckoEvent);

    *pResult = TRUE;
    return NOERROR;
}

ECode CGeckoSurfaceView::OnCheckIsTextEditor(
    /* [out] */ Boolean * pResult)
{
    *pResult = FALSE;
    return NOERROR;
}

ECode CGeckoSurfaceView::OnCreateInputConnection(
    /* [in] */ IEditorInfo * pOutAttrs,
    /* [out] */ IInputConnection ** ppInputConnection)
{
    /*pOutAttrs->SetInputType(InputType_TYPE_CLASS_TEXT);
    pOutAttrs->SetImeOptions(EditorInfo_IME_ACTION_NONE);
    pOutAttrs->SetActionLabel(NULL);
    mKeyListener = TextKeyListener::GetInstance();

    if (mIMEState == IME_STATE_PASSWORD) {
        outAttrs.inputType |= InputType_TYPE_TEXT_VARIATION_PASSWORD;
    }
    else if (mIMETypeHint.EqualsIgnoreCase("url")) {
        outAttrs.inputType |= InputType_TYPE_TEXT_VARIATION_URI;
    }
    else if (mIMETypeHint.EqualsIgnoreCase("email")) {
        outAttrs.inputType |= InputType_TYPE_TEXT_VARIATION_EMAIL_ADDRESS;
    }
    else if (mIMETypeHint.EqualsIgnoreCase("search")) {
        outAttrs.imeOptions = EditorInfo_IME_ACTION_SEARCH;
    }
    else if (mIMETypeHint.EqualsIgnoreCase("tel")) {
        outAttrs.inputType = InputType_TYPE_CLASS_PHONE;
    }
    else if (mIMETypeHint.EqualsIgnoreCase("number") ||
             mIMETypeHint.EqualsIgnoreCase("range")) {
        outAttrs.inputType = InputType_TYPE_CLASS_NUMBER;
    }
    else if (mIMETypeHint.EqualsIgnoreCase("datetime") ||
             mIMETypeHint.EqualsIgnoreCase("datetime-local")) {
        outAttrs.inputType = InputType_TYPE_CLASS_DATETIME |
                             InputType_TYPE_DATETIME_VARIATION_NORMAL;
    }
    else if (mIMETypeHint.EqualsIgnoreCase("date")) {
        outAttrs.inputType = InputType_TYPE_CLASS_DATETIME |
                             InputType_TYPE_DATETIME_VARIATION_DATE;
    }
    else if (mIMETypeHint.EqualsIgnoreCase("time")) {
        outAttrs.inputType = InputType_TYPE_CLASS_DATETIME |
                             InputType_TYPE_DATETIME_VARIATION_TIME;
    }

    if (mIMEActionHint.EqualsIgnoreCase("go")) {
        outAttrs.imeOptions = EditorInfo_IME_ACTION_GO;
    }
    else if (mIMEActionHint.equalsIgnoreCase("done")) {
        outAttrs.imeOptions = EditorInfo_IME_ACTION_DONE;
    }
    else if (mIMEActionHint.equalsIgnoreCase("next")) {
        outAttrs.imeOptions = EditorInfo_IME_ACTION_NEXT;
    }
    else if (mIMEActionHint.equalsIgnoreCase("search")) {
        outAttrs.imeOptions = EditorInfo_IME_ACTION_SEARCH;
    }
    else if (mIMEActionHint.equalsIgnoreCase("send")) {
        outAttrs.imeOptions = EditorInfo_IME_ACTION_SEND;
    }
    else if (!mIMEActionHint.IsNull() && mIMEActionHint.GetLength() != 0) {
        outAttrs.actionLabel = mIMEActionHint;
    }

    if (mIMELandscapeFS == FALSE) {
        outAttrs.imeOptions |= EditorInfo_IME_FLAG_NO_EXTRACT_UI;
    }*/

    mInputConnection->Reset();
    *ppInputConnection = mInputConnection;
    return NOERROR;
}

ECode CGeckoSurfaceView::CheckInputConnectionProxy(
    /* [in] */ IView * pView,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->CheckInputConnectionProxy(pView, pResult);
}

ECode CGeckoSurfaceView::CreateContextMenu(
    /* [in] */ IContextMenu * pMenu)
{
    return mSurfaceView->CreateContextMenu(pMenu);
}

ECode CGeckoSurfaceView::OnTrackballEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->OnTrackballEvent(pEvent, pResult);
}

ECode CGeckoSurfaceView::OnTouchEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    GeckoEvent* pGeckoEvent = new GeckoEvent(pEvent);
    if (!pGeckoEvent) return E_OUT_OF_MEMORY;
    GeckoAppShell::SendEventToGecko(pGeckoEvent);
    *pResult = TRUE;
    return NOERROR;
}

ECode CGeckoSurfaceView::SetTouchDelegate(
    /* [in] */ ITouchDelegate * pTouchDelegate)
{
    return mSurfaceView->SetTouchDelegate(pTouchDelegate);
}

ECode CGeckoSurfaceView::GetTouchDelegate(
    /* [out] */ ITouchDelegate ** ppTouchDelegate)
{
    return mSurfaceView->GetTouchDelegate(ppTouchDelegate);
}

ECode CGeckoSurfaceView::CancelLongPress()
{
    return mSurfaceView->CancelLongPress();
}

ECode CGeckoSurfaceView::BringToFront()
{
    return mSurfaceView->BringToFront();
}

ECode CGeckoSurfaceView::GetParent(
    /* [out] */ IViewParent ** ppParent)
{
    return mSurfaceView->GetParent(ppParent);
}

ECode CGeckoSurfaceView::GetScrollX(
    /* [out] */ Int32 * pX)
{
    return mSurfaceView->GetScrollX(pX);
}

ECode CGeckoSurfaceView::GetScrollY(
    /* [out] */ Int32 * pY)
{
    return mSurfaceView->GetScrollY(pY);
}

ECode CGeckoSurfaceView::GetWidth(
    /* [out] */ Int32 * pWidth)
{
    return mSurfaceView->GetWidth(pWidth);
}

ECode CGeckoSurfaceView::GetHeight(
    /* [out] */ Int32 * pHeight)
{
    return mSurfaceView->GetHeight(pHeight);
}

ECode CGeckoSurfaceView::GetDrawingRect(
    /* [in] */ IRect * pOutRect)
{
    return mSurfaceView->GetDrawingRect(pOutRect);
}

ECode CGeckoSurfaceView::GetMeasuredWidth(
    /* [out] */ Int32 * pWidth)
{
    return mSurfaceView->GetMeasuredWidth(pWidth);
}

ECode CGeckoSurfaceView::GetMeasuredHeight(
    /* [out] */ Int32 * pHeight)
{
    return mSurfaceView->GetMeasuredHeight(pHeight);
}

ECode CGeckoSurfaceView::GetTop(
    /* [out] */ Int32 * pTop)
{
    return mSurfaceView->GetTop(pTop);
}

ECode CGeckoSurfaceView::GetBottom(
    /* [out] */ Int32 * pBottom)
{
    return mSurfaceView->GetBottom(pBottom);
}

ECode CGeckoSurfaceView::GetLeft(
    /* [out] */ Int32 * pLeft)
{
    return mSurfaceView->GetLeft(pLeft);
}

ECode CGeckoSurfaceView::GetRight(
    /* [out] */ Int32 * pRight)
{
    return mSurfaceView->GetRight(pRight);
}

ECode CGeckoSurfaceView::GetHitRect(
    /* [in] */ IRect * pOutRect)
{
    return mSurfaceView->GetHitRect(pOutRect);
}

ECode CGeckoSurfaceView::GetFocusedRect(
    /* [in] */ IRect * pOutRect)
{
    return mSurfaceView->GetFocusedRect(pOutRect);
}

ECode CGeckoSurfaceView::GetGlobalVisibleRect(
    /* [in] */ IRect * pOutRect,
    /* [in] */ IPoint * pOutGlobalOffset,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->GetGlobalVisibleRect(
                    pOutRect, pOutGlobalOffset, pResult);
}

ECode CGeckoSurfaceView::GetGlobalVisibleRectEx(
    /* [in] */ IRect * pOutRect,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->GetGlobalVisibleRectEx(pOutRect, pResult);
}

ECode CGeckoSurfaceView::GetLocalVisibleRect(
    /* [in] */ IRect * pOutRect,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->GetLocalVisibleRect(pOutRect, pResult);
}

ECode CGeckoSurfaceView::OffsetTopAndBottom(
    /* [in] */ Int32 offset)
{
    return mSurfaceView->OffsetTopAndBottom(offset);
}

ECode CGeckoSurfaceView::OffsetLeftAndRight(
    /* [in] */ Int32 offset)
{
    return mSurfaceView->OffsetLeftAndRight(offset);
}

ECode CGeckoSurfaceView::GetLayoutParams(
    /* [out] */ IViewGroupLayoutParams ** ppParams)
{
    return mSurfaceView->GetLayoutParams(ppParams);
}

ECode CGeckoSurfaceView::SetLayoutParams(
    /* [in] */ IViewGroupLayoutParams * pParams)
{
    return mSurfaceView->SetLayoutParams(pParams);
}

ECode CGeckoSurfaceView::ScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return mSurfaceView->ScrollTo(x, y);
}

ECode CGeckoSurfaceView::ScrollBy(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return mSurfaceView->ScrollBy(x, y);
}

ECode CGeckoSurfaceView::Invalidate(
    /* [in] */ IRect * pDirty)
{
    return mSurfaceView->Invalidate(pDirty);
}

ECode CGeckoSurfaceView::InvalidateEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return mSurfaceView->InvalidateEx(left, top, right, bottom);
}

ECode CGeckoSurfaceView::InvalidateEx2()
{
    return mSurfaceView->InvalidateEx2();
}

ECode CGeckoSurfaceView::IsOpaque(
    /* [out] */ Boolean * pOpaque)
{
    return mSurfaceView->IsOpaque(pOpaque);
}

ECode CGeckoSurfaceView::GetHandler(
    /* [out] */ IApartment ** ppApartment)
{
    return mSurfaceView->GetHandler(ppApartment);
}

ECode CGeckoSurfaceView::PostInvalidate()
{
    return mSurfaceView->PostInvalidate();
}

ECode CGeckoSurfaceView::PostInvalidateEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return mSurfaceView->PostInvalidateEx(left, top, right, bottom);
}

ECode CGeckoSurfaceView::PostInvalidateDelayed(
    /* [in] */ Int64 delayMilliseconds)
{
    return mSurfaceView->PostInvalidateDelayed(delayMilliseconds);
}

ECode CGeckoSurfaceView::PostInvalidateDelayedEx(
    /* [in] */ Int64 delayMilliseconds,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return mSurfaceView->PostInvalidateDelayedEx(
                delayMilliseconds, left, top, right, bottom);
}

ECode CGeckoSurfaceView::ComputeScroll()
{
    return mSurfaceView->ComputeScroll();
}

ECode CGeckoSurfaceView::IsHorizontalFadingEdgeEnabled(
    /* [out] */ Boolean * pHorizontalFadingEdgeEnabled)
{
    return mSurfaceView->IsHorizontalFadingEdgeEnabled(
                        pHorizontalFadingEdgeEnabled);
}

ECode CGeckoSurfaceView::SetHorizontalFadingEdgeEnabled(
    /* [in] */ Boolean horizontalFadingEdgeEnabled)
{
    return mSurfaceView->SetHorizontalFadingEdgeEnabled(
                        horizontalFadingEdgeEnabled);
}

ECode CGeckoSurfaceView::IsVerticalFadingEdgeEnabled(
    /* [out] */ Boolean * pVerticalFadingEdgeEnabled)
{
    return mSurfaceView->IsVerticalFadingEdgeEnabled(
                        pVerticalFadingEdgeEnabled);
}

ECode CGeckoSurfaceView::SetVerticalFadingEdgeEnabled(
    /* [in] */ Boolean verticalFadingEdgeEnabled)
{
    return mSurfaceView->SetVerticalFadingEdgeEnabled(
                        verticalFadingEdgeEnabled);
}

ECode CGeckoSurfaceView::IsHorizontalScrollBarEnabled(
    /* [out] */ Boolean * pHorizontalScrollBarEnabled)
{
    return mSurfaceView->IsHorizontalScrollBarEnabled(
                        pHorizontalScrollBarEnabled);
}

ECode CGeckoSurfaceView::SetHorizontalScrollBarEnabled(
    /* [in] */ Boolean horizontalScrollBarEnabled)
{
    return mSurfaceView->SetHorizontalScrollBarEnabled(
                        horizontalScrollBarEnabled);
}

ECode CGeckoSurfaceView::IsVerticalScrollBarEnabled(
    /* [out] */ Boolean * pVerticalScrollBarEnabled)
{
    return mSurfaceView->IsVerticalScrollBarEnabled(
                        pVerticalScrollBarEnabled);
}

ECode CGeckoSurfaceView::SetVerticalScrollBarEnabled(
    /* [in] */ Boolean verticalScrollBarEnabled)
{
    return mSurfaceView->SetVerticalScrollBarEnabled(
                        verticalScrollBarEnabled);
}

ECode CGeckoSurfaceView::SetScrollbarFadingEnabled(
    /* [in] */ Boolean fadeScrollbars)
{
    return mSurfaceView->SetScrollbarFadingEnabled(fadeScrollbars);
}

ECode CGeckoSurfaceView::IsScrollbarFadingEnabled(
    /* [out] */ Boolean * pFadeScrollbars)
{
    return mSurfaceView->IsScrollbarFadingEnabled(pFadeScrollbars);
}

ECode CGeckoSurfaceView::SetScrollBarStyle(
    /* [in] */ Int32 style)
{
    return mSurfaceView->SetScrollBarStyle(style);
}

ECode CGeckoSurfaceView::GetScrollBarStyle(
    /* [out] */ Int32 * pStyle)
{
    return mSurfaceView->GetScrollBarStyle(pStyle);
}

ECode CGeckoSurfaceView::AssignParent(
    /* [in] */ IViewParent * pParent)
{
    return mSurfaceView->AssignParent(pParent);
}

ECode CGeckoSurfaceView::GetWindowToken(
    /* [out] */ IBinder ** ppToken)
{
    return mSurfaceView->GetWindowToken(ppToken);
}

ECode CGeckoSurfaceView::GetApplicationWindowToken(
    /* [out] */ IBinder ** ppToken)
{
    return mSurfaceView->GetApplicationWindowToken(ppToken);
}

ECode CGeckoSurfaceView::SaveHierarchyState(
    /* [in] */ IObjectIntegerMap * pContainer)
{
    return mSurfaceView->SaveHierarchyState(pContainer);
}

ECode CGeckoSurfaceView::RestoreHierarchyState(
    /* [in] */ IObjectIntegerMap * pContainer)
{
    return mSurfaceView->RestoreHierarchyState(pContainer);
}

ECode CGeckoSurfaceView::GetDrawingTime(
    /* [out] */ Int64 * pTime)
{
    return mSurfaceView->GetDrawingTime(pTime);
}

ECode CGeckoSurfaceView::SetDuplicateParentStateEnabled(
    /* [in] */ Boolean enabled)
{
    return mSurfaceView->SetDuplicateParentStateEnabled(enabled);
}

ECode CGeckoSurfaceView::IsDuplicateParentStateEnabled(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->IsDuplicateParentStateEnabled(pResult);
}

ECode CGeckoSurfaceView::SetDrawingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    return mSurfaceView->SetDrawingCacheEnabled(enabled);
}

ECode CGeckoSurfaceView::IsDrawingCacheEnabled(
    /* [out] */ Boolean * pEnabled)
{
    return mSurfaceView->IsDrawingCacheEnabled(pEnabled);
}

ECode CGeckoSurfaceView::GetDrawingCache(
    /* [out] */ IBitmap ** ppBitmap)
{
    return mSurfaceView->GetDrawingCache(ppBitmap);
}

ECode CGeckoSurfaceView::GetDrawingCacheEx(
    /* [in] */ Boolean autoScale,
    /* [out] */ IBitmap ** ppBitmap)
{
    return mSurfaceView->GetDrawingCacheEx(autoScale, ppBitmap);
}

ECode CGeckoSurfaceView::DestroyDrawingCache()
{
    return mSurfaceView->DestroyDrawingCache();
}

ECode CGeckoSurfaceView::SetDrawingCacheBackgroundColor(
    /* [in] */ Int32 color)
{
    return mSurfaceView->SetDrawingCacheBackgroundColor(color);
}

ECode CGeckoSurfaceView::GetDrawingCacheBackgroundColor(
    /* [out] */ Int32 * pColor)
{
    return mSurfaceView->GetDrawingCacheBackgroundColor(pColor);
}

ECode CGeckoSurfaceView::BuildDrawingCache()
{
    return mSurfaceView->BuildDrawingCache();
}

ECode CGeckoSurfaceView::BuildDrawingCacheEx(
    /* [in] */ Boolean autoScale)
{
    return mSurfaceView->BuildDrawingCacheEx(autoScale);
}

ECode CGeckoSurfaceView::CreateSnapshot(
    /* [in] */ BitmapConfig quality,
    /* [in] */ Int32 backgroundColor,
    /* [in] */ Boolean skipChildren,
    /* [out] */ IBitmap ** ppBitmap)
{
    return mSurfaceView->CreateSnapshot(
        quality, backgroundColor, skipChildren, ppBitmap);
}

ECode CGeckoSurfaceView::IsInEditMode(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->IsInEditMode(pResult);
}

ECode CGeckoSurfaceView::Draw(
    /* [in] */ ICanvas * pCanvas)
{
    return mSurfaceView->Draw(pCanvas);
}

ECode CGeckoSurfaceView::GetSolidColor(
    /* [out] */ Int32 * pColor)
{
    return mSurfaceView->GetSolidColor(pColor);
}

ECode CGeckoSurfaceView::OnFinishInflate()
{
    return mSurfaceView->OnFinishInflate();
}

ECode CGeckoSurfaceView::IsLayoutRequested(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->IsLayoutRequested(pResult);
}

ECode CGeckoSurfaceView::Layout(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    return mSurfaceView->Layout(l, t, r, b);
}

ECode CGeckoSurfaceView::GetResources(
    /* [out] */ IResources ** ppResources)
{
    return mSurfaceView->GetResources(ppResources);
}

ECode CGeckoSurfaceView::UnscheduleDrawableEx(
    /* [in] */ IDrawable * pWho)
{
    return mSurfaceView->UnscheduleDrawableEx(pWho);
}

ECode CGeckoSurfaceView::RefreshDrawableState()
{
    return mSurfaceView->RefreshDrawableState();
}

ECode CGeckoSurfaceView::GetDrawableState(
    /* [out, callee] */ ArrayOf<Int32> ** ppState)
{
    return mSurfaceView->GetDrawableState(ppState);
}

ECode CGeckoSurfaceView::SetBackgroundColor(
    /* [in] */ Int32 color)
{
    return mSurfaceView->SetBackgroundColor(color);
}

ECode CGeckoSurfaceView::SetBackgroundResource(
    /* [in] */ Int32 resid)
{
    return mSurfaceView->SetBackgroundResource(resid);
}

ECode CGeckoSurfaceView::SetBackgroundDrawable(
    /* [in] */ IDrawable * pD)
{
    return mSurfaceView->SetBackgroundDrawable(pD);
}

ECode CGeckoSurfaceView::GetBackground(
    /* [out] */ IDrawable ** ppDrawable)
{
    return mSurfaceView->GetBackground(ppDrawable);
}

ECode CGeckoSurfaceView::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return mSurfaceView->SetPadding(left, top, right, bottom);
}

ECode CGeckoSurfaceView::GetPaddingTop(
    /* [out] */ Int32 * pTop)
{
    return mSurfaceView->GetPaddingTop(pTop);
}

ECode CGeckoSurfaceView::GetPaddingBottom(
    /* [out] */ Int32 * pBottom)
{
    return mSurfaceView->GetPaddingBottom(pBottom);
}

ECode CGeckoSurfaceView::GetPaddingLeft(
    /* [out] */ Int32 * pLeft)
{
    return mSurfaceView->GetPaddingLeft(pLeft);
}

ECode CGeckoSurfaceView::GetPaddingRight(
    /* [out] */ Int32 * pRight)
{
    return mSurfaceView->GetPaddingRight(pRight);
}

ECode CGeckoSurfaceView::SetSelected(
    /* [in] */ Boolean selected)
{
    return mSurfaceView->SetSelected(selected);
}

ECode CGeckoSurfaceView::IsSelected(
    /* [out] */ Boolean * pSelected)
{
    return mSurfaceView->IsSelected(pSelected);
}

ECode CGeckoSurfaceView::GetViewTreeObserver(
    /* [out] */ IViewTreeObserver ** ppObserver)
{
    return mSurfaceView->GetViewTreeObserver(ppObserver);
}

ECode CGeckoSurfaceView::GetRootView(
    /* [out] */ IView ** ppView)
{
    return mSurfaceView->GetRootView(ppView);
}

ECode CGeckoSurfaceView::GetLocationOnScreen(
    /* [out] */ Int32 * pX,
    /* [out] */ Int32 * pY)
{
    return mSurfaceView->GetLocationOnScreen(pX, pY);
}

ECode CGeckoSurfaceView::GetLocationInWindow(
    /* [out] */ Int32 * pX,
    /* [out] */ Int32 * pY)
{
    return mSurfaceView->GetLocationInWindow(pX, pY);
}

ECode CGeckoSurfaceView::FindViewById(
    /* [in] */ Int32 id,
    /* [out] */ IView ** ppView)
{
    return mSurfaceView->FindViewById(id, ppView);
}

ECode CGeckoSurfaceView::SetId(
    /* [in] */ Int32 id)
{
    return mSurfaceView->SetId(id);
}

ECode CGeckoSurfaceView::GetId(
    /* [out] */ Int32 * pId)
{
    return mSurfaceView->GetId(pId);
}

ECode CGeckoSurfaceView::IsRootNamespace(
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->IsRootNamespace(pResult);
}

ECode CGeckoSurfaceView::GetBaseline(
    /* [out] */ Int32 * pResult)
{
    return mSurfaceView->GetBaseline(pResult);
}

ECode CGeckoSurfaceView::RequestLayout()
{
    return mSurfaceView->RequestLayout();
}

ECode CGeckoSurfaceView::ForceLayout()
{
    return mSurfaceView->ForceLayout();
}

ECode CGeckoSurfaceView::Measure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    return mSurfaceView->Measure(widthMeasureSpec, heightMeasureSpec);
}

ECode CGeckoSurfaceView::SetMinimumHeight(
    /* [in] */ Int32 minHeight)
{
    return mSurfaceView->SetMinimumHeight(minHeight);
}

ECode CGeckoSurfaceView::SetMinimumWidth(
    /* [in] */ Int32 minWidth)
{
    return mSurfaceView->SetMinimumWidth(minWidth);
}

ECode CGeckoSurfaceView::GetAnimation(
    /* [out] */ IAnimation ** ppAnimation)
{
    return mSurfaceView->GetAnimation(ppAnimation);
}

ECode CGeckoSurfaceView::StartAnimation(
    /* [in] */ IAnimation * pAnimation)
{
    return mSurfaceView->StartAnimation(pAnimation);
}

ECode CGeckoSurfaceView::ClearAnimation()
{
    return mSurfaceView->ClearAnimation();
}

ECode CGeckoSurfaceView::SetAnimation(
    /* [in] */ IAnimation * pAnimation)
{
    return mSurfaceView->SetAnimation(pAnimation);
}

ECode CGeckoSurfaceView::GatherTransparentRegion(
    /* [in] */ IRegion * pRegion,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->GatherTransparentRegion(pRegion, pResult);
}

ECode CGeckoSurfaceView::PlaySoundEffect(
    /* [in] */ Int32 soundConstant)
{
    return mSurfaceView->PlaySoundEffect(soundConstant);
}

ECode CGeckoSurfaceView::PerformHapticFeedback(
    /* [in] */ Int32 feedbackConstant,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->PerformHapticFeedback(feedbackConstant, pResult);
}

ECode CGeckoSurfaceView::PerformHapticFeedbackEx(
    /* [in] */ Int32 feedbackConstant,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean * pResult)
{
    return mSurfaceView->PerformHapticFeedbackEx(
                        feedbackConstant, flags, pResult);
}

ECode CGeckoSurfaceView::OnCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    return mSurfaceView->OnCloseSystemDialogs(reason);
}

ECode CGeckoSurfaceView::ApplyDrawableToTransparentRegion(
    /* [in] */ IDrawable * pDr,
    /* [in] */ IRegion * pRegion)
{
    return mSurfaceView->ApplyDrawableToTransparentRegion(pDr, pRegion);
}

ECode CGeckoSurfaceView::GetOverScrollMode(
    /* [out] */ Int32 * pMode)
{
    return mSurfaceView->GetOverScrollMode(pMode);
}

ECode CGeckoSurfaceView::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    return mSurfaceView->SetOverScrollMode(overScrollMode);
}

ECode CGeckoSurfaceView::GetHolder(
    /* [out] */ ISurfaceHolder ** ppHolder)
{
    return mSurfaceView->GetHolder(ppHolder);
}

ECode CGeckoSurfaceView::SetZOrderMediaOverlay(
    /* [in] */ Boolean isMediaOverlay)
{
    return mSurfaceView->SetZOrderMediaOverlay(isMediaOverlay);
}

ECode CGeckoSurfaceView::SetZOrderOnTop(
    /* [in] */ Boolean onTop)
{
    return mSurfaceView->SetZOrderOnTop(onTop);
}

ECode CGeckoSurfaceView::SetWindowType(
    /* [in] */ Int32 type)
{
    return mSurfaceView->SetWindowType(type);
}

ECode CGeckoSurfaceView::IsFixedSize(
    /* [out] */ Boolean * pSize)
{
    return mSurfaceView->IsFixedSize(pSize);
}

