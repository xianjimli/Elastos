
#ifndef __CCONTENTOBSERVABLE_H__
#define __CCONTENTOBSERVABLE_H__

#include "_CContentObservable.h"
#include "database/ContentObservable.h"
CarClass(CContentObservable), public ContentObservable
{
public:
    CARAPI RegisterObserver(
        /* [in] */ IInterface * pObserver);

    CARAPI UnregisterObserver(
        /* [in] */ IInterface * pObserver);

    CARAPI UnregisterAll();

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);

    CARAPI NotifyChange(
        /* [in] */ Boolean selfChange);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCONTENTOBSERVABLE_H__
