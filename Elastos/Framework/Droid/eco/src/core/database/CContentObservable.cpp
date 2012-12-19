
#include "database/CContentObservable.h"


ECode CContentObservable::RegisterObserver(
    /* [in] */ IInterface* observer)
{
    return ContentObservable::RegisterObserver(ILocalContentObserver::Probe(observer));
}

ECode CContentObservable::UnregisterObserver(
    /* [in] */ IInterface* observer)
{
    return ContentObservable::UnregisterObserver(ILocalContentObserver::Probe(observer));
}

ECode CContentObservable::UnregisterAll()
{
    return ContentObservable::UnregisterAll();
}

ECode CContentObservable::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    return ContentObservable::DispatchChange(selfChange);
}

ECode CContentObservable::NotifyChange(
    /* [in] */ Boolean selfChange)
{
    return ContentObservable::NotifyChange(selfChange);
}
