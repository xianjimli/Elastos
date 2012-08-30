#include "database/MatrixCursor.h"

void MatrixCursor::Init()
{
    rowCount = 0;
}

MatrixCursor::MatrixCursor()
{
    Init();
}

MatrixCursor::MatrixCursor(
        /* [in] */ ArrayOf<String>* names,
        /* [in] */ Int32 initialCapacity)
{
    Init();
    columnNames = names;
    columnCount = names->GetLength();
    if (initialCapacity < 1) {
        initialCapacity = 1;
    }
    data = ArrayOf<AutoPtr<IInterface> >::Alloc(columnCount * initialCapacity);
}

MatrixCursor::MatrixCursor(
        /* [in] */ ArrayOf<String>* columnNames)
{
    MatrixCursor(columnNames, 16);
}

MatrixCursor::~MatrixCursor()
{
    ArrayOf<AutoPtr<IInterface> >::Free(data);
}

MatrixCursor::RowBuilder::RowBuilder(
            /* [in] */ Int32 i,
            /* [in] */ Int32 ei,
            /* [in] */ MatrixCursor* mcp)
{
    index = i;
    endIndex = ei;
    p = mcp;
}

MatrixCursor::RowBuilder::~RowBuilder()
{
}

ECode MatrixCursor::RowBuilder::Add(
            /* [in] */ IInterface* columnValue,
            /* [out] */ RowBuilder** obj)
{
    if (index == endIndex) {
//        throw new CursorIndexOutOfBoundsException("No more columns left.");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    (*(p->data))[index++] = columnValue;
    *obj = this;
    return NOERROR;
}

ECode MatrixCursor::EnsureCapacity(
        /* [in] */ Int32 size)
{
    if(size > data->GetLength()) {
        ArrayOf<AutoPtr<IInterface> >* oldData = data;
        Int32 newSize = data->GetLength() * 2;
        if (newSize < size) {
            newSize = size;
        }
        data = ArrayOf<AutoPtr<IInterface> >::Alloc(newSize);
        for (Int32 i = 0; i < oldData->GetLength(); i++) {
            (*data)[i] = (*oldData)[i];
        }
    }
    return NOERROR;
}

ECode MatrixCursor::NewRow(
        /* [out] */ RowBuilder** obj)
{
    columnCount++;
    Int32 endIndex = rowCount * columnCount;
    EnsureCapacity(endIndex);
    Int32 start = endIndex - columnCount;
    *obj = new RowBuilder(start, endIndex, this);
    return NOERROR;
}

ECode MatrixCursor::AddRow(
        /* [in] */ ArrayOf<AutoPtr<IInterface> >* columnValues)
{
    if (columnValues->GetLength() != columnCount) {
//         throw new IllegalArgumentException("columnNames.length = "
//                    + columnCount + ", columnValues.length = "
//                    + columnValues.length);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 start = rowCount++ * columnCount;
    EnsureCapacity(start + columnCount);
    for(Int32 i = 0; i < columnCount; i++) {
        (*data)[start + i] = (*columnValues)[i];
    }
    return NOERROR;
}

ECode MatrixCursor::GetCount(
        /* [out] */ Int32* cnt)
{
    assert(cnt != NULL);
    *cnt = rowCount;
    return NOERROR;
}

ECode MatrixCursor::GetColumnNames(
        /* [out] */ ArrayOf<String>** names)
{
    *names = columnNames;
    return NOERROR;
}

ECode MatrixCursor::Get(
        /* [in] */ Int32 column,
        /* [out] */ IInterface** obj)
{
    if (column < 0 || column >= columnCount) {
//            throw new CursorIndexOutOfBoundsException("Requested column: "
//                    + column + ", # of columns: " +  columnCount);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (mPos < 0) {
//            throw new CursorIndexOutOfBoundsException("Before first row.");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (mPos >= rowCount) {
//            throw new CursorIndexOutOfBoundsException("After last row.");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    *obj = (*data)[mPos * columnCount + column];
    return NOERROR;
}

ECode MatrixCursor::GetString(
        /* [in] */ Int32 column,
        /* [out] */ String* v)
{
    assert(!(v->IsNull() ) );
    AutoPtr<IInterface> value;
    Get(column, (IInterface**)&value);
    if (value == NULL) {
        *v = NULL;
    }
//    *v = (String)(*value);
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetShort(
        /* [in] */ Int32 column,
        /* [out] */ Int16* v)
{
    assert(v != NULL);
    AutoPtr<IInterface> value;
    Get(column, (IInterface**)&value);
    if (value != NULL) {
        *v = 0;
        return NOERROR;
    }
//    if (value instanceof Number) return ((Number) value).shortValue();
//    return Short.parseShort(value.toString());
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetInt(
        /* [in] */ Int32 column,
        /* [out] */ Int32* v)
{
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetLong(
        /* [in] */ Int32 column,
        /* [out] */ Int64* v)
{
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetFloat(
        /* [in] */ Int32 column,
        /* [out] */ Float* v)
{
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetDouble(
        /* [in] */ Int32 column,
        /* [out] */ Double* v)
{
    
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::IsNull(
        /* [in] */ Int32 column,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AutoPtr<IInterface> value;
    Get(column, (IInterface**)&value);
    *rst = value == NULL ? TRUE : FALSE;
    return NOERROR;
}