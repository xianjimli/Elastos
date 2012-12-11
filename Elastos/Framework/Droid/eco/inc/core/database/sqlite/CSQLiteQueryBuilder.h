
#ifndef __CSQLITEQUERYBUILDER_H__
#define __CSQLITEQUERYBUILDER_H__

#include "_CSQLiteQueryBuilder.h"
#include "database/sqlite/SQLiteQueryBuilder.h"

CarClass(CSQLiteQueryBuilder), public SQLiteQueryBuilder
{
public:
    CARAPI constructor();

    CARAPI SetDistinct(
        /* [in] */ Boolean distinct);

    CARAPI GetTables(
        /* [out] */ String* tables);

    CARAPI SetTables(
        /* [in] */ const String& inTables);

    CARAPI AppendWhere(
        /* [in] */ ICharSequence *inWhere);

    CARAPI AppendWhereEscapeString(
        /* [in] */ const String& inWhere);

//        SetProjectionMap(
//            [in] Map<String, String> columnMap);

    CARAPI SetCursorFactory(
        /* [in] */ ICursorFactory* factory);

    CARAPI SetStrictProjectionMap(
        /* [in] */ Boolean flag);

    CARAPI Query(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);

    CARAPI QueryEx(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ ICursor **cursor);

    CARAPI BuildQuery(
        /* [in] */ ArrayOf<String>* projectionIn,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String *str);

    CARAPI BuildUnionSubQuery(
        /* [in] */ const String& typeDiscriminatorColumn,
        /* [in] */ const ArrayOf<String>& unionColumns,
        /* [in] */ Int32 computedColumnsOffset,
        /* [in] */ const String& typeDiscriminatorValue,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [out] */ String *str);

    CARAPI BuildUnionQuery(
        /* [in] */ const ArrayOf<String>& subQueries,
        /* [in] */ const String& sortOrder,
        /* [in] */ const String& limit,
        /* [out] */ String *str);
};

#endif // __CSQLITEQUERYBUILDER_H__
