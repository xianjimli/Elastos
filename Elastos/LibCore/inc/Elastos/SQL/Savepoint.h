#ifdef _SAVEPOINT_H_
#define _SAVEPOINT_H_

#include <elastos.h>

using namespace Elastos;

class Savepoint {
public:
	/**
     * Returns the constructed ID for this savepoint.
     *
     * @return the ID for this savepoint.
     * @throws SQLException
     *             if an error occurrs accessing the database.
     */
    virtual CARAPI  getSavepointId(
        /** [out] **/Int32* id)=0;

    /**
     * Returns the name for this savepoint.
     *
     * @return the name of this savepoint.
     * @throws SQLException
     *             if an error occurrs accessing the database.
     */
    virtual CARAPI getSavepointName(
        /** [out] **/String* name)=0;
};
#endif//_SAVEPOINT_H_