#include "cmdef.h"
#include "CShell.h"

ECode CShell::constructor(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ IPrintWriter* err)
{
    return Shell::Init(pw, err);
}

ECode CShell::constructor(
    /* [in] */ IPrintStream* pw,
    /* [in] */ IPrintStream* err)
{
    return Shell::Init(pw, err);
}

ECode CShell::Clone(
    /* [out] */IInterface** obj)
{
    return Shell::Clone(obj);
}

ECode CShell::Set_table_name(
    /* [in] */ const String& str)
{
    return Shell::Set_table_name(str);
}

ECode CShell::Columns(
    /* [in] */ArrayOf<String>* args)
{
    return Shell::Columns(args);
}

ECode CShell::Types(
    /* [in] */ArrayOf<String>* args)
{
    return Shell::Types(args);
}

ECode CShell::Newrow(
    /* [in] */ArrayOf<String>* args,
    /* [out] */Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return Shell::Newrow(args, result);
}

ECode CShell::Do_meta(
    /* [in] */ const String& line)
{
    return Shell::Do_meta(line);
}

ECode CShell::Read_line(
    /* [in] */ IBufferedReader* is, 
    /* [in] */ const String& prompt,
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
    /* [in] */ const String& sql)
{
    return Shell::Do_cmd(sql);
}