
#include "database/CContentObservable.h"
ECode CContentObservable::RegisterObserver(
    /* [in] */ IInterface * pObserver)
{
    ContentObservable::RegisterObserver((ILocalContentObserver*)pObserver);
    return NOERROR;
}

ECode CContentObservable::UnregisterObserver(
    /* [in] */ IInterface * pObserver)
{
    ContentObservable::UnregisterObserver((ILocalContentObserver*)pObserver);
    return NOERROR;
}

ECode CContentObservable::UnregisterAll()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentObservable::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    ContentObservable::DispatchChange(selfChange);
    return NOERROR;
}

ECode CContentObservable::NotifyChange(
    /* [in] */ Boolean selfChange)
{
    ContentObservable::NotifyChange(selfChange);
    return NOERROR;
}

ECode CContentObservable::constructor()
{
    ContentObservable::ContentObservable();
    return NOERROR;
}

