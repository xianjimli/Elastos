
#include "server/IntentBindRecord.h"

IntentBindRecord::IntentBindRecord(
    /* [in] */ CServiceRecord* service,
    /* [in] */ IIntentFilterComparison* intent) :
    mService(service),
    mIntent(intent),
    mRequested(FALSE),
    mReceived(FALSE),
    mHasBound(FALSE),
    mDoRebind(FALSE)
{
}

ECode IntentBindRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
