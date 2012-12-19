#ifndef __MERGECURSOR_H__
#define __MERGECURSOR_H__

#include "ext/frameworkext.h"
#include "database/AbstractCursor.h"
#include "database/DataSetObserver.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * A convience class that lets you present an array of Cursors as a single linear Cursor.
 * The schema of the cursors presented is entirely up to the creator of the MergeCursor, and
 * may be different if that is desired. Calls to getColumns, getColumnIndex, etc will return the
 * value for the row that the MergeCursor is currently pointing at.
 */
class MergeCursor : public AbstractCursor
{
private:
    class MyDataSetObserver
        : public ElRefBase
        , public IDataSetObserver
        , public DataSetObserver
    {
    public:
        MyDataSetObserver();

        ~MyDataSetObserver();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChanged();

        CARAPI OnInvalidated();
    };

public:
    virtual ~MergeCursor();

    //@Override
    CARAPI_(Int32) GetCount();

    //@Override
    CARAPI_(Boolean) OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition);

    /**
     * @hide
     * @deprecated
     */
    //@Override
    CARAPI DeleteRow(
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    //@Override
    CARAPI CommitUpdates(
        /* [out] */ Boolean* succeeded);

    //@Override
    CARAPI GetString(
        /* [in] */ Int32 column,
        /* [out] */ String* str);

    //@Override
    CARAPI GetInt16(
        /* [in] */ Int32 column,
        /* [out] */ Int16* value);

    //@Override
    CARAPI GetInt32(
        /* [in] */ Int32 column,
        /* [out] */ Int32* value);

    //@Override
    CARAPI GetInt64(
        /* [in] */ Int32 column,
        /* [out] */ Int64* value);

    //@Override
    CARAPI GetFloat(
        /* [in] */ Int32 column,
        /* [out] */ Float* value);

    //@Override
    CARAPI GetDouble(
        /* [in] */ Int32 column,
        /* [out] */ Double* value);

    //@Override
    CARAPI IsNull(
        /* [in] */ Int32 column,
        /* [out] */ Boolean* isNull);

    //@Override
    CARAPI GetBlob(
        /* [in] */ Int32 column,
        /* [out, callee] */ ArrayOf<Byte>** blob);

    //@Override
    CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String>** names);

    //@Override
    CARAPI Deactivate();

    //@Override
    CARAPI Close();

    //@Override
    CARAPI RegisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    //@Override
    CARAPI UnregisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    //@Override
    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    //@Override
    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    //@Override
    CARAPI Requery(
        /* [out] */ Boolean* result);

protected:
    MergeCursor();

    CARAPI Init(
        /* [in] */ ArrayOf<ICursor*>* cursors);

private:
    AutoPtr<MyDataSetObserver> mObserver;

    AutoPtr<ICursor> mCursor; //updated in onMove
    ArrayOf< AutoPtr<ICursor> >* mCursors;
};
#endif // __MERGECURSOR_H__
