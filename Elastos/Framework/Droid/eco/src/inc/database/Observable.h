
#ifndef __OBSERVABLE_H__
#define __OBSERVABLE_H__

#include "ext/frameworkext.h"
#include "utils/ElRefBase.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>
#include <elastos/Mutex.h>

using namespace Elastos::System::Threading;

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

    CARAPI RegisterObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterAll();

protected:
    /**
     * The list of observers.  An observer can be in the list at most
     * once and will never be NULL.
     */
    Set<AutoPtr<IDataSetObserver> > mObservers;

    Mutex mObserversLock;
};
#endif //__OBSERVABLE_H__
