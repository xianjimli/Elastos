
#ifndef __CNUMBERFORMATFIELD_H__
#define __CNUMBERFORMATFIELD_H__

#include "_CNumberFormatField.h"
#include "NumberFormat.h"


CarClass(CNumberFormatField), public NumberFormat::Field
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    CARAPI GetClassID(
         /* [out] */ ClassID* clsid);

    CARAPI GetName(
        /* [out] */ String* name);

private:
    // TODO: Add your private member variables here.
};

#endif // __CNUMBERFORMATFIELD_H__
