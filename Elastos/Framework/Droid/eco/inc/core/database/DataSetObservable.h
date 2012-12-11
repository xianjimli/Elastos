
#ifndef __DATASETOBSERVABLE_H__
#define __DATASETOBSERVABLE_H__

#include "database/Observable.h"

class DataSetObservable : public Observable
{
public:
    DataSetObservable();

    virtual ~DataSetObservable();

    virtual CARAPI NotifyChanged();

    virtual CARAPI NotifyInvalidated();

    CARAPI RegisterObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterObserver(
        /* [in] */ IDataSetObserver* observer);
};
#endif //__DATASETOBSERVABLE_H__
