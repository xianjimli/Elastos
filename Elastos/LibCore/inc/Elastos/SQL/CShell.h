#ifndef _CSHELL_H_
#define _CSHELL_H_

#include "Shell.h"
#include "_CShell.h"

CarClass(CShell), public Shell
{
    CARAPI Set_table_name(
        /* [in] */ String str);

    CARAPI Do_meta(
        /* [in] */ String line);

    CARAPI Read_line(
        /* [in] */ IBufferedReader* is, 
        /* [in] */ String prompt,
        /* [out] */ String* str);

    CARAPI Do_input(
        /* [in] */IBufferedReader* is);

    CARAPI Do_cmd(
        /* [in] */ String sql);
};
#endif //_CSHELL_H_
