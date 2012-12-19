
#ifndef __CCONTENTOBSERVABLE_H__
#define __CCONTENTOBSERVABLE_H__

#include "_CContentObservable.h"
#include "database/ContentObservable.h"

CarClass(CContentObservable), public ContentObservable
{
public:
    CARAPI RegisterObserver(
        /* [in] */ IInterface* observer);

    CARAPI UnregisterObserver(
        /* [in] */ IInterface* observer);

    CARAPI UnregisterAll();

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);

    CARAPI NotifyChange(
        /* [in] */ Boolean selfChange);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCONTENTOBSERVABLE_H__
