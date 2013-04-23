
#include "os/CMyResultReceiver.h"
#include "os/ResultReceiver.h"


ECode CMyResultReceiver::constructor(
    /* [in] */ Handle32 host)
{
    mHost = host;
    return NOERROR;
}

ECode CMyResultReceiver::Send(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    if (((ResultReceiver*)mHost)->mHandler != NULL) {
        //mHandler.post(new MyRunnable(resultCode, resultData, mHost));
        AutoPtr<IRunnable> runnable = new ResultReceiver::MyRunnable(
                resultCode, resultData, (ResultReceiver*)mHost);

        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

        //todo: maybe the runnable is deleted when call it's run
        ((ResultReceiver*)mHost)->mHandler->PostCppCallback(
            (Handle32)runnable.Get(), *(Handle32*)&pHandlerFunc, NULL, 0);
    }
    else {
        ((ResultReceiver*)mHost)->OnReceiveResult(resultCode, resultData);
    }
}
