
#ifndef __CWEBBACKFORWARDLIST_H__
#define __CWEBBACKFORWARDLIST_H__

#include "_CWebBackForwardList.h"

//#include "webkit/CWebHistoryItem.h"
//#include "webkit/CCallbackProxy.h"
#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include "webkit/WebBackForwardList.h"

CarClass(CWebBackForwardList), public WebBackForwardList
{
public:
    //synchronized
    /*public*/
    CARAPI GetCurrentItem(
        /* [out] */ IWebHistoryItem** item);

    /*public*/
    CARAPI GetCurrentIndex(
        /* [out] */ Int32* index);

    /*public*/
    CARAPI GetItemAtIndex(
        /* [in] */ Int32 index,
        /* [out] */ IWebHistoryItem** item);

    /*public*/
    CARAPI GetSize(
        /* [out] */ Int32* size);

    /*package*/
    CARAPI constructor(
        /* [in] */ ICallbackProxy* proxy);

protected:
    /**
     * Clone the entire object to be used in the UI thread by clients of
     * WebView. This creates a copy that should never be modified by any of the
     * webkit package classes.
     */
    //synchronized 
    CARAPI Clone(
        /* [in] */ IWebBackForwardList** webBackForwardList);

};

#endif // __CWEBBACKFORWARDLIST_H__
