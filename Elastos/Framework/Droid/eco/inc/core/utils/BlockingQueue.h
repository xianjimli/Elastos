
#ifndef __BLOCKINGQUEUE_H__
#define __BLOCKINGQUEUE_H__
#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif
#include <elastos/Queue.h>
#include <pthread.h>
#include <semaphore.h>

_ELASTOS_NAMESPACE_BEGIN

template <class Tp,
          class Sequence = Queue<Tp> >
class BlockingQueue;

template <class Tp, class Seq>
inline Boolean operator==(const BlockingQueue<Tp, Seq>&, const BlockingQueue<Tp, Seq>&);

template <class Tp, class Seq>
inline Boolean operator<(const BlockingQueue<Tp, Seq>&, const BlockingQueue<Tp, Seq>&);


template <class Tp, class Sequence>
class BlockingQueue
{
private:
    pthread_mutex_t mMutex;
    sem_t mSem;

    template <class Tp1, class Seq1>
    friend Boolean operator== (const BlockingQueue<Tp1, Seq1>&,
                            const BlockingQueue<Tp1, Seq1>&);
    template <class Tp1, class Seq1>
    friend Boolean operator< (const BlockingQueue<Tp1, Seq1>&,
                           const BlockingQueue<Tp1, Seq1>&);
protected:
    Sequence mQ;
public:
    explicit BlockingQueue(const Sequence& q = Sequence()) : mQ(q){
        sem_init(&mSem, 0, 0);
    }
    ~BlockingQueue() { sem_destroy(&mSem);  }

    void Put(Tp const& value){
	pthread_mutex_lock(&mMutex);
        mQ.Push(value);
      	pthread_mutex_unlock(&mMutex);
       	sem_post(&mSem);
    }

    Tp Take(){
	sem_wait(&mSem); //sem_timedwait()
	pthread_mutex_lock(&mMutex);
        Tp rc = mQ.GetFront();
        mQ.Pop();
        pthread_mutex_unlock(&mMutex);
        return rc;
    }

};

template <class Tp, class Sequence>
Boolean
operator==(const BlockingQueue<Tp, Sequence>& x, const BlockingQueue<Tp, Sequence>& y)
{
    return x.mQ == y.mQ;
}

template <class Tp, class Sequence>
Boolean
operator<(const BlockingQueue<Tp, Sequence>& x, const BlockingQueue<Tp, Sequence>& y)
{
    return x.mQ < y.mQ;
}

_ELASTOS_NAMESPACE_END

#endif //__BLOCKINGQUEUE_H__
