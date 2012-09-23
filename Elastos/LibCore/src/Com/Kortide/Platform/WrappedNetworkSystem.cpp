
#include "cmdef.h"
#include "WrappedNetworkSystem.h"

#define SOCKOPT_SO_LINGER 128

WrappedNetworkSystem::WrappedNetworkSystem(
    /* [in] */ INetworkSystem* networkSystem)
{
    mNetworkSystem = networkSystem;
    CBlockGuard::AcquireSingleton((IBlockGuard**)&mBlockGuard);
}

PInterface WrappedNetworkSystem::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(INetworkSystem*)this;
    }
    else if (riid == EIID_INetworkSystem) {
        return (INetworkSystem*)this;
    }
    return NULL;
}

UInt32 WrappedNetworkSystem::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WrappedNetworkSystem::Release()
{
    return ElRefBase::Release();
}

ECode WrappedNetworkSystem::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(INetworkSystem*)this) {
        *pIID = EIID_INetworkSystem;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode WrappedNetworkSystem::Accept(
    /* [in] */ Int32 serverFd,
    /* [out] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* port,
    /* [out] */ Int32* localPort,
    /* [out] */ Int32* clientFd)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->Accept(serverFd, ipAddress, port, localPort, clientFd);
}

ECode WrappedNetworkSystem::Bind(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [in] */ Int32 port)
{
    return mNetworkSystem->Bind(fd, ipAddress, port);
}

ECode WrappedNetworkSystem::Read(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Byte>* data,
    /* [out] */ Int32* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->Read(fd, offset, count, data, number);
}

ECode WrappedNetworkSystem::ReadDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->ReadDirect(fd, address, count, number);
}

ECode WrappedNetworkSystem::Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->Write(fd, data, offset, count, number);
}

ECode WrappedNetworkSystem::WriteDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->WriteDirect(fd, address, offset, count, number);
}

ECode WrappedNetworkSystem::ConnectNonBlocking(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [in] */ Int32 port,
    /* [out] */ Boolean* succeeded)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->ConnectNonBlocking(fd, ipAddress, port, succeeded);
}

ECode WrappedNetworkSystem::IsConnected(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isConnected)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->IsConnected(fd, timeout, isConnected);
}

ECode WrappedNetworkSystem::Send(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Int32 port,
    /* [in] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* number)
{
    // Note: no BlockGuard violation.  We permit datagrams
    // without hostname lookups.  (short, bounded amount of time)
    return mNetworkSystem->Send(fd, data, offset, length, port, ipAddress, number);
}

ECode WrappedNetworkSystem::SendDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Int32 port,
    /* [in] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* number)
{
    // Note: no BlockGuard violation.  We permit datagrams
    // without hostname lookups.  (short, bounded amount of time)
    return mNetworkSystem->SendDirect(fd, address, offset, length, port, ipAddress, number);
}

ECode WrappedNetworkSystem::Recv(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean peek,
    /* [in] */ Boolean connected,
    /* [out] */ ArrayOf<Byte>* data,
    /* [out] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* port,
    /* [out] */ Int32* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->Recv(fd, offset, length, peek, connected, data, ipAddress, port, number);
}

ECode WrappedNetworkSystem::RecvDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean peek,
    /* [in] */ Boolean connected,
    /* [out] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* port,
    /* [out] */ Int32* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->RecvDirect(fd, address, offset, length, peek, connected, ipAddress, port, number);
}

ECode WrappedNetworkSystem::DisconnectDatagram(
    /* [in] */ Int32 fd)
{
    return mNetworkSystem->DisconnectDatagram(fd);
}

ECode WrappedNetworkSystem::Socket(
    /* [in] */ Boolean stream,
    /* [out] */ Int32* fd)
{
    return mNetworkSystem->Socket(stream, fd);
}

ECode WrappedNetworkSystem::ShutdownInput(
    /* [in] */ Int32 fd)
{
    return mNetworkSystem->ShutdownInput(fd);
}

ECode WrappedNetworkSystem::ShutdownOutput(
    /* [in] */ Int32 fd)
{
    return mNetworkSystem->ShutdownOutput(fd);
}

ECode WrappedNetworkSystem::SendUrgentData(
    /* [in] */ Int32 fd,
    /* [in] */ Byte value)
{
    return mNetworkSystem->SendUrgentData(fd, value);
}

ECode WrappedNetworkSystem::Listen(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 backlog)
{
    return mNetworkSystem->Listen(fd, backlog);
}

ECode WrappedNetworkSystem::Connect(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [in] */ Int32 port,
    /* [in] */ Int32 timeout)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->Connect(fd, ipAddress, port, timeout);
}

ECode WrappedNetworkSystem::GetSocketLocalAddress(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* ipAddress)
{
    return mNetworkSystem->GetSocketLocalAddress(fd, ipAddress);
}

ECode WrappedNetworkSystem::Select(
    /* [in] */ const ArrayOf<Int32>& readFDs,
    /* [in] */ const ArrayOf<Int32>& writeFDs,
    /* [in] */ Int32 numReadable,
    /* [in] */ Int32 numWritable,
    /* [in] */ Int64 timeout,
    /* [out] */ ArrayOf<Int32>* flags,
    /* [out] */ Boolean* succeeded)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    return mNetworkSystem->Select(readFDs, writeFDs, numReadable, numWritable, timeout, flags, succeeded);
}

ECode WrappedNetworkSystem::GetSocketLocalPort(
    /* [in] */ Int32 fd,
    /* [out] */ Int32* localPort)
{
    return mNetworkSystem->GetSocketLocalPort(fd, localPort);
}

ECode WrappedNetworkSystem::GetSocketOption(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [out] */ Int32* optVal)
{
    return mNetworkSystem->GetSocketOption(fd, option, optVal);
}

ECode WrappedNetworkSystem::SetSocketOption(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [in] */ Int32 optVal)
{
    return mNetworkSystem->SetSocketOption(fd, option, optVal);
}

ECode WrappedNetworkSystem::Close(
    /* [in] */ Int32 fd)
{
    // We exclude sockets without SO_LINGER so that apps can close their network connections
    // in methods like onDestroy, which will run on the UI thread, without jumping through
    // extra hoops.
    if (IsLingerSocket(fd)) {
        AutoPtr<IPolicy> policy;
        mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
        FAIL_RETURN(policy->OnNetwork());
    }
    return mNetworkSystem->Close(fd);
}

Boolean WrappedNetworkSystem::IsLingerSocket(
    /* [in] */ Int32 fd)
{
    Int32 optVal;
    ECode ec = mNetworkSystem->GetSocketOption(fd, SOCKOPT_SO_LINGER, &optVal);
    // We're called via Socket.close (which doesn't ask for us to be called), so we
    // must not throw here, because Socket.close must not throw if asked to close an
    // already-closed socket.
    if (FAILED(ec)) return FALSE;
    if (optVal == -1) {
        return FALSE;
    }
    return optVal != 0;
}
