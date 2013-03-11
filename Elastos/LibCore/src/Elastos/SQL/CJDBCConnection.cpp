#include "JDBCStatement.h"
#include "CJDBCConnection.h"
#include "cmdef.h"

//DatabaseX::DatabaseX()
//{
//}

//DatabaseX::~DatabaseX()
//{
//}

//ECode DatabaseX::Wait(
//        /* [in] */Int32 ms)
//{
//}

//ECode DatabaseX::Exec(
//    /* [in] */String sql, 
//    /* [in] */SQLite.Callback cb)
//{
//}

//ECode DatabaseX::ExecEx(
//    /* [in] */String sql, 
//    /* [in] */SQLite.Callback cb, 
//    /* [in] */ArrayOf<String> args)
//{
//}

//ECode DatabaseX::Get_table(
//    /* [in] */String sql, 
//    /* [in] */ArrayOf<String> args,
//    /* [out] */ITableResult** result)
//{
//}

//ECode DatabaseX::Get_table(
//    /* [in] */String sql, 
//    /* [in] */ArrayOf<String> args, 
//    /* [in] */ITableResult* tbl)
//{
//}

//Boolean CJDBCConnection::Busy0(
//        /* [in] */DatabaseX db, 
//        /* [in] */Int32 count)
//{
    /*
    if (count <= 1) {
        t0 = System.currentTimeMillis();
    }
    if (db != null) {
        long t1 = System.currentTimeMillis();
        if (t1 - t0 > timeout) {
        return false;
        }
        db.wait(100);
        return true;
    }
    return false;*/
//    return NOERROR;
//}

//Boolean CJDBCConnection::Busy3(
//    /* [in] */DatabaseX db, 
//    /* [in] */Int32 count)
//{
    /*
    if (count <= 1) {
        t0 = System.currentTimeMillis();
    }
    if (db != null) {
        long t1 = System.currentTimeMillis();
        if (t1 - t0 > timeout) {
        return false;
        }
        return true;
    }
    return false;*/
//    return NOERROR;
//}

//CJDBCConnection::Open(
//    /* [in] */Boolean readonly,
//    /* [out] */IDatabaseX** result)
//{
    /*
    DatabaseX dbx = null;
    try {
        dbx = new DatabaseX();
        dbx.open(dbfile, readonly ? SQLite.Constants.SQLITE_OPEN_READONLY :
             (SQLite.Constants.SQLITE_OPEN_READWRITE |
              SQLite.Constants.SQLITE_OPEN_CREATE), vfs);
        dbx.set_encoding(enc);
    } catch (SQLite.Exception e) {
        throw new SQLException(e.toString());
    }
    int loop = 0;
    while (true) {
        try {
        dbx.exec("PRAGMA short_column_names = off;", null);
        dbx.exec("PRAGMA full_column_names = on;", null);
        dbx.exec("PRAGMA empty_result_callbacks = on;", null);
        if (SQLite.Database.version().compareTo("2.6.0") >= 0) {
            dbx.exec("PRAGMA show_datatypes = on;", null);
        }
        } catch (SQLite.Exception e) {
        if (dbx.last_error() != SQLite.Constants.SQLITE_BUSY ||
            !busy0(dbx, ++loop)) {
            try {
            dbx.close();
            } catch (SQLite.Exception ee) {
            }
            throw new SQLException(e.toString());
        }
        continue;
        }
        break;
    }
    return dbx;*/
//    return NOERROR;
//}

CJDBCConnection::CJDBCConnection()
    :mAutocommit(TRUE)
    ,mIntrans(FALSE)
    ,mTimeout(1000000)
    ,mUseJulian(FALSE)
    ,mDbfile(NULL)
    ,mReadonly(FALSE)
{

}
ECode CJDBCConnection::constructor(
    /* [in] */ String url,
    /* [in] */ String enc,
    /* [in] */ String pwd,
    /* [in] */ String drep,
    /* [in] */ String vfs)
    
