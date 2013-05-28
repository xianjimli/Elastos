
#include "content/CPeriodicSync.h"
#include "content/CSyncStorageEngineHelper.h"
#include "os/CBundle.h"

CPeriodicSync::CPeriodicSync()
{}

CPeriodicSync::~CPeriodicSync()
{}

ECode CPeriodicSync::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account);
    *account = mAccount;
    return NOERROR;
}

ECode CPeriodicSync::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority;
    return NOERROR;
}

ECode CPeriodicSync::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = mExtras;
    return NOERROR;
}

ECode CPeriodicSync::GetPeriod(
    /* [out] */ Int64* period)
{
    VALIDATE_NOT_NULL(period);
    *period = mPeriod;
    return NOERROR;
}

ECode CPeriodicSync::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if ((IPeriodicSync*)obj == this) {
        *result = TRUE;
        return NOERROR;
    }

    if (!(IPeriodicSync::Probe(obj) != NULL)) {
        *result = FALSE;
        return NOERROR;
    }

    const AutoPtr<IPeriodicSync> other = (IPeriodicSync*) obj;
    *result = FALSE;
    AutoPtr<IAccount> otherAccount;
    FAIL_RETURN(other->GetAccount((IAccount**)&otherAccount));  

    if (_CObject_Compare(mAccount, otherAccount)) {
        String otherAuthority;
        FAIL_RETURN(other->GetAuthority(&otherAuthority));
        if (!mAuthority.Compare(otherAuthority)) {
            Int64 otherPeriod;
            FAIL_RETURN(other->GetPeriod(&otherPeriod));
            if (mPeriod == otherPeriod) {
                Boolean isEqual;
                AutoPtr<IBundle> otherExtras;
                FAIL_RETURN(other->GetExtras((IBundle**)&otherExtras));
                AutoPtr<ISyncStorageEngineHelper> engineHelper;
                FAIL_RETURN(CSyncStorageEngineHelper::AcquireSingleton((ISyncStorageEngineHelper**) &engineHelper));
                FAIL_RETURN(engineHelper->Equals(mExtras, otherExtras, &isEqual));
                if (isEqual) {
                    *result = TRUE;
                }
            }
        }
    }

    return NOERROR;
}

ECode CPeriodicSync::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPeriodicSync::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (NULL != mAccount) {
        FAIL_RETURN(IParcelable::Probe(mAccount)->WriteToParcel(dest));
    }

    FAIL_RETURN(dest->WriteString(mAuthority));
    FAIL_RETURN(dest->WriteInterfacePtr(mExtras));
    FAIL_RETURN(dest->WriteInt64(mPeriod));
    return NOERROR;
}

ECode CPeriodicSync::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 period)
{
    mAccount = account;
    mAuthority = authority;
    FAIL_RETURN(CBundle::New(extras, (IBundle**)&mExtras));
    mPeriod = period;
    return NOERROR;
}

