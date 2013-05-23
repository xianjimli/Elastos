
#include "content/CSyncStatusInfo.h"

const CString CSyncStatusInfo::TAG = "Sync";

ECode CSyncStatusInfo::GetLastFailureMesgAsInt(
    /* [in] */ Int32 def,
    /* [out] */ Int32* msg)
{
    VALIDATE_NOT_NULL(msg);

//    try {
        if (!lastFailureMesg.IsNullOrEmpty()) {
            *msg = lastFailureMesg.ToInt32();
            return NOERROR;
        }
//    } catch (NumberFormatException e) {
//        Log.d(TAG, "error parsing lastFailureMesg of " + lastFailureMesg, e);
//    }

    *msg = def;
    return NOERROR;
}

ECode CSyncStatusInfo::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = 0;

    return NOERROR;
}

ECode CSyncStatusInfo::SetPeriodicSyncTime(
    /* [in] */ Int32 index,
    /* [in] */ Int64 when)
{
    EnsurePeriodicSyncTimeSize(index);

    (*periodicSyncTimes)[index] = when;

    return NOERROR;
}

ECode CSyncStatusInfo::GetPeriodicSyncTime(
    /* [in] */ Int32 index,
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);

    if (NULL == periodicSyncTimes || periodicSyncTimes->GetLength() < (index + 1)) {
        *time = 0;
    }else{
        *time = (*periodicSyncTimes)[index];
    }

    return NOERROR;
}

ECode CSyncStatusInfo::RemovePeriodicSyncTime(
    /* [in] */ Int32 index)
{
    Int32 length = periodicSyncTimes->GetLength();

    // Will add one element and then delete it, so we return directly
    if(length == index){
        return NOERROR;
    }else if(index > length){
        EnsurePeriodicSyncTimeSize(index - 1);
    }else{
        Int32 newSize = length - 1;

        if(newSize < 0){
            newSize = 0;
        }

        ArrayOf<Int64>* newPeriodicSyncTimes = ArrayOf<Int64>::Alloc(newSize);

        // Copy head partd data befor index
        newPeriodicSyncTimes->Copy(periodicSyncTimes->GetPayload(), index);

        // Copy tail partd data after index
        for(int i = index; i < newSize; i++){
            (*newPeriodicSyncTimes)[i] = (*periodicSyncTimes)[i + 1];
        }

        ArrayOf<Int64>::Free(periodicSyncTimes);

        periodicSyncTimes = newPeriodicSyncTimes;
    }

    return NOERROR;
}

ECode CSyncStatusInfo::GetAuthorityId(
    /* [out] */ Int32* authorityId)
{
    VALIDATE_NOT_NULL(authorityId);

    *authorityId = this->authorityId;

    return NOERROR;
}

ECode CSyncStatusInfo::GetTotalElapsedTime(
    /* [out] */ Int64* totalElapsedTime)
{
    VALIDATE_NOT_NULL(totalElapsedTime);

    *totalElapsedTime = this->totalElapsedTime;

    return NOERROR;
}

ECode CSyncStatusInfo::SetTotalElapsedTime(
    /* [in] */ Int64 totalElapsedTime)
{
    this->totalElapsedTime = totalElapsedTime;

    return NOERROR;
}

ECode CSyncStatusInfo::GetNumSyncs(
    /* [out] */ Int32* numSyncs)
{
    VALIDATE_NOT_NULL(numSyncs);

    *numSyncs = this->numSyncs;

    return NOERROR;
}

ECode CSyncStatusInfo::SetNumSyncs(
    /* [in] */ Int32 numSyncs)
{
    this->numSyncs = numSyncs;

    return NOERROR;
}

ECode CSyncStatusInfo::GetNumSourcePoll(
    /* [out] */ Int32* numSourcePoll)
{
    VALIDATE_NOT_NULL(numSourcePoll);

    *numSourcePoll = this->numSourcePoll;

    return NOERROR;
}

ECode CSyncStatusInfo::SetNumSourcePoll(
    /* [in] */ Int32 numSourcePoll)
{
    this->numSourcePoll = numSourcePoll;

    return NOERROR;
}

ECode CSyncStatusInfo::GetNumSourceServer(
    /* [out] */ Int32* numSourceServer)
{
    VALIDATE_NOT_NULL(numSourceServer);

    *numSourceServer = this->numSourceServer;

    return NOERROR;
}

ECode CSyncStatusInfo::SetNumSourceServer(
    /* [in] */ Int32 numSourceServer)
{
    this->numSourceServer = numSourceServer;

    return NOERROR;
}

ECode CSyncStatusInfo::GetNumSourceLocal(
    /* [out] */ Int32* numSourceLocal)
{
    VALIDATE_NOT_NULL(numSourceLocal);

    *numSourceLocal = this->numSourceLocal;

    return NOERROR;
}

