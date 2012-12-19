
#include "database/CursorWindow.h"
#include "database/sqlite/Sqlite3Exception.h"

CursorWindow::CursorWindow()
    : mNativeWindow(NULL)
{}

CursorWindow::~CursorWindow()
{
    // Just in case someone forgot to call close...
    NativeClose();
}

CARAPI CursorWindow::Init(
    /* [in] */ Boolean localWindow)
{
    mStartPos = 0;
    return NativeInit(localWindow);
}

Int32 CursorWindow::GetStartPosition()
{
    return mStartPos;
}

ECode CursorWindow::SetStartPosition(
    /* [in] */ Int32 pos)
{
    mStartPos = pos;
    return NOERROR;
}

Int32 CursorWindow::GetNumRows()
{
    AcquireReference();
    //try {
    Int32 num = NativeGetNumRows();
    //} finally {
    ReleaseReference();
    //}
    return num;
}

Int32 CursorWindow::NativeGetNumRows()
{
    return mNativeWindow->getNumRows();
}

Boolean CursorWindow::SetNumColumns(
    /* [in] */ Int32 columnNum)
{
    AcquireReference();
    //try {
    Boolean result = NativeSetNumColumns(columnNum);
    //} finally {
    ReleaseReference();
    //}
    return result;
}

Boolean CursorWindow::NativeSetNumColumns(
    /* [in] */ Int32 columnNum)
{
    return mNativeWindow->setNumColumns(columnNum);
}

Boolean CursorWindow::AllocRow()
{
    AcquireReference();
    //try {
    Boolean result = NativeAllocRow();
    //} finally {
    ReleaseReference();
    //}
    return result;
}

Boolean CursorWindow::NativeAllocRow()
{
    return mNativeWindow->allocRow() != NULL;
}

ECode CursorWindow::FreeLastRow()
{
    AcquireReference();
    //try {
    NativeFreeLastRow();
    //} finally {
    ReleaseReference();
    //}
    return NOERROR;
}

void CursorWindow::NativeFreeLastRow()
{
    mNativeWindow->freeLastRow();
}

Boolean CursorWindow::PutBlob(
    /* [in] */ const ArrayOf<Byte> & value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    AcquireReference();
    //try {
    Boolean result = NativePutBlob(value, row - mStartPos, col);
    //} finally {
    ReleaseReference();
    //}
    return result;
}

Boolean CursorWindow::NativePutBlob(
    /* [in] */ const ArrayOf<Byte>& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    NativeCursorWindow* window = mNativeWindow;
    field_slot_t* fieldSlot = window->getFieldSlotWithCheck(row, col);
    if (fieldSlot == NULL) {
        // LOG_WINDOW(" getFieldSlotWithCheck error ");
        return FALSE;
    }

    Int32 len = value.GetLength();
    Int32 offset = window->alloc(len);
    if (!offset) {
        // LOG_WINDOW("Failed allocating %u bytes", len);
        return FALSE;
    }
    Byte* bytes = value.GetPayload();
    window->copyIn(offset, (uint8_t const *)bytes, len);

    // This must be updated after the call to alloc(), since that
    // may move the field around in the window
    fieldSlot->type = FIELD_TYPE_BLOB;
    fieldSlot->data.buffer.offset = offset;
    fieldSlot->data.buffer.size = len;
    // LOG_WINDOW("%d,%d is BLOB with %u bytes @ %d", row, col, len, offset);
    return TRUE;
}

Boolean CursorWindow::PutString(
    /* [in] */ const String& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    AcquireReference();
    //try {
    Boolean result = NativePutString(value, row - mStartPos, col);
    //} finally {
    ReleaseReference();
    //}
    return result;
}

Boolean CursorWindow::NativePutString(
    /* [in] */ const String& value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    NativeCursorWindow * window = mNativeWindow;
    if (value.IsNull()) {
        // LOG_WINDOW("How did a null value send to here");
        return FALSE;
    }
    field_slot_t * fieldSlot = window->getFieldSlotWithCheck(row, col);
    if (fieldSlot == NULL) {
        // LOG_WINDOW(" getFieldSlotWithCheck error ");
        return FALSE;
    }

    Int32 len = value.GetLength() + 1;
    char const * valStr = (const char*)value;
    if (!valStr) {
        // LOG_WINDOW("value can't be transfer to UTFChars");
        return FALSE;
    }

    Int32 offset = window->alloc(len);
    if (!offset) {
        // LOG_WINDOW("Failed allocating %u bytes", len);
        return FALSE;
    }

    window->copyIn(offset, (uint8_t const *)valStr, len);

    // This must be updated after the call to alloc(), since that
    // may move the field around in the window
    fieldSlot->type = FIELD_TYPE_STRING;
    fieldSlot->data.buffer.offset = offset;
    fieldSlot->data.buffer.size = len;

    // LOG_WINDOW("%d,%d is TEXT with %u bytes @ %d", row, col, len, offset);

    return TRUE;
}

