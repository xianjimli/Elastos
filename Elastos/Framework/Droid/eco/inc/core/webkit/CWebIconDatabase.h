
#ifndef __CWEBICONDATABASE_H__
#define __CWEBICONDATABASE_H__

#include "_CWebIconDatabase.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

CarClass(CWebIconDatabase)
{
public:
    CARAPI Open(
        /* [in] */ CString path);

    CARAPI Close();

    CARAPI RemoveAllIcons();

    CARAPI RequestIconForPageUrl(
        /* [in] */ CString url,
        /* [in] */ IWebIconDatabaseIconListener* listener);

    CARAPI BulkRequestIconForPageUrl(
        /* [in] */ IContentResolver* cr,
        /* [in] */ CString where,
        /* [in] */ IWebIconDatabaseIconListener* listener);

    CARAPI RetainIconForPageUrl(
        /* [in] */ CString url);

    CARAPI ReleaseIconForPageUrl(
        /* [in] */ CString url);

    CARAPI GetInstance(
        /* [out] */ IWebIconDatabase** instance);

public:
    /**
     * Create the internal handler and transfer all pending messages.
     * XXX: Called by WebCore thread only!
     */
    /*package*/
    CARAPI_(void) CreateHandler();

private:

    static const CString LOGTAG;// = "WebIconDatabase";

private:
    // Class to handle messages before WebCore is ready
    class EventHandler : public IHandler
    {
        friend class CWebIconDatabase;

    public:
        // Message ids
        static const Int32 OPEN         = 0;
        static const Int32 CLOSE        = 1;
        static const Int32 REMOVE_ALL   = 2;
        static const Int32 REQUEST_ICON = 3;
        static const Int32 RETAIN_ICON  = 4;
        static const Int32 RELEASE_ICON = 5;
        static const Int32 BULK_REQUEST_ICON = 6;

    public:
        //@Override
        CARAPI_(void) HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        // Message for dispatching icon request results
        static const Int32 ICON_RESULT = 10;

    private:
        
        // Class to handle a result dispatch
        class IconResult
        {
        public:
            IconResult(
                /* [in] */ const String& url,
                /* [in] */ IBitmap* icon,
                /* [in] */ IWebIconDatabaseIconListener* l);
            
            void dispatch();

        private:
            String mUrl;
            const IBitmap* mIcon;
            const IWebIconDatabaseIconListener* mListener;            
        };

    private:       

        // Called by WebCore thread to create the actual handler
        /*synchronized*/
        CARAPI_(void) CreateHandler();

        /*synchronized*/
        CARAPI_(Boolean) HasHandler();

        /*synchronized*/
        CARAPI_(void) PostMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(void) BulkRequestIcons(
            /* [in] */ IMessage* msg);

        CARAPI_(void) RequestIconAndSendResult(
            /* [in] */ const String& url,
            /* [in] */ IWebIconDatabaseIconListener* listener);

    private:
        // Actual handler that runs in WebCore thread
        AutoPtr<IHandler> mHandler;
        // Vector of messages before the WebCore thread is ready
        Vector<AutoPtr<IMessage> > mMessages;
    };

private:
    /**
     * Private constructor to avoid anyone else creating an instance.
     */
    WebIconDatabase();

    // Native functions
    static CARAPI_(void) NativeOpen(
        /* [in] */ const String& path);

    static CARAPI_(void) NativeClose();

    static CARAPI_(void) NativeRemoveAllIcons();

    static CARAPI_(AutoPtr<IBitmap>) NativeIconForPageUrl(
        /* [in] */ const String& url);

    static CARAPI_(void) NativeRetainIconForPageUrl(
        /* [in] */ const String& url);

    static CARAPI_(void) NativeReleaseIconForPageUrl(
        /* [in] */ const String& url);

private:

    // Global instance of a WebIconDatabase
    static CWebIconDatabase* sIconDatabase;
    // EventHandler for handling messages before and after the WebCore thread is
    // ready.
    /*const*/ EventHandler* mEventHandler;
};

#endif // __CWEBICONDATABASE_H__
