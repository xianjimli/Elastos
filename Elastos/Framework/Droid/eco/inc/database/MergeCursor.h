#ifndef __MERGECURSOR_H__
#define __MERGECURSOR_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "database/AbstractCursor.h"
#include "database/DataSetObserver.h"
/**
 * A convience class that lets you present an array of Cursors as a single linear Cursor.
 * The schema of the cursors presented is entirely up to the creator of the MergeCursor, and
 * may be different if that is desired. Calls to getColumns, getColumnIndex, etc will return the
 * value for the row that the MergeCursor is currently pointing at.
 */
class MergeCursor : public AbstractCursor
{
public:
    MergeCursor();

    MergeCursor(
        /* [in] */ ArrayOf<AutoPtr<ICursor> >* cursors);

    ~MergeCursor();

    CARAPI GetCount(
        /* [out] */ Int32* cnt);

    CARAPI OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean* value);

    /**
     * @hide
     * @deprecated
     */
//    @Override
//    public boolean deleteRow()

    /**
     * @hide
     * @deprecated
     */
//    @Override
//    public boolean commitUpdates()

    CARAPI GetString(
        /* [in] */ Int32 column,
        /* [out] */ String* value);

    CARAPI GetInt16(
        /* [in] */ Int32 column,
        /* [out] */ Int16* value);

    CARAPI GetInt32(
        /* [in] */ Int32 column,
        /* [out] */ Int32* value);

    CARAPI GetInt64(
        /* [in] */ Int32 column,
        /* [out] */ Int64* value);

    CARAPI GetFloat(
        /* [in] */ Int32 column,
        /* [out] */ Float* value);

    CARAPI GetDouble(
        /* [in] */ Int32 column,
        /* [out] */ Double* value);

    CARAPI IsNull(
        /* [in] */ Int32 column,
        /* [out] */ Boolean* value);

    CARAPI GetBlob(
        /* [in] */ Int32 column,
        /* [out] */ ArrayOf<Byte>** blob);

    CARAPI GetColumnNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI Deactivate();

    CARAPI Close();

    CARAPI RegisterContentObserver(
        /* [in] */ IContentObserver* observer);

    CARAPI UnregisterContentObserver(
        /* [in] */ IContentObserver* observer);

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI Requery(
        /* [out] */ Boolean* value);

private:
    CARAPI_(void) Init();

public:
    class SubDataSetObserver : public DataSetObserver
    {
    public:
        SubDataSetObserver();

        ~SubDataSetObserver();

        CARAPI OnChanged();

        CARAPI OnInvalidated();
    };

private:
    SubDataSetObserver* mObserver;

    AutoPtr<ICursor> mCursor;

    ArrayOf<AutoPtr<ICursor> >* mCursors;
};
#endif // __MERGECURSOR_H__
