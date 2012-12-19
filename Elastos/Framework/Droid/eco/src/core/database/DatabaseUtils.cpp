
#include "ext/frameworkdef.h"
#include "database/DatabaseUtils.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;


DatabaseUtils::InsertHelper::InsertHelper(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ const String& tableName)
    : mDb(db)
    , mTableName(tableName)
    , mColumns(NULL)
{}

DatabaseUtils::InsertHelper::~InsertHelper()
{
    if (mColumns != NULL) {
        mColumns->Clear();
        delete mColumns;
    }
}

ECode DatabaseUtils::InsertHelper::BuildSQL()
{
    StringBuffer sb(128);
    sb += "INSERT INTO ";
    sb += mTableName;
    sb += " (";

    StringBuffer sbv(128);
    sbv += "VALUES (";

    Int32 i = 1;
    AutoPtr<ICursor> cur;
    //try {
        StringBuffer sql(128);
        sql = "PRAGMA table_info(";
        sql += mTableName;
        sql += ")";
        mDb->RawQuery(String(sql), NULL, (ICursor**)&cur);
        Int32 count;
        cur->GetCount(&count);
        mColumns = new HashMap<String, Int32>(count);
        Boolean moveToNext;
        while (cur->MoveToNext(&moveToNext), moveToNext) {
            String columnName, defaultValue;
            cur->GetString(TABLE_INFO_PRAGMA_COLUMNNAME_INDEX, &columnName);
            cur->GetString(TABLE_INFO_PRAGMA_DEFAULT_INDEX, &defaultValue);

            (*mColumns)[columnName] = i;
            sb += "'";
            sb += columnName;
            sb += "'";

            if (defaultValue.IsNull()) {
                sbv += "?";
            }
            else {
                sbv += "COALESCE(?, ";
                sbv += defaultValue;
                sbv += ")";
            }

            sb += (i == count ? ") " : ", ");
            sbv += (i == count ? ");" : ", ");
            ++i;
        }
    //} finally {
    if (cur != NULL) {
        cur->Close();
    }
    //}

    sb += sbv;

    mInsertSQL = (const char*)sb;
    if (LOCAL_LOGV) {
        StringBuffer log;
        log = "insert statement is ";
        log += mInsertSQL;
        Logger::V(TAG, (const char*)log);
    }
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::GetStatement(
    /* [in] */ Boolean allowReplace,
    /* [out] */ ISQLiteStatement** statement)
{
    if (allowReplace) {
        if (mReplaceStatement == NULL) {
            if (mInsertSQL == NULL) {
                BuildSQL();
            }
            // chop "INSERT" off the front and prepend "INSERT OR REPLACE" instead.
            String replaceSQL = String("INSERT OR REPLACE") + mInsertSQL.Substring(6);
            mDb->CompileStatement(replaceSQL, (ISQLiteStatement**)&mReplaceStatement);
        }
        *statement = mReplaceStatement;
        (*statement)->AddRef();
        return NOERROR;
    }
    else {
        if (mInsertStatement == NULL) {
            if (mInsertSQL == NULL) {
                BuildSQL();
            }
            mDb->CompileStatement(mInsertSQL, (ISQLiteStatement**)&mInsertStatement);
        }
        *statement = mInsertStatement;
        (*statement)->AddRef();
        return NOERROR;
    }
}

Int64 DatabaseUtils::InsertHelper::InsertInternal(
    /* [in] */ IContentValues* values,
    /* [in] */ Boolean allowReplace)
{
    Mutex::Autolock lock(mLock);

    //try {
    AutoPtr<ISQLiteStatement> stmt;
    GetStatement(allowReplace, (ISQLiteStatement**)&stmt);
    stmt->ClearBindings();
    if (LOCAL_LOGV) {
        Logger::V(TAG, StringBuffer("--- inserting in table ") + mTableName);
    }
    /*for (Map.Entry<String, Object> e: values.valueSet()) {
        final String key = e.getKey();
        int i = getColumnIndex(key);
        DatabaseUtils.bindObjectToProgram(stmt, i, e.getValue());
        if (LOCAL_LOGV) {
            Log.v(TAG, "binding " + e.getValue() + " to column " +
                  i + " (" + key + ")");
        }
    }
    return stmt.executeInsert();*/
    return -1;
    //} catch (SQLException e) {
        //Log.e(TAG, "Error inserting " + values + " into table  " + mTableName, e);
        //return -1;
    //}
}

ECode DatabaseUtils::InsertHelper::GetColumnIndex(
    /* [in] */ const String& key,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex);

    AutoPtr<ISQLiteStatement> stmt;
    GetStatement(FALSE, (ISQLiteStatement**)&stmt);
    HashMap<String, Int32 >::Iterator it = mColumns->Find(key);
    if (it == mColumns->End()) {
        //throw new IllegalArgumentException("column '" + key + "' is invalid");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *columnIndex = it->mSecond;
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Bind(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return mPreparedStatement->BindDouble(index, value);
}

ECode DatabaseUtils::InsertHelper::Bind(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return mPreparedStatement->BindDouble(index, value);
}

ECode DatabaseUtils::InsertHelper::Bind(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return mPreparedStatement->BindInt64(index, value);
}

ECode DatabaseUtils::InsertHelper::Bind(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return mPreparedStatement->BindInt64(index, value);
}

ECode DatabaseUtils::InsertHelper::Bind(
    /* [in] */ Int32 index,
    /* [in] */ Boolean value)
{
    return mPreparedStatement->BindInt64(index, value ? 1 : 0);
}

ECode DatabaseUtils::InsertHelper::BindNull(
    /* [in] */ Int32 index)
{
    return mPreparedStatement->BindNull(index);
}

ECode DatabaseUtils::InsertHelper::Bind(
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Byte>* value)
{
    if (value == NULL) {
        return mPreparedStatement->BindNull(index);
    }
    else {
        return mPreparedStatement->BindBlob(index, *value);
    }
}

ECode DatabaseUtils::InsertHelper::Bind(
    /* [in] */ Int32 index,
    /* [in] */ const String& value)
{
    if (value.IsNull()) {
        return mPreparedStatement->BindNull(index);
    }
    else {
        return mPreparedStatement->BindString(index, value);
    }
}

ECode DatabaseUtils::InsertHelper::Insert(
    /* [in] */ IContentValues* values,
    /* [out] */ Int64* rowId)
{
    VALIDATE_NOT_NULL(rowId);

    *rowId = InsertInternal(values, FALSE);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Execute(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    if (mPreparedStatement == NULL) {
        //throw new IllegalStateException("you must prepare this inserter before calling "
               // + "execute");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //try {
    if (LOCAL_LOGV) {
        Logger::V(TAG, StringBuffer("--- doing insert or replace in table ") + mTableName);
    }
    return mPreparedStatement->ExecuteInsert(value);
    //} catch (SQLException e) {
        //Log.e(TAG, "Error executing InsertHelper with table " + mTableName, e);
        //return -1;
    //} finally {
    // you can only call this once per prepare
    mPreparedStatement = NULL;
    //}
}

ECode DatabaseUtils::InsertHelper::PrepareForInsert()
{
    FAIL_RETURN(GetStatement(FALSE, (ISQLiteStatement**)&mPreparedStatement));
    return mPreparedStatement->ClearBindings();
}

ECode DatabaseUtils::InsertHelper::PrepareForReplace()
{
    FAIL_RETURN(GetStatement(TRUE, (ISQLiteStatement**)&mPreparedStatement));
    return mPreparedStatement->ClearBindings();
}

ECode DatabaseUtils::InsertHelper::Replace(
    /* [in] */ IContentValues* values,
    /* [out] */ Int64* rowId)
{
    VALIDATE_NOT_NULL(rowId);

    *rowId = InsertInternal(values, TRUE);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Close()
{
    if (mInsertStatement != NULL) {
        mInsertStatement->Close();
        mInsertStatement = NULL;
    }
    if (mReplaceStatement != NULL) {
        mReplaceStatement->Close();
        mReplaceStatement = NULL;
    }
    mInsertSQL = NULL;
    mColumns = NULL;
    return NOERROR;
}


const CString DatabaseUtils::TAG = "DatabaseUtils";

const ArrayOf<CString>* DatabaseUtils::sCountProjection;// = new String[]{"count(*)"};

// const Char8 DatabaseUtils::DIGITS[16] = {
//     '0','1','2','3','4','5','6','7',
//     '8','9','A','B','C','D','E','F' };


void DatabaseUtils::WriteExceptionToParcel(
    /* [in] */ IParcel* reply,
    /* [in] */ ECode e)
{
    // int code = 0;
    // boolean logException = true;
    // if (e instanceof FileNotFoundException) {
    //     code = 1;
    //     logException = false;
    // } else if (e instanceof IllegalArgumentException) {
    //     code = 2;
    // } else if (e instanceof UnsupportedOperationException) {
    //     code = 3;
    // } else if (e instanceof SQLiteAbortException) {
    //     code = 4;
    // } else if (e instanceof SQLiteConstraintException) {
    //     code = 5;
    // } else if (e instanceof SQLiteDatabaseCorruptException) {
    //     code = 6;
    // } else if (e instanceof SQLiteFullException) {
    //     code = 7;
    // } else if (e instanceof SQLiteDiskIOException) {
    //     code = 8;
    // } else if (e instanceof SQLiteException) {
    //     code = 9;
    // } else if (e instanceof OperationApplicationException) {
    //     code = 10;
    // } else {
    //     reply.writeException(e);
    //     Log.e(TAG, "Writing exception to parcel", e);
    //     return;
    // }
    // reply.writeInt(code);
    // reply.writeString(e.getMessage());

    // if (logException) {
    //     Log.e(TAG, "Writing exception to parcel", e);
    // }
}

ECode DatabaseUtils::ReadExceptionFromParcel(
    /* [in] */ IParcel* reply)
{
    // int code = reply.readExceptionCode();
    // if (code == 0) return;
    // String msg = reply.readString();
    // DatabaseUtils.readExceptionFromParcel(reply, msg, code);
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::ReadExceptionWithFileNotFoundExceptionFromParcel(
    /* [in] */ IParcel* reply)
{
    // int code = reply.readExceptionCode();
    // if (code == 0) return;
    // String msg = reply.readString();
    // if (code == 1) {
    //     throw new FileNotFoundException(msg);
    // } else {
    //     DatabaseUtils.readExceptionFromParcel(reply, msg, code);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::ReadExceptionWithOperationApplicationExceptionFromParcel(
    /* [in] */ IParcel* reply)
{
    // int code = reply.readExceptionCode();
    // if (code == 0) return;
    // String msg = reply.readString();
    // if (code == 10) {
    //     throw new OperationApplicationException(msg);
    // } else {
    //     DatabaseUtils.readExceptionFromParcel(reply, msg, code);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::ReadExceptionFromParcel(
    /* [in] */ IParcel* reply,
    /* [in] */ const String& msg,
    /* [in] */ Int32 code)
{
    switch (code) {
        case 2:
            //throw new IllegalArgumentException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case 3:
            //throw new UnsupportedOperationException(msg);
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        case 4:
            //throw new SQLiteAbortException(msg);
            return E_SQLITE_ABORT_EXCEPTION;
        case 5:
            //throw new SQLiteConstraintException(msg);
            return E_SQLITE_CONSTRAINT_EXCEPTION;
        case 6:
            //throw new SQLiteDatabaseCorruptException(msg);
            return E_SQLITE_DATABASE_CORRUPT_EXCEPTION;
        case 7:
            //throw new SQLiteFullException(msg);
            return E_SQLITE_FULL_EXCEPTION;
        case 8:
            //throw new SQLiteDiskIOException(msg);
            return E_SQLITE_DISK_IO_EXCEPTION;
        case 9:
            //throw new SQLiteException(msg);
            return E_SQLITE_EXCEPTION;
        default:
            //reply->readException(code, msg);
            return E_NOT_IMPLEMENTED;
    }
}

ECode DatabaseUtils::BindObjectToProgram(
    /* [in] */ ISQLiteProgram* prog,
    /* [in] */ Int32 index,
    /* [in] */ IInterface* value)
{
    /*if (value == NULL) {
        prog->BindNull(index);
    } else if (value instanceof Double || value instanceof Float) {
        prog.bindDouble(index, ((Number)value).doubleValue());
    } else if (value instanceof Number) {
        prog.bindLong(index, ((Number)value).longValue());
    } else if (value instanceof Boolean) {
        Boolean bool = (Boolean)value;
        if (bool) {
            prog.bindLong(index, 1);
        } else {
            prog.bindLong(index, 0);
        }
    } else if (value instanceof byte[]){
        prog.bindBlob(index, (byte[]) value);
    } else {
        prog.bindString(index, value.toString());
    }*/
    return E_NOT_IMPLEMENTED;
}

void DatabaseUtils::AppendEscapedSQLString(
    /* [in] */ StringBuffer& sb,
    /* [in] */ const String& sqlString)
{
    sb += '\'';
    if (sqlString.IndexOf('\'') != -1) {
        Int32 length = sqlString.GetLength();
        for (Int32 i = 0; i < length; i++) {
            Char32 c = sqlString.GetChar(i);
            if (c == '\'') {
                sb += '\'';
            }
            sb += c;
        }
    } else {
        sb += sqlString;
    }
    sb += '\'';
}

String DatabaseUtils::SqlEscapeString(
    /* [in] */ const String& value)
{
    StringBuffer escaper;

    AppendEscapedSQLString(escaper, value);

    return String(escaper);
}

const void DatabaseUtils::AppendValueToSql(
    /* [in] */ StringBuffer& sql,
    /* [in] */ IInterface* value)
{
    if (value == NULL) {
        sql += "NULL";
    }/* else if (value instanceof Boolean) {
        Boolean bool = (Boolean)value;
        if (bool) {
            *sql += '1';
        } else {
            *sql += '0';
        }
    } else {
        AppendEscapedSQLString(sql, value.toString());
    }*/
}

String DatabaseUtils::ConcatenateWhere(
    /* [in] */ const String& a,
    /* [in] */ const String& b)
{
    if (a.IsNullOrEmpty()) {
        return b;
    }
    if (b.IsNullOrEmpty()) {
        return a;
    }

    StringBuffer sb;
    sb += "(" + a + ") AND (" + b + ")";
    return String(sb);
}

String DatabaseUtils::GetCollationKey(
    /* [in] */ const String& name)
{
    // byte [] arr = getCollationKeyInBytes(name);
    // try {
    //     return new String(arr, 0, getKeyLen(arr), "ISO8859_1");
    // } catch (Exception ex) {
    //     return "";
    // }
    return String(NULL);
}

String DatabaseUtils::GetHexCollationKey(
    /* [in] */ const String& name)
{
    // byte [] arr = getCollationKeyInBytes(name);
    // char[] keys = Hex.encodeHex(arr);
    // return new String(keys, 0, getKeyLen(arr) * 2);
    return String(NULL);
}

Int32 DatabaseUtils::GetKeyLen(
    /* [in] */ const ArrayOf<Byte>& arr)
{
    Int32 size = arr.GetLength();
    if (arr[size - 1] != 0) {
        return size;
    }
    else {
        // remove zero "termination"
        return size - 1;
    }
}

ArrayOf<Byte>* DatabaseUtils::GetCollationKeyInBytes(
    /* [in] */ const String& name)
{
    // if (mColl == null) {
    //     mColl = Collator.getInstance();
    //     mColl.setStrength(Collator.PRIMARY);
    // }
    // return mColl.getCollationKey(name).toByteArray();
    return NULL;
}

void DatabaseUtils::DumpCursor(
    /* [in] */ ICursor* cursor)
{
    //DumpCursor(cursor, System.out);
}

// public static void dumpCursor(Cursor cursor, PrintStream stream) {
//     stream.println(">>>>> Dumping cursor " + cursor);
//     if (cursor != null) {
//         int startPos = cursor.getPosition();

//         cursor.moveToPosition(-1);
//         while (cursor.moveToNext()) {
//             dumpCurrentRow(cursor, stream);
//         }
//         cursor.moveToPosition(startPos);
//     }
//     stream.println("<<<<<");
// }

void DatabaseUtils::DumpCursor(
    /* [in] */ ICursor* cursor,
    /* [in] */ StringBuffer& sb)
{
    // *sb += ">>>>> Dumping cursor ";
    // //*sb += (*cursor);
    // *sb += "\n";
    // if (cursor != NULL) {
    //     Int32 startPos;
    //     cursor->GetPosition(&startPos);

    //     Boolean value1, value2;
    //     cursor->MoveToPosition(-1, &value1);
    //     cursor->MoveToNext(&value2);
    //     while (value2) {
    //         DumpCurrentRow(cursor, sb);
    //         cursor->MoveToNext(&value2);
    //     }
    //     cursor->MoveToPosition(startPos, &value1);
    // }
    // *sb += "<<<<<\n";
}

String DatabaseUtils::DumpCursorToString(
    /* [in] */ ICursor* cursor)
{
    StringBuffer sb;
    DumpCursor(cursor, sb);
    return String(sb);
}

void DatabaseUtils::DumpCurrentRow(
    /* [in] */ ICursor* cursor)
{
    //dumpCurrentRow(cursor, System.out);
}

// public static void dumpCurrentRow(Cursor cursor, PrintStream stream) {
//     String[] cols = cursor.getColumnNames();
//     stream.println("" + cursor.getPosition() + " {");
//     int length = cols.length;
//     for (int i = 0; i< length; i++) {
//         String value;
//         try {
//             value = cursor.getString(i);
//         } catch (SQLiteException e) {
//             // assume that if the getString threw this exception then the column is not
//             // representable by a string, e.g. it is a BLOB.
//             value = "<unprintable>";
//         }
//         stream.println("   " + cols[i] + '=' + value);
//     }
//     stream.println("}");
// }

void DatabaseUtils::DumpCurrentRow(
    /* [in] */ ICursor* cursor,
    /* [in] */ StringBuffer& sb)
{
    // ArrayOf<String>* cols;
    // cursor->GetColumnNames(&cols);
    // *sb += "";
    // Int32 pos;
    // cursor->GetPosition(&pos);
    // *sb += pos;
    // *sb += " {\n";
    // Int32 length = cols->GetLength();
    // for (Int32 i = 0; i < length; i++) {
    //     String value;
    //     //try {
    //         cursor->GetString(i, &value);
    //     //} catch (SQLiteException e) {
    //         // assume that if the getString threw this exception then the column is not
    //         // representable by a string, e.g. it is a BLOB.
    //     //    value = "<unprintable>";
    //     //}
    //     *sb += "   ";
    //     *sb += cols[i];
    //     *sb += '=';
    //     *sb += value;
    //     *sb += "\n";
    // }
    // *sb += "}\n";
}

String DatabaseUtils::DumpCurrentRowToString(
    /* [in] */ ICursor* cursor)
{
    StringBuffer sb;
    DumpCurrentRow(cursor, sb);
    return String(sb);
}

void DatabaseUtils::CursorStringToContentValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ IContentValues* values)
{
    CursorStringToContentValues(cursor, field, values, field);
}

void DatabaseUtils::CursorStringToInsertHelper(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ InsertHelper* inserter,
    /* [in] */ Int32 index)
{
    // Int32 columnIndexOrThrow;
    // cursor->GetColumnIndexOrThrow(field, &columnIndexOrThrow);
    // String str;
    // cursor->GetString(columnIndexOrThrow, &str);
    // inserter->Bind(index, str);
}

void DatabaseUtils::CursorStringToContentValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& key)
{
    //values.put(key, cursor.getString(cursor.getColumnIndexOrThrow(field)));
}

void DatabaseUtils::CursorInt32ToContentValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ IContentValues* values)
{
    CursorInt32ToContentValues(cursor, field, values, field);
}

void DatabaseUtils::CursorInt32ToContentValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& key)
{
    Int32 colIndex;
    cursor->GetColumnIndex(field, &colIndex);
    Boolean isNull;
    cursor->IsNull(colIndex, &isNull);
    if (!isNull) {
        //values.put(key, cursor.getInt(colIndex));
    }
    else {
        //values.put(key, (Integer) null);
    }
}

void DatabaseUtils::CursorInt64ToContentValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ IContentValues* values)
{
    CursorInt64ToContentValues(cursor, field, values, field);
}

void DatabaseUtils::CursorInt64ToContentValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& key)
{
    Int32 colIndex;
    cursor->GetColumnIndex(field, &colIndex);
    Boolean isNUll;
    cursor->IsNull(colIndex, &isNUll);
    if (!isNUll) {
        Int64 value;
        cursor->GetInt64(colIndex, &value);
        //values.put(key, value);
    }
    else {
        //values.put(key, (Long) null);
    }
}

