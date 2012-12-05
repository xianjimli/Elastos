
#include "widget/CursorFilter.h"

CursorFilter::CursorFilter()
{

}

CursorFilter::CursorFilter(
    /* [in] */ ICursorFilterClient* client) 
{
    mClient = client;
}

AutoPtr<ICharSequence> CursorFilter::ConvertResultToString(
    /* [in] */ IInterface* resultValue)
{
    AutoPtr<ICharSequence> cs;
    mClient->ConvertToString((ICursor*) resultValue, (ICharSequence**)&cs);

    return cs;
}

Filter::FilterResults* CursorFilter::PerformFiltering(
    /* [in] */ ICharSequence* constraint)
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
    return results;
}

void CursorFilter::PublishResults(
    /* [in] */ ICharSequence* constraint, 
    /* [in] */ Filter::FilterResults* results)
{
    AutoPtr<ICursor> oldCursor;
    mClient->GetCursor((ICursor**)&oldCursor);
    
    if (results->mValues != NULL && results->mValues != oldCursor) {
        mClient->ChangeCursor((ICursor*) results->mValues.Get());
    }
}
