
#ifndef __PENDINGTHUMBNAILSRECORD_H__
#define __PENDINGTHUMBNAILSRECORD_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "server/CActivityRecord.h"

/**
 * This class keeps track of calls to getTasks() that are still
 * waiting for thumbnail images.
 */
class PendingThumbnailsRecord
{
public:
    PendingThumbnailsRecord(
        /* [in] */ IThumbnailReceiver* receiver);

public:
    AutoPtr<IThumbnailReceiver> mReceiver;   // who is waiting.
    Set<AutoPtr<CActivityRecord> > mPendingRecords; // HistoryRecord objects we still wait for.
    Boolean mFinished;       // Is pendingRecords empty?
};

#endif //__PENDINGTHUMBNAILSRECORD_H__
