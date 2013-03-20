#include "cmdef.h"
#include "CFieldPosition.h"
ECode CFieldPosition::constructor(
    /* [in] */ Int32 field)
{
    FieldPosition::Init(field);
    return NOERROR;
}

ECode CFieldPosition::constructor(
    /* [in] */ IFormat_Field * pAttribute)
{
    FieldPosition::Init(pAttribute);
    return NOERROR;
}

ECode CFieldPosition::constructor(
    /* [in] */ IFormat_Field * pAttribute,
    /* [in] */ Int32 field)
{
    FieldPosition::Init(pAttribute, field);
    return NOERROR;
}

ECode CFieldPosition::Clear()
{
    return FieldPosition::Clear();
}

ECode CFieldPosition::GetBeginIndex(
    /* [out] */ Int32 * pBeginIndex)
{
    VALIDATE_NOT_NULL(pBeginIndex);

    return FieldPosition::GetBeginIndex(pBeginIndex);
}

ECode CFieldPosition::GetEndIndex(
    /* [out] */ Int32 * pEndIndex)
{
    VALIDATE_NOT_NULL(pEndIndex);

    return FieldPosition::GetEndIndex(pEndIndex);;
}

ECode CFieldPosition::GetField(
    /* [out] */ Int32 * pField)
{
    VALIDATE_NOT_NULL(pField);

    return FieldPosition::GetField(pField);
}

ECode CFieldPosition::GetFieldAttribute(
    /* [out] */ IFormat_Field ** ppFieldAttribute)
{

    return FieldPosition::GetFieldAttribute(ppFieldAttribute);
}

ECode CFieldPosition::SetBeginIndex(
    /* [in] */ Int32 index)
{
    
    return FieldPosition::SetBeginIndex(index);
}

ECode CFieldPosition::SetEndIndex(
    /* [in] */ Int32 index)
{
    
    return FieldPosition::SetEndIndex(index);
}


