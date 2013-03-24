
#include "cmdef.h"
#include "CFieldPosition.h"


CFieldPosition::CFieldPosition()
    : mMyField(0)
    , mBeginIndex(0)
    , mEndIndex(0)
{}

ECode CFieldPosition::constructor(
    /* [in] */ Int32 field)
{
    mMyField = field;
    return NOERROR;
}

ECode CFieldPosition::constructor(
    /* [in] */ IFormatField* attribute)
{
    mMyAttribute = attribute;
    mMyField = -1;
    return NOERROR;
}

ECode CFieldPosition::constructor(
    /* [in] */ IFormatField* attribute,
    /* [in] */ Int32 field)
{
    mMyAttribute = attribute;
    mMyField = field;
    return NOERROR;
}

void CFieldPosition::Clear()
{
    mBeginIndex = mEndIndex = 0;
}

ECode CFieldPosition::GetBeginIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = mBeginIndex;
    return NOERROR;
}

ECode CFieldPosition::GetEndIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = mEndIndex;
    return NOERROR;
}

ECode CFieldPosition::GetField(
    /* [out] */ Int32* field)
{
    VALIDATE_NOT_NULL(field);

    *field = mMyField;
    return NOERROR;
}

ECode CFieldPosition::GetFieldAttribute(
    /* [out] */ IFormatField** attribute)
{
    VALIDATE_NOT_NULL(attribute);

    *attribute = mMyAttribute;
    if (*attribute != NULL) (*attribute)->AddRef();
    return NOERROR;
}

ECode CFieldPosition::SetBeginIndex(
    /* [in] */ Int32 index)
{
    mBeginIndex = index;
    return NOERROR;
}

ECode CFieldPosition::SetEndIndex(
    /* [in] */ Int32 index)
{
    mEndIndex = index;
    return NOERROR;
}
