
#ifndef __CCURSORWRAPPER_H__
#define __CCURSORWRAPPER_H__

#include "_CCursorWrapper.h"
#include "database/CursorWrapper.h"
#include "database/CursorMacro.h"

CarClass(CCursorWrapper), public CursorWrapper
{
public:
    CARAPI constructor(
        /* [in] */ ICursor* cursor);

    ICURSOR_METHODS_DECL();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCURSORWRAPPER_H__
