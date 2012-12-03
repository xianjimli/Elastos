
#ifndef __CSQLITEOPENHELPER_H__
#define __CSQLITEOPENHELPER_H__

#include "_CSQLiteOpenHelper.h"
#include "SQLiteOpenHelper.h"

CarClass(CSQLiteOpenHelper),public SQLiteOpenHelper
{
public:
    CARAPI GetWritableDatabase(
        /* [out] */ ISQLiteDatabase **database);

    CARAPI GetReadableDatabase(
        /* [out] */ ISQLiteDatabase **database);

    CARAPI Close();

    CARAPI OnCreate(
        /* [in] */ ISQLiteDatabase *database);

    CARAPI OnUpgrade(
        /* [in] */ ISQLiteDatabase *db,
        /* [in] */ Int32 oldVersion,
        /* [in] */ Int32 newVersion);

    CARAPI OnOpen(
        /* [in] */ ISQLiteDatabase *db);

    CARAPI constructor(
        /* [in] */ IContext *context,
        /* [in] */ const String& name,
        /* [in] */ ICursorFactory *factory,
        /* [in] */ Int32 number);
};

#endif // __CSQLITEOPENHELPER_H__
