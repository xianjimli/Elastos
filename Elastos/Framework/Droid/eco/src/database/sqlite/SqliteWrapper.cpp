#include "database/sqlite/SqliteWrapper.h"

const String SqliteWrapper::TAG = String("SqliteWrapper");
const String SqliteWrapper::SQLITE_EXCEPTION_DETAIL_MESSAGE = String("unable to open database file");

SqliteWrapper::SqliteWrapper()
{
}

SqliteWrapper::~SqliteWrapper()
{
}

ECode SqliteWrapper::Query(
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

ECode SqliteWrapper::Requery(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor,
        /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode SqliteWrapper::Update(
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

ECode SqliteWrapper::Delete(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ String where,
        /* [in] */ ArrayOf<String> selectionArgs,
        /* [out] */ Int32* v)
{
    return E_NOT_IMPLEMENTED;
}

ECode SqliteWrapper::Insert(
        /* [in] */ IContext* context,
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [out] */ IUri** u)
{
    return E_NOT_IMPLEMENTED;
}