void DatabaseUtils::CursorDoubleToCursorValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ IContentValues* values)
{
    CursorDoubleToContentValues(cursor, field, values, field);
}


void DatabaseUtils::CursorDoubleToContentValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& field,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& key)
{
    Int32 colIndex;
    cursor->GetColumnIndex(field, &colIndex);
    Boolean isNUll;
    cursor->IsNull(colIndex, &isNUll);
    if (!isNUll) {
        //values.put(key, cursor.getDouble(colIndex));
    }
    else {
        //values.put(key, (Double) null);
    }
}

void DatabaseUtils::CursorRowToContentValues(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContentValues* values)
{
    /*AbstractWindowedCursor awc =
            (cursor instanceof AbstractWindowedCursor) ? (AbstractWindowedCursor) cursor : null;
    String[] columns = cursor.getColumnNames();
    int length = columns.length;
    for (int i = 0; i < length; i++) {
        if (awc != null && awc.isBlob(i)) {
            values.put(columns[i], cursor.getBlob(i));
        } else {
            values.put(columns[i], cursor.getString(i));
        }
    }*/
}

ECode DatabaseUtils::QueryNumEntries(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ const String& table,
    /* [out] */ Int64* numEntries)
{
    VALIDATE_NOT_NULL(numEntries);

    AutoPtr<ICursor> cursor;
    //db->QueryEx(table, countProjection,
    //        String(NULL), NULL, String(NULL), String(NULL), String(NULL), &cursor);
    //try {
    Boolean result;
    ECode ec = cursor->MoveToFirst(&result);
    if (SUCCEEDED(ec)) {
        ec = cursor->GetInt64(0, numEntries);
    }

    //} finally {
    cursor->Close();
    //}
    return ec;
}

