
#include "ext/frameworkdef.h"
#include "database/CCursorJoiner.h"


ECode CCursorJoiner::constructor(
    /* [in] */ ICursor* cursorLeft,
    /* [in] */ const ArrayOf<String>& columnNamesLeft,
    /* [in] */ ICursor* cursorRight,
    /* [in] */ const ArrayOf<String>& columnNamesRight)
{
    return CursorJoiner::Init(cursorLeft, columnNamesLeft, cursorRight, columnNamesRight);
}

ECode CCursorJoiner::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CursorJoiner::HasNext(result);
}

ECode CCursorJoiner::GetNext(
    /* [out] */ CursorJoinerResult* result)
{
    VALIDATE_NOT_NULL(result);
    return CursorJoiner::GetNext(result);
}

ECode CCursorJoiner::Remove()
{
    return CursorJoiner::Remove();
}
