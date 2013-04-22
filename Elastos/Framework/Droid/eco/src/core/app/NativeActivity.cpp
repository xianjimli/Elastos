
#include "app/NativeActivity.h"
#include "view/CInputMethodCallback.h"
#include "graphics/ElPixelFormat.h"
#include "os/Environment.h"
#include "os/Build.h"


PInterface NativeActivity::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(INativeActivity*)this;
    }
    else if (riid == EIID_ISurfaceHolderCallback2) {
        return (PInterface)(ISurfaceHolderCallback2*)this;
    }
    else if (riid == EIID_IInputQueueCallback) {
        return (PInterface)(IInputQueueCallback*)this;
    }
    else if (riid == EIID_IOnGlobalLayoutListener) {
        return (PInterface)(IOnGlobalLayoutListener*)this;
    }

    return NULL;
}

ECode NativeActivity::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(INativeActivity*)this) {
        *pIID = EIID_INativeActivity;
    }
    else if (pObject == (IInterface*)(ISurfaceHolderCallback2*)this) {
        *pIID = EIID_ISurfaceHolderCallback2;
    }
    else if (pObject == (IInterface*)(IInputQueueCallback*)this) {
        *pIID = EIID_IInputQueueCallback;
    }
    else if (pObject == (IInterface*)(IOnGlobalLayoutListener*)this) {
        *pIID = EIID_IOnGlobalLayoutListener;
    }

    return NOERROR;
}

UInt32 NativeActivity::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NativeActivity::Release()
{
    return ElRefBase::Release();
}

Int32 NativeActivity::LoadNativeCode(
    /* [in] */ const String& path,
    /* [in] */ const String& funcname,
    /* [in] */ NativeMessageQueue* queue,
    /* [in] */ const String& internalDataPath,
    /* [in] */ const String& externalDataPath,
    /* [in] */ Int32 sdkVersion,
    /* [in] */ IAssetManager* assetMgr,
    /* [in] */ ArrayOf<Byte>* savedState)
{
    return 0;
}

void NativeActivity::UnloadNativeCode(
    /* [in] */ Int32 handle)
{}

void NativeActivity::OnStartNative(
    /* [in] */ Int32 handle)
{}

void NativeActivity::OnResumeNative(
    /* [in] */ Int32 handle)
{}

ArrayOf<Byte>* OnSaveInstanceStateNative(
    /* [in] */ Int32 handle)
{
    return NULL;
}

void NativeActivity::OnPauseNative(
    /* [in] */ Int32 handle)
{}

void NativeActivity::OnStopNative(
    /* [in] */ Int32 handle)
{}

void NativeActivity::OnConfigurationChangedNative(
    /* [in] */ Int32 handle)
{}

void NativeActivity::OnLowMemoryNative(
    /* [in] */ Int32 handle)
{}

void NativeActivity::OnWindowFocusChangedNative(
    /* [in] */ Int32 handle,
    /* [in] */ Boolean focused)
{}

void NativeActivity::OnSurfaceCreatedNative(
    /* [in] */ Int32 handle,
    /* [in] */ ISurface* surface)
{}

void NativeActivity::OnSurfaceChangedNative(
    /* [in] */ Int32 handle,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{}

void NativeActivity::OnSurfaceRedrawNeededNative(
    /* [in] */ Int32 handle,
    /* [in] */ ISurface* surface)
{}

void NativeActivity::OnSurfaceDestroyedNative(
    /* [in] */ Int32 handle)
{}

void NativeActivity::OnInputChannelCreatedNative(
        /* [in] */ Int32 handle,
        /* [in] */ IInputChannel* channel)
{}

void NativeActivity::OnInputChannelDestroyedNative(
    /* [in] */ Int32 handle,
    /* [in] */ IInputChannel* channel)
{}

void NativeActivity::OnContentRectChangedNative(
    /* [in] */ Int32 handle,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{}

void NativeActivity::DispatchKeyEventNative(
    /* [in] */ Int32 handle,
    /* [in] */ IKeyEvent* event)
{}

void NativeActivity::FinishPreDispatchKeyEventNative(
    /* [in] */ Int32 handle,
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{}

NativeActivity::NativeContentView::NativeContentView(
    /* [in] */ IContext* context)
    : View(context)
{}

NativeActivity::NativeContentView::NativeContentView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : View(context, attrs)
{}

NativeActivity::NativeContentView::~NativeContentView()
{}

PInterface NativeActivity::NativeContentView::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IView) {
        return (PInterface)(IView*)this;
    }

    return NULL;
}

NativeActivity::InputMethodCallback::InputMethodCallback(
    /* [in] */ INativeActivity* na)
{
//    mNa = new WeakReference<NativeActivity>(na);
}

NativeActivity::InputMethodCallback::~InputMethodCallback()
{}

PInterface NativeActivity::InputMethodCallback::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IInputMethodCallback*)this;
    }

    return NULL;
}

