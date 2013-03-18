#ifndef _FUNCTIONCONTEXT_H_
#define _FUNCTIONCONTEXT_H_

#include <elastos.h>
#include <Elastos.SQL_server.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class FunctionContext
{
private:
    /**
     * Internal handle for the native SQLite API.
     */
    Int32 mHandle;// = 0;

public:
    FunctionContext();
    
    /**
     * Set function result from string.
     *
     * @param r result string
     */
    CARAPI Set_result(
        /* [in] */ const String& r);

    /**
     * Set function result from integer.
     *
     * @param r result integer
     */
    CARAPI Set_resultEx(
        /* [in] */ Int32 r);

    /**
     * Set function result from double.
     *
     * @param r result double
     */
    CARAPI Set_resultEx2(
        /* [in] */ Double r);

    /**
     * Set function result from error message.
     *
     * @param r result string (error message)
     */
    CARAPI Set_error(
        /* [in] */ const String& r);

    /**
     * Set function result from byte array.
     * Only provided by SQLite3 databases.
     *
     * @param r result byte array
     */
    CARAPI Set_resultEx3(
        /* [in] */ const ArrayOf<Byte>& r);
    /**
     * Set function result as empty blob given size.
     * Only provided by SQLite3 databases.
     *
     * @param n size for empty blob
     */
    CARAPI Set_result_zeroblob(
        /* [in] */ Int32 n);

    /**
     * Retrieve number of rows for aggregate function.
     */
    CARAPI Count(
        /* [out] */ Int32* number);

private:
    /**
     * Internal native initializer.
     */
    Internal_init();
};

#endif// _FUNCTIONCONTEXT_H_