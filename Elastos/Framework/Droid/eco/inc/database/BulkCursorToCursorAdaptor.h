#ifndef __BULKCURSORTOCURSORADAPTOR_H__
#define __BULKCURSORTOCURSORADAPTOR_H__

#include "ext/frameworkext.h"
#include "database/AbstractWindowedCursor.h"
#include <elastos/AutoPtr.h>
/**
 * Adapts an {@link IBulkCursor} to a {@link Cursor} for use in the local
 * process.
 *
 * {@hide}
 */
class BulkCursorToCursorAdaptor : public AbstractWindowedCursor
{
public:
    CARAPI Set1(
            /* [in] */ IBulkCursor* bulkCursor);

    /**
     * Version of set() that does fewer Binder calls if the caller
     * already knows BulkCursorToCursorAdaptor's properties.
     */
    CARAPI Set2(
            /* [in] */ IBulkCursor* bulkCursor,
            /* [in] */ Int32 count,
            /* [in] */ Int32 idIndex);

    static CARAPI FindRowIdColumnIndex(
            /* [in] */ ArrayOf<String>* columnNames,
            /* [out] */ Int32* index);

    /**
     * Gets a SelfDataChangeOberserver that can be sent to a remote
     * process to receive change notifications over IPC.
     *
     * @return A SelfContentObserver hooked up to this Cursor
     */
    CARAPI GetObserver(
            /* [out] */ IContentObserver** ico);

//    @Override
    CARAPI GetCount(
            /* [out] */ Int32* cnt);

//    @Override
    CARAPI OnMove(
            /* [in] */ Int32 oldPosition,
            /* [in] */ Int32 newPosition,
            /* [out] */ Boolean* rst);

//    @Override
    CARAPI Deactivate();

//    @Override
    CARAPI Close();

//    @Override
    CARAPI Requery(
            /* [out] */ Boolean* rst);

    /**
     * @hide
     * @deprecated
     */
//    @Override
//    public boolean deleteRow()

//    @Override
    CARAPI GetColumnNames(
            /* [out] */ ArrayOf<String>** names);

    /**
     * @hide
     * @deprecated
     */
//    @Override
//    public boolean commitUpdates(Map<? extends Long,
//            ? extends Map<String,Object>> additionalValues)

//    @Override
    CARAPI GetExtras(
            /* [out] */ IBundle** extras);

//    @Override
    CARAPI Respond(
            /* [in] */ IBundle* extras,
            /* [out] */ IBundle** v);
private:
    static const String TAG;

//    private SelfContentObserver mObserverBridge;
    AutoPtr<IBulkCursor> mBulkCursor;
    Int32 mCount;
    ArrayOf<String> *mColumns;
    Boolean mWantsAllOnMoveCalls;
};


#endif //__BULKCURSORTOCURSORADAPTOR_H__