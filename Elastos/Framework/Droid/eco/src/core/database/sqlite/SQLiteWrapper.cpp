#include "database/sqlite/SQLiteWrapper.h"

const String SQLiteWrapper::TAG = String("SQLiteWrapper");
const String SQLiteWrapper::SQLITE_EXCEPTION_DETAIL_MESSAGE = String("unable to open database file");

SQLiteWrapper::SQLiteWrapper()
{
}

SQLiteWrapper::~SQLiteWrapper()
{
}

ECode SQLiteWrapper::Query(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String> projection,
        /* [in] */ String selection,
        /* [in] */ ArrayOf<String> selectionArgs,
        /* [in] */ String sortOrder,
        /* [out] */ ICursor** c)
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteWrapper::Requery(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor,
        /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteWrapper::Update(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [in] */ String value,
        /* [in] */ ArrayOf<String> selectionArgs,
        /* [out] */ Int32* v)
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteWrapper::Delete(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ String where,
        /* [in] */ ArrayOf<String> selectionArgs,
        /* [out] */ Int32* v)
{
    return E_NOT_IMPLEMENTED;
}

ECode SQLiteWrapper::Insert(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [out] */ IUri** u)
{
    return E_NOT_IMPLEMENTED;
}