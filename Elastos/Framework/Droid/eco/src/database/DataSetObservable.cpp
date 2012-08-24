
#include "database/DataSetObservable.h"

/**
 * Invokes onChanged on each observer. Called when the data set being observed has
 * changed, and which when read contains the new state of the data.
 */
void DataSetObservable::NotifyChanged()
{
    Mutex::Autolock lock(mObserversLock);

    Set<AutoPtr<IDataSetObserver> >::Iterator iter;
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        (*iter)->OnChanged();
    }
}

/**
 * Invokes onInvalidated on each observer. Called when the data set being monitored
 * has changed such that it is no longer valid.
 */
void DataSetObservable::NotifyInvalidated()
{
    Mutex::Autolock lock(mObserversLock);

    Set<AutoPtr<IDataSetObserver> >::Iterator iter;
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        (*iter)->OnInvalidated();
    }
}
