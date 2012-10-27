
#include "MyTask.h"
#include <Elastos.Console.h>
#include <elastos/AutoPtr.h>
#include <Elastos.Console.h>

using namespace Elastos;

ECode OnFinishedEvent(PVoid userData, PInterface sender)
{
    CConsole::WriteLine("====== CALLing OnFinishedEvent ======");
    CMyTask::RemoveFinishedCallback(sender, OnFinishedEvent, NULL);
    CConsole::RequestToFinish();
    return NOERROR;
}

ECode ElastosMain(const BufferOf<CString>& args)
{
    CConsole::WriteLine("====== CALLing ElastosMain ======");
    AutoPtr<ITask> myTask;
    CMyTask::New((ITask**)&myTask);

    CConsole::ReleaseAtExit(myTask);

    ECode ec = CMyTask::AddFinishedCallback(myTask, OnFinishedEvent, NULL);
    if (FAILED(ec)) return ec;

    CConsole::WriteLine("====== CMyTask::AddFinishedCallback Succeeded! ======");

    myTask->Start(args);

    return NOERROR;
}
