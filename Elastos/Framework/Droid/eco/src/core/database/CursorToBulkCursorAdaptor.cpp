#include "database/CursorToBulkCursorAdaptor.h"
#include <Logger.h>
#include "utils/Config.h"

using namespace Elastos::Utility::Logging;

const String CursorToBulkCursorAdaptor::TAG = String("Cursor");

ECode CursorToBulkCursorAdaptor::Init(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentObserver* observer,
        /* [in] */ String providerName,
        /* [in] */ Boolean allowWrite,
        /* [in] */ ICursorWindow* window)
{
    //try {
        //mCursor = (ICrossProcessCursor*)cursor;

        if ((IAbstractWindowedCursor*)mCursor->Probe(EIID_IAbstractWindowedCursor)) {
            AutoPtr<IAbstractWindowedCursor> windowedCursor = (IAbstractWindowedCursor*) cursor;
            Boolean rst;
            windowedCursor->HasWindow(&rst);
            if (rst) {
                if (Logger::IsLoggable(TAG, Logger::VERBOSE) || Config::LOGV) {
                    //Logger::V(TAG, "Cross process cursor has a local window before setWindow in "
                    //        + providerName, new RuntimeException());
                    return E_RUNTIME_EXCEPTION;
                }
            }
            windowedCursor->SetWindow(window);
        } else {
            mWindow = window;
            ((ICrossProcessCursor*)(ICursor*)mCursor)->FillWindow(0, window);
        }
    //} catch (ClassCastException e) {
        // TODO Implement this case.
    //    throw new UnsupportedOperationException(
    //            "Only CrossProcessCursor cursors are supported across process for now", e);
    //}
    mProviderName = providerName;
    mReadOnly = !allowWrite;

    CreateAndRegisterObserverProxy(observer);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::BinderDied()
{
    mCursor->Close();
    if (mWindow != NULL) {
        mWindow->Close();
    }
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::GetWindow(
        /* [in] */ Int32 startPos,
        /* [out] */ ICursorWindow** window)
{
    Boolean value;
    mCursor->MoveToPosition(startPos, &value);

    if (mWindow != NULL) {
        Int32 startPosition, num;
        mWindow->GetStartPosition(&startPosition);
        mWindow->GetNumRows(&num);
        if (startPos < startPosition ||
                startPos >= (startPosition + num)) {
            mCrossProcessCursor->FillWindow(startPos, mWindow);
        }
        window = (ICursorWindow**)&mWindow;
        return NOERROR;
    } else {
        mCrossProcessCursor->GetWindow(window);
        return NOERROR;
    }
}

ECode CursorToBulkCursorAdaptor::OnMove(
        /* [in] */ Int32 position)
{
    Int32 pos;
    mCursor->GetPosition(&pos);
    Boolean succeeded;
    mCrossProcessCursor->OnMove(pos, position, &succeeded);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::Count(
        /* [out] */ Int32* value)
{
    mCursor->GetCount(value);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::GetColumnNames(
        /* [out] */ ArrayOf<String>** columnNames)
{
    mCursor->GetColumnNames(columnNames);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::Deactivate()
{
    MaybeUnregisterObserverProxy();
    mCursor->Deactivate();
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::Close()
{
    MaybeUnregisterObserverProxy();
    mCursor->Close();
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::Requery(
        /* [in] */ IContentObserver* observer,
        /* [in] */ ICursorWindow* window,
        /* [out] */ Int32* value)
{
    assert(value != NULL);
    if (mWindow == NULL) {
        ((IAbstractWindowedCursor*)(ICursor*)mCursor)->SetWindow(window);
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
        mCrossProcessCursor->FillWindow(0, window);
        mWindow = window;
    }
    MaybeUnregisterObserverProxy();
    CreateAndRegisterObserverProxy(observer);
    mCursor->GetCount(value);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result)
{
    mCursor->GetWantsAllOnMoveCalls(result);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::DeleteRow(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if (mReadOnly) {
        //Logger::W("ContentProvider", "Permission Denial: modifying "
        //        + mProviderName
        //        + " from pid=" + Binder.getCallingPid()
        //        + ", uid=" + Binder.getCallingUid());
        *result = FALSE;
        return NOERROR;
    }
    Boolean value;
    mCursor->MoveToPosition(position, &value);
    if (value == FALSE) {
        *result = FALSE;
        return NOERROR;
    }
    mCursor->DeleteRow(result);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::GetExtras(
        /* [out] */ IBundle** extras)
{
    mCursor->GetExtras(extras);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** respond)
{
    mCursor->Respond(extras, respond);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::CreateAndRegisterObserverProxy(
        /* [in] */ IContentObserver* observer)
{
    if (mObserver != NULL) {
        //throw new IllegalStateException("an observer is already registered");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //mObserver = new ContentObserverProxy(observer, this);
    //mCursor->RegisterContentObserver(mObserver);
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::MaybeUnregisterObserverProxy()
{
    if (mObserver != NULL) {
        //mCursor->UnregisterContentObserver(mObserver);
        //mObserver.unlinkToDeath(this);
        mObserver = NULL;
    }
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::
            ContentObserverProxy::DeliverSelfNotifications(
                    /* [out] */ Boolean* result)
{
    // The far side handles the self notifications.
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode CursorToBulkCursorAdaptor::
            ContentObserverProxy::OnChange(
                    /* [in] */ Boolean selfChange)
{
//    try {
        mRemote->OnChange(selfChange);
//    } catch (RemoteException ex) {
        // Do nothing, the far side is dead
//    }
    return NOERROR;
}