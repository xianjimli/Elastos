
#ifndef __CTEXTVIEWCOMMITSELECTIONRECEIVER_H__
#define __CTEXTVIEWCOMMITSELECTIONRECEIVER_H__

#include "_CTextViewCommitSelectionReceiver.h"
#include "os/ResultReceiver.h"

CarClass(CTextViewCommitSelectionReceiver), public ResultReceiver
{
    friend class TextView;

public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 prevStart,
        /* [in] */ Int32 prevEnd,
        /* [in] */ Handle32 host);

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
    //@Override
    CARAPI OnReceiveResult(
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* resultData);

private:
    CARAPI_(Mutex*) GetSelfLock();

private:
    Int32 mPrevStart;
    Int32 mPrevEnd;
    Handle32 mHost;
};


#endif //__CTEXTVIEWCOMMITSELECTIONRECEIVER_H__
