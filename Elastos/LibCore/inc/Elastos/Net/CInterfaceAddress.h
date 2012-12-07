
#ifndef __CINTERFACEADDRESS_H__
#define __CINTERFACEADDRESS_H__

#include "_CInterfaceAddress.h"
#include <elastos/AutoPtr.h>

CarClass(CInterfaceAddress)
{
public:
    CInterfaceAddress();

    CARAPI GetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetBroadcast(
        /* [out] */ IInetAddress** broadcast);

    CARAPI GetNetworkPrefixLength(
        /* [out] */ Int16* length);

    CARAPI constructor(
        /* [in] */ Int32 index,
        /* [in] */ const String& name,
        /* [in] */ IInetAddress* address,
        /* [in] */ IInetAddress* mask);

private:
    static CARAPI_(void) MakeBroadcastAddress(
        /* [in] */ IInetAddress* address,
        /* [in] */ IInetAddress* mask,
        /* [out] */ IInetAddress** broadcastAddress);

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

#endif //__CINTERFACEADDRESS_H__
