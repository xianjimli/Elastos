#include "FieldPosition.h"
#include "cmdef.h"
FieldPosition::FieldPosition()
{}

FieldPosition::~FieldPosition()
{}

void FieldPosition::Init(
    /* [in] */ Int32 field)
{
    mMyField = field;
}

void FieldPosition::Init(
    /* [in] */ IFormat_Field* attribute)
{
    mMyAttribute = attribute;
    mMyField = -1;
}

void FieldPosition::Init(
    /* [in] */ IFormat_Field* attribute,
    /* [in] */ Int32 field)
{
    mMyAttribute = attribute;
    mMyField = field;
}

ECode FieldPosition::Clear()
{
    mBeginIndex = mEndIndex = 0;
    return NOERROR;
}

ECode FieldPosition::GetBeginIndex(
    /* [out] */ Int32* beginIndex)
{
    VALIDATE_NOT_NULL(beginIndex);
    *beginIndex = mBeginIndex;
    return NOERROR;
}

ECode FieldPosition::GetEndIndex(
    /* [out] */ Int32* endIndex)
{
    VALIDATE_NOT_NULL(endIndex);
    *endIndex = mEndIndex;
    return NOERROR;
}

ECode FieldPosition::GetField(
    /* [out] */ Int32* field)
{
    VALIDATE_NOT_NULL(field);
    *field = mMyField;
    return NOERROR;
}

ECode FieldPosition::GetFieldAttribute(
    /* [out] */ IFormat_Field** fieldAttribute)
{
    *fieldAttribute = mMyAttribute;
    return NOERROR;
}

ECode FieldPosition::SetBeginIndex(
    /* [in] */ Int32 index)
{
    mBeginIndex = index;
    return NOERROR;
}

ECode FieldPosition::SetEndIndex(
    /* [in] */ Int32 index)
{
    mEndIndex = index;
    return NOERROR;
}