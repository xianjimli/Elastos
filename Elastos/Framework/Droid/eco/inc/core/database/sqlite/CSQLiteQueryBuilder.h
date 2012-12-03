
#ifndef __CSQLITEQUERYBUILDER_H__
#define __CSQLITEQUERYBUILDER_H__

#include "_CSQLiteQueryBuilder.h"
#include "SQLiteQueryBuilder.h"

CarClass(CSQLiteQueryBuilder),public SQLiteQueryBuilder
{
public:
    CARAPI SetDistinct(
        /* [in] */ Boolean distinct);

    CARAPI GetTables(
        /* [out] */ String *tab);

    CARAPI SetTables(
        /* [in] */ const String& inTables);

    CARAPI AppendWhere(
        /* [in] */ ICharSequence *inWhere);

    CARAPI AppendWhereEscapeString(
        /* [in] */ const String& inWhere);

    CARAPI SetProjectionMap();

    CARAPI SetCursorFactory();

    CARAPI SetStrictProjectionMap(
        /* [in] */ Boolean flag);

    CARAPI BuildQueryString(
        /* [in] */ Boolean distinct,
        /* [in] */ const String& tables,
        /* [in] */ const ArrayOf<String> &columns,
        /* [in] */ const String& where,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit);

    CARAPI AppendClause(
        /* [in] */ const String& name,
        /* [in] */ const String& clause);

    CARAPI appendClauseEscapeClause(
        /* [in] */ const String& name,
        /* [in] */ const String& clause);

    CARAPI AppendColumns(
        /* [in] */ const ArrayOf<String> &columns);

    CARAPI Query(
        /* [in] */ ISQLiteDatabase *db,
        /* [in] */ const ArrayOf<String> &projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> &selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor **cursor);

    CARAPI QueryEx(
        /* [in] */ ISQLiteDatabase *db,
        /* [in] */ const ArrayOf<String> &projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> &selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ ICursor **cursor);

    CARAPI BuildQuery(
        /* [in] */ const ArrayOf<String> &projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> &selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String *str);

    CARAPI BuildUnionSubQuery(
        /* [in] */ const String& typeDiscriminatorColumn,
        /* [in] */ const ArrayOf<String> &unionColumns,
        /* [in] */ Int32 computedColumnsOffset,
        /* [in] */ const String& typeDiscriminatorValue,
        /* [in] */ const String& selection,
        /* [in] */ const ArrayOf<String> &selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [out] */ String *str);

    CARAPI BuildUnionQuery(
        /* [in] */ const ArrayOf<String> &subQueries,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String *str);

    CARAPI ComputeProjection(
        /* [in] */ const ArrayOf<String> &projectionIn,
        /* [out] */ String *proj);

    CARAPI constructor();
};

#endif // __CSQLITEQUERYBUILDER_H__
