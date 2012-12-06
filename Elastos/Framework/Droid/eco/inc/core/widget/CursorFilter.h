
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
    
    virtual CARAPI ConvertResultToString(
        /* [in] */ IInterface* resultValue,
        /* [out] */ ICharSequence** cs);

    virtual CARAPI PerformFiltering(
        /* [in] */ ICharSequence* constraint,
        /* [out] */ IFilterResults** filterResults);

    virtual CARAPI PublishResults(
        /* [in] */ ICharSequence* constraint,
        /* [in] */ IFilterResults* results);

    AutoPtr<ICursorFilterClient> mClient;
};

#endif
