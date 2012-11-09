#include "database/sqlite/SQLiteCursorDriver.h"

ECode SQLiteCursorDriver::Query(
    	/*[in]*/  ICursorFactory* factory, 
    	/*[in]*/  ArrayOf<String>* bindArgs,
    	/*[out]*/ ICursor** cs)
{
    return NOERROR;
}


ECode SQLiteCursorDriver::CursorDeactivated()
{
    return NOERROR;
}


ECode SQLiteCursorDriver::CursorRequeried(
    	/*[in]*/ICursor* cursor)
{
    return NOERROR;
}

ECode SQLiteCursorDriver::CursorClosed()
{
    return NOERROR;
}


ECode SQLiteCursorDriver::SetBindArguments(
    	/*[in]*/ ArrayOf<String>* bindArgs)
{
    return NOERROR;
}