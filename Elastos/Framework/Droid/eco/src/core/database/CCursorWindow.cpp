
#include "database/CCursorWindow.h"
ECode CCursorWindow::OnAllReferencesReleased()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::OnAllReferencesReleasedFromContainer()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::AcquireReference()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::ReleaseReference()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::ReleaseReferenceFromContainer()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetStartPosition(
    /* [out] */ Int32 * pPos)
{
    CursorWindow::GetStartPosition(pPos);
    return NOERROR;
}

ECode CCursorWindow::SetStartPosition(
    /* [in] */ Int32 pos)
{
    CursorWindow::SetStartPosition(pos);
    return NOERROR;
}

ECode CCursorWindow::GetNumRows(
    /* [out] */ Int32 * pNum)
{
    CursorWindow::GetNumRows(pNum);
    return NOERROR;
}

ECode CCursorWindow::SetNumColumns(
    /* [in] */ Int32 columnNum,
    /* [out] */ Boolean * pSucceeded)
{
    CursorWindow::SetNumColumns(columnNum, pSucceeded);
    return NOERROR;
}

ECode CCursorWindow::AllocRow(
    /* [out] */ Boolean * pSucceeded)
{
    CursorWindow::AllocRow(pSucceeded);
    return NOERROR;
}

ECode CCursorWindow::FreeLastRow()
{
    CursorWindow::FreeLastRow();
    return NOERROR;
}

ECode CCursorWindow::PutBlob(
    /* [in] */ const ArrayOf<Byte> & value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    CursorWindow::PutBlob(value, row, col, pSucceeded);
    return NOERROR;
}

ECode CCursorWindow::PutString(
    /* [in] */ const String& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    CursorWindow::PutString(value, row, col, pSucceeded);
    return NOERROR;
}

ECode CCursorWindow::PutInt64(
    /* [in] */ Int64 value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    CursorWindow::PutInt64(value, row, col, pSucceeded);
    return NOERROR;
}

ECode CCursorWindow::PutDouble(
    /* [in] */ Double value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    CursorWindow::PutDouble(value, row, col, pSucceeded);
    return NOERROR;
}

ECode CCursorWindow::PutNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    CursorWindow::PutNull(row, col, pSucceeded);
    return NOERROR;
}

ECode CCursorWindow::IsNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    CursorWindow::IsNull(row, col, pSucceeded);
    return NOERROR;
}

ECode CCursorWindow::GetBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out, callee] */ ArrayOf<Byte> ** ppBlob)
{
    CursorWindow::GetBlob(row, col, ppBlob);
    return NOERROR;
}

ECode CCursorWindow::IsBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsBlob)
{
    CursorWindow::IsBlob(row, col, pIsBlob);
    return NOERROR;
}

ECode CCursorWindow::IsInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsInt64)
{
    CursorWindow::IsInt64(row, col, pIsInt64);
    return NOERROR;
}

ECode CCursorWindow::IsFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsFloat)
{
    CursorWindow::IsFloat(row, col, pIsFloat);
    return NOERROR;
}

ECode CCursorWindow::IsString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsString)
{
    CursorWindow::IsString(row, col, pIsString);
    return NOERROR;
}

ECode CCursorWindow::GetString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ String * pRst)
{
    CursorWindow::GetString(row, col, pRst);
    return NOERROR;
}

ECode CCursorWindow::CopyStringToBuffer(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ ICharArrayBuffer * pBuffer)
{
    CursorWindow::CopyStringToBuffer(row, col, pBuffer);
    return NOERROR;
}

ECode CCursorWindow::GetInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int64 * pValue)
{
    CursorWindow::GetInt64(row, col, pValue);
    return NOERROR;
}

ECode CCursorWindow::GetDouble(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Double * pValue)
{
    CursorWindow::GetDouble(row, col, pValue);
    return NOERROR;
}

ECode CCursorWindow::GetInt16(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int16 * pValue)
{
    CursorWindow::GetInt16(row, col, pValue);
    return NOERROR;
}

ECode CCursorWindow::GetInt32(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int32 * pValue)
{
    CursorWindow::GetInt32(row, col, pValue);
    return NOERROR;
}

ECode CCursorWindow::GetFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Float * pValue)
{
    CursorWindow::GetFloat(row, col, pValue);
    return NOERROR;
}

ECode CCursorWindow::Clear()
{
    CursorWindow::Clear();
    return NOERROR;
}

ECode CCursorWindow::Close()
{
    CursorWindow::Close();
    return NOERROR;
}

ECode CCursorWindow::ReadFromParcel(
    /* [in] */ IParcel * pSource)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::WriteToParcel(
    /* [in] */ IParcel * pDest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::constructor()
{
    CursorWindow::CursorWindow();
    return NOERROR;
}

ECode CCursorWindow::constructor(
    /* [in] */ Boolean localWindow)
{
    CursorWindow::Init(localWindow);
    return NOERROR;
}

