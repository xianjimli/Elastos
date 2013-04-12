
#include "os/UEventObserver.h"
#include <Logger.h>
#include <StringBuffer.h>
#include <hardware_legacy/uevent.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;


/**
 * UEventObserver is an abstract class that receives UEvent's from the kernel.<p>
 *
 * Subclass UEventObserver, implementing onUEvent(UEvent event), then call
 * startObserving() with a match string. The UEvent thread will then call your
 * onUEvent() method when a UEvent occurs that contains your match string.<p>
 *
 * Call stopObserving() to stop receiving UEvent's.<p>
 *
 * There is only one UEvent thread per process, even if that process has
 * multiple UEventObserver subclass instances. The UEvent thread starts when
 * the startObserving() is called for the first time in that process. Once
 * started the UEvent thread will not stop (although it can stop notifying
 * UEventObserver's via stopObserving()).<p>
 *
 * @hide
*/
UEventObserver::UEvent::UEvent(
    /* [in] */ const String& message)
{
    // collection of key=value pairs parsed from the uevent message
    Int32 offset = 0;
    Int32 length = message.GetLength();

    while (offset < length) {
        Int32 equals = message.IndexOf('=', offset);
        Int32 at = message.IndexOf((UInt32)0, offset);
        if (at < 0) break;

        if (equals > offset && equals < at) {
            // key is before the equals sign, and value is after
            mMap[message.Substring(offset, equals)]=
                    message.Substring(equals + 1, at);
        }

        offset = at + 1;
    }
}

UEventObserver::UEvent::~UEvent()
{
    mMap.Clear();
}

String UEventObserver::UEvent::Get(
    /* [in] */ const String& key)
{
    HashMap<String, String>::Iterator iter = mMap.Find(key);
    if (iter != mMap.End()) {
        return iter->mSecond;
    }
    else {
        return String(NULL);
    }
}

String UEventObserver::UEvent::Get(
    /* [in] */ const String& key,
    /* [in] */ const String& defaultValue)
{
    HashMap<String, String>::Iterator iter = mMap.Find(key);
    if (iter != mMap.End()) {
        return iter->mSecond;
    }
    else {
        return defaultValue;
    }
}

String UEventObserver::UEvent::ToString()
{
    //Returns a string representation of this map. The string representation consists of a
    //list of key-value mappings in the order returned by the map's entrySet view's
    //iterator, enclosed in braces ("{}"). Adjacent mappings are separated by the
    //characters ", " (comma and space). Each key-value mapping is rendered as the
    //key followed by an equals sign ("=") followed by the associated value.
    //Keys and values are converted to strings as by String.valueOf(Object).
    HashMap<String, String>::Iterator iter;
    StringBuffer buffer("{");
    for (iter = mMap.Begin(); iter != mMap.End(); ++iter) {
        buffer += iter->mFirst + "=" + iter->mSecond;
        HashMap<String, String>::Iterator nextIter = iter;
        if (++iter != mMap.End()) {
            buffer += ",";
        }
    }
    buffer += "}";
    return String(buffer);
}


UEventObserver::UEventThread::UEventThread()
{
    Thread::SetName(String("UEventObserver"));
}

UEventObserver::UEventThread::~UEventThread()
{
    List<Observer*>::Iterator iter = mObservers.Begin();
    while(iter != mObservers.End()) {
        Observer* ob = *iter;
        delete ob;
        iter = mObservers.Erase(iter);
    }
}

PInterface UEventObserver::UEventThread::Probe(
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

UInt32 UEventObserver::UEventThread::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UEventObserver::UEventThread::Release()
{
    return ElRefBase::Release();
}

ECode UEventObserver::UEventThread::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *iID)
{
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IThread*)this) {
        *iID = EIID_IThread;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode UEventObserver::UEventThread::Run()
{
    FAIL_RETURN(NativeSetup());

    ArrayOf_<Byte, 1024> buffer;
    Int32 len;
    while (TRUE) {
        len = NextEvent(buffer);
        if (len > 0) {
            String bufferStr((char*)buffer.GetPayload(), len);  // easier to search a String
            {
                Mutex::Autolock lock(&mObserversLock);

                List<Observer*>::Iterator iter;
                for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
                    Observer* ob = *iter;
                    assert(ob != NULL);
                    if (bufferStr.IndexOf(ob->mName) != -1) {
                        AutoPtr<UEvent> event = new UEvent(bufferStr);
                        ob->mObserver->OnUEvent(event);
                    }
                }
            }
        }
    }
    return NOERROR;
}

ECode UEventObserver::UEventThread::AddObserver(
    /* [in] */ const String& match,
    /* [in] */ UEventObserver* observer)
{
    Observer* ob = new Observer(match,observer);
    Mutex::Autolock lock(&mObserversLock);
    mObservers.PushBack(ob);
    return NOERROR;
}

/** Removes every key/value pair where value=observer from mObservers */
ECode UEventObserver::UEventThread::RemoveObserver(
    /* [in] */ UEventObserver* observer)
{
    Mutex::Autolock lock(&mObserversLock);
    Boolean found = TRUE;
    while (found) {
        found = FALSE;
        List<Observer*>::Iterator iter;
        for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
            Observer* ob = *iter;
            assert(ob != NULL);
            if (ob->mObserver.Get() == observer) {
                mObservers.Erase(iter);
                delete ob;
                found = TRUE;
                break;
            }
        }
    }
    return NOERROR;
}

ECode UEventObserver::UEventThread::CheckAccess()
{
    return Thread::CheckAccess();
}

ECode UEventObserver::UEventThread::CountStackFrames(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return Thread::CountStackFrames(number);
}

ECode UEventObserver::UEventThread::Destroy()
{
    return Thread::Destroy();
}

ECode UEventObserver::UEventThread::GetId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    return Thread::GetId(id);
}

ECode UEventObserver::UEventThread::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return Thread::GetName(name);
}

ECode UEventObserver::UEventThread::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    return Thread::GetPriority(priority);
}

ECode UEventObserver::UEventThread::GetState(
    /* [out] */ ThreadState* state)
{
    return Thread::GetState(state);
}

ECode UEventObserver::UEventThread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    VALIDATE_NOT_NULL(group);
    return Thread::GetThreadGroup(group);
}

ECode UEventObserver::UEventThread::Interrupt()
{
    return Thread::Interrupt();
}

ECode UEventObserver::UEventThread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    VALIDATE_NOT_NULL(isAlive);
    return Thread::IsAlive(isAlive);
}

ECode UEventObserver::UEventThread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    VALIDATE_NOT_NULL(isDaemon);
    return Thread::IsDaemon(isDaemon);
}

ECode UEventObserver::UEventThread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    VALIDATE_NOT_NULL(isInterrupted);
    return Thread::IsInterrupted(isInterrupted);
}

ECode UEventObserver::UEventThread::Join()
{
    return Thread::Join();
}

ECode UEventObserver::UEventThread::JoinEx(
    /* [in] */ Int64 millis)
{
    return Thread::JoinEx(millis);
}

ECode UEventObserver::UEventThread::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::JoinEx2(millis, nanos);
}

ECode UEventObserver::UEventThread::Resume()
{
    return Thread::Resume();
}

ECode UEventObserver::UEventThread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return Thread::SetDaemon(isDaemon);
}

ECode UEventObserver::UEventThread::SetName(
    /* [in] */ const String& threadName)
{
    return Thread::SetName(threadName);
}

ECode UEventObserver::UEventThread::SetPriority(
    /* [in] */ Int32 priority)
{
    return Thread::SetPriority(priority);
}

ECode UEventObserver::UEventThread::Start()
{
    return Thread::Start();
}

ECode UEventObserver::UEventThread::Stop()
{
    return Thread::Stop();
}

ECode UEventObserver::UEventThread::Suspend()
{
    return Thread::Suspend();
}

ECode UEventObserver::UEventThread::Detach()
{
    return Thread::Detach();
}

ECode UEventObserver::UEventThread::Lock()
{
    return Thread::Lock();
}

ECode UEventObserver::UEventThread::Unlock()
{
    return Thread::Unlock();
}

ECode UEventObserver::UEventThread::Notify()
{
    return Thread::Notify();
}

ECode UEventObserver::UEventThread::NotifyAll()
{
    return Thread::NotifyAll();
}

ECode UEventObserver::UEventThread::Wait()
{
    return Thread::Wait();
}

ECode UEventObserver::UEventThread::WaitEx(
    /* [in] */ Int64 millis)
{
    return Thread::Wait(millis);
}

ECode UEventObserver::UEventThread::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Wait(millis, nanos);
}

Mutex* UEventObserver::UEventThread::GetSelfLock()
{
    return &mLock;
}


const CString UEventObserver::TAG = "UEventObserver";
AutoPtr<UEventObserver::UEventThread> UEventObserver::sThread;
Boolean UEventObserver::sThreadStarted = FALSE;

UEventObserver::~UEventObserver()
{
    StopObserving();
}

ECode UEventObserver::NativeSetup()
{
    if (!uevent_init()) {
    //        jniThrowException(env, "java/lang/RuntimeException",
    //                          "Unable to open socket for UEventObserver");
        Logger::E(TAG,CString("Unable to open socket for UEventObserver"));
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;

}

Int32 UEventObserver::NextEvent(
    /* [in] */ ArrayOf<Byte>& buffer)
{
    int buf_sz = buffer.GetLength();
    char* buf = (char*)buffer.GetPayload();

    int length = uevent_next_event(buf, buf_sz - 1);

    return length;
}

void UEventObserver::EnsureThreadStarted()
{
    if (sThreadStarted == FALSE) {
        sThread = new UEventThread();
        sThread->Start();
        sThreadStarted = TRUE;
    }
}

/**
 * Begin observation of UEvent's.<p>
 * This method will cause the UEvent thread to start if this is the first
 * invocation of startObserving in this process.<p>
 * Once called, the UEvent thread will call onUEvent() when an incoming
 * UEvent matches the specified string.<p>
 * This method can be called multiple times to register multiple matches.
 * Only one call to stopObserving is required even with multiple registered
 * matches.
 * @param match A substring of the UEvent to match. Use "" to match all
 *              UEvent's
 */
void UEventObserver::StartObserving(
    /* [in] */ const String& match)
{
    Mutex::Autolock lock(&mLock);

    EnsureThreadStarted();
    sThread->AddObserver(match, this);
}

/**
 * End observation of UEvent's.<p>
 * This process's UEvent thread will never call onUEvent() on this
 * UEventObserver after this call. Repeated calls have no effect.
 */
//public final synchronized
void UEventObserver::StopObserving()
{
    Mutex::Autolock lock(&mLock);

    sThread->RemoveObserver(this);
}
