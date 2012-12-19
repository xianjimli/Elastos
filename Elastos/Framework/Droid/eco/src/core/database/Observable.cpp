
#include "database/Observable.h"
#include <elastos/Algorithm.h>
#include <Logger.h>

using namespace Elastos::Utility::Logging;


Observable::~Observable()
{
    mObservers.Clear();
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

    List< AutoPtr<IInterface> >::Iterator it =
            Find(mObservers.Begin(), mObservers.End(), AutoPtr<IInterface>(observer));
    if (it != mObservers.End()) {
        //throw new IllegalStateException("Observer " + observer + " is already registered.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mObservers.PushBack(observer);

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

    List< AutoPtr<IInterface> >::Iterator it =
            Find(mObservers.Begin(), mObservers.End(), AutoPtr<IInterface>(observer));
    if (it == mObservers.End()) {
        //throw new IllegalStateException("Observer " + observer + " was not registered.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mObservers.Erase(it);

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
