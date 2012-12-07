
#ifndef __NETWORKINTERFACE_H__
#define __NETWORKINTERFACE_H__

#include "Elastos.Net_server.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/List.h>

using namespace Elastos;

class NetworkInterface : public ElRefBase, INetworkInterface
{
public:
    /**
     * This constructor is used by the native method in order to construct the
     * NetworkInterface objects in the array that it returns.
     *
     * @param name
     *            internal name associated with the interface.
     * @param displayName
     *            a user interpretable name for the interface.
     * @param addresses
     *            the Internet addresses associated with the interface.
     * @param interfaceIndex
     *            an index for the interface. Only set for platforms that
     *            support IPV6.
     */
    NetworkInterface(
        /* [in]  */ const String& name,
        /* [in]  */ const String& displayName,
        /* [in]  */ ArrayOf<AutoPtr<IInetAddress> >* addresses,
        /* [in]  */ Int32 interfaceIndex);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

//    CARAPI ~NetworkInterface();

    /**
     * Returns the index for the network interface. Unless the system supports
     * IPV6 this will be 0.
     *
     * @return the index
     */
    CARAPI_(Int32) GetIndex();

    /**
     * Returns the first address for the network interface. This is used in the
     * natives when we need one of the addresses for the interface and any one
     * will do
     *
     * @return the first address if one exists, otherwise null.
     */
    CARAPI GetFirstAddress(
        /* [out] */ IInetAddress** firstAddr);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetInetAddresses(
        /* [out] */ IObjectContainer** addresses);

    CARAPI GetDisplayName(
        /* [out] */ String* name);

    /**
     * Gets the specific network interface according to a given name.
     *
     * @param interfaceName
     *            the name to identify the searched network interface.
     * @return the network interface with the specified name if one exists or
     *         {@code null} otherwise.
     * @throws SocketException
     *             if an error occurs while getting the network interface
     *             information.
     * @throws NullPointerException
     *             if the given interface's name is {@code null}.
     */
    static CARAPI  GetByName(
        /* [in] */ const String& interfaceName,
        /* [out] */ INetworkInterface** networkInterface);

    /**
     * Gets the specific network interface according to the given address.
     *
     * @param address
     *            the address to identify the searched network interface.
     * @return the network interface with the specified address if one exists or
     *         {@code null} otherwise.
     * @throws SocketException
     *             if an error occurs while getting the network interface
     *             information.
     * @throws NullPointerException
     *             if the given interface address is invalid.
     */
    static CARAPI GetByInetAddress(
        /* [in] */ IInetAddress* address,
        /* [out] */ INetworkInterface** networkInterface);

    /**
     * Gets a list of all network interfaces available on the local system or
     * {@code null} if no interface is available.
     *
     * @return the list of {@code NetworkInterface} instances representing the
     *         available interfaces.
     * @throws SocketException
     *             if an error occurs while getting the network interface
     *             information.
     */
    static CARAPI GetNetworkInterfaces(
        /* [out] */ IObjectContainer** netWrokInterfaces);

    CARAPI GetInterfaceAddresses(
        /* [out] */ IObjectContainer** addresses);

    CARAPI GetSubInterfaces(
        /* [out] */ IObjectContainer** subInterfaces);

    CARAPI GetParent(
        /* [out] */ INetworkInterface** parent);

    CARAPI IsUp(
        /* [out] */ Boolean* isUp);

    CARAPI IsLoopback(
        /* [out] */ Boolean* isLoopback);

    CARAPI IsPointToPoint(
        /* [out] */ Boolean* isPointToPoint);

    CARAPI SupportsMulticast(
        /* [out] */ Boolean* multicast);

    CARAPI GetHardwareAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

    CARAPI GetMTU(
        /* [out] */ Int32* mtu);

    CARAPI IsVirtual(
        /* [out] */ Boolean* isvirtual);

private:
    // BEGIN android-changed: we pay this extra complexity on the Java side
    // in return for vastly simpler native code.
    static CARAPI GetAllInterfaceAddressesImpl(
        /* [out] */ IObjectContainer** addresses);

    static CARAPI GetNetworkInterfacesImpl(
        /* [out] */ ArrayOf<NetworkInterface*>* interfaces);

    static CARAPI GetNetworkInterfacesList(
        /* [out] */ List<NetworkInterface*>* interfaceList);

    static CARAPI IsUpImpl(
        /* [in] */ const String& n,
        /* [out] */ Boolean* isUP);

    static CARAPI IsLoopbackImpl(
        /* [in] */ const String& n,
        /* [out] */ Boolean* isLoopback);

    static CARAPI IsPointToPointImpl(
        /* [in] */ const String& n,
        /* [out] */ Boolean* isPointToPoint);

    static CARAPI SupportsMulticastImpl(
        /* [in] */ const String& n,
        /* [out] */ Boolean* isPointToPoint);

    static CARAPI GetHardwareAddressImpl(
        /* [in] */ const String& n,
        /* [out, callee] */ ArrayOf<Byte>** address);

    static CARAPI GetMTUImpl(
        /* [in] */ const String& n,
        /* [out] */ Int32* mtu);

private:
    static const Int32 CHECK_CONNECT_NO_PORT = -1;

    String mName;
    String mDisplayName;
    List<AutoPtr<IInterfaceAddress> > mInterfaceAddresses;

    List<AutoPtr<IInetAddress> > mAddresses;

    // The interface index is a positive integer which is non-negative. Where
    // value is zero then we do not have an index for the interface (which
    // occurs in systems which only support IPV4)
    Int32 mInterfaceIndex;

    AutoPtr<INetworkInterface> mParent;

    List<AutoPtr<INetworkInterface> > mChildren;
};

#endif //__NETWORKINTERFACE_H__
