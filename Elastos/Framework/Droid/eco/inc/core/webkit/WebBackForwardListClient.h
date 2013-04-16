
#ifndef __WEBBACKFORWARDLISTCLIENT_H__
#define __WEBBACKFORWARDLISTCLIENT_H__

#include <elastos/ElRefBase.h>

class WebHistoryItem;

/**
 * Interface to receive notifications when items are added to the
 * {@link WebBackForwardList}.
 * {@hide}
 */
class CWebBackForwardListClient : public ElRefBase {

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

public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid)
        {
            if (riid == EIID_IInterface) {
                return (IInterface*)(IHandler*)this;
            }
            else if (riid == EIID_IHandler) {
                return (IHandler*)this;
            }
            return NULL;
        }

        CARAPI_(UInt32) AddRef()
        {
            return ElRefBase::AddRef();
        }

        CARAPI_(UInt32) Release()
        {
            return ElRefBase::Release();
        }

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *object,
            /* [out] */ InterfaceID *IID)
        {

            VALIDATE_NOT_NULL(IID);
            if (IID == NULL) {
                return E_INVALID_ARGUMENT;
            }

            if (object == (IInterface*)(IHandler*)this) {
                *IID = EIID_IHandler;
            }
            else {
                return E_INVALID_ARGUMENT;
            }
            return NOERROR;
        }
};

#endif //__WEBBACKFORWARDLISTCLIENT_H__