{
    /*
    if (url.startsWith("sqlite:/")) {
        dbfile = url.substring(8);
    } else if (url.startsWith("jdbc:sqlite:/")) {
        dbfile = url.substring(13);
    } else {
        throw new SQLException("unsupported url");
    }
    this.url = url;
    this.enc = enc;
    this.vfs = vfs;
    try {
        db = open(readonly);
        try {
        if (pwd != null && pwd.length() > 0) {
            db.key(pwd);
        }
        } catch (SQLite.Exception se) {
        throw new SQLException("error while setting key");
        }
        db.busy_handler(this);
    } catch (SQLException e) {
        if (db != null) {
        try {
            db.close();
        } catch (SQLite.Exception ee) {
        }
        }
        throw e;
    }
    useJulian = drep != null &&
        (drep.startsWith("j") || drep.startsWith("J"));*/
    return NOERROR;
}


ECode CJDBCConnection::Busy(
    /* [in] */String table, 
    /* [in] */Int32 count,
    /* [out] */Boolean* result)
{
    //return busy0(db, count);
    return NOERROR;
}

/* non-standard */
//ECode CJDBCConnection::GetSQLiteDatabase(
//    /* [out] */IDatabase** result)
//{
    //return (SQLite.Database) db;
//    return NOERROR;
//}

ECode CJDBCConnection::CreateStatement(
    /* [out] */IStatement** result)
{/*
    JDBCStatement s = new JDBCStatement(this);
    return s;*/
    return NOERROR;
}

ECode CJDBCConnection::CreateStatementEx(
    /* [in] */Int32 resultSetType,
    /* [in] */Int32 resultSetConcurrency,
    /* [out] */IStatement** result)
{/*
    if (resultSetType != ResultSet.TYPE_FORWARD_ONLY &&
        resultSetType != ResultSet.TYPE_SCROLL_INSENSITIVE &&
        resultSetType != ResultSet.TYPE_SCROLL_SENSITIVE) {
        throw new SQLFeatureNotSupportedException("unsupported result set type");
    }
    if (resultSetConcurrency != ResultSet.CONCUR_READ_ONLY &&
        resultSetConcurrency != ResultSet.CONCUR_UPDATABLE) {
        throw new SQLFeatureNotSupportedException("unsupported result set concurrency");
    }
    JDBCStatement s = new JDBCStatement(this);
    return s;*/
    return NOERROR;
}

ECode CJDBCConnection::GetMetaData(
    /* [out] */IDatabaseMetaData** result)
{/*
    if (meta == null) {
        meta = new JDBCDatabaseMetaData(this);
    }
    return meta;*/
    return NOERROR;
}

ECode CJDBCConnection::Close()
{/*
    try {
        rollback();
    } catch (SQLException e) {
        // ignored
    }
    intrans = false;
    if (db != null) {
        try {
        db.close();
        db = null;
        } catch (SQLite.Exception e) {
        throw new SQLException(e.toString());
        }
    }*/
    return NOERROR;
}

ECode CJDBCConnection::IsClosed(
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
 //   *result = mDb == NULL;
    return NOERROR;
}

ECode CJDBCConnection::IsReadOnly(
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result = mReadonly;
    return NOERROR;
}

ECode CJDBCConnection::ClearWarnings()
{
    return NOERROR;
}

ECode CJDBCConnection::Commit()
{/*
    if (db == null) {
        throw new SQLException("stale connection");
    }
    if (!intrans) {
        return;
    }
    try {
        db.exec("COMMIT", null);
        intrans = false;
    } catch (SQLite.Exception e) {
        throw new SQLException(e.toString());
    }*/
    return NOERROR;
}

ECode CJDBCConnection::GetAutoCommit(
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result = mAutocommit;
    return NOERROR;
}

ECode CJDBCConnection::GetCatalog(
    /* [out] */String* result)
{
    assert(result!=NULL);
    result = NULL;
    return NOERROR;
}

ECode CJDBCConnection::GetTransactionIsolation(
    /* [out] */Int32* result)
{
    assert(result!=NULL);
 //   *result = mTrmode;
    return NOERROR;
}

//    ECode CJDBCConnection::SQLWarning getWarnings(){}

ECode CJDBCConnection::NativeSQL(
    /* [in] */String sql,
    /* [out] */String* result)
{
    return E_SQL_EXCEPTION;
}

