
#include "server/AppBindRecord.h"

AppBindRecord::AppBindRecord(
    /* [in] */ CServiceRecord* service,
    /* [in] */ IntentBindRecord* intent,
    /* [in] */ ProcessRecord* client)
{
    mService = service;
    mIntent = intent;
    mClient = client;
}

ECode AppBindRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
