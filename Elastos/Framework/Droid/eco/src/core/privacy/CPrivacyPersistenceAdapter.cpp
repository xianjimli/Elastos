
#include "privacy/CPrivacyPersistenceAdapter.h"
#include <stdio.h>
#include <stdlib.h>
#include "privacy/CPrivacySettings.h"

Int32 CPrivacyPersistenceAdapter::mReadingThreadsCount = 0;

const char* CPrivacyPersistenceAdapter::LOG_TAG         = "PrivacyPersistenceAdapter";
const char* CPrivacyPersistenceAdapter::mDBFileName     = "/data/system/privacy.db";
const char* CPrivacyPersistenceAdapter::mSettingsDir    = "/data/system/privacy";

const char* CPrivacyPersistenceAdapter::mCreateSettingsTabStr = "CREATE TABLE IF NOT EXISTS " \
                                                                "settings" \
                                                                " ( " \
                                                                " _id INTEGER PRIMARY KEY AUTOINCREMENT, " \
                                                                " packageName TEXT, " \
                                                                " uid INTEGER, " \
                                                                " deviceIdSetting INTEGER, " \
                                                                " deviceId TEXT, " \
                                                                " line1NumberSetting INTEGER, " \
                                                                " line1Number TEXT, " \
                                                                " locationGpsSetting INTEGER, " \
                                                                " locationGpsLat TEXT, " \
                                                                " locationGpsLon TEXT, " \
                                                                " locationNetworkSetting INTEGER, " \
                                                                " locationNetworkLat TEXT, " \
                                                                " locationNetworkLon TEXT, " \
                                                                " networkInfoSetting INTEGER, " \
                                                                " simInfoSetting INTEGER, " \
                                                                " simSerialNumberSetting INTEGER, " \
                                                                " simSerialNumber TEXT, " \
                                                                " subscriberIdSetting INTEGER, " \
                                                                " subscriberId TEXT, " \
                                                                " accountsSetting INTEGER, " \
                                                                " accountsAuthTokensSetting INTEGER, " \
                                                                " outgoingCallsSetting INTEGER, " \
                                                                " incomingCallsSetting INTEGER, " \
                                                                " contactsSetting INTEGER, " \
                                                                " calendarSetting INTEGER, " \
                                                                " mmsSetting INTEGER, " \
                                                                " smsSetting INTEGER, " \
                                                                " callLogSetting INTEGER, " \
                                                                " bookmarksSetting INTEGER, " \
                                                                " systemLogsSetting INTEGER, " \
                                                                " externalStorageSetting INTEGER, " \
                                                                " cameraSetting INTEGER, " \
                                                                " recordAudioSetting INTEGER, " \
                                                                " notificationSetting INTEGER, " \
                                                                " intentBootCompletedSetting INTEGER" \
                                                                ");";

const char* CPrivacyPersistenceAdapter::mCreateVersionTabStr = "CREATE TABLE IF NOT EXISTS " \
                                                               "version" \
                                                               " ( _id INTEGER PRIMARY KEY, version INTEGER );";

const char* CPrivacyPersistenceAdapter::mInsetVersionStr = "INSERT OR REPLACE INTO " \
                                                           "version " \
                                                           "(_id, version) " \
                                                           "VALUES (1, 2);";

static char* intToStr(Int32 number)
{
    char* str = (char*)malloc(sizeof(char) * 25);
    Int32 index = 0;

    if (0 == number) {
        str[index++] = 0X30;
    }

    while (number != 0) {
        str[index++] = number % 10 + 0X30;
        number /= 10;
    }

    str[index] = '\0';

    char temp;
    for (int i = 0; i < index / 2; ++i) {
        temp = str[i];
        str[i] = str[index - 1 - i];
        str[index - 1 - i] = temp;
    }

    return str;
}

CPrivacyPersistenceAdapter::CPrivacyPersistenceAdapter()
: mSqlDB(NULL), mContext(NULL)
{
}

