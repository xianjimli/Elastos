
#include "webkit/CDateSorter.h"

const char* CDateSorter::LOGTAG = "webkit";

ECode CDateSorter::GetIndex(
    /* [in] */ Int64 time,
    /* [out] */ Int32 * pLastDay)
{
    if (pLastDay == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    int lastDay = DAY_COUNT - 1;
    for (int i = 0; i < lastDay; i++)
    {
        if (time > mBins[i])
        {
            *pLastDay = i;
            return E_NOT_IMPLEMENTED;
        }
    }

    *pLastDay = lastDay;

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CDateSorter::GetLabel(
    /* [in] */ Int32 index,
    /* [out] */ String * pLabel)
{
    if (pLabel == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    if (index < 0 || index >= DAY_COUNT)
    {
        *pLabel = "";
        return E_NOT_IMPLEMENTED;
    }
        
    *pLabel = mLabels[index];

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CDateSorter::GetBoundary(
    /* [in] */ Int32 index,
    /* [out] */ Int64 * pBoundary)
{
    if (pBoundary == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    int lastDay = DAY_COUNT - 1;
    // Error case
    if (index < 0 || index > lastDay) index = 0;
    // Since this provides a lower boundary on dates that will be included
    // in the given bin, provide the smallest value
    if (index == lastDay)
    {
//        *pBoundary = Long.MIN_VALUE;
        return E_NOT_IMPLEMENTED;
    }

    *pBoundary = mBins[index];

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CDateSorter::constructor(
    /* [in] */ IContext * pContext)
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

