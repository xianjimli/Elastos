#include "database/BulkCursorNative.h"

BulkCursorNative::BulkCursorNative()
{

}

ECode BulkCursorNative::AsInterface(
        /* [in] */ IBinder* obj,
        /* [out] */ IBulkCursor** bc)
{
    if (obj == NULL) {
        *bc = NULL;
        return NOERROR;
    }
    AutoPtr<IBulkCursor> in;
//    const String descriptor = String(IBulkCursor_descriptor);
//    obj->QueryLocalInterface(descriptor, (IInterface2**)&in);
    if (in != NULL) {
        *bc = in;
        return NOERROR;
    }
    BulkCursorProxy* bcp = new BulkCursorProxy(obj);
    *bc = (IBulkCursor*)bcp;
    return NOERROR;
}

ECode BulkCursorNative::OnTransact(
        /* [in] */ Int32 code,
        /* [in] */ IParcel* data,
        /* [in] */ IParcel* reply,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* rst)
{
/*
    assert(rst != NULL);
    const String descriptor = String(IBulkCursor_descriptor);
    switch(code) {
        case IBulkCursor_GET_CURSOR_WINDOW_TRANSACTION:
            data->EnforceInterface(descriptor);
            Int32 startPos;
            data->ReadInt(&startPos);
            AutoPtr<ICursorWindow> window;
            GetWindow(startPos,(ICursorWindow**)&window);
            if(window == NULL) {
                reply->WriteInt(0);
            }
            window->WriteToParcel(reply, 0);
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_COUNT_TRANSACTION:
            data->EnforceInterface(descriptor);
            Int32 count;
            Count(&count);
            reply->WriteNoException();
            reply->WriteInt(count);
            *rst = TRUE;
            return NOERROR

        case IBulkCursor_GET_COLUMN_NAMES_TRANSACTION:
            data->EnforceInterface(descriptor);
            ArrayOf<String>* columnNames;
            GetColumnNames(&columnNames);
            reply->WriteNoException();
            reply->WriteInt(columnNames->GetLength());
            Int32 length = columnNames->GetLength();
            for (Int32 i = 0; i < length; i++) {
                reply->WriteString((*columnNames)[i]);
            }
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_DEACTIVATE_TRANSACTION:
            data->EnforceInterface(descriptor);
            Deactivate();
            reply->WriteNoException();
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_CLOSE_TRANSACTION:
            data->EnforceInterface(descriptor);
            Close();
            reply->WriteNoException();
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_REQUERY_TRANSACTION:
            data->EnforceInterface(descriptor);
//            IContentObserver observer =
//                IContentObserver.Stub.asInterface(data.readStrongBinder());
//            CursorWindow window = CursorWindow.CREATOR.createFromParcel(data);
//            int count = requery(observer, window);
//            reply.writeNoException();
//            reply.writeInt(count);
//            reply.writeBundle(getExtras());
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_UPDATE_ROWS_TRANSACTION:
            data->EnforceInterface(descriptor);
            HashMap<Int64, Map<String,AutoPtr<IInterface> > > *values;
            data->ReadHashMap(NULL, &values);
            Boolean result;
            UpdateRows(values, &result);
            reply->WriteNoException();
            reply->WriteInt(result == TRUE ? 1 : 0);
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_DELETE_ROW_TRANSACTION:
            data->EnforceInterface(descriptor);
            Int32 position;
            data->ReadInt(&position);
            Boolean result;
            DeleteRow(position, &result);
            reply->WriteNoException();
            reply->WriteInt(result == TRUE ? 1 : 0);
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_ON_MOVE_TRANSACTION:
            data->EnforceInterface(descriptor);
            Int32 position;
            data->ReadInt(&position);
            OnMove(position);
            reply->WriteNoException();
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_WANTS_ON_MOVE_TRANSACTION:
            data->EnforceInterface(descriptor);
            Boolean result;
            GetWantsAllOnMoveCalls(&result);
            reply->WriteNoException();
            reply->WriteInt(result == TRUE ? 1 : 0);
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_GET_EXTRAS_TRANSACTION:
            data->EnforceInterface(descriptor);
            AutoPtr<IBundle> extras;
            GetExtras((IBundle**)&extras);
            reply->WriteNoException();
            reply->WriteBundle(extras);
            *rst = TRUE;
            return NOERROR;

        case IBulkCursor_RESPOND_TRANSACTION:
            data->EnforceInterface(descriptor);
            AutoPtr<IBundle> extras;
            data->ReadBundle((IBundle**)&extras);
            AutoPtr<IBundle> returnExtras;
            Respond(extras, (IBundle**)&returnExtras);
            reply->WriteNoException();
            reply->WriteBundle(returnExtras);
            *rst = TRUE;
            return NOERROR;
    }
//    return super.onTransact(code, data, reply, flags);
*/
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorNative::AsBinder(
        /* [out] */ IBinder** b)
{
    *b = (IBinder*)this;
    return NOERROR;
}

BulkCursorProxy::BulkCursorProxy()
{
}

BulkCursorProxy::BulkCursorProxy(
        /* [in] */ IBinder* remote)
{
    mRemote = remote;
    mExtras = NULL;
}

ECode BulkCursorProxy::AsBinder(
        /* [out] */ IBinder** b)
{
    *b = (IBinder*)mRemote;
    return NOERROR;
}

ECode BulkCursorProxy::GetWindow(
        /* [in] */ Int32 startPos,
        /* [out] */ ICursorWindow** cw)
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::OnMove(
        /* [in] */ Int32 position)
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::Count(
        /* [out] */ Int32* position)
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::GetColumnNames(
        /* [out] */ ArrayOf<String>** names)
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::Deactivate()
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::Requery(
        /* [in] */ ILocalContentObserver* observer,
        /* [in] */ ICursorWindow* window,
        /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

