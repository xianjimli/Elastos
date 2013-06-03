
#ifndef __CWEBHISTORYITEM_H__
#define __CWEBHISTORYITEM_H__

#include "_CWebHistoryItem.h"

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include <elastos/Mutex.h>
#include "webkit/WebHistoryItem.h"

CarClass(CWebHistoryItem),public WebHistoryItem
{
public:
    //friend class CWebHistoryItem;

    /*public*/
    CARAPI GetId(
        /* [out] */ Int32 * id);

    /*public*/
    CARAPI GetUrl(
        /* [out] */ String * url);

    /*public*/
    CARAPI GetOriginalUrl(
        /* [out] */ String * originalUrl);

    /*public*/
    CARAPI GetTitle(
        /* [out] */ String * title);

    /*public*/
    CARAPI GetFavicon(
        /* [out] */ IBitmap ** favicon);

    /*public*/
    CARAPI GetTouchIconUrl(
        /* [out] */ String * touchIconUrl);

    /*public*/
    CARAPI GetCustomData(
        /* [out] */ IInterface ** customData);

    /*public*/
    CARAPI SetCustomData(
        /* [in] */ IInterface * data);

    /**
     * Construct a new WebHistoryItem with initial flattened data.
     * @param data The pre-flattened data coming from restoreState.
     */
    /*package*/
    CARAPI constructor(
        /* [in] */ ArrayOf<byte>* data);

protected:
    /**
     * Clone the history item for use by clients of WebView.
     */
    //synchronized
    CARAPI Clone(
        /* [out] */ IWebHistoryItem** item);
        
private:
    /**
     * Basic constructor that assigns a unique id to the item. Called by JNI
     * only.
     */
    CARAPI constructor();

    /**
     * Construct a clone of a WebHistoryItem from the given item.
     * @param item The history item to clone.
     */
    //CARAPI constructor(IWebHistoryItem* item);


    CARAPI constructor(
        /* [in] */ IWebHistoryItem* item);

};

#endif // __CWEBHISTORYITEM_H__
