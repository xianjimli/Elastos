
#include "ext/frameworkdef.h"
#include "database/CCursorWindow.h"


ECode CCursorWindow::constructor(
    /* [in] */ Boolean localWindow)
{
    return CursorWindow::Init(localWindow);
}

PInterface CCursorWindow::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_SQLiteClosable) {
        return reinterpret_cast<PInterface>((SQLiteClosable*)this);
    }
    else return _CCursorWindow::Probe(riid);
}

ECode CCursorWindow::AcquireReference()
{
    return CursorWindow::AcquireReference();
}

ECode CCursorWindow::ReleaseReference()
{
    return CursorWindow::ReleaseReference();
}

ECode CCursorWindow::ReleaseReferenceFromContainer()
{
    return CursorWindow::ReleaseReferenceFromContainer();
}

ECode CCursorWindow::GetStartPosition(
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);

    *pos = CursorWindow::GetStartPosition();
    return NOERROR;
}

ECode CCursorWindow::SetStartPosition(
    /* [in] */ Int32 pos)
{
    return CursorWindow::SetStartPosition(pos);
}

ECode CCursorWindow::GetNumRows(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    *num = CursorWindow::GetNumRows();
    return NOERROR;
}

ECode CCursorWindow::SetNumColumns(
    /* [in] */ Int32 columnNum,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    *succeeded = CursorWindow::SetNumColumns(columnNum);
    return NOERROR;
}

ECode CCursorWindow::AllocRow(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    *succeeded = CursorWindow::AllocRow();
    return NOERROR;
}

ECode CCursorWindow::FreeLastRow()
{
    return CursorWindow::FreeLastRow();
}

ECode CCursorWindow::PutBlob(
    /* [in] */ const ArrayOf<Byte>& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    *succeeded = CursorWindow::PutBlob(value, row, col);
    return NOERROR;
}

ECode CCursorWindow::PutString(
    /* [in] */ const String& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    *succeeded = CursorWindow::PutString(value, row, col);
    return NOERROR;
}

ECode CCursorWindow::PutInt64(
    /* [in] */ Int64 value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    *succeeded = CursorWindow::PutInt64(value, row, col);
    return NOERROR;
}

ECode CCursorWindow::PutDouble(
    /* [in] */ Double value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    *succeeded = CursorWindow::PutDouble(value, row, col);
    return NOERROR;
}

ECode CCursorWindow::PutNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    *succeeded = CursorWindow::PutNull(row, col);
    return NOERROR;
}

ECode CCursorWindow::IsNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isNull)
{
    VALIDATE_NOT_NULL(isNull);

    *isNull = CursorWindow::IsNull(row, col);
    return NOERROR;
}

ECode CCursorWindow::GetBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out, callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob);

    return CursorWindow::GetBlob(row, col, blob);
}

ECode CCursorWindow::IsBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isBlob)
{
    VALIDATE_NOT_NULL(isBlob);

    return CursorWindow::IsBlob(row, col, isBlob);
}

ECode CCursorWindow::IsInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isInt64)
{
    VALIDATE_NOT_NULL(isInt64);

    return CursorWindow::IsInt64(row, col, isInt64);
}

ECode CCursorWindow::IsFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isFloat)
{
    VALIDATE_NOT_NULL(isFloat);

    return CursorWindow::IsFloat(row, col, isFloat);
}

ECode CCursorWindow::IsString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isString)
{
    VALIDATE_NOT_NULL(isString);

    return CursorWindow::IsString(row, col, isString);
}

ECode CCursorWindow::GetString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return CursorWindow::GetString(row, col, str);
}

ECode CCursorWindow::CopyStringToBuffer(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ ICharArrayBuffer* buffer)
{
    return CursorWindow::CopyStringToBuffer(row, col, buffer);
}

ECode CCursorWindow::GetInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    return CursorWindow::GetInt64(row, col, value);
}

ECode CCursorWindow::GetDouble(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    return CursorWindow::GetDouble(row, col, value);
}

ECode CCursorWindow::GetInt16(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    return CursorWindow::GetInt16(row, col, value);
}

ECode CCursorWindow::GetInt32(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return CursorWindow::GetInt32(row, col, value);
}

ECode CCursorWindow::GetFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    return CursorWindow::GetFloat(row, col, value);
}

ECode CCursorWindow::Clear()
{
    return CursorWindow::Clear();
}

ECode CCursorWindow::Close()
{
    return CursorWindow::Close();
}

ECode CCursorWindow::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWindow::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}
