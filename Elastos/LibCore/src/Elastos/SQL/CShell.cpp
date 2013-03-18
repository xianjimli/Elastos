#include "cmdef.h"
#include "CShell.h"

ECode CShell::Set_table_name(
    /* [in] */ String str)
{
    return Shell::Set_table_name(str);
}

ECode CShell::Do_meta(
    /* [in] */ String line)
{
    return Shell::Do_meta(line);
}

ECode CShell::Read_line(
    /* [in] */ IBufferedReader* is, 
    /* [in] */ String prompt,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    
    return Shell::Read_line(is, prompt, str);
}

ECode CShell::Do_input(
    /* [in] */IBufferedReader* is)
{
    return Shell::Do_input(is);
}

ECode CShell::Do_cmd(
    /* [in] */ String sql)
{
    return Shell::Do_cmd(sql);
}