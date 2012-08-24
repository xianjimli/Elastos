
#include "CHelloClient.h"
#include "CCallback.h"
#include <stdio.h>

ECode CHelloClient::OnTrigger(PInterface pSender)
{
    IHello* pIHello = IHello::Probe(pSender);
    pIHello->Hello();
    return NOERROR;
}

ECode CHelloClient::HelloClient(IHello* pIHello)
{
    return NOERROR;
}
