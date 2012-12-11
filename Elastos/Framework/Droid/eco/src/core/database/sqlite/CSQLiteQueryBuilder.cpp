
#include "ext/frameworkdef.h"
#include "database/sqlite/CSQLiteQueryBuilder.h"

ECode CSQLiteQueryBuilder::constructor()
{
    return NOERROR;
}

ECode CSQLiteQueryBuilder::SetDistinct(
    /* [in] */ Boolean distinct)
{
   return SQLiteQueryBuilder::SetDistinct(distinct);
}

ECode CSQLiteQueryBuilder::GetTables(
    /* [out] */ String* tables)
{
    VALIDATE_NOT_NULL(tables);

    *tables = SQLiteQueryBuilder::GetTables();
    return NOERROR;
}

ECode CSQLiteQueryBuilder::SetTables(
    /* [in] */ const String& inTables)
{
    return SQLiteQueryBuilder::SetTables(inTables);
}

ECode CSQLiteQueryBuilder::AppendWhere(
    /* [in] */ ICharSequence *inWhere)
{
    return SQLiteQueryBuilder::AppendWhere(inWhere);
}

ECode CSQLiteQueryBuilder::AppendWhereEscapeString(
    /* [in] */ const String& inWhere)
{
    return SQLiteQueryBuilder::AppendWhereEscapeString(inWhere);
}

//        SetProjectionMap(
//            [in] Map<String, String> columnMap);

ECode CSQLiteQueryBuilder::SetCursorFactory(
    /* [in] */ ICursorFactory* factory)
{
    return SQLiteQueryBuilder::SetCursorFactory(factory);
}

ECode CSQLiteQueryBuilder::SetStrictProjectionMap(
    /* [in] */ Boolean flag)
{
    return SQLiteQueryBuilder::SetStrictProjectionMap(flag);
}

ECode CSQLiteQueryBuilder::Query(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    return SQLiteQueryBuilder::Query(db, projectionIn, selection,
            selectionArgs, groupBy, having, sortOrder, cursor);
}

ECode CSQLiteQueryBuilder::QueryEx(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ ICursor **cursor)
{
    VALIDATE_NOT_NULL(cursor);

    return SQLiteQueryBuilder::Query(db, projectionIn, selection,
            selectionArgs, groupBy, having, sortOrder, limit, cursor);
}

ECode CSQLiteQueryBuilder::BuildQuery(
    /* [in] */ ArrayOf<String>* projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);

    return SQLiteQueryBuilder::BuildQuery(projectionIn, selection,
            selectionArgs, groupBy, having, sortOrder, limit, str);
}

ECode CSQLiteQueryBuilder::BuildUnionSubQuery(
    /* [in] */ const String& typeDiscriminatorColumn,
    /* [in] */ const ArrayOf<String>& unionColumns,
    /* [in] */ Int32 computedColumnsOffset,
    /* [in] */ const String& typeDiscriminatorValue,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);

    return SQLiteQueryBuilder::BuildUnionSubQuery(typeDiscriminatorColumn ,
            unionColumns, computedColumnsOffset, typeDiscriminatorValue,
            selection, selectionArgs, groupBy, having, str);
}

ECode CSQLiteQueryBuilder::BuildUnionQuery(
    /* [in] */ const ArrayOf<String>& subQueries,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str);

    return SQLiteQueryBuilder::BuildUnionQuery(subQueries, sortOrder, limit, str);
}
