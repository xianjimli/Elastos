#include "database/DatabaseUtils.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;

const String DatabaseUtils::TAG = String("DatabaseUtils");

const ArrayOf<String>* DatabaseUtils::countProjection = ArrayOf<String>::Alloc(1);

const Char8 DatabaseUtils::DIGITS[16] = {
    '0','1','2','3','4','5','6','7',
    '8','9','A','B','C','D','E','F'};

DatabaseUtils::DatabaseUtils()
{
}

DatabaseUtils::~DatabaseUtils()
{
}

const ECode DatabaseUtils::ReadExceptionFromParcel(
        /* [in] */ IParcel* reply)
{
    Int32 code = 0;
//    reply->ReadExceptionCode(&code);
    if (code == 0) {
        return NOERROR;
    }
    String msg = String(" ");
    reply->ReadString(&msg);
    DatabaseUtils::ReadExceptionFromParcel(reply, msg, code);
    return NOERROR;
}

ECode DatabaseUtils::ReadExceptionWithFileNotFoundExceptionFromParcel(
        /* [in] */ IParcel* reply)
{
    Int32 code = 0;
//    reply->ReadExceptionCode(&code);
    if (code == 0) {
        return NOERROR;
    }
    String msg = String(" ");
    reply->ReadString(&msg);
    if (code == 1) {
        //throw new FileNotFoundException(msg);
        return E_FILE_NOT_FOUND_EXCEPTION;
    } else {
        DatabaseUtils::ReadExceptionFromParcel(reply, msg, code);
    }
    return NOERROR;
}

ECode DatabaseUtils::ReadExceptionWithOperationApplicationExceptionFromParcel(
        /* [in] */ IParcel* reply)
{
    Int32 code = 0;
//    reply->ReadExceptionCode(&code);
    if (code == 0) {
        return NOERROR;
    }
    String msg = String(" ");
    reply->ReadString(&msg);
    if (code == 10) {
        //throw new OperationApplicationException(msg);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    } else {
        DatabaseUtils::ReadExceptionFromParcel(reply, msg, code);
    }
    return NOERROR;
}

