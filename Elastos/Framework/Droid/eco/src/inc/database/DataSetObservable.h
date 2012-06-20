
#ifndef __DATASETOBSERVABLE_H__
#define __DATASETOBSERVABLE_H__

#include "database/Observable.h"

class DataSetObservable : public Observable
{
public:
    CARAPI_(void) NotifyChanged();

    CARAPI_(void) NotifyInvalidated();
};
#endif //__DATASETOBSERVABLE_H__
