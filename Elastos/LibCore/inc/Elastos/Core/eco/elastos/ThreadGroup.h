
#ifndef __THREADGROUP_H__
#define __THREADGROUP_H__

#ifdef ELASTOS_CORE_ECO
#include "Elastos.Core_server.h"
#else
#include <Elastos.Core.h>
#endif
#include <elastos/ElRefBase.h>

extern "C" const InterfaceID EIID_ThreadGroup;

class ThreadGroup : public ElRefBase
{
public:
    // BEGIN android-added
    /**
     * Non-standard method for adding a thread to a group, required by Dalvik.
     *
     * @param thread Thread to add
     *
     * @throws IllegalThreadStateException if the thread has been destroyed
     *         already
     *
     * @see #add(java.lang.Thread)
     * @see #removeThread(java.lang.Thread)
     */
    virtual CARAPI AddThread(
        /* [in] */ IThread* thread);

    /**
     * Non-standard method for adding a thread to a group, required by Dalvik.
     *
     * @param thread Thread to add
     *
     * @throws IllegalThreadStateException if the thread has been destroyed
     *         already
     *
     * @see #remove(java.lang.Thread)
     * @see #addThread(java.lang.Thread)
     */
    virtual CARAPI RemoveThread(
        /* [in] */ IThread* thread);
    // END android-added
};

#endif //__THREADGROUP_H__
