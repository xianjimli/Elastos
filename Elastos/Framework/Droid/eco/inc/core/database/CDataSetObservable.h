
#ifndef __CDATASETOBSERVABLE_H__
#define __CDATASETOBSERVABLE_H__

#include "_CDataSetObservable.h"
#include "database/DataSetObservable.h"
CarClass(CDataSetObservable), public DataSetObservable
{
public:
    CARAPI RegisterObserver(
        /* [in] */ IInterface * pObserver);

    CARAPI UnregisterObserver(
        /* [in] */ IInterface * pObserver);

    CARAPI UnregisterAll();

    CARAPI NotifyChanged();

    CARAPI NotifyInvalidated();

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CDATASETOBSERVABLE_H__
