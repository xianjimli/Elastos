
#ifndef __CPRIVACYPERSISTENCEADAPTER_H__
#define __CPRIVACYPERSISTENCEADAPTER_H__

#include "_CPrivacyPersistenceAdapter.h"
#include <sqlite3.h>

CarClass(CPrivacyPersistenceAdapter)
{
public:
    CPrivacyPersistenceAdapter();

    CARAPI GetSettings(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean forceCloseDB,
        /* [out] */ IPrivacySettings ** ppPrivacySettings);

    CARAPI SaveSettings(
        /* [in] */ IPrivacySettings * pS,
        /* [out] */ Boolean * pResult);

    CARAPI DeleteSettings(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean * pResult);

    CARAPI PurgeSettings(
        /* [out] */ Boolean * pResult);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    void CreateDatabase();
    void UpgradeDatabase(Int32 curVersion);
    sqlite3* GetReadableDatabase();

    void CreateSettingsDir();
    Int32 GetVersion();


    static const char* LOG_TAG;
    static const char* mDBFileName;
    static const char* mSettingsDir;
    static const Int32 mDBVersion = 2;
    static const char* mCreateSettingsTabStr;
    static const char* mCreateVersionTabStr;
    static const char* mInsetVersionStr;

    static Int32 mReadingThreadsCount;

    sqlite3*        mSqlDB;
    IContext*       mContext;
};

#endif // __CPRIVACYPERSISTENCEADAPTER_H__
