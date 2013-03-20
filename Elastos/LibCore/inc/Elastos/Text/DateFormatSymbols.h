#ifndef __DATEFORMATSYMBOLS_H__
#define __DATEFORMATSYMBOLS_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
//#include <elastos/Mutex.h>
//#include <LocaleData.h>


//#include "CDateFormatSymbols.h"

using namespace Elastos;

class DateFormatSymbols
{
public:
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

//    @Override
//    public Object clone() {
//        try {
//            return super.clone();
//        } catch (CloneNotSupportedException e) {
//            throw new AssertionError();
//        }
//    }

    /**
     * Compares this object with the specified object and indicates if they are
     * equal.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if {@code object} is an instance of
     *         {@code DateFormatSymbols} and has the same symbols as this
     *         object, {@code false} otherwise.
     * @see #hashCode
     */
//    @Override
//    public boolean equals(Object object) {
//        if (this == object) {
//            return true;
//        }
//        if (!(object instanceof DateFormatSymbols)) {
//            return false;
//        }
//        DateFormatSymbols rhs = (DateFormatSymbols) object;
//        return localPatternChars.equals(rhs.localPatternChars) &&
//                Arrays.equals(ampms, rhs.ampms) &&
//                Arrays.equals(eras, rhs.eras) &&
//                Arrays.equals(months, rhs.months) &&
//                Arrays.equals(shortMonths, rhs.shortMonths) &&
//                Arrays.equals(shortWeekdays, rhs.shortWeekdays) &&
//                Arrays.equals(weekdays, rhs.weekdays) &&
//                timeZoneStringsEqual(this, rhs);
//    }

//    @Override
//    public String toString() {
        // 'locale' isn't part of the externally-visible state.
        // 'zoneStrings' is so large, we just print a representative value.
//        return getClass().getName() +
//                "[amPmStrings=" + Arrays.toString(ampms) +
//                ",customZoneStrings=" + customZoneStrings +
//                ",eras=" + Arrays.toString(eras) +
//                ",localPatternChars=" + new String(localPatternChars) +
//                ",months=" + Arrays.toString(months) +
//                ",shortMonths=" + Arrays.toString(shortMonths) +
//                ",shortWeekdays=" + Arrays.toString(shortWeekdays) +
//                ",weekdays=" + Arrays.toString(weekdays) +
//                ",zoneStrings=[" + Arrays.toString(internalZoneStrings()[0]) + "...]" +
//                "]";
//    }

    virtual CARAPI GetLongStandAloneMonths(
        /* [out, callee] */ ArrayOf<String>** longStandAloneMonths);

    virtual CARAPI GetShortStandAloneMonths(
        /* [out, callee] */ ArrayOf<String>** shortStandAloneMonths);

    virtual CARAPI GetLongStandAloneWeekdays(
        /* [out, callee] */ ArrayOf<String>** longStandAloneWeekdays);

    virtual CARAPI GetShortStandAloneWeekdays(
        /* [out, callee] */ ArrayOf<String>** shortStandAloneWeekdays);

    virtual CARAPI GetCustomZoneStrings(
        /* [out] */ Boolean* customZoneStrings);

    virtual CARAPI GetLocale(
        /* [out] */ ILocale** locale);    
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
        /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** zoneStrings);

