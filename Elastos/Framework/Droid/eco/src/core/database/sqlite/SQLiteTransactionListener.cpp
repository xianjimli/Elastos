#include "database/sqlite/SQLiteTransactionListener.h"

ECode SQLiteTransactionListener::OnBegin()
{
    return NOERROR;
}


ECode SQLiteTransactionListener::OnCommit()
{
    return NOERROR;
}


ECode SQLiteTransactionListener::OnRollback()
{
    return NOERROR;
}