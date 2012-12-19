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
    BulkCursorToCursorAdaptor();

    virtual ~BulkCursorToCursorAdaptor();

    CARAPI Set(
        /* [in] */ IBulkCursor* bulkCursor);

    /**
     * Version of set() that does fewer Binder calls if the caller
     * already knows BulkCursorToCursorAdaptor's properties.
     */
    CARAPI Set(
        /* [in] */ IBulkCursor* bulkCursor,
        /* [in] */ Int32 count,
        /* [in] */ Int32 idIndex);

    /**
     * Returns column index of "_id" column, or -1 if not found.
     */
    static CARAPI_(Int32) FindRowIdColumnIndex(
        /* [in] */ ArrayOf<String>* columnNames);

    /**
     * Gets a SelfDataChangeOberserver that can be sent to a remote
     * process to receive change notifications over IPC.
     *
     * @return A SelfContentObserver hooked up to this Cursor
     */
    CARAPI_(AutoPtr<IContentObserver>) GetObserver();

//    @Override
    CARAPI_(Int32) GetCount();

//    @Override
    CARAPI_(Boolean) OnMove(
            /* [in] */ Int32 oldPosition,
            /* [in] */ Int32 newPosition);

//    @Override
    CARAPI Deactivate();

//    @Override
    CARAPI Close();

//    @Override
    CARAPI Requery(
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    // @Override
    CARAPI DeleteRow(
        /* [out] */ Boolean* succeeded);

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
    static const CString TAG;

//    private SelfContentObserver mObserverBridge;
    AutoPtr<IBulkCursor> mBulkCursor;
    Int32 mCount;
    ArrayOf<String>* mColumns;
    Boolean mWantsAllOnMoveCalls;

    Mutex mLock;
};


#endif //__BULKCURSORTOCURSORADAPTOR_H__