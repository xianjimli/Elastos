
//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <elastos.h>

using namespace Elastos;

extern ECode ElastosMain(const BufferOf<CString>& args);

ELAPI_(void) _Console_Init();
ELAPI _Console_Enter();
ELAPI_(void) _Console_Exit();

int main(int argc, char *argv[])
{
    BufferOf<CString> box((CString *)argv, argc, argc);

    _Console_Init();

    ECode ec = ElastosMain(box);

    if (NOERROR_EXIT == ec) ec = NOERROR;
    else if (SUCCEEDED(ec)) {
        ec = _Console_Enter();
    }

    _Console_Exit();

    return ec;
}


//elastos foobar.eco foobar.js foobar.lua foobar.html