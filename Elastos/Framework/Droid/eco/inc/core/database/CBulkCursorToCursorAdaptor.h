
#ifndef __CBULKCURSORTOCURSORADAPTOR_H__
#define __CBULKCURSORTOCURSORADAPTOR_H__

#include "_CBulkCursorToCursorAdaptor.h"
#include "database/BulkCursorToCursorAdaptor.h"
#include "database/CursorMacro.h"

CarClass(CBulkCursorToCursorAdaptor), public BulkCursorToCursorAdaptor
{
public:
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

    CARAPI IsBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isBlob);

    CARAPI IsString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isString);

    CARAPI IsInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isInt64);

    CARAPI IsFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isFloat);

    CARAPI SetWindow(
        /* [in] */ ICursorWindow* window);

    CARAPI HasWindow(
        /* [out] */ Boolean* result);

    CARAPI Set(
        /* [in] */ IBulkCursor* bulkCursor);

    CARAPI SetEx(
        /* [in] */ IBulkCursor* bulkCursor,
        /* [in] */ Int32 count,
        /* [in] */ Int32 idIndex);

    CARAPI GetObserver(
        /* [out] */ IContentObserver** observer);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBULKCURSORTOCURSORADAPTOR_H__
