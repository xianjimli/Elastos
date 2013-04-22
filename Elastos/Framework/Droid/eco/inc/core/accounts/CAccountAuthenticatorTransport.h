
#ifndef __CACCOUNTAUTHENTICATORTRANSPORT_H__
#define __CACCOUNTAUTHENTICATORTRANSPORT_H__

#include "_CAccountAuthenticatorTransport.h"

CarClass(CAccountAuthenticatorTransport)
{
public:
    /**
     * prompts the user for account information and adds the result to the IAccountManager
     */
    CARAPI AddAccount(
        /* [in] */ IAccountAuthenticatorResponse* response,
        /* [in] */ const String& accountType,
        /* [in] */ const String& authTokenType,
        /* [in] */ const ArrayOf<String>& requiredFeatures,
        /* [in] */ IBundle* options);

    /**
     * prompts the user for the credentials of the account
     */
    CARAPI ConfirmCredentials(
        /* [in] */ IAccountAuthenticatorResponse* response,
        /* [in] */ IAccount* account,
        /* [in] */ IBundle* options);

    /**
     * gets the password by either prompting the user or querying the IAccountManager
     */
    CARAPI GetAuthToken(
        /* [in] */ IAccountAuthenticatorResponse* response,
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ IBundle* options);

    /**
     * Gets the user-visible label of the given authtoken type.
     */
    CARAPI GetAuthTokenLabel(
        /* [in] */ IAccountAuthenticatorResponse* response,
        /* [in] */ const String& authTokenType);

    /**
     * prompts the user for a new password and writes it to the IAccountManager
     */
    CARAPI UpdateCredentials(
        /* [in] */ IAccountAuthenticatorResponse* response,
        /* [in] */ IAccount* account,
        /* [in] */ const String& authTokenType,
        /* [in] */ IBundle* options);

    /**
     * launches an activity that lets the user edit and set the properties for an authenticator
     */
    CARAPI EditProperties(
        /* [in] */ IAccountAuthenticatorResponse* response,
        /* [in] */ const String& accountType);

    /**
     * returns a Bundle where the boolean value BOOLEAN_RESULT_KEY is set if the account has the
     * specified features
     */
    CARAPI HasFeatures(
        /* [in] */ IAccountAuthenticatorResponse* response,
        /* [in] */ IAccount* account,
        /* [in] */ const ArrayOf<String>& features);

    /**
     * Gets whether or not the account is allowed to be removed.
     */
    CARAPI GetAccountRemovalAllowed(
        /* [in] */ IAccountAuthenticatorResponse* response,
        /* [in] */ IAccount* account);
};

#endif //__CACCOUNTAUTHENTICATORTRANSPORT_H__
