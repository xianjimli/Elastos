
#ifndef __CPRIVACYSETTINGSMANAGER_H__
#define __CPRIVACYSETTINGSMANAGER_H__

#include "_CPrivacySettingsManager.h"

CarClass(CPrivacySettingsManager)
{
public:
    CARAPI GetSettings(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [out] */ IPrivacySettings ** ppPrivactySettings);

    CARAPI SaveSettings(
        /* [in] */ IPrivacySettings * pSettings,
        /* [out] */ Boolean * pResult);

    CARAPI DeleteSettings(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean * pResult);

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

    CARAPI GetVersion(
        /* [out] */ Double * pResult);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IPrivacySettingsManager * pService);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPRIVACYSETTINGSMANAGER_H__
