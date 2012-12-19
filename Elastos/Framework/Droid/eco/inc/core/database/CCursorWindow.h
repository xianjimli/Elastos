
#ifndef __CCURSORWINDOW_H__
#define __CCURSORWINDOW_H__

#include "_CCursorWindow.h"
#include "database/CursorWindow.h"

CarClass(CCursorWindow), public CursorWindow
{
public:
    CARAPI constructor(
        /* [in] */ Boolean localWindow);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

    CARAPI GetStartPosition(
        /* [out] */ Int32* pos);

    CARAPI SetStartPosition(
        /* [in] */ Int32 pos);

    CARAPI GetNumRows(
        /* [out] */ Int32* num);

    CARAPI SetNumColumns(
        /* [in] */ Int32 columnNum,
        /* [out] */ Boolean* succeeded);

    CARAPI AllocRow(
        /* [out] */ Boolean* succeeded);

    CARAPI FreeLastRow();

    CARAPI PutBlob(
        /* [in] */ const ArrayOf<Byte>& value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI PutString(
        /* [in] */ const String& value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI PutInt64(
        /* [in] */ Int64 value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI PutDouble(
        /* [in] */ Double value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI PutNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI IsNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isNull);

    CARAPI GetBlob(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out, callee] */ ArrayOf<Byte>** blob);

    CARAPI IsBlob(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isBlob);

    CARAPI IsInt64(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isInt64);

    CARAPI IsFloat(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isFloat);

    CARAPI IsString(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isString);

    CARAPI GetString(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ String* str);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [in] */ ICharArrayBuffer* buffer);

    CARAPI GetInt64(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int64* value);

    CARAPI GetDouble(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Double* value);

    CARAPI GetInt16(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int16* value);

    CARAPI GetInt32(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int32* value);

    CARAPI GetFloat(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Float* value);

    CARAPI Clear();

    CARAPI Close();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCURSORWINDOW_H__