//    @Override
//    public int hashCode() {
//        String[][] zoneStrings = internalZoneStrings();
//        int hashCode;
//        hashCode = localPatternChars.hashCode();
//        for (String element : ampms) {
//            hashCode += element.hashCode();
//        }
//        for (String element : eras) {
//            hashCode += element.hashCode();
//        }
//        for (String element : months) {
//            hashCode += element.hashCode();
//        }
//        for (String element : shortMonths) {
//            hashCode += element.hashCode();
//        }
//        for (String element : shortWeekdays) {
//            hashCode += element.hashCode();
//        }
//        for (String element : weekdays) {
//            hashCode += element.hashCode();
//        }
//        for (String[] element : zoneStrings) {
//            for (int j = 0; j < element.length; j++) {
//                if (element[j] != null) {
//                    hashCode += element[j].hashCode();
//                }
//            }
//        }
//        return hashCode;
//    }

    /**
     * Sets the array of strings which represent AM and PM. Use the
     * {@link java.util.Calendar} constants {@code Calendar.AM} and
     * {@code Calendar.PM} as indices for the array.
     *
     * @param data
     *            the array of strings for AM and PM.
     */
    virtual CARAPI SetAmPmStrings(
        /* [in] */ ArrayOf<String>* data);

    /**
     * Sets the array of Strings which represent BC and AD. Use the
     * {@link java.util.Calendar} constants {@code GregorianCalendar.BC} and
     * {@code GregorianCalendar.AD} as indices for the array.
     *
     * @param data
     *            the array of strings for BC and AD.
     */
    virtual CARAPI SetEras(
        /* [in] */ ArrayOf<String>* data);

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
        /* [in] */ ArrayOf<String>* data);

    /**
     * Sets the array of strings containing the abbreviated names of the months.
     * Use the {@link java.util.Calendar} constants {@code Calendar.JANUARY}
     * etc. as indices for the array.
     *
     * @param data
     *            the array of strings.
     */
    virtual CARAPI SetShortMonths(
        /* [in] */ ArrayOf<String>* data);

    /**
     * Sets the array of strings containing the abbreviated names of the days of
     * the week. Use the {@link java.util.Calendar} constants
     * {@code Calendar.SUNDAY} etc. as indices for the array.
     *
     * @param data
     *            the array of strings.
     */
    virtual CARAPI SetShortWeekdays(
        /* [in] */ ArrayOf<String>* data);

    /**
     * Sets the array of strings containing the full names of the days of the
     * week. Use the {@link java.util.Calendar} constants
     * {@code Calendar.SUNDAY} etc. as indices for the array.
     *
     * @param data
     *            the array of strings.
     */
    virtual CARAPI SetWeekdays(
        /* [in] */ ArrayOf<String>* data);

    /**
     * Sets the two-dimensional array of strings containing localized names for time zones.
     * See {@link #getZoneStrings} for details.
     * @throws IllegalArgumentException if any row has fewer than 5 elements.
     * @throws NullPointerException if {@code zoneStrings == null}.
     */
    virtual CARAPI SetZoneStrings(
        /* [in] */ ArrayOf<ArrayOf<String> *>* zoneStrings);

protected:
    /**
     * Gets zone strings, initializing them if necessary. Does not create
     * a defensive copy, so make sure you do so before exposing the returned
     * arrays to clients.
     */
    virtual CARAPI InternalZoneStrings(
        /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** zoneStrings);

private:
//    private void readObject(ObjectInputStream ois) throws IOException, ClassNotFoundException {
//        ois.defaultReadObject();

        // The RI doesn't have these fields, so we'll have to fall back and do the best we can.
//        longStandAloneMonths = months;
//        shortStandAloneMonths = shortMonths;
//        longStandAloneWeekdays = weekdays;
//        shortStandAloneWeekdays = shortWeekdays;
//    }

//    private void writeObject(ObjectOutputStream oos) throws IOException {
//        internalZoneStrings();
//        oos.defaultWriteObject();
//    }

    static CARAPI_(Boolean) TimeZoneStringsEqual(
        /* [in] */ IDateFormatSymbols *lhs,
        /* [in] */ IDateFormatSymbols *rhs);

protected:
    // These are used to implement ICU/Android extensions.
    mutable ArrayOf<String> *mLongStandAloneMonths;
    mutable ArrayOf<String> *mShortStandAloneMonths;
    mutable ArrayOf<String> *mLongStandAloneWeekdays;
    mutable ArrayOf<String> *mShortStandAloneWeekdays;

    // Localized display names.
    ArrayOf<ArrayOf<String> * > *mZoneStrings;
    // Has the user called setZoneStrings?
    mutable Boolean mCustomZoneStrings;

    /**
     * Locale, necessary to lazily load time zone strings. We force the time
     * zone names to load upon serialization, so this will never be needed
     * post deserialization.
     */
    mutable AutoPtr<ILocale> mLocale;

    //Mutex mLock;

private:
    //const static Int64 serialVersionUID = -5987973545549424702L;

    String mLocalPatternChars;

    ArrayOf<String> *mAmpms, *mEras, *mMonths, *mShortMonths, *mShortWeekdays, *mWeekdays;

};
#endif //__DATEFORMATSYMBOLS_H__