ECode CPrivacyPersistenceAdapter::constructor(
    /* [in] */ IContext * pContext)
{
    mContext    = pContext;
    mSqlDB      = NULL;
    mSqlDBW     = NULL;

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
    /* [out] */ IPrivacySettings** ppPrivacySettings)
{
    assert(packageName.GetLength() > 0);
    *ppPrivacySettings = NULL;

    if (packageName.GetLength() <= 0) {
        printf("%s ERROR: param package name is invalid", LOG_TAG);
        return NOERROR;
    }

    // indicate that the DB is being read to prevent closing by other threads
    ++mReadingThreadsCount;

    sqlite3* db = GetReadableDatabase();
    assert(NULL != db);

    // try to get settings based on package name only first; some system applications
    // (e.g., HTC Settings) run with a different UID than reported by package manager
    String queryStr;
    queryStr.AppendFormat("%s%s%s", "select * from settings where packageName='",
                          packageName.string(), "'");

    char** queryResult;
    char** errorMsg;
    int row, column;
    if (SQLITE_OK != sqlite3_get_table(db, queryStr.string(), &queryResult,
                                       &row, &column, errorMsg)) {
        printf("%s ERROR: querying where %s from settings fail by %s",
                LOG_TAG, packageName.string(), *errorMsg);
        return NOERROR;
    }

    if (row > 1) {
        // if we get multiple entries, try using UID as well; not guaranteed to find existing
        // settings for system applications (see above comment) but this is rather rare
        String uidStr;
        uidStr.AppendFormat("%d", uid);
        queryStr.AppendFormat("%s%s%s%s%s", "select * from settings where packageName='",
                              packageName.string(), "' and uid='", uidStr.string(), "'");
        if (SQLITE_OK != sqlite3_get_table(db, queryStr.string(), &queryResult,
                                               &row, &column, errorMsg)) {
            printf("%s ERROR: querying where %s from settings fail by %s",
                   LOG_TAG, packageName.string(), *errorMsg);
            return NOERROR;
        }
    }

    if (1 == row) {
        Int32 id = 0;
        Int32 index = 0;
        while ('\0' != queryResult[column][index]) {
            id = (id * 10) + (queryResult[column][index] - 48);
            ++index;
        }

        Int32 uid = 0;
        index = 0;
        while ('\0' != queryResult[column + 2][index]) {
            uid = (id * 10) + (queryResult[column + 2][index] - 48);
            ++index;
        }

        Byte locationGpsSetting = 0;
        index = 0;
        while ('\0' != queryResult[column + 7][index]) {
            locationGpsSetting = (locationGpsSetting * 10) +
                                 (queryResult[column + 7][index] - 48);
            ++index;
        }

        CPrivacySettings::New(id, String(queryResult[column + 1]), locationGpsSetting, uid,
                              String(queryResult[column + 8]), String(queryResult[column + 9]),
                              ppPrivacySettings);

        return NOERROR;
    } else {
        printf("%s ERROR: getSettings - duplicate entries in the privacy.db", LOG_TAG);
    }

    return NOERROR;
}

