#include "database/sqlite/SQLiteQueryBuilder.h"

const String SQLiteQueryBuilder::TAG = String("Database");

ECode SQLiteQueryBuilder::Init()
{
	mDistinct = false;
    mFactory = NULL;
	return NOERROR;
}

SQLiteQueryBuilder::SQLiteQueryBuilder()
{
    Init();
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