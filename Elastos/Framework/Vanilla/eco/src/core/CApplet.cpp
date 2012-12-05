#include <stdio.h>
#include "CApplet.h"

typedef struct
{
    CApplet* mApplet;
    const BufferOf<CString>* mArgs;
} AppletParams;

CApplet::CApplet()
{
    _Impl_CallbackSink_InitCallbackContext(&mCallbackContent);
    printf("============CApplet::CApplet();\n");
    mbActivated = FALSE;
}

CApplet::~CApplet()
{

}

void* CApplet::EntryRoutine(void *arg)
{
    ECode ec;

    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    AppletParams* params = reinterpret_cast<AppletParams*>(arg);
    CApplet* applet = params->mApplet;
    const BufferOf<CString>* args = params->mArgs;
    assert(applet != NULL && args != NULL);

    IInterface* orgCallbackContext = reinterpret_cast<IInterface*>(pthread_getspecific(TL_CALLBACK_SLOT));
    if (orgCallbackContext != NULL) {
        orgCallbackContext->Release();
    }
    pthread_setspecific(TL_CALLBACK_SLOT, applet->mCallbackContent);
    BufferOf_<CString, 0> box;
    applet->Main(box);
    printf("============CApplet::EntryRoutine:applet 0x%x TryToHandleEvents.\n", applet);
    ec = _Impl_CallbackSink_TryToHandleEvents(applet->mCallbackContent);
    printf("============CApplet::EntryRoutine:applet 0x%x NotifyQuitted.\n", applet);
    applet->NotifyQuitted(0);
    return reinterpret_cast<void*>(ec);
}

ECode CApplet::Start(
    /* [in] */ const BufferOf<CString>& args)
{
    mbActivated = TRUE;

    AppletParams* params = new AppletParams;
    params->mApplet = this;
    params->mArgs = &args;
    if (pthread_create(&mThread, NULL, EntryRoutine, (void*)params)) {
        return E_THREAD_ABORTED;
    }
    return NOERROR;
}

ECode CApplet::Quit(
    /* [in] */ Int32 n,
    /* [in] */ AppletQuit flag)
{
    printf("============CApplet::Quit() n = %d;\n",n);

    if (flag == AppletQuit_Nicely) {
        return _Impl_CallbackSink_RequestToFinish(mCallbackContent, CallbackContextFinish_Nice);
    }
    return E_NOT_IMPLEMENTED;
}

ECode CApplet::Destroy()
{
    return NOERROR;
}

ECode CApplet::Activate()
{
    if (! mbActivated)
        mbActivated = TRUE;

    return NOERROR;
}

ECode CApplet::Deactivate()
{
    if (mbActivated)
        mbActivated = FALSE;

    return NOERROR;
}

ECode CApplet::IsActivated(
    /* [out] */ Boolean * pActivated)
{
    *pActivated = mbActivated;
    return NOERROR;
}

ECode CApplet::Snooze()
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplet::Wakeup()
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplet::NotifyQuitted(
    /* [in] */ Int32 n)
{
    printf("============CApplet::NotifyQuitted();\n");
    return Callback::Quitted();
}
