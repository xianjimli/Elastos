//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

//
// There are several ways to start up a new program execution in Elastos.
// For example, the good old C/C++ way of execution still works, i.e.,
// we still support the following for backward compatibility:
//
// int main(int argc, char* argv[]);
// int wmain(int argc, wchar_t* wargv[]);
//
// The main() and wmain() functions will exit a program upon returning from
// the main body of code.  That is the way it has been for more than 30 years
// and we have no intention of changing that.
//
// Elastos, however, introduces two more ways of starting up a program,
// one for the Elastos EXE file and the other for the Elastos DLL module.
//
// ECode ElastosMain(const WStringArray& args);
//
// An EXE version of the Elastos source code should contain the above funtion,
// which has only one argument because WStringArray is self-describing, i.e.,
// it has the number of elements built-in.
//
// The Elastos shell, namely SuperExe.exe, may execute a DLL directly if
// it has an applet declaration in its CAR file.
//
// After returning from the ElastosMain() function successfully, the execution
// flow will fall into the CallbackEventProcessingLoop(), similar to what the
// Windows-Message-Loop() does.  To achieve the effect of Windows allowing
// code after the Windows-Message-Loop(), programmers may register
// CThread::AtQuit(), CApplet::AtFinish() or CProcess::AtExit() inside
// ElastosMain(), respectively.
//
// When executing a DLL which contains an applet class, the SuperExe.exe
// looks for the applet class.  If found, the SuperExe creates an instance
// of the applet class, e.g., TFoo::New().  If succeed, it will then create
// a new thread especially for the applet, called the applet thread, which
// will execute the TFoo::Main() routine of the instance.  In other words,
// the thread which executes the TFoo::constructor() and TFoo::Main() are
// two different threads. Functions which are invoked in TFoo::constructor()
// or TFoo::Main() may demonstrate some subtle semantic differences.
//
// Similar to ElastosMain(), the applet thread will not terminate after
// finishes all tasks in the method successfully.  Instead, it will sit in a
// loop processing all asynchronous callbacks registered by the applet thread
// and all its offspring threads.  The applet quits when CProcess:Exit()
// or CApplet::Finish() is explicitly called upon by the applet thread or
// any of its offspring threads.
//
// The Main routine of an applet component maybe considered as a special
// form of constructor, that may initialize the component according to the
// command arguments, i.e., the "args" in CFoo:Main(WStringArray& args).
//
// The CFoo::Main() routine may not be the only entry point of the DLL module
// because another application, other than the SuperExe, may also create any
// of the DLL's classes and invoke any public methods.
//

#include "FooBarDemo.h"
#include <Elastos.Console.h>

using namespace Elastos;

ECode OnFooEvent(PVoid userData, PInterface pSender)
{
    CConsole::WriteLine("====== CALLing OnFooEvent ======");
    CFooBar::RemoveFooEventCallback(pSender, OnFooEvent, NULL);
    CConsole::RequestToFinish();
    return NOERROR;
}

ECode ElastosMain(const BufferOf<CString>& args)
{
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

    return NOERROR;
}
