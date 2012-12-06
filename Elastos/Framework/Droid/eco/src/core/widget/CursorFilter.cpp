
#include "widget/CursorFilter.h"

CursorFilter::CursorFilter()
{

}

CursorFilter::CursorFilter(
    /* [in] */ ICursorFilterClient* client) 
{
    mClient = client;
}

ECode CursorFilter::ConvertResultToString(
    /* [in] */ IInterface* resultValue,
    /* [out] */ ICharSequence** cs)
{
    return mClient->ConvertToString((ICursor*) resultValue, cs);
}

ECode CursorFilter::PerformFiltering(
    /* [in] */ ICharSequence* constraint,
    /* [out] */ IFilterResults** filterResults)
{
    AutoPtr<ICursor> cursor;
    mClient->RunQueryOnBackgroundThread(constraint, (ICursor**)&cursor);

    Filter::FilterResults* results = new Filter::FilterResults();
    if (cursor != NULL) {
        cursor->GetCount(&results->mCount);
        results->mValues = cursor;
    } else {
        results->mCount = 0;
        results->mValues = NULL;
    }

    *filterResults = results;

    return NOERROR;
}

ECode CursorFilter::PublishResults(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterResults* res)
{
    AutoPtr<ICursor> oldCursor;
    mClient->GetCursor((ICursor**)&oldCursor);
    
    Filter::FilterResults* results = (Filter::FilterResults*)res;
    if (results->mValues != NULL && results->mValues != oldCursor) {
        mClient->ChangeCursor((ICursor*) results->mValues.Get());
    }

    return NOERROR;
}
