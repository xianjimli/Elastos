
#include "CApp.h"

ECode CApp::GetPackageName(
    /* [out] */ String* pPackageName)
{
    if (!pPackageName) return E_INVALID_ARGUMENT;
    *pPackageName = "org.mozilla.fennec_unofficial";
    return NOERROR;
}

ECode CApp::GetContentProcessName(
    /* [out] */ String* pProcessName)
{
    if (!pProcessName) return E_INVALID_ARGUMENT;
    *pProcessName = "plugin-container";
    return NOERROR;
}
