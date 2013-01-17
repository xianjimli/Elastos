
#include "cmdef.h"
#include "NetworkInterface.h"
#include "InterfaceAddress.h"
#include "CInet6Address.h"
#include <elastos/Map.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>

// A smart pointer that closes the given fd on going out of scope.
// Use this when the fd is incidental to the purpose of your function,
// but needs to be cleaned up on exit.
class ScopedFd {
public:
    explicit ScopedFd(int fd) : fd(fd) {
    }

    ~ScopedFd() {
        close(fd);
    }

    int get() const {
        return fd;
    }

private:
    int fd;

    // Disallow copy and assignment.
    ScopedFd(const ScopedFd&);
    void operator=(const ScopedFd&);
};

const Int32 NetworkInterface::CHECK_CONNECT_NO_PORT;

ECode NetworkInterface::GetAllInterfaceAddressesImpl(
    /* [out, callee] */ ArrayOf< AutoPtr<IInterfaceAddress> >** addresses)
{
    VALIDATE_NOT_NULL(addresses);

    // Get the list of interface addresses.
    // ScopedInterfaceAddresses addresses;
    // if (!addresses.init()) {
    //     jniThrowSocketException(env, errno);
    //     return NULL;
    // }

    // // Count how many there are.
    // int interfaceAddressCount = 0;
    // for (ifaddrs* ifa = addresses.list; ifa != NULL; ifa = ifa->ifa_next) {
    //     ++interfaceAddressCount;
    // }

    // // Build the InterfaceAddress[]...
    // jobjectArray result =
    //         env->NewObjectArray(interfaceAddressCount, JniConstants::interfaceAddressClass, NULL);
    // if (result == NULL) {
    //     return NULL;
    // }

    // // And fill it in...
    // int arrayIndex = 0;
    // for (ifaddrs* ifa = addresses.list; ifa != NULL; ifa = ifa->ifa_next) {
    //     // We're only interested in IP addresses.
    //     int family = ifa->ifa_addr->sa_family;
    //     if (family != AF_INET && family != AF_INET6) {
    //         continue;
    //     }
    //     // Until we implement Java 6's NetworkInterface.isUp,
    //     // we only want interfaces that are up.
    //     if ((ifa->ifa_flags & IFF_UP) == 0) {
    //         continue;
    //     }
    //     // Find the interface's index, and skip this address if
    //     // the interface has gone away.
    //     int interfaceIndex = if_nametoindex(ifa->ifa_name);
    //     if (interfaceIndex == 0) {
    //         continue;
    //     }
    //     // Make a new InterfaceAddress, and insert it into the array.
    //     jobject element = makeInterfaceAddress(env, interfaceIndex, ifa);
    //     if (element == NULL) {
    //         return NULL;
    //     }
    //     env->SetObjectArrayElement(result, arrayIndex, element);
    //     if (env->ExceptionCheck()) {
    //         return NULL;
    //     }
    //     ++arrayIndex;
    // }
    // return result;

    return E_NOT_IMPLEMENTED;
}

