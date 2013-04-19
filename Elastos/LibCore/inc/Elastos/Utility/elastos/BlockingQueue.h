
#ifndef __BLOCKINGQUEUE_H__
#define __BLOCKINGQUEUE_H__

#include <elastos/Queue.h>
#include <pthread.h>


_ELASTOS_NAMESPACE_BEGIN

template <class Tp,
          class Sequence = Deque<Tp> >
class Queue;

template <class Tp, class Sequence>
class BlockingQueue
{

private:
    pthread_mutex_t mWaitMutex;
    pthread_cond_t mWaitCond;
};


_ELASTOS_NAMESPACE_END

#endif //__BLOCKINGQUEUE_H__
