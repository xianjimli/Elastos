
#include "content/CContextWrapper.h"

ICONTEXT_METHODS_IMPL(CContextWrapper, ContextWrapper, ContextWrapper);

ECode CContextWrapper::constructor(
    /* [in] */ IContext* context)
{
    return ContextWrapper::Init(context);
}

PInterface CContextWrapper::Probe(
    /* [in] */ REIID riid)
{
    return _CContextWrapper::Probe(riid);
}

ECode CContextWrapper::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextWrapper::GetBaseContext(context);
}
