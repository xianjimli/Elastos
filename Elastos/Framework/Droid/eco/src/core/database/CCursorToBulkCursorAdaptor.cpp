
#include "database/CCursorToBulkCursorAdaptor.h"
#include "utils/Config.h"
#include <Logger.h>


using namespace Elastos::Utility::Logging;


CCursorToBulkCursorAdaptor::ContentObserverProxy::ContentObserverProxy(
    /* [in] */ IContentObserver* remoteObserver /*,  [in] DeathRecipient recipient*/)
    // : ContentObserver(NULL)
{
    mRemote = remoteObserver;
    // try {
    //     remoteObserver.asBinder().linkToDeath(recipient, 0);
    // } catch (RemoteException e) {
    //     // Do nothing, the far side is dead
    // }
}

PInterface CCursorToBulkCursorAdaptor::ContentObserverProxy::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ILocalContentObserver*)this;
    }
    else if (riid == EIID_ILocalContentObserver) {
        return (ILocalContentObserver*)this;
    }

    return NULL;
}

UInt32 CCursorToBulkCursorAdaptor::ContentObserverProxy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCursorToBulkCursorAdaptor::ContentObserverProxy::Release()
{
    return ElRefBase::Release();
}

ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ILocalContentObserver*)this) {
        *pIID = EIID_ILocalContentObserver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

//public boolean unlinkToDeath(DeathRecipient recipient)

ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::GetContentObserver(
    /* [out] */ IContentObserver** observer)
{
    VALIDATE_NOT_NULL(observer);

    AutoPtr<IContentObserver> _observer = ContentObserver::GetContentObserver();
    *observer = _observer.Get();
    if (*observer != NULL) (*observer)->AddRef();
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::ReleaseContentObserver(
    /* [out] */ IContentObserver** oldObserver)
{
    VALIDATE_NOT_NULL(oldObserver);

    AutoPtr<IContentObserver> _observer = ContentObserver::ReleaseContentObserver();
    *oldObserver = _observer.Get();
    if (*oldObserver != NULL) (*oldObserver)->AddRef();
    return NOERROR;
}

//@Override
ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::DeliverSelfNotifications(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // The far side handles the self notifications.
    *result = FALSE;
    return NOERROR;
}

//@Override
ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::OnChange(
    /* [in] */ Boolean selfChange)
{
    // try {
    return mRemote->OnChange(selfChange);
    // } catch (RemoteException ex) {
    //     // Do nothing, the far side is dead
    // }
}

ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    return ContentObserver::DispatchChange(selfChange);
}


const CString CCursorToBulkCursorAdaptor::TAG = "Cursor";

ECode CCursorToBulkCursorAdaptor::constructor(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentObserver* observer,
        /* [in] */ const String& providerName,
        /* [in] */ Boolean allowWrite,
        /* [in] */ ICursorWindow* window)
{
    //try {
    mCursor = ICrossProcessCursor::Probe(cursor);
    assert(mCursor != NULL);

    if (IAbstractWindowedCursor::Probe(mCursor) != NULL) {
        IAbstractWindowedCursor* windowedCursor = IAbstractWindowedCursor::Probe(mCursor);
        Boolean hasWindow;
        windowedCursor->HasWindow(&hasWindow);
        if (hasWindow) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE) || Config::LOGV) {
                //Logger::V(TAG, "Cross process cursor has a local window before setWindow in "
                //        + providerName, new RuntimeException());
            }
        }
        windowedCursor->SetWindow(window);
    }
    else {
        mWindow = window;
        mCursor->FillWindow(0, window);
    }
    //} catch (ClassCastException e) {
        // TODO Implement this case.
    //    throw new UnsupportedOperationException(
    //            "Only CrossProcessCursor cursors are supported across process for now", e);
    //}
    mProviderName = providerName;
    mReadOnly = !allowWrite;

    return CreateAndRegisterObserverProxy(observer);
}

