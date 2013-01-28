
#include "JDBCStatement.h"
#include "cmdef.h"

JDBCStatement::JDBCStatement():mConn(NULL), mRs(NULL), mMaxrows(0) {
}

JDBCStatement::~JDBCStatement() {
}

ECode JDBCStatement::Init(
    /* [in] */ IJDBCConnection* conn) {
    mConn = conn;
    mUpdcnt = 0;
    mRs = NULL;
//  this.batch = null;
    return NOERROR;
}

ECode JDBCStatement::SetFetchSize(
    /* [in] */ Int32 fetchSize)
{
    if(fetchSize != 1) {
        return E_SQL_EXCEPTION;
    }
    return NOERROR;
}

ECode JDBCStatement::GetFetchSize(
    /* [out] */ Int32* fetchSize)
{
    assert(fetchSize != NULL);
    *fetchSize = 1;
    return NOERROR;
}

ECode JDBCStatement::GetMaxRows(
    /* [out] */ Int32* maxrows)
{
    assert(maxrows != NULL);
    *maxrows = mMaxrows;
    return NOERROR;
}

ECode JDBCStatement::SetMaxRows(
    /* [in] */ Int32 maxrows)
{
    if(maxrows < 0) {
        return E_SQL_EXCEPTION;
    }
    mMaxrows = maxrows;
    return NOERROR;
}

ECode JDBCStatement::SetFetchDirection(
    /* [in] */ Int32 fetchDirection)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCStatement::GetFetchDirection(
    /* [out] */ Int32* fetchDirection)
{
    assert(fetchDirection != NULL);
    *fetchDirection = IResultSet_FETCH_UNKNOWN;
    return NOERROR;
}

ECode JDBCStatement::GetResultSetConcurrency(
    /* [out] */ Int32* resultSetConcurrency)
{
    assert(resultSetConcurrency != NULL);
    *resultSetConcurrency = IResultSet_CONCUR_READ_ONLY;
    return NOERROR;
}

ECode JDBCStatement::GetResultSetType(
    /* [out] */ Int32* resultSetType)
{
    assert(resultSetType != NULL);
    *resultSetType = IResultSet_TYPE_SCROLL_INSENSITIVE;
    return NOERROR;
}

ECode JDBCStatement::SetQueryTimeout(
    /* [in] */ Int32 seconds)
{
    // BEGIN android-changed: more closely follow specification:
    // "[throws SQLException if] this method is called on a closed Statement or the condition
    // seconds >= 0 is not satisfied"
    // (http://java.sun.com/javase/6/docs/api/java/sql/Statement.html#setQueryTimeout(int))
    Boolean closed;
    FAIL_RETURN(IsClosed(&closed));
    if (closed) {
        //throw new SQLException("can't set a query timeout on a closed statement");
        return E_SQL_EXCEPTION;
    } else if (seconds < 0) {
        //throw new SQLException("can't set a query timeout of less than 0 seconds");
        return E_SQL_EXCEPTION;
    } else if (seconds == 0) {
        // An argument of 0 seconds should set an unlimited timeout. However, since this was not
        // done previously, I assume it isn't implemented and use the same implementation.
        mConn->mTimeout = 5000;
    } else {
        mConn->mTimeout = seconds * 1000;
    }
    return NOERROR;
    // END android-changed
}

ECode JDBCStatement::GetQueryTimeout(
    /* [out] */ Int32* queryTimeout)
{
    assert( queryTimeout != NULL);
    *queryTimeout = mConn->mTimeout / 1000;
    return NOERROR;
}

ECode JDBCStatement::GetResultSet(
    /* [out] */ IResultSet** rs)
{
    *rs = mRs;
    return NOERROR;
}

ECode JDBCStatement::ExecuteQuery(
    /* [in] */ String sql, 
    /* [in] */ ArrayOf<String> args, 
    /* [in] */ Boolean updonly,
    /* [out] */ IResultSet** rs)
{
    return E_NOT_IMPLEMENTED;
}

ECode JDBCStatement::ExecuteQuery(
    /* [in] */ String sql,
    /* [out] */ IResultSet** rs)
{
    FAIL_RETURN(ExecuteQuery(sql, NULL, FALSE, rs));
    return NOERROR;
}

ECode JDBCStatement::Execute(
    /* [in] */ String sql,
    /* [out] */ Boolean* isExecute)
{
    assert(isExecute != NULL);
    AutoPtr<IResultSet>* rs;
    FAIL_RETURN(ExecuteQuery(sql, rs));
    if(*rs != NULL) {
        *isExecute = TRUE;
    }
    return NOERROR;
}

ECode JDBCStatement::Cancel()
{
//    if (conn == null || conn.db == null) {
//        throw new SQLException("stale connection");
//    }
//    conn.db.interrupt();
    return E_NOT_IMPLEMENTED;
}

