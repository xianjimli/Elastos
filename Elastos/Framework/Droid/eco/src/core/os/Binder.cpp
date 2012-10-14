
#include <binder/IPCThreadState.h>
#include "os/Binder.h"


/**
 * Return the ID of the process that sent you the current transaction
 * that is being processed.  This pid can be used with higher-level
 * system services to determine its identity and check permissions.
 * If the current thread is not currently executing an incoming transaction,
 * then its own pid is returned.
 */
Int32 Binder::GetCallingPid()
{
    return android::IPCThreadState::self()->getCallingPid();
}

/**
 * Return the ID of the user assigned to the process that sent you the
 * current transaction that is being processed.  This uid can be used with
 * higher-level system services to determine its identity and check
 * permissions.  If the current thread is not currently executing an
 * incoming transaction, then its own uid is returned.
 */
Int32 Binder::GetCallingUid()
{
    return android::IPCThreadState::self()->getCallingUid();
}

/**
 * Reset the identity of the incoming IPC on the current thread.  This can
 * be useful if, while handling an incoming call, you will be calling
 * on interfaces of other objects that may be local to your process and
 * need to do permission checks on the calls coming into them (so they
 * will check the permission of your own local process, and not whatever
 * process originally called you).
 *
 * @return Returns an opaque token that can be used to restore the
 * original calling identity by passing it to
 * {@link #restoreCallingIdentity(long)}.
 *
 * @see #getCallingPid()
 * @see #getCallingUid()
 * @see #restoreCallingIdentity(long)
 */
Int64 Binder::ClearCallingIdentity()
{
    return android::IPCThreadState::self()->clearCallingIdentity();
}

/**
 * Restore the identity of the incoming IPC on the current thread
 * back to a previously identity that was returned by {@link
 * #clearCallingIdentity}.
 *
 * @param token The opaque token that was previously returned by
 * {@link #clearCallingIdentity}.
 *
 * @see #clearCallingIdentity
 */
void Binder::RestoreCallingIdentity(
    /* [in] */ Int64 token)
{
    // XXX temporary sanity check to debug crashes.
    Int32 uid = (Int32)(token >> 32);
    if (uid > 0 && uid < 999) {
        // In Android currently there are no uids in this range.
//        char buf[128];
//        sprintf(buf, "Restoring bad calling ident: 0x%Lx", token);
//        jniThrowException(env, "java/lang/IllegalStateException", buf);
        return;
    }
    android::IPCThreadState::self()->restoreCallingIdentity(token);
}
