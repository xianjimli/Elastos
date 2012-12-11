
#include "database/ContentObservable.h"

ContentObservable::ContentObservable()
{
}

ContentObservable::~ContentObservable()
{
}

ECode ContentObservable::RegisterObserver(
        /* [in] */ ILocalContentObserver* observer)
{
    Observable::RegisterObserver(observer);
    return NOERROR;
}

ECode ContentObservable::DispatchChange(
        /* [in] */ Boolean selfChange)
{
    Mutex::Autolock lock(mObserversLock);
    Set<AutoPtr<ILocalContentObserver> >::Iterator iter;
    Set<AutoPtr<ILocalContentObserver> >(mObservers);
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        Boolean rst;
        (*iter)->DeliverSelfNotifications(&rst);
        if (!selfChange || rst) {
            FAIL_RETURN((*iter)->DispatchChange(selfChange));
        }
    }
    return NOERROR;
    
}

ECode ContentObservable::NotifyChange(
        /* [in] */ Boolean selfChange)
{
    Mutex::Autolock lock(mObserversLock);
    Set<AutoPtr<ILocalContentObserver> >::Iterator iter;
    Set<AutoPtr<ILocalContentObserver> >(mObservers);
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        Boolean rst;
        (*iter)->DeliverSelfNotifications(&rst);
        if (!selfChange || rst) {
            FAIL_RETURN((*iter)->OnChange(selfChange));
        }
    }
    return NOERROR;
}

ECode ContentObservable::UnregisterObserver(
        /* [in] */ ILocalContentObserver* observer)
{
    Observable::UnregisterObserver(observer);
    return NOERROR;
}
