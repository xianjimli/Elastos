
#include "database/sqlite/CSQLiteDebug.h"
ECode CSQLiteDebug::GetDatabaseInfo()
{
//    return SQLiteDebug::GetDatabaseInfo();
      return E_NOT_IMPLEMENTED;
}

ECode CSQLiteDebug::GetNumActiveCursorsFinalized(
    /* [out] */ Int32 *number)
{
    return SQLiteDebug::GetNumActiveCursorsFinalized(number);
}

ECode CSQLiteDebug::NotifyActiveCursorFinalized()
{
    return SQLiteDebug::NotifyActiveCursorFinalized();
}

