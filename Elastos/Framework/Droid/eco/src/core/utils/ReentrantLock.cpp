
#include "utils/ReentrantLock.h"

/**
 * Acquires the lock.
 *
 * <p>Acquires the lock if it is not held by another thread and returns
 * immediately, setting the lock hold count to one.
 *
 * <p>If the current thread already holds the lock then the hold
 * count is incremented by one and the method returns immediately.
 *
 * <p>If the lock is held by another thread then the
 * current thread becomes disabled for thread scheduling
 * purposes and lies dormant until the lock has been acquired,
 * at which time the lock hold count is set to one.
 */
void ReentrantLock::Lock()
{}

/**
 * Attempts to release this lock.
 *
 * <p>If the current thread is the holder of this lock then the hold
 * count is decremented.  If the hold count is now zero then the lock
 * is released.  If the current thread is not the holder of this
 * lock then {@link IllegalMonitorStateException} is thrown.
 *
 * @throws IllegalMonitorStateException if the current thread does not
 *         hold this lock
 */
void ReentrantLock::Unlock()
{}

/**
 * Queries the number of holds on this lock by the current thread.
 *
 * <p>A thread has a hold on a lock for each lock action that is not
 * matched by an unlock action.
 *
 * <p>The hold count information is typically only used for testing and
 * debugging purposes. For example, if a certain section of code should
 * not be entered with the lock already held then we can assert that
 * fact:
 *
 * <pre>
 * class X {
 *   ReentrantLock lock = new ReentrantLock();
 *   // ...
 *   public void m() {
 *     assert lock.getHoldCount() == 0;
 *     lock.lock();
 *     try {
 *       // ... method body
 *     } finally {
 *       lock.unlock();
 *     }
 *   }
 * }
 * </pre>
 *
 * @return the number of holds on this lock by the current thread,
 *         or zero if this lock is not held by the current thread
 */
Int32 ReentrantLock::GetHoldCount()
{}

/**
 * Queries if this lock is held by the current thread.
 *
 * <p>Analogous to the {@link Thread#holdsLock} method for built-in
 * monitor locks, this method is typically used for debugging and
 * testing. For example, a method that should only be called while
 * a lock is held can assert that this is the case:
 *
 * <pre>
 * class X {
 *   ReentrantLock lock = new ReentrantLock();
 *   // ...
 *
 *   public void m() {
 *       assert lock.isHeldByCurrentThread();
 *       // ... method body
 *   }
 * }
 * </pre>
 *
 * <p>It can also be used to ensure that a reentrant lock is used
 * in a non-reentrant manner, for example:
 *
 * <pre>
 * class X {
 *   ReentrantLock lock = new ReentrantLock();
 *   // ...
 *
 *   public void m() {
 *       assert !lock.isHeldByCurrentThread();
 *       lock.lock();
 *       try {
 *           // ... method body
 *       } finally {
 *           lock.unlock();
 *       }
 *   }
 * }
 * </pre>
 *
 * @return {@code true} if current thread holds this lock and
 *         {@code false} otherwise
 */
Boolean ReentrantLock::IsHeldByCurrentThread()
{}

/**
 * Queries if this lock is held by any thread. This method is
 * designed for use in monitoring of the system state,
 * not for synchronization control.
 *
 * @return {@code true} if any thread holds this lock and
 *         {@code false} otherwise
 */
Boolean ReentrantLock::IsLocked()
{}

/**
 * Returns an estimate of the number of threads waiting to
 * acquire this lock.  The value is only an estimate because the number of
 * threads may change dynamically while this method traverses
 * internal data structures.  This method is designed for use in
 * monitoring of the system state, not for synchronization
 * control.
 *
 * @return the estimated number of threads waiting for this lock
 */
Int32 ReentrantLock::GetQueueLength()
{}
