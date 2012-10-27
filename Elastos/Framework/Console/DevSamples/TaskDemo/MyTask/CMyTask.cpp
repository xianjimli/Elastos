
#include "CMyTask.h"
#include <Elastos.Console.h>

CMyTask::~CMyTask()
{
    CConsole::WriteLine("====== CALLING CMyTask::~CMyTask() ======");
}

ECode CMyTask::Main(
    /* [in] */ const BufferOf<CString>& args)
{
    CConsole::WriteLine("====== CALLing CMyTask::Main ======");

    Finish(TaskFinish_Nice);
    return NOERROR;
}
