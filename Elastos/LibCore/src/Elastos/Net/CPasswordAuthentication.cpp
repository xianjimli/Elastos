
#include "cmdef.h"
#include "CPasswordAuthentication.h"


CPasswordAuthentication::~CPasswordAuthentication()
{
    if (mPassword != NULL) ArrayOf<Char32>::Free(mPassword);
}

ECode CPasswordAuthentication::constructor(
    /* [in] */ const String& userName,
    /* [in] */ const ArrayOf<Char32>& password)
{
    mUserName = userName;
    mPassword = password.Clone();

    return NOERROR;
}

ECode CPasswordAuthentication::GetPassword(
    /* [out, callee] */ ArrayOf<Char32>** password)
{
    VALIDATE_NOT_NULL(password);
    *password = mPassword->Clone();
    return NOERROR;
}

/**
 * Gets the username stored by this instance.
 *
 * @return the stored username.
 */
ECode CPasswordAuthentication::GetUserName(
    /* [out] */ String* userName)
{
    VALIDATE_NOT_NULL(userName);
    *userName = mUserName;
    return NOERROR;
}
