
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "_CPlatform.h"
#include <elastos/AutoPtr.h>

/**
 * The Platform class gives access to the low-level underlying capabilities of
 * the operating system.
 *
 * The platform is structured into operations on the process heap memory,
 * network subsystem, and file system through different OS components.
 *
 * OS components are 'dangerous' in that they pass through the calls and
 * arguments to the OS with very little checking, and as such may cause fatal
 * exceptions in the runtime. Access to the OS components is restricted to
 * trusted code running on the system classpath.
 *
 * @see IFileSystem
 * @see INetworkSystem
 */
CarClass(CPlatform)
{
public:
    CPlatform();

    CARAPI GetFileSystem(
        /* [out] */ IFileSystem** fs);

    CARAPI GetNetworkSystem(
        /* [out] */ INetworkSystem** ns);

//    public static INetworkSystem getNetworkSystem() {
//        accessCheck();
//        // TODO: use BlockGuard here too, like in getFileSystem() above.
//        return NETWORK_SYSTEM;
//    }

private:
    /**
     * Checks to ensure that whoever is asking for the OS component is running
     * on the system classpath.
     */
    CARAPI AccessCheck();

private:
    // Note: for now, we're always wrapping the filesystem with
    // BlockGuard.  In the future we intend to measure this and, with
    // the arriving of upcoming method call Dalvik improvements,
    // remove a lot of the static caching in RandomAccessFile, etc.,
    // at which point we can make getFileSystem() return an unwrapped
    // filesystem in some cases so RandomAccessFiles created on
    // BlockGuard-policy-free threads have no extra overhead.  But for
    // now they do: ThreadLocal lookups will be done on most VFS
    // operations, which should be relatively less than the speed of
    // the flash.
    // TODO: measure & fix if needed.
    AutoPtr<IFileSystem> FILE_SYSTEM;

    AutoPtr<INetworkSystem> NETWORK_SYSTEM;
};

#endif //__PLATFORM_H__
