
#include "cmdef.h"
#include "CDrmConstraintInfo.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


/**
 * Construct the DrmConstraint.
 */
ECode CDrmConstraintInfo::constructor(){
    mCount = -1;
    mStartDate = -1;
    mEndDate = -1;
    mInterval = -1;
    return NOERROR;
}


PInterface IDrmConstraintInfo::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IDrmConstraintInfo*)this;
    }
    else if (riid == EIID_IDrmConstraintInfo) {
        return (IDrmConstraintInfo*)this;
    }

    return NULL;
}


UInt32 AddRef()
{
    //return ElRefBase::AddRef();
}

UInt32 Release()
{
    //return ElRefBase::Release();
}
/*
*/
ECode CDrmConstraintInfo::GetInterfaceID(
     /*[in]*/   IInterface *pObject,
     /*[out]*/   InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IDrmConstraintInfo*)this) {
        *pIID = EIID_IDrmConstraintInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}



/**
 * Get the count constraint.
 *
 * @return the count or -1 if no limit.
 */
ECode CDrmConstraintInfo::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mCount;
    return NOERROR;
}

/**
 * Get the start date constraint.
 *
 * @return the start date or null if no limit.
 */
ECode CDrmConstraintInfo::GetStartDate(
    /* [out] */ IDate** startDate)
{
    VALIDATE_NOT_NULL(startDate);

    if (mStartDate == -1) {
        *startDate = NULL;
        return NOERROR;
    }

    return CDate::New(mStartDate, startDate);
}

/**
 * Get the end date constraint.
 *
 * @return the end date or null if no limit.
 */
ECode CDrmConstraintInfo::GetEndDate(
    /* [out] */ IDate** endDate)
{
    VALIDATE_NOT_NULL(endDate);

    if (mEndDate == -1) {
        *endDate = NULL;
        return NOERROR;
    }

    return CDate::New(mEndDate, endDate);
}

/**
 * Get the Interval constraint.
 *
 * @return the interval or -1 if no limit.
 */
ECode CDrmConstraintInfo::GetInterval(
    /* [out] */ Int64* interval)
{
    VALIDATE_NOT_NULL(interval);
    *interval = mInterval;
    return NOERROR;
}