ECode CPrivacyPersistenceAdapter::SaveSettings(
    /* [in] */ IPrivacySettings* settings,
    /* [out] */ Boolean * result)
{
    // todo: Synchronization
    *result = true;
    if (NULL == settings) {
        *result = false;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String packageName;
    settings->GetCapsuleName(&packageName);
    if (packageName.GetLength() <= 0) {
        printf("%s ERROR: try to saving a privacy with an invalid capsule name\n",
               LOG_TAG);
        *result = false;
        return NOERROR;
    }

    sqlite3* db = GetWritableDatabase();    assert(NULL != db);
    String sqlTxt;
    char* errMsg;
    Int32 ID;
    settings->GetID(&ID);
    char* idStr = intToStr(ID);
    Int32 lGPSSetting;
    settings->GetLocationGpsSetting((Byte*)&lGPSSetting);
    char* GPSSettingStr = intToStr(lGPSSetting);
    String lat, lon;
    settings->GetLocationGpsLat(&lat);
    settings->GetLocationGpsLon(&lon);

    if (ID >= 0) {
        // Note: Use a negative value to identify that this is a value from DB
        sqlTxt.AppendFormat("update settings set locationGpsSetting=%s,locationGpsLat='%s',locationGpsLon='%s' where _id='%'",
                GPSSettingStr, lat.string(), lon.string(), idStr);
        if (SQLITE_OK != sqlite3_exec(db, sqlTxt, NULL, NULL, &errMsg)) {
            printf("%s ERROR: Update db fail: %s", LOG_TAG, errMsg);
            *result = false;
            return NOERROR;
        }
    } else {
        sqlTxt.AppendFormat("%s%s%s", "select * from settings where packageName='",
                                  packageName.string(), "'");
        char** queryResult;
        int row, column;
        if (SQLITE_OK != sqlite3_get_table(db, sqlTxt.string(), &queryResult,
                                              &row, &column, &errMsg)) {
            printf("%s ERROR: querying where %s from settings fail by %s",
                   LOG_TAG, packageName.string(), errMsg);
            *result = false;
            return NOERROR;
        }

        String secSqlTxt;
        if (1 == row) {
            secSqlTxt.AppendFormat("update settings set locationGpsSetting=%s,locationGpsLat='%s',locationGpsLon='%s' where _id='%'",
                   GPSSettingStr, lat.string(), lon.string(), idStr);
            if (SQLITE_OK != sqlite3_exec(db, secSqlTxt, NULL, NULL, &errMsg)) {
               printf("%s ERROR: create settings table fail: %s", LOG_TAG, errMsg);
               *result = false;
               return NOERROR;
            }
        } else if (0 == row) {
            secSqlTxt.AppendFormat("INSERT INTO settings (packageName, locationGpsSetting, locationGpsLat, locationGpsLon)  VALUES (%s, %s, %s, %s);",
                                    packageName.string(), lat.string(), lon.string());
            if (SQLITE_OK != sqlite3_exec(db, secSqlTxt, NULL, NULL, &errMsg)) {
                printf("%s ERROR: create settings table fail: %s", LOG_TAG, errMsg);
                *result = false;
                return NOERROR;
            }
       } else if (row > 1) {
           printf("%s ERROR: duplicated entry for package name: %s", LOG_TAG, packageName.string());
           *result = false;
           return NOERROR;
       } else {
           // error message
           printf("%s ERROR: No opeation for this state: %s", LOG_TAG, packageName.string());
           *result = false;
           return NOERROR;
       }
    }

    return NOERROR;
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

    sqlite3* db = NULL;
    char* errMsg;
    if (SQLITE_OK != sqlite3_open(mDBFileName, &db)) {
        printf("%s ERROR: Create/Open DB file fail", LOG_TAG);
    } else {
        if (SQLITE_OK != sqlite3_exec(db, mCreateSettingsTabStr, NULL, NULL, &errMsg)) {
            printf("%s ERROR: create settings table fail: %s", LOG_TAG, errMsg);
        }

        if (SQLITE_OK != sqlite3_exec(db, mCreateVersionTabStr, NULL, NULL, &errMsg)) {
            printf("%s ERROR: create version table fail: %s", LOG_TAG, errMsg);
        }

        if (SQLITE_OK != sqlite3_exec(db, mInsetVersionStr, NULL, NULL, &errMsg)) {
            printf("%s ERROR: insert version table fail: %s", LOG_TAG, errMsg);
        }

        sqlite3_close(db);
    }
}

sqlite3* CPrivacyPersistenceAdapter::GetReadableDatabase() {
    if (NULL == mSqlDB) {
        if (SQLITE_OK == sqlite3_open_v2(mDBFileName, &mSqlDB, SQLITE_OPEN_READONLY, NULL)) {
           return mSqlDB;
        }
    }

    return NULL;
}

sqlite3* CPrivacyPersistenceAdapter::GetWritableDatabase() {
    if (NULL == mSqlDBW) {
        if (SQLITE_OK == sqlite3_open_v2(mDBFileName, &mSqlDBW, SQLITE_OPEN_READWRITE, NULL)) {
            return mSqlDBW;
        }
    }

    return NULL;
}

void CPrivacyPersistenceAdapter::CreateSettingsDir()
{
    // todo: Synchorized

    // create settings directory (for settings accessed from core libraries)
    IFile* file;
    Boolean result;
    CFile::New(String("/data/system/privacy/"), &file);
    assert(NULL != file);
    file->Mkdirs(&result);
    assert(true == result);

    // make it readable for everybody
    file->SetReadable(true, false, &result);
    assert(true == result);

    file->SetExecutable(true, false, &result);
    assert(true == result);
}

Int32 CPrivacyPersistenceAdapter::GetVersion()
{
    // Template solution by Liwin
    return mDBVersion;
}

void CPrivacyPersistenceAdapter::UpgradeDatabase(Int32 curVersion)
{
    // NOT_IMPLEMENTED
}