NativeActivity::InputMethodCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IInputMethodCallback*)this) {
        *pIID = EIID_IInputMethodCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

UInt32 NativeActivity::InputMethodCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NativeActivity::InputMethodCallback::Release()
{
    return ElRefBase::Release();
}

ECode NativeActivity::InputMethodCallback::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    NativeActivity* na;// = mNa.get();
    if (na != NULL) {
            na->FinishPreDispatchKeyEventNative(na->mNativeHandle, seq, handled);
    }
    return NOERROR;
}

ECode NativeActivity::InputMethodCallback::SessionCreated(
    /* [in] */ IInputMethodSession* session)
{
    // Stub -- not for use in the client.
    return NOERROR;
}

ECode NativeActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    String libname = String("main");
    String funcname = String("ANativeActivity_onCreate");
    AutoPtr<IActivityInfo> ai;
    
    GetSystemService(Context_INPUT_METHOD_SERVICE, (IInterface**)&mIMM);
    mInputMethodCallback = new InputMethodCallback(this);

    GetWindow()->TakeSurface(this);
    GetWindow()->TakeInputQueue(this);
    GetWindow()->SetFormat(ElPixelFormat::RGB_565);
    GetWindow()->SetSoftInputMode(
            WindowManagerLayoutParams_SOFT_INPUT_STATE_UNSPECIFIED
                | WindowManagerLayoutParams_SOFT_INPUT_ADJUST_RESIZE);

    mNativeContentView = new NativeContentView(this);
    mNativeContentView->mActivity = (INativeActivity*)this;
    SetContentView((IView*)mNativeContentView);
    mNativeContentView->RequestFocus();
    AutoPtr<IViewTreeObserver> observer;
    observer = mNativeContentView->GetViewTreeObserver();
    observer->AddOnGlobalLayoutListener(this);
        
//    try {
    AutoPtr<ILocalCapsuleManager> capsulemanager;
    GetCapsuleManager((ILocalCapsuleManager**)&capsulemanager);
    AutoPtr<IIntent> intent;
    GetIntent((IIntent**)&intent);
    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    capsulemanager->GetActivityInfo(component, CapsuleManager_GET_META_DATA, (IActivityInfo**)&ai);
    AutoPtr<IBundle> data;
    ai->GetMetaData((IBundle**)&data);
    if (data != NULL) {
        String ln;
        data->GetString(String(NativeActivity_META_DATA_LIB_NAME), &ln);
        if (ln != NULL) libname = ln;
        data->GetString(String(NativeActivity_META_DATA_FUNC_NAME), &ln);
        if (ln != NULL) funcname = ln;
    }
//    } catch (PackageManager.NameNotFoundException e) {
//      throw new RuntimeException("Error getting activity info", e);
//    }

    String path = String(NULL);
    AutoPtr<IApplicationInfo> appInfo;
    ai->GetApplicationInfo((IApplicationInfo**)&appInfo);
    String dir;
    appInfo->GetNativeLibraryDir(&dir);
    AutoPtr<IFile> libraryFile;
