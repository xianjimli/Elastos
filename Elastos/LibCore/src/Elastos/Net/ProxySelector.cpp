
#include "cmdef.h"
#include "ProxySelector.h"
#include "CNetPermission.h"
#include "ProxySelectorImpl.h"
#include <Com.Kortide.Platform.h>

AutoPtr<IProxySelector> InitDefaultSelector()
{
    ProxySelectorImpl* impl = new ProxySelectorImpl();
    AutoPtr<IProxySelector> selector = (IProxySelector*)impl->Probe(EIID_IProxySelector);
    selector->AddRef();

    return selector;
}

AutoPtr<INetPermission> InitProxySelectorPermission(
    /* [in] */ const String& name)
{
    AutoPtr<INetPermission> netPermission;
    assert(CNetPermission::New(name, (INetPermission**)&netPermission));
    netPermission->AddRef();

    return netPermission;
}

AutoPtr<IProxySelector> ProxySelector::mDefaultSelector = InitDefaultSelector();

AutoPtr<INetPermission> ProxySelector::mGetProxySelectorPermission =
        InitProxySelectorPermission(
                String("getProxySelector"));

AutoPtr<INetPermission> ProxySelector::mSetProxySelectorPermission =
        InitProxySelectorPermission(
                String("setProxySelector"));

ProxySelector::ProxySelector()
{}

ECode ProxySelector::GetDefault(
    /* [in] */ IProxySelector** defaultSelector)
{
    VALIDATE_NOT_NULL(defaultSelector);
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(getProxySelectorPermission);
    // }
    *defaultSelector = mDefaultSelector;
    return NOERROR;
}

ECode ProxySelector::SetDefault(
    /* [in] */ IProxySelector* selector)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(setProxySelectorPermission);
    // }
    mDefaultSelector = selector;
    return NOERROR;
}
