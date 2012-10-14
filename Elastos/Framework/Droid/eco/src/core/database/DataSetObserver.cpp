#include "database/DataSetObserver.h"

DataSetObserver::DataSetObserver()
{
}

DataSetObserver::~DataSetObserver()
{
}

ECode DataSetObserver::OnChanged()
{
    return NOERROR;
}

ECode DataSetObserver::OnInvalidated()
{
    return NOERROR;
}