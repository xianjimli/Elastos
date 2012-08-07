
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Core {

class System
{
public:
    /**
     * Returns the current system time in milliseconds since January 1, 1970
     * 00:00:00 UTC. This method shouldn't be used for measuring timeouts or
     * other elapsed time measurements, as changing the system time can affect
     * the results.
     *
     * @return the local system time in milliseconds.
     */
    static CARAPI_(Int64) GetCurrentTimeMillis();

    /**
     * Returns the value of the environment variable with the given name {@code
     * var}.
     *
     * @param name
     *            the name of the environment variable.
     * @return the value of the specified environment variable or {@code null}
     *         if no variable exists with the given name.
     * @throws SecurityException
     *             if a {@link SecurityManager} is installed and its {@code
     *             checkPermission()} method does not allow the querying of
     *             single environment variables.
     */
    static CARAPI GetEnv(
        /* [in] */ CString name,
        /* [out] */ String* env);

private:
    /*
     * Returns an environment variable. No security checks are performed.
     * @param var the name of the environment variable
     * @return the value of the specified environment variable
     */
    static CARAPI_(String) GetEnvByName(
        /* [in] */ CString name);
};

} //Core
} //Elastos

#endif //__ELASTOS_SYSTEM_H__
