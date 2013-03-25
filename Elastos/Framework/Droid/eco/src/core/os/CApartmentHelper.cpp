
#include "ext/frameworkdef.h"
#include "os/CApartmentHelper.h"
#include "os/CApartment.h"


ECode CApartmentHelper::Prepare()
{
    return CApartment::Prepare();
}

    /** Initialize the current thread as a looper, marking it as an application's main
 *  looper. The main looper for your application is created by the Android environment,
 *  so you should never need to call this function yourself.
 * {@link #prepare()}
 */
ECode CApartmentHelper::PrepareMainApartment()
{
    return CApartment::PrepareMainApartment();
}

ECode CApartmentHelper::GetMainApartment(
    /* [out] */ IApartment** apartment)
{
    VALIDATE_NOT_NULL(apartment);

    return CApartment::GetMainApartment(apartment);
}

ECode CApartmentHelper::GetMyApartment(
    /* [out] */ IApartment** apartment)
{
    VALIDATE_NOT_NULL(apartment);

    return CApartment::GetMyApartment(apartment);
}

ECode CApartmentHelper::GetNativeMessageQueue(
    /* [out] */ Handle32* messageQueue)
{
    VALIDATE_NOT_NULL(messageQueue);

    NativeMessageQueue*  queue = CApartment::GetNativeMessageQueue();
    *messageQueue = (Handle32)queue;

    return NOERROR;
}
