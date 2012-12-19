
#ifndef __CMATRIXCURSOR_H__
#define __CMATRIXCURSOR_H__

#include "_CMatrixCursor.h"
#include "database/MatrixCursor.h"
#include "database/CursorMacro.h"

CarClass(CMatrixCursor), public MatrixCursor
{
public:
    CARAPI constructor(
        /* [in] */ ArrayOf<String>* columnNames,
        /* [in] */ Int32 initialCapacity);

    CARAPI constructor(
        /* [in] */ ArrayOf<String>* columnNames);

    ICURSOR_METHODS_DECL();

    CARAPI GetWindow(
        /* [out] */ ICursorWindow** window);

    CARAPI FillWindow(
        /* [in] */ Int32 pos,
        /* [in] */ ICursorWindow* window);

    CARAPI OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean* succeeded);

    CARAPI NewRow(
        /* [out] */ IRowBuilder** builder);

    CARAPI AddRow(
        /* [in] */ const ArrayOf<IInterface*>& columnValues);

    CARAPI AddRowEx(
        /* [in] */ IObjectContainer* columnValues);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMATRIXCURSOR_H__
