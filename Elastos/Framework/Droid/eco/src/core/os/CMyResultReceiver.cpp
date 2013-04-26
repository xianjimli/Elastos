
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
        //do not use AutoPtr<IRunnable> as the type of runnable here
        IRunnable* runnable = new ResultReceiver::MyRunnable(
                resultCode, resultData, (ResultReceiver*)mHost);

        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

        ((ResultReceiver*)mHost)->mHandler->PostCppCallback(
            (Handle32)runnable, *(Handle32*)&pHandlerFunc, NULL, 0);
    }
    else {
        ((ResultReceiver*)mHost)->OnReceiveResult(resultCode, resultData);
    }
}
