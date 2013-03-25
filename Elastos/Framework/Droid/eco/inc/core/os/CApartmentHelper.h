
#ifndef __CAPARTMENTHELPER_H__
#define __CAPARTMENTHELPER_H__

#include "_CApartmentHelper.h"

CarClass(CApartmentHelper)
{
public:
    /** Initialize the current thread as a looper.
      * This gives you a chance to create handlers that then reference
      * this looper, before actually starting the loop. Be sure to call
      * {@link #loop()} after calling this method, and end it by calling
      * {@link #quit()}.
      */
    CARAPI Prepare();

    /** Initialize the current thread as a looper, marking it as an application's main
     *  looper. The main looper for your application is created by the Android environment,
     *  so you should never need to call this function yourself.
     * {@link #prepare()}
     */
    CARAPI PrepareMainApartment();

    CARAPI GetMainApartment(
        /* [out] */ IApartment** apartment);

    CARAPI GetMyApartment(
        /* [out] */ IApartment** apartment);

    CARAPI GetNativeMessageQueue(
        /* [out] */ Handle32* messageQueue);
};


#endif //__CAPARTMENTHELPER_H__