ECode NetworkInterface::GetNetworkInterfacesImpl(
    /* [out, callee] */ ArrayOf< AutoPtr<NetworkInterface> >** interfaces)
{
    VALIDATE_NOT_NULL(interfaces);

    HashMap<String, NetworkInterface*> networkInterfaces(13);
    ArrayOf< AutoPtr<IInterfaceAddress> >* addresses;
    GetAllInterfaceAddressesImpl(&addresses);
    if (addresses != NULL) {
        for (Int32 i = 0; i < addresses->GetLength(); ++i) {
            IInterfaceAddress* ia = (*addresses)[i];
            if (ia != NULL) { // The array may contain harmless null elements.
                String name = ((InterfaceAddress*)ia)->mName;
                HashMap<String, NetworkInterface*>::Iterator it = networkInterfaces.Find(name);
                AutoPtr<NetworkInterface> ni;
                if (it == networkInterfaces.End()) {
                    ArrayOf_<IInetAddress*, 1> addrs;
                    addrs[0] = ((InterfaceAddress*)ia)->mAddress;
                    ni = new NetworkInterface(name, name, &addrs, ((InterfaceAddress*)ia)->mIndex);
                    ni->mInterfaceAddresses.PushBack(ia);
                    networkInterfaces[name] = ni;
                }
                else {
                    ni = it->mSecond;
                    assert(ni);
                    ni->mAddresses.PushBack(((InterfaceAddress*)ia)->mAddress);
                    ni->mInterfaceAddresses.PushBack(ia);
                }
            }
        }
    }

    *interfaces = ArrayOf< AutoPtr<NetworkInterface> >::Alloc(networkInterfaces.GetSize());
    HashMap<String, NetworkInterface*>::Iterator it;
    Int32 i = 0;
    for (it = networkInterfaces.Begin(); it != networkInterfaces.End(); ++it) {
        (**interfaces)[i] = it->mSecond;
        i++;
    }

    //free addresses
    if (addresses != NULL) {
        for (Int32 i = 0; i < addresses->GetLength(); ++i) {
            (*addresses)[i] = NULL;
        }
        ArrayOf< AutoPtr<IInterfaceAddress> >::Free(addresses);
    }

    return NOERROR;
}

NetworkInterface::NetworkInterface(
    /* [in]  */ const String& name,
    /* [in]  */ const String& displayName,
    /* [in]  */ ArrayOf<IInetAddress*>* addresses,
    /* [in]  */ Int32 interfaceIndex)
    : mName(name)
    , mDisplayName(displayName)
    , mInterfaceIndex(interfaceIndex)
{
    if (addresses != NULL) {
        for (Int32 i = 0; i < addresses->GetLength(); ++i) {
            mAddresses.PushBack((*addresses)[i]);
        }
    }
}

PInterface NetworkInterface::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_INetworkInterface) {
        return (INetworkInterface*)this;
    }

    return NULL;
}

UInt32 NetworkInterface::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NetworkInterface::Release()
{
    return ElRefBase::Release();
}

ECode NetworkInterface::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

NetworkInterface::~NetworkInterface()
{}

Int32 NetworkInterface::GetIndex()
{
    return mInterfaceIndex;
}

AutoPtr<IInetAddress> NetworkInterface::GetFirstAddress()
{
    if (mAddresses.GetSize() >= 1) {
        return mAddresses[0];
    }
    return NULL;
}

ECode NetworkInterface::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode NetworkInterface::GetInetAddresses(
    /* [out] */ IObjectContainer** addresses)
{
    VALIDATE_NOT_NULL(addresses);
//    SecurityManager sm = System.getSecurityManager();
//    if (sm == null || mAddresses.IsEmpty()) {
//        return Collections.enumeration(addresses);
//    }
    // TODO: Android should ditch SecurityManager and the associated pollution.

    FAIL_RETURN(CObjectContainer::New(addresses));
    List< AutoPtr<IInetAddress> >::Iterator it;
    for (it = mAddresses.Begin(); it != mAddresses.End(); ++it) {
        IInetAddress* address = *it;
//        try {
//        sm.checkConnect(address.getHostName(), CHECK_CONNECT_NO_PORT);
//        } catch (SecurityException e) {
//            continue;
//        }
        (*addresses)->Add(address);
    }
    return NOERROR;
}

ECode NetworkInterface::GetDisplayName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    /*
     * we should return the display name unless it is blank in this case
     * return the name so that something is displayed.
     */
    *name = mDisplayName.IsEmpty() ? mName : mDisplayName;

    return NOERROR;
}

ECode NetworkInterface::GetByName(
    /* [in] */ const String& interfaceName,
    /* [out] */ INetworkInterface** networkInterface)
{
    VALIDATE_NOT_NULL(networkInterface);

    if (interfaceName.IsNull()) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    List< AutoPtr<NetworkInterface> > interfaceList;
    FAIL_RETURN(GetNetworkInterfacesList(interfaceList));
    List< AutoPtr<NetworkInterface> >::Iterator it;
    for (it = interfaceList.Begin(); it != interfaceList.End(); ++it) {
        if ((*it)->mName.Equals(interfaceName)) {
            *networkInterface = (INetworkInterface*)(*it);
            if (*networkInterface != NULL) (*networkInterface)->AddRef();
            return NOERROR;
        }
    }
    *networkInterface = NULL;
    return NOERROR;
}

