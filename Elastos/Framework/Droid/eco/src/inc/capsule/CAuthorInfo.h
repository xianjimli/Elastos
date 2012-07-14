
#ifndef __CAUTHORINFO_H__
#define __CAUTHORINFO_H__

#include "_CAuthorInfo.h"

CarClass(CAuthorInfo)
{
public:
    CAuthorInfo();
    
    ~CAuthorInfo();
    
    CARAPI GetAuthorInfo(
        /* [in] */ const String& attrName,
        /* [out] */ String * pAttrValue);

    CARAPI SetAuthorInfo(
        /* [in] */ const String& attrName,
        /* [in] */ const String& attrValue);

private:
    String m_Name;
    String m_Email;
    String m_Phone;
    String m_Mobile;
    String m_Address;
    String m_Affiliation;
};

#endif // __CAUTHORINFO_H__