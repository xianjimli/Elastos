
#ifndef __OBSERVABLE_H__
#define __OBSERVABLE_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class Observable : public ElRefBase, public IObservable
{
protected:
    Observable() {}

    virtual ~Observable() {}

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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
    Set<AutoPtr<IInterface> > mObservers;
    Mutex mObserversLock;
};
#endif //__OBSERVABLE_H__
