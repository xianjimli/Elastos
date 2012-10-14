
#include "server/PendingThumbnailsRecord.h"

PendingThumbnailsRecord::PendingThumbnailsRecord(
    /* [in] */ IThumbnailReceiver* receiver) :
    mReceiver(receiver),
    mFinished(FALSE)
{
}
