#include "cmdef.h"
#include "CStmt.h"

ECode CStmt::constructor()
{
    return NOERROR;
}

ECode CStmt::Prepare(
    /** [out] **/Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return Stmt::Prepare(result);
}

ECode CStmt::Step(
    /** [out] **/Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return Stmt::Step(result);
}

ECode CStmt::Close()
{
    return Stmt::Close();
}

ECode CStmt::Reset()
{
    return Stmt::Reset();
}

ECode CStmt::Clear_bindings()
{
    return Stmt::Clear_bindings();
}

ECode CStmt::Bind(
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 value)
{
    return Stmt::Bind(pos, value);
}

ECode CStmt::BindEx(
    /** [in] **/Int32 pos, 
    /** [in] **/Int64 value)
{
    return Stmt::BindEx(pos, value);
}

ECode CStmt::BindEx2(
    /** [in] **/Int32 pos, 
    /** [in] **/Double value)
{
    return Stmt::BindEx2(pos, value);
}

ECode CStmt::BindEx3(
    /** [in] **/Int32 pos, 
    /** [in] **/const ArrayOf<Byte>& value)
{
    return Stmt::BindEx3(pos, value);
}

ECode CStmt::BindEx4(
    /** [in] **/Int32 pos, 
    /** [in] **/const String& value)
{
    return Stmt::BindEx4(pos, value);
}

ECode CStmt::BindEx5(
    /** [in] **/Int32 pos)
{
    return Stmt::BindEx5(pos);
}

ECode CStmt::Bind_zeroblob(
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 length)
{
    return Stmt::Bind_zeroblob(pos, length);
}

ECode CStmt::Bind_parameter_count(
    /** [out] **/Int32* count)
{
    VALIDATE_NOT_NULL(count);

    return Stmt::Bind_parameter_count(count);
}

ECode CStmt::Bind_parameter_name(
    /** [in] **/Int32 pos,
    /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Stmt::Bind_parameter_name(pos, str);
}

ECode CStmt::Bind_parameter_index(
    /** [in] **/const String& name,
    /** [out] **/Int32* index)
{
    VALIDATE_NOT_NULL(index);

    return Stmt::Bind_parameter_index(name, index);
}

ECode CStmt::Column_int(
    /** [in] **/Int32 col,
    /** [out] **/Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return Stmt::Column_int(col, result);
}

ECode CStmt::Column_long(
    /** [in] **/Int32 col,
    /** [out] **/Int64* result)
{
    VALIDATE_NOT_NULL(result);

    return Stmt::Column_long(col, result);
}

ECode CStmt::Column_double(
    /** [in] **/Int32 col,
    /** [out] **/Double* result)
{
    VALIDATE_NOT_NULL(result);

    return Stmt::Column_double(col, result);
}

ECode CStmt::Column_bytes(
    /** [in] **/Int32 col,
    /** [out] **/ArrayOf<Byte>** bt)
{
    return Stmt::Column_bytes(col, bt);
}

ECode CStmt::Column_string(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Stmt::Column_string(col, str);
}

ECode CStmt::Column_type(
    /** [in] **/Int32 col,
    /** [out] **/Int32* type)
{
    VALIDATE_NOT_NULL(type);

    return Stmt::Column_type(col, type);
}

ECode CStmt::Column_count(
    /** [out] **/Int32* count)
{
    VALIDATE_NOT_NULL(count);

    return Stmt::Column_count(count);
}

ECode CStmt::Column(
    /** [in] **/Int32 col,
    /** [out] **/IInterface** obj)
{
    return Stmt::Column(col, obj);
}

ECode CStmt::Column_table_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Stmt::Column_table_name(col, str);
}

ECode CStmt::Column_database_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Stmt::Column_database_name(col, str);
}

ECode CStmt::Column_decltype(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Stmt::Column_decltype(col, str);
}

ECode CStmt::Column_origin_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    VALIDATE_NOT_NULL(str);

    return Stmt::Column_origin_name(col, str);
}

ECode CStmt::Finalize()
{
    return Stmt::Finalize();
}