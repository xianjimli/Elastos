#ifdef _JDBCCONNECTION_H_
#define _JDBCCONNECTION_H_

#include <elastos.h>
#include <elastos/AutoPtr.h>
using namespace Elastos;

class DatabaseX : public Database 
{

    //static Object lock = new Object();

    DatabaseX();

    ~DatabaseX();

    CARAPI Wait(
        /* [in] */Int32 ms);

    CARAPI Exec(
        /* [in] */String sql, 
        /* [in] */SQLite.Callback cb);

    CARAPI ExecEx(
        /* [in] */String sql, 
        /* [in] */SQLite.Callback cb, 
        /* [in] */ArrayOf<String> args);

    CARAPI Get_table(
        /* [in] */String sql, 
        /* [in] */ArrayOf<String> args,
        /* [out] */ITableResult** result);

    CARAPI Get_table(
        /* [in] */String sql, 
        /* [in] */ArrayOf<String> args, 
        /* [in] */ITableResult* tbl);
};


class JDBCConnection
{
protected:
    /**
     * Open database.
     */
    AutoPtr<IDatabaseX> mDb;

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
    Boolean mAutocommit = true;

    /**
     * In-transaction flag.
     * Can be true only when autocommit false.
     */
    Boolean mInt32rans = false;

    /**
     * Timeout for Database.exec()
     */
    Int3232 mTimeout = 1000000;

    /**
     * Use double/julian date representation.
     */
    Boolean mUseJulian = false;

    /**
     * File name of database.
     */

private:
    String mDbfile = null;

    /**
     * Reference to meta data or null.
     */
    AutoPtr<IJDBCDatabaseMetaData> mMeta = null;

    /**
     * Base time value for timeout handling.
     */
    Int3264 t0;

    /**
     * Database in readonly mode.
     */
    Boolean readonly = false;

    /**
     * Transaction isolation mode.
     */
    Int3232 trmode = TRANSACTION_SERIALIZABLE;

private:
    Boolean Busy0(
        /* [in] */DatabaseX db, 
        /* [in] */Int32 count);

    Open(
        /* [in] */Boolean readonly,
        /* [out] */IDatabaseX** result);

protected:
    Boolean Busy3(
        /* [in] */DatabaseX db, 
        /* [in] */Int32 count);


public:
    JDBCConnection();

    ~JDBCConnection();

    CARAPI Busy(
        /* [in] */String table, 
        /* [in] */Int32 count,
        /* [out] */Boolean* result);

    CARAPI Init(
        /* [in] */String url, 
        /* [in] */String enc, 
        /* [in] */String pwd, 
        /* [in] */String drep,
        /* [in] */String vfs);

    /* non-standard */
    CARAPI GetSQLiteDatabase(
        /* [out] */IDatabase** result);
  
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
        /* [in] */boolean ac);

    CARAPI SetCatalog(
        /* [in] */String catalog);

    CARAPI SetReadOnly(
        /* [in] */boolean ro);

    CARAPI SetTransactionIsolation(
        /* [in] */Int32 level);

//    CARAPI java.util.Map<String, Class<?>> getTypeMap();

    CARAPI SetTypeMap(
        /* [in] */java.util.Map map);
  
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

    CARAPI SetClientInfoEx(
        /* [in] */Properties prop);

    CARAPI GetClientInfo(
        /* [in] */String name,
        /* [out] */String* str);

    CARAPI GetClientInfo(
        /* [out] */IProperties** info);

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
#endif//_JDBCCONNECTION_H_