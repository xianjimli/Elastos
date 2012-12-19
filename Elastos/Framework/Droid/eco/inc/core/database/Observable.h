
#ifndef __OBSERVABLE_H__
#define __OBSERVABLE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

/**
 * Provides methods for (un)registering arbitrary observers in an ArrayList.
 */
class Observable
{
public:
    virtual ~Observable();

    virtual CARAPI RegisterObserver(
        /* [in] */ IInterface* observer);

    virtual CARAPI UnregisterObserver(
        /* [in] */ IInterface* observer);

    virtual CARAPI UnregisterAll();

protected:
    /**
     * The list of observers.  An observer can be in the list at most
     * once and will never be NULL.
     */
    List< AutoPtr<IInterface> > mObservers;
    Mutex mObserversLock;
};

#endif //__OBSERVABLE_H__
