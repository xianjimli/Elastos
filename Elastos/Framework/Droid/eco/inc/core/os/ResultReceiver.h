
#ifndef  __RESULTRECEIVER_H__
#define  __RESULTRECEIVER_H__

#include "os/Runnable.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>


using namespace Elastos::Core::Threading;

/**
 * Generic interface for receiving a callback result from someone.  Use this
 * by creating a subclass and implement {@link #onReceiveResult}, which you can
 * then pass to others and send through IPC, and receive results they
 * supply with {@link #send}.
 */
class ResultReceiver
{
    friend class CMyResultReceiver;

private:
    class MyRunnable : public Runnable
    {
    public:
        MyRunnable(
            /* [in] */ Int32 resultCode,
            /* [in] */ IBundle* resultData,
            /* [in] */ ResultReceiver* host);

        CARAPI Run();

    private:
        const Int32 mResultCode;
        const AutoPtr<IBundle> mResultData;
        ResultReceiver* mHost;
    };

public:
    /**
     * Create a new ResultReceive to receive results.  Your
     * {@link #onReceiveResult} method will be called from the thread running
     * <var>handler</var> if given, or from an arbitrary thread if null.
     */
    ResultReceiver(
        /* [in] */ IApartment* handler);

    /**
     * Deliver a result to this receiver.  Will call {@link #onReceiveResult},
     * always asynchronously if the receiver has supplied a Handler in which
     * to dispatch the result.
     * @param resultCode Arbitrary result code to deliver, as defined by you.
     * @param resultData Any additional data provided by you.
     */
    CARAPI Send(
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* resultData);

    CARAPI DescribeContents(
            /* [out] */ Int32* contents);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

protected:
    ResultReceiver();

    /**
     * Create a new ResultReceive to receive results.  Your
     * {@link #onReceiveResult} method will be called from the thread running
     * <var>handler</var> if given, or from an arbitrary thread if null.
     */
    CARAPI Init(
        /* [in] */ IApartment* handler);

    /**
     * Override to receive results delivered to this object.
     *
     * @param resultCode Arbitrary result code delivered by the sender, as
     * defined by the sender.
     * @param resultData Any additional data provided by the sender.
     */
    CARAPI OnReceiveResult(
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* resultData);

protected:
    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

protected:
    Boolean mLocal;
    AutoPtr<IApartment> mHandler;
    AutoPtr<IResultReceiver> mReceiver;
};

#endif  //__RESULTRECEIVER_H__
