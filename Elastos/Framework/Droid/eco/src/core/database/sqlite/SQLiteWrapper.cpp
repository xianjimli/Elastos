#include "database/sqlite/SQLiteWrapper.h"

const CString SQLiteWrapper::TAG = "SQLiteWrapper";
const CString SQLiteWrapper::SQLITE_EXCEPTION_DETAIL_MESSAGE = "unable to open database file";


SQLiteWrapper::SQLiteWrapper()
{
    // Forbidden being instantiated.
}

// // FIXME: need to optimize this method.
// private static boolean isLowMemory(SQLiteException e) {
//     return e.getMessage().equals(SQLITE_EXCEPTION_DETAIL_MESSAGE);
// }

// public static void checkSQLiteException(Context context, SQLiteException e) {
//     if (isLowMemory(e)) {
//         Toast.makeText(context, com.android.internal.R.string.low_memory,
//                 Toast.LENGTH_SHORT).show();
//     } else {
//         throw e;
//     }
// }

AutoPtr<ICursor> SQLiteWrapper::Query(
    /* [in] */ IContext* context,
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<String>& projection,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [in] */ const String& sortOrder)
{
    AutoPtr<ICursor> cursor;
    ECode ec = resolver->Query(uri, projection, selection, selectionArgs, sortOrder, (ICursor**)&cursor);
    if (ec == E_SQLITE_EXCEPTION) {
        // Log.e(TAG, "Catch a SQLiteException when query: ", e);
        // checkSQLiteException(context, e);
        return NULL;
    }
    return cursor;
}

Boolean SQLiteWrapper::Requery(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor)
{
    Boolean result;
    ECode ec = cursor->Requery(&result);
    if (ec == E_SQLITE_EXCEPTION) {
        // Log.e(TAG, "Catch a SQLiteException when requery: ", e);
        // checkSQLiteException(context, e);
        return FALSE;
    }
    return result;
}

Int32 SQLiteWrapper::Update(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& where,
        /* [in] */ const ArrayOf<String>& selectionArgs)
{
    Int32 value;
    ECode ec = resolver->Update(uri, values, where, selectionArgs, &value);
    if (ec == E_SQLITE_EXCEPTION) {
        // Log.e(TAG, "Catch a SQLiteException when update: ", e);
        // checkSQLiteException(context, e);
        return -1;
    }
    return value;
}

Int32 SQLiteWrapper::Delete(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ const String& where,
        /* [in] */ const ArrayOf<String>& selectionArgs)
{
    Int32 value;
    ECode ec = resolver->Delete(uri, where, selectionArgs, &value);
    if (ec == E_SQLITE_EXCEPTION) {
        // Log.e(TAG, "Catch a SQLiteException when delete: ", e);
        // checkSQLiteException(context, e);
        return -1;
    }
    return value;
}

AutoPtr<IUri> SQLiteWrapper::Insert(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values)
{
    AutoPtr<IUri> outUri;
    ECode ec = resolver->Insert(uri, values, (IUri**)&outUri);
    if (ec == E_SQLITE_EXCEPTION) {
        // Log.e(TAG, "Catch a SQLiteException when insert: ", e);
        // checkSQLiteException(context, e);
        return NULL;
    }
    return uri;
}
