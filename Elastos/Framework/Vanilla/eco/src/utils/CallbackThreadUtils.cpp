
#include <pthread.h>
#include "utils/CallbackThreadUtils.h"

static void *pAppletObj;
pthread_mutex_t appletMutex;
pthread_cond_t appletCond;

static void* MsgLoopThread(void *vptr_args)
{
	ECode    ec;

    IInterface *pOrgCallbackContext;
    IInterface *m_pCallbackContext;

    _Impl_CallbackSink_InitCallbackContext(&m_pCallbackContext);
    pOrgCallbackContext = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
    if (NULL != pOrgCallbackContext) {
        pOrgCallbackContext->Release();
    }
    pthread_setspecific(TL_CALLBACK_SLOT, m_pCallbackContext);

    pAppletObj = m_pCallbackContext;

	//CApplet::Finish(AppletFinish_ASAP);
	pthread_mutex_lock(&appletMutex);
	pthread_cond_signal(&appletCond);
	pthread_mutex_unlock(&appletMutex);

	//ec = _Impl_TApplet_DoWork(pAppletObj, ec);
	ec = _Impl_CallbackSink_TryToHandleEvents(m_pCallbackContext);

	return NULL;
}

static bool dvmCreateMsgLoopThread(void **ppAppletObj)
{
	pthread_mutex_init(&appletMutex, NULL);
	pthread_cond_init(&appletCond, NULL);

	pthread_mutex_lock(&appletMutex);

    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

	if (pthread_create(&mThread, &threadAttr, MsgLoopThread, NULL) != 0) {
	    pthread_mutex_unlock(&appletMutex);
    	pthread_mutex_destroy(&appletMutex);
    	pthread_cond_destroy(&appletCond);
		return false;
	}

	pthread_cond_wait(&appletCond, &appletMutex);
	pthread_mutex_unlock(&appletMutex);

	if (ppAppletObj != NULL)
	    *ppAppletObj = pAppletObj;

	pthread_mutex_destroy(&appletMutex);
	pthread_cond_destroy(&appletCond);

	return true;
}


//shutdown not main thread's MsgLoopThread
static void dvmShutdownMsgLoopThread(void *pAppletObj)
{
	if (pAppletObj == NULL)
	    return;

    ECode ec;

    ec = _Impl_CallbackSink_RequestToFinish((IInterface *)pAppletObj, CallbackContextFinish_ASAP);

    ((IInterface *)pAppletObj)->Release();
}


//shutdown not main thread's MsgLoopThread
static void dvmAttachMsgLoopThread(void *pAppletObj)
{
	if (pAppletObj == NULL)
	    return;

	pthread_setspecific(TL_CALLBACK_SLOT, pAppletObj);
}


static void dvmDetachMsgLoopThread()
{
    pthread_setspecific(TL_CALLBACK_SLOT, NULL);
}


/*
 * CreateCallbackThread()
 */
void *CreateCallbackThread(void)
{
    void *pAppletObj;
    
    if (!dvmCreateMsgLoopThread(&pAppletObj))
    	return  NULL;
    
    dvmAttachMsgLoopThread(pAppletObj);
    
    return pAppletObj;
    	
}

/*
 * StopCallbackThread()
 */
void StopCallbackThread(void *pAppletObj)
{
    dvmShutdownMsgLoopThread(pAppletObj);
}
