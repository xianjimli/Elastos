
#include "content/CContextThemeWrapper.h"

ICONTEXT_METHODS_IMPL(CContextThemeWrapper, ContextThemeWrapper, ContextThemeWrapper);

ECode CContextThemeWrapper::constructor()
{
    return NOERROR;
}

ECode CContextThemeWrapper::constructor(
    /* [in] */ IContext* base,
    /* [in] */ Int32 themeres)
{
    return ContextThemeWrapper::Init(base, themeres);
}

PInterface CContextThemeWrapper::Probe(
    /* [in] */ REIID riid)
{
    return _CContextThemeWrapper::Probe(riid);
}

ECode CContextThemeWrapper::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextThemeWrapper::GetBaseContext(context);
}
