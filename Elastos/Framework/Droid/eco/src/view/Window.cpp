
#include "view/Window.h"
#include "view/CWindowManagerImpl.h"
#include "graphics/ElPixelFormat.h"
#include <StringBuffer.h>
#include <stdio.h>

using namespace Elastos::Core;

extern "C" const InterfaceID EIID_Window =
        {0xdddddddd,0xdddd,0xdddd,{0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd}};

const Int32 Window::ID_ANDROID_CONTENT;
const Int32 Window::DEFAULT_FEATURES;

Window::LocalWindowManager::LocalWindowManager(
    /* [in] */ Window* w,
    /* [in] */ IWindowManager* wm) :
    mWindow(w),
    mWindowManager(wm)
{
//    mDefaultDisplay = mContext.getResources().getDefaultDisplay(
//            mWindowManager.getDefaultDisplay());
}

PInterface Window::LocalWindowManager::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IWindowManager*)this;
    }
    else if (riid == EIID_IWindowManager) {
        return (IWindowManager*)this;
    }
    return NULL;
}

UInt32 Window::LocalWindowManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Window::LocalWindowManager::Release()
{
    return ElRefBase::Release();
}

ECode Window::LocalWindowManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Window::LocalWindowManager::AddViewEx5(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    // Let this throw an exception on a bad params.
    CWindowManagerLayoutParams* wp = (CWindowManagerLayoutParams*)params;
    AutoPtr<ICharSequence> curTitle;
    wp->GetTitle((ICharSequence**)&curTitle);
    if (wp->mType >= WindowManagerLayoutParams_FIRST_SUB_WINDOW &&
        wp->mType <= WindowManagerLayoutParams_LAST_SUB_WINDOW) {
        if (wp->mToken == NULL) {
            AutoPtr<IView> decor;
            mWindow->PeekDecorView((IView**)&decor);
            if (decor != NULL) {
                decor->GetWindowToken((IBinder**)&(wp->mToken));
            }
        }
        Int32 len;
        if (curTitle == NULL || ((curTitle->GetLength(&len), len) - 1) == 0) {
            StringBuffer title;
            if (wp->mType == WindowManagerLayoutParams_TYPE_APPLICATION_MEDIA) {
                title += "Media";
            } else if (wp->mType == WindowManagerLayoutParams_TYPE_APPLICATION_MEDIA_OVERLAY) {
                title += "MediaOvr";
            } else if (wp->mType == WindowManagerLayoutParams_TYPE_APPLICATION_PANEL) {
                title += "Panel";
            } else if (wp->mType == WindowManagerLayoutParams_TYPE_APPLICATION_SUB_PANEL) {
                title += "SubPanel";
            } else if (wp->mType == WindowManagerLayoutParams_TYPE_APPLICATION_ATTACHED_DIALOG) {
                title += "AtchDlg";
            } else {
                char* str = (char*)malloc(12);
                sprintf(str, "%d", wp->mType);
                title += str;
            }
            if (!mWindow->mAppName.IsNull()) {
                title = title + ":" + mWindow->mAppName;
            }
            AutoPtr<ICharSequence> tl;
            CStringWrapper::New(String(title), (ICharSequence**)&tl);
            wp->SetTitle(tl);
        }
    }
    else {
        if (wp->mToken == NULL) {
            wp->mToken = mWindow->mContainer == NULL ?
                    mWindow->mAppToken : mWindow->mContainer->mAppToken;
        }
        Int32 len;
        if (curTitle == NULL || ((curTitle->GetLength(&len), len) - 1) == 0
                && !mWindow->mAppName.IsNull()) {
            AutoPtr<ICharSequence> tl;
            CStringWrapper::New(mWindow->mAppName, (ICharSequence**)&tl);
            wp->SetTitle(tl);
        }
    }
    if (wp->mCapsuleName.IsNull()) {
        mWindow->mContext->GetCapsuleName(&(wp->mCapsuleName));
    }
    mWindowManager->AddViewEx5(view, params);
    return NOERROR;
}

ECode Window::LocalWindowManager::UpdateViewLayout(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return mWindowManager->UpdateViewLayout(view, params);
}

ECode Window::LocalWindowManager::RemoveView(
    /* [in] */ IView* view)
{
    return mWindowManager->RemoveView(view);
}

ECode Window::LocalWindowManager::RemoveViewImmediate(
    /* [in] */ IView* view)
{
    return mWindowManager->RemoveViewImmediate(view);
}

ECode Window::LocalWindowManager::GetDefaultDisplay(
    /* [out] */ IDisplay** display)
{
    VALIDATE_NOT_NULL(display);
    *display = mDefaultDisplay.Get();
    if (*display) {
        (*display)->AddRef();
    }

    return NOERROR;
}

