#ifndef __SQLITETRANSACTIONLISTENER_H__
#define __SQLITETRANSACTIONLISTENER_H__

#include "ext/frameworkext.h"

class SQLiteTransactionListener
{
    /**
     * Called immediately after the transaction begins.
     */
    CARAPI OnBegin();

    /**
     * Called immediately before commiting the transaction.
     */
    CARAPI OnCommit();

    /**
     * Called if the transaction is about to be rolled back.
     */
    CARAPI OnRollback();
};



#endif //__SQLITETRANSACTIONLISTENER_H__