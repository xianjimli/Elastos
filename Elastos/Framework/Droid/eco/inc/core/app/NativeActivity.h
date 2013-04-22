
#ifndef __NATIVEACTIVITY_H__
#define __NATIVEACTIVITY_H__

#include "app/Activity.h"
#include "view/View.h"
#include "os/NativeMessageQueue.h"
#include <elastos/ElRefBase.h>


class NativeActivity
    : public Activity
    , public INativeActivity
    , public ISurfaceHolderCallback2
    , public IInputQueueCallback
    , public IOnGlobalLayoutListener
{
public:
    class NativeContentView : public View
    {
    public:
        NativeContentView(
            /* [in] */ IContext* context);

        NativeContentView(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs);

        ~NativeContentView();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    public:
        AutoPtr<INativeActivity> mActivity;
    };

    class InputMethodCallback
        : public ElRefBase
        , public IInputMethodCallback
    {
    public:
        InputMethodCallback(
            /* [in] */ INativeActivity* na);

        ~InputMethodCallback();

        CARAPI_(PInterface) Probe(
             /* [in]  */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        //@Override
        CARAPI FinishedEvent(
            /* [in] */ Int32 seq,
            /* [in] */ Boolean handled);

        //@Override
        CARAPI SessionCreated(
            /* [in] */ IInputMethodSession* session);
    public:
        //WeakReference<NativeActivity> mNa;
    };

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    //@Override
    virtual CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    //@Override
    virtual CARAPI OnLowMemory();

    //@Override
    virtual CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    //@Override
    virtual CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI SurfaceCreated(
        /* [in] */ ISurfaceHolder* holder);

    CARAPI SurfaceChanged(
        /* [in] */ ISurfaceHolder* holder,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SurfaceRedrawNeeded(
        /* [in] */ ISurfaceHolder* holder);

    CARAPI SurfaceDestroyed(
        /* [in] */ ISurfaceHolder* holder);

    CARAPI OnInputQueueCreated(
        /* [in] */ IInputQueue* queue);

    CARAPI OnInputQueueDestroyed(
        /* [in] */ IInputQueue* queue);

    CARAPI OnGlobalLayout();

public:
    CARAPI_(void) DispatchUnhandledKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) PreDispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 seq);

    CARAPI_(void) SetWindowFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    CARAPI_(void) SetWindowFormat(
        /* [in] */ Int32 format);

    CARAPI_(void) ShowIme(
        /* [in] */ Int32 mode);

    CARAPI_(void) HideIme(
        /* [in] */ Int32 mode);

protected:
    //@Override
    virtual CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    //@Override
    virtual CARAPI OnDestroy();

    //@Override
    virtual CARAPI OnPause();

    //@Override
    virtual CARAPI OnResume();

    //@Override
    virtual CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    //@Override
    virtual CARAPI OnStart();

    //@Override
    virtual CARAPI OnStop();

private:
    CARAPI_(Int32) LoadNativeCode(
        /* [in] */ const String& path,
        /* [in] */ const String& funcname,
        /* [in] */ NativeMessageQueue* queue,
        /* [in] */ const String& internalDataPath,
        /* [in] */ const String& externalDataPath,
        /* [in] */ Int32 sdkVersion,
        /* [in] */ IAssetManager* assetMgr,
        /* [in] */ ArrayOf<Byte>* savedState);

    CARAPI_(void) UnloadNativeCode(
        /* [in] */ Int32 handle);

    CARAPI_(void) OnStartNative(
        /* [in] */ Int32 handle);

    CARAPI_(void) OnResumeNative(
        /* [in] */ Int32 handle);

    CARAPI_(ArrayOf<Byte>*) OnSaveInstanceStateNative(
        /* [in] */ Int32 handle);

    CARAPI_(void) OnPauseNative(
        /* [in] */ Int32 handle);

    CARAPI_(void) OnStopNative(
        /* [in] */ Int32 handle);

    CARAPI_(void) OnConfigurationChangedNative(
        /* [in] */ Int32 handle);

    CARAPI_(void) OnLowMemoryNative(
        /* [in] */ Int32 handle);

    CARAPI_(void) OnWindowFocusChangedNative(
        /* [in] */ Int32 handle,
        /* [in] */ Boolean focused);

    CARAPI_(void) OnSurfaceCreatedNative(
        /* [in] */ Int32 handle,
        /* [in] */ ISurface* surface);

    CARAPI_(void) OnSurfaceChangedNative(
        /* [in] */ Int32 handle,
        /* [in] */ ISurface* surface,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(void) OnSurfaceRedrawNeededNative(
        /* [in] */ Int32 handle,
        /* [in] */ ISurface* surface);

    CARAPI_(void) OnSurfaceDestroyedNative(
        /* [in] */ Int32 handle);

    CARAPI_(void) OnInputChannelCreatedNative(
        /* [in] */ Int32 handle,
        /* [in] */ IInputChannel* channel);

    CARAPI_(void) OnInputChannelDestroyedNative(
        /* [in] */ Int32 handle,
        /* [in] */ IInputChannel* channel);

    CARAPI_(void) OnContentRectChangedNative(
        /* [in] */ Int32 handle,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI_(void) DispatchKeyEventNative(
        /* [in] */ Int32 handle,
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) FinishPreDispatchKeyEventNative(
        /* [in] */ Int32 handle,
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

public:
    Int32 mLocation[2];
    Int32 mLastContentX;
    Int32 mLastContentY;
    Int32 mLastContentWidth;
    Int32 mLastContentHeight;

private:
    NativeContentView* mNativeContentView;
    AutoPtr<ILocalInputMethodManager> mIMM;
    AutoPtr<IInputMethodCallback> mInputMethodCallback;

    Int32 mNativeHandle;
    
    AutoPtr<IInputQueue> mCurInputQueue;
    AutoPtr<ISurfaceHolder> mCurSurfaceHolder;

    Boolean mDispatchingUnhandledKey;

    Boolean mDestroyed;
};

#endif //__NATIVEACTIVITY_H__
