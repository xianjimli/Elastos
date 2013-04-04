#ifndef _CSHELL_H_
#define _CSHELL_H_

#include "Shell.h"
#include "_CShell.h"

CarClass(CShell), public Shell
{
public:
    CARAPI constructor(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ IPrintWriter* err);

    CARAPI constructor(
        /* [in] */ IPrintStream* pw,
        /* [in] */ IPrintStream* err);

    CARAPI Clone(
        /* [out] */IInterface** obj);

    CARAPI Set_table_name(
        /* [in] */ const String& str);

    CARAPI Columns(
        /* [in] */ArrayOf<String>* args);

    CARAPI Types(
        /* [in] */ArrayOf<String>* args);

    CARAPI Newrow(
        /* [in] */ArrayOf<String>* args,
        /* [out] */Boolean* result);

    CARAPI Do_meta(
        /* [in] */ const String& line);

    CARAPI Read_line(
        /* [in] */ IBufferedReader* is, 
        /* [in] */ const String& prompt,
        /* [out] */ String* str);

    CARAPI Do_input(
        /* [in] */IBufferedReader* is);

    CARAPI Do_cmd(
        /* [in] */ const String& sql);
};
#endif //_CSHELL_H_
