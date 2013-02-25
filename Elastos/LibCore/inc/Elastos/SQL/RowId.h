#ifndef _ROWID_H_
#define _ROWID_H_

#include <elastos.h>

using namespace Elastos;

class RowId {
protected:
	virtual CARAPI Equals(
	    /** [in] **/IInterface* obj,
	    /** [out] **/ Boolean* result);

    /**
     * Returns an array of bytes representing the value of the corresponding SQL ROWID.
     */
    virtual CARAPI GetBytes(
        /** [out,collee] **/ArrayOf<Byte>* result);

//    String toString();

    virtual CARAPI HashCode(
        /** [out] **/Int32* code);
};
#endif//_ROWID_H_
