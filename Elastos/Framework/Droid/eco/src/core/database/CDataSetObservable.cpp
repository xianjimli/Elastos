
#include "database/CDataSetObservable.h"
ECode CDataSetObservable::RegisterObserver(
    /* [in] */ IInterface * pObserver)
{
    DataSetObservable::RegisterObserver((IDataSetObserver*)pObserver);
    return NOERROR;
}

ECode CDataSetObservable::UnregisterObserver(
    /* [in] */ IInterface * pObserver)
{
    DataSetObservable::UnregisterObserver((IDataSetObserver*)pObserver);
    return NOERROR;
}

ECode CDataSetObservable::UnregisterAll()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CDataSetObservable::NotifyChanged()
{
    DataSetObservable::NotifyChanged();
    return NOERROR;
}

ECode CDataSetObservable::NotifyInvalidated()
{
    DataSetObservable::NotifyInvalidated();
    return NOERROR;
}

ECode CDataSetObservable::constructor()
{
    DataSetObservable::DataSetObservable();
    return NOERROR;
}

