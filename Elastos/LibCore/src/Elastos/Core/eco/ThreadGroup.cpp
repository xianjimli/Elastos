
#include "ThreadGroup.h"
#include "Thread.h"
#include "cmdef.h"

// {8BF3F538-74DA-4997-B0DC-0F8086BA8835}
extern "C" const InterfaceID EIID_ThreadGroup =
        { 0x8bf3f538, 0x74da, 0x4997, { 0xb0, 0xdc, 0xf, 0x80, 0x86, 0xba, 0x88, 0x35 } };

ThreadGroup::ThreadGroup()
    : mNumThreads(0)
    , mNumGroups(0)
    , mMaxPriority(Thread_MAX_PRIORITY)
    , mIsDaemon(FALSE)
    , mIsDestroyed(FALSE)
{
    mChildrenThreads = ArrayOf<AutoPtr<IThread> >::Alloc(5);
    mChildrenGroups = ArrayOf<AutoPtr<ThreadGroup> >::Alloc(3);
}

ECode ThreadGroup::Init()
{
    mName = String("system");
    SetParent(NULL);
    return NOERROR;
}

ECode ThreadGroup::Init(
    /* [in] */ IThreadGroup* parent,
    /* [in] */ const String& name)
{
    //super();
    if (Thread::GetCurrentThread() != NULL) {
        // If parent is null we must throw NullPointerException, but that
        // will be done "for free" with the message send below
        parent->CheckAccess();
    }

    mName = name;
    SetParent((ThreadGroup*)parent->Probe(EIID_ThreadGroup));
    if (parent != NULL) {
        Int32 maxPriority;
        parent->GetMaxPriority(&maxPriority);
        SetMaxPriority(maxPriority);
        Boolean isDaemon;
        parent->IsDaemon(&isDaemon);
        if (isDaemon) {
            SetDaemon(TRUE);
        }
    }

    return NOERROR;
}

ECode ThreadGroup::ActiveCount(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    // BEGIN android-changed
    Int32 count = 0;
    // Lock the children thread list
    {
        Mutex::Autolock lock(mChildrenThreadsLock);
        for (Int32 i = 0; i < mNumThreads; i++) {
            Boolean isAlive;
            (*mChildrenThreads)[i]->IsAlive(&isAlive);
            if(isAlive) {
                count++;
            }
        }
    }

    // END android-changed
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 j = 0; j < mNumGroups; j++) {
            Int32 num;
            (*mChildrenGroups)[j]->ActiveCount(&num);
            count += num;
        }
    }

    *number = count;
    return NOERROR;
}

ECode ThreadGroup::ActiveGroupCount(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Int32 count = 0;
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = 0; i < mNumGroups; i++) {
            // One for this group & the subgroups
            Int32 num;
            (*mChildrenGroups)[i]->ActiveGroupCount(&num);
            count += 1 + num;
        }
    }

    *number = count;
    return NOERROR;
}

ECode ThreadGroup::AddThreadImpl(
    /* [in] */ IThread* thread)
{
    Mutex::Autolock lock(mChildrenThreadsLock);
    if (!mIsDestroyed) {
        Int32 len = mChildrenThreads->GetLength();
        if (len == mNumThreads) {
            ArrayOf<AutoPtr<IThread> >* newThreads = ArrayOf<AutoPtr<IThread> >::Alloc(len * 2);
            newThreads->Copy(mChildrenThreads->GetPayload(), mNumThreads);
            //System.arraycopy(childrenThreads, 0, newThreads, 0, numThreads);
            (*newThreads)[mNumThreads++] = thread;
            ArrayOf<AutoPtr<IThread> >::Free(mChildrenThreads);
            mChildrenThreads = newThreads;
        }
        else {
            (*mChildrenThreads)[mNumThreads++] = thread;
        }
    }
    else {
        return E_ILLEGAL_THREAD_STATE_EXCEPTION;
//        throw new IllegalThreadStateException();
    }

    return NOERROR;
}

ECode ThreadGroup::AddThreadGroup(
    /* [in] */ ThreadGroup* g)
{
    Mutex::Autolock lock(mChildrenGroupsLock);
    if (!mIsDestroyed) {
        Int32 len = mChildrenGroups->GetLength();
        if (len == mNumGroups) {
            ArrayOf<AutoPtr<ThreadGroup> >* newGroups = ArrayOf<AutoPtr<ThreadGroup> >::Alloc(len * 2);
            newGroups->Copy(mChildrenGroups->GetPayload(), mNumGroups);
            //System.arraycopy(childrenGroups, 0, newGroups, 0, numGroups);
            (*newGroups)[mNumGroups++] = g;
            ArrayOf<AutoPtr<ThreadGroup> >::Free(mChildrenGroups);
            mChildrenGroups = newGroups;
        } else {
            (*mChildrenGroups)[mNumGroups++] = g;
        }
    }
    else {
        return E_ILLEGAL_THREAD_STATE_EXCEPTION;
//        throw new IllegalThreadStateException();
    }

    return NOERROR;
}

