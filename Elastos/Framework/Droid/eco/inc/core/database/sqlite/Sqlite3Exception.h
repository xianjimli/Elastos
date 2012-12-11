
#ifndef __SQLITE3EXCEPTION_H__
#define __SQLITE3EXCEPTION_H__

#include <elastos.h>
#include <sqlite3.h>

/* throw a SQLiteException with a message appropriate for the error in handle */
Elastos::ECode throw_sqlite3_exception(sqlite3* handle);

/* throw a SQLiteException for a given error code */
Elastos::ECode throw_sqlite3_exception_errcode(Elastos::Int32 errcode);

#endif //__SQLITE3EXCEPTION_H__
