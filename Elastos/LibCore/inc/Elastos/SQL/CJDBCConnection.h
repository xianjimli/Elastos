#ifndef _CJDBCCONNECTION_H_
#define _CJDBCCONNECTION_H_

#include <elastos.h>
#include <elastos/AutoPtr.h>
#include "Connection.h"
#include "_CJDBCConnection.h"
#include <Elastos.SQL_server.h>
//#include "Database.h"

using namespace Elastos;

//class DatabaseX : public Database
//{

    //static Object lock = new Object();
    //Mutex mSyncLock;

//    DatabaseX();

//    ~DatabaseX();

//    CARAPI Wait(
//        /* [in] */Int32 ms);

//    CARAPI Exec(
//        /* [in] */String sql, 
//        /* [in] */ICallback* cb);

//    CARAPI ExecEx(
//        /* [in] */String sql, 
//        /* [in] */ICallback* cb, 
//        /* [in] */ArrayOf<String> args);

 //   CARAPI Get_tableEx3(
//        /* [in] */String sql, 
//        /* [in] */ArrayOf<String> args,
//        /* [out] */ITableResult** result);

//    CARAPI Get_tableEx4(
//        /* [in] */String sql, 
//        /* [in] */ArrayOf<String> args, 
//        /* [in] */ITableResult* tbl);
//};

