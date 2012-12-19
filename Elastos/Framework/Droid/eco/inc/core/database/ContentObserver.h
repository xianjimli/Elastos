#ifndef __CONTENTOBSERVER_H__
#define __CONTENTOBSERVER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>

using namespace Elastos::Core::Threading;

/**
 * Receives call backs for changes to content. Must be implemented by objects which are added
 * to a {@link ContentObservable}.
 */
class ContentObserver
{
//    private final class NotificationRunnable implements Runnable {
//
//        private boolean mSelf;
//
//        public NotificationRunnable(boolean self) {
//            mSelf = self;
//        }
//
//        public void run() {
//            ContentObserver.this.onChange(mSelf);
//        }
//    }

private:
    class Transport
        : public ElRefBase
        , public IContentObserver
    {
    public:
        Transport(
            /* [in] */ ILocalContentObserver* contentObserver);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI DeliverSelfNotifications(
            /* [out] */ Boolean* result);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

        CARAPI_(void) ReleaseContentObserver();

    public:
        AutoPtr<ILocalContentObserver> mContentObserver;
    };

public:
    /**
     * onChange() will happen on the provider Handler.
     *
     * @param handler The handler to run {@link #onChange} on.
     */
/*    public ContentObserver(Handler handler) {
        mHandler = handler;
    }*/

    ~ContentObserver();

    virtual CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid) = 0;

    /**
     * Gets access to the binder transport object. Not for public consumption.
     *
     * {@hide}
     */
    virtual CARAPI_(AutoPtr<IContentObserver>) GetContentObserver();

    /**
     * Gets access to the binder transport object, and unlinks the transport object
     * from the ContentObserver. Not for public consumption.
     *
     * {@hide}
     */
    virtual CARAPI_(AutoPtr<IContentObserver>) ReleaseContentObserver();

    /**
     * Returns true if this observer is interested in notifications for changes
     * made through the cursor the observer is registered with.
     */
    virtual CARAPI_(Boolean) DeliverSelfNotifications();

    /**
     * This method is called when a change occurs to the cursor that
     * is being observed.
     *
     * @param selfChange true if the update was caused by a call to <code>commit</code> on the
     *  cursor that is being observed.
     */
    virtual CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);

private:
    AutoPtr<Transport> mTransport;

    // Protects mTransport
    Mutex mTransportLock;

//    /* package */ Handler mHandler;
};

#endif //__CONTENTOBSERVER_H__
