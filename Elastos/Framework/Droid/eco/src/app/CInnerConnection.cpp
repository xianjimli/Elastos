
#include "app/CInnerConnection.h"

ECode CInnerConnection::Init(
    /* [in] */ LoadedCap::ServiceDispatcher* sd)
{
    mDispatcher = sd;

    return NOERROR;
}

ECode CInnerConnection::Connected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    LoadedCap::ServiceDispatcher* sd = mDispatcher;
    if (sd != NULL) {
        sd->Connected(name, service);
    }
    return NOERROR;
}

ECode CInnerConnection::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}