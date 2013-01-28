#ifdef _SQLPERMISSION_H_
#define _SQLPERMISSION_H_

#include <elastos.h>
#include "../Security/BasicPermission.h"

using namespace Elastos;

class SQLPermission : public BasicPermission{
{
public:
    SQLPermission();

    ~SQLPermission();
    /**
     * Creates a new {@code SQLPermission} object with the specified name.
     *
     * @param name
     *            the name to use for this {@code SQLPermission}.
     */
    CARAPI Init(
        /**[in]**/String name);

    /**
     * Creates a new {@code SQLPermission} object with the specified name.
     *
     * @param name
     *            is the name of the {@code SQLPermission}. Currently only
     *            {@code "setLog"} is allowed.
     * @param actions
     *            is currently unused and should be set to {@code null}.
     */
    CARAPI Init(
        /**[in]**/String name, 
        /**[in]**/String actions);
private:
    static const Int64 serialVersionUID;// = -1439323187199563495L;
};
#endif//_SQLPERMISSION_H_
