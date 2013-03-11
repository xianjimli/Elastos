
#include "webkit/CWebIconDatabase.h"

#include <elastos/Mutex.h>

const char* CWebIconDatabase::LOGTAG = "WebIconDatabase";

CWebIconDatabase* CWebIconDatabase::sIconDatabase = NULL;

ECode CWebIconDatabase::Open(
    /* [in] */ CString path)
{
    if (path.GetLength() != 0)
    {
        // Make the directories and parents if they don't exist
        AutoPtr<IFile> db = NULL;
        IFile* _db = NULL;
        
        CFile::New((String)path, &_db);
        db = _db;

        Boolean bExists = false;
        db->Exists(&bExists);
        if (!bExists)
        {
            Boolean bMkdirs = false;
            db->Mkdirs(&bMkdirs);
        }
//        mEventHandler.postMessage(Message.obtain(null, EventHandler.OPEN, db.getAbsolutePath()));
    }

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebIconDatabase::Close()
{
//    mEventHandler.postMessage(Message.obtain(null, EventHandler.CLOSE));

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebIconDatabase::RemoveAllIcons()
{
//    mEventHandler.postMessage(Message.obtain(null, EventHandler.REMOVE_ALL));

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebIconDatabase::RequestIconForPageUrl(
    /* [in] */ CString url,
    /* [in] */ IWebIconDatabaseIconListener * pListener)
{
    if (pListener == NULL || url.GetLength() == 0)
    {
        return E_INVALID_ARGUMENT;
    }

//    Message msg = Message.obtain(null, EventHandler.REQUEST_ICON, listener);
//    msg.getData().putString("url", url);
//    mEventHandler.postMessage(msg);

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebIconDatabase::BulkRequestIconForPageUrl(
    /* [in] */ IContentResolver * pCr,
    /* [in] */ CString where,
    /* [in] */ IWebIconDatabaseIconListener * pListener)
{
    if (pListener == NULL || pCr == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    // Special case situation: we don't want to add this message to the
    // queue if there is no handler because we may never have a real
    // handler to service the messages and the cursor will never get
    // closed.
/*    if (mEventHandler.hasHandler())
    {
        // Don't use Bundle as it is parcelable.
        HashMap<String, Object> map = new HashMap<String, Object>();
        map.put("contentResolver", cr);
        map.put("where", where);
        map.put("listener", listener);
        Message msg =
                Message.obtain(null, EventHandler.BULK_REQUEST_ICON, map);
        mEventHandler.postMessage(msg);
    }*/

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebIconDatabase::RetainIconForPageUrl(
    /* [in] */ CString url)
{
    if (url.GetLength() != 0)
    {
//        mEventHandler.postMessage(Message.obtain(null, EventHandler.RETAIN_ICON, url));
    }

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebIconDatabase::ReleaseIconForPageUrl(
    /* [in] */ CString url)
{
    if (url.GetLength() != 0)
    {
//        mEventHandler.postMessage(Message.obtain(null, EventHandler.RELEASE_ICON, url));
    }

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebIconDatabase::GetInstance(
    /* [out] */ IWebIconDatabase ** ppInstance)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Create the internal handler and transfer all pending messages.
 * XXX: Called by WebCore thread only!
 */
/*package*/
CARAPI_(void) CWebIconDatabase::CreateHandler()
{
    mEventHandler->CreateHandler();
}

/********************************EventHandler********************************/
//@Override
CARAPI_(void) CWebIconDatabase::EventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if (msg == NULL)
    {
        return;
    }

#if 0
    // Note: This is the message handler for the UI thread.
    switch (msg->what)
    {
        case ICON_RESULT:
            ((IconResult) msg.obj).dispatch();
            break;
    }
#endif
}

// Called by WebCore thread to create the actual handler
/*synchronized*/
CARAPI_(void) CWebIconDatabase::EventHandler::CreateHandler()
{
    Mutex mutex;
    Mutex::Autolock lock(mutex);

    if (mHandler == NULL)
    {
//        mHandler = new Handler();

        class newHandler : public IHandler
        {
        public:
            //@Override
            CARAPI_(void) HandleMessage(IMessage* msg)
            {
#if 0
                // Note: This is the message handler for the WebCore
                // thread.
                switch (msg->what)
                {
                    case OPEN:
                        nativeOpen((String) msg.obj);
                        break;

                    case CLOSE:
                        nativeClose();
                        break;

                    case REMOVE_ALL:
                        nativeRemoveAllIcons();
                        break;

                    case REQUEST_ICON:
                        IconListener l = (IconListener) msg.obj;
                        String url = msg.getData().getString("url");
                        requestIconAndSendResult(url, l);
                        break;

                    case BULK_REQUEST_ICON:
                        bulkRequestIcons(msg);
                        break;

                    case RETAIN_ICON:
                        nativeRetainIconForPageUrl((String) msg.obj);
                        break;

                    case RELEASE_ICON:
                        nativeReleaseIconForPageUrl((String) msg.obj);
                        break;
                }
#endif                
            }
        };
#if 0
        // Transfer all pending messages
        for (int size = mMessages.size(); size > 0; size--) {
            mHandler.sendMessage(mMessages.remove(0));
        }
        mMessages = null;
#endif
    }
}

/*synchronized*/
CARAPI_(Boolean) CWebIconDatabase::EventHandler::HasHandler()
{
    Mutex mutex;
    Mutex::Autolock lock(mutex);

    return mHandler != NULL;
}

/*synchronized*/
CARAPI_(void) CWebIconDatabase::EventHandler::PostMessage(
    /* [in] */ IMessage* msg)
{}

CARAPI_(void) CWebIconDatabase::EventHandler::BulkRequestIcons(
    /* [in] */ IMessage* msg)
{}

CARAPI_(void) CWebIconDatabase::EventHandler::RequestIconAndSendResult(
    /* [in] */ const String& url,
    /* [in] */ IWebIconDatabaseIconListener* listener)
{}

/**************************EventHandler::IconResult*************************/
CWebIconDatabase::EventHandler::IconResult::IconResult(
    /* [in] */ const String& url,
    /* [in] */ IBitmap* icon,
    /* [in] */ IWebIconDatabaseIconListener* l)
{
    mUrl = url;
    mIcon = icon;
    mListener = l;
}

void CWebIconDatabase::EventHandler::IconResult::dispatch()
{
//    mListener->OnReceivedIcon(mUrl, mIcon);
}
