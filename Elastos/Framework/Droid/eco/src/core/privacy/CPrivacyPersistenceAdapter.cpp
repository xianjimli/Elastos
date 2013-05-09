
#include "privacy/CPrivacyPersistenceAdapter.h"
#include <sqlite3.h>

const char* CPrivacyPersistenceAdapter::mDBFileName     = "/data/system/privacy.db";
const char* CPrivacyPersistenceAdapter::mSettingsDir    = "/data/system/privacy";

ECode CPrivacyPersistenceAdapter::constructor(
    /* [in] */ IContext * pContext)
{
    mContext = pContext;

    // check write permission for /data/system/
    IFile* path;
    Boolean canWrite;
    CFile::New(String("/data/system"), &path);
    path->CanWrite(&canWrite);
    path->Release();

    // create the database and settings directory if we have write permission and they do not exist
    if (canWrite) {
        IFile* file;
        Boolean isExist;
        CFile::New(String(mDBFileName), &file);
        file->Exists(&isExist);
        file->Release();
        if (!isExist)
            CreateDatabase();

        CFile::New(String(mSettingsDir), &file);
        file->Exists(&isExist);
        file->Release();
        if (!isExist)
            CreateSettingsDir();

        // upgrade if needed
        Int32 curVersion = GetVersion();
        if (curVersion < mDBVersion)
            UpgradeDatabase(curVersion);
    }

    return NOERROR;
}

ECode CPrivacyPersistenceAdapter::GetSettings(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean forceCloseDB,
    /* [out] */ IPrivacySettings ** ppPrivacySettings)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacyPersistenceAdapter::SaveSettings(
    /* [in] */ IPrivacySettings * pS,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacyPersistenceAdapter::DeleteSettings(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacyPersistenceAdapter::PurgeSettings(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

void CPrivacyPersistenceAdapter::CreateDatabase()
{
    // todo Synchronized
    /*
    try {
   +             SQLiteDatabase db =
   +                 SQLiteDatabase.openDatabase(DATABASE_FILE, null, SQLiteDatabase.OPEN_READWRITE |
   +                         SQLiteDatabase.CREATE_IF_NECESSARY);
   +             Log.i(TAG, "createDatabase - creating privacy database");
   +             db.execSQL(CREATE_TABLE_SETTINGS);
   +             db.execSQL(CREATE_TABLE_VERSION);
   +             db.execSQL(INSERT_VERSION);
   +     //        Log.d(TAG, "createDatabase - closing connection to privacy.db");
   +             if (db != null && db.isOpen()) db.close();
   +         } catch (SQLException e) {
   +             Log.e(TAG, "createDatabase - failed to create privacy database", e);
   +         }
   */
    sqlite3 *db = NULL;
    sqlite3_open(mDBFileName, &db);
}

void CPrivacyPersistenceAdapter::CreateSettingsDir()
{
}

Int32 CPrivacyPersistenceAdapter::GetVersion()
{
    return 0;
}

void CPrivacyPersistenceAdapter::UpgradeDatabase(Int32 curVersion)
{
}
