
#ifndef __CANNOTATION_H__
#define __CANNOTATION_H__

#include "_CAnnotation.h"
#include "Annotation.h"
CarClass(CAnnotation), public Annotation
{
public:
    CARAPI constructor(
        /* [in] */ IInterface * pAttribute);

    CARAPI GetValue(
        /* [out] */ IInterface ** ppValue);

private:
    // TODO: Add your private member variables here.
};

#endif // __CANNOTATION_H__