ECode ThreadGroup::AllowThreadSuspension(
    /* [in] */ Boolean b,
    /* [out] */ Boolean* result)
{
    // Does not apply to this VM, no-op
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ThreadGroup::CheckAccess()
{
    // SecurityManager currentManager = System.getSecurityManager();
    // if (currentManager != null) {
    //     currentManager.checkAccess(this);
    // }

    return NOERROR;
}

ECode ThreadGroup::Destroy()
{
    FAIL_RETURN(CheckAccess());

    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenThreadsLock);
        {
            Mutex::Autolock lock(mChildrenGroupsLock);
            // BEGIN android-added
            if (mIsDestroyed) {
                return E_ILLEGAL_THREAD_STATE_EXCEPTION;
        //        throw new IllegalThreadStateException(
        //                "Thread group was already destroyed: "
        //                + (this.name != null ? this.name : "n/a"));
            }
            if (mNumThreads > 0) {
                return E_ILLEGAL_THREAD_STATE_EXCEPTION;
        //        throw new IllegalThreadStateException(
        //                "Thread group still contains threads: "
        //                + (this.name != null ? this.name : "n/a"));
            }
            // END android-added
            Int32 toDestroy = mNumGroups;
            // Call recursively for subgroups
            for (Int32 i = 0; i < toDestroy; i++) {
                // We always get the first element - remember, when the
                // child dies it removes itself from our collection. See
                // below.
                (*mChildrenGroups)[0]->Destroy();
            }

            if (mParent != NULL) {
                ThreadGroup* g = (ThreadGroup*)mParent->Probe(EIID_ThreadGroup);
                g->RemoveThreadGroup(this);
            }

            // Now that the ThreadGroup is really destroyed it can be tagged
            // as so
            mIsDestroyed = true;
        }
    }

    return NOERROR;
}

void ThreadGroup::DestroyIfEmptyDaemon()
{
    // Has to be non-destroyed daemon to make sense
    Mutex::Autolock lock(mChildrenThreadsLock);
    if (mIsDaemon && !mIsDestroyed && mNumThreads == 0) {
        Mutex::Autolock lock(mChildrenGroupsLock);
        if (mNumGroups == 0) {
            Destroy();
        }
    }
}

ECode ThreadGroup::EnumerateThread(
    /* [out] */ ArrayOf<IThread*>* threads,
    /* [out] */ Int32* number)
{
    return EnumerateThreadEx(TRUE, threads, number);
}

ECode ThreadGroup::EnumerateThreadEx(
    /* [in] */ Boolean recurse,
    /* [out] */ ArrayOf<IThread*>* threads,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = EnumerateThreadGeneric(recurse, 0, threads);
    return NOERROR;
}

ECode ThreadGroup::EnumerateThreadGroup(
    /* [out] */ ArrayOf<IThreadGroup*>* groups,
    /* [out] */ Int32* number)
{
    return EnumerateThreadGroupEx(TRUE, groups, number);
}

ECode ThreadGroup::EnumerateThreadGroupEx(
    /* [in] */ Boolean recurse,
    /* [out] */ ArrayOf<IThreadGroup*>* groups,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    * number = EnumerateThreadGroupGeneric(recurse, 0, groups);
    return NOERROR;
}

Int32 ThreadGroup::EnumerateThreadGeneric(
    /* [in] */ Boolean recurse,
    /* [in] */ Int32 enumerationIndex,
    /* [out] */ ArrayOf<IThread*>* enumeration)
{
    CheckAccess();

    Int32 len = enumeration->GetLength();
    {
        Mutex::Autolock lock(mChildrenThreadsLock); // Lock this subpart of the tree as we walk
        for (Int32 i = mNumThreads; --i >= 0;) {
            Boolean isAlive;
            (*mChildrenThreads)[i]->IsAlive(&isAlive);
            if (isAlive) {
                if (enumerationIndex >= len) {
                    return enumerationIndex;
                }
                (*enumeration)[enumerationIndex++] = (*mChildrenThreads)[i];
            }
        }
    }

    if (recurse) {  // Lock this subpart of the tree as we walk
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = 0; i < mNumGroups; i++) {
            if (enumerationIndex >= len) {
                return enumerationIndex;
            }
            enumerationIndex = (*mChildrenGroups)[i]->EnumerateThreadGeneric(
                    recurse, enumerationIndex, enumeration);
        }
    }

    return enumerationIndex;
}

