#include "TimeZones.h"

static ILocale* sInitLocale()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return locale;
}

static ArrayOf<ArrayOf<String> * > * sInitNames()
{
    ArrayOf<ArrayOf<String> * > * names;
    TimeZones::CreateZoneStringsFor(sInitLocale(), &names);
    return names->Clone();
}

static ArrayOf<String> * sInitTimeZones()
{
    AutoFree<ArrayOf<String> > array;
    //TimeZone::GetAvailableIDs(&array);
    return array;
}

const AutoPtr<ILocale> TimeZones::CachedTimeZones::mLocale = sInitLocale();

const ArrayOf<ArrayOf<String> * > * TimeZones::CachedTimeZones::mNames = sInitNames();

const ArrayOf<String> * TimeZones::mAvailableTimeZones = sInitTimeZones();

ECode TimeZones::Clone2dStringArray(
    /* [in] */ const ArrayOf<ArrayOf<String> * > * array,
    /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** resultArray)
{
    AutoFree<ArrayOf<ArrayOf<String> * > > result = ArrayOf<ArrayOf<String> * >::Alloc(array->GetLength());
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        (*result.Get())[i] = (*array)[i]->Clone();
    }
    *resultArray = (*result.Get()).Clone();
    return NOERROR;
}

ECode TimeZones::GetZoneStrings(
    /* [in] */ ILocale* locale,
    /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** zoneStrings)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    if (locale == NULL) {
        FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    }

    // TODO: We should force a reboot if the default locale changes.
    if (CachedTimeZones::mLocale == locale) {
        return Clone2dStringArray(CachedTimeZones::mNames, zoneStrings);
    }

    return CreateZoneStringsFor(locale, zoneStrings);
}

ECode TimeZones::CreateZoneStringsFor(
    /* [in] */ ILocale* locale,
    /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** newZoneStrings)
{
    //Int64 start = System::GetCurrentTimeMillis();

    /*
     * The following code is optimized for fast native response (the time a
     * method call can be in native code is limited). It prepares an empty
     * array to keep native code from having to create new Java objects. It
     * also fill in the time zone IDs to speed things up a bit. There's one
     * array for each time zone name type. (standard/long, standard/short,
     * daylight/long, daylight/short) The native method that fetches these
     * strings is faster if it can do all entries of one type, before having
     * to change to the next type. That's why the array passed down to
     * native has 5 entries, each providing space for all time zone names of
     * one type. Likely this access to the fields is much faster in the
     * native code because there's less array access overhead.
     *//*
    AutoFree<ArrayOf<ArrayOf<String> * > > arrayToFill = ArrayOf<ArrayOf<String> * >::Alloc(5);
    (*arrayToFill.Get())[0] = availableTimeZones.clone();
    arrayToFill[1] = new String[availableTimeZones.length];
    arrayToFill[2] = new String[availableTimeZones.length];
    arrayToFill[3] = new String[availableTimeZones.length];
    arrayToFill[4] = new String[availableTimeZones.length];

    // Don't be distracted by all the code either side of this line: this is the expensive bit!
    getZoneStringsImpl(arrayToFill, locale.toString());

    // Reorder the entries so we get the expected result.
    // We also take the opportunity to de-duplicate the names (http://b/2672057).
    HashMap<String, String> internTable = new HashMap<String, String>();
    String[][] result = new String[availableTimeZones.length][5];
    for (int i = 0; i < availableTimeZones.length; ++i) {
        result[i][0] = arrayToFill[0][i];
        for (int j = 1; j <= 4; ++j) {
            String original = arrayToFill[j][i];
            String nonDuplicate = internTable.get(original);
            if (nonDuplicate == null) {
                internTable.put(original, original);
                nonDuplicate = original;
            }
            result[i][j] = nonDuplicate;
        }
    }

    long duration = System.currentTimeMillis() - start;
    Logger.global.info("Loaded time zone names for " + locale + " in " + duration + "ms.");

    return result;*/
    return NOERROR;
}