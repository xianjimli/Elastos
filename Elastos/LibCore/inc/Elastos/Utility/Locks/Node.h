#ifndef __NODE_H__
#define __NODE_H__

#include <elastos.h>
#include <elastos/Mutex.h>
#include "Elastos.Core.h"

using namespace Elastos;
using namespace Elastos::Core::Threading;

class Node {
public:
    Node();

    Node(
        /* [in] */ IThread* pThread,
        /* [in] */ Node* mode);

    Node(
        /* [in] */ IThread* pThread,
        /* [in] */ Int32 waitStatus);

    Boolean IsShared();

    Node* Predecessor();

public:
    static Node* m_SHARED;
    static Node* m_EXCLUSIVE;
    static Int32 m_CANCELLED;
    static Int32 m_SIGNAL;
    static Int32 m_CONDITION;
    static Int32 m_PROPAGATE;

    Int32 m_WaitStatus;

    Node* m_Prev;
    Node* m_Next;

    IThread* m_Thread;

    Node* m_NextWaiter;
};


#endif //__NODE__H__
