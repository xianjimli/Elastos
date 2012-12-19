
#include "database/sqlite/SQLiteQuery.h"
#include "database/sqlite/Sqlite3Exception.h"
#include "database/NativeCursorWindow.h"

const CString SQLiteQuery::TAG = "Cursor";


SQLiteQuery::SQLiteQuery(
        /* [in] */ SQLiteDatabase* db,
        /* [in] */ const String& query,
        /* [in] */ Int32 offsetIndex,
        /* [in] */ ArrayOf<String>* bindArgs)
    : SQLiteProgram(db, query)
    , mOffsetIndex(offsetIndex)
    , mBindArgs(bindArgs)
    , mClosed(FALSE)
{}

SQLiteQuery::~SQLiteQuery()
{
    if (mBindArgs != NULL) {
        //todo:
        // free mBindArgs
    }
}

ECode SQLiteQuery::FillWindow(
        /* [in] */ ICursorWindow* window,
        /* [in] */ Int32 maxRead,
        /* [in] */ Int32 lastPos,
        /* [out] */ Int32* rowNum)
{
    // Int64 timeStart = SystemClock::GetUptimeMillis();
    mDatabase->Lock();
    // mDatabase.logTimeStat(mSql, timeStart, SQLiteDatabase.GET_LOCK_LOG_PREFIX);
//    try {
        AcquireReference();
//        try {
        window->AcquireReference();
        // if the start pos is not equal to 0, then most likely window is
        // too small for the data set, loading by another thread
        // is not safe in this situation. the native code will ignore maxRead
        Int32 winPos;
        window->GetStartPosition(&winPos);
        ECode ec = NativeFillWindow(window, winPos, mOffsetIndex,
                maxRead, lastPos, rowNum);

        // // Logging
        // if (SQLiteDebug.DEBUG_SQL_STATEMENTS) {
        //     Log.d(TAG, "fillWindow(): " + mSql);
        // }
        // mDatabase.logTimeStat(mSql, timeStart);
        // } catch (IllegalStateException e){
        //     // simply ignore it
        //     return 0;
        // } catch (SQLiteDatabaseCorruptException e) {
        //     mDatabase.onCorruption();
        //     throw e;
        // } finally {
        //     window.releaseReference();
        // }
        if (ec == (ECode)E_ILLEGAL_STATE_EXCEPTION) {
            *rowNum = 0;
            ec = NOERROR;
        }
        else if (ec == E_SQLITE_DATABASE_CORRUPT_EXCEPTION) {
            mDatabase->OnCorruption();
        }
        window->ReleaseReference();
    // } finally {
        ReleaseReference();
        mDatabase->Unlock();
    // }

    return ec;
}

Int32 SQLiteQuery::ColumnCountLocked()
{
    AcquireReference();
//    try {
    Int32 count = NativeColumnCount();
//    } finally {
    ReleaseReference();
//    }
    return count;
}

String SQLiteQuery::ColumnNameLocked(
        /* [in] */ Int32 columnIndex)
{
    AcquireReference();
//    try {
    String name = NativeColumnName(columnIndex);
//    } finally {
    ReleaseReference();
//    }
    return name;
}

ECode SQLiteQuery::ToString(
        /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = String("SQLiteQuery") + mSql;
    return NOERROR;
}

ECode SQLiteQuery::Close()
{
    ECode ec = SQLiteProgram::Close();
    mClosed = TRUE;
    return ec;
}

