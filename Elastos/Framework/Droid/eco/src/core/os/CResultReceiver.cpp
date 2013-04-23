
#include "os/CResultReceiver.h"
#include "ext/frameworkdef.h"


ECode CResultReceiver::constructor(
    /* [in] */ IApartment* handler)
{
    return ResultReceiver::Init(handler);
}

ECode CResultReceiver::Send(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    return ResultReceiver::Send(resultCode, resultData);
}

ECode CResultReceiver::DescribeContents(
    /* [out] */ Int32* contents)
{
    return ResultReceiver::DescribeContents(contents);
}

ECode CResultReceiver::ReadFromParcel(
    /* [in] */ IParcel *dest)
{
    return ResultReceiver::ReadFromParcel(dest);
}

ECode CResultReceiver::WriteToParcel(
    /* [in] */ IParcel* out)
{
    return ResultReceiver::WriteToParcel(out);
}

Mutex* CResultReceiver::GetSelfLock()
{
    return &_m_syncLock;
}
