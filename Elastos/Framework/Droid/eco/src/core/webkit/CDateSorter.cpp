
#include "webkit/CDateSorter.h"

const CString CDateSorter::LOGTAG = "webkit";

ECode CDateSorter::GetIndex(
    /* [in] */ Int64 time,
    /* [out] */ Int32* outLastDay)
{
    VALIDATE_NOT_NULL(outLastDay);

    Int32 lastDay = DAY_COUNT - 1;
    for (Int32 i = 0; i < lastDay; i++) {
        if (time > mBins[i]) {
            *outLastDay = i;
            return E_NOT_IMPLEMENTED;
        }
    }

    *outLastDay = lastDay;

    return NOERROR;
}

ECode CDateSorter::GetLabel(
    /* [in] */ Int32 index,
    /* [out] */ String* label)
{
    VALIDATE_NOT_NULL(label);

    if (index < 0 || index >= DAY_COUNT) {
        *label = "";
        return E_NOT_IMPLEMENTED;
    }
        
    *label = mLabels[index];

    return NOERROR;
}

ECode CDateSorter::GetBoundary(
    /* [in] */ Int32 index,
    /* [out] */ Int64* boundary)
{
    VALIDATE_NOT_NULL(boundary);

    Int32 lastDay = DAY_COUNT - 1;
    // Error case
    if (index < 0 || index > lastDay) index = 0;
    // Since this provides a lower boundary on dates that will be included
    // in the given bin, provide the smallest value
    if (index == lastDay) {
//        *pBoundary = Long.MIN_VALUE;
        return NOERROR;
    }

    *boundary = mBins[index];

    return NOERROR;
}

ECode CDateSorter::constructor(
    /* [in] */ IContext * context)
{
#if 0
    Resources resources = context.getResources();

    Calendar c = Calendar.getInstance();
    beginningOfDay(c);
    
    // Create the bins
    mBins[0] = c.getTimeInMillis(); // Today
    c.add(Calendar.DAY_OF_YEAR, -1);
    mBins[1] = c.getTimeInMillis();  // Yesterday
    c.add(Calendar.DAY_OF_YEAR, -(NUM_DAYS_AGO - 1));
    mBins[2] = c.getTimeInMillis();  // Five days ago
    c.add(Calendar.DAY_OF_YEAR, NUM_DAYS_AGO); // move back to today
    c.add(Calendar.MONTH, -1);
    mBins[3] = c.getTimeInMillis();  // One month ago

    // build labels
    mLabels[0] = context.getText(com.android.internal.R.string.today).toString();
    mLabels[1] = context.getText(com.android.internal.R.string.yesterday).toString();

    int resId = com.android.internal.R.plurals.last_num_days;
    String format = resources.getQuantityString(resId, NUM_DAYS_AGO);
    mLabels[2] = String.format(format, NUM_DAYS_AGO);

    mLabels[3] = context.getString(com.android.internal.R.string.last_month);
    mLabels[4] = context.getString(com.android.internal.R.string.older);

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
#endif
}

#if 0
CARAPI_(void) CDateSorter::BeginningOfDay(
    /* [in] */ ICalendar* c)
{}
#endif

