
#ifndef __CONNECTIONRECORD_H__
#define __CONNECTIONRECORD_H__

#include <elastos.h>
#include <elastos/AutoPtr.h>
#include "server/AppBindRecord.h"
#include "server/CActivityRecord.h"

using namespace Elastos;

class CActivityRecord;
class AppBindRecord;

/**
 * Description of a single binding to a service.
 */
class ConnectionRecord
{
public:
    ConnectionRecord(
        /* [in] */ AppBindRecord* binding,
        /* [in] */ CActivityRecord* activity,
        /* [in] */ IServiceConnectionInner* conn,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 clientLabel,
        /* [in] */ IPendingIntent* clientIntent);

    CARAPI GetDescription(
        /* [out] */ String* description);

public:
    AppBindRecord* mBinding;    // The application/service binding.
    AutoPtr<CActivityRecord> mActivity;  // If non-null, the owning activity.
    AutoPtr<IServiceConnectionInner> mConn;  // The client connection.
    Int32 mFlags;   // Binding options.
    Int32 mClientLabel;          // String resource labeling this client.
    AutoPtr<IPendingIntent> mClientIntent; // How to launch the client.
    String mStringName;     // Caching of toString.
};

#endif //__CONNECTIONRECORD_H__
