
#ifndef __MEDIASCANNERCONNECTION_H__
#define __MEDIASCANNERCONNECTION_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * MediaScannerConnection provides a way for applications to pass a
 * newly created or downloaded media file to the media scanner service.
 * The media scanner service will read metadata from the file and add
 * the file to the media content provider.
 * The MediaScannerConnectionClient provides an interface for the
 * media scanner service to return the Uri for a newly scanned file
 * to the client of the MediaScannerConnection class.
 */
class MediaScannerConnection
{
public:
    MediaScannerConnection();

    /**
     * Constructs a new MediaScannerConnection object.
     * @param context the Context object, required for establishing a connection to
     * the media scanner service.
     * @param client an optional object implementing the MediaScannerConnectionClient
     * interface, for receiving notifications from the media scanner.
     */
    MediaScannerConnection(
        /* [in] */ IContext* context, 
        /* [in] */ IMediaScannerConnectionClient* client);

    /**
     * Initiates a connection to the media scanner service.
     * {@link MediaScannerConnectionClient#onMediaScannerConnected()}
     * will be called when the connection is established.
     */
    virtual CARAPI Connect();

    /**
     * Releases the connection to the media scanner service.
     */
    virtual CARAPI Disconnect();

    /**
     * Returns whether we are connected to the media scanner service
     * @return true if we are connected, false otherwise
     */
    virtual CARAPI_(Boolean) IsConnected();

    /**
     * Requests the media scanner to scan a file.
     * Success or failure of the scanning operation cannot be determined until
     * {@link MediaScannerConnectionClient#onScanCompleted(String, Uri)} is called.
     *
     * @param path the path to the file to be scanned.
     * @param mimeType  an optional mimeType for the file.
     * If mimeType is NULL, then the mimeType will be inferred from the file extension.
     */
     virtual CARAPI ScanFile(
         /* [in] */ String path, 
         /* [in] */ String mimeType);

     class ClientProxy : public IMediaScannerConnectionClient, public ElRefBase
     {
     public:
        ClientProxy(
            /* [in] */ ArrayOf<String>* paths, 
            /* [in] */ ArrayOf<String>* mimeTypes, 
            /* [in] */ IOnScanCompletedListener* client);

        virtual CARAPI OnMediaScannerConnected();

        virtual CARAPI OnScanCompleted(
            /* [in] */ const String& path, 
            /* [in] */ IUri* uri);

        virtual CARAPI ScanNextPath();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        ArrayOf<String>* mPaths;
        ArrayOf<String>* mMimeTypes;
        AutoPtr<IOnScanCompletedListener> mClient;
        MediaScannerConnection* mConnection;
        Int32 mNextPath;
    };

    /**
     * Convenience for constructing a {@link MediaScannerConnection}, calling
     * {@link #connect} on it, and calling {@link #scanFile} with the given
     * <var>path</var> and <var>mimeType</var> when the connection is
     * established.
     * @param context The caller's Context, required for establishing a connection to
     * the media scanner service.
     * Success or failure of the scanning operation cannot be determined until
     * {@link MediaScannerConnectionClient#onScanCompleted(String, Uri)} is called.
     * @param paths Array of paths to be scanned.
     * @param mimeTypes Optional array of MIME types for each path.
     * If mimeType is NULL, then the mimeType will be inferred from the file extension.
     * @param callback Optional callback through which you can receive the
     * scanned URI and MIME type; If NULL, the file will be scanned but
     * you will not get a result back.
     * @see scanFile(String, String)
     */
    virtual CARAPI ScanFile(
        /* [in] */ IContext* context, 
        /* [in] */ ArrayOf<String>* paths, 
        /* [in] */ ArrayOf<String>* mimeTypes,
        /* [in] */ IOnScanCompletedListener* callback);

    /**
     * Part of the ServiceConnection interface.  Do not call.
     */
    virtual CARAPI OnServiceConnected(
        /* [in] */ IComponentName* className, 
        /* [in] */ IBinder* service);

    /**
     * Part of the ServiceConnection interface.  Do not call.
     */
    virtual CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName* className);

private:
    static String TAG;// = "MediaScannerConnection";

    AutoPtr<IContext> mContext;
    AutoPtr<IMediaScannerConnectionClient> mClient;
    AutoPtr<IMediaScannerService> mService;
    Boolean mConnected; // true if connect() has been called since last disconnect()

    /*private IMediaScannerListener.Stub mListener = new IMediaScannerListener.Stub() {
        public void scanCompleted(String path, Uri uri) {
            MediaScannerConnectionClient client = mClient;
            if (client != NULL) {
                client.onScanCompleted(path, uri);
            }
        }
    };*/
};

#endif
