
#include "database/CDataSetObservable.h"

ECode CDataSetObservable::RegisterObserver(
    /* [in] */ IInterface* observer)
{
    return DataSetObservable::RegisterObserver(observer);
}

ECode CDataSetObservable::UnregisterObserver(
    /* [in] */ IInterface* observer)
{
    return DataSetObservable::UnregisterObserver(observer);
}

ECode CDataSetObservable::UnregisterAll()
{
    return DataSetObservable::UnregisterAll();
}

ECode CDataSetObservable::NotifyChanged()
{
    return DataSetObservable::NotifyChanged();
}

ECode CDataSetObservable::NotifyInvalidated()
{
    return DataSetObservable::NotifyInvalidated();
}