CarClass(CJDBCConnection), public Connection
{
protected:
    /**
     * Open database.
     */
 //   AutoPtr<DatabaseX> mDb;

    /**
     * Database URL.
     */
    String mUrl;

    /**
     * Character encoding.
     */
    String mEnc;

    /**
     * SQLite 3 VFS to use.
     */
    String mVfs;

    /**
     * Autocommit flag, true means autocommit.
     */
    Boolean mAutocommit;

    /**
     * In-transaction flag.
     * Can be true only when autocommit false.
     */
    Boolean mIntrans;

    /**
     * Timeout for Database.exec()
     */
    Int32 mTimeout;

    /**
     * Use double/julian date representation.
     */
    Boolean mUseJulian;

private:
    /**
     * File name of database.
     */
    String mDbfile;

    /**
     * Reference to meta data or null.
     */
    AutoPtr<IJDBCDatabaseMetaData> mMeta;

    /**
     * Base time value for timeout handling.
     */
    Int64 t0;

    /**
     * Database in readonly mode.
     */
    Boolean mReadonly;

    /**
     * Transaction isolation mode.
     */
//   Int32 trmode = TRANSACTION_SERIALIZABLE;///////////////////

private:
//    Boolean Busy0(
//        /* [in] */DatabaseX db, 
//        /* [in] */Int32 count);

//    Open(
//        /* [in] */Boolean readonly,
//        /* [out] */IDatabaseX** result);

protected:
//    Boolean Busy3(
//        /* [in] */DatabaseX db, 
//        /* [in] */Int32 count);


public:
    CJDBCConnection();

    CARAPI constructor(
        /* [in] */ String url,
        /* [in] */ String enc,
        /* [in] */ String pwd,
        /* [in] */ String drep,
        /* [in] */ String vfs);

    CARAPI Busy(
        /* [in] */String table, 
        /* [in] */Int32 count,
        /* [out] */Boolean* result);

    /* non-standard */
//    CARAPI GetSQLiteDatabase(
///        /* [out] */IDatabase** result);
  
    CARAPI CreateStatement(
        /* [out] */IStatement** result);

    CARAPI CreateStatementEx(
        /* [in] */Int32 resultSetType,
        /* [in] */Int32 resultSetConcurrency,
        /* [out] */IStatement** result);
    
    CARAPI GetMetaData(
        /* [out] */IDatabaseMetaData** result);

    CARAPI Close();

    CARAPI IsClosed(
        /* [out] */Boolean* result);

    CARAPI IsReadOnly(
        /* [out] */Boolean* result);

    CARAPI ClearWarnings();

    CARAPI Commit();

    CARAPI GetAutoCommit(
        /* [out] */Boolean* result);

    CARAPI GetCatalog(
        /* [out] */String* result);

    CARAPI GetTransactionIsolation(
        /* [out] */Int32* result);

//    CARAPI SQLWarning getWarnings();

    CARAPI NativeSQL(
        /* [in] */String sql,
        /* [out] */String* result);

    CARAPI PrepareCall(
        /* [in] */String sql,
        /* [out] */ICallableStatement** result);

    CARAPI PrepareCallx(
        /* [in] */String sql, 
        /* [in] */Int32 x, 
        /* [in] */Int32 y,
        /* [out] */ICallableStatement** result);

    CARAPI PrepareStatement(
        /* [in] */String sql,
        /* [out] */IPreparedStatement** result);

    CARAPI PrepareStatementEx(
        /* [in] */String sql, 
        /* [in] */Int32 resultSetType,
        /* [in] */Int32 resultSetConcurrency,
        /* [out] */IPreparedStatement** result);

    CARAPI Rollback();

    CARAPI SetAutoCommit(
        /* [in] */Boolean ac);

    CARAPI SetCatalog(
        /* [in] */String catalog);

    CARAPI SetReadOnly(
        /* [in] */Boolean ro);

    CARAPI SetTransactionIsolation(
        /* [in] */Int32 level);

//    CARAPI java.util.Map<String, Class<?>> getTypeMap();

 //   CARAPI SetTypeMap(
 //       /* [in] */java.util.Map map);
  
    CARAPI GetHoldability(
        /* [out] */Int32* result);

    CARAPI SetHoldability(
        /* [in] */Int32 holdability);

    CARAPI SetSavepoint(
         /* [out] */ISavepoint** result);

    CARAPI SetSavepointEx(
        /* [in] */String name,
        /* [out] */ISavepoint** result);

    CARAPI Rollback(
        /* [in] */ISavepoint* x);

    CARAPI ReleaseSavepoint(
        /* [in] */ISavepoint* x);

    CARAPI CreateStatementEx2(
        /* [in] */Int32 resultSetType,
        /* [in] */Int32 resultSetConcurrency,
        /* [in] */Int32 resultSetHoldability,
        /* [out] */IStatement** result);

    CARAPI PrepareStatementEx2(
        /* [in] */String sql, 
        /* [in] */Int32 resultSetType,
        /* [in] */Int32 resultSetConcurrency,
        /* [in] */Int32 resultSetHoldability,
        /* [out] */IPreparedStatement** result);

    CARAPI PrepareCallEx(
        /* [in] */String sql, 
        /* [in] */Int32 x, 
        /* [in] */Int32 y, 
        /* [in] */Int32 z,
        /* [out] */ICallableStatement** result);

    CARAPI PrepareStatementEx3(
        /* [in] */String sql, 
        /* [in] */Int32 autokeys,
        /* [out] */IPreparedStatement** result);

    CARAPI PrepareStatementEx4(
        /* [in] */String sql, 
        /* [in] */ArrayOf<Int32> colIndexes,
        /* [out] */IPreparedStatement** result);

    CARAPI PrepareStatementEx5(
        /* [in] */String sql, 
        /* [in] */ArrayOf<String> columns,
        /* [out] */IPreparedStatement** result);

    CARAPI  CreateClob(
        /* [out] */IClob** result);

    CARAPI CreateBlob(
        /* [out] */IBlob** result);

    CARAPI CreateNClob(
        /* [out] */INClob** result);

    CARAPI CreateSQLXML(
        /* [out] */ISQLXML** result);

    CARAPI IsValid(
        /* [in] */Int32 timeout,
        /* [out] */Boolean* result);

    CARAPI SetClientInfo(
        /* [in] */String name, 
        /* [in] */String value);

//    CARAPI SetClientInfoEx(
//        /* [in] */IProperties* prop);

    CARAPI GetClientInfo(
        /* [in] */String name,
        /* [out] */String* str);

//    CARAPI GetClientInfo(
 //       /* [out] */IProperties** info);

    CARAPI CreateArrayOf(
        /* [in] */String type, 
        /* [in] */ArrayOf<IInterface*> elems,
        /* [out] */IArray** result);

//    CARAPI Struct createStruct(
//       /* [in] */String type, 
//        /* [in] */ArrayOf<IInterface*> attrs);

//    CARAPI <T> T unwrap(
//        /* [in] */java.lang.Class<T> iface);

//    CARAPI boolean isWrapperFor(
//        /* [in] */java.lang.Class iface);

};
#endif //_JDBCCONNECTION_H_