const ECode DatabaseUtils::ReadExceptionFromParcel(
        /* [in] */ IParcel* reply,
        /* [in] */ String msg,
        /* [in] */ Int32 code)
{
    switch (code) {
        case 2:
            //throw new IllegalArgumentException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case 3:
            //throw new UnsupportedOperationException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case 4:
            //throw new SQLiteAbortException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case 5:
            //throw new SQLiteConstraintException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case 6:
            //throw new SQLiteDatabaseCorruptException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case 7:
            //throw new SQLiteFullException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case 8:
            //throw new SQLiteDiskIOException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case 9:
            //throw new SQLiteException(msg);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        default:
            //reply->readException(code, msg);
            return NOERROR;
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

ECode DatabaseUtils::AppendEscapedSQLString(
        /* [in] */ StringBuffer* sb,
        /* [in] */ String sqlString)
{
    *sb += '\'';
    if (sqlString.IndexOf('\'') != -1) {
        Int32 length = (Int32)sqlString.GetLength();
        for (Int32 i = 0; i < length; i++) {
            Char8 c = sqlString.GetChar(i);
            if (c == '\'') {
                *sb += '\'';
            }
            *sb += c;
        }
    } else {
        *sb += sqlString;
    }
    *sb += '\'';
    return NOERROR;
}

ECode DatabaseUtils::SqlEscapeString(
        /* [in] */ String value,
        /* [out] */ String* sqlEscapeString)
{
    assert(sqlEscapeString != NULL);

    StringBuffer *escaper = new StringBuffer();

    AppendEscapedSQLString(escaper, value);

    Int32 end = escaper->GetLength();
    *sqlEscapeString = escaper->Substring(0, end);

    return NOERROR;
}

const ECode DatabaseUtils::AppendValueToSql(
        /* [in] */ StringBuffer* sql,
        /* [in] */ IInterface* value)
{
    if (value == NULL) {
        *sql += String("NULL");
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
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::ConcatenateWhere(
        /* [in] */ String a,
        /* [in] */ String b,
        /* [out] */ String* where)
{
    assert(where != NULL);
    /*AutoPtr<ICharSequence> csa = (ICharSequence**)&a;
    AutoPtr<ICharSequence> csb = (ICharSequence**)&b;
    if (TextUtils::IsEmpty(csa)) {
        *where = b;
        return NOERROR;
    }
    if (TextUtils::IsEmpty(csb)) {
        *where = a;
        return NOERROR;
    }

    *where = String("(" + a + ") AND (" + b + ")");*/
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::GetCollationKey(
        /* [in] */ String name,
        /* [out] */ String* collationKey)
{
    assert(collationKey != NULL);

    ArrayOf<Byte>* arr;
    GetCollationKeyInBytes(name, &arr);

    //try {
        //return new String(arr, 0, getKeyLen(arr), "ISO8859_1");

        Int32 keyLen;
        GetKeyLen(arr, &keyLen);
        Char8* value = new Char8;
        for (Int32 i = 0; i < keyLen; ++i) {
            value[i] = (Char8)((*arr)[i] & 0xFF);
        }
        *collationKey = String(value);
    //} catch (Exception ex) {
    //    return "";
    //}
    return NOERROR;
}

ECode DatabaseUtils::GetHexCollationKey(
        /* [in] */ String name,
        /* [out] */ String* hexCollationKey)
{
    assert(hexCollationKey != NULL);

    ArrayOf<Byte>* arr;
    GetCollationKeyInBytes(name, &arr);

    //char[] keys = Hex.encodeHex(arr);

    Int32 I = arr->GetLength();
    Char8 keys[I<<1];
    for (Int32 i = 0, j = 0; i < I; i++) {
        keys[j++] = DIGITS[0xF0 & (*arr)[i] >> 4];
        keys[j++] = DIGITS[0x0F & (*arr)[i]];
    }

    //return new String(keys, 0, getKeyLen(arr) * 2);
    Int32 keyLen;
    GetKeyLen(arr, &keyLen);
    Char8* value = new Char8;
    for (Int32 i = 0; i < keyLen * 2; ++i) {
        value[i] = keys[i];
    }
    *hexCollationKey = String(value);
    return NOERROR;
}

ECode DatabaseUtils::GetKeyLen(
        /* [in] */ ArrayOf<Byte>* arr,
        /* [out] */ Int32* keyLen)
{
    assert(keyLen != NULL);

    if ((*arr)[arr->GetLength() - 1] != 0) {
        *keyLen = arr->GetLength();
        return NOERROR;
    } else {
        // remove zero "termination"
        *keyLen = arr->GetLength() - 1;
        return NOERROR;
    }
}

ECode DatabaseUtils::GetCollationKeyInBytes(
        /* [in] */ String name,
        /* [out] */ ArrayOf<Byte>** collationKeyInBytes)
{
    /*if (mColl == null) {
        mColl = Collator.getInstance();
        mColl.setStrength(Collator.PRIMARY);
    }
    return mColl.getCollationKey(name).toByteArray(); */
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::DumpCursor(
        /* [in] */ ICursor* cursor)
{
    //DumpCursor(cursor, System.out);
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::DumpCursor(
        /* [in] */ ICursor* cursor,
        /* [in] */ StringBuffer* sb)
{
    *sb += ">>>>> Dumping cursor ";
    //*sb += (*cursor);
    *sb += "\n";
    if (cursor != NULL) {
        Int32 startPos;
        cursor->GetPosition(&startPos);

        Boolean value1, value2;
        cursor->MoveToPosition(-1, &value1);
        cursor->MoveToNext(&value2);
        while (value2) {
            DumpCurrentRow(cursor, sb);
            cursor->MoveToNext(&value2);
        }
        cursor->MoveToPosition(startPos, &value1);
    }
    *sb += "<<<<<\n";
    return NOERROR;
}

ECode DatabaseUtils::DumpCursorToString(
        /* [in] */ ICursor* cursor,
        /* [out] */ String* dumpCursor)
{
    assert(dumpCursor != NULL);

    StringBuffer* sb = new StringBuffer();
    DumpCursor(cursor, sb);
    *dumpCursor = sb->Substring(0, sb->GetLength());
    return NOERROR;
}

ECode DatabaseUtils::DumpCurrentRow(
        /* [in] */ ICursor* cursor)
{
    //dumpCurrentRow(cursor, System.out);
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::DumpCurrentRow(
        /* [in] */ ICursor* cursor,
        /* [in] */ StringBuffer* sb)
{
    ArrayOf<String>* cols;
    cursor->GetColumnNames(&cols);
    *sb += "";
    Int32 pos;
    cursor->GetPosition(&pos);
    *sb += pos;
    *sb += " {\n";
    Int32 length = cols->GetLength();
    for (Int32 i = 0; i < length; i++) {
        String value;
        //try {
            cursor->GetString(i, &value);
        //} catch (SQLiteException e) {
            // assume that if the getString threw this exception then the column is not
            // representable by a string, e.g. it is a BLOB.
        //    value = "<unprintable>";
        //}
        *sb += "   ";
        *sb += cols[i];
        *sb += '=';
        *sb += value;
        *sb += "\n";
    }
    *sb += "}\n";
    return NOERROR;
}

ECode DatabaseUtils::DumpCurrentRowToString(
        /* [in] */ ICursor* cursor,
        /* [out] */ String* dumpCurrentRow)
{
    assert(dumpCurrentRow != NULL);
    StringBuffer* sb = new StringBuffer();
    DumpCurrentRow(cursor, sb);
    *dumpCurrentRow = sb->Substring(0, sb->GetLength());
    return NOERROR;
}

ECode DatabaseUtils::CursorStringToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ IContentValues* values)
{
    CursorStringToContentValues(cursor, field, values, field);
    return NOERROR;
}

ECode DatabaseUtils::CursorStringToInsertHelper(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ InsertHelper* inserter,
        /* [in] */ Int32 index)
{
    Int32 columnIndexOrThrow;
    cursor->GetColumnIndexOrThrow(field, &columnIndexOrThrow);
    String str;
    cursor->GetString(columnIndexOrThrow, &str);
    inserter->Bind(index, str);
    return NOERROR;
}

ECode DatabaseUtils::CursorStringToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ IContentValues* values,
        /* [in] */ String key)
{
    //values.put(key, cursor.getString(cursor.getColumnIndexOrThrow(field)));
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::CursorIntToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ IContentValues* values)
{
    CursorIntToContentValues(cursor, field, values, field);
    return NOERROR;
}

ECode DatabaseUtils::CursorIntToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ IContentValues* values,
        /* [in] */ String key)
{
    Int32 colIndex;
    cursor->GetColumnIndex(field, &colIndex);
    Boolean value;
    cursor->IsNull(colIndex, &value);
    if (!value) {
        //values.put(key, cursor.getInt(colIndex));
    } else {
        //values.put(key, (Integer) null);
    }
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::CursorLongToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ IContentValues* values)
{
    CursorLongToContentValues(cursor, field, values, field);
    return NOERROR;
}

ECode DatabaseUtils::CursorLongToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ IContentValues* values,
        /* [in] */ String key)
{
    Int32 colIndex;
    cursor->GetColumnIndex(field, &colIndex);
    Boolean res;
    cursor->IsNull(colIndex, &res);
    if (!res) {
        Int64 value;
        cursor->GetInt64(colIndex, &value);
        //values.put(key, value);
    } else {
        //values.put(key, (Long) null);
    }
    return NOERROR;
}

ECode DatabaseUtils::CursorDoubleToCursorValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ IContentValues* values)
{
    CursorDoubleToContentValues(cursor, field, values, field);
    return NOERROR;
}


ECode DatabaseUtils::CursorDoubleToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ String field,
        /* [in] */ IContentValues* values,
        /* [in] */ String key)
{
    Int32 colIndex;
    cursor->GetColumnIndex(field, &colIndex);
    Boolean res;
    cursor->IsNull(colIndex, &res);
    if (!res) {
        //values.put(key, cursor.getDouble(colIndex));
    } else {
        //values.put(key, (Double) null);
    }
    return NOERROR;
}

ECode DatabaseUtils::CursorRowToContentValues(
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
    return E_NOT_IMPLEMENTED;
}

ECode DatabaseUtils::QueryNumEntries(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String table,
        /* [out] */ Int64* numEntries)
{
    assert(numEntries != NULL);

    AutoPtr<ICursor> cursor;

    //db->QueryEx(table, countProjection,
    //        String(NULL), NULL, String(NULL), String(NULL), String(NULL), &cursor);
    //try {
        Boolean result;
        cursor->MoveToFirst(&result);

        cursor->GetInt64(0, numEntries);

        return E_NOT_IMPLEMENTED;
    //} finally {
        //cursor.close();
    //}
}

ECode DatabaseUtils::LongForQuery(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String query,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ Int64* longValue)
{
    AutoPtr<ISQLiteStatement> prog;
    db->CompileStatement(query, (ISQLiteStatement**)&prog);
    //try {
        LongForQuery(prog, selectionArgs, longValue);
        return NOERROR;
    //} finally {
    //    prog.close();
    //}
}

ECode DatabaseUtils::LongForQuery(
        /* [in] */ ISQLiteStatement* prog,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ Int64* longValue)
{
    assert(longValue != NULL);

    if (selectionArgs != NULL) {
        Int32 size = selectionArgs->GetLength();
        for (Int32 i = 0; i < size; i++) {
            //BindObjectToProgram(prog, i + 1, (*selectionArgs)[i]);
        }
    }
    prog->SimpleQueryForInt64(longValue);
    return NOERROR;
}

ECode DatabaseUtils::StringForQuery(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String query,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ String* stringValue)
{
    assert(stringValue != NULL);

    AutoPtr<ISQLiteStatement> prog;
    db->CompileStatement(query, (ISQLiteStatement**)&prog);
    //try {
        StringForQuery(prog, selectionArgs, stringValue);
        return NOERROR;
    //} finally {
        //prog.close();
    //}
}

ECode DatabaseUtils::StringForQuery(
        /* [in] */ ISQLiteStatement* prog,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ String* stringValue)
{
    assert(stringValue != NULL);

    if (selectionArgs != NULL) {
        Int32 size = selectionArgs->GetLength();
        for (Int32 i = 0; i < size; i++) {
            //BindObjectToProgram(prog, i + 1, (*selectionArgs)[i]);
        }
    }
    prog->SimpleQueryForString(stringValue);
    return NOERROR;
}

ECode DatabaseUtils::CursorStringToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ String column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean value;
    cursor->IsNull(index, &value);
    if (!value) {
        String* columnValue = new String;
        cursor->GetString(index, columnValue);
        //values.put(column, *columnValue);
    }
    return NOERROR;
}

ECode DatabaseUtils::CursorLongToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ String column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean value;
    cursor->IsNull(index, &value);
    if (!value) {
        Int64 columnValue;
        cursor->GetInt64(index, &columnValue);
        //values.put(column, columnValue);
    }
    return NOERROR;
}

