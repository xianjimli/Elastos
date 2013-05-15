
#include "webkit/CHttpAuthHandlerHelper.h"
#include "webkit/CHttpAuthHandler.h"
ECode CHttpAuthHandlerHelper::OnReceivedCredentials(
    /* [in] */ Handle32 loader,
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    CHttpAuthHandler::OnReceivedCredentials((LoadListener*)loader, host, realm, username, password);
    return NOERROR;
}

