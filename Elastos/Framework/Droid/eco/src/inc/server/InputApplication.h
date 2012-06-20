
#ifndef __INPUTAPPLICATION_H__
#define __INPUTAPPLICATION_H__

/**
 * Describes input-related application properties for use by the input dispatcher.
 *
 * @hide
 */
class InputApplication
{
public:
    // Application name.
    String mName;

    // Dispatching timeout.
    Int64 mDispatchingTimeoutNanos;

    // The application window token.
    void* mToken;
};

#endif //__INPUTAPPLICATION_H__