ECode DatabaseUtils::CursorShortToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ String column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean value;
    cursor->IsNull(index, &value);
    if (!value) {
        Int16 columnValue;
        cursor->GetInt16(index, &columnValue);
        //values.put(column, columnValue);
    }
    return NOERROR;
}

ECode DatabaseUtils::CursorIntToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ String column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean value;
    cursor->IsNull(index, &value);
    if (!value) {
        Int32 columnValue;
        cursor->GetInt32(index, &columnValue);
        //values.put(column, columnValue);
    }
    return NOERROR;
}

ECode DatabaseUtils::CursorFloatToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ String column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean value;
    cursor->IsNull(index, &value);
    if (!value) {
        Float columnValue;
        cursor->GetFloat(index, &columnValue);
        //values.put(column, columnValue);
    }
    return NOERROR;
}

ECode DatabaseUtils::CursorDoubleToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ String column)
{
    Int32 index;
    cursor->GetColumnIndexOrThrow(column, &index);
    Boolean value;
    cursor->IsNull(index, &value);
    if (!value) {
        Double columnValue;
        cursor->GetDouble(index, &columnValue);
        //values.put(column, columnValue);
    }
    return NOERROR;
}

DatabaseUtils::InsertHelper::InsertHelper()
{
    mInsertSQL = String(NULL);

    mInsertStatement = NULL;
    mReplaceStatement = NULL;
    mPreparedStatement = NULL;
}

