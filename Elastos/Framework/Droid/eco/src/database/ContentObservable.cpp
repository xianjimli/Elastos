
#include "database/ContentObservable.h"

ECode ContentObservable::RegisterObserver(
        /* [in] */ IContentObserver* observer)
{
    Observable::RegisterObserver(observer);
    return NOERROR;
}

ECode ContentObservable::DispatchChange(
        /* [in] */ Boolean selfChange)
{
    Mutex::Autolock lock(mObserversLock);
    Set<AutoPtr<IContentObserver> >::Iterator iter;
    Set<AutoPtr<IContentObserver> >(mObservers);
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
    Set<AutoPtr<IContentObserver> >::Iterator iter;
    Set<AutoPtr<IContentObserver> >(mObservers);
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
        /* [in] */ IContentObserver* observer)
{
    Observable::UnregisterObserver(observer);
    return NOERROR;
}