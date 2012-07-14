
#ifndef __HH_FILEOBSERVER_H
#define __HH_FILEOBSERVER_H

#include "ext/frameworkext.h"
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace::Elastos::Core::Threading;

class FileObserver
{
public:
    class ObserverThread
    {
    public:
        ObserverThread();

        ~ObserverThread();

        CARAPI Start();

        CARAPI_(Int32) StartWatching(
            /* [in] */ String path,
            /* [in] */ Int32 mask,
            /* [in] */ IFileObserver* observer);

        CARAPI_(void) StopWatching(
            /* [in] */ Int32 descriptor);

        CARAPI_(void) OnEvent(
            /* [in] */ Int32 wfd,
            /* [in] */ Int32 mask,
            /* [in] */ String path);

    private:
        static void* EntryRoutine(void *arg);

        CARAPI_(Int32) Init();

        CARAPI_(void) Observe(
            /* [in] */ Int32 fd);

        CARAPI_(Int32) StartWatching(
            /* [in] */ Int32 fd,
            /* [in] */ String path,
            /* [in] */ Int32 mask);

        CARAPI_(void) StopWatching(
            /* [in] */ Int32 fd,
            /* [in] */ Int32 wfd);

    private:
        //todo: weak reference
        HashMap<Int32, AutoPtr<IFileObserver> > mObservers;
        Mutex mObserversLock;
        Int32 mFd;

        pthread_t mThread;
    };

public:
    FileObserver() {}

    /**
     * Create a new file observer for a certain file or directory.
     * Monitoring does not start on creation!  You must call
     * {@link #startWatching()} before you will receive events.
     *
     * @param path The file or directory to monitor
     * @param mask The event or events (added together) to watch for
     */
    FileObserver(
        /* [in] */ const String& path,
        /* [in] */ Int32 mask = FileObserver_ALL_EVENTS)
        : mPath(path)
        , mDescriptor(-1)
        , mMask(mask){}

    ~FileObserver() { StopWatching(); }

    virtual CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid) = 0;

    /**
     * Start watching for events.  The monitored file or directory must exist at
     * this time, or else no events will be reported (even if it appears later).
     * If monitoring is already started, this call has no effect.
     */
    CARAPI StartWatching();

    /**
     * Stop watching for events.  Some events may be in process, so events
     * may continue to be reported even after this method completes.  If
     * monitoring is already stopped, this call has no effect.
     */
    CARAPI StopWatching();

    virtual CARAPI OnEvent(
        /* [in] */ Int32 event,
        /* [in] */ const String& path) = 0;

private:
    static const char* LOG_TAG;

    // instance
    String mPath;
    Int32 mDescriptor;
    Int32 mMask;
};

#endif // __HH_FILEOBSERVER_H

