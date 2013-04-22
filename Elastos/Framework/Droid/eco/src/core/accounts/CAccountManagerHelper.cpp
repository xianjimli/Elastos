
#include "ext/frameworkdef.h"
#include "accounts/CAccountManagerHelper.h"

ECode CAccountManagerHelper::Get(
    /* [in] */ IContext* context,
    /* [out] */ ILocalAccountManager** accountManager)
{
    VALIDATE_NOT_NULL(accountManager);
    if (context == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //throw new IllegalArgumentException("context is null");
    }
    return context->GetSystemService(Context_ACCOUNT_SERVICE,
            (IInterface**)accountManager);
}
