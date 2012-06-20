
#include "capsule/CAuthorInfo.h"
CAuthorInfo::CAuthorInfo()
{
    m_Name = NULL;
    m_Email = NULL;
    m_Phone = NULL;
    m_Mobile = NULL;
    m_Address = NULL;
    m_Affiliation = NULL;
}
    
CAuthorInfo::~CAuthorInfo()
{
    m_Name = NULL;
    m_Email = NULL;
    m_Phone = NULL;
    m_Mobile = NULL;
    m_Address = NULL;
    m_Affiliation = NULL;
}

ECode CAuthorInfo::GetAuthorInfo(
    /* [in] */ String attrName,
    /* [out] */ String * pAttrValue)
{
    if (!strcmp("name", attrName)) *pAttrValue = m_Name;
    else if (!strcmp("email", attrName)) *pAttrValue = m_Email;
    else if (!strcmp("phone", attrName)) *pAttrValue = m_Phone;
    else if (!strcmp("mobile", attrName)) *pAttrValue = m_Mobile;
    else if (!strcmp("address", attrName)) *pAttrValue = m_Address;
    else if (!strcmp("affiliation", attrName)) *pAttrValue = m_Affiliation;
    else return E_INVALID_ARGUMENT;

    return NOERROR;
}

ECode CAuthorInfo::SetAuthorInfo(
    /* [in] */ String attrName,
    /* [in] */ String attrValue)
{
    if (!strcmp("name", attrName)) m_Name = String::Duplicate(attrValue);
    else if (!strcmp("email", attrName)) m_Email = String::Duplicate(attrValue);
    else if (!strcmp("phone", attrName)) m_Phone = String::Duplicate(attrValue);
    else if (!strcmp("mobile", attrName)) m_Mobile = String::Duplicate(attrValue);
    else if (!strcmp("address", attrName)) m_Address = String::Duplicate(attrValue);
    else if (!strcmp("affiliation", attrName)) m_Affiliation = String::Duplicate(attrValue);
    else return E_INVALID_ARGUMENT;

    return NOERROR;
}

