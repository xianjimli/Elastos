#ifndef __ABSTRACTCURSOR_H__
#define __ABSTRACTCURSOR_H__

#include "ext/frameworkext.h"
#include "database/DataSetObservable.h"
#include "database/ContentObservable.h"
#include "database/CharArrayBuffer.h"
#include "content/ContentResolver.h"
#include <elastos/Map.h>
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

/**
 * This is an abstract cursor class that handles a lot of the common code
 * that all cursors need to deal with and is provided for convenience reasons.
 */
class AbstractCursor
{
public:
    AbstractCursor();

    virtual ~AbstractCursor();

    virtual CARAPI_(Int32) GetCount() = 0;

    virtual CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String>** names) = 0;

    virtual CARAPI GetString(
        /* [in] */ Int32 column,
        /* [out] */ String* str) = 0;

    virtual CARAPI GetInt16(
        /* [in] */ Int32 column,
        /* [out] */ Int16* value) = 0;

    virtual CARAPI GetInt32(
        /* [in] */ Int32 column,
        /* [out] */ Int32* value) = 0;

    virtual CARAPI GetInt64(
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
        /* [out] */ Boolean* isNull) = 0;

    // TODO implement getBlob in all cursor types
    virtual CARAPI GetBlob(
        /* [in] */ Int32 column,
        /* [out] */ ArrayOf<Byte>** blob);

    /* Methods that may optionally be implemented by subclasses */
    /**@implements
     * returns a pre-filled window, return NULL if no such window
     */
    virtual CARAPI_(AutoPtr<ICursorWindow>) GetWindow();

    virtual CARAPI_(Int32) GetColumnCount();

    virtual CARAPI Deactivate();

    virtual CARAPI DeactivateInternal();

    virtual CARAPI Requery(
        /* [out] */ Boolean* result);

    virtual CARAPI_(Boolean) IsClosed();

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
    virtual CARAPI DeleteRow(
        /* [out] */ Boolean* succeeded);

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
    virtual CARAPI_(Boolean) OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition);

    virtual CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer* buffer);

    CARAPI_(Int32) GetPosition();

    CARAPI_(Boolean) MoveToPosition(
        /* [in] */ Int32 position);

    /**@implements
     * Copy data from cursor to CursorWindow
     * @param position start position of data
     * @param window
     */
    virtual CARAPI FillWindow(
        /* [in] */ Int32 position,
        /* [in] */ ICursorWindow* window);

    CARAPI_(Boolean) Move(
        /* [in] */ Int32 offset);

    CARAPI_(Boolean) MoveToFirst();

    CARAPI_(Boolean) MoveToLast();

    CARAPI_(Boolean) MoveToNext();

    CARAPI_(Boolean) MoveToPrevious();

    CARAPI_(Boolean) IsFirst();

    CARAPI_(Boolean) IsLast();

    CARAPI_(Boolean) IsBeforeFirst();

    CARAPI_(Boolean) IsAfterLast();

    virtual CARAPI_(Int32) GetColumnIndex(
        /* [in] */ const String& columnName);

    virtual CARAPI GetColumnIndexOrThrow(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* index);

    virtual CARAPI_(String) GetColumnName(
        /* [in] */ Int32 columnIndex);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) UpdateBlob(
        /* [in] */ Int32 columnIndex,
        /* [in, out] */ const ArrayOf<Byte>& value);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) UpdateString(
        /* [in] */ Int32 columnIndex,
        /* [in] */ const String& value);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) UpdateInt16(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int16 value);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) UpdateInt32(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 value);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) UpdateInt64(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int64 value);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) UpdateFloat(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Float value);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) UpdateDouble(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Double value);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) UpdateToNull(
        /* [in] */ Int32 columnIndex);

    /**
     * @hide
     * @deprecated
     */
    // public boolean update(int columnIndex, Object obj)

    /**
     * Returns <code>true</code> if there are pending updates that have not yet been committed.
     *
     * @return <code>true</code> if there are pending updates that have not yet been committed.
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) HasUpdates();

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI AbortUpdates();

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI CommitUpdates(
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI_(Boolean) SupportsUpdates();

    virtual CARAPI RegisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    virtual CARAPI UnregisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

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

     virtual CARAPI_(Boolean) GetWantsAllOnMoveCalls();

     virtual CARAPI GetExtras(
        /* [out] */ IBundle** extras);

     virtual CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** v);

protected:
    /**
     * This is hidden until the data set change model has been re-evaluated.
     * @hide
     */
    virtual CARAPI_(void) NotifyDataSetChange();

    /**
     * This is hidden until the data set change model has been re-evaluated.
     * @hide
     */
    virtual CARAPI_(AutoPtr<IDataSetObservable>) GetDataSetObservable();

    /**
     * Subclasses must call this method when they finish committing updates to notify all
     * observers.
     *
     * @param selfChange
     */
    virtual CARAPI_(void) OnChange(
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
    virtual CARAPI_(Boolean) IsFieldUpdated(
        /* [in] */ Int32 columnIndex);

    /**
     * This function returns the uncommitted updated value for the field
     * at columnIndex.  NOTE: This function and {@link #isFieldUpdated} should
     * be called together inside of a block synchronized on mUpdatedRows.
     *
     * @param columnIndex the column index of the field to retrieve
     * @return the updated value
     */
    virtual CARAPI_(AutoPtr<IInterface>) GetUpdatedField(
        /* [in] */ Int32 columnIndex);

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

protected:
    AutoPtr<IDataSetObservable> mDataSetObservable;
    AutoPtr<IContentObservable> mContentObservable;

    /**
     * This HashMap contains a mapping from Long rowIDs to another Map
     * that maps from String column names to new values. A NULL value means to
     * remove an existing value, and all numeric values are in their class
     * forms, i.e. Integer, Long, Float, etc.
     */
    HashMap<Int64, Map<String, AutoPtr<IInterface> >* > mUpdatedRows;
    Mutex mUpdatedRowsLock;

    /**
     * This must be set to the index of the row ID column by any
     * subclass that wishes to support updates.
     */
    Int32 mRowIdColumnIndex;

    Int32 mPos;
    Int64 mCurrentRowID;
    AutoPtr<IContentResolver> mContentResolver;
    Boolean mClosed;

private:
    const static CString TAG;
    AutoPtr<IUri> mNotifyUri;
    AutoPtr<ILocalContentObserver> mSelfObserver;
    Mutex mSelfObserverLock;

    Boolean mSelfObserverRegistered;
};
#endif //__ABSTRACTCURSOR_H__
