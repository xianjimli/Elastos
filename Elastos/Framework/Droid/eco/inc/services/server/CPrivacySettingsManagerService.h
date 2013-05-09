
#ifndef __CPRIVACYSETTINGSMANAGERSERVICE_H__
#define __CPRIVACYSETTINGSMANAGERSERVICE_H__

#include "_CPrivacySettingsManagerService.h"

CarClass(CPrivacySettingsManagerService)
{
public:
    CARAPI GetSettings(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [out] */ IPrivacySettings ** ppPrivacySettings);

    CARAPI SaveSettings(
        /* [in] */ IPrivacySettings * pSettings,
        /* [out] */ Boolean * pResult);

    CARAPI DeleteSettings(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean * pResult);

    CARAPI GetVersion();

    CARAPI Notification(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ Byte accessMode,
        /* [in] */ const String& dataType,
        /* [in] */ const String& output);

    CARAPI RegisterObservers();

    CARAPI AddObserver(
        /* [in] */ const String& packageName);

    CARAPI PurgeSettings(
        /* [out] */ Boolean * pResult);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    IContext*       mContext;
};

#endif // __CPRIVACYSETTINGSMANAGERSERVICE_H__
