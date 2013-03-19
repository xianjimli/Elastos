#ifndef __CSTMT_H__
#define __CSTMT_H__

#include "Stmt.h"
#include "_CStmt.h"

CarClass(CStmt), public Stmt
{
public:
    CARAPI constructor();

    CARAPI Prepare(
        /** [out] **/Boolean* result);

    CARAPI Step(
        /** [out] **/Boolean* result);

    CARAPI Close();

    CARAPI Reset();

    CARAPI Clear_bindings();

    CARAPI Bind(
        /** [in] **/Int32 pos, 
        /** [in] **/Int32 value);

    CARAPI BindEx(
        /** [in] **/Int32 pos, 
        /** [in] **/Int64 value);

    CARAPI BindEx2(
        /** [in] **/Int32 pos, 
        /** [in] **/Double value);

    CARAPI BindEx3(
        /** [in] **/Int32 pos, 
        /** [in] **/const ArrayOf<Byte>& value);

    CARAPI BindEx4(
        /** [in] **/Int32 pos, 
        /** [in] **/const String& value);

    CARAPI BindEx5(
        /** [in] **/Int32 pos);

    CARAPI Bind_zeroblob(
        /** [in] **/Int32 pos, 
        /** [in] **/Int32 length);

    CARAPI Bind_parameter_count(
        /** [out] **/Int32* count);

    CARAPI Bind_parameter_name(
        /** [in] **/Int32 pos,
        /** [out] **/String* str);

    CARAPI Bind_parameter_index(
        /** [in] **/const String& name,
        /** [out] **/Int32* index);

    CARAPI Column_int(
        /** [in] **/Int32 col,
        /** [out] **/Int32* result);

    CARAPI Column_long(
        /** [in] **/Int32 col,
        /** [out] **/Int64* result);

    CARAPI Column_double(
        /** [in] **/Int32 col,
        /** [out] **/Double* result);

    CARAPI Column_bytes(
        /** [in] **/Int32 col,
        /** [out] **/ArrayOf<Byte>** bt);

    CARAPI Column_string(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    CARAPI Column_type(
        /** [in] **/Int32 col,
        /** [out] **/Int32* type);

    CARAPI Column_count(
        /** [out] **/Int32* count);

    CARAPI Column(
        /** [in] **/Int32 col,
        /** [out] **/IInterface** obj);

    CARAPI Column_table_name(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    CARAPI Column_database_name(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    CARAPI Column_decltype(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    CARAPI Column_origin_name(
        /** [in] **/Int32 col,
        /** [out] **/String* str);

    CARAPI Finalize();
};

#endif //__CSTMT_H__