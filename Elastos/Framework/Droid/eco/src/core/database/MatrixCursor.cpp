
#include "database/MatrixCursor.h"


MatrixCursor::RowBuilder::RowBuilder(
    /* [in] */ Int32 index,
    /* [in] */ Int32 endIndex,
    /* [in] */ MatrixCursor* owner)
    : mIndex(index)
    , mEndIndex(endIndex)
    , mOwner(owner)
{}

PInterface MatrixCursor::RowBuilder::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IRowBuilder*)this;
    }
    else if (riid == EIID_IRowBuilder) {
        return (IRowBuilder*)this;
    }

    return NULL;
}

UInt32 MatrixCursor::RowBuilder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MatrixCursor::RowBuilder::Release()
{
    return ElRefBase::Release();
}

ECode MatrixCursor::RowBuilder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRowBuilder*)this) {
        *pIID = EIID_IRowBuilder;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MatrixCursor::RowBuilder::Add(
    /* [in] */ IInterface* columnValue)
{
    if (mIndex == mEndIndex) {
//        throw new CursorIndexOutOfBoundsException("No more columns left.");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    assert(mOwner->mData != NULL);
    (*(mOwner->mData))[mIndex++] = columnValue;
    return NOERROR;
}


MatrixCursor::MatrixCursor()
    : mColumnNames(NULL)
    , mData(NULL)
    , mRowCount(0)
{}

MatrixCursor::~MatrixCursor()
{
    if (mColumnNames != NULL) {
        for (Int32 i = 0; i < mColumnNames->GetLength(); ++i) {
            (*mColumnNames)[i] = NULL;
        }
        ArrayOf<String>::Free(mColumnNames);
    }

    if (mData != NULL) {
        for (Int32 i = 0; i < mData->GetLength(); ++i) {
            (*mData)[i] = NULL;
        }
        ArrayOf< AutoPtr<IInterface> >::Free(mData);
    }
}

ECode MatrixCursor::Init(
    /* [in] */ ArrayOf<String>* columnNames,
    /* [in] */ Int32 initialCapacity)
{
    assert(columnNames != NULL);

    mColumnNames = columnNames;
    mColumnCount = columnNames->GetLength();

    if (initialCapacity < 1) {
        initialCapacity = 1;
    }

    mData = ArrayOf< AutoPtr<IInterface> >::Alloc(mColumnCount * initialCapacity);
    return NOERROR;
}

ECode MatrixCursor::Init(
    /* [in] */ ArrayOf<String>* columnNames)
{
    return Init(columnNames, 16);
}

ECode MatrixCursor::Get(
    /* [in] */ Int32 column,
    /* [out] */ IInterface** obj)
{
    if (column < 0 || column >= mColumnCount) {
//            throw new CursorIndexOutOfBoundsException("Requested column: "
//                    + column + ", # of columns: " +  columnCount);
        return E_CURSOR_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (mPos < 0) {
//            throw new CursorIndexOutOfBoundsException("Before first row.");
        return E_CURSOR_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (mPos >= mRowCount) {
//            throw new CursorIndexOutOfBoundsException("After last row.");
        return E_CURSOR_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    *obj = (*mData)[mPos * mColumnCount + column];
    if (*obj != NULL) (*obj)->AddRef();
    return NOERROR;
}

AutoPtr<IRowBuilder> MatrixCursor::NewRow()
{
    mColumnCount++;
    Int32 endIndex = mRowCount * mColumnCount;
    EnsureCapacity(endIndex);
    Int32 start = endIndex - mColumnCount;
    AutoPtr<IRowBuilder> builder = new RowBuilder(start, endIndex, this);
    return builder;
}

ECode MatrixCursor::AddRow(
    /* [in] */ const ArrayOf<IInterface*>& columnValues)
{
    if (columnValues.GetLength() != mColumnCount) {
//         throw new IllegalArgumentException("columnNames.length = "
//                    + columnCount + ", columnValues.length = "
//                    + columnValues.length);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 start = mRowCount++ * mColumnCount;
    EnsureCapacity(start + mColumnCount);
    for(Int32 i = 0; i < mColumnCount; i++) {
        (*mData)[start + i] = columnValues[i];
    }
    return NOERROR;
}

ECode MatrixCursor::AddRow(
    /* [in] */ IObjectContainer* columnValues)
{
    // int start = rowCount * columnCount;
    // int end = start + columnCount;
    // ensureCapacity(end);

    // if (columnValues instanceof ArrayList<?>) {
    //     addRow((ArrayList<?>) columnValues, start);
    //     return;
    // }

    // int current = start;
    // Object[] localData = data;
    // for (Object columnValue : columnValues) {
    //     if (current == end) {
    //         // TODO: null out row?
    //         throw new IllegalArgumentException(
    //                 "columnValues.size() > columnNames.length");
    //     }
    //     localData[current++] = columnValue;
    // }

    // if (current != end) {
    //     // TODO: null out row?
    //     throw new IllegalArgumentException(
    //             "columnValues.size() < columnNames.length");
    // }

    // // Increase row count here in case we encounter an exception.
    // rowCount++;

    return E_NOT_IMPLEMENTED;
}

void MatrixCursor::EnsureCapacity(
    /* [in] */ Int32 size)
{
    if (size > mData->GetLength()) {
        ArrayOf< AutoPtr<IInterface> >* oldData = mData;
        Int32 newSize = mData->GetLength() * 2;
        if (newSize < size) {
            newSize = size;
        }
        mData = ArrayOf< AutoPtr<IInterface> >::Alloc(newSize);
        memcpy(mData->GetPayload(), oldData->GetPayload(),
                oldData->GetLength() * sizeof(AutoPtr<IInterface>));
        ArrayOf< AutoPtr<IInterface> >::Free(oldData);
    }
}

Int32 MatrixCursor::GetCount()
{
    return mRowCount;
}

ECode MatrixCursor::GetColumnNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    *names = NULL;

    if (mColumnNames != NULL) {
        Int32 N = mColumnNames->GetLength();
        *names = ArrayOf<String>::Alloc(N);
        for (Int32 i = 0; i < N; ++i) {
            (**names)[i] = (*mColumnNames)[i];
        }
    }
    return NOERROR;
}

ECode MatrixCursor::GetString(
    /* [in] */ Int32 column,
    /* [out] */ String* str)
{
    // Object value = get(column);
    // if (value == null) return null;
    // return value.toString();
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetInt16(
    /* [in] */ Int32 column,
    /* [out] */ Int16* value)
{
    // Object value = get(column);
    // if (value == null) return 0;
    // if (value instanceof Number) return ((Number) value).shortValue();
    // return Short.parseShort(value.toString());
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetInt32(
    /* [in] */ Int32 column,
    /* [out] */ Int32* value)
{
    // Object value = get(column);
    // if (value == null) return 0;
    // if (value instanceof Number) return ((Number) value).intValue();
    // return Integer.parseInt(value.toString());
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetInt64(
    /* [in] */ Int32 column,
    /* [out] */ Int64* value)
{
    // Object value = get(column);
    // if (value == null) return 0;
    // if (value instanceof Number) return ((Number) value).longValue();
    // return Long.parseLong(value.toString());
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetFloat(
    /* [in] */ Int32 column,
    /* [out] */ Float* value)
{
    // Object value = get(column);
    // if (value == null) return 0.0f;
    // if (value instanceof Number) return ((Number) value).floatValue();
    // return Float.parseFloat(value.toString());
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::GetDouble(
    /* [in] */ Int32 column,
    /* [out] */ Double* value)
{
    // Object value = get(column);
    // if (value == null) return 0.0d;
    // if (value instanceof Number) return ((Number) value).doubleValue();
    // return Double.parseDouble(value.toString());
    return E_NOT_IMPLEMENTED;
}

ECode MatrixCursor::IsNull(
    /* [in] */ Int32 column,
    /* [out] */ Boolean* isNull)
{
    // return get(column) == null;
    return E_NOT_IMPLEMENTED;
}
