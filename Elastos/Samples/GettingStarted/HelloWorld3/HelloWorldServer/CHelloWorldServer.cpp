
#include "CHelloWorldServer.h"
#include <stdio.h>

ECode CHelloWorldServer::Hello()
{
    printf("Hello\n");
    return NOERROR;
}

ECode CHelloWorldServer::World()
{
    printf("World\n");
    return NOERROR;
}
