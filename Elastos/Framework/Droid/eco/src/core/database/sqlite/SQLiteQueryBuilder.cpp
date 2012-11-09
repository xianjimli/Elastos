#include "database/sqlite/SQLiteQueryBuilder.h"

const String SQLiteQueryBuilder::TAG = String("Database");

SQLiteQueryBuilder::SQLiteQueryBuilder()
{
    mDistinct = false;
    mFactory = NULL;
}

SQLiteQueryBuilder::~SQLiteQueryBuilder()
{
    
}

ECode SQLiteQueryBuilder::SetDistinct(
        /*[in]*/ Boolean distinct)
{
    mDistinct = distinct;
    return NOERROR;
}