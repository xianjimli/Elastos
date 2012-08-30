#ifndef __MATRIXCURSOR_H__
#define __MATRIXCURSOR_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "database/AbstractCursor.h"
class MatrixCursor : public AbstractCursor
{
public:
    class RowBuilder
    {
    public:
        RowBuilder(
            /* [in] */ Int32 index,
            /* [in] */ Int32 endIndex,
            /* [in] */ MatrixCursor* mcp);

        ~RowBuilder();

        CARAPI Add(
            /* [in] */ IInterface* columnValue,
            /* [out] */ RowBuilder** obj);
    private:
        Int32 index;
        Int32 endIndex;
        MatrixCursor* p;

    };
public:
    MatrixCursor();

    MatrixCursor(
        /* [in] */ ArrayOf<String>* columnNames,
        /* [in] */ Int32 initialCapacity);

    MatrixCursor(
        /* [in] */ ArrayOf<String>* columnNames);

    ~MatrixCursor();

    virtual CARAPI NewRow(
        /* [out] */ RowBuilder** obj);

    virtual CARAPI AddRow(
        /* [in] */ ArrayOf<AutoPtr<IInterface> >* columnValues);

//    public void addRow(Iterable<?> columnValues)

    CARAPI GetCount(
        /* [out] */ Int32* cnt);

    CARAPI GetColumnNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetString(
        /* [in] */ Int32 column,
        /* [out] */ String* v);

    CARAPI GetShort(
        /* [in] */ Int32 column,
        /* [out] */ Int16* v);

    CARAPI GetInt(
        /* [in] */ Int32 column,
        /* [out] */ Int32* v);

    CARAPI GetLong(
        /* [in] */ Int32 column,
        /* [out] */ Int64* v);

    CARAPI GetFloat(
        /* [in] */ Int32 column,
        /* [out] */ Float* v);

    CARAPI GetDouble(
        /* [in] */ Int32 column,
        /* [out] */ Double* v);

    CARAPI IsNull(
        /* [in] */ Int32 column,
        /* [out] */ Boolean* rst);
private:
    CARAPI_(void) Init();

    CARAPI Get(
        /* [in] */ Int32 column,
        /* [out] */ IInterface** obj);

    CARAPI EnsureCapacity(
        /* [in] */ Int32 size);

//    private void addRow(ArrayList<?> columnValues, int start)
private:
    ArrayOf<String>* columnNames;
    ArrayOf<AutoPtr<IInterface> >* data;
    Int32 rowCount;
    Int32 columnCount;
};
#endif //__MATRIXCURSOR_H__