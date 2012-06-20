
#ifndef __HH_CAPSULEHELPER_H
#define __HH_CAPSULEHELPER_H

#include "ext/frameworkext.h"
#include "elastos/AutoPtr.h"
#include <elastos.h>
using namespace Elastos;

class CapsuleHelper
{
public:
    static CARAPI_(AutoPtr<IMountService>) GetMountService()
        { return NULL; }

    static CARAPI GetSecureContainerList(
        /* [out] */ ArrayOf<String>** list)
        { return E_NOT_IMPLEMENTED; }

    static CARAPI_(Boolean) DestroySdDir(
        /* [in] */ String cid)
        { return FALSE; }

public:
    static const Int32 RECOMMEND_INSTALL_INTERNAL = 1;
    static const Int32 RECOMMEND_INSTALL_EXTERNAL = 2;
    static const Int32 RECOMMEND_FAILED_INSUFFICIENT_STORAGE = -1;
    static const Int32 RECOMMEND_FAILED_INVALID_APK = -2;
    static const Int32 RECOMMEND_FAILED_INVALID_LOCATION = -3;
    static const Int32 RECOMMEND_FAILED_ALREADY_EXISTS = -4;
    static const Int32 RECOMMEND_MEDIA_UNAVAILABLE = -5;

private:
    static const Boolean mLocalLOGV = TRUE;
    static const String TAG;

public:
    // App installation location settings values
    static const Int32 APP_INSTALL_AUTO = 0;
    static const Int32 APP_INSTALL_INTERNAL = 1;
    static const Int32 APP_INSTALL_EXTERNAL = 2;
};

#endif // __HH_CAPSULEHELPER_H

