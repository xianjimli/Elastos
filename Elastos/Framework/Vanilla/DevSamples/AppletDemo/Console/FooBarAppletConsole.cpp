
#include "FooBarApplet.h"
#include <elastos/AutoPtr.h>
#include <Elastos.Framework.Vanilla>

ECode OnQuittedEvent(PVoid userData, PInterface sender)
{
    CConsole::WriteLine("====FooBarAppletConsole -> OnQuittedEvent();");
//    IAppletQuit::Probe(sender)->AtQuit(0);
    CConsole::AddExitRoutine(sender,userData);
    CFooBarApplet::RemoveQuittedCallback(sender, OnQuittedEvent, userData);
    CConsole::RequestToFinish();
    return NOERROR;
}

ECode ElastosMain(const BufferOf<CString>& args)
{
    CConsole::WriteLine("====FooBarAppletConsole -> ElastosMain();");
    AutoPtr<IApplet> myApplet;
    CFooBarApplet::New((IApplet**)&myApplet);

    CConsole::ReleaseAtExit(myApplet);

    ECode ec = CFooBarApplet::AddQuittedCallback(myApplet, OnQuittedEvent, NULL);
    if (FAILED(ec)) return ec;

    CConsole::WriteLine("====FooBarAppletConsole -> CFooBarApplet::AddQuittedCallback();");

    myApplet->Start(args);

    return NOERROR;
}
