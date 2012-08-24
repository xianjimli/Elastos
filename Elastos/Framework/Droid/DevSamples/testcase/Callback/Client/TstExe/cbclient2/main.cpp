
#include <elastos.h>
#include <stdio.h>

using namespace Elastos;

#include "carcallback10.h"

int main(int argc, char **argv)
{
    ECode ec;

    ICBServer * piCBServer;
    ec = CSystem::FindRunningObject("CBServer", (IInterface**)&piCBServer);
    if (FAILED(ec)) {
        printf("FindRunningObject Error, ec = %x\n", ec);
        return -1;
    }

    piCBServer->Trigger2();

    piCBServer->Release();

    return 0;
}
