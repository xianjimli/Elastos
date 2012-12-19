
#include "ext/frameworkdef.h"
#include "database/CBulkCursorToCursorAdaptor.h"


ICURSOR_METHODS_IMPL(CBulkCursorToCursorAdaptor, BulkCursorToCursorAdaptor, BulkCursorToCursorAdaptor);

ECode CBulkCursorToCursorAdaptor::GetWindow(
    /* [out] */ ICursorWindow** window)
{
    VALIDATE_NOT_NULL(window);

    AutoPtr<ICursorWindow> _window = BulkCursorToCursorAdaptor::GetWindow();
    *window = _window.Get();
    if (*window != NULL) (*window)->AddRef();
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow* window)
{
    return BulkCursorToCursorAdaptor::FillWindow(pos, window);
}

ECode CBulkCursorToCursorAdaptor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    *succeeded = BulkCursorToCursorAdaptor::OnMove(oldPosition, newPosition);
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::IsBlob(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isBlob)
{
    VALIDATE_NOT_NULL(isBlob);

    return BulkCursorToCursorAdaptor::IsBlob(columnIndex, isBlob);
}

ECode CBulkCursorToCursorAdaptor::IsString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isString)
{
    VALIDATE_NOT_NULL(isString);

    return BulkCursorToCursorAdaptor::IsString(columnIndex, isString);
}

ECode CBulkCursorToCursorAdaptor::IsInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isInt64)
{
    VALIDATE_NOT_NULL(isInt64);

    return BulkCursorToCursorAdaptor::IsInt64(columnIndex, isInt64);
}

ECode CBulkCursorToCursorAdaptor::IsFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isFloat)
{
    VALIDATE_NOT_NULL(isFloat);

    return BulkCursorToCursorAdaptor::IsFloat(columnIndex, isFloat);
}

ECode CBulkCursorToCursorAdaptor::SetWindow(
    /* [in] */ ICursorWindow* window)
{
    return BulkCursorToCursorAdaptor::SetWindow(window);
}

ECode CBulkCursorToCursorAdaptor::HasWindow(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = BulkCursorToCursorAdaptor::HasWindow();
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::Set(
    /* [in] */ IBulkCursor* cursor)
{
    return BulkCursorToCursorAdaptor::Set(cursor);
}

ECode CBulkCursorToCursorAdaptor::SetEx(
    /* [in] */ IBulkCursor* cursor,
    /* [in] */ Int32 count,
    /* [in] */ Int32 idIndex)
{
    return BulkCursorToCursorAdaptor::Set(cursor, count, idIndex);
}

ECode CBulkCursorToCursorAdaptor::GetObserver(
    /* [out] */ IContentObserver** observer)
{
    VALIDATE_NOT_NULL(observer);

    AutoPtr<IContentObserver> _observer = BulkCursorToCursorAdaptor::GetObserver();
    *observer = _observer.Get();
    if (*observer != NULL) (*observer)->AddRef();
    return NOERROR;
}

