
#include "database/sqlite/CSQLiteQueryBuilder.h"
ECode CSQLiteQueryBuilder::SetDistinct(
    /* [in] */ Boolean distinct)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::SetDistinct(distinct);
}

ECode CSQLiteQueryBuilder::GetTables(
    /* [out] */ String *tab)
{
    return E_NOT_IMPLEMENTED;
 //   return SQLiteQueryBuilder::GetTables(tab);
}

ECode CSQLiteQueryBuilder::SetTables(
    /* [in] */ const String& inTables)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::SetTables(inTables);
}

ECode CSQLiteQueryBuilder::AppendWhere(
    /* [in] */ ICharSequence *inWhere)
{
    return E_NOT_IMPLEMENTED;
 //   return SQLiteQueryBuilder::AppendWhere(inWhere);
}

ECode CSQLiteQueryBuilder::AppendWhereEscapeString(
    /* [in] */ const String& inWhere)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::AppendWhereEscapeString(inWhere);
}

ECode CSQLiteQueryBuilder::SetProjectionMap()
{
    return E_NOT_IMPLEMENTED;
 //   return SQLiteQueryBuilder::SetProjectionMap();
}

ECode CSQLiteQueryBuilder::SetCursorFactory()
{
    return E_NOT_IMPLEMENTED;
 //   return SQLiteQueryBuilder::SetCursorFactory();
}

ECode CSQLiteQueryBuilder::SetStrictProjectionMap(
    /* [in] */ Boolean flag)
{
    return E_NOT_IMPLEMENTED;
 //   return SQLiteQueryBuilder::SetStrictProjectionMap(flag);
}

ECode CSQLiteQueryBuilder::BuildQueryString(
    /* [in] */ Boolean distinct,
    /* [in] */ const String& tables,
    /* [in] */ const ArrayOf<String> &columns,
    /* [in] */ const String& where,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& orderBy,
    /* [in] */ const String& limit)
{
    return E_NOT_IMPLEMENTED;
 //   return SQLiteQueryBuilder::BuildQueryString(distinct,tables,columns,where,groupBy,having,orderBy,limit);
}

ECode CSQLiteQueryBuilder::AppendClause(
    /* [in] */ const String& name,
    /* [in] */ const String& clause)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::AppendClause(name,clause);
}

ECode CSQLiteQueryBuilder::appendClauseEscapeClause(
    /* [in] */ const String& name,
    /* [in] */ const String& clause)
{
    return E_NOT_IMPLEMENTED;
 //   return SQLiteQueryBuilder::appendClauseEscapeClause(name,clause);
}

ECode CSQLiteQueryBuilder::AppendColumns(
    /* [in] */ const ArrayOf<String> &columns)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::AppendColumns(columns);
}

ECode CSQLiteQueryBuilder::Query(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ const ArrayOf<String> &projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> &selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor **cursor)
{
    return E_NOT_IMPLEMENTED;
 //   return SQLiteQueryBuilder::Query(db,projectionIn,selection,selectionArgs,groupBy,having,sortOrder,cursor);
}

ECode CSQLiteQueryBuilder::QueryEx(
    /* [in] */ ISQLiteDatabase *db,
    /* [in] */ const ArrayOf<String> &projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> &selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ ICursor **cursor)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::QueryEx(db,projectionIn,selection,selectionArgs,groupBy,having,sortOrder,limit,cursor);
}

ECode CSQLiteQueryBuilder::BuildQuery(
    /* [in] */ const ArrayOf<String> &projectionIn,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> &selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String *str)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::BuildQuery(projectionIn,selection,selectionArgs,groupBy,having,sortOrder,limit,str);
}

ECode CSQLiteQueryBuilder::BuildUnionSubQuery(
    /* [in] */ const String& typeDiscriminatorColumn,
    /* [in] */ const ArrayOf<String> &unionColumns,
    /* [in] */ Int32 computedColumnsOffset,
    /* [in] */ const String& typeDiscriminatorValue,
    /* [in] */ const String& selection,
    /* [in] */ const ArrayOf<String> &selectionArgs,
    /* [in] */ const String& groupBy,
    /* [in] */ const String& having,
    /* [out] */ String *str)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::BuildUnionSubQuery(typeDiscriminatorColumn,unionColumns,computedColumnsOffset,
//                                typeDiscriminatorValue,selection,selectionArgs,groupBy,having,str);
}

ECode CSQLiteQueryBuilder::BuildUnionQuery(
    /* [in] */ const ArrayOf<String> &subQueries,
    /* [in] */ const String& sortOrder,
    /* [in] */ const String& limit,
    /* [out] */ String *str)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::BuildUnionQuery(subQueries,sortOrder,limit,str);
}

ECode CSQLiteQueryBuilder::ComputeProjection(
    /* [in] */ const ArrayOf<String> &projectionIn,
    /* [out] */ String *proj)
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::ComputeProjection(projectionIn,proj);
}

ECode CSQLiteQueryBuilder::constructor()
{
    return E_NOT_IMPLEMENTED;
//    return SQLiteQueryBuilder::Init();
}

