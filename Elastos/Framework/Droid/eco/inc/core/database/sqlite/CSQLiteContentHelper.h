
#ifndef __CSQLITECONTENTHELPER_H__
#define __CSQLITECONTENTHELPER_H__

#include "_CSQLiteContentHelper.h"
#include "SQLiteContentHelper.h"

CarClass(CSQLiteContentHelper),public SQLiteContentHelper
{
public:
    CARAPI GetBlobColumnAsAssetFile(
        /* [in] */ ISQLiteDatabase *db,
        /* [in] */ const String& sql,
        /* [in] */ ArrayOf<String> *selectionArgs,
        /*[out]*/  IAssetFileDescriptor** fd);
};
#endif // __CSQLITECONTENTHELPER_H__
