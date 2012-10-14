
#ifndef __CONTENTOBSERVABLE_H__
#define __CONTENTOBSERVABLE_H__

#include "database/Observable.h"

class ContentObservable : public Observable
{
public:
    CARAPI RegisterObserver(
        /* [in] */ ILocalContentObserver* observer);

    /**
     * invokes dispatchUpdate on each observer, unless the observer doesn't want
     * self-notifications and the update is from a self-notification
     * @param selfChange
     */
    virtual CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);

    /**
     * invokes onChange on each observer
     * @param selfChange
     */
    virtual CARAPI NotifyChange(
        /* [in] */ Boolean selfChange);

    CARAPI UnregisterObserver(
        /* [in] */ ILocalContentObserver* observer);
};
#endif //__CONTENTOBSERVABLE_H__
