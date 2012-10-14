
#include "app/CApplication.h"

ICONTEXT_METHODS_IMPL(CApplication, ContextWrapper, ContextWrapper);

ECode CApplication::constructor()
{
    return ContextWrapper::Init(NULL);
}

PInterface CApplication::Probe(
    /* [in] */ REIID riid)
{
    return _CApplication::Probe(riid);
}

ECode CApplication::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextWrapper::GetBaseContext(context);
}

ECode CApplication::Create()
{
    return OnCreate();
}

ECode CApplication::OnCreate()
{
    return NOERROR;
}
