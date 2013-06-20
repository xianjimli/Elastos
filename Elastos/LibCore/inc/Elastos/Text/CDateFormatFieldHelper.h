
#ifndef __CDATEFORMATFIELDHELPER_H__
#define __CDATEFORMATFIELDHELPER_H__

#include "_CDateFormatFieldHelper.h"

CarClass(CDateFormatFieldHelper)
{
public:
    /**
     * Returns the Calendar field that this field represents.
     */
    CARAPI OfCalendarField(
        /* [in] */ Int32 calendarField,
        /* [out] */ IDateFormatField** field);

    CARAPI GetDateFormatFieldByName(
        /* [in] */ String name,
        /* [out] */ IDateFormatField** field);
};

#endif // __CDATEFORMATFIELDHELPER_H__
