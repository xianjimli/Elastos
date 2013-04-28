
#ifndef __CPLAINSOCKETIMPL_H__
#define __CPLAINSOCKETIMPL_H__

#include "_CPlainSocketImpl.h"
#include "PlainSocketImpl.h"

CarClass(CPlainSocketImpl), public PlainSocketImpl
{
public:
    CARAPI GetOption(
        /* [in] */ Int32 optID,
        /* [out] */ IInterface ** ppOptVal);

    CARAPI SetOption(
        /* [in] */ Int32 optID,
        /* [in] */ IInterface * pOptVal);

    CARAPI Accept(
        /* [in] */ ISocketImpl * pNewSocket);

    CARAPI Available(
        /* [out] */ Int32 * pNum);

    CARAPI Bind(
        /* [in] */ IInetAddress * pAddress,
        /* [in] */ Int32 port);

    CARAPI Close();

    CARAPI Connect(
        /* [in] */ const String& host,
        /* [in] */ Int32 port);

    CARAPI ConnectEx(
        /* [in] */ IInetAddress * pAddress,
        /* [in] */ Int32 port);

    CARAPI Create(
        /* [in] */ Boolean isStreaming);

    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor ** ppFileDescriptor);

    CARAPI GetInetAddress(
        /* [out] */ IInetAddress ** ppNetAddress);

    CARAPI GetInputStream(
        /* [out] */ IInputStream ** ppInputStream);

    CARAPI GetLocalPort(
        /* [out] */ Int32 * pNum);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream ** ppOutputStream);

    CARAPI GetPort(
        /* [out] */ Int32 * pPort);

    CARAPI Listen(
        /* [in] */ Int32 backlog);

    CARAPI ShutdownInput();

    CARAPI ShutdownOutput();

    CARAPI ConnectEx2(
        /* [in] */ ISocketAddress * pRemoteAddr,
        /* [in] */ Int32 timeout);

    CARAPI SupportsUrgentData(
        /* [out] */ Boolean * pIsSupports);

    CARAPI SendUrgentData(
        /* [in] */ Int32 value);

    CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IFileDescriptor * pFd);

    CARAPI constructor(
        /* [in] */ IProxy * pProxy);

    CARAPI constructor(
        /* [in] */ IFileDescriptor * pFd,
        /* [in] */ Int32 localport,
        /* [in] */ IInetAddress * pAddr,
        /* [in] */ Int32 port);

//    CARAPI Read(
//        /* [in] */ ArrayOf<Byte>* buffer,
//        /* [in] */ Int32 offset,
//        /* [in] */ Int32 count,
//        /* [out] */ Int32* value);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPLAINSOCKETIMPL_H__
