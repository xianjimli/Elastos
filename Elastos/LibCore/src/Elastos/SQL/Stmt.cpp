#include "Stmt.h"

Stmt::Stmt()
{
}

ECode Stmt::Prepare(
    /** [out] **/Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}
 
ECode Stmt::Step(
    /** [out] **/Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Reset()
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Clear_bindings()
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind(
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx(
    /** [in] **/Int32 pos, 
    /** [in] **/Int64 value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx2(
    /** [in] **/Int32 pos, 
    /** [in] **/Double value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx3(
    /** [in] **/Int32 pos, 
    /** [in] **/ArrayOf<Byte>& value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx4(
    /** [in] **/Int32 pos, 
    /** [in] **/ArrayOf<String>& value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx5(
    /** [in] **/Int32 pos)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind_zeroblob(
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 length)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind_parameter_count(
    /** [out] **/Int32* count)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind_parameter_name(
    /** [in] **/Int32 pos,
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind_parameter_index(
    /** [in] **/String name,
    /** [out] **/Int32* index)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_int(
    /** [in] **/Int32 col,
    /** [out] **/Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_long(
    /** [in] **/Int32 col,
    /** [out] **/Int64* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_double(
    /** [in] **/Int32 col,
    /** [out] **/Double* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_bytes(
    /** [in] **/Int32 col,
    /** [out] **/ArrayOf<Byte>* bt)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_string(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_type(
    /** [in] **/Int32 col,
    /** [out] **/Int32* type)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_count(
    /** [out] **/Int32* count)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column(
    /** [in] **/Int32 col,
    /** [out] **/IInterface** obj)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_table_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_database_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_decltype(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_origin_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Finalize()
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Internal_init()
{
    return E_NOT_IMPLEMENTED;
}