#ifndef __IWEBBACKFORWARDLISTCLIENT_H__
#define __IWEBBACKFORWARDLISTCLIENT_H__

/**
 * Interface to receive notifications when items are added to the
 * {@link WebBackForwardList}.
 * {@hide}
 */
class IWebBackForwardListClient {

public:
    /**
     * Notify the client that <var>item</var> has been added to the
     * WebBackForwardList.
     * @param item The newly created WebHistoryItem
     */
	virtual CARAPI_(void) OnNewHistoryItem(
		/* [in] */ IWebHistoryItem* item) = 0;

    /**
     * Notify the client that the <var>item</var> at <var>index</var> is now
     * the current history item.
     * @param item A WebHistoryItem
     * @param index The new history index
     */
	virtual CARAPI_(void) OnIndexChanged(
		/* [in] */ IWebHistoryItem* item, 
		/* [in] */ Int32 index) = 0;
};

#endif //__IWEBBACKFORWARDLISTCLIENT_H__