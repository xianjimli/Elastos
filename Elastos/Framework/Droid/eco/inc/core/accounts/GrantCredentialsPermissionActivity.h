
#ifndef __GRANTCREDENTIALSPERMISSIONACTIVITY_H__
#define __GRANTCREDENTIALSPERMISSIONACTIVITY_H__

#include "ext/frameworkext.h"
#include "app/Activity.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

class GrantCredentialsPermissionActivity
        : public Activity
        , public ElRefBase
        , public IViewOnClickListener
{
public:
    GrantCredentialsPermissionActivity(
        /* [in] */ IContext* context);

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI_(void) SetAccountAuthenticatorResult(
        /* [in] */ IBundle* result);

    /**
     * Sends the result or a {@link AccountManager#ERROR_CODE_CANCELED} error if a
     * result isn't present.
     */
    CARAPI Finish();

private:
    CARAPI NewCapsuleView(
        /* [in] */ const String& capsuleLabel,
        /* [out] */ IView** view);

public:
    static const CString EXTRAS_ACCOUNT;
    static const CString EXTRAS_AUTH_TOKEN_LABEL;
    static const CString EXTRAS_AUTH_TOKEN_TYPE;
    static const CString EXTRAS_RESPONSE;
    static const CString EXTRAS_ACCOUNT_TYPE_LABEL;
    static const CString EXTRAS_PACKAGES;
    static const CString EXTRAS_REQUESTING_UID;

protected:
    AutoPtr<ILayoutInflater> mInflater;

private:
    AutoPtr<IAccount> mAccount;
    String mAuthTokenType;
    Int32 mUid;
    AutoPtr<IBundle> mResultBundle;

    AutoPtr<IAccountManager> mAccountManagerService;// = AccountManagerService.getSingleton();


private:
    static const CString TAG;

    AutoPtr<IContext> mContext;

    AutoPtr<IAccountAuthenticator> mTransport;
};

#endif //__GRANTCREDENTIALSPERMISSIONACTIVITY_H__
