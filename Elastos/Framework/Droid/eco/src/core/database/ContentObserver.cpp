
#include "database/ContentObserver.h"

ContentObserver::ContentObserver()
{
}

ContentObserver::~ContentObserver()
{
}

ECode ContentObserver::GetContentObserver(
    /* [out] */IContentObserver** observer)
{
    Mutex::Autolock lock(mTransportLock);
//    if (mTransport == null) {
//        mTransport = new Transport(this);
//    }
//    return mTransport;
    return E_NOT_IMPLEMENTED;
}

ECode ContentObserver::ReleaseContentObserver(
    /* [out] */IContentObserver** oldObserver)
{
/*    synchronized(lock) {
        Transport oldTransport = mTransport;
        if (oldTransport != null) {
            oldTransport.releaseContentObserver();
            mTransport = null;
        }
        return oldTransport;
    }
*/
    return E_NOT_IMPLEMENTED;
}

ECode ContentObserver::DeliverSelfNotifications(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
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
/*    if (mHandler == null) {
        onChange(selfChange);
    } else {
        mHandler.post(new NotificationRunnable(selfChange));
    } */
    return E_NOT_IMPLEMENTED;
}