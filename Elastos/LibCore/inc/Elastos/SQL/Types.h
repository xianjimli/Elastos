#ifdef _TYPE_H_
#define _TYPE_H_

#include <elastos.h>

using namespace Elastos;

class Types {
private:
    /*
     * Private constructor to prevent instantiation.
     */
    Types() {}

public:
    /**
     * The type code that identifies the SQL type {@code ARRAY}.
     */
    static const Int32 ARRAY;// 2003;

    /**
     * The type code that identifies the SQL type {@code BIGINT}.
     */
    static const Int32 BIGINT;// -5;

    /**
     * The type code that identifies the SQL type {@code BINARY}.
     */
    static const Int32 BINARY;// -2;

    /**
     * The type code that identifies the SQL type {@code BIT}.
     */
    static const Int32 BIT;// -7;

    /**
     * The type code that identifies the SQL type {@code BLOB}.
     */
    static const Int32 BLOB;// 2004;

    /**
     * The type code that identifies the SQL type {@code BOOLEAN}.
     */
    static const Int32 BOOLEAN;// 16;

    /**
     * The type code that identifies the SQL type {@code CHAR}.
     */
    static const Int32 CHAR;// 1;

    /**
     * The type code that identifies the SQL type {@code CLOB}.
     */
    static const Int32 CLOB;// 2005;

    /**
     * The type code that identifies the SQL type {@code DATALINK}.
     */
    static const Int32 DATALINK;// 70;

    /**
     * The type code that identifies the SQL type {@code DATE}.
     */
    static const Int32 DATE;// 91;

    /**
     * The type code that identifies the SQL type {@code DECIMAL}.
     */
    static const Int32 DECIMAL;// 3;

    /**
     * The type code that identifies the SQL type {@code DISTINCT}.
     */
    static const Int32 DISTINCT;// 2001;

    /**
     * The type code that identifies the SQL type {@code DOUBLE}.
     */
    static const Int32 DOUBLE;// 8;

    /**
     * The type code that identifies the SQL type {@code FLOAT}.
     */
    static const Int32 FLOAT;// 6;

    /**
     * The type code that identifies the SQL type {@code INTEGER}.
     */
    static const Int32 INTEGER;// 4;

    /**
     * The type code that identifies the SQL type {@code JAVA_OBJECT}.
     */
    static const Int32 JAVA_OBJECT;// 2000;

    /**
     * The type code that identifies the SQL type {@code LONGVARBINARY}.
     */
    static const Int32 LONGVARBINARY;// -4;

    /**
     * The type code that identifies the SQL type {@code LONGVARCHAR}.
     */
    static const Int32 LONGVARCHAR;// -1;

    /**
     * The type code that identifies the SQL type {@code NULL}.
     */
    static const Int32 TYPES_NULL;// 0;  ///////////////////////

    /**
     * The type code that identifies the SQL type {@code NUMERIC}.
     */
    static const Int32 NUMERIC;// 2;

    /**
     * The type code that identifies that the SQL type is database specific and
     * is mapped to a Java object, accessed via the methods
     * {@code getObject} and {@code setObject}.
     */
    static const Int32 OTHER;// 1111;

    /**
     * The type code that identifies the SQL type {@code REAL}.
     */
    static const Int32 REAL;// 7;

    /**
     * The type code that identifies the SQL type {@code REF}.
     */
    static const Int32 REF;// 2006;

    /**
     * The type code that identifies the SQL type {@code SMALLINT}.
     */
    static const Int32 SMALLINT;// 5;

    /**
     * The type code that identifies the SQL type {@code STRUCT}.
     */
    static const Int32 STRUCT;// 2002;

    /**
     * The type code that identifies the SQL type {@code TIME}.
     */
    static const Int32 TIME;// 92;

    /**
     * The type code that identifies the SQL type {@code TIMESTAMP}.
     */
    static const Int32 TIMESTAMP;// 93;

    /**
     * The type code that identifies the SQL type {@code TINYINT}.
     */
    static const Int32 TINYINT;// -6;

    /**
     * The type code that identifies the SQL type {@code VARBINARY}.
     */
    static const Int32 VARBINARY;// -3;

    /**
     * The type code that identifies the SQL type {@code VARCHAR}.
     */
    static const Int32 VARCHAR;// 12;

    /**
     * The type code that identifies the SQL type ROWID.
     */
    static const Int32 ROWID;// -8;

    /**
     * The type code that identifies the SQL type NCHAR.
     */
    static const Int32 NCHAR;// -15;

    /**
     * The type code that identifies the SQL type NVARCHAR.
     */
    static const Int32 NVARCHAR;// -9;

    /**
     * The type code that identifies the SQL type LONGNVARCHAR.
     */
    static const Int32 LONGNVARCHAR;// -16;

    /**
     * The type code that identifies the SQL type NCLOB.
     */
    static const Int32 NCLOB;// 2011;

    /**
     * The type code that identifies the SQL type SQLXML.
     */
    static const Int32 SQLXML;// 2009;
};
#endif//_TYPE_H_