DatabaseUtils::InsertHelper::InsertHelper(
            /* [in] */ ISQLiteDatabase* db,
            /* [in] */ String tableName)
{
    mDb = db;
    mTableName = tableName;
}

ECode DatabaseUtils::InsertHelper::BuildSQL()
{
    StringBuffer* sb = new StringBuffer();
    *sb += "INSERT INTO ";
    *sb += mTableName;
    *sb += " (";

    StringBuffer* sbv = new StringBuffer();
    *sbv += "VALUES (";

    Int32 i = 1;
    AutoPtr<ICursor> cur;
    //try {
        StringBuffer* tmp = new StringBuffer();
        *tmp = "PRAGMA table_info(";
        *tmp += mTableName;
        *tmp += ")";
        String sql = tmp->Substring(0, tmp->GetLength());
        //mDb->RawQuery(sql, NULL, (ICursor**)&cur);
        Int32 count;
        cur->GetCount(&count);
        mColumns = new HashMap<String, Int32>(count);
        Boolean moveToNext;
        cur->MoveToNext(&moveToNext);
        while (moveToNext) {
            String* columnName = new String;;
            cur->GetString(TABLE_INFO_PRAGMA_COLUMNNAME_INDEX, columnName);
            String* defaultValue = new String;
            cur->GetString(TABLE_INFO_PRAGMA_DEFAULT_INDEX, defaultValue);

            //mColumns->Insert(columnName, i);
            *sb += "'";
            *sb += columnName;
            *sb += "'";

            if (defaultValue->IsNull()) {
                *sbv += "?";
            } else {
                *sbv += "COALESCE(?, ";
                *sbv += defaultValue;
                *sbv += ")";
            }

            *sb += (i == count ? ") " : ", ");
            *sbv += (i == count ? ");" : ", ");
            ++i;

            cur->MoveToNext(&moveToNext);
        }
    //} finally {
    //    if (cur != null) cur.close();
    //}

    *sb += *sbv;

    mInsertSQL = sb->Substring(0, sb->GetLength());
    if (LOCAL_LOGV) {
        StringBuffer* tmp2 = new StringBuffer();
        *tmp2 = "insert statement is ";
        *tmp2 += mInsertSQL;
        String args2 = tmp2->Substring(0, tmp2->GetLength());
        Logger::V(TAG, args2);
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
            StringBuffer* sb = new StringBuffer();
            *sb = "INSERT OR REPLACE";
            *sb += mInsertSQL.Substring(6);
            String replaceSQL = sb->Substring(0, sb->GetLength());
            mDb->CompileStatement(replaceSQL, (ISQLiteStatement**)&mReplaceStatement);
        }
        *statement = mReplaceStatement;
        return NOERROR;
    } else {
        if (mInsertStatement == NULL) {
            if (mInsertSQL == NULL) {
                BuildSQL();
            }
            mDb->CompileStatement(mInsertSQL, (ISQLiteStatement**)&mInsertStatement);
        }
        *statement = mInsertStatement;
        return NOERROR;
    }
}

