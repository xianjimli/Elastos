#ifndef __ABSTRACTCURSOR_H__
#define __ABSTRACTCURSOR_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include "database/DataSetObservable.h"
#include "database/ContentObservable.h"
#include "content/ContentResolver.h"
#include "database/CharArrayBuffer.h"
#include <elastos/HashMap.h>
#include <elastos/Map.h>

class AbstractCursor
{
public:
    AbstractCursor();

    virtual CARAPI GetCount(
        /* [out] */ Int32* cnt) = 0;

    virtual GetColumnNames(
        /* [out] */ ArrayOf<String>** names) = 0;

    virtual CARAPI GetString(
        /* [in] */ Int32 column,
        /* [out] */ String* value) = 0;

    virtual CARAPI GetShort(
        /* [in] */ Int32 column,
        /* [out] */ Int16* value) = 0;

    virtual CARAPI GetInt(
        /* [in] */ Int32 column,
        /* [out] */ Int32* value) = 0;

    virtual CARAPI GetLong(
        /* [in] */ Int32 column,
        /* [out] */ Int64* value) = 0;

    virtual CARAPI GetFloat(
        /* [in] */ Int32 column,
        /* [out] */ Float* value) = 0;

    virtual CARAPI GetDouble(
        /* [in] */ Int32 column,
        /* [out] */ Double* value) = 0;

    virtual CARAPI IsNull(
        /* [in] */ Int32 column,
        /* [out] */ Boolean* value) = 0;

    // TODO implement getBlob in all cursor types
    virtual CARAPI GetBlob(
        /* [in] */ Int32 column,
        /* [out] */ ArrayOf<Byte>** blob);

    /* Methods that may optionally be implemented by subclasses */
    /**@implements
     * returns a pre-filled window, return NULL if no such window
     */            
    CARAPI GetWindow(
        /* [out] */ ICursorWindow** window);

    virtual CARAPI GetColumnCount(
        /* [out] */ Int32* count);

    virtual CARAPI Deactivate();

    virtual CARAPI DeactivateInternal();

    virtual CARAPI Requery(
        /* [out] */ Boolean* value);

    virtual CARAPI IsClosed(
        /* [out] */ Boolean* value);

    virtual CARAPI Close();

    /**
     * @hide
     * @deprecated
     */
//    public boolean commitUpdates(Map<? extends Long,? extends Map<String,Object>> values) {
//        return false;
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean deleteRow() {
//        return false;
//    }
    /**@implements
     * This function is called every time the cursor is successfully scrolled
     * to a new position, giving the subclass a chance to update any state it
     * may have. If it returns false the move function will also do so and the
     * cursor will scroll to the beforeFirst position.
     *
     * @param oldPosition the position that we're moving from
     * @param newPosition the position that we're moving to
     * @return true if the move is successful, false otherwise
     */
    CARAPI OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean* value);
    
    virtual CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ CharArrayBuffer* buffer);

    CARAPI GetPosition(
        /* [out] */ Int32* position);

    CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* rst);

    /**@implements
     * Copy data from cursor to CursorWindow
     * @param position start position of data
     * @param window
     */
    CARAPI FillWindow(
        /* [in] */ Int32 position,
        /* [in] */ ICursorWindow* window);

    CARAPI Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* rst);

    CARAPI MoveToFirst(
        /* [out] */ Boolean* rst);

    CARAPI MoveToLast(
        /* [out] */ Boolean* rst);

    CARAPI MoveToNext(
        /* [out] */ Boolean* rst);

    CARAPI MoveToPrevious(
        /* [out] */ Boolean* rst);

    CARAPI IsFirst(
        /* [out] */ Boolean* rst);

    CARAPI IsLast(
        /* [out] */ Boolean* rst);

    CARAPI IsBeforeFirst(
        /* [out] */ Boolean* rst);

    CARAPI IsAfterLast(
        /* [out] */ Boolean* rst);

    virtual CARAPI GetColumnIndex(
        /* [in] */ String columnName,
        /* [out] */ Int32* index);

    virtual CARAPI GetColumnIndexOrThrow(
        /* [in] */ String columnName,
        /* [out] */ Int32* index);

    virtual CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* name);

    virtual CARAPI RegisterContentObserver(
        /* [in] */ IContentObserver* observer);

    virtual CARAPI UnregisterContentObserver(
        /* [in] */ IContentObserver* observer);

    virtual CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    virtual CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    /**
     * Specifies a content URI to watch for changes.
     *
     * @param cr The content resolver from the caller's context.
     * @param notifyUri The URI to watch for changes. This can be a
     * specific row URI, or a base URI for a whole class of content.
     */
     virtual CARAPI SetNotificationUri(
         /* [in] */ IContentResolver* cr,
         /* [in] */ IUri* notifyUri);

     virtual CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result);

     virtual CARAPI GetExtras(
        /* [out] */ IBundle** extras);

     virtual CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** result);

