
#include "utils/FinitePool.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;

FinitePool::FinitePool(
    /* [in] */ IPoolableManager* manager)
    : mManager(manager)
    , mLimit(0)
    , mInfinite(TRUE)
{
}

FinitePool::FinitePool(
    /* [in] */ IPoolableManager* manager,
    /* [in] */ Int32 limit)
    : mManager(manager)
    , mLimit(limit)
    , mInfinite(FALSE)
{
    if (limit <= 0) {
        Logger::E("FinitePool", "The pool limit must be > 0");
        assert(0);
    }
}

PInterface FinitePool::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPool) {
        return (IPool*)this;
    }

    return NULL;
}

UInt32 FinitePool::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FinitePool::Release()
{
    return ElRefBase::Release();
}

ECode FinitePool::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IPool*)this) {
        *pIID = EIID_IPool;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode FinitePool::Acquire(
    /* [out] */ IPoolable** element)
{
    if (mRoot != NULL) {
        *element = mRoot;
        (*element)->AddRef();
        mRoot = NULL;
        (*element)->GetNextPoolable((IPoolable**)&mRoot);
        mPoolCount--;
    }
    else {
        FAIL_RETURN(mManager->NewInstance(element));
    }

    if (*element != NULL) {
        (*element)->SetNextPoolable(NULL);
        mManager->OnAcquired(*element);
    }

    return NOERROR;
}

ECode FinitePool::ReleaseElement(
    /* [in] */ IPoolable* element)
{
    if (mInfinite || mPoolCount < mLimit) {
        mPoolCount++;
        element->SetNextPoolable(mRoot);
        mRoot = element;
    }

    return mManager->OnReleased(element);
}
