
#ifndef __WEBBACKFORWARDLISTCLIENT_H__
#define __WEBBACKFORWARDLISTCLIENT_H__

class WebHistoryItem;

/**
 * Interface to receive notifications when items are added to the
 * {@link WebBackForwardList}.
 * {@hide}
 */
class CWebBackForwardListClient {

public:
    /**
     * Notify the client that <var>item</var> has been added to the
     * WebBackForwardList.
     * @param item The newly created WebHistoryItem
     */
	virtual CARAPI_(void) OnNewHistoryItem(
		/* [in] */ WebHistoryItem* item) = 0;

    /**
     * Notify the client that the <var>item</var> at <var>index</var> is now
     * the current history item.
     * @param item A WebHistoryItem
     * @param index The new history index
     */
	virtual CARAPI_(void) OnIndexChanged(
		/* [in] */ WebHistoryItem* item, 
		/* [in] */ Int32 index) = 0;
};

#endif //__WEBBACKFORWARDLISTCLIENT_H__