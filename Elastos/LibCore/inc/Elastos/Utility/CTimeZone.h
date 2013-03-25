
#ifndef __CTIMEZONE_H__
#define __CTIMEZONE_H__
#include <elastos.h>
#include <Elastos.Core.h>
#include "CLocale.h"
#include "CDate.h"
#include <StringBuffer.h>

using namespace Elastos;

class CTimeZone
{
public:
    CARAPI GetDisplayName(
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx(
        /* [in] */ ILocale * pLocale,
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx1(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx2(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [in] */ ILocale * pLocale,
        /* [out] */ String * pName);

    CARAPI GetID(
        /* [out] */ String * pId);

    CARAPI GetDSTSavings(
        /* [out] */ Int32 * pSavings);

    CARAPI GetOffset(
        /* [in] */ Int64 time,
        /* [out] */ Int32 * pOffset);

    virtual CARAPI GetOffsetVir(
        /* [in] */ Int32 era,
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 timeOfDayMillis,
        /* [out] */ Int32 * pOffset) = 0;

    virtual CARAPI GetRawOffsetVir(
        /* [out] */ Int32 * pRawOffset) = 0;

    CARAPI HasSameRules(
        /* [in] */ ITimeZone * pTimeZone,
        /* [out] */ Boolean * pHasRules);

    virtual CARAPI InDaylightTimeVir(
        /* [in] */ IDate * pTime,
        /* [out] */ Boolean * pIsIn) = 0;

    CARAPI SetID(
        /* [in] */ const String& id);

    virtual CARAPI SetRawOffsetVir(
        /* [in] */ Int32 offsetMillis) = 0;

    virtual CARAPI UseDaylightTimeVir(
        /* [out] */ Boolean * pIsUsed) = 0;

private:
    void AppendNumber(
        /* [out] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ Int32 value);

private:
    String mID;
    // TODO: Add your private member variables here.
};

#endif // __CTIMEZONE_H__
