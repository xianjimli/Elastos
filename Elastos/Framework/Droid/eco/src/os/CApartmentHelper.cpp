
#include "ext/frameworkdef.h"
#include "os/CApartmentHelper.h"
#include "os/CApartment.h"

ECode CApartmentHelper::GetDefaultApartment(
    /* [out] */ IApartment** apartment)
{
    VALIDATE_NOT_NULL(apartment);

    *apartment = (CApartment*)pthread_getspecific(CApartment::sKey);
    if (*apartment != NULL) (*apartment)->AddRef();

    return NOERROR;
}
