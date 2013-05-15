
#include "content/CSyncInfo.h"
#include "accounts/CAccount.h"

ECode CSyncInfo::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = 0;

    return NOERROR;
}

ECode CSyncInfo::GetAuthorityId(
    /* [out] */ Int32* authorityId)
{
    VALIDATE_NOT_NULL(authorityId);

    *authorityId = this->authorityId;

    return NOERROR;
}

ECode CSyncInfo::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account);

    *account = this->account;

    return NOERROR;
}

ECode CSyncInfo::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);

    *authority = this->authority;

    return NOERROR;
}

ECode CSyncInfo::GetStartTime(
    /* [out] */ Int64* startTime)
{
    VALIDATE_NOT_NULL(startTime);

    *startTime = this->startTime;

    return NOERROR;
}

ECode CSyncInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&authorityId);
    source->ReadString(&authority);
    source->ReadInt64(&startTime);

    CAccount::New(source, (IAccount**)&account);

    return NOERROR;
}

ECode CSyncInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(authorityId);
    dest->WriteString(authority);
    dest->WriteInt64(startTime);

    ((CAccount*)(IAccount*)account)->WriteToParcel(dest);

    return NOERROR;
}


ECode CSyncInfo::constructor(
    /* [in] */ IParcel* parcel)
{
    ReadFromParcel(parcel);

    return NOERROR;
}

ECode CSyncInfo::constructor(
    /* [in] */ Int32 authorityId,
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int64 startTime)
{
    this->authorityId = authorityId;
    this->account = account;
    this->authority = authority;
    this->startTime = startTime;

    return NOERROR;
}

