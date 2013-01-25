
#ifndef __CWEBSTORAGE_H__
#define __CWEBSTORAGE_H__

#include "_CWebStorage.h"

CarClass(CWebStorage)
{
public:
    CARAPI CreateUIHandler();

    CARAPI CreateHandler();

    CARAPI GetOrigins(
        /* [in] */ IValueCallback * pCallBack);

    CARAPI GetOriginsSync();

    CARAPI GetUsageForOrigin(
        /* [in] */ CString origin,
        /* [in] */ IValueCallback * pCallBack);

    CARAPI GetQuotaForOrigin(
        /* [in] */ CString origin,
        /* [in] */ IValueCallback * pCallBack);

    CARAPI SetQuotaForOrigin(
        /* [in] */ CString origin,
        /* [in] */ Int64 quota);

    CARAPI DeleteOrigin(
        /* [in] */ CString origin);

    CARAPI DeleteAllData();

    CARAPI SetAppCacheMaximumSize(
        /* [in] */ Int64 size);

    CARAPI GetInstance(
        /* [out] */ IWebStorage ** ppInstance);

    CARAPI Update();

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBSTORAGE_H__