Window::Window()
    : mIsActive(FALSE)
    , mHasChildren(FALSE)
    , mForcedWindowFlags(0)
    , mFeatures(DEFAULT_FEATURES)
    , mLocalFeatures(DEFAULT_FEATURES)
    , mHaveWindowFormat(FALSE)
    , mDefaultWindowFormat(ElPixelFormat::OPAQUE)
    , mHasSoftInputMode(FALSE)
{
    CWindowManagerLayoutParams::NewByFriend(
            (CWindowManagerLayoutParams**)&mWindowAttributes);
}

Window::Window(
    /* [in] */ IContext* context)
    : mContext(context)
    , mIsActive(FALSE)
    , mHasChildren(FALSE)
    , mForcedWindowFlags(0)
    , mFeatures(DEFAULT_FEATURES)
    , mLocalFeatures(DEFAULT_FEATURES)
    , mHaveWindowFormat(FALSE)
    , mDefaultWindowFormat(ElPixelFormat::OPAQUE)
    , mHasSoftInputMode(FALSE)
{
    CWindowManagerLayoutParams::NewByFriend(
            (CWindowManagerLayoutParams**)&mWindowAttributes);
}

/**
 * Return the Context this window policy is running in, for retrieving
 * resources and other information.
 *
 * @return Context The Context that was supplied to the constructor.
 */
ECode Window::GetContext(
    /* [out] */ IContext** context)
{
    *context = mContext.Get();
    if (*context != NULL) {
        (*context)->AddRef();
    }
    return NOERROR;
}

static Int32 R_Styleable_Window[] = {
    0x01010032, 0x01010054, 0x01010055, 0x01010056,
    0x01010057, 0x01010058, 0x01010059, 0x01010098,
    0x010100ae, 0x0101020d, 0x0101021e, 0x0101021f,
    0x01010222, 0x0101022b, 0x01010292
};

ECode Window::GetWindowStyle(
    /* [out] */ ITypedArray** attrs)
{
    Mutex& mSyncLock = GetSelfSyncLock();
    Mutex::Autolock lock(mSyncLock);

    if (mWindowStyle == NULL) {
        FAIL_RETURN(mContext->ObtainStyledAttributes(
                ArrayOf<Int32>(R_Styleable_Window, 15)/*com.android.internal.R.styleable.Window*/,
                (ITypedArray**)&mWindowStyle));
    }
    *attrs = mWindowStyle;
    (*attrs)->AddRef();
    return NOERROR;
}

ECode Window::SetContainer(
    /* [in] */ IWindow* container)
{
    mContainer = (Window*)container->Probe(EIID_Window);
    if (mContainer != NULL) {
        // Embedded screens never have a title.
        mFeatures |= 1<<Window_FEATURE_NO_TITLE;
        mLocalFeatures |= 1<<Window_FEATURE_NO_TITLE;
        mContainer->mHasChildren = TRUE;
    }
    return NOERROR;
}

ECode Window::GetContainer(
    /* [out] */ IWindow** container)
{
    *container = NULL;

    if (mContainer != NULL) {
        *container = (IWindow*)mContainer->Probe(EIID_IWindow);
        (*container)->AddRef();
    }
    return NOERROR;
}

ECode Window::HasChildren(
    /* [out] */  Boolean* hasChildren)
{
    *hasChildren = mHasChildren;
    return NOERROR;
}

/**
 * Set the window manager for use by this Window to, for example,
 * display panels.  This is <em>not</em> used for displaying the
 * Window itself -- that must be done by the client.
 *
 * @param wm The ViewManager for adding new windows.
 */
ECode Window::SetWindowManager(
    /* [in] */ IWindowManager* wm,
    /* [in] */ IBinder* appToken,
    /* [in] */ const String& appName)
{
    mAppToken = appToken;
    mAppName = appName;
    if (wm == NULL) {
        CWindowManagerImpl::AcquireSingleton((IWindowManager**)&wm);
    }
    else {
        wm->AddRef();
    }
    mWindowManager = new LocalWindowManager(this, wm);
    wm->Release();
    return NOERROR;
}

/**
 * Return the window manager allowing this Window to display its own
 * windows.
 *
 * @return WindowManager The ViewManager.
 */
ECode Window::GetWindowManager(
    /* [out] */ IWindowManager** wm)
{
    *wm = mWindowManager.Get();
    if (*wm != NULL) (*wm)->AddRef();
    return NOERROR;
}

/**
 * Set the Callback interface for this window, used to intercept key
 * events and other dynamic operations in the window.
 *
 * @param callback The desired Callback interface.
 */
ECode Window::SetCallback(
    /* [in] */ IWindowCallback* cb)
{
    mCallback = cb;
    return NOERROR;
}

/**
 * Return the current Callback interface for this window.
 */
ECode Window::GetCallback(
    /* [out] */ IWindowCallback** cb)
{
    *cb = mCallback;
    if (*cb) (*cb)->AddRef();
    return NOERROR;
}

ECode Window::SetLayout(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWindowAttributes->mWidth = width;
    mWindowAttributes->mHeight = height;
    if (mCallback != NULL) {
        mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
    }
    return NOERROR;
}

