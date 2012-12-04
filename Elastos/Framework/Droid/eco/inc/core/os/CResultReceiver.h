
#ifndef  _CRESULTRECEIVER_H__
#define  _CRESULTRECEIVER_H__

#include "_CResultReceiver.h"
#include "os/Runnable.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>


/**
 * Generic interface for receiving a callback result from someone.  Use this
 * by creating a subclass and implement {@link #onReceiveResult}, which you can
 * then pass to others and send through IPC, and receive results they
 * supply with {@link #send}.
 */
CarClass(CResultReceiver)
{
private:
    class MyRunnable : public Runnable
    {
    public:
        MyRunnable(
            /* [in] */ Int32 resultCode,
            /* [in] */ IBundle* resultData,
            /* [in] */ CResultReceiver* host);

        CARAPI Run();

    private:
        const Int32 mResultCode;
        const AutoPtr<IBundle> mResultData;
        CResultReceiver* mHost;
    };

    class MyResultReceiver
        : public ElRefBase
        , public IResultReceiver
    {
    public:
        MyResultReceiver(
            /* [in] */ CResultReceiver* host);

        CARAPI Send(
            /* [in] */ Int32 resultCode,
            /* [in] */ IBundle* resultData);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        CResultReceiver* mHost;
    };

public:
    CResultReceiver();

    /**
     * Create a new ResultReceive to receive results.  Your
     * {@link #onReceiveResult} method will be called from the thread running
     * <var>handler</var> if given, or from an arbitrary thread if null.
     */
    CARAPI constructor(
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
    /**
     * Override to receive results delivered to this object.
     *
     * @param resultCode Arbitrary result code delivered by the sender, as
     * defined by the sender.
     * @param resultData Any additional data provided by the sender.
     */
    virtual CARAPI_(void) OnReceiveResult(
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* resultData);

private:
    Boolean mLocal;
    AutoPtr<IApartment> mHandler;

    AutoPtr<IResultReceiver> mReceiver;
};

#endif  //_CRESULTRECEIVER_H__
