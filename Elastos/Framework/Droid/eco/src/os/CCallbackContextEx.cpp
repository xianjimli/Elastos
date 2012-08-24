//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "os/CCallbackContextEx.h"
#include "os/SystemClock.h"
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define MillisecPerSec 1000
#define MicrosecPerSec 1000000

#define ForEachDLinkNode(t, p, h) \
        for (p = (t)((h)->m_pNext); p != (t)h; p = (t)(p->m_pNext))

#define ForEachDLinkNodeReversely(t, p, h) \
        for (p = (t)((h)->m_pPrev); p != (t)h; p = (t)(p->m_pPrev))

EXTERN_C Int32 CDECL printf(const char *fmt,...);
EXTERN_C Int32 invokeCoalescer(void* pFunc, void* pOldParam, void* pNewParam, int nSize);
EXTERN_C Int32 invokeCallback(CallbackEventFlags cFlags, void* pSender, void* pThis, void* pFunc, void* pParam, int size);

ELAPI_(PCallbackEvent) _Impl_CallbackSink_AllocCallbackEvent(MemorySize size)
{
    // TODO: optimize
    return (PCallbackEvent)malloc(size);
}

ELAPI_(void) _Impl_CallbackSink_FreeCallbackEvent(PVoid pCallbackEvent)
{
    // TODO: optimize

    ((PCallbackEvent)pCallbackEvent)->~_EzCallbackEvent();
    free(pCallbackEvent);
}

//
//  class CCallbackContextEx
//
UInt32 CCallbackContextEx::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCallbackContextEx::Release()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is decreased
    Int32 ref = atomic_dec(&mRef);
    // so we should decrease ref
    if (--ref == 0) {
        assert(m_currentEvents.IsEmpty());
        delete this;
    }
    assert(ref >= 0);
    return ref;
}

PInterface CCallbackContextEx::Probe(REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    return NULL;
}

