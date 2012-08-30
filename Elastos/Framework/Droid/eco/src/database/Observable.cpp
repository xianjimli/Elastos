
#include "database/Observable.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;

PInterface Observable::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IObservable) {
        return (IObservable*)this;
    }

    return NULL;
}

UInt32 Observable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Observable::Release()
{
    return ElRefBase::Release();
}

ECode Observable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IObservable*)this) {
        *pIID = EIID_IObservable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

/**
 * Adds an observer to the list. The observer cannot be NULL and it must not already
 * be registered.
 * @param observer the observer to register
 * @throws IllegalArgumentException the observer is NULL
 * @throws IllegalStateException the observer is already registered
 */
ECode Observable::RegisterObserver(
    /* [in] */ IInterface* observer)
{
    if (observer == NULL) {
        Logger::E("Observable", "The observer is NULL.");

        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mObserversLock);
    if (mObservers.Find(observer) != mObservers.End()) {
        //throw new IllegalStateException("Observer " + observer + " is already registered.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mObservers.Insert(observer);

    return NOERROR;
}

/**
 * Removes a previously registered observer. The observer must not be NULL and it
 * must already have been registered.
 * @param observer the observer to unregister
 * @throws IllegalArgumentException the observer is NULL
 * @throws IllegalStateException the observer is not yet registered
 */
ECode Observable::UnregisterObserver(
    /* [in] */ IInterface* observer)
{
    if (observer == NULL) {
        Logger::E("Observable", "The observer is NULL.");

        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mObserversLock);

    if (mObservers.Find(observer) == mObservers.End()) {
        //throw new IllegalStateException("Observer " + observer + " was not registered.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mObservers.Erase(observer);

    return NOERROR;
}

/**
 * Remove all registered observer
 */
ECode Observable::UnregisterAll()
{
    Mutex::Autolock lock(mObserversLock);
    mObservers.Clear();

    return NOERROR;
}
