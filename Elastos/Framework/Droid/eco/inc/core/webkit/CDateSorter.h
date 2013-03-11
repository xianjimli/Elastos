
#ifndef __CDATESORTER_H__
#define __CDATESORTER_H__

#include "_CDateSorter.h"

CarClass(CDateSorter)
{
public:
    CARAPI GetIndex(
        /* [in] */ Int64 time,
        /* [out] */ Int32 * pLastDay);

    CARAPI GetLabel(
        /* [in] */ Int32 index,
        /* [out] */ String * pLabel);

    CARAPI GetBoundary(
        /* [in] */ Int32 index,
        /* [out] */ Int64 * pBoundary);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    static const char* LOGTAG;// = "webkit";
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
