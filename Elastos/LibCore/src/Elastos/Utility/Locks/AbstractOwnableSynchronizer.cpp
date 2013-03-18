#include "AbstractOwnableSynchronizer.h"


AbstractOwnableSynchronizer::AbstractOwnableSynchronizer()
{
}

void AbstractOwnableSynchronizer::SetExclusiveOwnerThread(
    /* [in] */ IThread* pThread)
{
    m_ExclusiveOwnerThread = pThread;
}

AutoPtr<IThread> AbstractOwnableSynchronizer::GetExclusiveOwnerThread()
{
    return m_ExclusiveOwnerThread;
}