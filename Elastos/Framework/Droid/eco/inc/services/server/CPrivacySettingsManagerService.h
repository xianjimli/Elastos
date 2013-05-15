
#ifndef __CPRIVACYSETTINGSMANAGERSERVICE_H__
#define __CPRIVACYSETTINGSMANAGERSERVICE_H__

#include "_CPrivacySettingsManagerService.h"
#include <elastos/AutoPtr.h>

CarClass(CPrivacySettingsManagerService)
{
public:
    CARAPI GetSettings(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 uid,
        /* [out] */ IPrivacySettings** privacySettings);

    CARAPI SaveSettings(
        /* [in] */ IPrivacySettings* settings,
        /* [out] */ Boolean* result);

    CARAPI DeleteSettings(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* result);

    CARAPI GetVersion(
        /* [out] */ Double* version);

    CARAPI Notification(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 uid,
        /* [in] */ Byte accessMode,
        /* [in] */ const String& dataType,
        /* [in] */ const String& output);

    CARAPI RegisterObservers();

    CARAPI AddObserver(
        /* [in] */ const String& capsuleName);

    CARAPI PurgeSettings(
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    AutoPtr<IContext> mContext;
    IPrivacyPersistenceAdapter* mPersistence;
};

#endif // __CPRIVACYSETTINGSMANAGERSERVICE_H__
