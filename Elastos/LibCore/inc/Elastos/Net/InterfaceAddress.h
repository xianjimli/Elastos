
#ifndef __INTERFACEADDRESS_H__
#define __INTERFACEADDRESS_H__

#include "Elastos.Net_server.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * Identifies one of a network interface's addresses.
 * These are passed back from the JNI behind NetworkInterface.getNetworkInterfaces.
 * Multiple addresses for the same interface are collected together on the Java side.
 *
 * @since 1.6
 */
class InterfaceAddress
    : public ElRefBase
    , public IInterfaceAddress
{
public:
    InterfaceAddress(
        /* [in] */ Int32 index,
        /* [in] */ const String& name,
        /* [in] */ IInetAddress* address,
        /* [in] */ IInetAddress* mask);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetBroadcast(
        /* [out] */ IInetAddress** broadcast);

    CARAPI GetNetworkPrefixLength(
        /* [out] */ Int16* length);

private:
    static CARAPI_(AutoPtr<IInetAddress>) MakeBroadcastAddress(
        /* [in] */ IInetAddress* address,
        /* [in] */ IInetAddress* mask);

    static CARAPI_(Int16) CountPrefixLength(
        /* [in] */ IInetAddress* mask);

public:
    /**
     * The kernel's interface index for the network interface this address
     * is currently assigned to. Values start at 1, because 0 means "unknown"
     * or "any", depending on context.
     */
    Int32 mIndex;

    /**
     * The network interface's name. "lo" or "eth0", for example.
     */
    String mName;

    /**
     * An IPv4 or IPv6 address.
     */
    AutoPtr<IInetAddress> mAddress;

private:
    /**
     * The IPv4 broadcast address, or null for IPv6.
     */
    AutoPtr<IInetAddress> mBroadcastAddress;

    Int16 mPrefixLength;
};

#endif //__INTERFACEADDRESS_H__