ECode SQLiteQuery::Requery()
{
    if (mBindArgs != NULL) {
        Int32 len = mBindArgs->GetLength();
//        try {
        for (Int32 i = 0; i < len; i++) {
            ECode ec = SQLiteProgram::BindString(i + 1, (*mBindArgs)[i]);
            if (ec == E_SQLITE_MISUSE_EXCEPTION) {
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
//            } catch (SQLiteMisuseException e) {
//                StringBuilder errMsg = new StringBuilder("mSql " + mSql);
//                for (int i = 0; i < len; i++) {
//                    errMsg.append(" ");
//                    errMsg.append(mBindArgs[i]);
//                }
//                errMsg.append(" ");
//                IllegalStateException leakProgram = new IllegalStateException(
//                        errMsg.toString(), e);
//                throw leakProgram;
//            }
//         }
    }
    return NOERROR;
}

ECode SQLiteQuery::BindNull(
        /* [in] */ Int32 index)
{
    (*mBindArgs)[index - 1] = NULL;
    if (!mClosed) {
        SQLiteProgram::BindNull(index);
    }
    return NOERROR;
}

ECode SQLiteQuery::BindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value)
{
    (*mBindArgs)[index - 1] = String::FromInt64(value);
    if (!mClosed) {
        SQLiteProgram::BindInt64(index, value);
    }
    return NOERROR;
}

ECode SQLiteQuery::BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value)
{
    (*mBindArgs)[index - 1] = String::FromDouble(value);
    if (!mClosed) {
        SQLiteProgram::BindDouble(index, value);
    }
    return NOERROR;
}

ECode SQLiteQuery::BindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value)
{
    (*mBindArgs)[index - 1] = value;
    if (!mClosed) {
        SQLiteProgram::BindString(index, value);
    }
    return NOERROR;
}

static Int32 skip_rows(sqlite3_stmt *statement, Int32 maxRows)
{
    Int32 retryCount = 0;
    for (Int32 i = 0; i < maxRows; i++) {
        Int32 err = sqlite3_step(statement);
        if (err == SQLITE_ROW){
            // do nothing
        }
        else if (err == SQLITE_DONE) {
            return i;
        }
        else if (err == SQLITE_LOCKED || err == SQLITE_BUSY) {
            // The table is locked, retry
            LOG_WINDOW("Database locked, retrying");
           if (retryCount > 50) {
                LOGE("Bailing on database busy rety");
                break;
            }
            // Sleep to give the thread holding the lock a chance to finish
            usleep(1000);
            retryCount++;
            continue;
        }
        else {
            return -1;
        }
    }
    LOGD("skip_rows row %d", maxRows);
    return maxRows;
}

static Int32 finish_program_and_get_row_count(sqlite3_stmt *statement)
{
    Int32 numRows = 0;
    Int32 retryCount = 0;
    while (TRUE) {
        Int32 err = sqlite3_step(statement);
        if (err == SQLITE_ROW){
            numRows++;
        }
        else if (err == SQLITE_LOCKED || err == SQLITE_BUSY) {
            // The table is locked, retry
            LOG_WINDOW("Database locked, retrying");
            if (retryCount > 50) {
                LOGE("Bailing on database busy rety");
                break;
            }
            // Sleep to give the thread holding the lock a chance to finish
            usleep(1000);
            retryCount++;
            continue;
        }
        else {
            // no need to throw exception
            break;
        }
    }
    sqlite3_reset(statement);
    LOGD("finish_program_and_get_row_count row %d", numRows);
    return numRows;
}

