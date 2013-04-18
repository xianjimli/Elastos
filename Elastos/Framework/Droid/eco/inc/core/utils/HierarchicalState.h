#ifndef __HIERARCHICAL_STATE_H__
#define __HIERARCHICAL_STATE_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
//import android.os.Message;

/**
 * {@hide}
 *
 * The class for implementing states in a HierarchicalStateMachine
 */
class HierarchicalState : public ElRefBase
{
public:
    HierarchicalState()
    {}

    /**
     * @return name of state, but default returns the states
     * class name. An instance name would be better but requiring
     * it seems unnecessary.
     */
    CARAPI_(String) GetName();

protected:
    /**
     * Called when a state is entered.
     */
    CARAPI_(void) Enter()
    {}

    /**
     * Called when a message is to be processed by the
     * state machine.
     *
     * This routine is never reentered thus no synchronization
     * is needed as only one processMessage method will ever be
     * executing within a state machine at any given time. This
     * does mean that processing by this routine must be completed
     * as expeditiously as possible as no subsequent messages will
     * be processed until this routine returns.
     *
     * @param msg to process
     * @return true if processing has completed and false
     *         if the parent state's processMessage should
     *         be invoked.
     */
    CARAPI_(Boolean) ProcessMessage(
        /* [in] */ IMessage* msg)
    { return FALSE; }

    /**
     * Called when a state is exited.
     */
    CARAPI_(void) Exit()
    {}
};

#endif //__HIERARCHICAL_STATE_H__
