
#include "cmdef.h"
#include "CookieHandler.h"
#include "CNetPermission.h"

AutoPtr<INetPermission> InitNetPermission(
    /* [in] */ const String& name)
{
    AutoPtr<INetPermission> permission;
    ASSERT_SUCCEEDED(CNetPermission::New(name, (INetPermission**)&permission));
    permission->AddRef();

    return permission;
}

AutoPtr<ICookieHandler> CookieHandler::mSystemWideCookieHandler;

AutoPtr<INetPermission> CookieHandler::mGetCookieHandlerPermission =
        InitNetPermission(String("getCookieHandler"));

AutoPtr<INetPermission> CookieHandler::mSetCookieHandlerPermission =
        InitNetPermission(String("setCookieHandler"));

ECode CookieHandler::GetDefault(
    /* [out] */ ICookieHandler** handler)
{
    VALIDATE_NOT_NULL(handler);

    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(getCookieHandlerPermission);
    // }
    *handler = mSystemWideCookieHandler;

    return NOERROR;
}

/**
 * Sets the system-wide cookie handler.
 *
 * @param cHandler
 *            a cookie handler to set as the system-wide default handler.
 */
void CookieHandler::setDefault(
    /* [in] */ ICookieHandler* cHandler)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(setCookieHandlerPermission);
    // }
    mSystemWideCookieHandler = cHandler;
}
