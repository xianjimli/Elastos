
#ifndef __HH_CAPSULEHELPER_H
#define __HH_CAPSULEHELPER_H

#include "ext/frameworkext.h"
#include "elastos/AutoPtr.h"
#include <elastos.h>
using namespace Elastos;

class CapsuleHelper
{
public:
    static CARAPI_(AutoPtr<IMountService>) GetMountService();

    static CARAPI_(String) CreateSdDir(
        /* [in] */ Int64 sizeBytes,
        /* [in] */ const String& cid,
        /* [in] */ const String& sdEncKey,
        /* [in] */ Int32 uid);

    static CARAPI_(String) MountSdDir(
        /* [in] */ const String& cid,
        /* [in] */ const String& key,
        /* [in] */ Int32 ownerUid);

    static CARAPI_(Boolean) UnMountSdDir(
        /* [in] */ const String& cid);

    static CARAPI_(Boolean) RenameSdDir(
        /* [in] */ const String& oldId,
        /* [in] */ const String& newId);

    static CARAPI_(String) GetSdDir(
        /* [in] */ const String& cid);

    static CARAPI_(Boolean) FinalizeSdDir(
        /* [in] */ const String& cid);

    static CARAPI_(Boolean) DestroySdDir(
        /* [in] */ const String& cid);

    static CARAPI GetSecureContainerList(
        /* [out, callee] */ ArrayOf<String>** list);

    static CARAPI_(Boolean) IsContainerMounted(
        /* [in] */ const String& cid);

public:
    static const Int32 RECOMMEND_INSTALL_INTERNAL = 1;
    static const Int32 RECOMMEND_INSTALL_EXTERNAL = 2;
    static const Int32 RECOMMEND_FAILED_INSUFFICIENT_STORAGE = -1;
    static const Int32 RECOMMEND_FAILED_INVALID_APK = -2;
    static const Int32 RECOMMEND_FAILED_INVALID_LOCATION = -3;
    static const Int32 RECOMMEND_FAILED_ALREADY_EXISTS = -4;
    static const Int32 RECOMMEND_MEDIA_UNAVAILABLE = -5;

    // App installation location settings values
    static const Int32 APP_INSTALL_AUTO = 0;
    static const Int32 APP_INSTALL_INTERNAL = 1;
    static const Int32 APP_INSTALL_EXTERNAL = 2;

private:
    static const Boolean mLocalLOGV = TRUE;
    static CString TAG;
};

#endif // __HH_CAPSULEHELPER_H

