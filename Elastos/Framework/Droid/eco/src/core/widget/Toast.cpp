
#include "widget/Toast.h"
#include "view/CGravity.h"
#include "app/NotificationManager.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/CWindowManagerImpl.h"
#include "os/CApartment.h"
#include "os/ServiceManager.h"
#include "graphics/ElPixelFormat.h"
#include "stdio.h"

Toast::TN::ShowAction::ShowAction(
    /* [in] */ TN* host)
    : mHost(host)
{}

ECode Toast::TN::ShowAction::Run()
{
    mHost->HandleShow();

    return NOERROR;
}

Toast::TN::HideAction::HideAction(
    /* [in] */ TN* host)
    : mHost(host)
{}

ECode Toast::TN::HideAction::Run()
{
    mHost->HandleHide();

    return NOERROR;
}

Toast::TN::TN(
    /* [in] */ Toast* host)
    : mHost(host)
    , mWM(NULL)

{
    /*
    // XXX This should be changed to use a Dialog, with a Theme.Toast
    // defined that sets up the layout params appropriately.
    final WindowManager.LayoutParams params = mParams;
    params.height = WindowManager.LayoutParams.WRAP_CONTENT;
    params.width = WindowManager.LayoutParams.WRAP_CONTENT;
    params.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
            | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
            | WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON;
    params.format = PixelFormat.TRANSLUCENT;
    params.windowAnimations = com.android.internal.R.style.Animation_Toast;
    params.type = WindowManager.LayoutParams.TYPE_TOAST;
    params.setTitle("Toast");    
    */
    mShow = (IRunnable*)new ShowAction(this);
    mHide = (IRunnable*)new HideAction(this);

    CWindowManagerLayoutParams::New(
            ViewGroupLayoutParams_WRAP_CONTENT,
            ViewGroupLayoutParams_WRAP_CONTENT,
            WindowManagerLayoutParams_TYPE_TOAST,
            WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE
            | WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE
            | WindowManagerLayoutParams_FLAG_KEEP_SCREEN_ON,
            ElPixelFormat::TRANSLUCENT,
            (IWindowManagerLayoutParams**)&mParams);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("Toast"), (ICharSequence**)&cs);
    mParams->SetTitle(cs);

    //public static final int Animation_Toast=0x01030004;
    ((CWindowManagerLayoutParams*)mParams.Get())->mWindowAnimations = 0x01030004;
}

Toast::TN::~TN()
{
    if(mWM != NULL) {
        mWM->Release();
    }
}

PInterface Toast::TN::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (ITransientNotification*)this;
    }

    if (riid == EIID_ITransientNotification) {
        return (ITransientNotification*)this;
    }

    return NULL;
}

UInt32 Toast::TN::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Toast::TN::Release()
{
    return ElRefBase::Release();
}

ECode Toast::TN::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;
    if (pObject == (IInterface*)(ITransientNotification*)this) {
        *pIID = EIID_ITransientNotification;
    }

    return NOERROR;
}

ECode Toast::TN::Show()
{
    //if (localLOGV) Log.v(TAG, "SHOW: " + this);
    // mHandler.post(mShow);
    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;

    mHost->mApartment->PostCppCallback(
        (Handle32)mShow.Get(), *(Handle32*)&pHandlerFunc, NULL, 0);
    return NOERROR;
}

ECode Toast::TN::Hide()
{
    //if (localLOGV) Log.v(TAG, "HIDE: " + this);
    //mHandler.post(mHide);
    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;

    mHost->mApartment->PostCppCallback(
        (Handle32)mHide.Get(), *(Handle32*)&pHandlerFunc, NULL, 0);
    return NOERROR;
}

ECode Toast::TN::HandleShow()
{
    /*if (localLOGV) Log.v(TAG, "HANDLE SHOW: " + this + " mView=" + mView
                    + " mNextView=" + mNextView);
    if (mView != mNextView) {
        // remove the old view if necessary
        handleHide();
        mView = mNextView;
        mWM = WindowManagerImpl.getDefault();
        final int gravity = mGravity;
        mParams.gravity = gravity;
        if ((gravity & Gravity.HORIZONTAL_GRAVITY_MASK) == Gravity.FILL_HORIZONTAL) {
            mParams.horizontalWeight = 1.0f;
        }
        if ((gravity & Gravity.VERTICAL_GRAVITY_MASK) == Gravity.FILL_VERTICAL) {
            mParams.verticalWeight = 1.0f;
        }
        mParams.x = mX;
        mParams.y = mY;
        mParams.verticalMargin = mVerticalMargin;
        mParams.horizontalMargin = mHorizontalMargin;
        if (mView.getParent() != null) {
            if (localLOGV) Log.v(
                    TAG, "REMOVE! " + mView + " in " + this);
            mWM.removeView(mView);
        }
        if (localLOGV) Log.v(TAG, "ADD! " + mView + " in " + this);
        mWM.addView(mView, mParams);
    }*/
    if (mHost->mView != mHost->mNextView) {
        HandleHide();
        mHost->mView = mHost->mNextView;
        if (mWM == NULL) {
            CWindowManagerImpl::AcquireSingleton((ILocalWindowManager**)&mWM);
        }
        else {
            mWM->AddRef();
        }
        
        Int32 gravity = mHost->mGravity;
        ((CWindowManagerLayoutParams*)mParams.Get())->mGravity = gravity;

        if ((gravity & Gravity_HORIZONTAL_GRAVITY_MASK) == Gravity_FILL_HORIZONTAL) {
            ((CWindowManagerLayoutParams*)mParams.Get())->mHorizontalWeight = 1.0f;
        }
        if ((gravity & Gravity_VERTICAL_GRAVITY_MASK) == Gravity_FILL_VERTICAL) {
            ((CWindowManagerLayoutParams*)mParams.Get())->mVerticalWeight = 1.0f;
        }
        ((CWindowManagerLayoutParams*)mParams.Get())->mX = mHost->mX;
        ((CWindowManagerLayoutParams*)mParams.Get())->mY = mHost->mY;
        ((CWindowManagerLayoutParams*)mParams.Get())->mVerticalMargin = mHost->mVerticalMargin;
        ((CWindowManagerLayoutParams*)mParams.Get())->mHorizontalMargin = mHost->mHorizontalMargin;

        AutoPtr<IViewParent> parent;
        mHost->mView->GetParent((IViewParent**)&parent);
        if (parent != NULL) {
            //if (localLOGV) Log.v(
            //        TAG, "REMOVE! " + mView + " in " + this);
            mWM->RemoveView(mHost->mView);
        }
        //if (localLOGV) Log.v(TAG, "ADD! " + mView + " in " + this);
        mWM->AddViewEx5(mHost->mView, mParams);
    }    
    return NOERROR;
}

ECode Toast::TN::HandleHide()
{
    /*if (localLOGV) Log.v(TAG, "HANDLE HIDE: " + this + " mView=" + mView);
    if (mView != null) {
        // note: checking parent() just to make sure the view has
        // been added...  i have seen cases where we get here when
        // the view isn't yet added, so let's try not to crash.
        if (mView.getParent() != null) {
            if (localLOGV) Log.v(
                    TAG, "REMOVE! " + mView + " in " + this);
            mWM.removeView(mView);
        }

        mView = null;
    }*/

    if (mHost->mView != NULL) {
        // note: checking parent() just to make sure the view has
        // been added...  i have seen cases where we get here when
        // the view isn't yet added, so let's try not to crash.
        AutoPtr<IViewParent> parent;
        mHost->mView->GetParent((IViewParent**)&parent);
        if (parent != NULL) {
            //if (localLOGV) Log.v(
            //        TAG, "REMOVE! " + mView + " in " + this);
            mWM->RemoveView(mHost->mView);
        }

        mHost->mView = NULL;
    }

    return NOERROR;
}

Toast::Toast()
    : mDuration(0)
    , mGravity(Gravity_CENTER_HORIZONTAL | Gravity_BOTTOM)
    , mX(0)
    , mY(0)
    , mHorizontalMargin(0.0f)
    , mVerticalMargin(0.0f)
{}

/**
 * Construct an empty Toast object.  You must call {@link #setView} before you
 * can call {@link #show}.
 *
 * @param context  The context to use.  Usually your {@link android.app.Application}
 *                 or {@link android.app.Activity} object.
 */
Toast::Toast(
    /* [in] */ IContext* context)
{
     Init(context);
}

/**
 * Show the view for the specified duration.
 */