ECode CSyncStatusInfo::SetNumSourceLocal(
    /* [in] */ Int32 numSourceLocal)
{
    this->numSourceLocal = numSourceLocal;

    return NOERROR;
}

ECode CSyncStatusInfo::GetNumSourceUser(
    /* [out] */ Int32* numSourceUser)
{
    VALIDATE_NOT_NULL(numSourceUser);

    *numSourceUser = this->numSourceUser;

    return NOERROR;
}

ECode CSyncStatusInfo::SetNumSourceUser(
    /* [in] */ Int32 numSourceUser)
{
    this->numSourceUser = numSourceUser;

    return NOERROR;
}

ECode CSyncStatusInfo::GetNumSourcePeriodic(
    /* [out] */ Int32* numSourcePeriodic)
{
    VALIDATE_NOT_NULL(numSourcePeriodic);

    *numSourcePeriodic = this->numSourcePeriodic;

    return NOERROR;
}

ECode CSyncStatusInfo::SetNumSourcePeriodic(
    /* [in] */ Int32 numSourcePeriodic)
{
    this->numSourcePeriodic = numSourcePeriodic;

    return NOERROR;
}

ECode CSyncStatusInfo::GetLastSuccessSource(
    /* [out] */ Int32* lastSuccessSource)
{
    VALIDATE_NOT_NULL(lastSuccessSource);

    *lastSuccessSource = this->lastSuccessSource;

    return NOERROR;
}

ECode CSyncStatusInfo::SetLastSuccessSource(
    /* [in] */ Int32 lastSuccessSource)
{
    this->lastSuccessSource = lastSuccessSource;

    return NOERROR;
}

ECode CSyncStatusInfo::GetLastFailureSource(
    /* [out] */ Int32* lastFailureSource)
{
    VALIDATE_NOT_NULL(lastFailureSource);

    *lastFailureSource = this->lastFailureSource;

    return NOERROR;
}

ECode CSyncStatusInfo::SetLastFailureSource(
    /* [in] */ Int32 lastFailureSource)
{
    this->lastFailureSource = lastFailureSource;

    return NOERROR;
}

ECode CSyncStatusInfo::GetLastSuccessTime(
    /* [out] */ Int64* lastSuccessTime)
{
    VALIDATE_NOT_NULL(lastSuccessTime);

    *lastSuccessTime = this->lastSuccessTime;

    return NOERROR;
}

ECode CSyncStatusInfo::SetLastSuccessTime(
    /* [in] */ Int64 lastSuccessTime)
{
    this->lastSuccessTime = lastSuccessTime;

    return NOERROR;
}

ECode CSyncStatusInfo::GetLastFailureTime(
    /* [out] */ Int64* lastFailureTime)
{
    VALIDATE_NOT_NULL(lastFailureTime);

    *lastFailureTime = this->lastFailureTime;

    return NOERROR;
}

ECode CSyncStatusInfo::SetLastFailureTime(
    /* [in] */ Int64 lastFailureTime)
{
    this->lastFailureTime = lastFailureTime;

    return NOERROR;
}

ECode CSyncStatusInfo::GetInitialFailureTime(
    /* [out] */ Int64* initialFailureTime)
{
    VALIDATE_NOT_NULL(initialFailureTime);

    *initialFailureTime = this->initialFailureTime;

    return NOERROR;
}

ECode CSyncStatusInfo::SetInitialFailureTime(
    /* [in] */ Int64 initialFailureTime)
{
    this->initialFailureTime = initialFailureTime;

    return NOERROR;
}

ECode CSyncStatusInfo::GetLastFailureMesg(
    /* [out] */ String* lastFailureMesg)
{
    VALIDATE_NOT_NULL(lastFailureMesg);

    *lastFailureMesg = this->lastFailureMesg;

    return NOERROR;
}

ECode CSyncStatusInfo::SetLastFailureMesg(
    /* [in] */ const String& lastFailureMesg)
{
    this->lastFailureMesg = lastFailureMesg;

    return NOERROR;
}

ECode CSyncStatusInfo::GetPending(
    /* [out] */ Boolean* pending)
{
    VALIDATE_NOT_NULL(pending);

    *pending = this->pending;

    return NOERROR;
}

ECode CSyncStatusInfo::SetPending(
    /* [in] */ Boolean pending)
{
    this->pending = pending;

    return NOERROR;
}

ECode CSyncStatusInfo::GetInitialize(
    /* [out] */ Boolean* initialize)
{
    VALIDATE_NOT_NULL(initialize);

    *initialize = this->initialize;

    return NOERROR;
}

ECode CSyncStatusInfo::SetInitialize(
    /* [in] */ Boolean initialize)
{
    this->initialize = initialize;

    return NOERROR;
}