ECode CCallbackContextEx::GetInterfaceID(
    IInterface *pObject,
    InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

void CCallbackContextEx::PushEvent(PCallbackEvent pEvent)
{
    pthread_mutex_lock(&m_queueLock);
    m_currentEvents.InsertNext(pEvent);
    pthread_mutex_unlock(&m_queueLock);
}

PCallbackEvent CCallbackContextEx::PopEvent()
{
    pthread_mutex_lock(&m_queueLock);
    PCallbackEvent pCallbackEvent = m_currentEvents.First();
    pCallbackEvent->Detach();
    pthread_mutex_unlock(&m_queueLock);
    return pCallbackEvent;
}

Boolean CCallbackContextEx::HasCallbackEvent(
    PInterface pSender,
    CallbackEventId id,
    PVoid pHandlerThis,
    PVoid pHandlerFunc)
{
    PCallbackEvent pCurtEvent;
    pthread_mutex_lock(&m_queueLock);
    ForEachDLinkNode(PCallbackEvent, pCurtEvent, &m_eventQueue) {
        if ((pSender == NULL || pSender == pCurtEvent->m_pSender)
            && (id == (CallbackEventId)0 || id == pCurtEvent->m_id)
            && (pHandlerThis == (PVoid)NULL || pHandlerThis == pCurtEvent->m_pHandlerThis)
            && (pHandlerFunc == (PVoid)NULL || pHandlerFunc == pCurtEvent->m_pHandlerFunc)) {
            pthread_mutex_unlock(&m_queueLock);
            return TRUE;
        }
    }

    pthread_mutex_unlock(&m_queueLock);
    return FALSE;
}

ECode CCallbackContextEx::PostCallbackEvent(PCallbackEvent pCallbackEvent)
{
    assert(pCallbackEvent);

    pthread_mutex_lock(&m_queueLock);

    pCallbackEvent->AddRef();
    if (m_bExitRequested ||
            m_bRequestToQuit ||
            m_Status >= CallbackContextStatus_Finishing) {

        pthread_mutex_unlock(&m_queueLock);

        pCallbackEvent->m_Status = CallingStatus_Cancelled;
        pCallbackEvent->m_bCompleted = TRUE;
        sem_post(&pCallbackEvent->m_pSyncEvent);
        pCallbackEvent->Release();

        return NOERROR;
    }

    Boolean bNeedNotify = m_eventQueue.IsEmpty();

    ECode ec;
    PCallbackEvent pPrevCBEvent;
    PCallbackEvent pCancelingCBEvent;

    if (pCallbackEvent->m_flags & CallbackEventFlag_Duplicated) {
        CallbackEventFlags highestFlag =
            pCallbackEvent->m_flags & CallbackEventFlag_PriorityMask;
        ForEachDLinkNodeReversely(PCallbackEvent, pPrevCBEvent, &m_eventQueue) {
            if (pCallbackEvent->m_id == pPrevCBEvent->m_id
              && pCallbackEvent->m_pSender == pPrevCBEvent->m_pSender
              && pCallbackEvent->m_pHandlerThis == pPrevCBEvent->m_pHandlerThis
              && pCallbackEvent->m_pHandlerFunc == pPrevCBEvent->m_pHandlerFunc) {
                PVoid pUserFunc = pCallbackEvent->m_pCoalesceFunc;
                if (pUserFunc) {
                    Int32 nSize = 0;
                    Byte *pNewBuf = NULL;
                    Byte *pOldBuf = NULL;

                    if ((NULL != pCallbackEvent->m_pParameters)
                        && (NULL != pPrevCBEvent->m_pParameters)) {
                        pCallbackEvent->m_pParameters->GetElementSize(&nSize);
                        pCallbackEvent->m_pParameters->GetElementPayload((Handle32*)&pNewBuf);
                        pPrevCBEvent->m_pParameters->GetElementPayload((Handle32*)&pOldBuf);
                    }

                    ec = invokeCoalescer(pUserFunc, pOldBuf, pNewBuf, nSize);
                    if (FAILED(ec)) {
                        if (ec == E_CANCLE_BOTH_EVENTS) {
                            pCallbackEvent->Release();
                            pPrevCBEvent->Detach();
                            pPrevCBEvent->Release();
                            m_nEventsCount--;
                            bNeedNotify = FALSE;
                            goto Next;
                        }
                        continue;
                    }
                }
                highestFlag =
                    pPrevCBEvent->m_flags & CallbackEventFlag_PriorityMask;
                pCancelingCBEvent = pPrevCBEvent;
                pPrevCBEvent = (PCallbackEvent)pPrevCBEvent->Next();
                pCancelingCBEvent->Detach();
                m_nEventsCount--;
                pCancelingCBEvent->Release();
            }
        }
        if ((pCallbackEvent->m_flags & CallbackEventFlag_PriorityMask)
                > highestFlag) {
            pCallbackEvent->m_flags &= ~CallbackEventFlag_PriorityMask;
            pCallbackEvent->m_flags |= highestFlag;
        }
    }

    ForEachDLinkNodeReversely(PCallbackEvent, pPrevCBEvent, &m_eventQueue) {
        CallbackEventFlags prevPri = pPrevCBEvent->m_flags & CallbackEventFlag_PriorityMask;
        CallbackEventFlags currPri = pCallbackEvent->m_flags & CallbackEventFlag_PriorityMask;
        if ((prevPri < currPri) || ((prevPri == currPri) &&
            (pPrevCBEvent->m_when <= pCallbackEvent->m_when))) {
            pPrevCBEvent->InsertNext(pCallbackEvent);
            m_nEventsCount++;
            goto Next;
        }
    }

    m_eventQueue.InsertFirst(pCallbackEvent);
    m_nEventsCount++;

Next:
    pthread_mutex_unlock(&m_queueLock);

    if (bNeedNotify) {
        sem_post(&m_pThreadEvent);
    }

    return NOERROR;
}

ECode CCallbackContextEx::SendCallbackEvent(
    PCallbackEvent pCallbackEvent,
    Millisecond32 timeOut)
{
    ECode _ec_;
    Int32 CCStatus = 0;
    WaitResult _wr_ = WaitResult_OK;
    PInterface _pOrgCallbackContext_;
    Boolean bCancelled = FALSE;
    struct timespec ts;

    if (INFINITE != timeOut) {
        ts.tv_sec = ts.tv_nsec = 0;
        clock_gettime(CLOCK_MONOTONIC, &ts);
    }

    _pOrgCallbackContext_ = (PInterface)pthread_getspecific(TL_ORG_CALLBACK_SLOT);

    if (_pOrgCallbackContext_) {
        CCStatus = _Impl_CallbackSink_GetStatus(_pOrgCallbackContext_);
    }

    if (_pOrgCallbackContext_ && CCStatus > CallbackContextStatus_Created) {
        _ec_ = _Impl_CallbackSink_GetThreadEvent(
                        _pOrgCallbackContext_, &pCallbackEvent->m_pSyncEvent);
        if (NOERROR == _ec_) {
            _ec_ = _Impl_CallbackSink_PostCallbackEvent(
                            this, pCallbackEvent);
            if (NOERROR == _ec_) {
                _ec_ = _Impl_CallbackSink_WaitForCallbackEvent(
                            _pOrgCallbackContext_, timeOut, &_wr_,
                            (Boolean*)&pCallbackEvent->m_bCompleted,
                            CallbackEventFlag_SyncCall);
                if (NOERROR == _ec_) {
                    if (WaitResult_OK == _wr_) _ec_ = NOERROR;
                    else if (WaitResult_Interrupted == _wr_) {
                        _ec_ = E_INTERRUPTED;
                        bCancelled = TRUE;
                    }
                    else if (WaitResult_TimedOut == _wr_) {
                        _ec_= E_TIMED_OUT;
                        bCancelled = TRUE;
                    }
                }
            }
        }
    }
    else {
        _ec_ = sem_init(&pCallbackEvent->m_pSyncEvent, 0, 0);
        if (NOERROR == _ec_) {
            _ec_ = _Impl_CallbackSink_PostCallbackEvent(this, pCallbackEvent);
            if (NOERROR == _ec_) {
                if (INFINITE == timeOut) {
                    _ec_ = sem_wait(&pCallbackEvent->m_pSyncEvent);
                } else {
                    _ec_ = sem_timedwait(&pCallbackEvent->m_pSyncEvent, &ts);
                }

                if ( _ec_ == -1) {
                    if (errno == ETIMEDOUT) {
                        _ec_= E_TIMED_OUT;
                        bCancelled = TRUE;
                    } else if (errno == EINTR) {
                        _ec_ = E_INTERRUPTED;
                        bCancelled = TRUE;
                    }
                } else {
                    _ec_ = NOERROR;
                }
            }
        }
    }

    if (pCallbackEvent->m_Status == CallingStatus_Cancelled) {
        _ec_ = E_CALLBACK_CANCELED;
    }

    if (bCancelled) {
        pCallbackEvent->m_Status = CallingStatus_Cancelled;
    }

    return _ec_;
}

ECode CCallbackContextEx::RequestToFinish(Int32 flag)
{
    ThreadEvent eventType = ThreadEvent_RequestToQuit;
    if (flag ==CallbackContextFinish_Nice) {
        m_bRequestToQuit = TRUE;
        eventType = ThreadEvent_RequestToQuit;
    }
    else if (flag == CallbackContextFinish_ASAP) {
        m_bRequestToQuit = FALSE;
        eventType = ThreadEvent_ForceToQuit;
    }
    else return E_NOT_IMPLEMENTED;

    m_bExitRequested = TRUE;
    sem_post(&m_pThreadEvent);

    return NOERROR;
}

ECode CCallbackContextEx::CancelAllCallbackEvents(
    PInterface pSender)
{
    register PCallbackEvent pCBEvent;
    PCallbackEvent pCancelingCBEvent;

    pthread_mutex_lock(&m_queueLock);
    ForEachDLinkNode(PCallbackEvent, pCBEvent, &m_eventQueue) {
        if (pSender == NULL || pCBEvent->m_pSender == pSender) {
            pCancelingCBEvent = pCBEvent;
            pCBEvent = (PCallbackEvent)pCancelingCBEvent->Prev();
            pCancelingCBEvent->Detach();
            --m_nEventsCount;

            pCancelingCBEvent->m_Status = CallingStatus_Cancelled;
            pCancelingCBEvent->m_bCompleted = TRUE;
            sem_post(&pCancelingCBEvent->m_pSyncEvent);
            pCancelingCBEvent->Release();
        }
    }

    pthread_mutex_unlock(&m_queueLock);
    return NOERROR;
}

ECode CCallbackContextEx::CancelCallbackEvents(
    PInterface pSender,
    CallbackEventId id,
    PVoid pHandlerThis,
    PVoid pHandlerFunc)
{
    register PCallbackEvent pCBEvent;
    PCallbackEvent pCancelingCBEvent;

    pthread_mutex_lock(&m_queueLock);
    ForEachDLinkNode(PCallbackEvent, pCBEvent, &m_eventQueue) {
        if ((pSender == NULL || pSender == pCBEvent->m_pSender)
            && (id == (CallbackEventId)0 || id == pCBEvent->m_id)
            && (pHandlerThis == NULL || pHandlerThis == pCBEvent->m_pHandlerThis)
            && (pHandlerFunc == NULL || pHandlerFunc == pCBEvent->m_pHandlerFunc)) {
            pCancelingCBEvent = pCBEvent;
            pCBEvent = (PCallbackEvent)pCancelingCBEvent->Prev();
            pCancelingCBEvent->Detach();
            --m_nEventsCount;

            pCancelingCBEvent->m_Status = CallingStatus_Cancelled;
            pCancelingCBEvent->m_bCompleted = TRUE;
            sem_post(&pCancelingCBEvent->m_pSyncEvent);
            pCancelingCBEvent->Release();
        }
    }

    pthread_mutex_unlock(&m_queueLock);
    return NOERROR;
}

PCallbackEvent CCallbackContextEx::GetEvent(Flags32 fPriority)
{
    Int64 now = SystemClock::GetUptimeMillis();

    PCallbackEvent pCallbackEvent = m_eventQueue.First();
    if (fPriority) {
        while (pCallbackEvent != &m_eventQueue) {
            if ((fPriority & CallbackEventFlag_PriorityMask) <
                ((Flags32)pCallbackEvent->m_flags & CallbackEventFlag_PriorityMask)) {
                if ((fPriority & ~CallbackEventFlag_PriorityMask)==0
                    || ((fPriority & ~CallbackEventFlag_PriorityMask)
                            & pCallbackEvent->m_flags)){
                    if (now < pCallbackEvent->m_when) {
                        usleep((pCallbackEvent->m_when - now) *
                                (MicrosecPerSec / MillisecPerSec));
                    }

                    pCallbackEvent->Detach();
                    return pCallbackEvent;
                }
            }

            pCallbackEvent = pCallbackEvent->Next();
        }
    }
    else {
        if (now < pCallbackEvent->m_when) {
            usleep((pCallbackEvent->m_when - now) *
                    (MicrosecPerSec / MillisecPerSec));
        }

        pCallbackEvent->Detach();
        return pCallbackEvent;
    }

    return NULL;
}

Int32 CCallbackContextEx::HandleCallbackEvents(
    Millisecond32 msTimeOut,
    WaitResult *pResult,
    Boolean* pbOccured,
    Flags32 fPriority)
{
    Boolean bEmpty = FALSE;
    PCallbackEvent pCallbackEvent = NULL;
    IObject* pClientObj = NULL;
    PVoid pLock = NULL;
    struct timespec ts;

    Millisecond32 msBegPoint = clock();
    Millisecond32 msLapsedTime = 0;

    PInterface pCallbackContext;
    pCallbackContext = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
    this->AddRef();
    pthread_setspecific(TL_CALLBACK_SLOT, (PInterface)this);

    if (INFINITE != msTimeOut) {
        ts.tv_sec = ts.tv_nsec = 0;
        clock_gettime(CLOCK_MONOTONIC, &ts);
    }

    while (TRUE) {
        pthread_mutex_lock(&m_queueLock);

        if (m_bExitRequested) {
            if (!m_bRequestToQuit) {
                pthread_mutex_unlock(&m_queueLock);
                if (pResult) *pResult = WaitResult_Interrupted;
                goto Exit;
            }
        }
        if (m_eventQueue.IsEmpty()) {
            bEmpty = TRUE;
        }
        else {
            pCallbackEvent = GetEvent(fPriority);
            if (pCallbackEvent == NULL) {
                bEmpty = TRUE;
            }
        }
        if (bEmpty) {
            bEmpty = FALSE;
            if (m_bRequestToQuit) {
                pthread_mutex_unlock(&m_queueLock);
                if (pResult) *pResult = WaitResult_Interrupted;
                goto Exit;
            }
            m_Status = CallbackContextStatus_Idling;
            pthread_mutex_unlock(&m_queueLock);
            if (INFINITE == msTimeOut) {
                sem_wait(&m_pThreadEvent);
            } else {
                sem_timedwait(&m_pThreadEvent, &ts);
            }

            if (pbOccured) {
                if (*pbOccured) {
                    if (pResult) *pResult = WaitResult_OK;
                    *pbOccured = FALSE;
                    goto Exit;
                }
            }
        }
        else {
            --m_nEventsCount;
            assert(pCallbackEvent);
            PushEvent(pCallbackEvent);
            pthread_mutex_lock(&m_workingLock);
            m_Status = CallbackContextStatus_Working;
            pthread_mutex_unlock(&m_queueLock);

            if (NULL != pCallbackEvent->m_pHandlerFunc) {
                pClientObj = NULL;
                if (CallbackType_CAR == pCallbackEvent->m_typeOfFunc) {
                    pClientObj = (IObject*)pCallbackEvent->m_pReceiver->Probe(EIID_IObject);
                    assert(pClientObj);
                    if (pClientObj->AddRef() == 1) goto Cancel;
                    pClientObj->Aggregate(AggrType_FriendEnter, (PInterface)&pLock);
                }

                if (pCallbackEvent->m_flags & CallbackEventFlag_SyncCall) {
                    if (CallingStatus_Cancelled == pCallbackEvent->m_Status) {
                        pCallbackEvent->m_bCompleted = TRUE;
                        sem_post(&pCallbackEvent->m_pSyncEvent);
                        goto CallingCompleted;
                    }
                    pCallbackEvent->m_Status = CallingStatus_Running;
                }

                if (pCallbackEvent->m_flags & CallbackEventFlag_DirectCall) {
                        _Impl_SetHelperInfoFlag(HELPER_ASYNC_CALLING, TRUE);
                }
                {
                    Int32 nSize = 0;
                    Byte* pBuf = NULL;

                    if (pCallbackEvent->m_pParameters != NULL) {
                        pCallbackEvent->m_pParameters->GetElementPayload((Handle32*)&pBuf);
                        pCallbackEvent->m_pParameters->GetElementSize(&nSize);
                    }

                    CallbackEventFlags cFlags = pCallbackEvent->m_flags;
                    PVoid pSender = pCallbackEvent->m_pSender;
                    PVoid pThis = pCallbackEvent->m_pHandlerThis;
                    PVoid pFunc = pCallbackEvent->m_pHandlerFunc;

                    pCallbackEvent->m_ecRet = invokeCallback(cFlags, pSender, pThis, pFunc, pBuf, nSize);
                }
                if (pCallbackEvent->m_flags & CallbackEventFlag_DirectCall) {
                    _Impl_SetHelperInfoFlag(HELPER_ASYNC_CALLING, FALSE);
                }

                if (pCallbackEvent->m_flags & CallbackEventFlag_SyncCall) {
                    pCallbackEvent->m_Status = CallingStatus_Completed;
                    pCallbackEvent->m_bCompleted = TRUE;
                    sem_post(&pCallbackEvent->m_pSyncEvent);
                }

CallingCompleted:
                if ((CallbackType_CAR == pCallbackEvent->m_typeOfFunc) && (pClientObj)) {
                    pClientObj->Aggregate(AggrType_FriendLeave, (PInterface)pLock);
                    pClientObj->Release();
                }
            }
Cancel:
            pthread_mutex_unlock(&m_workingLock);

            PopEvent();
            pCallbackEvent->Release();

            if (pbOccured) {
                if (*pbOccured) {
                    if (pResult) *pResult = WaitResult_OK;
                    *pbOccured = FALSE;
                    goto Exit;
                }
            }
        }

        if (msTimeOut != INFINITE) {
            msLapsedTime = clock() - msBegPoint;
            if (msLapsedTime > msTimeOut) {
                if (pResult) *pResult = WaitResult_TimedOut;
                goto Exit;
            }
        }
    }


Exit:
    this->Release();
    pthread_setspecific(TL_CALLBACK_SLOT, pCallbackContext);
    return 0;
}
