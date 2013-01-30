
#ifndef __CDRMCONSTRAINTINFO_H__
#define __CDRMCONSTRAINTINFO_H__

#include "_CDrmConstraintInfo.h"
//#include "Elastos.Droid.Drm.Mobile1_server.h"

/**
 * This class provides interfaces to access the DRM constraint.
 */
CarClass(CDrmConstraintInfo)
{
public:
    CARAPI GetInterfaceID(
        /*[in]*/ IInterface *pObject,
        /*[out]*/ InterfaceID *pIID);
    /**
     * Construct the DrmConstraint.
     */
    CARAPI constructor();
    /**
     * Get the count constraint.
     *
     * @return the count or -1 if no limit.
     */
    CARAPI GetCount(
        /* [out] */ Int32* count);

    /**
     * Get the start date constraint.
     *
     * @return the start date or null if no limit.
     */
    CARAPI GetStartDate(
        /* [out] */ IDate** startDate);

    /**
     * Get the end date constraint.
     *
     * @return the end date or null if no limit.
     */
    CARAPI GetEndDate(
        /* [out] */ IDate** endDate);

    /**
     * Get the Interval constraint.
     *
     * @return the interval or -1 if no limit.
     */
    CARAPI GetInterval(
        /* [out] */ Int64* interval);

public:
    /**
     * The constraint of count.
     */
    Int32 mCount;

    /**
     * The constraint of start date.
     */
    Int64 mStartDate;

    /**
     * The constraint of end date.
     */
    Int64 mEndDate;

    /**
     * The constraint of interval.
     */
    Int64 mInterval;
};


#endif // __CDRMCONSTRAINTINFO_H__

