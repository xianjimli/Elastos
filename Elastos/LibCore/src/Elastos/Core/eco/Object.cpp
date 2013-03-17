
#include "Thread.h"
#include "Object.h"


Object::Object()
    : mNativeObject(NativeCreateObject())
{}

Object::~Object()
{
    NativeDestroyObject(mNativeObject);
}

ECode Object::Lock()
{
    return NativeLockObject(mNativeObject);
}

ECode Object::Unlock()
{
    return NativeUnlockObject(mNativeObject);
}

ECode Object::Notify()
{
    return NativeObjectNotify(mNativeObject);
}

ECode Object::NotifyAll()
{
    return NativeObjectNotifyAll(mNativeObject);
}

ECode Object::Wait()
{
    return Wait(0, 0);
}

ECode Object::Wait(
    /* [in] */ Int64 millis)
{
    return Wait(millis, 0);
}

ECode Object::Wait(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return NativeObjectWait(mNativeObject, millis, nanos, TRUE);
}
