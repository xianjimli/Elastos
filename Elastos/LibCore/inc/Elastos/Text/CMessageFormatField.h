
#ifndef __CMESSAGEFORMATFIELD_H__
#define __CMESSAGEFORMATFIELD_H__

#include "_CMessageFormatField.h"
#include "MessageFormat.h"

CarClass(CMessageFormatField), public MessageFormat::MessageFormatField
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI GetClassID(
         /* [out] */ ClassID* clsid);

    CARAPI GetName(
        /* [out] */ String* name);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMESSAGEFORMATFIELD_H__
