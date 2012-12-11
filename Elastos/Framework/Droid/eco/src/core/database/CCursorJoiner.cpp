
#include "database/CCursorJoiner.h"
ECode CCursorJoiner::HasNext(
    /* [out] */ Boolean * pRst)
{
    CursorJoiner::HasNext(pRst);
    return NOERROR;
}

ECode CCursorJoiner::Remove()
{
    CursorJoiner::Remove();
    return NOERROR;
}

ECode CCursorJoiner::constructor()
{
    CursorJoiner::CursorJoiner();
    return NOERROR;
}

ECode CCursorJoiner::constructor(
    /* [in] */ ICursor * pCursorLeft,
    /* [in] */ const ArrayOf<String> & columnNamesLeft,
    /* [in] */ ICursor * pCursorRight,
    /* [in] */ const ArrayOf<String> & columnNamesRight)
{
    CursorJoiner::Init(pCursorLeft, columnNamesLeft, pCursorRight, columnNamesRight);
    return NOERROR;
}

