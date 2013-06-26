#ifndef __TIMEZONES_H__
#define __TIMEZONES_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos/AutoFree.h>
#include <elastos/AutoPtr.h>

class TimeZones
{
public:
    static CARAPI_(String) LookupDisplayName(
        /* [in] */ ArrayOf<ArrayOf<String>*>* zoneStrings,
        /* [in] */ String id,
        /* [in] */ Boolean daylight,
        /* [in] */ Int32 style);

    static CARAPI Clone2dStringArray(
        /* [in] */ const ArrayOf<ArrayOf<String>*>* array,
        /* [out, callee] */ ArrayOf<ArrayOf<String>*>** resultArray);

    /**
     * Returns an array of time zone strings, as used by DateFormatSymbols.getZoneStrings.
     */
    static CARAPI GetZoneStrings(
        /* [in] */ ILocale* locale,
        /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** zoneStrings);

    /**
     * Creates array of time zone names for the given locale.
     * This method takes about 2s to run on a 400MHz ARM11.
     */
    static CARAPI CreateZoneStringsFor(
        /* [in] */ ILocale* locale,
        /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** newZoneStrings);
private:
    /**
     * Initialization holder for default time zone names. This class will
     * be preloaded by the zygote to share the time and space costs of setting
     * up the list of time zone names, so although it looks like the lazy
     * initialization idiom, it's actually the opposite.
     */
    class CachedTimeZones
    {
    public:
        /**
         * Name of default locale at the time this class was initialized.
         */
        const static AutoPtr<ILocale> mLocale;

        /**
         * Names of time zones for the default locale.
         */
        const static ArrayOf<ArrayOf<String> * > * mNames;
    };

private:
    const static ArrayOf<String>* mAvailableTimeZones;
};
#endif //__TIMEZONES_H__