ECode NetworkInterface::GetByInetAddress(
    /* [in] */ IInetAddress* address,
    /* [out] */ INetworkInterface** networkInterface)
{
    VALIDATE_NOT_NULL(networkInterface);

    if (address == NULL) {
//        throw new NullPointerException("address == null");
        return E_NULL_POINTER_EXCEPTION;
    }

    List< AutoPtr<NetworkInterface> > interfaceList;
    FAIL_RETURN(GetNetworkInterfacesList(interfaceList));
    List< AutoPtr<NetworkInterface> >::Iterator it;
    for (it = interfaceList.Begin(); it != interfaceList.End(); ++it) {
        if (Find((*it)->mAddresses.Begin(), (*it)->mAddresses.End(), AutoPtr<IInetAddress>(address))
                != (*it)->mAddresses.End()) {
            *networkInterface = (INetworkInterface*)(*it);
            if (*networkInterface != NULL) (*networkInterface)->AddRef();
            return NOERROR;
        }
    }
    *networkInterface = NULL;
    return NOERROR;
}

ECode NetworkInterface::GetNetworkInterfaces(
    /* [out] */ IObjectContainer** networkInterfaces)
{
    VALIDATE_NOT_NULL(networkInterfaces);

//    return Collections.enumeration(getNetworkInterfacesList());
    FAIL_RETURN(CObjectContainer::New(networkInterfaces));

    List< AutoPtr<NetworkInterface> > interfaceList;
    FAIL_RETURN(GetNetworkInterfacesList(interfaceList));
    List< AutoPtr<NetworkInterface> >::Iterator it;
    for (it = interfaceList.Begin(); it != interfaceList.End(); ++it) {
        (*networkInterfaces)->Add((INetworkInterface*)(*it));
    }

    return NOERROR;
}

ECode NetworkInterface::GetNetworkInterfacesList(
    /* [out] */ List< AutoPtr<NetworkInterface> >& interfaceList)
{
    ArrayOf< AutoPtr<NetworkInterface> >* interfaces;
    FAIL_RETURN(GetNetworkInterfacesImpl(&interfaces));

    assert(interfaces != NULL);
    for (Int32 i = 0; i < interfaces->GetLength(); ++i) {
        NetworkInterface* netif = (*interfaces)[i];
        // Ensure that current NetworkInterface is bound to at least
        // one InetAddress before processing
        List< AutoPtr<IInetAddress> >::Iterator it;
        //begin from this, whether the iobjcontainer should be modified?
        for (it = netif->mAddresses.Begin(); it != netif->mAddresses.End(); ++it) {
            IInetAddress* addr = *it;
            assert(0);
            //todo:
            // if (addr.ipaddress->GetLength() == 16) {
                Boolean isLinkLocalAddress, isSiteLocalAddress;
                if ((addr->IsLinkLocalAddress(&isLinkLocalAddress), isLinkLocalAddress) ||
                    (addr->IsSiteLocalAddress(&isSiteLocalAddress), isSiteLocalAddress)) {
                    ((CInet6Address*)addr)->mScopedIf = (INetworkInterface*)netif;
                    ((CInet6Address*)addr)->mIfname = netif->mName;
                    ((CInet6Address*)addr)->mScopeIfnameSet = TRUE;
                }
            // }
        }
    }

    ArrayOf<Boolean>* peeked = ArrayOf<Boolean>::Alloc(interfaces->GetLength());
    for (Int32 counter = 0; counter < interfaces->GetLength(); counter++) {
        // If this interface has been touched, continue.
        if ((*peeked)[counter]) {
            continue;
        }
        Int32 counter2 = counter;
        // Checks whether the following interfaces are children.
        for (; counter2 < interfaces->GetLength(); counter2++) {
            if ((*peeked)[counter2]) {
                continue;
            }
            if ((*interfaces)[counter2]->mName.StartWith((*interfaces)[counter]->mName + ":")) {
                // Tagged as peeked
                (*peeked)[counter2] = TRUE;
                (*interfaces)[counter]->mChildren.PushBack((INetworkInterface*)(*interfaces)[counter2]);
                (*interfaces)[counter2]->mParent = (INetworkInterface*)(*interfaces)[counter];
                (*interfaces)[counter]->mAddresses = (*interfaces)[counter2]->mAddresses;
            }
        }
        // Tagged as peeked
        interfaceList.PushBack((*interfaces)[counter]);
        (*peeked)[counter] = TRUE;
    }

    //free interfaces;
    for (Int32 i = 0; i < interfaces->GetLength(); ++i) {
        (*interfaces)[i] = NULL;
    }
    ArrayOf< AutoPtr<NetworkInterface> >::Free(interfaces);
    ArrayOf<Boolean>::Free(peeked);

    return NOERROR;
}

