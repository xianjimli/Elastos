#include "database/DataSetObserver.h"

DataSetObserver::DataSetObserver()
{
}

DataSetObserver::~DataSetObserver()
{
}

ECode DataSetObserver::OnChanged()
{
    // Do nothing
    return NOERROR;
}

ECode DataSetObserver::OnInvalidated()
{
    // Do nothing
    return NOERROR;
}