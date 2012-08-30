#ifndef __CONTENTOBSERVER_H__
#define __CONTENTOBSERVER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
/**
 * Receives call backs for changes to content. Must be implemented by objects which are added
 * to a {@link ContentObservable}.
 */
using namespace Elastos::Core::Threading;
class ContentObserver
{
public:
    ContentObserver();

    ~ContentObserver();

    virtual CARAPI DeliverSelfNotifications(
        /* [out] */ Boolean* rst);

    virtual CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);
private:
//    private Transport mTransport;
    Mutex lock;



};
#endif //__CONTENTOBSERVER_H__