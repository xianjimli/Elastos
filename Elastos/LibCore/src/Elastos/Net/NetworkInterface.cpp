
#include "cmdef.h"
#include "NetworkInterface.h"
#include "CInterfaceAddress.h"
#include "CInet6Address.h"
#include <elastos/Map.h>

const Int32 NetworkInterface::CHECK_CONNECT_NO_PORT;

NetworkInterface::NetworkInterface(
    /* [in]  */ const String& name,
    /* [in]  */ const String& displayName,
    /* [in]  */ ArrayOf<AutoPtr<IInetAddress> >* addresses,
    /* [in]  */ Int32 interfaceIndex)
    : mInterfaceIndex(0)
{
    mName = name;
    mDisplayName = displayName;
    mInterfaceIndex = interfaceIndex;
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

//    ECode NetworkInterface::~NetworkInterface();

ECode NetworkInterface::GetAllInterfaceAddressesImpl(
    /* [out] */ IObjectContainer** addresses)
{
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
    /* [out] */ ArrayOf<NetworkInterface*>* interfaces)
{
    VALIDATE_NOT_NULL(interfaces);

    Map<String, NetworkInterface*> networkInterfaces;
    AutoPtr<IObjectContainer> addresses;
    GetAllInterfaceAddressesImpl((IObjectContainer**)&addresses);
    if (addresses != NULL) {
        AutoPtr<IObjectEnumerator> enumerator;
        addresses->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext;
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterfaceAddress> ia;
            enumerator->Current((IInterface**)&ia);
            if (ia != NULL) { // The array may contain harmless null elements.
                String name = ((CInterfaceAddress*)ia.Get())->mName;
                Map<String, NetworkInterface*>::Iterator it = networkInterfaces.Find(name);
                NetworkInterface* ni = NULL;
                if (it == networkInterfaces.End()) {
                    ArrayOf<AutoPtr<IInetAddress> >* addrArrayOf = ArrayOf<AutoPtr<IInetAddress> >::Alloc(1);
                    (*addrArrayOf)[0] = ((CInterfaceAddress*)ia.Get())->mAddress;
                    ni = new NetworkInterface(name, name, addrArrayOf, ((CInterfaceAddress*)ia.Get())->mIndex);
                    ni->mInterfaceAddresses.PushBack(ia);
                    networkInterfaces[name] = ni;
                    ArrayOf<AutoPtr<IInetAddress> >::Free(addrArrayOf);
                }
                else {
                    ni = it->mSecond;
                    assert(ni);
                    ni->mAddresses.PushBack(((CInterfaceAddress*)ia.Get())->mAddress);
                    ni->mInterfaceAddresses.PushBack(ia);
                }
            }
        }
    }

    Map<String, NetworkInterface*>::Iterator it;
    Int32 i =0;
    for (it = networkInterfaces.Begin(); it != networkInterfaces.End(); ++it) {
        (*interfaces)[i] = it->mSecond;
        i++;
    }

    return NOERROR;
}

Int32 NetworkInterface::GetIndex()
{
    return mInterfaceIndex;
}

