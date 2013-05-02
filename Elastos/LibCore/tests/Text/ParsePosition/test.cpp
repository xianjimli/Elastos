#include "test.h"
#include <stdio.h>

#include "Elastos.Text.h"
#include <elastos/AutoPtr.h>
#include <elastos/System.h>

using namespace Elastos;


int CTest::test_getErrorIndex(int argc, char* argv[])
{

    AutoPtr<IParsePosition> pp;
    CParsePosition::New(0x7FFFFFFF, (IParsePosition**)&pp);
    pp->SetErrorIndex(56);
    Int32 result;
    pp->GetErrorIndex(&result);
    assert(result == 56);
    pp->SetErrorIndex(0x7FFFFFFF);
    pp->GetErrorIndex(&result);
    assert(result == 0x7FFFFFFF);
    return 0;
}


