#include "Node.h"

Node* Node::m_SHARED = new Node();

Node* Node::m_EXCLUSIVE = NULL;

        /** waitStatus value to indicate thread has cancelled */
Int32 Node::m_CANCELLED =  1;
        /** waitStatus value to indicate successor's thread needs unparking */
Int32 Node::m_SIGNAL    = -1;
        /** waitStatus value to indicate thread is waiting on condition */
Int32 Node::m_CONDITION = -2;
        /**
         * waitStatus value to indicate the next acquireShared should
         * unconditionally propagate
         */
Int32 Node::m_PROPAGATE = -3;


Node::Node()
{
}

Node::Node(
            /* [in] */ IThread* pThread,
            /* [in] */ Node* mode)
{
    this->m_NextWaiter = mode;
    this->m_Thread = pThread;
}

Node::Node(
            /* [in] */ IThread* pThread,
            /* [in] */ Int32 waitStatus)
{
    this->m_WaitStatus = waitStatus;
    this->m_Thread = pThread;
}

Boolean Node::IsShared()
{
    return m_NextWaiter == m_SHARED;
}

Node* Node::Predecessor()
{
    Node* p = m_Prev;
    if (p == NULL)
        return NULL;
    else
        return p;
}

