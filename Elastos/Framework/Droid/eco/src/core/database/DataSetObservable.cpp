
#include "database/DataSetObservable.h"


/**
 * Invokes onChanged on each observer. Called when the data set being observed has
 * changed, and which when read contains the new state of the data.
 */
ECode DataSetObservable::NotifyChanged()
{
    Mutex::Autolock lock(mObserversLock);

    List< AutoPtr<IInterface> >::Iterator it;
    for (it = mObservers.Begin(); it != mObservers.End(); ++it) {
        IDataSetObserver* observer = IDataSetObserver::Probe(*it);
        assert(observer != NULL);
        observer->OnChanged();
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

    List< AutoPtr<IInterface> >::Iterator it;
    for (it = mObservers.Begin(); it != mObservers.End(); ++it) {
        IDataSetObserver* observer = IDataSetObserver::Probe(*it);
        assert(observer != NULL);
        observer->OnInvalidated();
    }

    return NOERROR;
}
