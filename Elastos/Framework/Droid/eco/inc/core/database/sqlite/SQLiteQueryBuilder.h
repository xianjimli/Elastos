#ifndef __SQLITEQUERYBUILDER_H__
#define __SQLITEQUERYBUILDER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Map.h>

class SQLiteQueryBuilder
{
public:
    CARAPI Init();

    SQLiteQueryBuilder();

    ~SQLiteQueryBuilder();

    /**
     * Mark the query as DISTINCT.
     *
     * @param distinct if true the query is DISTINCT, otherwise it isn't
     */
    CARAPI SetDistinct(
        /*[in]*/ Boolean distinct);

private:
    const static String TAG;
/*
    static final Pattern sLimitPattern =
            Pattern.compile("\\s*\\d+\\s*(,\\s*\\d+\\s*)?");  */

    Map<String, String> mProjectionMap;

    String mTables;

//    StringBuilder mWhereClause = NULL;  // lazily created

    Boolean mDistinct;

//    SQLiteDatabase.CursorFactory mFactory;
    AutoPtr<ICursorFactory> mFactory;

    Boolean mStrictProjectionMap;
};


#endif //__SQLITEQUERYBUILDER_H__