ECode NetworkInterface::GetFirstAddress(
    /* [out] */ IInetAddress** firstAddr)
{
    VALIDATE_NOT_NULL(firstAddr);

    if (mAddresses.GetSize() >= 1) {
        *firstAddr = mAddresses[0];
        return NOERROR;
    }

    *firstAddr = NULL;
    return NOERROR;
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
//    SecurityManager sm = System.getSecurityManager();
//    if (sm == null || mAddresses.IsEmpty()) {
//        return Collections.enumeration(addresses);
//    }
    // TODO: Android should ditch SecurityManager and the associated pollution.
    VALIDATE_NOT_NULL(addresses);

    ASSERT_SUCCEEDED(CObjectContainer::New(addresses));
    List<AutoPtr<IInetAddress> >::Iterator it;
    for (it = mAddresses.Begin(); it != mAddresses.End(); ++it) {
        AutoPtr<IInetAddress> address = *it;
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
    /*
     * we should return the display name unless it is blank in this case
     * return the name so that something is displayed.
     */
    VALIDATE_NOT_NULL(name);

    *name = mDisplayName.IsEmpty() ? mName : mDisplayName;

    return NOERROR;
}

ECode NetworkInterface::GetByName(
    /* [in] */ const String& interfaceName,
    /* [out] */ INetworkInterface** networkInterface)
{
    if (interfaceName.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException();
    }
    List<NetworkInterface*> interfaceList;
    FAIL_RETURN(GetNetworkInterfacesList(&interfaceList));
    List<NetworkInterface*>::Iterator it;
    for (it = interfaceList.Begin(); it != interfaceList.End(); ++it) {
        if ((*it)->mName.Equals(interfaceName)) {
            *networkInterface = (INetworkInterface*)(*it);
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
    if (address == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("address == null");
    }

    List<NetworkInterface*> interfaceList;
    FAIL_RETURN(GetNetworkInterfacesList(&interfaceList));
    List<NetworkInterface*>::Iterator it;
    for (it = interfaceList.Begin(); it != interfaceList.End(); ++it) {
        List<AutoPtr<IInetAddress> >::Iterator addrIt;
        for (addrIt = (*it)->mAddresses.Begin(); addrIt != (*it)->mAddresses.End(); ++addrIt) {
            if ((*addrIt).Get() == address) {
                *networkInterface = (INetworkInterface*)(*it);
                return NOERROR;
            }
        }
    }

    *networkInterface = NULL;
    return NOERROR;
}

ECode NetworkInterface::GetNetworkInterfaces(
    /* [out] */ IObjectContainer** netWrokInterfaces)
{
    ASSERT_SUCCEEDED(CObjectContainer::New(netWrokInterfaces));

    List<NetworkInterface*> interfaceList;
    FAIL_RETURN(GetNetworkInterfacesList(&interfaceList));
    List<NetworkInterface*>::Iterator it;
    for (it = interfaceList.Begin(); it != interfaceList.End(); ++it) {
        (*netWrokInterfaces)->Add((INetworkInterface*)(*it));
    }
//    return Collections.enumeration(getNetworkInterfacesList());

    return NOERROR;
}

ECode NetworkInterface::GetNetworkInterfacesList(
    /* [out] */ List<NetworkInterface*>* interfaceList)
{
    ArrayOf<NetworkInterface*>* interfaces;
    FAIL_RETURN(GetNetworkInterfacesImpl(interfaces));

    if (interfaces != NULL) {
        for (Int32 i = 0; i < interfaces->GetLength(); ++i) {
            NetworkInterface* netif = (*interfaces)[i];
            // Ensure that current NetworkInterface is bound to at least
            // one InetAddress before processing
            List<AutoPtr<IInetAddress> >::Iterator it;
            //begin from this, whether the iobjcontainer should be modified?
            for (it = netif->mAddresses.Begin(); it != netif->mAddresses.End(); ++it) {
                AutoPtr<IInetAddress> addr = *it;
//                if (addr.ipaddress->GetLength() == 16) {//todo: how to Transform to inetaddress?
                    Boolean isLinkLocalAddress, isSiteLocalAddress;
                    addr->IsLinkLocalAddress(&isLinkLocalAddress);
                    addr->IsSiteLocalAddress(&isSiteLocalAddress);
                    if (isLinkLocalAddress || isSiteLocalAddress) {
                        ((CInet6Address*)addr.Get())->mScopedIf = (INetworkInterface*)netif;
                        ((CInet6Address*)addr.Get())->mIfname = netif->mName;
                        ((CInet6Address*)addr.Get())->mScopeIfnameSet = TRUE;
                    }
//                }
            }
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
        interfaceList->PushBack((*interfaces)[counter]);
        (*peeked)[counter] = TRUE;
    }

    ArrayOf<NetworkInterface*>::Free(interfaces);
    ArrayOf<Boolean>::Free(peeked);

    return NOERROR;
}

ECode NetworkInterface::GetInterfaceAddresses(
    /* [out] */ IObjectContainer** addresses)
{
    VALIDATE_NOT_NULL(addresses);
//    SecurityManager sm = System.getSecurityManager();
//    if (sm == NULL) {
        ASSERT_SUCCEEDED(CObjectContainer::New(addresses));
        List<AutoPtr<IInterfaceAddress> >::Iterator it;
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

    ASSERT_SUCCEEDED(CObjectContainer::New(subInterfaces));
    List<AutoPtr<INetworkInterface> >::Iterator it;
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

ECode NetworkInterface::IsUpImpl(
    /* [in] */ const String& n,
    /* [out] */ Boolean* isUP)
{
    return E_NOT_IMPLEMENTED;
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
    return E_NOT_IMPLEMENTED;
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
    return E_NOT_IMPLEMENTED;
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
    return E_NOT_IMPLEMENTED;
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
    return E_NOT_IMPLEMENTED;
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
    return E_NOT_IMPLEMENTED;
}

ECode NetworkInterface::IsVirtual(
    /* [out] */ Boolean* isvirtual)
{
    VALIDATE_NOT_NULL(isvirtual);

    *isvirtual = mParent != NULL;
    return NOERROR;
}
