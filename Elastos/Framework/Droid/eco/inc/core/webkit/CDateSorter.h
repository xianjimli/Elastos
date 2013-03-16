
#ifndef __CDATESORTER_H__
#define __CDATESORTER_H__

#include "_CDateSorter.h"
#include "ext/frameworkext.h"

CarClass(CDateSorter)
{
public:
    CARAPI GetIndex(
        /* [in] */ Int64 time,
        /* [out] */ Int32* lastDay);

    CARAPI GetLabel(
        /* [in] */ Int32 index,
        /* [out] */ String* label);

    CARAPI GetBoundary(
        /* [in] */ Int32 index,
        /* [out] */ Int64* boundary);

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    static const CString LOGTAG;// = "webkit";
    static const Int32 NUM_DAYS_AGO = 7;

    Int64 mBins[];
    String mLabels[];   
    
#if 0
    /**
     * Calcuate 12:00am by zeroing out hour, minute, second, millisecond
     */
    CARAPI_(void) BeginningOfDay(
        /* [in] */ ICalendar* c);
#endif
};

#endif // __CDATESORTER_H__
