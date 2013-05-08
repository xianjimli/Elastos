
#include "CDateFormatHelper.h"
#include <elastos/AutoPtr.h>
#include "cmdef.h"

ECode CDateFormatHelper::CheckDateStyle(
    /* [in] */ Int32 style)
{
    if (!(style == IDateFormat_SHORT
            || style == IDateFormat_MEDIUM
            || style == IDateFormat_LONG
            || style == IDateFormat_FULL
            || style == IDateFormat_DEFAULT)) {
        //throw new IllegalArgumentException("Illegal date style " + style);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CDateFormatHelper::CheckTimeStyle(
    /* [in] */ Int32 style)
{
    if (!(style == IDateFormat_SHORT
            || style == IDateFormat_MEDIUM
            || style == IDateFormat_LONG
            || style == IDateFormat_FULL
            || style == IDateFormat_DEFAULT)) {
        //throw new IllegalArgumentException("Illegal time style " + style);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CDateFormatHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale *> ** locales)
{
    AutoPtr<IICUHelper> ICUHelper;
    FAIL_RETURN(CICUHelper::AcquireSingleton((IICUHelper**)&ICUHelper));
    return ICUHelper->GetAvailableDateFormatLocales(locales);
}

ECode CDateFormatHelper::GetDateInstance(
    /* [out] */ IDateFormat ** instance)
{
    return GetDateInstanceEx(IDateFormat_DEFAULT, instance);
}

ECode CDateFormatHelper::GetDateInstanceEx(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat ** instance)
{
    FAIL_RETURN(CheckDateStyle(style));
    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetDateInstanceEx2(style, locale, instance);
}

ECode CDateFormatHelper::GetDateInstanceEx2(
    /* [in] */ Int32 style,
    /* [in] */ ILocale * locale,
    /* [out] */ IDateFormat ** instance)
{
    assert(0);
    FAIL_RETURN(CheckDateStyle(style));
    //return new SimpleDateFormat(LocaleData.get(locale).getDateFormat(style), locale);
    return E_NOT_IMPLEMENTED;
}

ECode CDateFormatHelper::GetDateTimeInstance(
    /* [out] */ IDateFormat ** instance)
{
    return GetDateTimeInstanceEx(IDateFormat_DEFAULT, IDateFormat_DEFAULT, instance);
}

ECode CDateFormatHelper::GetDateTimeInstanceEx(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [out] */ IDateFormat ** instance)
{
    FAIL_RETURN(CheckTimeStyle(timeStyle));
    FAIL_RETURN(CheckDateStyle(dateStyle));
    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetDateTimeInstanceEx2(dateStyle, timeStyle, locale, instance);
}

ECode CDateFormatHelper::GetDateTimeInstanceEx2(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [in] */ ILocale * locale,
    /* [out] */ IDateFormat ** instance)
{
    assert(0);
    FAIL_RETURN(CheckTimeStyle(timeStyle));
    FAIL_RETURN(CheckDateStyle(dateStyle));
    //LocaleData localeData = LocaleData.get(locale);
    //String pattern = localeData.getDateFormat(dateStyle) + " " + localeData.getTimeFormat(timeStyle);
    //return new SimpleDateFormat(pattern, locale);
    return E_NOT_IMPLEMENTED;
}

ECode CDateFormatHelper::GetInstance(
    /* [out] */ IDateFormat ** instance)
{
    return GetDateTimeInstanceEx(IDateFormat_SHORT, IDateFormat_SHORT, instance);
}

ECode CDateFormatHelper::GetTimeInstance(
    /* [out] */ IDateFormat ** instance)
{
    return GetTimeInstanceEx(IDateFormat_DEFAULT, instance);
}

ECode CDateFormatHelper::GetTimeInstanceEx(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat ** instance)
{
    FAIL_RETURN(CheckTimeStyle(style));
    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetTimeInstanceEx2(style, locale, instance);
}

ECode CDateFormatHelper::GetTimeInstanceEx2(
    /* [in] */ Int32 style,
    /* [in] */ ILocale * pLocale,
    /* [out] */ IDateFormat ** ppInstance)
{
    assert(0);
    FAIL_RETURN(CheckTimeStyle(style));
    //return new SimpleDateFormat(LocaleData.get(locale).getTimeFormat(style), locale);
    return E_NOT_IMPLEMENTED;
}

