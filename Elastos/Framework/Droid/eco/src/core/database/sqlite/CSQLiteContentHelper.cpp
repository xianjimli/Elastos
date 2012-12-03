
#include "database/sqlite/CSQLiteContentHelper.h"
ECode CSQLiteContentHelper::GetBlobColumnAsAssetFile(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<String> *selectionArgs,
    /*[out]*/ IAssetFileDescriptor** fd)
{
    return SQLiteContentHelper::GetBlobColumnAsAssetFile(db, sql, selectionArgs,fd);
}