ECode JDBCStatement::ClearWarnings()
{
    return E_NOT_IMPLEMENTED; 
}

ECode JDBCStatement::GetConnection(
    /* [out] */ IConnection** conn)
{
    mConn = *conn;
    return NOERROR;
}

ECode JDBCStatement::AddBatch(
    /* [in] */ String sql)
{
//    if (batch == null) {
//        batch = new ArrayList<String>(1);
//    }
//    batch.add(sql);
    return E_NOT_IMPLEMENTED;
}

ECode JDBCStatement::ExecuteBatch(
    /* [out, callee] */ ArrayOf<Int32>** ret)
{
    return E_NOT_IMPLEMENTED;
}

ECode JDBCStatement::ClearBatch()
{
    return E_NOT_IMPLEMENTED;
}

ECode JDBCStatement::Close()
{
    FAIL_RETURN(ClearBatch());
    mConn = NULL;
    return NOERROR;
}

ECode JDBCStatement::ExecuteUpdate(
    /* [in] */ String sql,
    /* [out] */ Int32* updcnt)
{
    AutoPtr<IResultSet>* rs;
    FAIL_RETURN(ExecuteQuery(sql, NULL, TRUE, rs));
    assert(updcnt != NULL);
    *updcnt = mUpdcnt;
    return NOERROR;
}

ECode JDBCStatement::GetMaxFieldSize(
    /* [out] */ Int32* maxFieldSize)
{
    assert(maxFieldSize != NULL);
    *maxFieldSize = 0;
    return NOERROR;
}

ECode JDBCStatement::GetMoreResults((
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if(mRs != NULL) {
        mRs->Close();
        mRs = NULL;
    }
    *result = FALSE;
    return NOERROR;
}

ECode JDBCStatement::GetUpdateCount(
    /* [out] */ Int32* updcnt)
{
    assert(updcnt != NULL);
    *updcnt = mUpdcnt;
    return NOERROR;    
}

//    public SQLWarning getWarnings() throws SQLException {
//    return null;
//    }

ECode JDBCStatement::SetCursorName(
    /* [in] */ String name)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;   
}

ECode JDBCStatement::SetEscapeProcessing(
    /* [in] */ Boolean enable)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCStatement::SetMaxFieldSize(
    /* [in] */ Int32 max)
{
    return E_SQL_EXCEPTION;
}

ECode JDBCStatement::GetMoreResultsEx(
    /* [in] */ Int32 x,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;   
}

ECode JDBCStatement::GetGeneratedKeys(
    /* [out] */ IResultSet** keys)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::ExecuteUpdateEx(
    /* [in] */ String sql, 
    /* [in] */ Int32 autokeys,
    /* [out] */ Int32* count)
{
    assert(count != NULL);
    if(autokeys != IStatement_NO_GENERATED_KEYS) {
        return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;   
    }
    FAIL_RETURN(ExecuteUpdate(sql, count));
    return NOERROR;
}

ECode JDBCStatement::ExecuteUpdateEx2(
    /* [in] */ String sql, 
    /* [in] */ ArrayOf<Int32> colIndexes,
    /* [out] */ Int32* count)
{
    assert(count != NULL);
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::ExecuteUpdateEx3(
    /* [in] */ String sql,
    /* [in] */ ArrayOf<String> columnNames,
    /* [out] */ Int32* result)
{
    assert(result != NULL);
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;  
}

ECode JDBCStatement::Execute2(
    /* [in] */ String sql, 
    /* [in] */ Int32 autokeys,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if(autokeys != IStatement_NO_GENERATED_KEYS) {
        return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;  
    }
    FAIL_RETURN(Execute(sql, result));
    return NOERROR;
}

ECode JDBCStatement::Execute3(
    /* [in] */ String sql, 
    /* [in] */ ArrayOf<Int32> colIndexes,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION; 
}

ECode JDBCStatement::Execute4(
    /* [in] */ String sql,
    /* [in] */ ArrayOf<String> colIndexes,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION; 
}

ECode JDBCStatement::GetResultSetHoldability(
    /* [out] */ Int32* holdability)
{
    assert(holdability != NULL);
    *holdability = IResultSet_HOLD_CURSORS_OVER_COMMIT;
    return NOERROR;
}

ECode JDBCStatement::IsClosed(
    /* [out] */ Boolean* closed)
{
    assert(closed != NULL);
    if(mConn == NULL) {
        *closed = TRUE;
    }
    return NOERROR;
}

ECode JDBCStatement::SetPoolable(
    /* [in] */ Boolean yes)
{
    if(yes) {
        return E_SQL_EXCEPTION;
    }
    return NOERROR;
}

ECode JDBCStatement::IsPoolable(
    /* [out] */ Boolean* poolable)
{
    assert(poolable != NULL);
    *poolable = FALSE;
    return NOERROR;
}