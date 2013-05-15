
#ifndef __CHTTPAUTHHANDLERHELPER_H__
#define __CHTTPAUTHHANDLERHELPER_H__

#include "_CHttpAuthHandlerHelper.h"

CarClass(CHttpAuthHandlerHelper)
{
public:
    CARAPI OnReceivedCredentials(
        /* [in] */ Handle32 loader,
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

private:
    // TODO: Add your private member variables here.
};

#endif // __CHTTPAUTHHANDLERHELPER_H__