ECode DatabaseUtils::Int64ForQuery(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ const String& query,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    AutoPtr<ISQLiteStatement> prog;
    db->CompileStatement(query, (ISQLiteStatement**)&prog);
    //try {
    ECode ec = Int64ForQuery(prog, selectionArgs, value);
    //} finally {
    prog->Close();
    //}
    return ec;
}

ECode DatabaseUtils::Int64ForQuery(
    /* [in] */ ISQLiteStatement* prog,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    if (selectionArgs != NULL) {
        Int32 size = selectionArgs->GetLength();
        for (Int32 i = 0; i < size; i++) {
            //BindObjectToProgram(prog, i + 1, (*selectionArgs)[i]);
        }
    }
    return prog->SimpleQueryForInt64(value);
}

ECode DatabaseUtils::StringForQuery(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ const String& query,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<ISQLiteStatement> prog;
    db->CompileStatement(query, (ISQLiteStatement**)&prog);
    //try {
    ECode ec = StringForQuery(prog, selectionArgs, str);
    //} finally {
    prog->Close();
    //}
    return ec;
}

ECode DatabaseUtils::StringForQuery(
    /* [in] */ ISQLiteStatement* prog,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    if (selectionArgs != NULL) {
        Int32 size = selectionArgs->GetLength();
        for (Int32 i = 0; i < size; i++) {
            //BindObjectToProgram(prog, i + 1, (*selectionArgs)[i]);
        }
    }
    return prog->SimpleQueryForString(str);
}

