#include "ZoneInfo.h"
#include <elastos/System.h>

const Int32 ZoneInfo::NORMAL[] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334,
};

const Int32 ZoneInfo::LEAP[] = {
        0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335,
};


String ZoneInfo::NullName(
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 begin)
{
}

ZoneInfo::ZoneInfo(
    /* [in] */ const String& name,
    /* [in] */ const ArrayOf<Int32>& transitions,
    /* [in] */ const ArrayOf<Byte>& type,
    /* [in] */ const ArrayOf<Int32>& gmtoff,
    /* [in] */ const ArrayOf<Byte>& isdst,
    /* [in] */ const ArrayOf<Byte>& abbreviationIndexes,
    /* [in] */ const ArrayOf<Byte>& abbreviationList)
{
    mTransitions = &transitions;
    mTypes = &type;
    mGmtOffs = (ArrayOf<Int32>*) &gmtoff;
    mIsDsts = &isdst;
    SetID(name);

    // Find the latest GMT and non-GMT offsets for their abbreviations

    Int32 lastdst;
    for (lastdst = mTransitions->GetLength() - 1; lastdst >= 0; lastdst--) {
        if ((*mIsDsts)[(*mTypes)[lastdst] & 0xFF] != 0) {
            break;
        }
    }

    Int32 laststd;
    for (laststd = mTransitions->GetLength() - 1; laststd >= 0; laststd--) {
        if ((*mIsDsts)[(*mTypes)[laststd] & 0xFF] == 0) {
            break;
        }
    }

    if (lastdst >= 0) {
        mDaylightName = NullName(abbreviationList, abbreviationIndexes[(*mTypes)[lastdst] & 0xFF]);
    } else {
        mDaylightName = NULL;
    }
    if (laststd >= 0) {
        mStandardName = NullName(abbreviationList, abbreviationIndexes[(*mTypes)[laststd] & 0xFF]);
    } else {
        mStandardName = NULL;
    }

    // Use the latest non-DST offset if any as the raw offset

    if (laststd < 0) {
        laststd = 0;
    }

    if (laststd >= mTypes->GetLength()) {
        mRawOffset = (*mGmtOffs)[0];
    } else {
        mRawOffset = (*mGmtOffs)[(*mTypes)[laststd] & 0xFF];
    }

    // Subtract the raw offset from all offsets so it can be changed
    // and affect them too.
    for (Int32 i = 0; i < mGmtOffs->GetLength(); i++) {
        (*mGmtOffs)[i] -= mRawOffset;
    }

    // Is this zone still observing DST?
    // We don't care if they've historically used it: most places have at least once.
    // We want to know whether the last "schedule info" (the unix times in the mTransitions
    // array) is in the future. If it is, DST is still relevant.
    // See http://code.google.com/p/android/issues/detail?id=877.
    // This test means that for somewhere like Morocco, which tried DST in 2009 but has
    // no future plans (and thus no future schedule info) will report "true" from
    // useDaylightTime at the start of 2009 but "false" at the end. This seems appropriate.
    Boolean usesDst = FALSE;
    Int64 currentUnixTime = System::GetCurrentTimeMillis() / 1000;
    if (mTransitions->GetLength() > 0) {
        // (We're really dealing with uint32_t values, so long is most convenient in Java.)
        Int64 latestScheduleTime = (*mTransitions)[mTransitions->GetLength() - 1] & 0xffffffff;
        if (currentUnixTime < latestScheduleTime) {
            usesDst = true;
        }
    }
    mUseDst = usesDst;

    mRawOffset *= 1000;
}

ECode ZoneInfo::GetOffsetVir(
    /* [in] */ Int32 era,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 millis,
    /* [out] */ Int32* offset)
{
    // XXX This assumes Gregorian always; Calendar switches from
    // Julian to Gregorian in 1582.  What calendar system are the
    // arguments supposed to come from?

    Int64 calc = (year / 400) * MILLISECONDS_PER_400_YEARS;
    year %= 400;

    calc += year * (365 * MILLISECONDS_PER_DAY);
    calc += ((year + 3) / 4) * MILLISECONDS_PER_DAY;

    if (year > 0)
        calc -= ((year - 1) / 100) * MILLISECONDS_PER_DAY;

    Boolean isLeap = (year == 0 || (year % 4 == 0 && year % 100 != 0));
    const Int32* mlen = isLeap ? LEAP : NORMAL;

    calc += mlen[month] * MILLISECONDS_PER_DAY;
    calc += (day - 1) * MILLISECONDS_PER_DAY;
    calc += millis;

    calc -= mRawOffset;
    calc -= UNIX_OFFSET;

    return GetOffset(calc, offset);
}

ECode ZoneInfo::GetOffset(
    /* [in] */ Int64 when,
    /* [out] */ Int32* offset)
{
    Int32 ix = (Int32) (when / 1000);
    Int32 trans;
    Int32 len = mTransitions->GetLength();

    for(Int32 i = 0; i < len; i++) {
        if ( (*mTransitions)[i] == ix ) {
            trans = i;
        }
    }

    if (trans == ~0) {
        *offset = (*mGmtOffs)[0] * 1000 + mRawOffset;
        return NOERROR;
    }
    if (trans < 0) {
        trans = ~trans - 1;
    }
    *offset = (*mGmtOffs)[(*mTypes)[trans] & 0xFF] * 1000 + mRawOffset;
    return NOERROR;
}

ECode ZoneInfo::GetRawOffsetVir(
    /* [out] */ Int32* offset)
{
     *offset = mRawOffset;
     return NOERROR;
}

ECode ZoneInfo::SetRawOffsetVir(
    /* [in] */ Int32 off)
{
    mRawOffset = off;
    return NOERROR;
}

ECode ZoneInfo::InDaylightTimeVir(
    /* [in] */ IDate *when,
    /* [out] */ Boolean* value)
{
    Int64 mill;
    when->GetTime(&mill);

    Int32 ix = (Int32) (mill / 1000);
    Int32 trans;
    Int32 len = mTransitions->GetLength();

    for(Int32 i = 0; i < len; i++) {
        if ( (*mTransitions)[i] == ix ) {
            trans = i;
        }
    }

    if (trans == ~0) {
        return (*mIsDsts)[0] != 0;
    }
    if (trans < 0) {
        trans = ~trans - 1;
    }

    *value = (*mIsDsts)[(*mTypes)[trans] & 0xFF] != 0;
    return NOERROR;
}

ECode ZoneInfo::UseDaylightTimeVir(
    /* [out] */ Boolean* value)
{
    *value = mUseDst;
    return NOERROR;
}

ECode ZoneInfo::HasSameRules(
    /* [in] */ ITimeZone* timeZone,
    /* [out] */ Boolean* value)
{
}

String ZoneInfo::ToString()
{
    return String("");
}

Int32 ZoneInfo::HashCode()
{
    return 0;
}

Boolean ZoneInfo::Equals(IInterface* obj)
{
    return TRUE;
}