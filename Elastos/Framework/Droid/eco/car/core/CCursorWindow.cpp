
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::SetStartPosition(
    /* [in] */ Int32 pos)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetNumRows(
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::SetNumColumns(
    /* [in] */ Int32 columnNum,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::AllocRow(
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::FreeLastRow()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::PutBlob(
    /* [in] */ const ArrayOf<Byte> & value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::PutString(
    /* [in] */ const String& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::PutInt64(
    /* [in] */ Int64 value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::PutDouble(
    /* [in] */ Double value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::PutNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::IsNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out, callee] */ ArrayOf<Byte> ** ppBlob)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::IsBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsBlob)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::IsInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsInt64)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::IsFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsFloat)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::IsString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsString)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ String * pRst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::CopyStringToBuffer(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ ICharArrayBuffer * pBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetDouble(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Double * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetInt16(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int16 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetInt32(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::GetFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Float * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::Clear()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::constructor(
    /* [in] */ Boolean localWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

