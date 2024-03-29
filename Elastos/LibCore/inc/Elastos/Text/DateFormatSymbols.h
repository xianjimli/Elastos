#ifndef __DATEFORMATSYMBOLS_H__
#define __DATEFORMATSYMBOLS_H__

#include "Elastos.Text_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class DateFormatSymbols
{
public:
    DateFormatSymbols();

    virtual ~DateFormatSymbols();

    /**
     * Constructs a new {@code DateFormatSymbols} instance containing the
     * symbols for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     */
    CARAPI Init();

    /**
     * Constructs a new {@code DateFormatSymbols} instance containing the
     * symbols for the specified locale.
     *
     * @param locale
     *            the locale.
     */
    CARAPI Init(
        /* [in] */ ILocale* locale);

    /**
     * Gets zone strings, initializing them if necessary. Does not create
     * a defensive copy, so make sure you do so before exposing the returned
     * arrays to clients.
     */
    CARAPI_(ArrayOf<ArrayOf<String>*>*) InternalZoneStrings();

    /**
     * Returns a new {@code DateFormatSymbols} instance for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     *
     * @return an instance of {@code DateFormatSymbols}
     * @since 1.6
     */
    static CARAPI GetInstance(
        /* [out] */ IDateFormatSymbols** instance);

    /**
     * Returns a new {@code DateFormatSymbols} for the given locale.
     *
     * @param locale the locale
     * @return an instance of {@code DateFormatSymbols}
     * @throws NullPointerException if {@code locale == null}
     * @since 1.6
     */
    static CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormatSymbols** instance);

    /**
     * Returns an array of locales for which custom {@code DateFormatSymbols} instances
     * are available.
     * <p>Note that Android does not support user-supplied locale service providers.
     * @since 1.6
     */
    static CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** arrayOfLocales);

    /**
     * Returns the array of strings which represent AM and PM. Use the
     * {@link java.util.Calendar} constants {@code Calendar.AM} and
     * {@code Calendar.PM} as indices for the array.
     *
     * @return an array of strings.
     */
    virtual CARAPI GetAmPmStrings(
        /* [out, callee] */ ArrayOf<String> ** arrayOfStrings);

    /**
     * Returns the array of strings which represent BC and AD. Use the
     * {@link java.util.Calendar} constants {@code GregorianCalendar.BC} and
     * {@code GregorianCalendar.AD} as indices for the array.
     *
     * @return an array of strings.
     */
    virtual CARAPI GetEras(
        /* [out, callee] */ ArrayOf<String> ** arrayOfStrings);

    /**
     * Returns the pattern characters used by {@link SimpleDateFormat} to
     * specify date and time fields.
     *
     * @return a string containing the pattern characters.
     */
    virtual CARAPI GetLocalPatternChars(
        /* [out] */ String* string);

    /**
     * Returns the array of strings containing the full names of the months. Use
     * the {@link java.util.Calendar} constants {@code Calendar.JANUARY} etc. as
     * indices for the array.
     *
     * @return an array of strings.
     */
    virtual CARAPI GetMonths(
        /* [out, callee] */ ArrayOf<String>** arrayOfStrings);

    /**
     * Returns the array of strings containing the abbreviated names of the
     * months. Use the {@link java.util.Calendar} constants
     * {@code Calendar.JANUARY} etc. as indices for the array.
     *
     * @return an array of strings.
     */
    virtual CARAPI GetShortMonths(
            /* [out, callee] */ ArrayOf<String>** arrayOfStrings);

    /**
     * Returns the array of strings containing the abbreviated names of the days
     * of the week. Use the {@link java.util.Calendar} constants
     * {@code Calendar.SUNDAY} etc. as indices for the array.
     *
     * @return an array of strings.
     */
    virtual CARAPI GetShortWeekdays(
            /* [out, callee] */ ArrayOf<String>** arrayOfStrings);

    /**
     * Returns the array of strings containing the full names of the days of the
     * week. Use the {@link java.util.Calendar} constants
     * {@code Calendar.SUNDAY} etc. as indices for the array.
     *
     * @return an array of strings.
     */
    virtual CARAPI GetWeekdays(
            /* [out, callee] */ ArrayOf<String>** arrayOfStrings);

    /**
     * Returns the two-dimensional array of strings containing localized names for time zones.
     * Each row is an array of five strings:
     * <ul>
     * <li>The time zone ID, for example "America/Los_Angeles".
     * This is not localized, and is used as a key into the table.
     * <li>The long display name, for example "Pacific Standard Time".
     * <li>The short display name, for example "PST".
     * <li>The long display name for DST, for example "Pacific Daylight Time".
     * <li>The short display name for DST, for example "PDT".
     * </ul>
     */
    virtual CARAPI GetZoneStrings(
        /* [out, callee] */ ArrayOf<ArrayOf<String>*>** zoneStrings);

    /**
     * Sets the array of strings which represent AM and PM. Use the
     * {@link java.util.Calendar} constants {@code Calendar.AM} and
     * {@code Calendar.PM} as indices for the array.
     *
     * @param data
     *            the array of strings for AM and PM.
     */
    virtual CARAPI SetAmPmStrings(
        /* [in] */ const ArrayOf<String>& data);

    /**
     * Sets the array of Strings which represent BC and AD. Use the
     * {@link java.util.Calendar} constants {@code GregorianCalendar.BC} and
     * {@code GregorianCalendar.AD} as indices for the array.
     *
     * @param data
     *            the array of strings for BC and AD.
     */
    virtual CARAPI SetEras(
        /* [in] */ const ArrayOf<String>& data);

    /**
     * Sets the pattern characters used by {@link SimpleDateFormat} to specify
     * date and time fields.
     *
     * @param data
     *            the string containing the pattern characters.
     * @throws NullPointerException
     *            if {@code data} is null
     */
    virtual CARAPI SetLocalPatternChars(
        /* [in] */ String data);

    /**
     * Sets the array of strings containing the full names of the months. Use
     * the {@link java.util.Calendar} constants {@code Calendar.JANUARY} etc. as
     * indices for the array.
     *
     * @param data
     *            the array of strings.
     */
    virtual CARAPI SetMonths(
        /* [in] */ const ArrayOf<String>& data);

    /**
     * Sets the array of strings containing the abbreviated names of the months.
     * Use the {@link java.util.Calendar} constants {@code Calendar.JANUARY}
     * etc. as indices for the array.
     *
     * @param data
     *            the array of strings.
     */
    virtual CARAPI SetShortMonths(
        /* [in] */ const ArrayOf<String>& data);

    /**
     * Sets the array of strings containing the abbreviated names of the days of
     * the week. Use the {@link java.util.Calendar} constants
     * {@code Calendar.SUNDAY} etc. as indices for the array.
     *
     * @param data
     *            the array of strings.
     */
    virtual CARAPI SetShortWeekdays(
        /* [in] */ const ArrayOf<String>& data);

    /**
     * Sets the array of strings containing the full names of the days of the
     * week. Use the {@link java.util.Calendar} constants
     * {@code Calendar.SUNDAY} etc. as indices for the array.
     *
     * @param data
     *            the array of strings.
     */
    virtual CARAPI SetWeekdays(
        /* [in] */ const ArrayOf<String>& data);

    /**
     * Sets the two-dimensional array of strings containing localized names for time zones.
     * See {@link #getZoneStrings} for details.
     * @throws IllegalArgumentException if any row has fewer than 5 elements.
     * @throws NullPointerException if {@code zoneStrings == null}.
     */
    virtual CARAPI SetZoneStrings(
        /* [in] */ ArrayOf<ArrayOf<String>*>* zoneStrings);

private:
//    private void readObject(ObjectInputStream ois);

//    private void writeObject(ObjectOutputStream oos);

    static CARAPI_(Boolean) TimeZoneStringsEqual(
        /* [in] */ DateFormatSymbols* lhs,
        /* [in] */ DateFormatSymbols* rhs);

public:
    ArrayOf<String> *mAmpms, *mEras, *mMonths, *mShortMonths, *mShortWeekdays, *mWeekdays;

    // These are used to implement ICU/Android extensions.
    ArrayOf<String>* mLongStandAloneMonths;
    ArrayOf<String>* mShortStandAloneMonths;
    ArrayOf<String>* mLongStandAloneWeekdays;
    ArrayOf<String>* mShortStandAloneWeekdays;

    // Localized display names.
    ArrayOf<ArrayOf<String>*>* mZoneStrings;
    // Has the user called setZoneStrings?
    Boolean mCustomZoneStrings;

    /**
     * Locale, necessary to lazily load time zone strings. We force the time
     * zone names to load upon serialization, so this will never be needed
     * post deserialization.
     */
    AutoPtr<ILocale> mLocale;

    Mutex* mLock;

private:
    String mLocalPatternChars;
};
#endif //__DATEFORMATSYMBOLS_H__