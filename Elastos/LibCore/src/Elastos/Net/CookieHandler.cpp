
#include "cmdef.h"
#include "CookieHandler.h"
#include "CNetPermission.h"


AutoPtr<INetPermission> InitNetPermission(
    /* [in] */ const String& name)
{
    AutoPtr<INetPermission> permission;
    ASSERT_SUCCEEDED(CNetPermission::New(name, (INetPermission**)&permission));
    return permission;
}

AutoPtr<ICookieHandler> CookieHandler::sSystemWideCookieHandler;

AutoPtr<INetPermission> CookieHandler::sGetCookieHandlerPermission;

AutoPtr<INetPermission> CookieHandler::sSetCookieHandlerPermission;

ECode CookieHandler::GetDefault(
    /* [out] */ ICookieHandler** handler)
{
    VALIDATE_NOT_NULL(handler);

    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(getCookieHandlerPermission);
    // }
    *handler = sSystemWideCookieHandler;
    if (*handler != NULL) (*handler)->AddRef();

    return NOERROR;
}

/**
 * Sets the system-wide cookie handler.
 *
 * @param cHandler
 *            a cookie handler to set as the system-wide default handler.
 */
ECode CookieHandler::SetDefault(
    /* [in] */ ICookieHandler* cHandler)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(setCookieHandlerPermission);
    // }
    sSystemWideCookieHandler = cHandler;
    return NOERROR;
}
