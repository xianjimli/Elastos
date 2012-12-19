
#ifndef __CCURSORJOINER_H__
#define __CCURSORJOINER_H__

#include "_CCursorJoiner.h"
#include "database/CursorJoiner.h"

CarClass(CCursorJoiner), public CursorJoiner
{
public:
    CARAPI constructor(
        /* [in] */ ICursor* cursorLeft,
        /* [in] */ const ArrayOf<String>& columnNamesLeft,
        /* [in] */ ICursor* cursorRight,
        /* [in] */ const ArrayOf<String>& columnNamesRight);

    CARAPI HasNext(
        /* [out] */ Boolean* result);

    CARAPI GetNext(
        /* [out] */ CursorJoinerResult* result);

    CARAPI Remove();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCURSORJOINER_H__
