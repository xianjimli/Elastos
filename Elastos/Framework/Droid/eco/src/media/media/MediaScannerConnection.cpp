
#include "media/MediaScannerConnection.h"

MediaScannerConnection::MediaScannerConnection()
{

}

/**
 * Constructs a new MediaScannerConnection object.
 * @param context the Context object, required for establishing a connection to
 * the media scanner service.
 * @param client an optional object implementing the MediaScannerConnectionClient
 * interface, for receiving notifications from the media scanner.
 */
MediaScannerConnection::MediaScannerConnection(
    /* [in] */ IContext* context, 
    /* [in] */ IMediaScannerConnectionClient* client) 
{
    mContext = context;
    mClient = client;
}

/**
 * Initiates a connection to the media scanner service.
 * {@link MediaScannerConnectionClient#onMediaScannerConnected()}
 * will be called when the connection is established.
 */
ECode MediaScannerConnection::Connect()
{
    //synchronized (this) {
        if (!mConnected) {
            AutoPtr<IIntent> intent;// = new Intent(IMediaScannerService.class.getName());
            //mContext->BindService(intent, this, Context.BIND_AUTO_CREATE);
            mConnected = TRUE;
        }
    //}

        return NOERROR;
}

/**
 * Releases the connection to the media scanner service.
 */
ECode MediaScannerConnection::Disconnect() 
{
    /*synchronized (this) {
        if (mConnected) {
            if (Config.LOGV) {
                Log.v(TAG, "Disconnecting from Media Scanner");
            }
            try {
                mContext.unbindService(this);
            } catch (IllegalArgumentException ex) {
                if (Config.LOGV) {
                    Log.v(TAG, "disconnect failed: " + ex);
                }
            }
            mConnected = FALSE;
        }
    }*/

    return NOERROR;
}

/**
 * Returns whether we are connected to the media scanner service
 * @return true if we are connected, false otherwise
 */
Boolean MediaScannerConnection::IsConnected()
{
    return (mService != NULL && mConnected);
}

/**
 * Requests the media scanner to scan a file.
 * Success or failure of the scanning operation cannot be determined until
 * {@link MediaScannerConnectionClient#onScanCompleted(String, Uri)} is called.
 *
 * @param path the path to the file to be scanned.
 * @param mimeType  an optional mimeType for the file.
 * If mimeType is NULL, then the mimeType will be inferred from the file extension.
 */
ECode MediaScannerConnection::ScanFile(
    /* [in] */ String path, 
    /* [in] */ String mimeType) 
{
    /*synchronized (this) {
        if (mService == NULL || !mConnected) {
            throw new IllegalStateException("not connected to MediaScannerService");
        }
        try {
            if (Config.LOGV) {
                Log.v(TAG, "Scanning file " + path);
            }
            mService.requestScanFile(path, mimeType, mListener);
        } catch (RemoteException e) {
            if (Config.LOGD) {
                Log.d(TAG, "Failed to scan file " + path);
            }
        }
    }*/

    return NOERROR;
}



MediaScannerConnection::ClientProxy::ClientProxy(
    /* [in] */ ArrayOf<String>* paths, 
    /* [in] */ ArrayOf<String>* mimeTypes, 
    /* [in] */ IOnScanCompletedListener* client) 
{
    mPaths = paths;
    mMimeTypes = mimeTypes;
    mClient = client;
}

ECode MediaScannerConnection::ClientProxy::OnMediaScannerConnected()
{
    return ScanNextPath();
}

ECode MediaScannerConnection::ClientProxy::OnScanCompleted(
    /* [in] */ const String& path, 
    /* [in] */ IUri* uri)
{
    if (mClient != NULL) {
        mClient->OnScanCompleted(path, uri);
    }
    return ScanNextPath();
}

ECode MediaScannerConnection::ClientProxy::ScanNextPath()
{
    if (mNextPath >= mPaths->GetLength()) {
        mConnection->Disconnect();
        return NOERROR;
    }
    String mimeType = mMimeTypes != NULL ? (*mMimeTypes)[mNextPath] : String("");
    mConnection->ScanFile((*mPaths)[mNextPath], mimeType);
    mNextPath++;

    return NOERROR;
}

PInterface MediaScannerConnection::ClientProxy::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 MediaScannerConnection::ClientProxy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MediaScannerConnection::ClientProxy::Release()
{
    return ElRefBase::Release();
}

ECode MediaScannerConnection::ClientProxy::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}


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
ECode MediaScannerConnection::ScanFile(
    /* [in] */ IContext* context, 
    /* [in] */ ArrayOf<String>* paths, 
    /* [in] */ ArrayOf<String>* mimeTypes,
    /* [in] */ IOnScanCompletedListener* callback)
{
    ClientProxy* client = new ClientProxy(paths, mimeTypes, callback);
    MediaScannerConnection* connection = new MediaScannerConnection(context, client);
    client->mConnection = connection;
    connection->Connect();

    return NOERROR;
}

/**
 * Part of the ServiceConnection interface.  Do not call.
 */
ECode MediaScannerConnection::OnServiceConnected(
    /* [in] */ IComponentName* className, 
    /* [in] */ IBinder* service) 
{
    /*if (Config.LOGV) {
        Log.v(TAG, "Connected to Media Scanner");
    }
    synchronized (this) {
        mService = IMediaScannerService.Stub.asInterface(service);
        if (mService != NULL && mClient != NULL) {
            mClient.onMediaScannerConnected();
        }
    }*/

    return NOERROR;
}

/**
 * Part of the ServiceConnection interface.  Do not call.
 */
ECode MediaScannerConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* className) 
{
    /*if (Config.LOGV) {
        Log.v(TAG, "Disconnected from Media Scanner");
    }
    synchronized (this) {*/
        mService = NULL;
    //}

        return NOERROR;
}

