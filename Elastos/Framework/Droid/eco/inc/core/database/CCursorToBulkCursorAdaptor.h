#ifndef __CURSORTOBULKCURSORADAPTOR_H__
#define __CURSORTOBULKCURSORADAPTOR_H__

#include "ext/frameworkext.h"
#include "_CCursorToBulkCursorAdaptor.h"
#include "database/ContentObserver.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * Wraps a BulkCursor around an existing Cursor making it remotable.
 *
 * {@hide}
 */
CarClass(CCursorToBulkCursorAdaptor)
{
private:
    class ContentObserverProxy
        : public ElRefBase
        , public ContentObserver
        , public ILocalContentObserver
    {
    public:
        ContentObserverProxy(
            /* [in] */ IContentObserver* remoteObserver /*,  [in] DeathRecipient recipient*/);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        //public boolean unlinkToDeath(DeathRecipient recipient)

        CARAPI GetContentObserver(
            /* [out] */ IContentObserver** observer);

        CARAPI ReleaseContentObserver(
            /* [out] */ IContentObserver** oldObserver);

        //@Override
        CARAPI DeliverSelfNotifications(
            /* [out] */ Boolean* result);

        //@Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

        CARAPI DispatchChange(
            /* [in] */ Boolean selfChange);

    protected:
        AutoPtr<IContentObserver> mRemote;
    };

public:
    CARAPI constructor(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentObserver* observer,
        /* [in] */ const String& providerName,
        /* [in] */ Boolean allowWrite,
        /* [in] */ ICursorWindow* window);

    CARAPI BinderDied();

    CARAPI GetWindow(
        /* [in] */ Int32 startPos,
        /* [out] */ ICursorWindow** window);

    CARAPI OnMove(
        /* [in] */ Int32 position);

    CARAPI GetCount(
        /* [out] */ Int32* value);

    CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String>** columnNames);

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
    static const CString TAG;
    AutoPtr<ICrossProcessCursor> mCursor;
    AutoPtr<ICursorWindow> mWindow;
    String mProviderName;
    Boolean mReadOnly;
    AutoPtr<ContentObserverProxy> mObserver;
};

#endif //__CURSORTOBULKCURSORADAPTOR_H__
