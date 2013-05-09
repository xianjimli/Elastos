
#ifndef __CPRIVACYPERSISTENCEADAPTER_H__
#define __CPRIVACYPERSISTENCEADAPTER_H__

#include "_CPrivacyPersistenceAdapter.h"

CarClass(CPrivacyPersistenceAdapter)
{
public:
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
    void CreateSettingsDir();
    Int32 GetVersion();
    void UpgradeDatabase(Int32 curVersion);

    static const char* mDBFileName;
    static const char* mSettingsDir;
    static const Int32 mDBVersion = 2;

    IContext*       mContext;
};

#endif // __CPRIVACYPERSISTENCEADAPTER_H__