Int32 ThreadGroup::EnumerateThreadGroupGeneric(
    /* [in] */ Boolean recurse,
    /* [in] */ Int32 enumerationIndex,
    /* [out] */ ArrayOf<IThreadGroup*>* enumeration)
{
    CheckAccess();

    Int32 len = enumeration->GetLength();
    {
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = mNumGroups; --i >= 0;) {
            if (enumerationIndex >= len) {
                return enumerationIndex;
            }
            (*enumeration)[enumerationIndex++] =
                    (IThreadGroup*)(*mChildrenGroups)[i]->Probe(EIID_IThreadGroup);
        }
    }

    if (recurse) {  // Lock this subpart of the tree as we walk
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = 0; i < mNumGroups; i++) {
            if (enumerationIndex >= len) {
                return enumerationIndex;
            }
            enumerationIndex = (*mChildrenGroups)[i]->EnumerateThreadGroupGeneric(
                    recurse, enumerationIndex, enumeration);
        }
    }

    return enumerationIndex;
}

ECode ThreadGroup::GetMaxPriority(
    /* [out] */ Int32* maxPriority)
{
    VALIDATE_NOT_NULL(maxPriority);
    *maxPriority = mMaxPriority;
    return NOERROR;
}

ECode ThreadGroup::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode ThreadGroup::GetParent(
    /* [out] */ IThreadGroup** parent)
{
    VALIDATE_NOT_NULL(parent);
    if (mParent != NULL) {
        mParent->CheckAccess();
    }
    *parent = mParent;
    return NOERROR;
}

ECode ThreadGroup::Interrupt()
{
    CheckAccess();
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenThreadsLock);
        for (Int32 i = 0; i < mNumThreads; i++) {
            (*mChildrenThreads)[i]->Interrupt();
        }
    }

    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = 0; i < mNumGroups; i++) {
            (*mChildrenGroups)[i]->Interrupt();
        }
    }

    return NOERROR;
}

ECode ThreadGroup::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    VALIDATE_NOT_NULL(isDaemon);
    *isDaemon = mIsDaemon;
    return NOERROR;
}

ECode ThreadGroup::IsDestroyed(
    /* [out] */ Boolean* isDestroyed)
{
    VALIDATE_NOT_NULL(isDestroyed);
    *isDestroyed = mIsDestroyed;
    return NOERROR;
}

ECode ThreadGroup::List()
{
    // We start in a fresh line
    //System.out.println();
    ListImpl(0);
    return NOERROR;
}

void ThreadGroup::ListImpl(
    /* [in] */ Int32 levels)
{
    // for (int i = 0; i < levels; i++) {
    //     System.out.print("    "); // 4 spaces for each level
    // }

    // Print the receiver
    //System.out.println(this.toString());

    // Print the children threads, with 1 extra indentation
    // synchronized (this.childrenThreadsLock) {
    //     for (int i = 0; i < numThreads; i++) {
    //         // children get an extra indentation, 4 spaces for each level
    //         for (int j = 0; j <= levels; j++) {
    //             System.out.print("    ");
    //         }
    //         System.out.println(this.childrenThreads[i]);
    //     }
    // }
    Mutex::Autolock lock(mChildrenGroupsLock);
    for (Int32 i = 0; i < mNumGroups; i++) {
        (*mChildrenGroups)[i]->ListImpl(levels + 1);
    }
}

ECode ThreadGroup::IsParentOf(
    /* [in] */ IThreadGroup* g,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    while (g != NULL) {
        if ((IThreadGroup*)this->Probe(EIID_IThreadGroup) == g) {
            *result = TRUE;
            return NOERROR;
        }
        g = ((ThreadGroup*)g->Probe(EIID_ThreadGroup))->mParent;
    }
    *result = FALSE;
    return NOERROR;
}

ECode ThreadGroup::RemoveThreadImpl(
    /* [in] */ IThread* thread)
{
    {
        Mutex::Autolock lock(mChildrenThreadsLock);
        for (Int32 i = 0; i < mNumThreads; i++) {
            if ((*mChildrenThreads)[i].Get() == thread) {
                mNumThreads--;
                for (Int32 j = 1; j < mNumThreads - i; ++j) {
                    (*mChildrenThreads)[i] = (*mChildrenThreads)[i + j];
                }
                //System.arraycopy(childrenThreads, i + 1, childrenThreads, i, numThreads
                //                - i);
                (*mChildrenThreads)[mNumThreads] = NULL;
                break;
            }
        }
    }

    DestroyIfEmptyDaemon();
    return NOERROR;
}

