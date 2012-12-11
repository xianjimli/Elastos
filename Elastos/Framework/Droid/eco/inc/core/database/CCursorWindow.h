
#ifndef __CCURSORWINDOW_H__
#define __CCURSORWINDOW_H__

#include "_CCursorWindow.h"
#include "database/CursorWindow.h"
CarClass(CCursorWindow), public CursorWindow
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI OnAllReferencesReleased();

    CARAPI OnAllReferencesReleasedFromContainer();

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

    CARAPI GetStartPosition(
        /* [out] */ Int32 * pPos);

    CARAPI SetStartPosition(
        /* [in] */ Int32 pos);

    CARAPI GetNumRows(
        /* [out] */ Int32 * pNum);

    CARAPI SetNumColumns(
        /* [in] */ Int32 columnNum,
        /* [out] */ Boolean * pSucceeded);

    CARAPI AllocRow(
        /* [out] */ Boolean * pSucceeded);

    CARAPI FreeLastRow();

    CARAPI PutBlob(
        /* [in] */ const ArrayOf<Byte> & value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pSucceeded);

    CARAPI PutString(
        /* [in] */ const String& value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pSucceeded);

    CARAPI PutInt64(
        /* [in] */ Int64 value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pSucceeded);

    CARAPI PutDouble(
        /* [in] */ Double value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pSucceeded);

    CARAPI PutNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pSucceeded);

    CARAPI IsNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pSucceeded);

    CARAPI GetBlob(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out, callee] */ ArrayOf<Byte> ** ppBlob);

    CARAPI IsBlob(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pIsBlob);

    CARAPI IsInt64(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pIsInt64);

    CARAPI IsFloat(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pIsFloat);

    CARAPI IsString(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean * pIsString);

    CARAPI GetString(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ String * pRst);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [in] */ ICharArrayBuffer * pBuffer);

    CARAPI GetInt64(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int64 * pValue);

    CARAPI GetDouble(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Double * pValue);

    CARAPI GetInt16(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int16 * pValue);

    CARAPI GetInt32(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int32 * pValue);

    CARAPI GetFloat(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Float * pValue);

    CARAPI Clear();

    CARAPI Close();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel * pSource);

    CARAPI WriteToParcel(
        /* [in] */ IParcel * pDest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean localWindow);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCURSORWINDOW_H__
