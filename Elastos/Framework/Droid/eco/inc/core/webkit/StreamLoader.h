#ifndef __STREAMLOADER_H__
#define __STREAMLOADER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "webkit/LoadListener.h"
#include <elastos/Mutex.h>
#include <elastos/AutoFree.h>

//class LoadListener;

/**
 * This abstract class is used for all content loaders that rely on streaming
 * content into the rendering engine loading framework.
 *
 * The class implements a state machine to load the content into the frame in
 * a similar manor to the way content arrives from the network. The class uses
 * messages to move from one state to the next, which enables async. loading of
 * the streamed content.
 *
 * Classes that inherit from this class must implement two methods, the first
 * method is used to setup the InputStream and notify the loading framework if
 * it can load it's content. The other method allows the derived class to add
 * additional HTTP headers to the response.
 *
 * By default, content loaded with a StreamLoader is marked with a HTTP header
 * that indicates the content should not be cached.
 *
 */
class StreamLoader //: public IHandlerCallback
{
public:
    /**
     * Constructor. Although this class calls the LoadListener, it only calls
     * the EventHandler Interface methods. LoadListener concrete class is used
     * to avoid the penality of calling an interface.
     *
     * @param loadlistener The LoadListener to call with the data.
     */
    //package
    StreamLoader(
        /* [in] */ /*const*/ LoadListener * loadListener);

    /**
     * Calling this method starts the load of the content for this StreamLoader.
     * This method simply creates a Handler in the current thread and posts a
     * message to send the status and returns immediately.
     */
    /*final*/
    //package
    CARAPI_(void) Load();

    //public
    virtual CARAPI_(Boolean) HandleMessage(
        /* [in] */ const IMessage* msg);

protected:
    /*const*/ AutoPtr<IContext>  mContext;
    /*const*/ AutoPtr<LoadListener>  mLoadListener; // loader class
    /*const*/ AutoPtr<IInputStream>  mDataStream; // stream to read data from
    long mContentLength; // content length of data

    /**
     * This method is called when the derived class should setup mDataStream,
     * and call mLoadListener.status() to indicate that the load can occur. If it
     * fails to setup, it should still call status() with the error code.
     *
     * @return true if stream was successfully setup
     */
    virtual CARAPI_(Boolean) SetupStreamAndSendStatus();

    /**
     * This method is called when the headers are about to be sent to the
     * load framework. The derived class has the opportunity to add addition
     * headers.
     *
     * @param headers Map of HTTP headers that will be sent to the loader.
     */
    virtual CARAPI_(void) BuildHeaders(
        /* [in] */ const IHeaders* headers);

private:
    /**
     * Construct the headers and pass them to the EventHandler.
     */
    CARAPI_(void) SendHeaders();

    /**
     * Read data from the stream and pass it to the EventHandler.
     * If an error occurs reading the stream, then an error is sent to the
     * EventHandler, and moves onto the next state - end of data.
     * @return True if all the data has been read. False if sendData should be
     *         called again.
     */
    CARAPI_(Boolean) SendData();

    /**
     * Close the stream and inform the EventHandler that load is complete.
     */
    CARAPI_(void) CloseStreamAndSendEndData();

private:
    static const Int32 MSG_STATUS = 100;  // Send status to loader
    static const Int32 MSG_HEADERS = 101; // Send headers to loader
    static const Int32 MSG_DATA = 102;  // Send data to loader
    static const Int32 MSG_END = 103;  // Send endData to loader

    AutoFree < ArrayOf<Byte> > mData; // buffer to pass data to loader with.

    // Handler which will be initialized in the thread where load() is called.
    AutoPtr<IHandler> mHandler;

    Elastos::Core::Threading::Mutex mMutex;
};

#endif //__STREAMLOADER_H__