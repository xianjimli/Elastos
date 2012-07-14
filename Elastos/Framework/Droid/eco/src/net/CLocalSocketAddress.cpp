
#include "net/CLocalSocketAddress.h"
#include "ext/frameworkdef.h"


ECode CLocalSocketAddress::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
}

ECode CLocalSocketAddress::GetNamespace(
    /* [out] */ LocalSocketAddressNamespace* ns)
{
    VALIDATE_NOT_NULL(ns);

    *ns = mNamespace;
    return NOERROR;
}

ECode CLocalSocketAddress::constructor(
    /* [in] */ const String& name,
    /* [in] */ LocalSocketAddressNamespace ns)
{
    mName = name;
    mNamespace = ns;
    return NOERROR;
}

ECode CLocalSocketAddress::constructor(
    /* [in] */ const String& name)
{
    return constructor(name, LocalSocketAddressNamespace_ABSTRACT);
}
