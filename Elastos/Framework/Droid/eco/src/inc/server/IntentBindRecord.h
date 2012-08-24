
#ifndef __INTENTBINDRECORD_H__
#define __INTENTBINDRECORD_H__

#include <elastos.h>
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/Map.h>
#include "server/CServiceRecord.h"
#include "content/CIntent.h"
#include "server/ProcessRecord.h"
#include "server/AppBindRecord.h"

using namespace Elastos;

class AppBindRecord;

/**
 * A particular Intent that has been bound to a Service.
 */
class IntentBindRecord
{
public:
    IntentBindRecord(
        /* [in] */ CServiceRecord* service,
        /* [in] */ CIntent::FilterComparison* intent);

    CARAPI GetDescription(
        /* [out] */ String* description);

public:
     /** The running service. */
    AutoPtr<CServiceRecord> mService;
    /** The intent that is bound.*/
    CIntent::FilterComparison* mIntent; //
     /** All apps that have bound to this Intent. */
    Map<ProcessRecord*, AppBindRecord*> mApps;
    /** Binder published from service. */
    AutoPtr<IBinder> mBinder;
    /** Set when we have initiated a request for this binder. */
    Boolean mRequested;
    /** Set when we have received the requested binder. */
    Boolean mReceived;
    /** Set when we still need to tell the service all clients are unbound. */
    Boolean mHasBound;
    /** Set when the service's onUnbind() has asked to be told about new clients. */
    Boolean mDoRebind;

    String stringName;      // caching of toString
};

#endif //__INTENTBINDRECORD_H__
