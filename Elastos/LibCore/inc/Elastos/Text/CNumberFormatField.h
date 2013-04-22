
#ifndef __CNUMBERFORMATFIELD_H__
#define __CNUMBERFORMATFIELD_H__

#include "_CNumberFormatField.h"
#include "NumberFormat.h"


CarClass(CNumberFormatField), public NumberFormat::Field
{
public:
    CARAPI GetName(
        /* [out] */ String * name);

    CARAPI constructor(
        /* [in] */ const String& fieldName);

private:
    // TODO: Add your private member variables here.
};

#endif // __CNUMBERFORMATFIELD_H__
