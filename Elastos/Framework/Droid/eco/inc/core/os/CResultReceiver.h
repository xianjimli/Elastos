
#ifndef  __CRESULTRECEIVER_H__
#define  __CRESULTRECEIVER_H__

#include "_CResultReceiver.h"
#include "os/Runnable.h"
#include "os/ResultReceiver.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>


/**
 * Generic interface for receiving a callback result from someone.  Use this
 * by creating a subclass and implement {@link #onReceiveResult}, which you can
 * then pass to others and send through IPC, and receive results they
 * supply with {@link #send}.
 */
CarClass(CResultReceiver), public ResultReceiver
{
public:
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
};

#endif  //__CRESULTRECEIVER_H__
