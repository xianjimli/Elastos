
#include "Shell.h"

Shell::Shell()
{}

Shell::Shell(
    /* [in] */IPrintWriter* pw, 
    /* [in] */IPrintWriter* err)
{
}

//Shell::Shell(
//    /* [in] */IPrintStream* ps, 
//    /* [in] */IPrintStream* errs)
//{
//}

ECode Shell::Sql_quote_dbl(
    /* [in] */String str,
    /* [out] */String* result)
{
    assert(result != NULL);
    return NOERROR;
}

ECode Shell::Sql_quote(
    /* [in] */String str,
    /* [out] */String* result)
{
    assert(result != NULL);
    return NOERROR;
}

ECode Shell::Columns(
    /* [in] */ArrayOf<String>* args)
{
    return NOERROR;
}

ECode Shell::Types(
    /* [in] */ArrayOf<String>* args)
{
    return NOERROR;
}

ECode Shell::Newrow(
    /* [in] */ArrayOf<String>* args,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    return NOERROR;
}

ECode Shell::Clone(
    /* [out] */IInterface** obj)
{
    return NOERROR;
}

ECode Shell::Html_quote(
    /* [in] */String str,
    /* [out] */String* result)
{
    assert(result != NULL);
    return NOERROR;
}

ECode Shell::Is_numeric(
    /* [in] */String str,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    return NOERROR;
}

ECode Shell::Set_table_name(
    /* [in] */const String& str)
{
    return NOERROR;
}

ECode Shell::Do_meta(
    /* [in] */const String& line)
{
    return NOERROR;
}

ECode Shell::Read_line(
    /* [in] */IBufferedReader* is, 
    /* [in] */const String& prompt,
    /* [out] */String* str)
{
    assert(str != NULL);
    return NOERROR;
}

ECode Shell::Do_input(
    /* [in] */IBufferedReader* is)
{
    return NOERROR;
}

ECode Shell::Do_cmd(
    /* [in] */const String& sql)
{
    return NOERROR;
}

DBDump::DBDump(
//    /* [in] */AutoPtr<Shell> s, 
    /* [in] */const ArrayOf<String>& tables)
{
}

ECode DBDump::Columns(
    /* [in] */ArrayOf<String>* args)
{
    return NOERROR;
}

ECode DBDump::Types(
    /* [in] */ArrayOf<String>* args)
{
    return NOERROR;
}

ECode DBDump::Newrow(
    /* [in] */ArrayOf<String>* args,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    return NOERROR;
}