Boolean CursorWindow::PutInt64(
    /* [in] */ Int64 value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    AcquireReference();
    //try {
    Boolean result = NativePutInt64(value, row - mStartPos, col);
    //} finally {
    ReleaseReference();
    //}
    return result;
}

Boolean CursorWindow::NativePutInt64(
    /* [in] */ Int64 value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    if (!mNativeWindow->putInt64(row, col, value)) {
        // LOG_WINDOW(" getFieldSlotWithCheck error ");
        return FALSE;
    }

    // LOG_WINDOW("%d,%d is INTEGER 0x%016llx", row, col, value);

    return TRUE;
}

Boolean CursorWindow::PutDouble(
    /* [in] */ Double value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    AcquireReference();
    //try {
    Boolean result = NativePutDouble(value, row - mStartPos, col);
    //} finally {
    ReleaseReference();
    //}
    return result;
}

Boolean CursorWindow::NativePutDouble(
    /* [in] */ Double value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    if (!mNativeWindow->putDouble(row, col, value)) {
        // LOG_WINDOW(" getFieldSlotWithCheck error ");
        return FALSE;
    }

    // LOG_WINDOW("%d,%d is FLOAT %lf", row, col, value);

    return TRUE;
}

Boolean CursorWindow::PutNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    AcquireReference();
    //try {
    Boolean result = NativePutNull(row - mStartPos, col);
    //} finally {
    ReleaseReference();
    //}
    return result;
}

Boolean CursorWindow::NativePutNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    if (!mNativeWindow->putNull(row, col)) {
        // LOG_WINDOW(" getFieldSlotWithCheck error ");
        return FALSE;
    }

    // LOG_WINDOW("%d,%d is NULL", row, col);

    return TRUE;
}

Boolean CursorWindow::IsNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col)
{
    AcquireReference();
    //try {
    Boolean isNull = NativeIsNull(row - mStartPos, col);
    //} finally {
    ReleaseReference();
    //}
    return isNull;
}

Boolean CursorWindow::NativeIsNull(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column)
{
    // LOG_WINDOW("Checking for NULL at %d,%d from %p", row, column, window);

    Boolean isNull;
    if (mNativeWindow->getNull(row, column, &isNull)) {
        return isNull;
    }

    //TODO throw execption?
    return TRUE;
}

