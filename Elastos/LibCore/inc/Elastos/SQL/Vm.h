#ifndef _VM_H_
#define _VM_H_

#include <elastos.h>
#include <Elastos.SQL_server.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class Vm
{
    friend class Database;
private:
    /**
     * Internal handle for the compiled SQLite VM.
     */
    Int64 mHandle;// = 0;

protected:
    /**
     * Internal last error code for compile()/step() methods.
     */
    Int32 mError_code;// = 0;

public:
    Vm();
    
    /**
     * Perform one step on compiled SQLite VM.
     * The result row is passed to the given callback interface.<BR><BR>
     *
     * Example:<BR>
     * <PRE>
     *   ...
     *   try {
     *     Vm vm = db.compile("select * from x; select * from y;");
     *     while (vm.step(cb)) {
     *       ...
     *     }
     *     while (vm.compile()) {
     *       while (vm.step(cb)) {
     *         ...
     *       }
     *     }
     *   } catch (SQLite.Exception e) {
     *   }
     * </PRE>
     *
     * @param cb the object implementing the callback methods.
     * @return true as long as more row data can be retrieved,
     * false, otherwise.
     */

    CARAPI Step(
        /** [in] **/ICallback* cb,
        /** [out] **/Boolean* result);

    /**
     * Compile the next SQL statement for the SQLite VM instance.
     * @return true when SQL statement has been compiled, false
     * on end of statement sequence.
     */
    CARAPI Compile(
        /** [out] **/Boolean* result);

    /**
     * Abort the compiled SQLite VM.
     */
    CARAPI Stop();

protected:
    /**
     * Destructor for object.
     */
    CARAPI Finalize();

private:
    /**
     * Internal native initializer.
     */
    CARAPI Internal_init();
};
#endif//_VM_H_