ECode CCursorToBulkCursorAdaptor::BinderDied()
{
    mCursor->Close();
    if (mWindow != NULL) {
        mWindow->Close();
    }
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::GetWindow(
    /* [in] */ Int32 startPos,
    /* [out] */ ICursorWindow** window)
{
    VALIDATE_NOT_NULL(window);

    Boolean succeeded;
    mCursor->MoveToPosition(startPos, &succeeded);

    if (mWindow != NULL) {
        Int32 startPosition, num;
        mWindow->GetStartPosition(&startPosition);
        mWindow->GetNumRows(&num);
        if (startPos < startPosition ||
                startPos >= (startPosition + num)) {
            mCursor->FillWindow(startPos, mWindow);
        }
        *window = ICursorWindow::Probe(mWindow);
        assert(*window != NULL);
        (*window)->AddRef();
        return NOERROR;
    }
    else {
        return mCursor->GetWindow(window);
    }
}

ECode CCursorToBulkCursorAdaptor::OnMove(
    /* [in] */ Int32 position)
{
    Int32 pos;
    mCursor->GetPosition(&pos);
    Boolean succeeded;
    return mCursor->OnMove(pos, position, &succeeded);
}

ECode CCursorToBulkCursorAdaptor::GetCount(
    /* [out] */ Int32* value)
{
    return mCursor->GetCount(value);
}

ECode CCursorToBulkCursorAdaptor::GetColumnNames(
    /* [out, callee] */ ArrayOf<String>** columnNames)
{
    return mCursor->GetColumnNames(columnNames);
}

ECode CCursorToBulkCursorAdaptor::Deactivate()
{
    MaybeUnregisterObserverProxy();
    return mCursor->Deactivate();
}

ECode CCursorToBulkCursorAdaptor::Close()
{
    MaybeUnregisterObserverProxy();
    return mCursor->Close();
}

ECode CCursorToBulkCursorAdaptor::Requery(
    /* [in] */ IContentObserver* observer,
    /* [in] */ ICursorWindow* window,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    if (mWindow == NULL) {
        IAbstractWindowedCursor::Probe(mCursor)->SetWindow(window);
    }
    //try {
    Boolean succeeded;
    mCursor->Requery(&succeeded);
    if (!succeeded) {
        *value = -1;
        return NOERROR;
    }
    //} catch (IllegalStateException e) {
    //    IllegalStateException leakProgram = new IllegalStateException(
    //            mProviderName + " Requery misuse db, mCursor isClosed:" +
    //            mCursor.isClosed(), e);
    //    throw leakProgram;
    //}

    if (mWindow != NULL) {
        mCursor->FillWindow(0, window);
        mWindow = window;
    }
    MaybeUnregisterObserverProxy();
    CreateAndRegisterObserverProxy(observer);
    return mCursor->GetCount(value);
}

ECode CCursorToBulkCursorAdaptor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* result)
{
    return mCursor->GetWantsAllOnMoveCalls(result);
}

ECode CCursorToBulkCursorAdaptor::CreateAndRegisterObserverProxy(
    /* [in] */ IContentObserver* observer)
{
    if (mObserver != NULL) {
        //throw new IllegalStateException("an observer is already registered");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mObserver = new ContentObserverProxy(observer/*, this*/);
    mCursor->RegisterContentObserver(mObserver);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::MaybeUnregisterObserverProxy()
{
    if (mObserver != NULL) {
        mCursor->UnregisterContentObserver(mObserver);
        // mObserver.unlinkToDeath(this);
        mObserver = NULL;
    }
    return NOERROR;
}

// public boolean updateRows(Map<? extends Long, ? extends Map<String, Object>> values) {
//     if (mReadOnly) {
//         Log.w("ContentProvider", "Permission Denial: modifying "
//                 + mProviderName
//                 + " from pid=" + Binder.getCallingPid()
//                 + ", uid=" + Binder.getCallingUid());
//         return false;
//     }
//     return mCursor.commitUpdates(values);
// }

ECode CCursorToBulkCursorAdaptor::DeleteRow(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mReadOnly) {
        //Logger::W("ContentProvider", "Permission Denial: modifying "
        //        + mProviderName
        //        + " from pid=" + Binder.getCallingPid()
        //        + ", uid=" + Binder.getCallingUid());
        *result = FALSE;
        return NOERROR;
    }
    Boolean succeeded;
    mCursor->MoveToPosition(position, &succeeded);
    if (!succeeded) {
        *result = FALSE;
        return NOERROR;
    }
    return mCursor->DeleteRow(result);
}

ECode CCursorToBulkCursorAdaptor::GetExtras(
    /* [out] */ IBundle** extras)
{
    return mCursor->GetExtras(extras);
}

ECode CCursorToBulkCursorAdaptor::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** respond)
{
    return mCursor->Respond(extras, respond);
}
