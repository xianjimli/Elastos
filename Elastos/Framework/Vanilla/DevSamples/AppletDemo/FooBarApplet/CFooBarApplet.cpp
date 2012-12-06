
#include "CFooBarApplet.h"
#include "FooBar.h"
#include <Elastos.Framework.Vanilla>

IApplet * gApplet;

ECode OnFooEvent(PVoid userData, PInterface pSender)
{
    CConsole::WriteLine("========CFooBarApplet -> OnFooEvent();");
    CFooBar::RemoveFooEventCallback(pSender, OnFooEvent, NULL);

    gApplet->Quit(0, AppletQuit_Nicely);  //CApplet::Quit();

    return NOERROR;
}

CFooBarApplet::CFooBarApplet()
{
    CConsole::WriteLine("========CFooBarApplet::CFooBarApplet();");
 
    gApplet = this;
}

CFooBarApplet::~CFooBarApplet()
{
    CConsole::WriteLine("========CFooBarApplet::~CFooBarApplet();");
}

ECode CFooBarApplet::Main(
    /* [in] */ const BufferOf<CString>& args)
{
    CConsole::WriteLine("========CFooBarApplet::Main();");

    Boolean bActivated;
    gApplet->IsActivated(&bActivated);
    if (bActivated) {
        CConsole::WriteLine("========CFooBarApplet IsActivated;");
    }

    IBar* pBar;
    ECode ec = CFooBar::New(&pBar);
    if (FAILED(ec)) {
        return ec;
    }
    pBar->Bar();

    ec = CFooBar::AddFooEventCallback(pBar, OnFooEvent, NULL);
    if (FAILED(ec)) {
        pBar->Release();
        return ec;
    }

    IFoo* pFoo;
    pFoo = IFoo::Probe(pBar);
    if (pFoo) {
        pFoo->Foo();
    }

    gApplet->Deactivate();
    gApplet->IsActivated(&bActivated);
    if (!bActivated) {
        CConsole::WriteLine("========CFooBarApplet Is not Activated;");
    }

//    CApplet::DeleteObjectAtQuit(pBar);
//    CApplet::DeleteObjectAtQuit(pFoo);
    return NOERROR;
}

ECode CFooBarApplet::AtQuit(Int32 n)
{
    CConsole::WriteLine("========CFooBarApplet::AtQuit();");
//    m_pFoo->Release();
    return NOERROR;
}
