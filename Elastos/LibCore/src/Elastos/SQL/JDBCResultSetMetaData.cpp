
#include "JDBCResultSetMetaData.h"
#include "cmdef.h"

JDBCResultSetMetaData::JDBCResultSetMetaData():mr(NULL){
}

JDBCResultSetMetaData::~JDBCResultSetMetaData() {
}

ECode Init(
    /* [in] */ IJDBCResultSet* r)
{
    mr = r;
    return NOERROR;
}

ECode GetCatalogName(
    /* [in] */Int32 column,
    /* [out] */String* str)
{
    assert(str!=NULL);
    str = NULL;
    return NOERROR;
}

ECode GetColumnClassName(
    /* [in] */Int32 column,
    /* [out] */String* str)
{
    assert(str!=NULL);
/*    column--;
    if (r != null && r.tr != null) {
        if (column < 0 || column >= r.tr.ncolumns) {
        return null;
        }
        if (r.tr instanceof TableResultX) {
        switch (((TableResultX) r.tr).sql_type[column]) {
        case Types.SMALLINT:    return "java.lang.Short";
        case Types.INTEGER:    return "java.lang.Integer";
        case Types.REAL:
        case Types.DOUBLE:    return "java.lang.Double";
        case Types.FLOAT:    return "java.lang.Float";
        case Types.BIGINT:    return "java.lang.Long";
        case Types.DATE:    return "java.sql.Date";
        case Types.TIME:    return "java.sql.Time";
        case Types.TIMESTAMP:    return "java.sql.Timestamp";
        case Types.BINARY:
        case Types.VARBINARY:    return "[B";

        }
        }
        return "java.lang.String"; 
    }*/
    str=NULL;
    return NOERROR;
}

ECode GetColumnCount(
    /* [out] */Int32* count)
{
    assert(count!=NULL);
    if (mr != NULL && mr.tr != NULL) {
        count = mr.tr.ncolumns;
        return NOERROR;
    }
    count = 0;
    return NOERROR;
}

ECode GetColumnDisplaySize(
    /* [in] */Int32 column,
    /* [out] */Int32* size)
{
    assert(size!=NULL);
    size = 0;
    return NOERROR;
}

ECode GetColumnLabel(
    /* [in] */Int32 column,
    /* [out] */String* lable)
{
    assert(lable!=NULL);
    column--;
    String c = NULL;
    if (mr != NULL && mr.tr != NULL) {
        if (column < 0 || column >= mr.tr.ncolumns) {
            *lable = c;
            return NOERROR;
        }
        c = mr.tr.column[column];
    }
    *lable = c;
    return NOERROR;
}

ECode GetColumnName(
    /* [in] */Int32 column,
    /* [out] */String* name)
{
    assert(name!=NULL);
    column--;
    String c = NULL;
    if (mr != NULL && mr.tr != NULL) {
        if (column < 0 || column >= mr.tr.ncolumns) {
            *name = c;
            return NOERROR;
        }
        c = mr.tr.column[column];
        if (c != NULL) {
            Int32 i = c.IndexOf('.');
            if (i > 0) {
                *name =  c.Substring(i + 1);
                return NOERROR;
            }
        }
    }
    *name = c;
    return NOERROR;
}

ECode GetColumnType(
    /* [in] */Int32 column,
    /* [out] */Int32* type)
{/*
    column--;
    if (r != null && r.tr != null) {
        if (column >= 0 && column < r.tr.ncolumns) {
        if (r.tr instanceof TableResultX) {
            return ((TableResultX) r.tr).sql_type[column];
        }
        return Types.VARCHAR;
        }
    }
    throw new SQLException("bad column index");*/
    return E_NOT_IMPLEMENTED;
}

