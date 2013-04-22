
#ifndef __CMESSAGEFORMATFIELD_H__
#define __CMESSAGEFORMATFIELD_H__

#include "_CMessageFormatField.h"
#include "MessageFormat.h"

CarClass(CMessageFormatField), public MessageFormat::MessageFormatField
{
public:
    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI constructor(
        /* [in] */ const String& name);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMESSAGEFORMATFIELD_H__
