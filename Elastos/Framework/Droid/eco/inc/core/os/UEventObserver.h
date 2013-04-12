
#ifndef __UEVENTOBSERVER_H__
#define __UEVENTOBSERVER_H__

#include "ext/frameworkext.h"
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>
#include <elastos/Thread.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

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
class UEventObserver : public ElRefBase
{
public:
    /**
     * Representation of a UEvent.
     */
    class UEvent : public ElRefBase
    {
    public:
        UEvent(
            /* [in] */ const String& message);

        virtual ~UEvent();

        CARAPI_(String) Get(
            /* [in] */ const String& key);

        CARAPI_(String) Get(
            /* [in] */ const String& key,
            /* [in] */ const String& defaultValue);

        CARAPI_(String) ToString();

    public:
        // collection of key=value pairs parsed from the uevent message
        HashMap<String, String> mMap;
    };

    class Observer
    {
    public:
        Observer(
            /* [in] */ const String& str,
            /* [in] */ UEventObserver* obs)
            : mName(str), mObserver(obs) {}

        String mName;
        AutoPtr<UEventObserver> mObserver;
    };

    class UEventThread
        : public ElRefBase
        , public Thread
        , public IThread
    {
    public:
        UEventThread();

        virtual ~UEventThread();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *object,
            /* [out] */ InterfaceID *iID);

        CARAPI CheckAccess();

        CARAPI CountStackFrames(
            /* [out] */ Int32* number);

        CARAPI Destroy();

        CARAPI GetId(
            /* [out] */ Int64* id);

        CARAPI GetName(
            /* [out] */ String* name);

        CARAPI GetPriority(
            /* [out] */ Int32* priority);

        CARAPI GetState(
            /* [out] */ ThreadState* state);

        CARAPI GetThreadGroup(
            /* [out] */ IThreadGroup** group);

        CARAPI Interrupt();

        CARAPI IsAlive(
            /* [out] */ Boolean* isAlive);

        CARAPI IsDaemon(
            /* [out] */ Boolean* isDaemon);

        CARAPI IsInterrupted(
            /* [out] */ Boolean* isInterrupted);

        CARAPI Join();

        CARAPI JoinEx(
            /* [in] */ Int64 millis);

        CARAPI JoinEx2(
            /* [in] */ Int64 millis,
            /* [in] */ Int32 nanos);

        CARAPI Resume();

        CARAPI SetDaemon(
            /* [in] */ Boolean isDaemon);

        CARAPI SetName(
            /* [in] */ const String& threadName);

        CARAPI SetPriority(
            /* [in] */ Int32 priority);

        CARAPI Start();

        CARAPI Stop();

        CARAPI Suspend();

        CARAPI Detach();

        CARAPI Lock();

        CARAPI Unlock();

        CARAPI Notify();

        CARAPI NotifyAll();

        CARAPI Wait();

        CARAPI WaitEx(
            /* [in] */ Int64 millis);

        CARAPI WaitEx2(
            /* [in] */ Int64 millis,
            /* [in] */ Int32 nanos);

        CARAPI Run();

        CARAPI AddObserver(
            /* [in] */ const String& match,
            /* [in] */ UEventObserver * observer);

        /** Removes every key/value pair where value=observer from mObservers */
        CARAPI RemoveObserver(
            /* [in] */ UEventObserver * observer);

    private:
        CARAPI_(Mutex*) GetSelfLock();

    private:
        /** Many to many mapping of string match to observer.
         *  Multimap would be better, but not available in android, so use
         *  an ArrayList where even elements are the String match and odd
         *  elements the corresponding UEventObserver observer */
        List<Observer*> mObservers;
        Mutex mObserversLock;
        Mutex mLock;
    };

    virtual ~UEventObserver();

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
    CARAPI_(void) StartObserving(
        /* [in] */ const String& match);

    /**
     * End observation of UEvent's.<p>
     * This process's UEvent thread will never call onUEvent() on this
     * UEventObserver after this call. Repeated calls have no effect.
     */
    CARAPI_(void) StopObserving();

    /**
     * Subclasses of UEventObserver should override this method to handle
     * UEvents.
     */
    virtual CARAPI OnUEvent(
        /* [in] */ UEvent* event) = 0;

private:
    static CARAPI NativeSetup();

    static CARAPI_(Int32) NextEvent(
        /* [in] */ ArrayOf<Byte>& buffer);

    static CARAPI_(void) EnsureThreadStarted();

private:
    static const CString TAG;
    static AutoPtr<UEventThread> sThread;
    static Boolean sThreadStarted;

    Mutex mLock;
};

#endif //__UEVENTOBSERVER_H__