ECode NetworkInterface::GetInterfaceAddresses(
    /* [out] */ IObjectContainer** addresses)
{
    VALIDATE_NOT_NULL(addresses);

//    SecurityManager sm = System.getSecurityManager();
//    if (sm == NULL) {
        FAIL_RETURN(CObjectContainer::New(addresses));
        List< AutoPtr<IInterfaceAddress> >::Iterator it;
        for (it = mInterfaceAddresses.Begin(); it != mInterfaceAddresses.End(); ++it) {
            (*addresses)->Add(*it);
        }

        return NOERROR;
//        return Collections.unmodifiableList(interfaceAddresses);
//    }
    // TODO: Android should ditch SecurityManager and the associated pollution.
    // List<InterfaceAddress> result = new ArrayList<InterfaceAddress>(interfaceAddresses.size());
    // for (InterfaceAddress ia : interfaceAddresses) {
    //     try {
    //         sm.checkConnect(ia.getAddress().getHostName(), CHECK_CONNECT_NO_PORT);
    //     } catch (SecurityException e) {
    //         continue;
    //     }
    //     result.add(ia);
    // }
    // return result;
}

ECode NetworkInterface::GetSubInterfaces(
    /* [out] */ IObjectContainer** subInterfaces)
{
    VALIDATE_NOT_NULL(subInterfaces);

    FAIL_RETURN(CObjectContainer::New(subInterfaces));
    List< AutoPtr<INetworkInterface> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        (*subInterfaces)->Add(*it);
    }

    return NOERROR;
//    return Collections.enumeration(children);
}

ECode NetworkInterface::GetParent(
    /* [out] */ INetworkInterface** parent)
{
    VALIDATE_NOT_NULL(parent);
    *parent = mParent;
    if (*parent != NULL) (*parent)->AddRef();

    return NOERROR;
}

ECode NetworkInterface::IsUp(
    /* [out] */ Boolean* isUp)
{
    VALIDATE_NOT_NULL(isUp);

    if (mAddresses.IsEmpty()) {
        *isUp = FALSE;
        return NOERROR;
    }

    return IsUpImpl(mName, isUp);
}

static ECode DoIoctl(const String& name, Int32 request, ifreq& ifr, Boolean* result)
{
    // Copy the name into the ifreq structure, if there's room...
    Int32 nameLength = name.GetCharCount();
    if (nameLength >= IFNAMSIZ) {
        errno = ENAMETOOLONG;
        *result = FALSE;
        return E_SOCKET_EXCEPTION;
    }
    memset(&ifr, 0, sizeof(ifr));
    memcpy(ifr.ifr_name, name.string(), name.GetLength());

    // ...and do the ioctl.
    ScopedFd fd(socket(AF_INET, SOCK_DGRAM, 0));
    if (fd.get() == -1) {
        *result = FALSE;
        return E_SOCKET_EXCEPTION;
    }
    Int32 rc = ioctl(fd.get(), request, &ifr);
    if (rc == -1) {
        *result = FALSE;
        return E_SOCKET_EXCEPTION;
    }
    *result = TRUE;
    return NOERROR;
}

static ECode HasFlag(const String& name, Int32 flag, Boolean* result)
{
    ifreq ifr;
    FAIL_RETURN(DoIoctl(name, SIOCGIFFLAGS, ifr, result)); // May throw.
    *result = (ifr.ifr_flags & flag) != 0;
    return NOERROR;
}