ECode SQLiteQuery::NativeFillWindow(
    /* [in] */ ICursorWindow* _window,
    /* [in] */ Int32 startPos,
    /* [in] */ Int32 offsetParam,
    /* [in] */ Int32 maxRead,
    /* [in] */ Int32 lastPos,
    /* [out] */ Int32* rowNum)
{
    ECode ec;
    Int32 err;
    sqlite3_stmt* statement = mNativeStatement;
    Int32 numRows = lastPos;
    maxRead += lastPos;
    Int32 numColumns;
    Int32 retryCount;
    Int32 boundParams;
    NativeCursorWindow* window;

    if (statement == NULL) {
        LOGE("Invalid statement in fillWindow()");
        // jniThrowException(env, "java/lang/IllegalStateException",
        //                   "Attempting to access a deactivated, closed, or empty cursor");
        *rowNum = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // Only do the binding if there is a valid offsetParam. If no binding needs to be done
    // offsetParam will be set to 0, an invliad value.
    if(offsetParam > 0) {
        // Bind the offset parameter, telling the program which row to start with
        err = sqlite3_bind_int(statement, offsetParam, startPos);
        if (err != SQLITE_OK) {
            LOGE("Unable to bind offset position, offsetParam = %d", offsetParam);
            // jniThrowException(env, "java/lang/IllegalArgumentException",
            //                   sqlite3_errmsg(GET_HANDLE(env, object)));
            *rowNum = 0;
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        LOG_WINDOW("Bound to startPos %d", startPos);
    }
    else {
        LOG_WINDOW("Not binding to startPos %d", startPos);
    }

    // Get the native window
//    window = ((CCursorWindow*)_window)->mNativeWindow;
    if (!window) {
        LOGE("Invalid CursorWindow");
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //                   "Bad CursorWindow");
        *rowNum = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    LOG_WINDOW("Window: numRows = %d, size = %d, freeSpace = %d", window->getNumRows(), window->size(), window->freeSpace());

    numColumns = sqlite3_column_count(statement);
    if (!window->setNumColumns(numColumns)) {
        LOGE("Failed to change column count from %d to %d", window->getNumColumns(), numColumns);
        // jniThrowException(env, "java/lang/IllegalStateException", "numColumns mismatch");
        *rowNum = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    retryCount = 0;
    if (startPos > 0) {
        Int32 num = skip_rows(statement, startPos);
        if (num < 0) {
            *rowNum = 0;
            return throw_sqlite3_exception(mNativeHandle);
        }
        else if (num < startPos) {
            LOGE("startPos %d > actual rows %d", startPos, num);
            *rowNum = num;
            return NOERROR;
        }
    }

    while(startPos != 0 || numRows < maxRead) {
        err = sqlite3_step(statement);
        if (err == SQLITE_ROW) {
            LOG_WINDOW("\nStepped statement %p to row %d", statement, startPos + numRows);
            retryCount = 0;

            // Allocate a new field directory for the row. This pointer is not reused
            // since it mey be possible for it to be relocated on a call to alloc() when
            // the field data is being allocated.
            {
                field_slot_t * fieldDir = window->allocRow();
                if (!fieldDir) {
                    LOGE("Failed allocating fieldDir at startPos %d row %d", startPos, numRows);
                    *rowNum = startPos + numRows + finish_program_and_get_row_count(statement) + 1;
                    return NOERROR;
                }
            }

            // Pack the row into the window
            Int32 i;
            for (i = 0; i < numColumns; i++) {
                Int32 type = sqlite3_column_type(statement, i);
                if (type == SQLITE_TEXT) {
                    // TEXT data
#if WINDOW_STORAGE_UTF8
                    uint8_t const * text = (uint8_t const *)sqlite3_column_text(statement, i);
                    // SQLite does not include the NULL terminator in size, but does
                    // ensure all strings are NULL terminated, so increase size by
                    // one to make sure we store the terminator.
                    size_t size = sqlite3_column_bytes(statement, i) + 1;
#else
                    uint8_t const * text = (uint8_t const *)sqlite3_column_text16(statement, i);
                    size_t size = sqlite3_column_bytes16(statement, i);
#endif
                    int offset = window->alloc(size);
                    if (!offset) {
                        window->freeLastRow();
                        LOGE("Failed allocating %u bytes for text/blob at %d,%d", size,
                                   startPos + numRows, i);
                        *rowNum = startPos + numRows + finish_program_and_get_row_count(statement) + 1;
                        return NOERROR;
                    }

                    window->copyIn(offset, text, size);

                    // This must be updated after the call to alloc(), since that
                    // may move the field around in the window
                    field_slot_t * fieldSlot = window->getFieldSlot(numRows, i);
                    fieldSlot->type = FIELD_TYPE_STRING;
                    fieldSlot->data.buffer.offset = offset;
                    fieldSlot->data.buffer.size = size;

                    LOG_WINDOW("%d,%d is TEXT with %u bytes", startPos + numRows, i, size);
                }
                else if (type == SQLITE_INTEGER) {
                    // INTEGER data
                    int64_t value = sqlite3_column_int64(statement, i);
                    if (!window->putInt64(numRows, i, value)) {
                        window->freeLastRow();
                        LOGE("Failed allocating space for a long in column %d", i);
                        *rowNum = startPos + numRows + finish_program_and_get_row_count(statement) + 1;
                        return NOERROR;
                    }
                    LOG_WINDOW("%d,%d is INTEGER 0x%016llx", startPos + numRows, i, value);
                }
                else if (type == SQLITE_FLOAT) {
                    // FLOAT data
                    double value = sqlite3_column_double(statement, i);
                    if (!window->putDouble(numRows, i, value)) {
                        window->freeLastRow();
                        LOGE("Failed allocating space for a double in column %d", i);
                        *rowNum = startPos + numRows + finish_program_and_get_row_count(statement) + 1;
                        return NOERROR;
                    }
                    LOG_WINDOW("%d,%d is FLOAT %lf", startPos + numRows, i, value);
                }
                else if (type == SQLITE_BLOB) {
                    // BLOB data
                    uint8_t const * blob = (uint8_t const *)sqlite3_column_blob(statement, i);
                    size_t size = sqlite3_column_bytes16(statement, i);
                    int offset = window->alloc(size);
                    if (!offset) {
                        window->freeLastRow();
                        LOGE("Failed allocating %u bytes for blob at %d,%d", size,
                                   startPos + numRows, i);
                        *rowNum = startPos + numRows + finish_program_and_get_row_count(statement) + 1;
                        return NOERROR;
                    }

                    window->copyIn(offset, blob, size);

                    // This must be updated after the call to alloc(), since that
                    // may move the field around in the window
                    field_slot_t * fieldSlot = window->getFieldSlot(numRows, i);
                    fieldSlot->type = FIELD_TYPE_BLOB;
                    fieldSlot->data.buffer.offset = offset;
                    fieldSlot->data.buffer.size = size;

                    LOG_WINDOW("%d,%d is Blob with %u bytes @ %d", startPos + numRows, i, size, offset);
                }
                else if (type == SQLITE_NULL) {
                    // NULL field
                    window->putNull(numRows, i);

                    LOG_WINDOW("%d,%d is NULL", startPos + numRows, i);
                }
                else {
                    // Unknown data
                    LOGE("Unknown column type when filling database window");
                    ec = throw_sqlite3_exception(NULL);
                    break;
                }
            }

            if (i < numColumns) {
                // Not all the fields fit in the window
                // Unknown data error happened
                break;
            }

            // Mark the row as complete in the window
            numRows++;
        }
        else if (err == SQLITE_DONE) {
            // All rows processed, bail
            LOG_WINDOW("Processed all rows");
            break;
        }
        else if (err == SQLITE_LOCKED || err == SQLITE_BUSY) {
            // The table is locked, retry
            LOG_WINDOW("Database locked, retrying");
            if (retryCount > 50) {
                LOGE("Bailing on database busy rety");
                break;
            }

            // Sleep to give the thread holding the lock a chance to finish
            usleep(1000);

            retryCount++;
            continue;
        }
        else {
            ec = throw_sqlite3_exception(mNativeHandle);
            break;
        }
    }

    LOG_WINDOW("Resetting statement %p after fetching %d rows in %d bytes\n\n\n\n", statement,
            numRows, window->size() - window->freeSpace());
//    LOGI("Filled window with %d rows in %d bytes", numRows, window->size() - window->freeSpace());
    if (err == SQLITE_ROW) {
        *rowNum = -1;
    }
    else {
        sqlite3_reset(statement);
        *rowNum = startPos + numRows;
    }
    return ec;
}

Int32 SQLiteQuery::NativeColumnCount()
{
    return sqlite3_column_count(mNativeStatement);
}

String SQLiteQuery::NativeColumnName(
    /* [in] */ Int32 columnIndex)
{
    const char* name = sqlite3_column_name(mNativeStatement, columnIndex);
    return String(name);
}