//    CFile::New(dir, System.mapLibraryName(libname), (IFile**)&libraryFile);
    Boolean isExist;
    libraryFile->Exists(&isExist);
    if (isExist) {
        libraryFile->GetPath(&path);
    }
        
    if (path == NULL) {
        //     throw new IllegalArgumentException("Unable to find native library: " + libname);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    ArrayOf<Byte>* value;
    savedInstanceState->GetByteArray(String(NativeActivity_KEY_NATIVE_SAVED_STATE), &value);    
    ArrayOf<Byte>* nativeSavedState = savedInstanceState != NULL ? value : NULL;
    AutoPtr<IFile> filedir;
    GetFilesDir((IFile**)&filedir);
    String fstring;
    filedir->ToString(&fstring);
    String cName;
    ai->GetCapsuleName(&cName);
    AutoPtr<IFile> appFile = Environment::GetExternalStorageAppFilesDirectory(cName);
    String appFileString;
    appFile->ToString(&appFileString);
    AutoPtr<IAssetManager> asset;
    GetAssets((IAssetManager**)&asset);
    mNativeHandle = LoadNativeCode(path, funcname, NULL/*Looper.myQueue()*/,
            fstring,
            appFileString,
            Build::VERSION::SDK_INT, asset, nativeSavedState);
        
    if (mNativeHandle == 0) {
        //     throw new IllegalArgumentException("Unable to load native library: " + path);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Activity::OnCreate(savedInstanceState);
    return NOERROR;
}

ECode NativeActivity::OnDestroy()
{
    mDestroyed = TRUE;
    if (mCurSurfaceHolder != NULL) {
        OnSurfaceDestroyedNative(mNativeHandle);
        mCurSurfaceHolder = NULL;
    }
    if (mCurInputQueue != NULL) {
        AutoPtr<IInputChannel> channel;
        mCurInputQueue->GetInputChannel((IInputChannel**)&channel);
        OnInputChannelDestroyedNative(mNativeHandle, channel);
        mCurInputQueue = NULL;
    }
    UnloadNativeCode(mNativeHandle);
    Activity::OnDestroy();
    return NOERROR;
}

ECode NativeActivity::OnPause()
{
    Activity::OnPause();
    OnPauseNative(mNativeHandle);
    return NOERROR;
}

ECode NativeActivity::OnResume()
{
    Activity::OnResume();
    OnResumeNative(mNativeHandle);
    return NOERROR;
}

ECode NativeActivity::OnSaveInstanceState(
        /* [in] */ IBundle* outState)
{
    Activity::OnSaveInstanceState(outState);
    ArrayOf<Byte>* state = OnSaveInstanceStateNative(mNativeHandle);
    if (state != NULL) {
        outState->PutByteArray(String(NativeActivity_KEY_NATIVE_SAVED_STATE), state);
    }
    return NOERROR;
}

ECode NativeActivity::OnStart()
{
    Activity::OnStart();
    OnStartNative(mNativeHandle);
    return NOERROR;
}

ECode NativeActivity::OnStop()
{
    Activity::OnStop();
    OnStopNative(mNativeHandle);
    return NOERROR;
}

ECode NativeActivity::OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig)
{
    Activity::OnConfigurationChanged(newConfig);
    if (!mDestroyed) {
        OnConfigurationChangedNative(mNativeHandle);
    }
    return NOERROR;
}

ECode NativeActivity::OnLowMemory()
{
    Activity::OnLowMemory();
    if (!mDestroyed) {
        OnLowMemoryNative(mNativeHandle);
    }
    return NOERROR;
}

ECode NativeActivity::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    Activity::OnWindowFocusChanged(hasFocus);
    if (!mDestroyed) {
        OnWindowFocusChangedNative(mNativeHandle, hasFocus);
    }
    return NOERROR;
}

ECode NativeActivity::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    if (mDispatchingUnhandledKey) {
        return Activity::DispatchKeyEvent(event, result);
    }
    else {
        // Key events from the IME do not go through the input channel;
        // we need to intercept them here to hand to the application.
        DispatchKeyEventNative(mNativeHandle, event);
        *result = TRUE;
        return NOERROR;
    }
}

ECode NativeActivity::SurfaceCreated(
        /* [in] */ ISurfaceHolder* holder)
{
    if (!mDestroyed) {
        mCurSurfaceHolder = holder;
        AutoPtr<ISurface> surface;
        holder->GetSurface((ISurface**)&surface);
        OnSurfaceCreatedNative(mNativeHandle, surface);
    }
    return NOERROR;
}

