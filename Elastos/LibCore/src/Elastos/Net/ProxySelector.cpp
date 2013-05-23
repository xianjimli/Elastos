
#include "cmdef.h"
#include "ProxySelector.h"
#include "CNetPermission.h"
#include "ProxySelectorImpl.h"
#include <Com.Kortide.Platform.h>

AutoPtr<INetPermission> InitProxySelectorPermission(
    /* [in] */ const String& name)
{
    AutoPtr<INetPermission> netPermission;
    assert(CNetPermission::New(name, (INetPermission**)&netPermission));

    return netPermission;
}

AutoPtr<IProxySelector> ProxySelector::sDefaultSelector;

AutoPtr<INetPermission> ProxySelector::sGetProxySelectorPermission;

AutoPtr<INetPermission> ProxySelector::sSetProxySelectorPermission;

ECode ProxySelector::GetDefault(
    /* [in] */ IProxySelector** defaultSelector)
{
    VALIDATE_NOT_NULL(defaultSelector);
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(getProxySelectorPermission);
    // }
    if (sDefaultSelector == NULL) {
        sDefaultSelector = new ProxySelectorImpl();
    }

    *defaultSelector = sDefaultSelector;
    if (*defaultSelector != NULL) (*defaultSelector)->AddRef();
    return NOERROR;
}

ECode ProxySelector::SetDefault(
    /* [in] */ IProxySelector* selector)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(setProxySelectorPermission);
    // }
    sDefaultSelector = selector;
    return NOERROR;
}
