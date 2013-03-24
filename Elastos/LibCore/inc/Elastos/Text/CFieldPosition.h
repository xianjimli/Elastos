
#ifndef __CFIELDPOSITION_H__
#define __CFIELDPOSITION_H__

#include "_CFieldPosition.h"
#include <elastos/AutoPtr.h>

CarClass(CFieldPosition)
{
public:
    CFieldPosition();

    CARAPI constructor(
        /* [in] */ Int32 field);

    CARAPI constructor(
        /* [in] */ IFormatField* attribute);

    CARAPI constructor(
        /* [in] */ IFormatField* attribute,
        /* [in] */ Int32 field);

    CARAPI_(void) Clear();

    CARAPI GetBeginIndex(
        /* [out] */ Int32* index);

    CARAPI GetEndIndex(
        /* [out] */ Int32* index);

    CARAPI GetField(
        /* [out] */ Int32* field);

    CARAPI GetFieldAttribute(
        /* [out] */ IFormatField** attribute);

    CARAPI SetBeginIndex(
        /* [in] */ Int32 index);

    CARAPI SetEndIndex(
        /* [in] */ Int32 index);

private:
    Int32 mMyField;
    Int32 mBeginIndex;
    Int32 mEndIndex;

    AutoPtr<IFormatField> mMyAttribute;
};

#endif // __CFIELDPOSITION_H__
