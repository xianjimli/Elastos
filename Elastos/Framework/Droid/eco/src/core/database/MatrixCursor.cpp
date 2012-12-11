#include "database/MatrixCursor.h"

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

void MatrixCursor::Init()
{
    rowCount = 0;
}

MatrixCursor::MatrixCursor()
{
    Init();
}

MatrixCursor::~MatrixCursor()
{
    ArrayOf<IInterface*>::Free(data);
}

ECode MatrixCursor::Init(
        /* [in] */ const ArrayOf<String> & names,
        /* [in] */ Int32 initialCapacity)
{
    Init();
    columnNames = const_cast<ArrayOf<String>*>(&names);
    columnCount = names.GetLength();
    if (initialCapacity < 1) {
        initialCapacity = 1;
    }
    data = ArrayOf<IInterface*>::Alloc(columnCount * initialCapacity);
    return NOERROR;
}

ECode MatrixCursor::Init(
        /* [in] */ const ArrayOf<String> & columnNames)
{
    Init(columnNames, 16);
    return NOERROR;
}

ECode MatrixCursor::NewRow(
        /* [out] */ IRowBuilder** obj)
{
    columnCount++;
    Int32 endIndex = rowCount * columnCount;
    EnsureCapacity(endIndex);
    Int32 start = endIndex - columnCount;
    //*obj = new RowBuilder(start, endIndex, this);
    return NOERROR;
}

ECode MatrixCursor::AddRow(
        /* [in] */ const ArrayOf<IInterface*> & columnValues)
{
    if (columnValues.GetLength() != columnCount) {
//         throw new IllegalArgumentException("columnNames.length = "
//                    + columnCount + ", columnValues.length = "
//                    + columnValues.length);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 start = rowCount++ * columnCount;
    EnsureCapacity(start + columnCount);
    for(Int32 i = 0; i < columnCount; i++) {
        (*data)[start + i] = columnValues[i];
    }
    return NOERROR;
}

ECode MatrixCursor::AddRowEx(
        /* [in] */ IObjectContainer* columnValues)
{
    Int32 start = rowCount * columnCount;
    Int32 end = start + columnCount;
    EnsureCapacity(end);

    /*if (columnValues instanceof ArrayList<?>) {
        addRow((ArrayList<?>) columnValues, start);
        return;
    }*/

    Int32 current = start;
    ArrayOf<IInterface*>* localData = data;
    Int32 length;
    columnValues->GetObjectCount(&length);
    for (Int32 i = 0; i < length; i++) {
        if (current == end) {
            // TODO: null out row?
            //throw new IllegalArgumentException(
            //        "columnValues.size() > columnNames.length");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        //(*localData)[current++] = (*columnValues)[i];
    }

    if (current != end) {
        // TODO: null out row?
        //throw new IllegalArgumentException(
        //        "columnValues.size() < columnNames.length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Increase row count here in case we encounter an exception.
    rowCount++;

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
        /* [out, callee] */ ArrayOf<String>** names)
{

    *names = columnNames;
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
    v = (String*)(IInterface*)value;
    return NOERROR;
}

ECode MatrixCursor::GetInt16(
        /* [in] */ Int32 column,
        /* [out] */ Int16* v)
{
    assert(v != NULL);
    AutoPtr<IInterface> value;
    Get(column, (IInterface**)&value);
    if (value != NULL) {
        *v = (Int16)0;
        return NOERROR;
    }
    v = (Int16*)(IInterface*)value;
    return NOERROR;
}

ECode MatrixCursor::GetInt32(
        /* [in] */ Int32 column,
        /* [out] */ Int32* v)
{
    assert(v != NULL);
    AutoPtr<IInterface> value;
    Get(column, (IInterface**)&value);
    if (value == NULL) {
        *v = 0;
        return NOERROR;
    }
    v = (Int32*)(IInterface*)value;
    return NOERROR;
}

ECode MatrixCursor::GetInt64(
        /* [in] */ Int32 column,
        /* [out] */ Int64* v)
{
    assert(v != NULL);
    AutoPtr<IInterface> value;
    Get(column, (IInterface**)&value);
    if (value == NULL) {
        *v = 0L;
        return NOERROR;
    }
    v = (Int64*)(IInterface*)value;
    return NOERROR;
}

ECode MatrixCursor::GetFloat(
        /* [in] */ Int32 column,
        /* [out] */ Float* v)
{
    assert(v != NULL);
    AutoPtr<IInterface> value;
    Get(column, (IInterface**)&value);
    if (value == NULL) {
        *v = (Float)0.0;
        return NOERROR;
    }
    v = (Float*)(IInterface*)value;
    return NOERROR;
}

ECode MatrixCursor::GetDouble(
        /* [in] */ Int32 column,
        /* [out] */ Double* v)
{
    assert(v != NULL);
    AutoPtr<IInterface> value;
    Get(column, (IInterface**)&value);
    if (value == NULL) {
        *v = (Double)0.0;
        return NOERROR;
    }
    v = (Double*)(IInterface*)value;
    return NOERROR;
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

ECode MatrixCursor::EnsureCapacity(
        /* [in] */ Int32 size)
{
    if(size > data->GetLength()) {
        ArrayOf<IInterface*>* oldData = data;
        Int32 newSize = data->GetLength() * 2;
        if (newSize < size) {
            newSize = size;
        }
        data = ArrayOf<IInterface*>::Alloc(newSize);
        for (Int32 i = 0; i < oldData->GetLength(); i++) {
            (*data)[i] = (*oldData)[i];
        }
    }
    return NOERROR;
}

ECode MatrixCursor::AddRow(
        /* [in] */ Set<IInterface*>* columnValues,
        /* [in] */ Int32 start)
{
    Int32 size = columnValues->GetSize();
    if (size != columnCount) {
        //throw new IllegalArgumentException("columnNames.length = "
        //        + columnCount + ", columnValues.size() = " + size);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    rowCount++;
    ArrayOf<IInterface*>* localData = data;
    Set<IInterface*>::Iterator iter = columnValues->Begin();
    for (Int32 i = 0; i < size; i++) {
        (*localData)[start + i] = *iter;
        iter++;
    }
    return NOERROR;
}