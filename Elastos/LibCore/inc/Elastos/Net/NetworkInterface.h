
#ifndef __NETWORKINTERFACE_H__
#define __NETWORKINTERFACE_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/List.h>

using namespace Elastos;

/**
 * This class is used to represent a network interface of the local device. An
 * interface is defined by its address and a platform dependent name. The class
 * provides methods to get all information about the available interfaces of the
 * system or to identify the local interface of a joined multicast group.
 */
class NetworkInterface
    : public ElRefBase
    , public INetworkInterface
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
        /* [in]  */ ArrayOf<IInetAddress*>* addresses,
        /* [in]  */ Int32 interfaceIndex);

    ~NetworkInterface();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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
    CARAPI_(AutoPtr<IInetAddress>) GetFirstAddress();

    /**
     * Gets the name associated with this network interface.
     *
     * @return the name of this {@code NetworkInterface} instance.
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Gets a list of addresses bound to this network interface.
     *
     * @return the address list of the represented network interface.
     */
    CARAPI GetInetAddresses(
        /* [out] */ IObjectContainer** addresses);

    /**
     * Gets the human-readable name associated with this network interface.
     *
     * @return the display name of this network interface or the name if the
     *         display name is not available.
     */
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

    /**
     * Returns a List the InterfaceAddresses for this network interface.
     * <p>
     * If there is a security manager, its checkConnect method is called with
     * the InetAddress for each InterfaceAddress. Only InterfaceAddresses where
     * the checkConnect doesn't throw a SecurityException will be returned.
     *
     * @return a List of the InterfaceAddresses for this network interface.
     * @since 1.6
     */
    CARAPI GetInterfaceAddresses(
        /* [out] */ IObjectContainer** addresses);

    /**
     * Returns an {@code Enumeration} of all the sub-interfaces of this network interface.
     * Sub-interfaces are also known as virtual interfaces.
     * <p>
     * For example, {@code eth0:1} would be a sub-interface of {@code eth0}.
     *
     * @return an Enumeration of all the sub-interfaces of this network interface
     * @since 1.6
     */
    CARAPI GetSubInterfaces(
        /* [out] */ IObjectContainer** subInterfaces);

    /**
     * Returns the parent NetworkInterface of this interface if this is a
     * sub-interface, or null if it's a physical (non virtual) interface.
     *
     * @return the NetworkInterface this interface is attached to.
     * @since 1.6
     */
    CARAPI GetParent(
        /* [out] */ INetworkInterface** parent);

    /**
     * Returns true if this network interface is up.
     *
     * @return true if the interface is up.
     * @throws SocketException if an I/O error occurs.
     * @since 1.6
     */
    CARAPI IsUp(
        /* [out] */ Boolean* isUp);

    /**
     * Returns true if this network interface is a loopback interface.
     *
     * @return true if the interface is a loopback interface.
     * @throws SocketException if an I/O error occurs.
     * @since 1.6
     */
    CARAPI IsLoopback(
        /* [out] */ Boolean* isLoopback);

    /**
     * Returns true if this network interface is a point-to-point interface.
     * (For example, a PPP connection using a modem.)
     *
     * @return true if the interface is point-to-point.
     * @throws SocketException if an I/O error occurs.
     * @since 1.6
     */
    CARAPI IsPointToPoint(
        /* [out] */ Boolean* isPointToPoint);

    /**
     * Returns true if this network interface supports multicast.
     *
     * @throws SocketException if an I/O error occurs.
     * @since 1.6
     */
    CARAPI SupportsMulticast(
        /* [out] */ Boolean* multicast);

    /**
     * Returns the hardware address of the interface, if it has one, and the
     * user has the necessary privileges to access the address.
     *
     * @return a byte array containing the address or null if the address
     *         doesn't exist or is not accessible.
     * @throws SocketException if an I/O error occurs.
     * @since 1.6
     */
    CARAPI GetHardwareAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

    /**
     * Returns the Maximum Transmission Unit (MTU) of this interface.
     *
     * @return the value of the MTU for the interface.
     * @throws SocketException if an I/O error occurs.
     * @since 1.6
     */
    CARAPI GetMTU(
        /* [out] */ Int32* mtu);

    /**
     * Returns true if this interface is a virtual interface (also called
     * a sub-interface). Virtual interfaces are, on some systems, interfaces
     * created as a child of a physical interface and given different settings
     * (like address or MTU). Usually the name of the interface will the name of
     * the parent followed by a colon (:) and a number identifying the child,
     * since there can be several virtual interfaces attached to a single
     * physical interface.
     *
     * @return true if this interface is a virtual interface.
     * @since 1.6
     */
    CARAPI IsVirtual(
        /* [out] */ Boolean* isvirtual);

private:
    // BEGIN android-changed: we pay this extra complexity on the Java side
    // in return for vastly simpler native code.
    static CARAPI GetAllInterfaceAddressesImpl(
        /* [out, callee] */ ArrayOf< AutoPtr<IInterfaceAddress> >** addresses);

    static CARAPI GetNetworkInterfacesImpl(
        /* [out, callee] */ ArrayOf< AutoPtr<NetworkInterface> >** interfaces);

    static CARAPI GetNetworkInterfacesList(
        /* [out] */ List< AutoPtr<NetworkInterface> >& interfaceList);

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

public:
    static const Int32 NO_INTERFACE_INDEX = 0;
    static const Int32 UNSET_INTERFACE_INDEX = -1;

private:
    static const Int32 CHECK_CONNECT_NO_PORT = -1;

    String mName;
    String mDisplayName;
    List< AutoPtr<IInterfaceAddress> > mInterfaceAddresses;

    List< AutoPtr<IInetAddress> > mAddresses;

    // The interface index is a positive integer which is non-negative. Where
    // value is zero then we do not have an index for the interface (which
    // occurs in systems which only support IPV4)
    Int32 mInterfaceIndex;

    AutoPtr<INetworkInterface> mParent;

    List< AutoPtr<INetworkInterface> > mChildren;
};

#endif //__NETWORKINTERFACE_H__