ECode GetColumnTypeName(
    /* [in] */Int32 column,
    /* [out] */String* name)
{/*
    column--;
    if (r != null && r.tr != null) {
        if (column >= 0 && column < r.tr.ncolumns) {
        if (r.tr instanceof TableResultX) {
            switch (((TableResultX) r.tr).sql_type[column]) {
            case Types.SMALLINT:    return "smallint";
            case Types.INTEGER:        return "integer";
            case Types.DOUBLE:        return "double";
            case Types.FLOAT:        return "float";
            case Types.BIGINT:        return "bigint";
            case Types.DATE:        return "date";
            case Types.TIME:        return "time";
            case Types.TIMESTAMP:    return "timestamp";
            case Types.BINARY:        return "binary";
            case Types.VARBINARY:    return "varbinary";
            case Types.REAL:        return "real";
            }
        }
        return "varchar";
        }
    }
    throw new SQLException("bad column index");*/
    return E_NOT_IMPLEMENTED;
}

ECode GetPrecision(
    /* [in] */Int32 column,
    /* [out] */Int32* result)
{
    assert(result!=NULL);
    result = 0;
    return NOERROR;
}

ECode GetScale(
    /* [in] */Int32 column,
    /* [out] */Int32* scale)
{
    assert(scale!=NULL);
    scale = 0;
    return NOERROR;
}

ECode GetSchemaName(
    /* [in] */Int32 column,
    /* [out] */String* name)
{
    assert(name!=NULL);
    name = NULL;
    return NOERROR;
}

ECode GetTableName(
    /* [in] */Int32 column,
    /* [out] */String* name)
{
    assert(name!=NULL);
    column--;
    String c = NULL;
    if (mr != NULL && mr.tr != NULL) {
        if (column < 0 || column >= mr.tr.ncolumns) {
            *name = c;
            return NOERROR;
        }
        c = mr.tr.column[column];
        if (c != NULL) {
        Int32 i = c.IndexOf('.');
        if (i > 0) {
            StringBuf_<50> sub;
            *name = c.Substring(0, i,sub);
            return NOERROR;
        }
        c = NULL;
        }
    }
    *name = c;
    return NOERROR;
}

ECode IsAutoIncrement(
    /* [in] */Int32 column,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result  = FALSE;
    return NOERROR;
}

ECode IsCaseSensitive(
    /* [in] */Int32 column,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result  = FALSE;
    return NOERROR;
}

ECode IsCurrency(
    /* [in] */Int32 column,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result  = FALSE;
    return NOERROR;
}

ECode IsDefinitelyWritable(
    /* [in] */Int32 column,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result  = TRUE;
    return NOERROR;
}

ECode IsNullable(
    /* [in] */Int32 column,
    /* [out] */Int32* result)
{
    assert(result!=NULL);
   // *result = columnNullableUnknown; ??????????????
    return NOERROR;
}

ECode IsReadOnly(
    /* [in] */Int32 column,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result  = FALSE;
    return NOERROR;
}

ECode IsSearchable(
    /* [in] */Int32 column,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result  = TRUE;
    return NOERROR;
}

ECode IsSigned(
    /* [in] */Int32 column,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result  = FALSE;
    return NOERROR;
}

ECode IsWritable(
    /* [in] */Int32 column,
    /* [out] */Boolean* result)
{
    assert(result!=NULL);
    *result  = TRUE;
    return NOERROR;
}

ECode FindColByName(
    /* [in] */String columnName,
    /* [out] */Int32* result)
{/*
    String c = null;
    if (r != null && r.tr != null) {
        for (int i = 0; i < r.tr.ncolumns; i++) {
            c = r.tr.column[i];
            if (c != null) {
                if (c.compareToIgnoreCase(columnName) == 0) {
                    return i + 1;
                }
                int k = c.indexOf('.');
                if (k > 0) {
                    c = c.substring(k + 1);
                    if (c.compareToIgnoreCase(columnName) == 0) {
                        return i + 1;
                    }
                }
            }
            c = null;
        }
    }
    throw new SQLException("column " + columnName + " not found");*/
    return E_NOT_IMPLEMENTED;
}