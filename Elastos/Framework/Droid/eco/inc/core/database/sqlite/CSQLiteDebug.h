
#ifndef __CSQLITEDEBUG_H__
#define __CSQLITEDEBUG_H__

#include "_CSQLiteDebug.h"
#include "SQLiteDebug.h"

CarClass(CSQLiteDebug), public SQLiteDebug
{
public:
    CARAPI GetDatabaseInfo();

    CARAPI GetNumActiveCursorsFinalized(
        /* [out] */ Int32 *number);

    CARAPI NotifyActiveCursorFinalized();
};

#endif // __CSQLITEDEBUG_H__
