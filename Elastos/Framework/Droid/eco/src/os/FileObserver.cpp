
#include "os/FileObserver.h"
#include <unistd.h>
#include <errno.h>
#ifdef HAVE_INOTIFY
#include <sys/inotify.h>
#endif


FileObserver::ObserverThread::ObserverThread()
    : mObservers(5)
{
    mFd = Init();
}

FileObserver::ObserverThread::~ObserverThread()
{
    mObservers.Clear();
}

FileObserver::ObserverThread::Start()
{
    if (pthread_create(&mThread, NULL, EntryRoutine, (void*)(FileObserver*)this)) {
        return E_THREAD_ABORTED;
    }
    return NOERROR;
}

void* FileObserver::ObserverThread::EntryRoutine(void *arg)
{
    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    ObserverThread* obt = (ObserverThread*)arg;
    obt->Observe(obt->mFd);
    return (void*)NOERROR;
}

Int32 FileObserver::ObserverThread::StartWatching(
    /* [in] */ String path,
    /* [in] */ Int32 mask,
    /* [in] */ IFileObserver* observer)
{
    Int32 wfd = StartWatching(mFd, path, mask);

    if (wfd >= 0) {
        Mutex::Autolock lock(&mObserversLock);

        mObservers[wfd] = observer;
    }

    return wfd;
}

void FileObserver::ObserverThread::StopWatching(
    /* [in] */ Int32 descriptor)
{
    StopWatching(mFd, descriptor);
}

void FileObserver::ObserverThread::OnEvent(
    /* [in] */ Int32 wfd,
    /* [in] */ Int32 mask,
    /* [in] */ String path)
{
    // look up our observer, fixing up the map if necessary...
    IFileObserver* observer = NULL;

    {
        Mutex::Autolock lock(&mObserversLock);

        HashMap<Int32, AutoPtr<IFileObserver> >::Iterator it = mObservers.Find(wfd);
        if (it != mObservers.End()) {
            observer = it->mSecond;
        }
//        if (observer == null) {
//            m_observers.remove(wfd);
//        }
    }

    // ...then call out to the observer without the sync lock held
    if (observer != NULL) {
//        try {
        observer->OnEvent(mask, path);
//        } catch (Throwable throwable) {
//            Log.wtf(LOG_TAG, "Unhandled exception in FileObserver " + observer, throwable);
//        }
    }
}

Int32 FileObserver::ObserverThread::Init()
{
#ifdef HAVE_INOTIFY

    return (Int32)inotify_init();

#else // HAVE_INOTIFY

    return -1;

#endif // HAVE_INOTIFY
}

void FileObserver::ObserverThread::Observe(
    /* [in] */ Int32 fd)
{
#ifdef HAVE_INOTIFY

    char event_buf[512];
    struct inotify_event* event;

    while (1) {
        int event_pos = 0;
        int num_bytes = read(fd, event_buf, sizeof(event_buf));

        if (num_bytes < (int)sizeof(*event)) {
            if (errno == EINTR) {
                continue;
            }

//            LOGE("***** ERROR! android_os_fileobserver_observe() got a short event!");
            return;
        }

        while (num_bytes >= (int)sizeof(*event)) {
            int event_size;
            event = (struct inotify_event *)(event_buf + event_pos);

            String path;

            if (event->len > 0) {
                path = event->name;
            }

            OnEvent(event->wd, event->mask, path);

            event_size = sizeof(*event) + event->len;
            num_bytes -= event_size;
            event_pos += event_size;
        }
    }

#endif // HAVE_INOTIFY
}

Int32 FileObserver::ObserverThread::StartWatching(
    /* [in] */ Int32 fd,
    /* [in] */ String path,
    /* [in] */ Int32 mask)
{
    int res = -1;

#ifdef HAVE_INOTIFY

    if (fd >= 0) {
        res = inotify_add_watch(fd, (const char*)path, mask);
    }

#endif // HAVE_INOTIFY

    return res;
}

CARAPI_(void) FileObserver::ObserverThread::StopWatching(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 wfd)
{
#ifdef HAVE_INOTIFY

    inotify_rm_watch((int)fd, (uint32_t)wfd);

#endif // HAVE_INOTIFY
}

const char* FileObserver::LOG_TAG = "FileObserver";

FileObserver::ObserverThread* StartObserverThread()
{
    FileObserver::ObserverThread* obt = new FileObserver::ObserverThread();
    obt->Start();
    return obt;
}

FileObserver::ObserverThread* sObserverThread = StartObserverThread();

ECode FileObserver::StartWatching()
{
    if (mDescriptor < 0) {
        mDescriptor = sObserverThread->StartWatching(
                mPath, mMask, (IFileObserver*)this->Probe(EIID_IFileObserver));
    }
    return NOERROR;
}

ECode FileObserver::StopWatching()
{
    if (mDescriptor >= 0) {
        sObserverThread->StopWatching(mDescriptor);
        mDescriptor = -1;
    }
    return NOERROR;
}
