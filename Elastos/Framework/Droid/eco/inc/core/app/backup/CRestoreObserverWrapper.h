
#ifndef  __CRESTOREOBSERVERWRAPPER_H__
#define  __CRESTOREOBSERVERWRAPPER_H__

#include "_CRestoreObserverWrapper.h"
#include <elastos/AutoPtr.h>


/*
 * We wrap incoming binder calls with a private class implementation that
 * redirects them into main-thread actions.  This serializes the restore
 * progress callbacks nicely within the usual main-thread lifecycle pattern.
 */
CarClass(CRestoreObserverWrapper)
{
public:
	CRestoreObserverWrapper();

	~CRestoreObserverWrapper();

    CARAPI constructor(
    	/* [in] */ IContext* context,
    	/* [in] */ IRestoreObserver* appObserver);

    // Binder calls into this object just enqueue on the main-thread handler
    CARAPI RestoreSetsAvailable(
    	/* [in] */ ArrayOf<IRestoreSet*>* result);

    CARAPI RestoreStarting(
    	/* [in] */ Int32 numPackages);

    CARAPI OnUpdate(
    	/* [in] */ Int32 nowBeingRestored,
    	/* [in] */ const String& currentPackage);

    CARAPI RestoreFinished(
    	/* [in] */ Int32 error);

private:
    AutoPtr<IApartment> mHandler;
    AutoPtr<IRestoreObserver> mAppObserver;

    static const Int32 MSG_RESTORE_STARTING = 1;
    static const Int32 MSG_UPDATE = 2;
    static const Int32 MSG_RESTORE_FINISHED = 3;
    static const Int32 MSG_RESTORE_SETS_AVAILABLE = 4;
};

#endif  //__CRESTOREOBSERVERWRAPPER_H__

