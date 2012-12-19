
#include "database/ContentObservable.h"


ECode ContentObservable::RegisterObserver(
        /* [in] */ ILocalContentObserver* observer)
{
    return Observable::RegisterObserver(observer);
}

ECode ContentObservable::DispatchChange(
        /* [in] */ Boolean selfChange)
{
    Mutex::Autolock lock(mObserversLock);

    List< AutoPtr<IInterface> >::Iterator iter;
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        ILocalContentObserver* observer = ILocalContentObserver::Probe(*iter);
        assert(observer != NULL);
        Boolean result;
        if (!selfChange || (observer->DeliverSelfNotifications(&result), result)) {
            FAIL_RETURN(observer->DispatchChange(selfChange));
        }
    }
    return NOERROR;

}

ECode ContentObservable::NotifyChange(
        /* [in] */ Boolean selfChange)
{
    Mutex::Autolock lock(mObserversLock);

    List< AutoPtr<IInterface> >::Iterator iter;
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        ILocalContentObserver* observer = ILocalContentObserver::Probe(*iter);
        assert(observer != NULL);
        Boolean result;
        if (!selfChange || (observer->DeliverSelfNotifications(&result), result)) {
            FAIL_RETURN(observer->OnChange(selfChange));
        }
    }
    return NOERROR;
}
