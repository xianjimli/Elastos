
#include <StringBuffer.h>
#include "content/CSyncStats.h"

ECode CSyncStats::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer sb("");

    sb += " stats [";

    if (numAuthExceptions > 0) {
        sb += " numAuthExceptions: ";
        sb += numAuthExceptions;
    }

    if (numIoExceptions > 0) {
        sb += " numIoExceptions: ";
        sb += numIoExceptions;
    }

    if (numParseExceptions > 0) {
        sb += " numParseExceptions: ";
        sb += numParseExceptions;
    }

    if (numConflictDetectedExceptions > 0) {
        sb += " numConflictDetectedExceptions: ";
        sb += numConflictDetectedExceptions;
    }

    if (numInserts > 0) {
        sb += " numInserts: ";
        sb += numInserts;
    }

    if (numUpdates > 0) {
        sb += " numUpdates: ";
        sb += numUpdates;
    }

    if (numDeletes > 0) {
        sb += " numDeletes: ";
        sb += numDeletes;
    }

    if (numEntries > 0) {
        sb += " numEntries: ";
        sb += numEntries;
    }

    if (numSkippedEntries > 0) {
        sb += " numSkippedEntries: ";
        sb += numSkippedEntries;
    }

    sb += "]";

    *str = String(sb);

    return NOERROR;
}

ECode CSyncStats::Clear()
{
    numAuthExceptions = 0;
    numIoExceptions = 0;
    numParseExceptions = 0;
    numConflictDetectedExceptions = 0;
    numInserts = 0;
    numUpdates = 0;
    numDeletes = 0;
    numEntries = 0;
    numSkippedEntries = 0;

    return NOERROR;
}

ECode CSyncStats::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = 0;
    return NOERROR;
}

ECode CSyncStats::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt64(&numAuthExceptions);
    source->ReadInt64(&numIoExceptions);
    source->ReadInt64(&numParseExceptions);
    source->ReadInt64(&numConflictDetectedExceptions);
    source->ReadInt64(&numInserts);
    source->ReadInt64(&numUpdates);
    source->ReadInt64(&numDeletes);
    source->ReadInt64(&numEntries);
    source->ReadInt64(&numSkippedEntries);

    return NOERROR;
}

ECode CSyncStats::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt64(numAuthExceptions);
    dest->WriteInt64(numIoExceptions);
    dest->WriteInt64(numParseExceptions);
    dest->WriteInt64(numConflictDetectedExceptions);
    dest->WriteInt64(numInserts);
    dest->WriteInt64(numUpdates);
    dest->WriteInt64(numDeletes);
    dest->WriteInt64(numEntries);
    dest->WriteInt64(numSkippedEntries);

    return NOERROR;
}

ECode CSyncStats::constructor()
{
    numAuthExceptions = 0;
    numIoExceptions = 0;
    numParseExceptions = 0;
    numConflictDetectedExceptions = 0;
    numInserts = 0;
    numUpdates = 0;
    numDeletes = 0;
    numEntries = 0;
    numSkippedEntries = 0;

    return NOERROR;
}

ECode CSyncStats::constructor(
    /* [in] */ IParcel* parcel)
{
    return ReadFromParcel(parcel);
}
