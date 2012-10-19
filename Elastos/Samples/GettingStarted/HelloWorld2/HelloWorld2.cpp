//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

// This is the recommended version of "Hello World" on Elastos.
//
// The old LibC has two major problems:
// (1) prone to virus attacks (due to buffer overrun);
// (2) lack of internationalization support.
//

#include <stdio.h>
#include <elastos.h>        // the only hearder to remember for Elastos APIs
using namespace Elastos;

ECode ElastosMain(const BufferOf<String>& args) // entery of the main applet
{
    printf("Hello World!\n");
    return NOERROR_EXIT;    // "return NOERROR" enters the Applet's event loop.
}

