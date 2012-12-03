
#include "database/sqlite/CSQLiteCursor.h"
ECode CSQLiteCursor::OnAllReferencesReleased()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::OnAllReferencesReleasedFromContainer()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::AcquireReference()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::ReleaseReference()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::ReleaseReferenceFromContainer()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetStartPosition(
    /* [out] */ Int32 * pPos)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::SetStartPosition(
    /* [in] */ Int32 pos)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetNumRows(
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::SetNumColumns(
    /* [in] */ Int32 columnNum,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::AllocRow(
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::FreeLastRow()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::PutBlob(
    /* [in] */ const ArrayOf<Byte> & value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::PutString(
    /* [in] */ const String& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::PutInt64(
    /* [in] */ Int64 value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::PutDouble(
    /* [in] */ Double value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::PutNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out, callee] */ ArrayOf<Byte> ** ppBlob)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsBlob)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsInt64)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsFloat)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean * pIsString)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ String * pRst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::CopyStringToBuffer(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ ICharArrayBuffer * pBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetDouble(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Double * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetInt16(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int16 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetInt32(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Float * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::Clear()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::SetLoadStyle(
    /* [in] */ Int32 initialRead,
    /* [in] */ Int32 maxRead)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetDatabase()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::SetSelectionArguments(
    /* [in] */ const ArrayOf<String> & selectionArgs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

