
#ifndef __ASYNCHRONOUSSOCKETCLOSEMONITOR_H__
#define __ASYNCHRONOUSSOCKETCLOSEMONITOR_H__

#include <elastos.h>
#include <pthread.h>

using namespace Elastos;

/**
 * AsynchronousSocketCloseMonitor helps implement Java's asynchronous Socket.close semantics.
 *
 * AsynchronousSocketCloseMonitor::init must be called before anything else.
 *
 * Every blocking network I/O operation must be surrounded by an AsynchronousSocketCloseMonitor
 * instance. For example:
 *
 *   {
 *     AsynchronousSocketCloseMonitor monitor(fd);
 *     byteCount = ::read(fd, buf, sizeof(buf));
 *   }
 *
 * To interrupt all threads currently blocked on file descriptor 'fd', call signalBlockedThreads:
 *
 *   AsynchronousSocketCloseMonitor::signalBlockedThreads(fd);
 */
class AsynchronousSocketCloseMonitor
{
public:
    AsynchronousSocketCloseMonitor(Int32 fd);
    ~AsynchronousSocketCloseMonitor();

    static void Init();

    static void SignalBlockedThreads(Int32 fd);

private:
    AsynchronousSocketCloseMonitor* mPrev;
    AsynchronousSocketCloseMonitor* mNext;
    pthread_t mThread;
    Int32 mFd;

    // Disallow copy and assignment.
    AsynchronousSocketCloseMonitor(const AsynchronousSocketCloseMonitor&);
    void operator=(const AsynchronousSocketCloseMonitor&);
};


#endif //__ASYNCHRONOUSSOCKETCLOSEMONITOR_H__