ECode CursorWindow::GetBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ ArrayOf<Byte>** blob)
{
    AcquireReference();
    //try {
    ECode ec = NativeGetBlob(row - mStartPos, col, blob);
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::NativeGetBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ ArrayOf<Byte>** blob)
{
    ECode ec = NOERROR;
    int32_t err;
    NativeCursorWindow * window = mNativeWindow;
    // LOG_WINDOW("Getting blob for %d,%d from %p", row, column, window);

    field_slot_t field;
    err = window->read_field_slot(row, column, &field);
    if (err != 0) {
        *blob = NULL;
        // throwExceptionWithRowCol(env, row, column);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    uint8_t type = field.type;
    if (type == FIELD_TYPE_BLOB || type == FIELD_TYPE_STRING) {
        ArrayOf<Byte>* byteArray = ArrayOf<Byte>::Alloc(field.data.buffer.size);
        // LOG_ASSERT(byteArray, "Native could not create new byte[]");
        memcpy(byteArray->GetPayload(), window->offsetToPtr(field.data.buffer.offset),
                field.data.buffer.size);
        *blob = byteArray;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_INTEGER) {
        ec = throw_sqlite3_exception(NULL);
    }
    else if (type == FIELD_TYPE_FLOAT) {
        ec = throw_sqlite3_exception(NULL);
    }
    else if (type == FIELD_TYPE_NULL) {
        // do nothing
    }
    else {
        // throwUnknowTypeException(env, type);
        ec = E_ILLEGAL_STATE_EXCEPTION;
    }
    *blob = NULL;
    return ec;
}

ECode CursorWindow::IsBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isBlob)
{
    AcquireReference();
    //try {
    ECode ec = NativeIsBlob(row - mStartPos, col, isBlob);
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::IsInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isInt64)
{
    AcquireReference();
    //try {
    ECode ec = NativeIsInteger(row - mStartPos, col, isInt64);
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::IsFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isFloat)
{
    AcquireReference();
    //try {
    ECode ec = NativeIsFloat(row - mStartPos, col, isFloat);
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::IsString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* isString)
{
    AcquireReference();
    //try {
    ECode ec = NativeIsString(row - mStartPos, col, isString);
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::NativeIsBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ Boolean* isBlob)
{
    int32_t err;
    // LOG_WINDOW("Checking if column is a blob or null for %d,%d from %p", row, column, window);

    field_slot_t field;
    err = mNativeWindow->read_field_slot(row, column, &field);
    if (err != 0) {
        // throwExceptionWithRowCol(env, row, column);
        *isBlob = FALSE;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *isBlob = field.type == FIELD_TYPE_BLOB || field.type == FIELD_TYPE_NULL;
    return NOERROR;
}

ECode CursorWindow::NativeIsString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ Boolean* isString)
{
    int32_t err;
    // LOG_WINDOW("Checking if column is a string or null for %d,%d from %p", row, column, window);

    field_slot_t field;
    err = mNativeWindow->read_field_slot(row, column, &field);
    if (err != 0) {
        // throwExceptionWithRowCol(env, row, column);
        *isString = FALSE;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *isString = field.type == FIELD_TYPE_STRING || field.type == FIELD_TYPE_NULL;
    return NOERROR;
}

ECode CursorWindow::NativeIsInteger(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ Boolean* isInteger)
{
    int32_t err;
    // LOG_WINDOW("Checking if column is an integer for %d,%d from %p", row, column, window);

    field_slot_t field;
    err = mNativeWindow->read_field_slot(row, column, &field);
    if (err != 0) {
        // throwExceptionWithRowCol(env, row, column);
        *isInteger = FALSE;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *isInteger = field.type == FIELD_TYPE_INTEGER;
    return NOERROR;
}

ECode CursorWindow::NativeIsFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ Boolean* isFloat)
{
    int32_t err;
    // LOG_WINDOW("Checking if column is a float for %d,%d from %p", row, column, window);

    field_slot_t field;
    err = mNativeWindow->read_field_slot(row, column, &field);
    if (err != 0) {
        // throwExceptionWithRowCol(env, row, column);
        *isFloat = FALSE;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *isFloat = field.type == FIELD_TYPE_FLOAT;
    return NOERROR;
}

ECode CursorWindow::GetString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ String* str)
{
    AcquireReference();
    //try {
    ECode ec = NativeGetString(row - mStartPos, col, str);
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::NativeGetString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ String* str)
{
    int32_t err;
    NativeCursorWindow * window = mNativeWindow;
    // LOG_WINDOW("Getting string for %d,%d from %p", row, column, window);

    field_slot_t field;
    err = window->read_field_slot(row, column, &field);
    if (err != 0) {
        // throwExceptionWithRowCol(env, row, column);
        *str = NULL;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    uint8_t type = field.type;
    if (type == FIELD_TYPE_STRING) {
        uint32_t size = field.data.buffer.size;
        if (size > 0) {
            // Pass size - 1 since the UTF8 is null terminated and we don't want a null terminator on the UTF16 string
            *str = (char const *)window->offsetToPtr(field.data.buffer.offset);
            return NOERROR;
        }
        else {
            *str = "";
            return NOERROR;
        }
    }
    else if (type == FIELD_TYPE_INTEGER) {
        int64_t value;
        if (window->getInt64(row, column, &value)) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%lld", value);
            *str = buf;
            return NOERROR;
        }
        *str = NULL;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_FLOAT) {
        Double value;
        if (window->getDouble(row, column, &value)) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%g", value);
            *str = buf;
            return NOERROR;
        }
        *str = NULL;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_NULL) {
        *str = NULL;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_BLOB) {
        // throw_sqlite3_exception(env, "Unable to convert BLOB to string");
        *str = NULL;
        return throw_sqlite3_exception(NULL);
    }
    else {
        // throwUnknowTypeException(env, type);
        *str = NULL;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

ECode CursorWindow::CopyStringToBuffer(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ ICharArrayBuffer* buffer)
{
    if (buffer == NULL) {
        //throw new IllegalArgumentException("CharArrayBuffer should not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // if (buffer.data == null) {
    //         buffer.data = new char[64];
    //     }
    //     acquireReference();
    //     try {
    //         char[] newbuf = copyStringToBuffer_native(
    //                 row - mStartPos, col, buffer.data.length, buffer);
    //         if (newbuf != null) {
    //             buffer.data = newbuf;
    //         }
    //     } finally {
    //         releaseReference();
    //     }

    return E_NOT_IMPLEMENTED;
}

CARAPI_(ArrayOf<Char32>*) NativeCopyStringToBuffer(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ Int32 bufferSize,
    /* [in] */ ICharArrayBuffer* buffer)
{
//     int32_t err;
//     CursorWindow * window = GET_WINDOW(env, object);
// LOG_WINDOW("Copying string for %d,%d from %p", row, column, window);

//     field_slot_t field;
//     err = window->read_field_slot(row, column, &field);
//     if (err != 0) {
//         jniThrowException(env, "java/lang/IllegalStateException", "Unable to get field slot");
//         return NULL;
//     }

//     jcharArray buffer = (jcharArray)env->GetObjectField(buf, gBufferField);
//     if (buffer == NULL) {
//         jniThrowException(env, "java/lang/IllegalStateException", "buf should not be null");
//         return NULL;
//     }
//     jchar* dst = env->GetCharArrayElements(buffer, NULL);
//     uint8_t type = field.type;
//     uint32_t sizeCopied = 0;
//     jcharArray newArray = NULL;
//     if (type == FIELD_TYPE_STRING) {
//         uint32_t size = field.data.buffer.size;
//         if (size > 0) {
// #if WINDOW_STORAGE_UTF8
//             // Pass size - 1 since the UTF8 is null terminated and we don't want a null terminator on the UTF16 string
//             String16 utf16((char const *)window->offsetToPtr(field.data.buffer.offset), size - 1);
//             int32_t strSize = utf16.size();
//             if (strSize > bufferSize || dst == NULL) {
//                 newArray = env->NewCharArray(strSize);
//                 env->SetCharArrayRegion(newArray, 0, strSize, (jchar const *)utf16.string());
//             } else {
//                 memcpy(dst, (jchar const *)utf16.string(), strSize * 2);
//             }
//             sizeCopied = strSize;
// #else
//             sizeCopied = size/2 + size % 2;
//             if (size > bufferSize * 2 || dst == NULL) {
//                 newArray = env->NewCharArray(sizeCopied);
//                 memcpy(newArray, (jchar const *)window->offsetToPtr(field.data.buffer.offset), size);
//             } else {
//                 memcpy(dst, (jchar const *)window->offsetToPtr(field.data.buffer.offset), size);
//             }
// #endif
//         }
//     } else if (type == FIELD_TYPE_INTEGER) {
//         int64_t value;
//         if (window->getLong(row, column, &value)) {
//             char buf[32];
//             int len;
//             snprintf(buf, sizeof(buf), "%lld", value);
//             jchar* dst = env->GetCharArrayElements(buffer, NULL);
//             sizeCopied = charToJchar(buf, dst, bufferSize);
//          }
//     } else if (type == FIELD_TYPE_FLOAT) {
//         double value;
//         if (window->getDouble(row, column, &value)) {
//             char tempbuf[32];
//             snprintf(tempbuf, sizeof(tempbuf), "%g", value);
//             jchar* dst = env->GetCharArrayElements(buffer, NULL);
//             sizeCopied = charToJchar(tempbuf, dst, bufferSize);
//         }
//     } else if (type == FIELD_TYPE_NULL) {
//     } else if (type == FIELD_TYPE_BLOB) {
//         throw_sqlite3_exception(env, "Unable to convert BLOB to string");
//     } else {
//         LOGE("Unknown field type %d", type);
//         throw_sqlite3_exception(env, "UNKNOWN type in copyStringToBuffer_native()");
//     }
//     SET_SIZE_COPIED(env, buf, sizeCopied);
//     env->ReleaseCharArrayElements(buffer, dst, JNI_OK);
//     return newArray;
    return NULL;
}

ECode CursorWindow::GetInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int64* value)
{
    AcquireReference();
    //try {
    ECode ec = NativeGetInt64(row - mStartPos, col, value);
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::NativeGetInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ Int64* _value)
{
    int32_t err;
    NativeCursorWindow * window = mNativeWindow;
    // LOG_WINDOW("Getting long for %d,%d from %p", row, column, window);

    field_slot_t field;
    err = window->read_field_slot(row, column, &field);
    if (err != 0) {
        // throwExceptionWithRowCol(env, row, column);
        *_value = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    uint8_t type = field.type;
    if (type == FIELD_TYPE_INTEGER) {
        int64_t value;
        if (window->getInt64(row, column, &value)) {
            *_value = value;
            return NOERROR;
        }
        *_value = 0;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_STRING) {
        uint32_t size = field.data.buffer.size;
        if (size > 0) {
            *_value = strtoll((char const *)window->offsetToPtr(field.data.buffer.offset), NULL, 0);
            return NOERROR;
        }
        else {
            *_value = 0;
            return NOERROR;
        }
    }
    else if (type == FIELD_TYPE_FLOAT) {
        Double value;
        if (window->getDouble(row, column, &value)) {
            *_value = value;
            return NOERROR;
        }
        *_value = 0;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_NULL) {
        *_value = 0;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_BLOB) {
        // throw_sqlite3_exception(env, "Unable to convert BLOB to long");
        *_value = 0;
        return throw_sqlite3_exception(NULL);
    }
    else {
        // throwUnknowTypeException(env, type);
        *_value = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

ECode CursorWindow::GetDouble(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Double* value)
{
    AcquireReference();
    //try {
    ECode ec = NativeGetDouble(row - mStartPos, col, value);
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::NativeGetDouble(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [out] */ Double* _value)
{
    int32_t err;
    NativeCursorWindow * window = mNativeWindow;
    // LOG_WINDOW("Getting double for %d,%d from %p", row, column, window);

    field_slot_t field;
    err = window->read_field_slot(row, column, &field);
    if (err != 0) {
        // throwExceptionWithRowCol(env, row, column);
        *_value = 0.0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    uint8_t type = field.type;
    if (type == FIELD_TYPE_FLOAT) {
        Double value;
        if (window->getDouble(row, column, &value)) {
            *_value = value;
            return NOERROR;
        }
        *_value = 0.0;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_STRING) {
        uint32_t size = field.data.buffer.size;
        if (size > 0) {
            *_value = strtod((char const *)window->offsetToPtr(field.data.buffer.offset), NULL);
            return NOERROR;
        }
        else {
            *_value = 0.0;
            return NOERROR;
        }
    }
    else if (type == FIELD_TYPE_INTEGER) {
        int64_t value;
        if (window->getInt64(row, column, &value)) {
            *_value = (Double)value;
            return NOERROR;
        }
        *_value = 0.0;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_NULL) {
        *_value = 0.0;
        return NOERROR;
    }
    else if (type == FIELD_TYPE_BLOB) {
        // throw_sqlite3_exception(env, "Unable to convert BLOB to double");
        *_value = 0.0;
        return throw_sqlite3_exception(NULL);
    }
    else {
        // throwUnknowTypeException(env, type);
        *_value = 0.0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

ECode CursorWindow::GetInt16(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int16* value)
{
    AcquireReference();
    //try {
    Int64 i64Value;
    ECode ec = NativeGetInt64(row - mStartPos, col, &i64Value);
    *value = (Int16)i64Value;
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::GetInt32(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int32* value)
{
    AcquireReference();
    //try {
    Int64 i64Value;
    ECode ec = NativeGetInt64(row - mStartPos, col, &i64Value);
    *value = (Int32)i64Value;
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::GetFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Float* value)
{
    AcquireReference();
    //try {
    Double dValue;
    ECode ec = NativeGetDouble(row - mStartPos, col, &dValue);
    *value = dValue;
    //} finally {
    ReleaseReference();
    //}
    return ec;
}

ECode CursorWindow::Clear()
{
    AcquireReference();
    //try {
    mStartPos = 0;
    NativeClear();
    //} finally {
    ReleaseReference();
    //}
    return NOERROR;
}

void CursorWindow::NativeClear()
{
    // LOG_WINDOW("Clearing window %p", window);
    assert(mNativeWindow != NULL);
    mNativeWindow->clear();
}

ECode CursorWindow::Close()
{
    ReleaseReference();
    return NOERROR;
}

void CursorWindow::NativeClose()
{
    if (mNativeWindow) {
        // LOG_WINDOW("Closing window %p", window);
        delete mNativeWindow;
        mNativeWindow = NULL;
    }
}

ECode CursorWindow::NativeInit(
    /* [in] */ Boolean localOnly)
{
    uint8_t* data;
    size_t size;
    NativeCursorWindow* window;

    window = new NativeCursorWindow(MAX_WINDOW_SIZE);
    if (!window) {
        // jniThrowException(env, "java/lang/RuntimeException", "No memory for native window object");
        return E_RUNTIME_EXCEPTION;
    }

    if (!window->initBuffer(localOnly)) {
        // jniThrowException(env, "java/lang/IllegalStateException", "Couldn't init cursor window");
        delete window;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

// LOG_WINDOW("native_init_empty: window = %p", window);
    mNativeWindow = window;
    return NOERROR;
}


ECode CursorWindow::OnAllReferencesReleased()
{
    NativeClose();
    return NOERROR;
}
