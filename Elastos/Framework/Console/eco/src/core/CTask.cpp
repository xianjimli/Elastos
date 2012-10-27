
#include "CTask.h"

typedef struct
{
    CTask* mTask;
    const BufferOf<CString>* mArgs;
} TaskParams;

CTask::CTask()
{
    _Impl_CallbackSink_InitCallbackContext(&mCallbackContent);
}

CTask::~CTask()
{

}

ECode CTask::Main(
    /* [in] */ const BufferOf<CString>& args)
{
    return NOERROR;
}

void* CTask::EntryRoutine(void *arg)
{
    ECode ec;

    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    TaskParams* params = reinterpret_cast<TaskParams*>(arg);
    CTask* task = params->mTask;
    const BufferOf<CString>* args = params->mArgs;
    assert(task != NULL && args != NULL);

    IInterface* orgCallbackContext = reinterpret_cast<IInterface*>(pthread_getspecific(TL_CALLBACK_SLOT));
    if (orgCallbackContext != NULL) {
        orgCallbackContext->Release();
    }
    pthread_setspecific(TL_CALLBACK_SLOT, task->mCallbackContent);
    BufferOf_<CString, 0> box;
    task->Main(box);
    ec = _Impl_CallbackSink_TryToHandleEvents(task->mCallbackContent);
    task->NotifyFinished();
    return reinterpret_cast<void*>(ec);
}

ECode CTask::Start(
    /* [in] */ const BufferOf<CString>& args)
{
    TaskParams* params = new TaskParams;
    params->mTask = this;
    params->mArgs = &args;
    if (pthread_create(&mThread, NULL, EntryRoutine, (void*)params)) {
        return E_THREAD_ABORTED;
    }
    return NOERROR;
}

ECode CTask::Finish(
    /* [in] */ TaskFinish flag)
{
    if (flag == TaskFinish_Nice) {
        return _Impl_CallbackSink_RequestToFinish(mCallbackContent, CallbackContextFinish_Nice);
    }
    return E_NOT_IMPLEMENTED;
}

ECode CTask::NotifyFinished()
{
    return Callback::Finished();
}