ECode CSyncStatusInfo::GetperiodicSyncTimes(
    /* [out, callee] */ ArrayOf<Int64>** periodicSyncTimes)
{
    VALIDATE_NOT_NULL(periodicSyncTimes);

    *periodicSyncTimes = this->periodicSyncTimes;

    return NOERROR;
}

ECode CSyncStatusInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 version;

    source->ReadInt32(&version);

    if (version != SyncStatusInfo_VERSION && version != 1) {
        Logger::W("SyncStatusInfo", String("Unknown version: ") + String::FromInt32(version));
    }

    source->ReadInt32(&authorityId);
    source->ReadInt64(&totalElapsedTime);
    source->ReadInt32(&numSyncs);
    source->ReadInt32(&numSourcePoll);
    source->ReadInt32(&numSourceServer);
    source->ReadInt32(&numSourceLocal);
    source->ReadInt32(&numSourceUser);
    source->ReadInt64(&lastSuccessTime);
    source->ReadInt32(&lastSuccessSource);
    source->ReadInt64(&lastFailureTime);
    source->ReadInt32(&lastFailureSource);
    source->ReadString(&lastFailureMesg);
    source->ReadInt64(&initialFailureTime);

    Int32 tempValue;

    source->ReadInt32(&tempValue);
    pending = (tempValue != 0 ? TRUE : FALSE);

    source->ReadInt32(&tempValue);
    initialize = (tempValue != 0 ? TRUE : FALSE);

    if (version == 1) {
        periodicSyncTimes = NULL;
    } else {
        Int32 N = 0;
        source->ReadInt32(&N);

        if (N < 0) {
            periodicSyncTimes = NULL;
        } else {
            periodicSyncTimes = ArrayOf<Int64>::Alloc(N);

            Int64 time = 0;

            for (int i = 0; i < N; i++) {
                source->ReadInt64(&time);

                (*periodicSyncTimes)[i] = time;
            }
        }
    }

    return NOERROR;
}

ECode CSyncStatusInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(SyncStatusInfo_VERSION);
    dest->WriteInt32(authorityId);
    dest->WriteInt64(totalElapsedTime);
    dest->WriteInt32(numSyncs);
    dest->WriteInt32(numSourcePoll);
    dest->WriteInt32(numSourceServer);
    dest->WriteInt32(numSourceLocal);
    dest->WriteInt32(numSourceUser);
    dest->WriteInt64(lastSuccessTime);
    dest->WriteInt32(lastSuccessSource);
    dest->WriteInt64(lastFailureTime);
    dest->WriteInt32(lastFailureSource);
    dest->WriteString(lastFailureMesg);
    dest->WriteInt64(initialFailureTime);
    dest->WriteInt32(pending ? 1 : 0);
    dest->WriteInt32(initialize ? 1 : 0);

    if (NULL != periodicSyncTimes) {
        Int32 length = periodicSyncTimes->GetLength();
        dest->WriteInt32(length);

        for (int i = 0; i < length; i++) {
            dest->WriteInt64((*periodicSyncTimes)[i]);
        }
    } else {
        dest->WriteInt32(-1);
    }

    return NOERROR;
}

ECode CSyncStatusInfo::constructor(
    /* [in] */ IParcel* parcel)
{
    ReadFromParcel(parcel);

    return NOERROR;
}

ECode CSyncStatusInfo::constructor(
    /* [in] */ Int32 authorityId)
{
    this->authorityId = authorityId;
    this->totalElapsedTime = 0;
    this->numSyncs = 0;
    this->numSourcePoll = 0;
    this->numSourceServer = 0;
    this->numSourceLocal = 0;
    this->numSourceUser = 0;
    this->numSourcePeriodic = 0;
    this->lastSuccessTime = 0;
    this->lastSuccessSource = 0;
    this->lastFailureTime = 0;
    this->lastFailureSource = 0;
    this->initialFailureTime = 0;
    this->pending = FALSE;
    this->initialize = FALSE;

    return NOERROR;
}

void CSyncStatusInfo::EnsurePeriodicSyncTimeSize(Int32 index)
{
    Int32 requiredSize = index + 1;
    Int32 length = 0;

    if (NULL == periodicSyncTimes) {
        periodicSyncTimes = ArrayOf< Int64 >::Alloc(requiredSize);
        for (int i = length; i < requiredSize; i++) {
            (*periodicSyncTimes)[i] = 0;
        }

    }else{
        length = periodicSyncTimes->GetLength();

        if (length < requiredSize) {
            ArrayOf<Int64>* newPeriodicSyncTimes = ArrayOf<Int64>::Alloc(requiredSize);

            newPeriodicSyncTimes->Copy(periodicSyncTimes->GetPayload(), length);

            ArrayOf<Int64>::Free(periodicSyncTimes);

            periodicSyncTimes = newPeriodicSyncTimes;

            for (int i = length; i < requiredSize; i++) {
                (*periodicSyncTimes)[i] = 0;
            }
        }
    }
}

