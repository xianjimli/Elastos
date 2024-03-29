
#include "AsynchronousSocketCloseMonitor.h"

#include <elastos/Mutex.h>
#include <errno.h>
#include <signal.h>

using namespace Elastos::Core::Threading;

/**
 * We use an intrusive doubly-linked list to keep track of blocked threads.
 * This gives us O(1) insertion and removal, and means we don't need to do any allocation.
 * (The objects themselves are stack-allocated.)
 * Waking potentially-blocked threads when a socket is closed is O(n) in the total number of
 * blocked threads (not the number of threads actually blocked on the socket in question).
 * For now at least, this seems like a good compromise for Android.
 */
static Mutex blockedThreadListMutex;
static AsynchronousSocketCloseMonitor* blockedThreadList = NULL;

/**
 * The specific signal chosen here is arbitrary.
 */
static const Int32 BLOCKED_THREAD_SIGNAL = SIGRTMIN + 2;

static void blockedThreadSignalHandler(Int32 /*signal*/)
{
    // Do nothing. We only sent this signal for its side-effect of interrupting syscalls.
}

void AsynchronousSocketCloseMonitor::Init()
{
    // Ensure that the signal we send interrupts system calls but doesn't kill threads.
    // Using sigaction(2) lets us ensure that the SA_RESTART flag is not set.
    // (The whole reason we're sending this signal is to unblock system calls!)
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = blockedThreadSignalHandler;
    sa.sa_flags = 0;
    Int32 rc = sigaction(BLOCKED_THREAD_SIGNAL, &sa, NULL);
    if (rc == -1) {
//        LOGE("setting blocked thread signal handler failed: %s", strerror(errno));
    }
}

void AsynchronousSocketCloseMonitor::SignalBlockedThreads(Int32 fd)
{
    Mutex::Autolock lock(&blockedThreadListMutex);
    for (AsynchronousSocketCloseMonitor* it = blockedThreadList; it != NULL; it = it->mNext) {
        if (it->mFd == fd) {
            pthread_kill(it->mThread, BLOCKED_THREAD_SIGNAL);
            // Keep going, because there may be more than one thread...
        }
    }
}

AsynchronousSocketCloseMonitor::AsynchronousSocketCloseMonitor(Int32 fd)
{
    Mutex::Autolock lock(&blockedThreadListMutex);
    // Who are we, and what are we waiting for?
    mThread = pthread_self();
    mFd = fd;
    // Insert ourselves at the head of the intrusive doubly-linked list...
    mPrev = NULL;
    mNext = blockedThreadList;
    if (mNext != NULL) {
        mNext->mPrev = this;
    }
    blockedThreadList = this;
}

AsynchronousSocketCloseMonitor::~AsynchronousSocketCloseMonitor()
{
    Mutex::Autolock lock(&blockedThreadListMutex);
    // Unlink ourselves from the intrusive doubly-linked list...
    if (mNext != NULL) {
        mNext->mPrev = mPrev;
    }
    if (mPrev == NULL) {
        blockedThreadList = mNext;
    }
    else {
        mPrev->mNext = mNext;
    }
}
