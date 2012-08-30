
#include "database/DataSetObservable.h"

/**
 * Invokes onChanged on each observer. Called when the data set being observed has
 * changed, and which when read contains the new state of the data.
 */
ECode DataSetObservable::NotifyChanged()
{
    Mutex::Autolock lock(mObserversLock);

    Set<AutoPtr<IDataSetObserver> >::Iterator iter;
    Set<AutoPtr<IDataSetObserver> >(mObservers);
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        (*iter)->OnChanged();
    }
    return NOERROR;
}

/**
 * Invokes onInvalidated on each observer. Called when the data set being monitored
 * has changed such that it is no longer valid.
 */
ECode DataSetObservable::NotifyInvalidated()
{
    Mutex::Autolock lock(mObserversLock);

    Set<AutoPtr<IDataSetObserver> >::Iterator iter;
    Set<AutoPtr<IDataSetObserver> >(mObservers);
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        (*iter)->OnInvalidated();
    }
    return NOERROR;
}

ECode DataSetObservable::RegisterObserver(
        /* [in] */ IDataSetObserver* observer)
{
    Observable::RegisterObserver(observer);
    return NOERROR;
}

ECode DataSetObservable::UnregisterObserver(
        /* [in] */ IDataSetObserver* observer)
{
    Observable::UnregisterObserver(observer);
    return NOERROR;
}