ECode CJDBCConnection::PrepareCall(
    /* [in] */String sql,
    /* [out] */ICallableStatement** result)
{
    return E_SQL_EXCEPTION;
}

ECode CJDBCConnection::PrepareCallx(
    /* [in] */String sql, 
    /* [in] */Int32 x, 
    /* [in] */Int32 y,
    /* [out] */ICallableStatement** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::PrepareStatement(
    /* [in] */String sql,
    /* [out] */IPreparedStatement** result)
{/*
    JDBCPreparedStatement s = new JDBCPreparedStatement(this, sql);
    return s;*/
    return NOERROR;
}

ECode CJDBCConnection::PrepareStatementEx(
    /* [in] */String sql, 
    /* [in] */Int32 resultSetType,
    /* [in] */Int32 resultSetConcurrency,
    /* [out] */IPreparedStatement** result)
{/*
    if (resultSetType != ResultSet.TYPE_FORWARD_ONLY &&
        resultSetType != ResultSet.TYPE_SCROLL_INSENSITIVE &&
        resultSetType != ResultSet.TYPE_SCROLL_SENSITIVE) {
        throw new SQLFeatureNotSupportedException("unsupported result set type");
    }
    if (resultSetConcurrency != ResultSet.CONCUR_READ_ONLY &&
        resultSetConcurrency != ResultSet.CONCUR_UPDATABLE) {
        throw new SQLFeatureNotSupportedException("unsupported result set concurrency");
    }
    JDBCPreparedStatement s = new JDBCPreparedStatement(this, sql);
    return s;*/
    return NOERROR;
}

ECode CJDBCConnection::Rollback()
{/*
    if (db == null) {
        throw new SQLException("stale connection");
    }
    if (!intrans) {
        return;
    }
    try {
        db.exec("ROLLBACK", null);
        intrans = false;
    } catch (SQLite.Exception e) {
        throw new SQLException(e.toString());
    }*/
    return NOERROR;
}

ECode CJDBCConnection::SetAutoCommit(
    /* [in] */Boolean ac)
{/*
    if (ac && intrans && db != null) {
        try {
        db.exec("ROLLBACK", null);
        } catch (SQLite.Exception e) {
        throw new SQLException(e.toString());
        } finally {
        intrans = false;
        }
    }
    autocommit = ac;*/
    return NOERROR;
}

ECode CJDBCConnection::SetCatalog(
    /* [in] */String catalog)
{
    return NOERROR;
}

ECode CJDBCConnection::SetReadOnly(
    /* [in] */Boolean ro)
{/*
    if (intrans) {
        throw new SQLException("incomplete transaction");
    }
    if (ro != readonly) {
        DatabaseX dbx = null;
        try {
        dbx = open(ro);
        db.close();
        db = dbx;
        dbx = null;
        readonly = ro;
        } catch (SQLException e) {
        throw e;
        } catch (SQLite.Exception ee) {
        if (dbx != null) {
            try {
            dbx.close();
            } catch (SQLite.Exception eee) {
            }
        }
        throw new SQLException(ee.toString());
        }
    }*/
    return NOERROR;
}

ECode CJDBCConnection::SetTransactionIsolation(
    /* [in] */Int32 level)
{/*
    if (db.is3() && SQLite.JDBCDriver.sharedCache) {
        String flag = null;
        if (level == TRANSACTION_READ_UNCOMMITTED &&
        trmode != TRANSACTION_READ_UNCOMMITTED) {
        flag = "on";
        } else if (level == TRANSACTION_SERIALIZABLE &&
               trmode != TRANSACTION_SERIALIZABLE) {
        flag = "off";
        }
        if (flag != null) {
        try {
            db.exec("PRAGMA read_uncommitted = " + flag + ";", null);
            trmode = level;
        } catch (java.lang.Exception e) {
        }
        }
    }
    if (level != trmode) {
        throw new SQLException("not supported");
    }*/
    return NOERROR;
}

//    ECode CJDBCConnection::java.util.Map<String, Class<?>> getTypeMap(){}

//ECode CJDBCConnection::SetTypeMap(
//    /* [in] */java.util.Map map)
//{
//    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
//}

ECode CJDBCConnection::GetHoldability(
    /* [out] */Int32* result)
{
    //return ResultSet.HOLD_CURSORS_OVER_COMMIT;
    return NOERROR;
}

ECode CJDBCConnection::SetHoldability(
    /* [in] */Int32 holdability)
{/*
    if (holdability == ResultSet.HOLD_CURSORS_OVER_COMMIT) {
        return;
    }
    throw new SQLFeatureNotSupportedException("unsupported holdability");*/
    return NOERROR;
}

ECode CJDBCConnection::SetSavepoint(
     /* [out] */ISavepoint** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::SetSavepointEx(
    /* [in] */String name,
    /* [out] */ISavepoint** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::Rollback(
    /* [in] */ISavepoint* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::ReleaseSavepoint(
    /* [in] */ISavepoint* x)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::CreateStatementEx2(
    /* [in] */Int32 resultSetType,
    /* [in] */Int32 resultSetConcurrency,
    /* [in] */Int32 resultSetHoldability,
    /* [out] */IStatement** result)
{/*
    if (resultSetHoldability != ResultSet.HOLD_CURSORS_OVER_COMMIT) {
        throw new SQLFeatureNotSupportedException("unsupported holdability");
    }
    return createStatement(resultSetType, resultSetConcurrency);*/
    return NOERROR;
}

ECode CJDBCConnection::PrepareStatementEx2(
    /* [in] */String sql, 
    /* [in] */Int32 resultSetType,
    /* [in] */Int32 resultSetConcurrency,
    /* [in] */Int32 resultSetHoldability,
    /* [out] */IPreparedStatement** result)
{/*
    if (resultSetHoldability != ResultSet.HOLD_CURSORS_OVER_COMMIT) {
        throw new SQLFeatureNotSupportedException("unsupported holdability");
    }
    return prepareStatement(sql, resultSetType, resultSetConcurrency);*/
    return NOERROR;
}

ECode CJDBCConnection::PrepareCallEx(
    /* [in] */String sql, 
    /* [in] */Int32 x, 
    /* [in] */Int32 y, 
    /* [in] */Int32 z,
    /* [out] */ICallableStatement** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::PrepareStatementEx3(
    /* [in] */String sql, 
    /* [in] */Int32 autokeys,
    /* [out] */IPreparedStatement** result)
{/*
    if (autokeys != Statement.NO_GENERATED_KEYS) {
        throw new SQLFeatureNotSupportedException("generated keys not supported");
    }
    return prepareStatement(sql);*/
    return NOERROR;
}

ECode CJDBCConnection::PrepareStatementEx4(
    /* [in] */String sql, 
    /* [in] */ArrayOf<Int32> colIndexes,
    /* [out] */IPreparedStatement** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::PrepareStatementEx5(
    /* [in] */String sql, 
    /* [in] */ArrayOf<String> columns,
    /* [out] */IPreparedStatement** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::CreateClob(
    /* [out] */IClob** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::CreateBlob(
    /* [out] */IBlob** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::CreateNClob(
    /* [out] */INClob** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::CreateSQLXML(
    /* [out] */ISQLXML** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode CJDBCConnection::IsValid(
    /* [in] */Int32 timeout,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result = TRUE;
    return NOERROR;
}

ECode CJDBCConnection::SetClientInfo(
    /* [in] */String name, 
    /* [in] */String value)
{
    return E_SQL_CLIENT_INFO_EXCEPTION;
}

//ECode CJDBCConnection::SetClientInfoEx(
//    /* [in] */Properties prop)
//{
 //   return E_SQL_CLIENT_INFO_EXCEPTION;
//}

ECode CJDBCConnection::GetClientInfo(
    /* [in] */String name,
    /* [out] */String* str)
{
    return E_SQL_EXCEPTION;
}

//ECode CJDBCConnection::GetClientInfo(
//    /* [out] */IProperties** info)
//{
//    //return new Properties();
//    return NOERROR;
//}

ECode CJDBCConnection::CreateArrayOf(
    /* [in] */String type, 
    /* [in] */ArrayOf<IInterface*> elems,
    /* [out] */IArray** result)
{
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}