
#ifndef __CURSORFILTER_H__
#define __CURSORFILTER_H__

#include "widget/Filter.h"

/**
 * <p>The CursorFilter delegates most of the work to the CursorAdapter.
 * Subclasses should override these delegate methods to run the queries
 * and convert the results into String that can be used by auto-completion
 * widgets.</p>
 */
class CursorFilter : public Filter
{
public:
    CursorFilter();

    CursorFilter(
        /* [in] */ ICursorFilterClient* client);
    
    AutoPtr<ICharSequence> ConvertResultToString(
        /* [in] */ IInterface* resultValue);

    Filter::FilterResults* PerformFiltering(
        /* [in] */ ICharSequence* constraint);

    virtual CARAPI_(void) PublishResults(
        /* [in] */ ICharSequence* constraint, 
        /* [in] */ Filter::FilterResults* results);

    AutoPtr<ICursorFilterClient> mClient;
};

#endif
