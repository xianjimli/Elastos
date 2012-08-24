
#ifndef __CRUNNINGAPPPROCESSINFO_H__
#define __CRUNNINGAPPPROCESSINFO_H__

#include "_CRunningAppProcessInfo.h"

CarClass(CRunningAppProcessInfo)
{
public:
    /**
     * Constant for {@link #importanceReasonCode}: nothing special has
     * been specified for the reason for this level.
     */
    static const Int32 REASON_UNKNOWN = 0;

    /**
     * Constant for {@link #importanceReasonCode}: one of the application's
     * content providers is being used by another process.  The pid of
     * the client process is in {@link #importanceReasonPid} and the
     * target provider in this process is in
     * {@link #importanceReasonComponent}.
     */
    static const Int32 REASON_PROVIDER_IN_USE = 1;

    /**
     * Constant for {@link #importanceReasonCode}: one of the application's
     * content providers is being used by another process.  The pid of
     * the client process is in {@link #importanceReasonPid} and the
     * target provider in this process is in
     * {@link #importanceReasonComponent}.
     */
    static const Int32 REASON_SERVICE_IN_USE = 2;

public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CRUNNINGAPPPROCESSINFO_H__
