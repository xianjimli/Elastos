
#ifndef __HH_HANDLER_H
#define __HH_HANDLER_H

#include "ext/frameworkext.h"
#include "os/Message.h"
#include <elastos.h>

using namespace Elastos;

class Handler
{
public:
    /**
     * Returns a new {@link android.os.Message Message} from the global message pool. More efficient than
     * creating and allocating new instances. The retrieved message has its handler set to this instance (Message.target == this).
     *  If you don't want that facility, just call Message.obtain() instead.
     */
    Message* ObtainMessage();

    /**
     * Same as {@link #obtainMessage()}, except that it also sets the what member of the returned Message.
     *
     * @param what Value to assign to the returned Message.what field.
     * @return A Message from the global message pool.
     */
    Message* ObtainMessage(
        /* [in] */ Int32 what);

    /**
     *
     * Same as {@link #obtainMessage()}, except that it also sets the what and obj members
     * of the returned Message.
     *
     * @param what Value to assign to the returned Message.what field.
     * @param obj Value to assign to the returned Message.obj field.
     * @return A Message from the global message pool.
     */
    Message* ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ void* obj);

    /**
     *
     * Same as {@link #obtainMessage()}, except that it also sets the what, arg1 and arg2 members of the returned
     * Message.
     * @param what Value to assign to the returned Message.what field.
     * @param arg1 Value to assign to the returned Message.arg1 field.
     * @param arg2 Value to assign to the returned Message.arg2 field.
     * @return A Message from the global message pool.
     */
    Message* ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2);

    /**
     *
     * Same as {@link #obtainMessage()}, except that it also sets the what, obj, arg1,and arg2 values on the
     * returned Message.
     * @param what Value to assign to the returned Message.what field.
     * @param arg1 Value to assign to the returned Message.arg1 field.
     * @param arg2 Value to assign to the returned Message.arg2 field.
     * @param obj Value to assign to the returned Message.obj field.
     * @return A Message from the global message pool.
     */
    Message* ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ void* obj);
};

#endif // __HH_HANDLER_H

