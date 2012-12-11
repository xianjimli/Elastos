#include "database/BulkCursorNative.h"

const String BulkCursorNative::IBulkCursor_descriptor = String("android.constent.IBulkCursor");

PInterface BulkCursorNative::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IObservable) {
        return (IBulkCursorNative*)this;
    }

    return NULL;
}

UInt32 BulkCursorNative::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BulkCursorNative::Release()
{
    return ElRefBase::Release();
}

ECode BulkCursorNative::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IBulkCursorNative*)this) {
        *pIID = EIID_IBulkCursorNative;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

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
    const String descriptor = String("android.content.IBulkCursor");
    //obj->QueryLocalInterface(descriptor, (IInterface2**)&in);
    if (in != NULL) {
        *bc = in;
        return NOERROR;
    }
    AutoPtr<IBulkCursorProxy> bcp;
    CBulkCursorProxy::New(obj, (IBulkCursorProxy**)&bcp);
    *bc = (IBulkCursor*)((IBulkCursorProxy*)bcp);
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
    const String descriptor = String("android.content.IBulkCursor");
    switch(code) {
        case 1:
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

        case 2:
            data->EnforceInterface(descriptor);
            Int32 count;
            Count(&count);
            reply->WriteNoException();
            reply->WriteInt(count);
            *rst = TRUE;
            return NOERROR

        case 3:
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

        case 6:
            data->EnforceInterface(descriptor);
            Deactivate();
            reply->WriteNoException();
            *rst = TRUE;
            return NOERROR;

        case 12:
            data->EnforceInterface(descriptor);
            Close();
            reply->WriteNoException();
            *rst = TRUE;
            return NOERROR;

        case 7:
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

        case 4:
            data->EnforceInterface(descriptor);
            HashMap<Int64, Map<String,AutoPtr<IInterface> > > *values;
            data->ReadHashMap(NULL, &values);
            Boolean result;
            UpdateRows(values, &result);
            reply->WriteNoException();
            reply->WriteInt(result == TRUE ? 1 : 0);
            *rst = TRUE;
            return NOERROR;

        case 5:
            data->EnforceInterface(descriptor);
            Int32 position;
            data->ReadInt(&position);
            Boolean result;
            DeleteRow(position, &result);
            reply->WriteNoException();
            reply->WriteInt(result == TRUE ? 1 : 0);
            *rst = TRUE;
            return NOERROR;

        case 8:
            data->EnforceInterface(descriptor);
            Int32 position;
            data->ReadInt(&position);
            OnMove(position);
            reply->WriteNoException();
            *rst = TRUE;
            return NOERROR;

        case 9:
            data->EnforceInterface(descriptor);
            Boolean result;
            GetWantsAllOnMoveCalls(&result);
            reply->WriteNoException();
            reply->WriteInt(result == TRUE ? 1 : 0);
            *rst = TRUE;
            return NOERROR;

        case 10:
            data->EnforceInterface(descriptor);
            AutoPtr<IBundle> extras;
            GetExtras((IBundle**)&extras);
            reply->WriteNoException();
            reply->WriteBundle(extras);
            *rst = TRUE;
            return NOERROR;

        case 11:
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



