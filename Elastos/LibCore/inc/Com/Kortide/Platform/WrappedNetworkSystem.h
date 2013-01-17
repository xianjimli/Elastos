
#ifndef __WRAPPEDNETWORKSYSTEM_H__
#define __WRAPPEDNETWORKSYSTEM_H__


#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "Com.Kortide.Platform_server.h"
#include "Droid.System.h"

class WrappedNetworkSystem : public ElRefBase, public INetworkSystem
{
public:
    WrappedNetworkSystem(
        /* [in] */ INetworkSystem* networkSystem);

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Accept(
        /* [in] */ Int32 serverFd,
        /* [out, callee] */ ArrayOf<Byte>** ipAddress,
        /* [out] */ Int32* port,
        /* [out] */ Int32* localPort,
        /* [out] */ Int32* clientFd);

    CARAPI Bind(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& ipAddress,
        /* [in] */ Int32 port);

    CARAPI Read(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ ArrayOf<Byte>* data,
        /* [out] */ Int32* number);

    CARAPI ReadDirect(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 address,
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

    CARAPI Write(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

    CARAPI WriteDirect(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

    CARAPI ConnectNonBlocking(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& ipAddress,
        /* [in] */ Int32 port,
        /* [out] */ Boolean* succeeded);

    CARAPI IsConnected(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isConnected);

    CARAPI Send(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Int32 port,
        /* [in] */ ArrayOf<Byte>* ipAddress,
        /* [out] */ Int32* number);

    CARAPI SendDirect(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Int32 port,
        /* [in] */ ArrayOf<Byte>* ipAddress,
        /* [out] */ Int32* number);

    CARAPI Recv(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean peek,
        /* [in] */ Boolean connected,
        /* [out] */ ArrayOf<Byte>* data,
        /* [out, callee] */ ArrayOf<Byte>** ipAddress,
        /* [out] */ Int32* port,
        /* [out] */ Int32* number);

    CARAPI RecvDirect(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean peek,
        /* [in] */ Boolean connected,
        /* [out, callee] */ ArrayOf<Byte>** ipAddress,
        /* [out] */ Int32* port,
        /* [out] */ Int32* number);

    CARAPI DisconnectDatagram(
        /* [in] */ Int32 fd);

    CARAPI Socket(
        /* [in] */ Boolean stream,
        /* [out] */ Int32* fd);

    CARAPI ShutdownInput(
        /* [in] */ Int32 fd);

    CARAPI ShutdownOutput(
        /* [in] */ Int32 fd);

    CARAPI SendUrgentData(
        /* [in] */ Int32 fd,
        /* [in] */ Byte value);

    CARAPI Listen(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 backlog);

    CARAPI Connect(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& ipAddress,
        /* [in] */ Int32 port,
        /* [in] */ Int32 timeout);

    CARAPI GetSocketLocalAddress(
        /* [in] */ Int32 fd,
        /* [out, callee] */ ArrayOf<Byte>** ipAddress);

    CARAPI Select(
        /* [in] */ const ArrayOf<Int32>& readFDs,
        /* [in] */ const ArrayOf<Int32>& writeFDs,
        /* [in] */ Int32 numReadable,
        /* [in] */ Int32 numWritable,
        /* [in] */ Int64 timeout,
        /* [out] */ ArrayOf<Int32>* flags,
        /* [out] */ Boolean* succeeded);

    CARAPI GetSocketLocalPort(
        /* [in] */ Int32 fd,
        /* [out] */ Int32* localPort);

    CARAPI GetSocketOption(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 option,
        /* [out] */ Int32* optVal);

    CARAPI SetSocketOption(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 option,
        /* [in] */ Int32 optVal);

    CARAPI Close(
        /* [in] */ Int32 fd);

private:
    CARAPI_(Boolean) IsLingerSocket(
        /* [in] */ Int32 fd);

private:
    AutoPtr<INetworkSystem> mNetworkSystem;
    AutoPtr<IBlockGuard> mBlockGuard;
};

#endif //__WRAPPEDNETWORKSYSTEM_H__
