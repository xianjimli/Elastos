#ifndef _JDBCSTATEMENT_H_
#define _JDBCSTATEMENT_H_

#include <elastos.h>
#include <Elastos.SQL_server.h>
#include <elastos/AutoPtr.h>
#include "CJDBCConnection.h"
#include "Statement.h"

using namespace Elastos;

class JDBCStatement : public Statement
{
protected:
    AutoPtr<IJDBCConnection> mConn;
    AutoPtr<IJDBCResultSet> mRs;
    Int32 mUpdcnt;
    Int32 mMaxrows;
    
private:
    //private ArrayList<String> batch;

protected:
    CARAPI ExecuteQuery(
        /* [in] */ String sql, 
        /* [in] */ ArrayOf<String> args, 
        /* [in] */ Boolean updonly,
        /* [out] */ IResultSet** rs);
public:
    JDBCStatement();

    ~JDBCStatement();

    CARAPI Init(
        /* [in] */ IJDBCConnection* conn);

    CARAPI SetFetchSize(
        /* [in] */ Int32 fetchSize);

    CARAPI GetFetchSize(
        /* [out] */ Int32* fetchSize);

    CARAPI GetMaxRows(
        /* [out] */ Int32* maxrows);

    CARAPI SetMaxRows(
        /* [in] */ Int32 maxrows);

    CARAPI SetFetchDirection(
        /* [in] */ Int32 fetchDirection);

    CARAPI GetFetchDirection(
        /* [out] */ Int32* fetchDirection);

    CARAPI GetResultSetConcurrency(
        /* [out] */ Int32* resultSetConcurrency);

    CARAPI GetResultSetType(
        /* [out] */ Int32* resultSetType);

    CARAPI SetQueryTimeout(
        /* [in] */ Int32 seconds);

    CARAPI GetQueryTimeout(
        /* [out] */ Int32* queryTimeout);

    CARAPI GetResultSet(
        /* [out] */ IResultSet** rs);

    CARAPI ExecuteQuery(
        /* [in] */ String sql,
        /* [out] */ IResultSet** rs);

    CARAPI Execute(
        /* [in] */ String sql,
        /* [out] */ Boolean* isExecute);

    CARAPI Cancel();

    CARAPI ClearWarnings();

    CARAPI GetConnection(
        /* [out] */ IConnection** conn);

    CARAPI AddBatch(
        /* [in] */ String sql);

    CARAPI ExecuteBatch(
        /* [out, callee] */ ArrayOf<Int32>** ret);

    CARAPI ClearBatch();

    CARAPI Close();

    CARAPI ExecuteUpdate(
        /* [in] */ String sql,
        /* [out] */ Int32* updcnt);

    CARAPI GetMaxFieldSize(
        /* [out] */ Int32* maxFieldSize);

    CARAPI GetMoreResults(
        /* [out] */ Boolean* result);

    CARAPI GetUpdateCount(
        /* [out] */ Int32* updcnt);

//    public SQLWarning getWarnings() throws SQLException {
//    return null;
//    }

    CARAPI SetCursorName(
        /* [in] */ String name);

    CARAPI SetEscapeProcessing(
        /* [in] */ Boolean enable);

    CARAPI SetMaxFieldSize(
        /* [in] */ Int32 max);

    CARAPI GetMoreResultsEx(
        /* [in] */ Int32 x,
        /* [out] */ Boolean* result);

    CARAPI GetGeneratedKeys(
        /* [out] */ IResultSet** keys);

    CARAPI ExecuteUpdateEx(
        /* [in] */ String sql, 
        /* [in] */ Int32 autokeys,
        /* [out] */ Int32* count);


    CARAPI ExecuteUpdateEx2(
        /* [in] */ String sql, 
        /* [in] */ ArrayOf<Int32> colIndexes,
        /* [out] */ Int32* count);

    CARAPI ExecuteUpdateEx3(
        /* [in] */ String sql,
        /* [in] */ ArrayOf<String> columnNames,
        /* [out] */ Int32* result);

    CARAPI Execute2(
        /* [in] */ String sql, 
        /* [in] */ Int32 autokeys,
        /* [out] */ Boolean* result);

    CARAPI Execute3(
        /* [in] */ String sql, 
        /* [in] */ ArrayOf<Int32> colIndexes,
        /* [out] */ Boolean* result);

    CARAPI Execute4(
        /* [in] */ String sql,
        /* [in] */ ArrayOf<String> colIndexes,
        /* [out] */ Boolean* result);

    CARAPI GetResultSetHoldability(
        /* [out] */ Int32* holdability);

    CARAPI IsClosed(
        /* [out] */ Boolean* closed);

    CARAPI SetPoolable(
        /* [in] */ Boolean yes);

    CARAPI IsPoolable(
        /* [out] */ Boolean* poolable);

//    public <T> T unwrap(java.lang.Class<T> iface) throws SQLException {
//     throw new SQLException("unsupported");
//    }

//    public boolean isWrapperFor(java.lang.Class iface) throws SQLException {
//    return false;
//    }

};
#endif