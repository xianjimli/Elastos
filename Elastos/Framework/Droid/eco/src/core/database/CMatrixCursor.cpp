
#include "database/CMatrixCursor.h"


ECode CMatrixCursor::constructor(
    /* [in] */ ArrayOf<String>* columnNames,
    /* [in] */ Int32 initialCapacity)
{
    return MatrixCursor::Init(columnNames, initialCapacity);
}

ECode CMatrixCursor::constructor(
    /* [in] */ ArrayOf<String>* columnNames)
{
    return MatrixCursor::Init(columnNames);
}

ICURSOR_METHODS_IMPL(CMatrixCursor, MatrixCursor, MatrixCursor);

ECode CMatrixCursor::GetWindow(
    /* [out] */ ICursorWindow** window)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow* window)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::NewRow(
    /* [out] */ IRowBuilder** builder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::AddRow(
    /* [in] */ const ArrayOf<IInterface*>& columnValues)
{
    return MatrixCursor::AddRow(columnValues);
}

ECode CMatrixCursor::AddRowEx(
    /* [in] */ IObjectContainer* columnValues)
{
    return MatrixCursor::AddRow(columnValues);
}

