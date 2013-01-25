
#ifndef __CWEBVIEWDATABASE_H__
#define __CWEBVIEWDATABASE_H__

#include "_CWebViewDatabase.h"

CarClass(CWebViewDatabase)
{
public:
    CARAPI GetInstance(
        /* [out] */ IWebViewDatabase ** ppInstance,
        /* [in] */ IContext * pContext);

    CARAPI HasUsernamePassword(
        /* [out] */ Boolean * pFlag);

    CARAPI ClearUsernamePassword();

    CARAPI HasHttpAuthUsernamePassword(
        /* [out] */ Boolean * pFlag);

    CARAPI ClearHttpAuthUsernamePassword();

    CARAPI HasFormData(
        /* [out] */ Boolean * pFlag);

    CARAPI ClearFormData();

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBVIEWDATABASE_H__
