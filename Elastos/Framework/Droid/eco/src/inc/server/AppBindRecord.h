
#ifndef __APPBINDRECORD_H__
#define __APPBINDRECORD_H__

#include <elastos.h>
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>
#include "server/CServiceRecord.h"
#include "server/IntentBindRecord.h"
#include "server/ProcessRecord.h"
#include "server/ConnectionRecord.h"

using namespace Elastos;

class IntentBindRecord;
class ConnectionRecord;

/**
 * An association between a service and one of its client applications.
 */
class AppBindRecord
{
public:
    AppBindRecord(
        /* [in] */ CServiceRecord* service,
        /* [in] */ IntentBindRecord* intent,
        /* [in] */ ProcessRecord* client);

    CARAPI GetDescription(
        /* [out] */ String* description);

public:
    AutoPtr<CServiceRecord> mService;   // The running service.
    IntentBindRecord* mIntent;  // The intent we are bound to.
    ProcessRecord* mClient;     // Who has started/bound the service.

    Set<ConnectionRecord*> mConnections;// All ConnectionRecord for this client.
};

#endif //__APPBINDRECORD_H__