void ThreadGroup::RemoveThreadGroup(
        /* [in] */ ThreadGroup* g)
{
    {
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = 0; i < mNumGroups; i++) {
            if ((*mChildrenGroups)[i].Get() == g) {
                mNumGroups--;
                for (Int32 j = 1; j < mNumGroups - i; ++j) {
                    (*mChildrenGroups)[i] = (*mChildrenGroups)[i + j];
                }
                //System.arraycopy(childrenGroups, i + 1, childrenGroups, i, numGroups - i);
                (*mChildrenGroups)[mNumGroups] = NULL;
                break;
            }
        }
    }

    DestroyIfEmptyDaemon();
}

ECode ThreadGroup::Resume()
{
    CheckAccess();
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenThreadsLock);
        for (Int32 i = 0; i < mNumThreads; i++) {
            (*mChildrenThreads)[i]->Resume();
        }
    }
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = 0; i < mNumGroups; i++) {
            (*mChildrenGroups)[i]->Resume();
        }
    }

    return NOERROR;
}

ECode ThreadGroup::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    CheckAccess();
    mIsDaemon = isDaemon;
    return NOERROR;
}

ECode ThreadGroup::SetMaxPriority(
    /* [in] */ Int32 newMax)
{
    CheckAccess();

    if (newMax <= mMaxPriority) {
        if (newMax < Thread_MIN_PRIORITY) {
            newMax = Thread_MIN_PRIORITY;
        }

        Int32 parentMaxPriority;
        mParent->GetMaxPriority(&parentMaxPriority);
        Int32 parentPriority = mParent == NULL ? newMax : parentMaxPriority;
        mMaxPriority = parentPriority <= newMax ? parentPriority : newMax;
        // Lock this subpart of the tree as we walk
        Mutex::Autolock lock(mChildrenGroupsLock);
        // ??? why not maxPriority
        for (Int32 i = 0; i < mNumGroups; i++) {
            (*mChildrenGroups)[i]->SetMaxPriority(newMax);
        }
    }

    return NOERROR;
}

void ThreadGroup::SetParent(
    /* [in] */ ThreadGroup* parent)
{
    if (parent != NULL) {
        parent->AddThreadGroup(this);
    }
    mParent = (IThreadGroup*)parent->Probe(EIID_IThreadGroup);
}

ECode ThreadGroup::Stop()
{
    if (StopHelper()) {
        return Thread::GetCurrentThread()->Stop();
    }

    return NOERROR;
}

Boolean ThreadGroup::StopHelper()
{
    CheckAccess();

    Boolean stopCurrent = FALSE;
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenThreadsLock);
        AutoPtr<IThread> current = Thread::GetCurrentThread();
        for (Int32 i = 0; i < mNumThreads; i++) {
            if ((*mChildrenThreads)[i].Get() == current) {
                stopCurrent = TRUE;
            }
            else {
                (*mChildrenThreads)[i]->Stop();
            }
        }
    }
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = 0; i < mNumGroups; i++) {
            stopCurrent |= (*mChildrenGroups)[i]->StopHelper();
        }
    }
    return stopCurrent;
}

ECode ThreadGroup::Suspend()
{
    if (SuspendHelper()) {
        return Thread::GetCurrentThread()->Suspend();
    }

    return NOERROR;
}

Boolean ThreadGroup::SuspendHelper()
{
    CheckAccess();

    Boolean suspendCurrent = FALSE;
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenThreadsLock);
        AutoPtr<IThread> current = Thread::GetCurrentThread();
        for (Int32 i = 0; i < mNumThreads; i++) {
            if ((*mChildrenThreads)[i].Get() == current) {
                suspendCurrent = TRUE;
            }
            else {
                (*mChildrenThreads)[i]->Suspend();
            }
        }
    }
    // Lock this subpart of the tree as we walk
    {
        Mutex::Autolock lock(mChildrenGroupsLock);
        for (Int32 i = 0; i < mNumGroups; i++) {
            suspendCurrent |= (*mChildrenGroups)[i]->SuspendHelper();
        }
    }

    return suspendCurrent;
}

ECode ThreadGroup::AddThread(
    /* [in] */ IThread* thread)
{
    return AddThreadImpl(thread);
}

ECode ThreadGroup::RemoveThread(
    /* [in] */ IThread* thread)
{
    return RemoveThreadImpl(thread);
}
