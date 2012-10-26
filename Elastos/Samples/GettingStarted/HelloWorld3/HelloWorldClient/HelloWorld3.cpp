//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

// This is the client/service version of "Hello World" on Elastos.
//
// HelloWorldServer.eco is the server. It is a CAR component which could be loaded in runtime. 
// Helloworld3.ecx is the client. It is a exacutalbe program with a spacial "ElastosMain" running entry.
//

#include <stdio.h>
#include <HelloWorldServer.h>
#include <elastos.h>        // the only hearder to remember for Elastos APIs
using namespace Elastos;

ECode ElastosMain(const BufferOf<String>& args) // entery of the main applet
{
    IHello* pHello;
    ECode ec = CHelloWorldServer::New(&pHello);
    if (FAILED(ec)) {
        return ec;
    }
    pHello->Hello();

    IWorld* pWorld;
    pWorld = IWorld::Probe(pHello);
    if (pWorld) {
        pWorld->World();
    }

    return NOERROR_EXIT;
}

