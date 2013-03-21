
#ifndef __CTEXTATTRIBUTE_H__
#define __CTEXTATTRIBUTE_H__

#include "_CTextAttribute.h"
#include "TextAttribute.h"
CarClass(CTextAttribute), public TextAttribute
{
public:
    CARAPI ReadResolve(
        /* [out] */ IInterface ** ppResolve);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI constructor(
        /* [in] */ const String& name);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTEXTATTRIBUTE_H__
