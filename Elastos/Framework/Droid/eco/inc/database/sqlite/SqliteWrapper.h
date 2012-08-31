#ifndef __SQLITEWRAPPER_H__
#define __SQLITEWRAPPER_H__

#include "ext/frameworkext.h"

class SqliteWrapper
{
public:
//    static CARAPI CheckSQLiteException(Context context, SQLiteException e)

    static CARAPI Query(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String> projection,
        /* [in] */ String selection,
        /* [in] */ ArrayOf<String> selectionArgs,
        /* [in] */ String sortOrder,
        /* [out] */ ICursor** c);

    static CARAPI Requery(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor,
        /* [out] */ Boolean* rst);

    static CARAPI Update(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [in] */ String value,
        /* [in] */ ArrayOf<String> selectionArgs,
        /* [out] */ Int32* v);

    static CARAPI Delete(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ String where,
        /* [in] */ ArrayOf<String> selectionArgs,
        /* [out] */ Int32* v);

    static CARAPI Insert(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [out] */ IUri** u);

private:
    SqliteWrapper();

    ~SqliteWrapper();

//    static CARAPI IsLowMemory(SQLiteException e)

private:
    const static String TAG;
    const static String SQLITE_EXCEPTION_DETAIL_MESSAGE;
};
#endif //__SQLITEWRAPPER_H__