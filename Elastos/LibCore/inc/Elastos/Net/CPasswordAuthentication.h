
#ifndef __CPASSWORDAUTHENTICATION_H__
#define __CPASSWORDAUTHENTICATION_H__

#include "_CPasswordAuthentication.h"

CarClass(CPasswordAuthentication)
{
public:
    CARAPI GetPassword(
        /* [out, callee] */ ArrayOf<Char32>** password);

    /**
     * Gets the username stored by this instance.
     *
     * @return the stored username.
     */
    CARAPI GetUserName(
        /* [out] */ String* userName);

    CARAPI constructor(
        /* [in] */ String userName,
        /* [in] */ const ArrayOf<Char32>& password);

private:
    String mUserName;

    ArrayOf<Char32>* mPassword;
};

#endif //__CPASSWORDAUTHENTICATION_H__
