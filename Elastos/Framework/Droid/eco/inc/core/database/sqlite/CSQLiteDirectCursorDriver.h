
#ifndef __CSQLITEDIRECTCURSORDRIVER_H__
#define __CSQLITEDIRECTCURSORDRIVER_H__

#include "_CSQLiteDirectCursorDriver.h"
#include "SQLiteDirectCursorDriver.h"

CarClass(CSQLiteDirectCursorDriver),public SQLiteDirectCursorDriver
{
public:
    CARAPI Query(
        /* [in] */ ICursorFactory *factory,
        /* [in] */ ArrayOf<String>* bindArgs,
        /* [out] */ ICursor **cursor);

    CARAPI CursorDeactivated();

    CARAPI CursorRequeried(
        /* [in] */ ICursor *cursor);

    CARAPI CursorClosed();

    CARAPI SetBindArguments(
        /* [in] */ ArrayOf<String>* bindArgs);

    CARAPI constructor(
        /* [in] */ ISQLiteDatabase *pb,
        /* [in] */ const String &sql,
        /* [in] */ const String &editTable);
};

#endif // __CSQLITEDIRECTCURSORDRIVER_H__
