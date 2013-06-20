
#include "cmdef.h"
#include "CDateFormatFieldHelper.h"
#include "DateFormat.h"

ECode CDateFormatFieldHelper::OfCalendarField(
    /* [in] */ Int32 calendarField,
    /* [out] */ IDateFormatField** field)
{
    return DateFormat::Field::OfCalendarField(calendarField, field);
}

ECode CDateFormatFieldHelper::GetDateFormatFieldByName(
    /* [in] */ String name,
    /* [out] */ IDateFormatField** field)
{
    VALIDATE_NOT_NULL(field);
    if (name.Equals("era")) {
        *field = DateFormat::Field::ERA;
    }
    else if (name.Equals("year")) {
        *field = DateFormat::Field::YEAR;
    }
    else {
        *field = NULL;
        return NOERROR;
    }

    (*field)->AddRef();
    return NOERROR;
}
