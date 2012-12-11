#ifndef __CONTENTOBSERVER_H__
#define __CONTENTOBSERVER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
/**
 * Receives call backs for changes to content. Must be implemented by objects which are added
 * to a {@link ContentObservable}.
 */
using namespace Elastos::Core::Threading;
class ContentObserver
{
/*    private final class NotificationRunnable implements Runnable {

        private boolean mSelf;

        public NotificationRunnable(boolean self) {
            mSelf = self;
        }

        public void run() {
            ContentObserver.this.onChange(mSelf);
        }
    }

    private static final class Transport extends IContentObserver.Stub {
        ContentObserver mContentObserver;

        public Transport(ContentObserver contentObserver) {
            mContentObserver = contentObserver;
        }

        public boolean deliverSelfNotifications() {
            ContentObserver contentObserver = mContentObserver;
            if (contentObserver != null) {
                return contentObserver.deliverSelfNotifications();
            }
            return false;
        }

        public void onChange(boolean selfChange) {
            ContentObserver contentObserver = mContentObserver;
            if (contentObserver != null) {
                contentObserver.dispatchChange(selfChange);
            }
        }

        public void releaseContentObserver() {
            mContentObserver = null;
        }
    }*/

public:
    ContentObserver();

    /**
     * onChange() will happen on the provider Handler.
     *
     * @param handler The handler to run {@link #onChange} on.
     */
/*    public ContentObserver(Handler handler) {
        mHandler = handler;
    }*/

    ~ContentObserver();

    virtual CARAPI GetContentObserver(
        /* [out] */ IContentObserver** observer);

    virtual CARAPI ReleaseContentObserver(
        /* [out] */ IContentObserver** oldObserver);

    virtual CARAPI DeliverSelfNotifications(
        /* [out] */ Boolean* rst);

    virtual CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);
private:
//    private Transport mTransport;

    // Protects mTransport
//    private Object lock = new Object();
    Mutex mTransportLock;

//    /* package */ Handler mHandler;





};
#endif //__CONTENTOBSERVER_H__