ECode Window::SetGravity(
    /* [in] */ Int32 gravity)
{
    mWindowAttributes->mGravity = gravity;
    if (mCallback != NULL) {
        mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
    }
    return NOERROR;
}

ECode Window::SetType(
    /* [in] */ Int32 type)
{
    mWindowAttributes->mType = type;
    if (mCallback != NULL) {
        mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
    }
    return NOERROR;
}

ECode Window::SetFormat(
    /* [in] */ Int32 format)
{
    if (format != ElPixelFormat::UNKNOWN) {
        mWindowAttributes->mFormat = format;
        mHaveWindowFormat = TRUE;
    } else {
        mWindowAttributes->mFormat = mDefaultWindowFormat;
        mHaveWindowFormat = FALSE;
    }
    if (mCallback != NULL) {
        mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
    }
    return NOERROR;
}

ECode Window::SetWindowAnimations(
    /* [in] */ Int32 resId)
{
    mWindowAttributes->mWindowAnimations = resId;
    if (mCallback != NULL) {
        mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
    }
    return NOERROR;
}

ECode Window::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    if (mode != WindowManagerLayoutParams_SOFT_INPUT_STATE_UNSPECIFIED) {
        mWindowAttributes->mSoftInputMode = mode;
        mHasSoftInputMode = TRUE;
    } else {
        mHasSoftInputMode = FALSE;
    }
    if (mCallback != NULL) {
        mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
    }
    return NOERROR;
}

ECode Window::AddFlags(
    /* [in] */ Int32 flags)
{
    return SetFlags(flags, flags);
}

ECode Window::ClearFlags(
    /* [in] */ Int32 flags)
{
    return SetFlags(0, flags);
}

ECode Window::SetFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    mWindowAttributes->mFlags = (mWindowAttributes->mFlags & ~mask) | (flags & mask);
    mForcedWindowFlags |= mask;
    if (mCallback != NULL) {
        mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
    }
    return NOERROR;
}

ECode Window::SetAttributes(
    /* [in] */ IWindowManagerLayoutParams* a)
{
    mWindowAttributes->CopyFrom(a);
    if (mCallback != NULL) {
        mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
    }
    return NOERROR;
}

/**
 * Retrieve the current window attributes associated with this panel.
 *
 * @return WindowManager.LayoutParams Either the existing window
 *         attributes object, or a freshly created one if there is none.
 */
ECode Window::GetAttributes(
    /* [out] */ IWindowManagerLayoutParams** params)
{
    *params = (IWindowManagerLayoutParams*)mWindowAttributes.Get();
    if (*params) (*params)->AddRef();
    return NOERROR;
}

Int32 Window::GetForcedWindowFlags()
{
    return mForcedWindowFlags;
}

Boolean Window::HasSoftInputMode()
{
    return mHasSoftInputMode;
}

ECode Window::RequestFeature(
    /* [in] */ Int32 featureId,
    /* [out] */ Boolean* result)
{
    Int32 flag = 1<<featureId;
    mFeatures |= flag;
    mLocalFeatures |= mContainer != NULL ? (flag & ~mContainer->mFeatures) : flag;
    *result = (mFeatures & flag) != 0;
    return NOERROR;
}

ECode Window::MakeActive()
{
    if (mContainer != NULL) {
        if (mContainer->mActiveChild != NULL) {
            mContainer->mActiveChild->mIsActive = FALSE;
        }
        mContainer->mActiveChild = this;
    }
    mIsActive = TRUE;
    OnActive();
    return NOERROR;
}

ECode Window::IsActive(
    /* [out] */ Boolean* isActive)
{
    *isActive = mIsActive;
    return NOERROR;
}

/**
 * Finds a view that was identified by the id attribute from the XML that
 * was processed in {@link android.app.Activity#onCreate}.  This will
 * implicitly call {@link #getDecorView} for you, with all of the
 * associated side-effects.
 *
 * @return The view if found or null otherwise.
 */
ECode Window::FindViewById(
    /* [in] */ Int32 id,
    /* [out] */ IView** view)
{
    AutoPtr<IView> decor;
    GetDecorView((IView**)&decor);
    return decor->FindViewById(id, view);
}

ECode Window::SetBackgroundDrawableResource(
    /* [in] */ Int32 resid)
{
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoPtr<IDrawable> drawable;
    FAIL_RETURN(res->GetDrawable(resid, (IDrawable**)&drawable));
    return SetBackgroundDrawable(drawable);
}

Int32 Window::GetFeatures()
{
    return mFeatures;
}

Int32 Window::GetLocalFeatures()
{
    return mLocalFeatures;
}

void Window::SetDefaultWindowFormat(
    /* [in] */ Int32 format)
{
    mDefaultWindowFormat = format;
    if (!mHaveWindowFormat) {
        mWindowAttributes->mFormat = format;
        if (mCallback != NULL) {
            mCallback->OnWindowAttributesChanged(mWindowAttributes.Get());
        }
    }
}
