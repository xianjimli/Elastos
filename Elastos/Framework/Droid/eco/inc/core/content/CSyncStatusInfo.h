
#ifndef __CSYNCSTATUSINFO_H__
#define __CSYNCSTATUSINFO_H__

#include "_CSyncStatusInfo.h"

#include <ext/frameworkdef.h>
#include <elastos/AutoPtr.h>
#include <Logger.h>

using namespace Elastos::Utility::Logging;

CarClass(CSyncStatusInfo)
{
public:
    CARAPI GetLastFailureMesgAsInt(
        /* [in] */ Int32 def,
        /* [out] */ Int32* msg);

    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI SetPeriodicSyncTime(
        /* [in] */ Int32 index,
        /* [in] */ Int64 when);

    CARAPI GetPeriodicSyncTime(
        /* [in] */ Int32 index,
        /* [out] */ Int64* time);

    CARAPI RemovePeriodicSyncTime(
        /* [in] */ Int32 index);

    CARAPI GetAuthorityId(
        /* [out] */ Int32* authorityId);

    CARAPI GetTotalElapsedTime(
        /* [out] */ Int64* totalElapsedTime);

    CARAPI SetTotalElapsedTime(
        /* [in] */ Int64 totalElapsedTime);

    CARAPI GetNumSyncs(
        /* [out] */ Int32* numSyncs);

    CARAPI SetNumSyncs(
        /* [in] */ Int32 numSyncs);

    CARAPI GetNumSourcePoll(
        /* [out] */ Int32* numSourcePoll);

    CARAPI SetNumSourcePoll(
        /* [in] */ Int32 numSourcePoll);

    CARAPI GetNumSourceServer(
        /* [out] */ Int32* numSourceServer);

    CARAPI SetNumSourceServer(
        /* [in] */ Int32 numSourceServer);

    CARAPI GetNumSourceLocal(
        /* [out] */ Int32* numSourceLocal);

    CARAPI SetNumSourceLocal(
        /* [in] */ Int32 numSourceLocal);

    CARAPI GetNumSourceUser(
        /* [out] */ Int32* numSourceUser);

    CARAPI SetNumSourceUser(
        /* [in] */ Int32 numSourceUser);

    CARAPI GetNumSourcePeriodic(
        /* [out] */ Int32* numSourcePeriodic);

    CARAPI SetNumSourcePeriodic(
        /* [in] */ Int32 numSourcePeriodic);

    CARAPI GetLastSuccessSource(
        /* [out] */ Int32* lastSuccessSource);

    CARAPI SetLastSuccessSource(
        /* [in] */ Int32 lastSuccessSource);

    CARAPI GetLastFailureSource(
        /* [out] */ Int32* lastFailureSource);

    CARAPI SetLastFailureSource(
        /* [in] */ Int32 lastFailureSource);

    CARAPI GetLastSuccessTime(
        /* [out] */ Int64* lastSuccessTime);

    CARAPI SetLastSuccessTime(
        /* [in] */ Int64 lastSuccessTime);

    CARAPI GetLastFailureTime(
        /* [out] */ Int64* lastFailureTime);

    CARAPI SetLastFailureTime(
        /* [in] */ Int64 lastFailureTime);

    CARAPI GetInitialFailureTime(
        /* [out] */ Int64* initialFailureTime);

    CARAPI SetInitialFailureTime(
        /* [in] */ Int64 initialFailureTime);

    CARAPI GetLastFailureMesg(
        /* [out] */ String* lastFailureMesg);

    CARAPI SetLastFailureMesg(
        /* [in] */ const String& lastFailureMesg);

    CARAPI GetPending(
        /* [out] */ Boolean* pending);

    CARAPI SetPending(
        /* [in] */ Boolean pending);

    CARAPI GetInitialize(
        /* [out] */ Boolean* initialize);

    CARAPI SetInitialize(
        /* [in] */ Boolean initialize);

    CARAPI GetperiodicSyncTimes(
        /* [out, callee] */ ArrayOf<Int64>** periodicSyncTimes);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

    CARAPI constructor(
        /* [in] */ Int32 authorityId);

public:

    /*final*/ Int32 authorityId;
    Int64 totalElapsedTime;
    Int32 numSyncs;
    Int32 numSourcePoll;
    Int32 numSourceServer;
    Int32 numSourceLocal;
    Int32 numSourceUser;
    Int32 numSourcePeriodic;
    Int64 lastSuccessTime;
    Int32 lastSuccessSource;
    Int64 lastFailureTime;
    Int32 lastFailureSource;
    String lastFailureMesg;
    Int64 initialFailureTime;
    Boolean pending;
    Boolean initialize;

    /*ArrayList<Long>*/ ArrayOf<Int64>* periodicSyncTimes;

private:
    void EnsurePeriodicSyncTimeSize(Int32 index);

private:
    static const CString TAG;// = "Sync";

};

#endif // __CSYNCSTATUSINFO_H__
