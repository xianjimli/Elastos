
#ifndef __CPASSWORDAUTHENTICATION_H__
#define __CPASSWORDAUTHENTICATION_H__

#include "_CPasswordAuthentication.h"

/**
 * This immutable class is a data structure that encapsulates username and
 * password which is used by the {@code Authenticator} class.
 *
 * @see Authenticator
 */
CarClass(CPasswordAuthentication)
{
public:
    ~CPasswordAuthentication();

    /**
     * Creates an instance of a password authentication with a specified
     * username and password.
     *
     * @param userName
     *            the username to store.
     * @param password
     *            the associated password to store.
     */
    CARAPI constructor(
        /* [in] */ const String& userName,
        /* [in] */ const ArrayOf<Char32>& password);

    /**
     * Gets a clone of the password stored by this instance. The user is
     * responsible to finalize the returned array if the password clone is no
     * longer needed.
     *
     * @return the copied password.
     */
    CARAPI GetPassword(
        /* [out, callee] */ ArrayOf<Char32>** password);

    /**
     * Gets the username stored by this instance.
     *
     * @return the stored username.
     */
    CARAPI GetUserName(
        /* [out] */ String* userName);

private:
    String mUserName;

    ArrayOf<Char32>* mPassword;
};

#endif //__CPASSWORDAUTHENTICATION_H__
