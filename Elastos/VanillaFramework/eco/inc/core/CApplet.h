
#ifndef __CApplet_H__
#define __CApplet_H__

#include "_CApplet.h"
#include <pthread.h>

CarClass(CApplet)
{
public:
    CApplet();

    virtual ~CApplet();

    virtual CARAPI Main(
        /* [in] */ const BufferOf<CString>& args) { return NOERROR; };

    virtual CARAPI AtQuit(
        /* [in] */ Int32 n) { return NOERROR; };

    CARAPI Start(
        /* [in] */ const BufferOf<CString>& args);

    CARAPI Quit(
        /* [in] */ Int32 n,
        /* [in] */ AppletQuit flag);

    CARAPI Destroy();

    CARAPI Activate();

    CARAPI Deactivate();

    CARAPI IsActivated(
      /* [out] */ Boolean * pbActivated);

    CARAPI Snooze();

    CARAPI Wakeup();

private:
    static void* EntryRoutine(void *arg);

    CARAPI NotifyQuitted(
        /* [in] */ Int32 n);

private:
    pthread_t   mThread;
    IInterface* mCallbackContent;
    Boolean     mbActivated;
};

#endif //__CApplet_H__