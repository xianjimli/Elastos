
#include "ext/frameworkdef.h"
#include "os/CApartmentHelper.h"
#include "os/CApartment.h"

ECode CApartmentHelper::GetDefaultApartment(
    /* [out] */ IApartment** apartment)
{
    VALIDATE_NOT_NULL(apartment);

    *apartment = CApartment::sDefaultApartment; //(CApartment*)pthread_getspecific(CApartment::sKey);
    if (*apartment != NULL) (*apartment)->AddRef();

    return NOERROR;
}

ECode CApartmentHelper::GetNativeMessageQueue(
    /* [out] */ Handle32* messageQueue)
{
    VALIDATE_NOT_NULL(messageQueue);

    CApartment* apartment = CApartment::sDefaultApartment; //(CApartment*)pthread_getspecific(CApartment::sKey);
    if (apartment != NULL) {
        *messageQueue = (Handle32)apartment->mMessageQueue;
    }
    else {
        *messageQueue = NULL;
    }
    return NOERROR;
}
