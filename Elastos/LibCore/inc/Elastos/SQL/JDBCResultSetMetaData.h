#ifdef _JDBCRESULTSETMETADATA_H_
#define _JDBCRESULTSETMETADATA_H_

#include <elastos.h>
#include <elastos/AutoPtr.h>
using namespace Elastos;

class JDBCResultSetMetaData
{
private:
    AutoPtr<IJDBCResultSet> mr;

public:
    JDBCResultSetMetaData();

    ~JDBCResultSetMetaData();

    CARAPI Init(
        /* [in] */ IJDBCResultSet* r);

    CARAPI GetCatalogName(
        /* [in] */Int32 column,
        /* [out] */String* str);

    CARAPI GetColumnClassName(
        /* [in] */Int32 column,
        /* [out] */String* str);

    CARAPI GetColumnCount(
        /* [out] */Int32* count);

    CARAPI GetColumnDisplaySize(
        /* [in] */Int32 column,
        /* [out] */Int32* size);

    CARAPI GetColumnLabel(
        /* [in] */Int32 column,
        /* [out] */String* lable);

    CARAPI GetColumnName(
        /* [in] */Int32 column,
        /* [out] */String* name);

    CARAPI GetColumnType(
        /* [in] */Int32 column,
        /* [out] */Int32* type);

    CARAPI GetColumnTypeName(
        /* [in] */Int32 column,
        /* [out] */String* name);

    CARAPI GetPrecision(
        /* [in] */Int32 column,
        /* [out] */Int32* result);

    CARAPI GetScale(
        /* [in] */Int32 column,
        /* [out] */Int32* scale);

    CARAPI GetSchemaName(
        /* [in] */Int32 column,
        /* [out] */String* name);

    CARAPI GetTableName(
        /* [in] */Int32 column,
        /* [out] */String* name);

    CARAPI IsAutoIncrement(
        /* [in] */Int32 column,
        /* [out] */Boolean* result);

    CARAPI IsCaseSensitive(
        /* [in] */Int32 column,
        /* [out] */Boolean* result);

    CARAPI IsCurrency(
        /* [in] */Int32 column,
        /* [out] */Boolean* result);

    CARAPI IsDefinitelyWritable(
        /* [in] */Int32 column,
        /* [out] */Boolean* result);

    CARAPI IsNullable(
        /* [in] */Int32 column,
        /* [out] */Int32* result);

    CARAPI IsReadOnly(
        /* [in] */Int32 column,
        /* [out] */Boolean* result);

    CARAPI IsSearchable(
        /* [in] */Int32 column,
        /* [out] */Boolean* result);

    CARAPI IsSigned(
        /* [in] */Int32 column,
        /* [out] */Boolean* result);

    CARAPI IsWritable(
        /* [in] */Int32 column,
        /* [out] */Boolean* result);

    CARAPI FindColByName(
        /* [in] */String columnName,
        /* [out] */Int32* result);

//    public <T> T unwrap(java.lang.Class<T> iface);

 //   public boolean isWrapperFor(java.lang.Class iface);
};
#endif//_JDBCRESULTSETMETADATA_H_
