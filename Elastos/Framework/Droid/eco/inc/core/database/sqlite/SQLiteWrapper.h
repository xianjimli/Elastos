#ifndef __SQLITEWRAPPER_H__
#define __SQLITEWRAPPER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * @hide
 */
class SQLiteWrapper
{
public:
//    static CARAPI CheckSQLiteException(Context context, SQLiteException e)

    static CARAPI_(AutoPtr<ICursor>) Query(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ const ArrayOf<String>& projection,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [in] */ const String& sortOrder);

    static CARAPI_(Boolean) Requery(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor);

    static CARAPI_(Int32) Update(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& where,
        /* [in] */ ArrayOf<String>* selectionArgs);

    static CARAPI_(Int32) Delete(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ const String& where,
        /* [in] */ ArrayOf<String>* selectionArgs);

    static CARAPI_(AutoPtr<IUri>) Insert(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values);

private:
    SQLiteWrapper();

    // FIXME: need to optimize this method.
    // static CARAPI IsLowMemory(SQLiteException e)

private:
    const static CString TAG;
    const static CString SQLITE_EXCEPTION_DETAIL_MESSAGE;
};

#endif //__SQLITEWRAPPER_H__
