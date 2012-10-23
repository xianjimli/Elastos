
#include "database/ContentObserver.h"

ContentObserver::ContentObserver()
{}

ECode ContentObserver::DeliverSelfNotifications(
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    *rst = FALSE;
    return NOERROR;
}

ECode ContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    return NOERROR;
}

ECode ContentObserver::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    return OnChange(selfChange);
}
