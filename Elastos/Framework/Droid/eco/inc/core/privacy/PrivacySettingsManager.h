
#ifndef __PRIVACYSETTINGSMANAGER_H__
#define __PRIVACYSETTINGSMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

class PrivacySettingsManager
    : public ElRefBase
    , public IPrivacySettingsManager
{
public:
    PrivacySettingsManager(
        /* [in] */ IContext* context,
        /* [in] */ IPrivacySettingsManager* service);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetSettings(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 uid,
        /* [out] */ IPrivacySettings** privactySettings);

    CARAPI SaveSettings(
        /* [in] */ IPrivacySettings* settings,
        /* [out] */ Boolean* result);

    CARAPI DeleteSettings(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* result);

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

    CARAPI GetVersion(
        /* [out] */ Double* result);

private:
    AutoPtr<IPrivacySettingsManager> mService;
};

#endif // __PRIVACYSETTINGSMANAGER_H__
