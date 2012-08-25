
#ifndef __INPUTHANDLER_H__
#define __INPUTHANDLER_H__

#include "ext/frameworkext.h"

/**
 * Handles input messages that arrive on an input channel.
 * @hide
 */
interface InputHandler
{
public:
    /**
     * Handle a key event.
     * It is the responsibility of the callee to ensure that the finished callback is
     * eventually invoked when the event processing is finished and the input system
     * can send the next event.
     * @param event The key event data.
     * @param finishedCallback The callback to invoke when event processing is finished.
     */
    virtual CARAPI HandleKey(
        /* [in] */ IKeyEvent* event,
        /* [in] */ IRunnable* finishedCallback) = 0;

    /**
     * Handle a motion event.
     * It is the responsibility of the callee to ensure that the finished callback is
     * eventually invoked when the event processing is finished and the input system
     * can send the next event.
     * @param event The motion event data.
     * @param finishedCallback The callback to invoke when event processing is finished.
     */
    virtual CARAPI HandleMotion(
        /* [in] */ IMotionEvent* event,
        /* [in] */ IRunnable* finishedCallback) = 0;
};

#endif //__INPUTHANDLER_H__
