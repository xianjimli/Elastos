#ifndef __CURSORTOBULKCURSORADAPTOR_H__
#define __CURSORTOBULKCURSORADAPTOR_H__

#include "ext/frameworkext.h"
#include "database/BulkCursorNative.h"
#include "database/ContentObserver.h"
#include <elastos/AutoPtr.h>
/**
 * Wraps a BulkCursor around an existing Cursor making it remotable.
 *
 * {@hide}
 */
class CursorToBulkCursorAdaptor : public BulkCursorNative
{
public:
    CARAPI Init(
        /* [in] */ ICursor* cursor, 
        /* [in] */ IContentObserver* observer,
        /* [in] */ String providerName,
        /* [in] */ Boolean allowWrite,
        /* [in] */ ICursorWindow* window);
    
    CARAPI BinderDied();
    
    CARAPI GetWindow(
        /* [in] */ Int32 startPos,
        /* [out] */ ICursorWindow** window);

    CARAPI OnMove(
        /* [in] */ Int32 position);

    CARAPI Count(
        /* [out] */ Int32* value);

    CARAPI GetColumnNames(
        /* [out] */ ArrayOf<String>** columnNames);

    CARAPI Deactivate();

    CARAPI Close();

    CARAPI Requery(
        /* [in] */ IContentObserver* observer,
        /* [in] */ ICursorWindow* window,
        /* [out] */ Int32* value);

    CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result);

    //public boolean updateRows(Map<? extends Long, ? extends Map<String, Object>> values)

    CARAPI DeleteRow(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* result);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** respond);

private:
    /**
     * Create a ContentObserver from the observer and register it as an observer on the
     * underlying cursor.
     * @param observer the IContentObserver that wants to monitor the cursor
     * @throws IllegalStateException if an observer is already registered
     */
    CARAPI CreateAndRegisterObserverProxy(
        /* [in] */ IContentObserver* observer);

    /** Unregister the observer if it is already registered. */
    CARAPI MaybeUnregisterObserverProxy();

private:
    class ContentObserverProxy : public ContentObserver 
    {
    protected:
        AutoPtr<IContentObserver> mRemote;

    public:
        //public ContentObserverProxy(IContentObserver remoteObserver, DeathRecipient recipient)
        
        //public boolean unlinkToDeath(DeathRecipient recipient)

        //@Override
        CARAPI DeliverSelfNotifications(
            /* [out] */ Boolean* result);

        //@Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);
    };

private:
    static const String TAG;
    const AutoPtr<ICrossProcessCursor> mCrossProcessCursor;
    AutoPtr<ICursor> mCursor;
    AutoPtr<ICursorWindow> mWindow;
    String mProviderName;
    Boolean mReadOnly;
    ContentObserverProxy * mObserver;

};
#endif //__CURSORTOBULKCURSORADAPTOR_H__