#ifndef __ZONEINFO_H__
#define __ZONEINFO_H__

#include "CTimeZone.h"
#include <elastos.h>

using namespace Elastos;

class ZoneInfo : public CTimeZone
{
public:
    ZoneInfo(
        /* [in] */ const String& name,
        /* [in] */ const ArrayOf<Int32>& transitions,
        /* [in] */ const ArrayOf<Byte>& type,
        /* [in] */ const ArrayOf<Int32>& gmtoff,
        /* [in] */ const ArrayOf<Byte>& isdst,
        /* [in] */ const ArrayOf<Byte>& abbreviationIndexes,
        /* [in] */ const ArrayOf<Byte>& abbreviationList);

    virtual CARAPI GetOffsetVir(
        /* [in] */ Int32 era,
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 millis,
        /* [out] */ Int32* offset);

    CARAPI GetOffset(
        /* [in] */ Int64 when,
        /* [out] */ Int32* offset);

    virtual CARAPI GetRawOffsetVir(
        /* [out] */ Int32* offset);

    virtual CARAPI SetRawOffsetVir(
        /* [in] */ Int32 off);

    virtual CARAPI InDaylightTimeVir(
        /* [in] */ IDate *when,
        /* [out] */ Boolean* value);

    virtual CARAPI UseDaylightTimeVir(
        /* [out] */ Boolean* value);

    CARAPI HasSameRules(
        /* [in] */ ITimeZone* timeZone,
        /* [out] */ Boolean* value);

    String ToString();
    Int32 HashCode();
    Boolean Equals(IInterface* obj);

private:
    static String NullName(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 begin);


private:
    static const Int64 MILLISECONDS_PER_DAY = 24 * 60 * 60 * 1000;
    static const Int64 MILLISECONDS_PER_400_YEARS =
            MILLISECONDS_PER_DAY * (400 * 365 + 100 - 3);

    static const Int64 UNIX_OFFSET = 62167219200000L;

    static const Int32 NORMAL[];
    static const Int32 LEAP[];

private:
    Int32 mRawOffset;

    const ArrayOf<Int32>* mTransitions;
    ArrayOf<Int32>* mGmtOffs;
    const ArrayOf<Byte>* mTypes;
    const ArrayOf<Byte>* mIsDsts;
    Boolean mUseDst;
    String mDaylightName;
    String mStandardName;
};
#endif //__ZONEINFO_H__
