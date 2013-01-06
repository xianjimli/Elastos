
#include "MyThread.h"


UInt32 MyThread::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MyThread::Release()
{
    return ElRefBase::Release();
}

PInterface MyThread::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IThread) {
        return (IThread*)this;
    }

    return NULL;
}

ECode MyThread::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IThread*)this) {
        *pIID = EIID_IThread;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MyThread::Run()
{
    return NOERROR;
}

ECode MyThread::CheckAccess()
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::CountStackFrames(
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::Destroy()
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::GetId(
    /* [out] */ Int64* id)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::GetName(
    /* [out] */ String* name)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::GetPriority(
    /* [out] */ Int32* priority)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::GetState(
    /* [out] */ ThreadState* state)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::Interrupt()
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::Join()
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::JoinEx(
    /* [in] */ Int64 millis)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::Resume()
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::SetName(
    /* [in] */ const String& threadName)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::SetPriority(
    /* [in] */ Int32 priority)
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::Start()
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::Stop()
{
    return E_NOT_IMPLEMENTED;
}

ECode MyThread::Suspend()
{
    return E_NOT_IMPLEMENTED;
}

Mutex* MyThread::GetSelfLock()
{
    return &mLock;
}