void DatabaseUtils::CursorStringToContentValuesIfPresent(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean isNUll;
    cursor->IsNull(index, &isNUll);
    if (!isNUll) {
        String columnValue;
        cursor->GetString(index, &columnValue);
        //values.put(column, *columnValue);
    }
}

void DatabaseUtils::CursorInt64ToContentValuesIfPresent(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean isNull;
    cursor->IsNull(index, &isNull);
    if (!isNull) {
        Int64 columnValue;
        cursor->GetInt64(index, &columnValue);
        //values.put(column, columnValue);
    }
}

void DatabaseUtils::CursorInt16ToContentValuesIfPresent(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean isNull;
    cursor->IsNull(index, &isNull);
    if (!isNull) {
        Int16 columnValue;
        cursor->GetInt16(index, &columnValue);
        //values.put(column, columnValue);
    }
}

void DatabaseUtils::CursorInt32ToContentValuesIfPresent(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean isNull;
    cursor->IsNull(index, &isNull);
    if (!isNull) {
        Int32 columnValue;
        cursor->GetInt32(index, &columnValue);
        //values.put(column, columnValue);
    }
}

void DatabaseUtils::CursorFloatToContentValuesIfPresent(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean isNull;
    cursor->IsNull(index, &isNull);
    if (!isNull) {
        Float columnValue;
        cursor->GetFloat(index, &columnValue);
        //values.put(column, columnValue);
    }
}

void DatabaseUtils::CursorDoubleToContentValuesIfPresent(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean isNull;
    cursor->IsNull(index, &isNull);
    if (!isNull) {
        Double columnValue;
        cursor->GetDouble(index, &columnValue);
        //values.put(column, columnValue);
    }
}

ECode DatabaseUtils::CreateDbFromSqlStatements(
    /* [in] */ IContext* context,
    /* [in] */ const String& dbName,
    /* [in] */ Int32 dbVersion,
    /* [in] */ const String& sqlStatements)
{
    /*SQLiteDatabase db = context.openOrCreateDatabase(dbName, 0, null);
    // TODO: this is not quite safe since it assumes that all semicolons at the end of a line
    // terminate statements. It is possible that a text field contains ;\n. We will have to fix
    // this if that turns out to be a problem.
    Arrayof<String>* statements;
    TextUtils::split(sqlStatements, String(";\n"), &statements);
    for (String statement : statements) {
        if (TextUtils.isEmpty(statement)) continue;
        db.execSQL(statement);
    }
    db.setVersion(dbVersion);
    db.close(); */
    return E_NOT_IMPLEMENTED;
}
