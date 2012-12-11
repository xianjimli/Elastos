
#ifndef __CCURSORJOINER_H__
#define __CCURSORJOINER_H__

#include "_CCursorJoiner.h"
#include "database/CursorJoiner.h"
CarClass(CCursorJoiner), public CursorJoiner
{
public:
    CARAPI HasNext(
        /* [out] */ Boolean * pRst);

    CARAPI Remove();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICursor * pCursorLeft,
        /* [in] */ const ArrayOf<String> & columnNamesLeft,
        /* [in] */ ICursor * pCursorRight,
        /* [in] */ const ArrayOf<String> & columnNamesRight);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCURSORJOINER_H__
