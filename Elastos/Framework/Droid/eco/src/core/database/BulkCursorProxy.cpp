#include "database/BulkCursorProxy.h"
const String BulkCursorProxy::IBulkCursor_descriptor = String("android.constent.IBulkCursor");
BulkCursorProxy::BulkCursorProxy()
{
}

BulkCursorProxy::~BulkCursorProxy()
{
}

ECode BulkCursorProxy::Init(
        /* [in] */ IBinder* remote)
{
    mRemote = remote;
    mExtras = NULL;
    return NOERROR;
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
        /* [in] */ IContentObserver* observer,
        /* [in] */ ICursorWindow* window,
        /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::DeleteRow(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::GetExtras(
    /* [out] */ IBundle** extras)
{
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorProxy::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** returnExtras)
{
    return E_NOT_IMPLEMENTED;
}