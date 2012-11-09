#include "database/sqlite/SQLiteQuery.h"

const String SQLiteQuery::TAG = String("Cursor");

SQLiteQuery::SQLiteQuery()
{
}

SQLiteQuery::~SQLiteQuery()
{
}

SQLiteQuery::SQLiteQuery(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String query,
        /* [in] */ Int32 offsetIndex,
        /* [in] */ ArrayOf<String>* bindArgs) : mClosed(FALSE)
{
    SQLiteProgram::SQLiteProgram(db, query);
    mOffsetIndex = offsetIndex;
    mBindArgs = bindArgs;
}

ECode SQLiteQuery::FillWindow(
        /* [in] */ ICursorWindow* window,
        /* [in] */ Int32 maxRead,
        /* [in] */ Int32 lastPos,
        /* [out] */ Int32* rowNum)
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteQuery::ColumnCountLocked(
        /* [out] */ Int32* value)
{
    AcquireReference();
//    try {
//        return native_column_count();
//    } finally {
        ReleaseReference();
//    }
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteQuery::ColumnNameLocked(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* value)
{
    AcquireReference();
//    try {
//        return native_column_name(columnIndex);
//    } finally {
        ReleaseReference();
//    }
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteQuery::toString(
        /* [out] */ String* value)
{
    assert(!(value->IsNull()));
    *value = String("SQLiteQuery") + mSql;
    return NOERROR;
}

ECode SQLiteQuery::Close()
{
    SQLiteProgram::Close();
    mClosed = TRUE;
    return NOERROR;
}

ECode SQLiteQuery::Requery()
{
    if (mBindArgs != NULL) {
        Int32 len = mBindArgs->GetLength();
//        try {
                for (Int32 i = 0; i < len; i++) {
                    SQLiteProgram::BindString(i + 1, (*mBindArgs)[i]);
//                }
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
                }
    }
    return E_NOT_IMPLEMENTED;
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

ECode SQLiteQuery::BindLong(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value)
{
    (*mBindArgs)[index - 1] = String::FromInt64(value);
    if (!mClosed) {
        SQLiteProgram::BindLong(index, value);
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
        /* [in] */ String value)
{
    (*mBindArgs)[index - 1] = value;
    if (!mClosed) {
        SQLiteProgram::BindString(index, value);
    }
    return NOERROR;
}

Int32 SQLiteQuery::Native_Fill_Window(
        /* [in] */ ICursorWindow* window,
        /* [in] */ Int32 startPos,
        /* [in] */ Int32 offsetParam,
        /* [in] */ Int32 maxRead,
        /* [in] */ Int32 lastPos)
{
    return NOERROR;
}
    
Int32 SQLiteQuery::Native_Column_Count()
{/*
    sqlite3_stmt * statement = GET_STATEMENT(env, object); */

    sqlite3_stmt * statement = NULL;
    return sqlite3_column_count(statement);
}

String SQLiteQuery::Native_Column_Name(
        /* [in] */ Int32 columnIndex)
{/*
    sqlite3_stmt * statement = GET_STATEMENT(env, object);*/
    sqlite3_stmt * statement = NULL;
    char const * name;

    name = sqlite3_column_name(statement, columnIndex);

    return String(name);
}