protected:
    virtual ~AbstractCursor();

    /**
     * This is hidden until the data set change model has been re-evaluated.
     * @hide
     */
    virtual CARAPI NotifyDataSetChange();
    
    /**
     * This is hidden until the data set change model has been re-evaluated.
     * @hide
     */
    virtual CARAPI GetDataSetObservable(
        /* [out] */ DataSetObservable** dso);

    /**
     * Subclasses must call this method when they finish committing updates to notify all
     * observers.
     *
     * @param selfChange
     */
    virtual CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    /**
     * This function returns true if the field has been updated and is
     * used in conjunction with {@link #getUpdatedField} to allow subclasses to
     * support reading uncommitted updates. NOTE: This function and
     * {@link #getUpdatedField} should be called together inside of a
     * block synchronized on mUpdatedRows.
     *
     * @param columnIndex the column index of the field to check
     * @return true if the field has been updated, false otherwise
     */
    virtual CARAPI IsFieldUpdated(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* rst);

    /**
     * This function returns the uncommitted updated value for the field
     * at columnIndex.  NOTE: This function and {@link #isFieldUpdated} should
     * be called together inside of a block synchronized on mUpdatedRows.
     *
     * @param columnIndex the column index of the field to retrieve
     * @return the updated value
     */
    virtual CARAPI GetUpdatedField(
        /* [in] */ Int32 columnIndex,
        /* [out] */ IInterface** obj);

    /**
     * This function throws CursorIndexOutOfBoundsException if
     * the cursor position is out of bounds. Subclass implementations of
     * the get functions should call this before attempting
     * to retrieve data.
     *
     * @throws CursorIndexOutOfBoundsException
     */
    virtual CARAPI CheckPosition();

//    class SelfContentObserver : public IContentObserver {
//    public:
//        SelfContentObserver();

//        ~SelfContentObserver();

//        CARAPI Init(
//            /* [in] */ IAbstractCursor* cursor);

        //@Override
//        virtual CARAPI DeliverSelfNotifications(
//            /* [out] */ Boolean* rst);

        //@Override
//        virtual CARAPI OnChange(
//            /* [in] */ Boolean* selfChange);

//    public:
//        WeakReference<AbstractCursor> mCursor;
//    };

private:
    const static String TAG;
    AutoPtr<IUri> mNotifyUri;
    AutoPtr<IContentObserver> mSelfObserver;

    //final private Object mSelfObserverLock = new Object();
    Mutex mSelfObserverLock;

    Boolean mSelfObserverRegistered;

protected:
    DataSetObservable* mDataSetObservable;
    ContentObservable* mContentObservable;
    /**
     * This HashMap contains a mapping from Long rowIDs to another Map
     * that maps from String column names to new values. A NULL value means to
     * remove an existing value, and all numeric values are in their class
     * forms, i.e. Integer, Long, Float, etc.
     */
    HashMap<Int64, Map<String, AutoPtr<IInterface> > > *mUpdatedRows;
    Mutex mUpdatedRowsLock;

    /**
     * This must be set to the index of the row ID column by any
     * subclass that wishes to support updates.
     */
    Int32 mRowIdColumnIndex;

    Int32 mPos;
    Int64 mCurrentRowID;
    ContentResolver* mContentResolver;
    Boolean mClosed;

};
#endif //__ABSTRACTCURSOR_H__