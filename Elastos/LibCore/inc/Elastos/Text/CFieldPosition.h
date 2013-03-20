
#ifndef __CFIELDPOSITION_H__
#define __CFIELDPOSITION_H__

#include "_CFieldPosition.h"
#include "FieldPosition.h"

CarClass(CFieldPosition), public FieldPosition
{
public:
    CARAPI constructor(
        /* [in] */ Int32 field);

    CARAPI constructor(
        /* [in] */ IFormat_Field * pAttribute);

    CARAPI constructor(
        /* [in] */ IFormat_Field * pAttribute,
        /* [in] */ Int32 field);

    CARAPI Clear();

    CARAPI GetBeginIndex(
        /* [out] */ Int32 * pBeginIndex);

    CARAPI GetEndIndex(
        /* [out] */ Int32 * pEndIndex);

    CARAPI GetField(
        /* [out] */ Int32 * pField);

    CARAPI GetFieldAttribute(
        /* [out] */ IFormat_Field ** ppFieldAttribute);

    CARAPI SetBeginIndex(
        /* [in] */ Int32 index);

    CARAPI SetEndIndex(
        /* [in] */ Int32 index);

private:
    // TODO: Add your private member variables here.
};

#endif // __CFIELDPOSITION_H__