ECode NetworkInterface::IsUpImpl(
    /* [in] */ const String& n,
    /* [out] */ Boolean* isUp)
{
    return HasFlag(n, IFF_UP, isUp);
}

ECode NetworkInterface::IsLoopback(
    /* [out] */ Boolean* isLoopback)
{
    VALIDATE_NOT_NULL(isLoopback);

    if (mAddresses.IsEmpty()) {
        *isLoopback = FALSE;
        return NOERROR;

    }
    return IsLoopbackImpl(mName, isLoopback);
}

ECode NetworkInterface::IsLoopbackImpl(
    /* [in] */ const String& n,
    /* [out] */ Boolean* isLoopback)
{
    return HasFlag(n, IFF_LOOPBACK, isLoopback);
}

ECode NetworkInterface::IsPointToPoint(
    /* [out] */ Boolean* isPointToPoint)
{
    VALIDATE_NOT_NULL(isPointToPoint);

    if (mAddresses.IsEmpty()) {
        *isPointToPoint = FALSE;
        return NOERROR;

    }
    return IsPointToPointImpl(mName, isPointToPoint);
}

ECode NetworkInterface::IsPointToPointImpl(
    /* [in] */ const String& n,
    /* [out] */ Boolean* isPointToPoint)
{
    return HasFlag(n, IFF_POINTOPOINT, isPointToPoint); // Unix API typo!
}

ECode NetworkInterface::SupportsMulticast(
    /* [out] */ Boolean* multicast)
{
    VALIDATE_NOT_NULL(multicast);

    if (mAddresses.IsEmpty()) {
        *multicast = FALSE;
        return NOERROR;

    }
    return SupportsMulticastImpl(mName, multicast);
}

ECode NetworkInterface::SupportsMulticastImpl(
    /* [in] */ const String& n,
    /* [out] */ Boolean* multicast)
{
    return HasFlag(n, IFF_MULTICAST, multicast);
}

ECode NetworkInterface::GetHardwareAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    VALIDATE_NOT_NULL(address);

    if (mAddresses.IsEmpty()) {
        *address = ArrayOf<Byte>::Alloc(0);
        return NOERROR;

    }
    return GetHardwareAddressImpl(mName, address);
}

ECode NetworkInterface::GetHardwareAddressImpl(
    /* [in] */ const String& n,
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    ifreq ifr;
    Boolean result;
    FAIL_RETURN(DoIoctl(n, SIOCGIFHWADDR, ifr, &result))
    if (!result) {
        *address = NULL;
        return NOERROR;
    }
    Byte bytes[IFHWADDRLEN];
    Boolean isEmpty = TRUE;
    for (Int32 i = 0; i < IFHWADDRLEN; ++i) {
        bytes[i] = ifr.ifr_hwaddr.sa_data[i];
        if (bytes[i] != 0) {
            isEmpty = FALSE;
        }
    }
    if (isEmpty) {
        *address = NULL;
        return NOERROR;
    }
    *address = ArrayOf<Byte>::Alloc(IFHWADDRLEN);
    memcpy((*address)->GetPayload(), bytes, IFHWADDRLEN);
    return NOERROR;
}

ECode NetworkInterface::GetMTU(
    /* [out] */ Int32* mtu)
{
    VALIDATE_NOT_NULL(mtu);

    if (mAddresses.IsEmpty()) {
        *mtu = 0;
        return NOERROR;

    }
    return GetMTUImpl(mName, mtu);
}

ECode NetworkInterface::GetMTUImpl(
    /* [in] */ const String& n,
    /* [out] */ Int32* mtu)
{
    ifreq ifr;
    Boolean result;
    FAIL_RETURN(DoIoctl(n, SIOCGIFMTU, ifr, &result)); // May throw.
    *mtu = ifr.ifr_mtu;
    return NOERROR;
}

ECode NetworkInterface::IsVirtual(
    /* [out] */ Boolean* isvirtual)
{
    VALIDATE_NOT_NULL(isvirtual);

    *isvirtual = mParent != NULL;
    return NOERROR;
}
