#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <elastos.h>

using namespace Elastos;

class Wrapper {
protected:
    /**
     * Returns an object that implements the given interface. If the caller is
     * not a wrapper, a SQLException will be thrown.
     *
     * @param iface -
     *            the class that defines the interface
     * @return - an object that implements the interface
     * @throws SQLException -
     *             if there is no object implementing the specific interface
     */
    //<T> T unwrap(Class<T> iface) throws SQLException;
    virtual CARAPI Unwrap(
        /** [in] **/ InterfaceID iface,
        /** [out] **/ IInterface *obj) = 0;

    /**
     * If the caller is a wrapper of the class or implements the given
     * interface, the methods return false and vice versa.
     *
     * @param iface -
     *            the class that defines the interface
     * @return - true if the instance implements the interface
     * @throws SQLException -
     *             when an error occurs when judges the object
     */
    //boolean isWrapperFor(Class<?> iface) throws SQLException;
    virtual CARAPI IsWrapperFor(
        /** [in] **/ InterfaceID iface,
        /** [out] **/ Boolean *result) = 0;
};
#endif