ECode NativeActivity::SurfaceChanged(
        /* [in] */ ISurfaceHolder* holder,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height)
{
    if (!mDestroyed) {
        mCurSurfaceHolder = holder;
        AutoPtr<ISurface> surface;
        holder->GetSurface((ISurface**)&surface);
        OnSurfaceChangedNative(mNativeHandle, surface, format, width, height);
    }
    return NOERROR;
}

ECode NativeActivity::SurfaceRedrawNeeded(
        /* [in] */ ISurfaceHolder* holder)
{
    if (!mDestroyed) {
        mCurSurfaceHolder = holder;
        AutoPtr<ISurface> surface;
        holder->GetSurface((ISurface**)&surface);
        OnSurfaceRedrawNeededNative(mNativeHandle, surface);
    }
    return NOERROR;
}

ECode NativeActivity::SurfaceDestroyed(
        /* [in] */ ISurfaceHolder* holder)
{
    mCurSurfaceHolder = NULL;
    if (!mDestroyed) {
        OnSurfaceDestroyedNative(mNativeHandle);
    }
    return NOERROR;
}

ECode NativeActivity::OnInputQueueCreated(
        /* [in] */IInputQueue* queue)
{
    if (!mDestroyed) {
        mCurInputQueue = queue;
        AutoPtr<IInputChannel> channel;
        queue->GetInputChannel((IInputChannel**)&channel);
        OnInputChannelCreatedNative(mNativeHandle, channel);
    }
    return NOERROR;
}

ECode NativeActivity::OnInputQueueDestroyed(
        /* [in] */ IInputQueue* queue)
{
    mCurInputQueue = NULL;
    if (!mDestroyed) {
        AutoPtr<IInputChannel> channel;
        queue->GetInputChannel((IInputChannel**)&channel);
        OnInputChannelDestroyedNative(mNativeHandle, channel);
    }
    return NOERROR;
}

ECode NativeActivity::OnGlobalLayout()
{
    mNativeContentView->GetLocationInWindow(mLocation,mLocation+1);
    Int32 w = mNativeContentView->GetWidth();
    Int32 h = mNativeContentView->GetHeight();
    if (mLocation[0] != mLastContentX || mLocation[1] != mLastContentY
            || w != mLastContentWidth || h != mLastContentHeight) {
        mLastContentX = mLocation[0];
        mLastContentY = mLocation[1];
        mLastContentWidth = w;
        mLastContentHeight = h;
        if (!mDestroyed) {
            OnContentRectChangedNative(mNativeHandle, mLastContentX,
                    mLastContentY, mLastContentWidth, mLastContentHeight);
        }
    }
    return NOERROR;
}

void NativeActivity::DispatchUnhandledKeyEvent(
        /* [in] */ IKeyEvent* event)
{
    //try {
    mDispatchingUnhandledKey = TRUE;
    AutoPtr<IView> decor;
    GetWindow()->GetDecorView((IView**)&decor);
    if (decor != NULL) {
        Boolean res;
        decor->DispatchKeyEvent(event, &res);
    }
    //} finally {
    mDispatchingUnhandledKey = FALSE;
    //}
}

void NativeActivity::PreDispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 seq)
{
    mIMM->DispatchKeyEvent(this, seq, event, mInputMethodCallback);
}

void NativeActivity::SetWindowFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    GetWindow()->SetFlags(flags, mask);
}

void NativeActivity::SetWindowFormat(
    /* [in] */ Int32 format)
{
    GetWindow()->SetFormat(format);
}

void NativeActivity::ShowIme(
    /* [in] */ Int32 mode)
{
    Boolean res;
    mIMM->ShowSoftInput((IView*)mNativeContentView, mode, &res);
}

void NativeActivity::HideIme(
    /* [in] */ Int32 mode)
{
    Boolean res;
    mIMM->HideSoftInputFromWindow(mNativeContentView->GetWindowToken(), mode, &res);
}