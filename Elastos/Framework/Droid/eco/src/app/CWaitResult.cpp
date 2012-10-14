
#include "ext/frameworkdef.h"
#include "app/CWaitResult.h"

ECode CWaitResult::constructor()
{
    return NOERROR;
}

ECode CWaitResult::constructor(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode CWaitResult::GetResult(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mResult;
    return NOERROR;
}

ECode CWaitResult::SetResult(
    /* [in] */ Int32 result)
{
    mResult = result;
    return NOERROR;
}

ECode CWaitResult::IsTimeout(
    /* [out] */ Boolean* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    *timeout = mTimeout;
    return NOERROR;
}

ECode CWaitResult::SetTimeout(
    /* [in] */ Boolean timeout)
{
    mTimeout = timeout;
    return NOERROR;
}

ECode CWaitResult::GetWho(
    /* [out] */ IComponentName** who)
{
    VALIDATE_NOT_NULL(who);
    *who = mWho;
    if (*who != NULL) (*who)->AddRef();
    return NOERROR;
}

ECode CWaitResult::SetWho(
    /* [in] */ IComponentName* who)
{
    mWho = who;
    return NOERROR;
}

ECode CWaitResult::GetThisTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mThisTime;
    return NOERROR;
}

ECode CWaitResult::SetThisTime(
    /* [in] */ Int64 time)
{
    mThisTime = time;
    return NOERROR;
}

ECode CWaitResult::GetTotalTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mTotalTime;
    return NOERROR;
}

ECode CWaitResult::SetTotalTime(
    /* [in] */ Int64 time)
{
    mTotalTime = time;
    return NOERROR;
}

ECode CWaitResult::DescribeContents(
    /* [out] */ Int32* result)
{
    if (result == NULL) return E_NOT_IMPLEMENTED;

    *result = 0;
    return NOERROR;
}

ECode CWaitResult::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWaitResult::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}