ECode DatabaseUtils::InsertHelper::InsertInternal(
            /* [in] */ IContentValues* values,
            /* [in] */ Boolean allowReplace,
            /* [out] */ Int64* value)
{
    //try {
        AutoPtr<ISQLiteStatement> stmt;
        GetStatement(allowReplace, (ISQLiteStatement**)&stmt);
        stmt->ClearBindings();
        if (LOCAL_LOGV) {
            StringBuffer* tmp = new StringBuffer();
            *tmp = "--- inserting in table ";
            *tmp += mTableName;
            String sql = tmp->Substring(0, tmp->GetLength());
            Logger::V(TAG, sql);
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
        return E_NOT_IMPLEMENTED;
    //} catch (SQLException e) {
        //Log.e(TAG, "Error inserting " + values + " into table  " + mTableName, e);
        //return -1;
    //}
}

ECode DatabaseUtils::InsertHelper::GetColumnIndex(
            /* [in] */ String key,
            /* [out] */ Int32* columnIndex)
{
    assert(columnIndex != NULL);

    AutoPtr<ISQLiteStatement> stmt;
    GetStatement(FALSE, (ISQLiteStatement**)&stmt);
    //final Integer index = mColumns.get(key);
    HashMap<String, Int32 >::Iterator iter = mColumns->Begin();
    for (; iter != mColumns->End(); ++iter) {
        if(iter == mColumns->Find(key))
            break;
    }
    Int32 index;
    if(iter != mColumns->End()) {
        index = iter->mSecond;
    }
    //if (index == NULL) {
        //throw new IllegalArgumentException("column '" + key + "' is invalid");
        //return E_ILLEGAL_ARGUMENT_EXCEPTION;
    //}
    *columnIndex = index;
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Bind(
            /* [in] */ Int32 index,
            /* [in] */ Double value)
{
    mPreparedStatement->BindDouble(index, value);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Bind(
            /* [in] */ Int32 index,
            /* [in] */ Float value)
{
    mPreparedStatement->BindDouble(index, value);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Bind(
            /* [in] */ Int32 index,
            /* [in] */ Int64 value)
{
    mPreparedStatement->BindInt64(index, value);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Bind(
            /* [in] */ Int32 index,
            /* [in] */ Int32 value)
{
    mPreparedStatement->BindInt64(index, value);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Bind(
            /* [in] */ Int32 index,
            /* [in] */ Boolean value)
{
    mPreparedStatement->BindInt64(index, value ? 1 : 0);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::BindNull(
            /* [in] */ Int32 index)
{
    mPreparedStatement->BindNull(index);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Bind(
            /* [in] */ Int32 index,
            /* [in] */ ArrayOf<Byte>* value)
{
    if (value == NULL) {
        mPreparedStatement->BindNull(index);
    } else {
        mPreparedStatement->BindBlob(index, *value);
    }
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Bind(
            /* [in] */ Int32 index,
            /* [in] */ String value)
{
    if (value == NULL) {
        mPreparedStatement->BindNull(index);
    } else {
        mPreparedStatement->BindString(index, value);
    }
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Insert(
            /* [in] */ IContentValues* values,
            /* [out] */ Int64* value)
{
    InsertInternal(values, FALSE, value);
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Execute(
            /* [out] */ Int64* value)
{
    assert(value != NULL);
    if (mPreparedStatement == NULL) {
        //throw new IllegalStateException("you must prepare this inserter before calling "
               // + "execute");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //try {
        if (LOCAL_LOGV) {
            Logger::V(TAG, "--- doing insert or replace in table " + mTableName);
        }
        mPreparedStatement->ExecuteInsert(value);
        return NOERROR;
    //} catch (SQLException e) {
        //Log.e(TAG, "Error executing InsertHelper with table " + mTableName, e);
        //return -1;
    //} finally {
        // you can only call this once per prepare
        //mPreparedStatement = null;
    //}
}

ECode DatabaseUtils::InsertHelper::PrepareForInsert()
{
    GetStatement(FALSE, (ISQLiteStatement**)&mPreparedStatement);
    mPreparedStatement->ClearBindings();
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::PrepareForReplace()
{
    GetStatement(TRUE, (ISQLiteStatement**)&mPreparedStatement);
    mPreparedStatement->ClearBindings();
    return NOERROR;
}

ECode DatabaseUtils::InsertHelper::Replace(
            /* [in] */ IContentValues* values,
            /* [out] */ Int64* value)
{
    InsertInternal(values, TRUE, value);
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

ECode DatabaseUtils::CreateDbFromSqlStatements(
            /* [in] */ IContext* context,
            /* [in] */ String dbName,
            /* [in] */ Int32 dbVersion,
            /* [in] */ String sqlStatements)
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
