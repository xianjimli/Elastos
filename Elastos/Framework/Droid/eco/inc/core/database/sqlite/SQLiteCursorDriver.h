#ifndef __SQLITECUSORDRIVER_H__
#define __SQLITECUSORDRIVER_H__

#include "ext/frameworkext.h"

class SQLiteCursorDriver
{
    /**
     * Executes the query returning a Cursor over the result set.
     * 
     * @param factory The CursorFactory to use when creating the Cursors, or
     *         null if standard SQLiteCursors should be returned.
     * @return a Cursor over the result set
     */
    CARAPI Query(
        /*[in]*/  ICursorFactory* factory, 
        /*[in]*/  ArrayOf<String>* bindArgs,
        /*[out]*/ ICursor** cs);

    /**
     * Called by a SQLiteCursor when it is released.
     */
    CARAPI CursorDeactivated();

    /**
     * Called by a SQLiteCursor when it is requeryed.
     * 
     * @return The new count value.
     */
    CARAPI CursorRequeried(
        /*[in]*/ICursor* cursor);

    /**
     * Called by a SQLiteCursor when it it closed to destroy this object as well.
     */
    CARAPI CursorClosed();

    /**
     * Set new bind arguments. These will take effect in cursorRequeried().
     * @param bindArgs the new arguments
     */
    CARAPI SetBindArguments(
    	/*[in]*/ ArrayOf<String>* bindArgs);
};


#endif // __SQLITECUSORDRIVER_H__