ECode Toast::Show()
{
    /*if (mNextView == null) {
        throw new RuntimeException("setView must have been called");
    }

    INotificationManager service = getService();

    String pkg = mContext.getPackageName();

    TN tn = mTN;

    try {
        service.enqueueToast(pkg, tn, mDuration);
    } catch (RemoteException e) {
        // Empty
    }*/

    if(mNextView == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<INotificationManager> service = NotificationManager::GetService();

    String cap;
    mContext->GetCapsuleName(&cap);
    return service->EnqueueToast(cap, (ITransientNotification*)mTN, mDuration);
}

/**
 * Close the view if it's showing, or don't show it if it isn't showing yet.
 * You do not normally have to call this.  Normally view will disappear on its own
 * after the appropriate duration.
 */
ECode Toast::Cancel()
{
    //mTN.hide();
    return mTN->Hide();
}

/**
 * Set the view to show.
 * @see #getView
 */
ECode Toast::SetView(
    /* [in] */ IView* view)
{
    mNextView = view;
    return NOERROR;
}

/**
 * Return the view.
 * @see #setView
 */
ECode Toast::GetView(
    /* [out] */ IView** view)
{
    //return mNextView;
    assert(view != NULL);
    *view = mNextView;
    return NOERROR;
}

/**
 * Set how long to show the view for.
 * @see #LENGTH_SHORT
 * @see #LENGTH_LONG
 */
ECode Toast::SetDuration(
    /* [in] */ Int32 duration)
{
    mDuration = duration;
    return NOERROR;
}

/**
 * Return the duration.
 * @see #setDuration
 */
ECode Toast::GetDuration(
    /* [out] */ Int32* duration)
{
    //return mDuration;
    assert(duration != NULL);
    *duration = mDuration;
    return NOERROR;
}

/**
 * Set the margins of the view.
 *
 * @param horizontalMargin The horizontal margin, in percentage of the
 *        container width, between the container's edges and the
 *        notification
 * @param verticalMargin The vertical margin, in percentage of the
 *        container height, between the container's edges and the
 *        notification
 */
ECode Toast::SetMargin(
    /* [in] */ Float horizontalMargin,
    /* [in] */ Float verticalMargin)
{
    mHorizontalMargin = horizontalMargin;
    mVerticalMargin = verticalMargin;
    return NOERROR;
}

/**
 * Return the horizontal margin.
 */    
ECode Toast::GetHorizontalMargin(
    /* [out] */ Float* horizontalMargin)
{
    //return mHorizontalMargin;
    assert(horizontalMargin != NULL);
    *horizontalMargin = mHorizontalMargin;
    return NOERROR;
}

/**
 * Return the vertical margin.
 */
ECode Toast::GetVerticalMargin(
    /* [out] */ Float* verticalMargin)
{
    //return mVerticalMargin;
    assert(verticalMargin != NULL);
    *verticalMargin = mVerticalMargin;
    return NOERROR;
}

/**
 * Set the location at which the notification should appear on the screen.
 * @see android.view.Gravity
 * @see #getGravity
 */
ECode Toast::SetGravity(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 xOffset, 
    /* [in] */ Int32 yOffset)
{
    mGravity = gravity;
    mX = xOffset;
    mY = yOffset;
    return NOERROR;
}

/**
 * Get the location at which the notification should appear on the screen.
 * @see android.view.Gravity
 * @see #getGravity
 */
ECode Toast::GetGravity(
    /* [out] */ Int32* gravity)
{
    //return mGravity;
    assert(gravity != NULL);
    *gravity = mGravity;
    return NOERROR;
}

/**
 * Return the X offset in pixels to apply to the gravity's location.
 */
ECode Toast::GetXOffset(
    /* [out] */ Int32* xOffset)
{
    //return mX;
    assert(xOffset != NULL);
    *xOffset = mX;
    return NOERROR;
}

/**
 * Return the Y offset in pixels to apply to the gravity's location.
 */
ECode Toast::GetYOffset(
    /* [out] */ Int32* yOffset)
{
    //return mY;
    assert(yOffset != NULL);
    *yOffset = mY;
    return NOERROR;
}

/**
 * Update the text in a Toast that was previously created using one of the makeText() methods.
 * @param resId The new text for the Toast.
 */
ECode Toast::SetText(
    /* [in] */ Int32 resId)
{
    //setText(mContext.getText(resId));

    AutoPtr<ICharSequence> text;
    mContext->GetText(resId, (ICharSequence**)&text);
    return SetTextEx(text);
}

/**
 * Update the text in a Toast that was previously created using one of the makeText() methods.
 * @param s The new text for the Toast.
 */
ECode Toast::SetTextEx(
    /* [in] */ ICharSequence* s)
{
    /*if (mNextView == null) {
        throw new RuntimeException("This Toast was not created with Toast.makeText()");
    }
    TextView tv = (TextView) mNextView.findViewById(com.android.internal.R.id.message);
    if (tv == null) {
        throw new RuntimeException("This Toast was not created with Toast.makeText()");
    }
    tv.setText(s);*/
    if (mNextView == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    //TextView tv = (TextView) mNextView.findViewById(com.android.internal.R.id.message);
    //public static final int message=0x0102000b;
    AutoPtr<ITextView> tv;
    mNextView->FindViewById(0x0102000b, (IView**)&tv);

    if (tv == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return tv->SetText(s);
}

ECode Toast::Init(
    /* [in] */ IContext* context)
{
    /*mContext = context;
    mTN = new TN();
    mY = context.getResources().getDimensionPixelSize(
         com.android.internal.R.dimen.toast_y_offset);*/

    mContext = context ;
    mTN = new Toast::TN(this);
    AutoPtr<IResources> res;
    context->GetResources((IResources**) &res);
    //public static final int toast_y_offset=0x01050003;
    res->GetDimensionPixelSize(0x01050003, &mY);

    assert(SUCCEEDED(CApartment::GetDefaultApartment((IApartment**)&mApartment))
        && (mApartment != NULL));

    return NOERROR;
}