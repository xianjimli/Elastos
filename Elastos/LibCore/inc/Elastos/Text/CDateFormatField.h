
#ifndef __CDATEFORMATFIELD_H__
#define __CDATEFORMATFIELD_H__

#include "_CDateFormatField.h"

CarClass(CDateFormatField)
{
public:
    CARAPI constructor(
        /* [in] */ const String& fieldName,
        /* [in] */ Int32 calendarField);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetCalendarField(
        /* [out] */ Int32* field);
};

#endif //__CDATEFORMATFIELD_H__
