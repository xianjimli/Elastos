
#ifndef __DRMCONSTRAINTINFO_H__
#define __DRMCONSTRAINTINFO_H__

#include "Elastos.Droid.Drm.Mobile1_server.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * This class provides interfaces to access the DRM constraint.
 */
class DrmConstraintInfo
    : public ElRefBase
    , public IDrmConstraintInfo
{
public:
    /**
     * Construct the DrmConstraint.
     */
    /* package */ DrmConstraintInfo();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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

private:
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


#endif //__DRMCONSTRAINTINFO_H__

